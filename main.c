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
#include <stm32f10x_tim.h>
#include <stm32f10x_usart.h>

#include "usart_terminal.h"
#include "system.h"
#include "main.h"
#include "audio.h"

void SysTick_Handler(void) {
//	led_red_toggle();
	usart_terminal_tick();
}

int main(void) {
	printf("Device startup complete.\n");
	audio_init();
	//spiflash_reset();
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
	led_green_set_active();
	power_set_set_active();
	while (true) {
		if (power_on_is_active()) {
			led_red_set_active();
		} else {
			led_red_set_inactive();
		}
		/*
		printf("status: %x\n", spiflash_read_status());

		uint8_t data[80];
		spiflash_read(0, data, sizeof(data));
		for (int i = 0; i < sizeof(data); i++) printf("%02x", data[i]);
		printf("\n");

		if (data[1] == 0) {
			printf("erasing...\n");
			spiflash_erase_sector(0);
			printf("erase done.\n");
		}

		delay(10000000);
*/
	}
}
