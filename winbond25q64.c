/**
 *	bobbycar - Modded Bobby Car toy for toddlers
 *	Copyright (C) 2020-2020 Johannes Bauer
 *
 *	This file is part of bobbycar.
 *
 *	bobbycar is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; this program is ONLY licensed under
 *	version 3 of the License, later versions are explicitly excluded.
 *
 *	bobbycar is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with bobbycar; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	Johannes Bauer <JohannesBauer@gmx.de>
**/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32f10x_spi.h>
#include <stm32f10x_dma.h>
#include "system.h"
#include "winbond25q64.h"
#include "init.h"

static uint8_t spiflash_txrx_byte(uint8_t send_byte) {
	/* Wait until transmit register empty, then send */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, send_byte);

	/* Wait until receive register full, then receive */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}

static void spiflash_txrx_raw(void *vdata, unsigned int length) {
	uint8_t *data = (uint8_t*)vdata;
	for (unsigned int i = 0; i < length; i++) {
		data[i] = spiflash_txrx_byte(data[i]);
	}
}

static void spiflash_txrx(void *vdata, unsigned int length) {
	w25qxx_cs_set_active();
	spiflash_txrx_raw(vdata, length);
	w25qxx_cs_set_inactive();
}

void DMA1_Channel2_Handler(void) {
	DMA_Channel_TypeDef *dma_channel_rx = DMA1_Channel2;
	if (DMA_GetITStatus(DMA1_IT_TC2)) {
		DMA_ClearITPendingBit(DMA1_IT_TC2);
		DMA_Cmd(dma_channel_rx, DISABLE);
		w25qxx_cs_set_inactive();
	}
}

void DMA1_Channel3_Handler(void) {
	DMA_Channel_TypeDef *dma_channel_tx = DMA1_Channel3;
	if (DMA_GetITStatus(DMA1_IT_TC3)) {
		DMA_ClearITPendingBit(DMA1_IT_TC3);
		DMA_Cmd(dma_channel_tx, DISABLE);
	}
}

static void spiflash_txrx_dma(void *vdata, unsigned int length) {
	first = true;

	w25qxx_cs_set_active();
	DMA_Channel_TypeDef *dma_channel_tx = DMA1_Channel3;
	DMA_Channel_TypeDef *dma_channel_rx = DMA1_Channel2;

	DMA_ClearFlag(DMA1_FLAG_TC3 | DMA1_FLAG_TE3 | DMA1_FLAG_HT3 | DMA1_FLAG_TC2 | DMA1_FLAG_TE2 | DMA1_FLAG_HT2);
	init_spi_dma(vdata, length);

	DMA_Cmd(dma_channel_tx, ENABLE);
	DMA_Cmd(dma_channel_rx, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx | SPI_I2S_DMAReq_Rx, ENABLE);
}

static bool dma_channel_active(DMA_Channel_TypeDef *dma_channel) {
	return (dma_channel->CCR) & 1;
}

static void spiflash_dma_wait(void) {
	DMA_Channel_TypeDef *dma_channel_tx = DMA1_Channel3;
	DMA_Channel_TypeDef *dma_channel_rx = DMA1_Channel2;
	while ((dma_channel_active(dma_channel_tx)) || (dma_channel_active(dma_channel_rx)));
}

struct spiflash_manufacturer_t spiflash_read_id(void) {
	uint8_t data[6] = { SPIFLASH_READ_MANUFACTURER };
	spiflash_txrx(data, sizeof(data));

	return (struct spiflash_manufacturer_t){
		.manufacturer_id = data[4],
		.device_id = data[5],
	};
}

struct spiflash_manufacturer_t spiflash_read_id_dma(void) {
	uint8_t data[6] = { SPIFLASH_READ_MANUFACTURER };
	spiflash_txrx_dma(data, 6);
	spiflash_dma_wait();

	return (struct spiflash_manufacturer_t){
		.manufacturer_id = data[4],
		.device_id = data[5],
	};
}

void spiflash_reset(void) {
	uint8_t data = SPIFLASH_ENABLE_RESET;
	spiflash_txrx(&data, 1);

	data = SPIFLASH_RESET;
	spiflash_txrx(&data, 1);
}

void spiflash_read(uint32_t start_address, void *buffer, unsigned int length) {
	uint8_t data[4] = { SPIFLASH_READ_DATA, (start_address >> 16) & 0xff, (start_address >> 8) & 0xff, (start_address >> 0) & 0xff };
	w25qxx_cs_set_active();
	spiflash_txrx_raw(data, sizeof(data));
	spiflash_txrx_raw(buffer, length);
	w25qxx_cs_set_inactive();
}

void spiflash_selfcheck(void) {
	struct spiflash_manufacturer_t id = spiflash_read_id();
	printf("SPI flash self-test: manufacturer ID 0x%x (%s), device ID 0x%x (%s)\n", id.manufacturer_id, (id.manufacturer_id == 0xef) ? "Winbond Serial Flash" : "?", id.device_id, (id.device_id == 0x16) ? "W25Q64FV" : "?");
	if ((id.manufacturer_id == 0xef) && (id.device_id == 0x16)) {
		const unsigned int capacity_mbit = 64;
		const unsigned int capacity_bytes = capacity_mbit * 1024 * 1024 / 8;
		const unsigned int chunk_size = 256;
		printf("Device capacity: %d bytes (%d MBit); reading in chunks of %d bytes each.\n", capacity_bytes, capacity_mbit, chunk_size);

		const unsigned int chunks_total = capacity_bytes / chunk_size;
		unsigned int empty_chunks = 0;
		for (unsigned int chunkno = 0; chunkno < chunks_total; chunkno++) {
			uint32_t chunk_data[chunk_size / 4];
			const unsigned int address = chunkno * chunk_size;
			spiflash_read(address, chunk_data, chunk_size);

			bool empty = true;
			for (unsigned int i = 0; i < chunk_size / 4; i++) {
				if (chunk_data[i] != 0xffffffff) {
					//printf("chunk not empty: %d / %d %lx\n", chunkno, i, chunk_data[i]);
					empty = false;
					break;
				}
			}
			if (empty) {
				empty_chunks++;
			} else {
				printf("CHUNK-> 0x%x  ", address);
				for (unsigned int i = 0; i < chunk_size; i++) {
					printf("%02x", ((uint8_t*)chunk_data)[i]);
				}
				printf("\n");
			}
		}
		printf("Finished reading whole memory. Empty chunks: %d of %d.\n", empty_chunks, chunks_total);
	}
}
