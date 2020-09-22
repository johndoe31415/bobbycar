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

#include "usart_terminal.h"
#include "system.h"
#include "main.h"
#include "audio.h"
#include "ws2812.h"
#include "adc.h"

struct uistate_t {
	unsigned int undervoltage_counter;
};

volatile unsigned int timectr = 0;

static void wait_systick(void) {
	unsigned int old_timectr = timectr;
	while (old_timectr == timectr);
}

void SysTick_Handler(void) {
	timectr++;
	usart_terminal_tick();
}

static void enter_error_mode(unsigned int error_code) {
	printf("%u: error code %u\n", timectr, error_code);
	led_red_set_active();
	led_green_set_to(error_code & 1);
	led_orange_set_to(error_code & 2);
	led_siren_set_inactive();
	audio_shutoff();
	while (true) {
		__WFI();
	}
}

int main(void) {
	led_green_set_active();
	power_keepalive_set_active();
	printf("Device cold start complete.\n");
	while (timectr < 125);		/* Wait 250 ms before flash settles */
	audio_init();

	struct uistate_t state = { 0 };
	while (true) {
		wait_systick();

		uint32_t voltage_millivolts = adc_get_ext_voltage_millivolts();
		if (voltage_millivolts < 3500 * 3) {
			state.undervoltage_counter++;
		} else {
			state.undervoltage_counter = 0;
		}
		if (state.undervoltage_counter >= 100) {
			/* One second of straight undervoltage, go into error mode. */
			enter_error_mode(0);
		}

#if 1
		uint8_t foo[3] = { 0xff, 0 , 0 };
		ws2812_sendbits(ws2812_PORT, ws2812_PIN, 1, foo);
#endif
	}
}
