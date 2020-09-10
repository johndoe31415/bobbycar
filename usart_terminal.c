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
#include <string.h>
#include <stm32f10x_dma.h>
#include "usart.h"
#include "usart_terminal.h"

#define CHAR_BACKSPACE		0x7f

#define MAX_TERMINAL_BUF		32

static struct terminal_options_t {
	char input_buffer[MAX_TERMINAL_BUF];
	unsigned int fill;
} terminal;

static void create_prompt(void) {
	printf("Command: ");
	fflush(stdout);
}

static void debug_dma_channel(const char *name, DMA_Channel_TypeDef *channel) {
	printf("  %s: ", name);
	printf("CCR %08lx ", channel->CCR);
	printf("%s ", (channel->CCR & 1) ? "enabled": "disabled");
	printf("%lu bytes remain", channel->CNDTR & 0xffff);
	printf("\n");
}

static void debug_dma(void) {
	printf("DMA debugging info:\n");
	printf("DMA1 Interrupt Status (ISR): %08lx\n", DMA1->ISR);
	debug_dma_channel("DMA1_Channel2 (SPI1 RX)", DMA1_Channel2);
	debug_dma_channel("DMA1_Channel3 (SPI1 TX)", DMA1_Channel3);
}

static void clear_command(void) {
	printf("\n");
	terminal.input_buffer[MAX_TERMINAL_BUF - 1] = 0;
	if (!strcmp(terminal.input_buffer, "?") || !strcmp(terminal.input_buffer, "help")) {
		printf("Help page:\n");
		printf("? or help     This help page.\n");
		printf("dma           DMA debugging.\n");
	} else if (!strcmp(terminal.input_buffer, "dma")) {
		debug_dma();
	} else {
		printf("Unknown command: %s\n", terminal.input_buffer);
	}
	terminal.fill = 0;
	create_prompt();
}

void usart_terminal_rx(char character) {
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
		if (terminal.fill < MAX_TERMINAL_BUF - 1) {
			terminal.input_buffer[terminal.fill] = character;
			terminal.fill++;
			printf("%c", character);
			fflush(stdout);
		}
	}
}
