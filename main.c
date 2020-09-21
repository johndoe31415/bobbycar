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
#include <stdbool.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_adc.h>

#include "usart_terminal.h"
#include "system.h"
#include "main.h"
#include "audio.h"
#include "ws2812.h"

void SysTick_Handler(void) {
//	led_red_toggle();
	usart_terminal_tick();
}

uint32_t adc_getvalue(void) {
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}

uint32_t adc_getavgsamples(unsigned int count) {
	uint32_t sum = count / 2;
	for (unsigned int i = 0; i < count; i++) {
		sum += adc_getvalue();
	}
	return sum / count;
}

uint32_t adu_to_millivolts(uint32_t adu) {
	return adu * 5000 / 1493;
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
	//led_green_set_active();
	//power_set_set_active();
	while (true) {
		led_red_set_to(power_sense_is_active());
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

*/
#if 1
		uint8_t foo[3] = { 0xff, 0 , 0 };
		ws2812_sendbits(ws2812_PORT, ws2812_PIN, 1, foo);
		for (volatile unsigned int i = 0; i < 1000000; i++);
#endif

		printf("%ld\n", adu_to_millivolts(adc_getavgsamples(128)));
	}
}
