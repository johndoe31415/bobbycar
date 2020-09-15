/**
 *	defiant - Modded Bobby Car toy for toddlers
 *	Copyright (C) 2020-2020 Johannes Bauer
 *
 *	This file is part of defiant.
 *
 *	defiant is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; this program is ONLY licensed under
 *	version 3 of the License, later versions are explicitly excluded.
 *
 *	defiant is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with defiant; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	Johannes Bauer <JohannesBauer@gmx.de>
**/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_spi.h>
#include "usart.h"
#include "usart_terminal.h"
#include "winbond25q64.h"
#include "crc32.h"
#include "audio.h"
#include "audio_hl.h"
#include "stats.h"

#define CHAR_BACKSPACE				0x7f
#define TERMINAL_BUFFER_SIZE		384

enum protocol_t {
	ASCII = 0,
	BINARY = 1,
};

enum commandcodes_t {
	CMDCODE_IDENTIFY = 1,
	CMDCODE_READ_PAGE = 2,
	CMDCODE_ERASE_SECTOR = 3,
	CMDCODE_WRITE_PAGE = 4,
	CMDCODE_REBOOT = 5,
	CMDCODE_ERROR = 0xdeadbeef,
};

struct binary_command_t {
	uint32_t total_length;
	uint32_t crc;
	struct {
		uint32_t command_code;
		uint8_t data[];
	} payload __attribute__ ((packed));
} __attribute__ ((packed));

struct binary_payload_read_page_t {
	uint32_t page_no;
} __attribute__ ((packed));

struct binary_payload_write_page_t {
	uint32_t page_no;
	uint8_t page_data[SPIFLASH_PAGE_SIZE];
} __attribute__ ((packed));

struct binary_payload_erase_sector_t {
	uint32_t sector_no;
} __attribute__ ((packed));

static struct terminal_options_t {
	uint8_t input_buffer[TERMINAL_BUFFER_SIZE];
	unsigned int fill;
	uint32_t ticks;
	enum protocol_t protocol;
} terminal;

static void device_reset(void) {
	SCB->AIRCR = (0x5fa << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ;
}

static void create_prompt(void) {
	if (terminal.protocol == ASCII) {
		printf("Command: ");
		fflush(stdout);
	}
}

static void debug_dma_channel(const char *name, DMA_Channel_TypeDef *channel, unsigned int flags) {
	printf("  %s: ", name);
	printf("[");
	printf("%s", (flags & (1 << 3)) ? "E" : " ");
	printf("%s", (flags & (1 << 2)) ? "H" : " ");
	printf("%s", (flags & (1 << 1)) ? "C" : " ");
	printf("%s", (flags & (1 << 0)) ? "G" : " ");
	printf("]");
	printf("CCR %08lx ", channel->CCR);
	printf("%s ", (channel->CCR & 1) ? "enabled": "disabled");
	printf("%lu bytes remain", channel->CNDTR & 0xffff);
	printf("\n");
}

static void debug_dma(void) {
	printf("DMA debugging info:\n");
	printf("DMA1 Interrupt Status (ISR): %08lx\n", DMA1->ISR);
	debug_dma_channel("DMA1_Channel2 (SPI1 RX)", DMA1_Channel2, DMA1->ISR >> (4 * 1));
	debug_dma_channel("DMA1_Channel3 (SPI1 TX)", DMA1_Channel3, DMA1->ISR >> (4 * 2));
}

static void debug_spi(void) {
	SPI_TypeDef *spi = SPI1;
	unsigned int sr = spi->SR;
	printf("SPI1: %s %s %s %s %s",
			(sr & (1 << 7)) ? "BSY" : "   ",
			(sr & (1 << 6)) ? "OVR" : "   ",
			(sr & (1 << 5)) ? "MODF" :  "   ",
			(sr & (1 << 4)) ? "CRCERR" : "     ",
			(sr & (1 << 3)) ? "UDR" : "   ");
	printf("\n");
}

static void clear_command(void) {
	printf("\n");
	terminal.input_buffer[terminal.fill] = 0;
	if (!strcmp((char*)terminal.input_buffer, "?") || !strcmp((char*)terminal.input_buffer, "help")) {
		printf("Help page:\n");
		printf("? or help              This help page.\n");
		printf("stats                  Show some statistical data.\n");
		printf("dma                    DMA debugging.\n");
		printf("spi                    SPI debugging.\n");
		printf("flash-id               Identify the flash ROM.\n");
		printf("flash-read (offset)    Read bytes from the flash ROM.\n");
		printf("binary                 Switch to binary protocol.\n");
		printf("play (no)              Playback sample #n\n");
		printf("stop                   Stop audio playback\n");
		printf("eng-start              Start the engine\n");
		printf("eng-stop               Stop the engine\n");
		printf("turn-on                Activate the turn signal\n");
		printf("turn-off               Deactivate the turn signal\n");
		printf("siren-on               Activate the siren\n");
		printf("siren-off              Deactivate the siren\n");
		printf("reset                  Reset the device.\n");
	} else if (!strcmp((char*)terminal.input_buffer, "stats")) {
		printf("DMA requests total : %u\n", stats->dma_requests_total);
		printf("DMA requests failed: %u\n", stats->dma_requests_failed);
	} else if (!strcmp((char*)terminal.input_buffer, "dma")) {
		debug_dma();
	} else if (!strcmp((char*)terminal.input_buffer, "spi")) {
		debug_spi();
	} else if (!strcmp((char*)terminal.input_buffer, "flash-id")) {
		spiflash_identify();
	} else if (!strncmp((char*)terminal.input_buffer, "flash-read ", 11)) {
		const unsigned int offset = atoi((char*)terminal.input_buffer + 11);
		uint8_t buffer[64];
		spiflash_read(offset, buffer, sizeof(buffer));
		printf("Dumping %d bytes starting at offset 0x%x:\n", sizeof(buffer), offset);
		for (unsigned int i = 0; i < sizeof(buffer); i++) {
			printf("%02x", buffer[i]);
		}
		printf("\n");
	} else if (!strcmp((char*)terminal.input_buffer, "reset")) {
		device_reset();
	} else if (!strncmp((char*)terminal.input_buffer, "play ", 5)) {
		const unsigned int fileno = atoi((char*)terminal.input_buffer + 5);
		audio_playback_fileno(fileno, true);
	} else if (!strcmp((char*)terminal.input_buffer, "stop")) {
		audio_shutoff();
	} else if (!strcmp((char*)terminal.input_buffer, "eng-start")) {
		audio_hl_start_engine();
	} else if (!strcmp((char*)terminal.input_buffer, "eng-stop")) {
		audio_hl_stop_engine();
	} else if (!strcmp((char*)terminal.input_buffer, "turn-on")) {
		audio_hl_turn_signal_on();
	} else if (!strcmp((char*)terminal.input_buffer, "turn-off")) {
		audio_hl_turn_signal_off();
	} else if (!strcmp((char*)terminal.input_buffer, "siren-on")) {
		audio_hl_siren_on();
	} else if (!strcmp((char*)terminal.input_buffer, "siren-off")) {
		audio_hl_siren_off();
	} else if (!strcmp((char*)terminal.input_buffer, "binary")) {
		printf("Now switching to binary protocol.\n");
		terminal.fill = 0;
		terminal.protocol = BINARY;
		audio_shutoff();
	} else {
		printf("Unknown command: %s\n", (char*)terminal.input_buffer);
	}
	terminal.fill = 0;
	create_prompt();
}

static void binary_reply(enum commandcodes_t command_code, const void *payload, unsigned int payload_length) {
	unsigned int total_length = 12 + payload_length;
	uint8_t data[total_length];
	struct binary_command_t *cmd = (struct binary_command_t*)data;
	cmd->total_length = total_length;
	cmd->payload.command_code = command_code;
	memcpy(cmd->payload.data, payload, payload_length);

	/* We need to invert the CRC for the response because otherwise commands
	 * sent in ASCII mode that are echoed back to the host are valid responses. */
	cmd->crc = compute_crc32(&cmd->payload, cmd->total_length - 8) ^ 0xa5a5a5a5;
	for (unsigned int i = 0; i < total_length; i++) {
		usart_transmit_char(data[i]);
	}
}

static void execute_binary_command(struct binary_command_t *command) {
	unsigned int payload_size = command->total_length - 12;
	if (command->payload.command_code == CMDCODE_IDENTIFY) {
		/* Hello message */
		binary_reply(command->payload.command_code, NULL, 0);
	} else if ((command->payload.command_code == CMDCODE_READ_PAGE) && (payload_size == sizeof(struct binary_payload_read_page_t))) {
		const struct binary_payload_read_page_t *payload = (const struct binary_payload_read_page_t*)command->payload.data;
		uint8_t page_data[SPIFLASH_PAGE_SIZE];
		spiflash_read(SPIFLASH_PAGE_SIZE * payload->page_no, page_data, sizeof(page_data));
		binary_reply(command->payload.command_code, page_data, SPIFLASH_PAGE_SIZE);
	} else if ((command->payload.command_code == CMDCODE_WRITE_PAGE) && (payload_size == sizeof(struct binary_payload_write_page_t))) {
		const struct binary_payload_write_page_t *payload = (const struct binary_payload_write_page_t*)command->payload.data;
		spiflash_write_page(payload->page_no, payload->page_data);
		binary_reply(command->payload.command_code, NULL, 0);
	} else if ((command->payload.command_code == CMDCODE_ERASE_SECTOR) && (payload_size == sizeof(struct binary_payload_erase_sector_t))) {
		const struct binary_payload_erase_sector_t *payload = (const struct binary_payload_erase_sector_t*)command->payload.data;
		spiflash_erase_sector(payload->sector_no);
		binary_reply(command->payload.command_code, NULL, 0);
	} else if (command->payload.command_code == CMDCODE_REBOOT) {
		device_reset();
	} else {
		binary_reply(CMDCODE_ERROR, NULL, 0);
	}
}

static void usart_binary_terminal_rx(uint8_t character) {
	if (terminal.fill < TERMINAL_BUFFER_SIZE) {
		terminal.input_buffer[terminal.fill] = character;
		terminal.fill++;
	}

	struct binary_command_t *command = (struct binary_command_t*)terminal.input_buffer;
	if ((terminal.fill >= sizeof(struct binary_command_t)) && (terminal.fill >= command->total_length)) {
		if (command->crc == compute_crc32(&command->payload, command->total_length - 8)) {
			execute_binary_command(command);
		}
		terminal.fill = 0;
	}
}

static void usart_ascii_terminal_rx(char character) {
	if (character == '\n') {
	} else if (character == '\r') {
		clear_command();
	} else if (character == CHAR_BACKSPACE) {
		if (terminal.fill > 0) {
			terminal.fill--;
			printf("\x08 \x08");
			fflush(stdout);
		}
	} else {
		if (terminal.fill < TERMINAL_BUFFER_SIZE - 1) {
			terminal.input_buffer[terminal.fill] = character;
			terminal.fill++;
			printf("%c", character);
			fflush(stdout);
		}
	}
}

void usart_terminal_rx(char character) {
	terminal.ticks = 0;
	if (terminal.protocol == ASCII) {
		usart_ascii_terminal_rx(character);
	} else {
		usart_binary_terminal_rx(character);
	}
}

void usart_terminal_tick(void) {
	terminal.ticks++;
	if ((terminal.ticks >= 3) && (terminal.protocol == BINARY)) {
		terminal.fill = 0;
		terminal.ticks = 0;
	}
}
