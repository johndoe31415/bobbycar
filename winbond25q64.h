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

#ifndef __WINBOND25Q64_H__
#define __WINBOND25Q64_H__

#include <stdint.h>

enum spiflash_insn_t {
	SPIFLASH_WRITE_ENABLE = 0x06,
	SPIFLASH_WRITE_ENABLE_VOLATILE_STATUS = 0x50,
	SPIFLASH_WRITE_DISABLE = 0x04,
	SPIFLASH_READ_STATUS1 = 0x05,
	SPIFLASH_READ_STATUS2 = 0x35,
	SPIFLASH_WRITE_STATUS = 0x01,
	SPIFLASH_READ_DATA = 0x03,
	SPIFLASH_FAST_READ = 0x0b,
	SPIFLASH_FAST_READ_DUAL_OUTPUT = 0x3b,
	SPIFLASH_FAST_READ_QUAD_OUTPUT = 0x6b,
	SPIFLASH_FAST_READ_DUAL_IO = 0xbb,
	SPIFLASH_FAST_READ_QUAD_IO = 0xeb,
	SPIFLASH_WORD_READ_QUAD_IO = 0xe7,
	SPIFLASH_OCTWORD_READ_QUAD_IO = 0xe3,
	SPIFLASH_SET_BURST_WITH_WRAP = 0x77,
	SPIFLASH_PAGE_PROGRAM = 0x02,
	SPIFLASH_QUAD_INPUT_PAGE_PROGRAM = 0x32,
	SPIFLASH_SECTOR_ERASE = 0x20,
	SPIFLASH_BLOCK_ERASE_32K = 0x52,
	SPIFLASH_BLOCK_ERASE_64K = 0xd8,
	SPIFLASH_CHIP_ERASE = 0xc7,		// TODO 0x60h?
	SPIFLASH_ERASE_PROGRAM_SUSPEND = 0x75,
	SPIFLASH_ERASE_PROGRAM_RESUME = 0x7a,
	SPIFLASH_POWER_DOWN = 0xb9,
	SPIFLASH_RSPIFLASH_ELEASE_POWER_DOWN = 0xab,
	SPIFLASH_READ_MANUFACTURER = 0x90,
	// Some missing
	SPIFLASH_READ_UNIQUE_ID = 0x4b,
	SPIFLASH_READ_JEDEC_ID = 0x9f,
	SPIFLASH_READ_SFDP_REGISTER = 0x5a,
	SPIFLASH_ENABLE_RESET = 0x66,
	SPIFLASH_RESET = 0x99,
};

struct spiflash_manufacturer_t {
	uint8_t manufacturer_id;
	uint8_t device_id;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void DMA1_Channel2_Handler(void);
void DMA1_Channel3_Handler(void);
struct spiflash_manufacturer_t spiflash_read_id(void);
struct spiflash_manufacturer_t spiflash_read_id_dma(void);
void spiflash_reset(void);
void spiflash_read(uint32_t start_address, void *buffer, unsigned int length);
void spiflash_selfcheck(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif