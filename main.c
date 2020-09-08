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
#include <stm32f10x_usart.h>

#include "winbond25q64.h"
#include "system.h"
#include "main.h"

void SysTick_Handler(void) {
	led_red_toggle();
}

static void delay(uint32_t duration) {
	volatile uint32_t ctr = duration;
	while (ctr--);
}

void USART1_Handler(void) {
	if (USART_GetITStatus(USART1, USART_IT_RXNE)) {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		led_green_toggle();
		printf("\n");
	}
}

int main(void) {
	printf("Device startup complete.\n");
	spiflash_reset();
#if 0
	while (true);
	//spiflash_selfcheck();
	while (true) {
		led_green_toggle();
		delay(1000000);

		led_orange_toggle();
		delay(1000000);

		led_red_toggle();
		delay(1000000);

		struct spiflash_manufacturer_t id = spiflash_read_id_dma();
		printf("ID %x %x\n", id.manufacturer_id, id.device_id);
	}
#endif
	while (true) {
		struct spiflash_manufacturer_t id = spiflash_read_id_dma();
		printf("ID %x / %x\n", id.manufacturer_id, id.device_id);
		delay(1000000);
	}
}
