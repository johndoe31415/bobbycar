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
#include "debounce.h"

enum ignition_state_t {
	IGNITION_UNDEFINED,
	IGNITION_ON,
	IGNITION_OFF,
	IGNITION_CRANK,
	IGNITION_CCW,
};

struct uistate_t {
	unsigned int undervoltage_counter;
	struct debounce_t button_left;
	struct debounce_t button_right;
	struct debounce_t button_siren;
	struct debounce_t button_parent;
	struct debounce_t button_ignition_crank;
	struct debounce_t button_ignition_ccw;
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
	led_yellow_set_to(error_code & 2);
	led_siren_set_inactive();
	audio_shutoff();
	while (true) {
		__WFI();
	}
}

static enum ignition_state_t determine_ignition_state(const struct uistate_t *state) {
	enum ignition_state_t ignition_state = IGNITION_UNDEFINED;
	bool have_power = pwr_sense_is_active();
//	printf("Crank %d CCW %d Pwr %d\n", state.button_ignition_crank.last_state, state.button_ignition_ccw.last_state, pwr_sense_is_active());
	if (have_power) {
		/* Either "ignition on" or "ignition crank" */
		if ((state->button_ignition_crank.last_state == DEBOUNCE_OPEN) && (state->button_ignition_ccw.last_state == DEBOUNCE_CLOSED)) {
			ignition_state = IGNITION_ON;
		} else if ((state->button_ignition_crank.last_state == DEBOUNCE_CLOSED) && (state->button_ignition_ccw.last_state == DEBOUNCE_OPEN)) {
			ignition_state = IGNITION_CRANK;
		}
	} else {
		/* Either "ignition off" or "ignition CCW" */
		if ((state->button_ignition_crank.last_state == DEBOUNCE_OPEN) && (state->button_ignition_ccw.last_state == DEBOUNCE_OPEN)) {
			ignition_state = IGNITION_OFF;
		} else if ((state->button_ignition_crank.last_state == DEBOUNCE_OPEN) && (state->button_ignition_ccw.last_state == DEBOUNCE_CLOSED)) {
			ignition_state = IGNITION_CCW;
		}
	}
	return ignition_state;
}

int main(void) {
	led_green_set_active();
	pwr_keepalive_set_active();
	printf("Device cold start complete.\n");
	while (timectr < 125);		/* Wait 250 ms before flash settles */
	audio_init();

	const struct debounce_config_t default_button_config = {
		.fire_threshold = 10,
	};
	struct uistate_t state = {
		.button_left.config = &default_button_config,
		.button_right.config = &default_button_config,
		.button_siren.config = &default_button_config,
		.button_parent.config = &default_button_config,
		.button_ignition_crank.config = &default_button_config,
		.button_ignition_ccw.config = &default_button_config,
	};
	while (true) {
		/* Execute roughly 100 Hz */
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

		if (debounce_button(&state.button_left, button_left_is_active()) == DEBOUNCE_PRESSED) {
			printf("Left\n");
		}
		if (debounce_button(&state.button_right, button_right_is_active()) == DEBOUNCE_PRESSED) {
			printf("Right\n");
		}
		if (debounce_button(&state.button_parent, button_parent_is_active()) == DEBOUNCE_PRESSED) {
			printf("Parent\n");
		}
		if (debounce_button(&state.button_siren, button_siren_is_active()) == DEBOUNCE_PRESSED) {
			printf("Siren\n");
		}

		debounce_button(&state.button_ignition_crank, ignition_crank_is_active());
		debounce_button(&state.button_ignition_ccw, ignition_ccw_is_active());
		enum ignition_state_t ignition_state = determine_ignition_state(&state);
		printf("Ignition %d\n", ignition_state);


#if 1
		uint8_t foo[6] = { 0xff, 0x0 , 0, 0x0,0xff,0 };
		ws2812_sendbits(ws2812_PORT, ws2812_PIN, 2, foo);
#endif
	}
}
