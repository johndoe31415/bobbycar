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

enum audio_fileno_t {
	FILENO_ENGINE_START = 0,
	FILENO_ENGINE_IDLE = 1,
	FILENO_ENGINE_STOP = 2,
	FILENO_SIREN_NO_ENGINE = 3,
	FILENO_SIREN_WITH_ENGINE = 4,
	FILENO_TURN_SIGNAL_NO_ENGINE = 5,
	FILENO_TURN_SIGNAL_WITH_ENGINE = 6,
};

enum ignition_state_t {
	IGNITION_UNDEFINED,
	IGNITION_ON,
	IGNITION_OFF,
	IGNITION_CRANK,
	IGNITION_CCW,
};

enum turn_signal_state_t {
	TURN_OFF,
	TURN_LEFT,
	TURN_RIGHT,
	TURN_EMERGENCY,
};

enum engine_state_t {
	ENGINE_OFF,
	ENGINE_CRANKING,
	ENGINE_ON,
	ENGINE_SHUTTING_OFF,
};

struct uistate_t {
	enum engine_state_t engine_state;
	bool siren;
	enum turn_signal_state_t turn_signal;
	unsigned int undervoltage_counter;
	struct debounce_t button_left;
	struct debounce_t button_right;
	struct debounce_t button_siren;
	struct debounce_t button_parent;
	struct debounce_t button_ignition_crank;
	struct debounce_t button_ignition_ccw;
	struct debounce_t ignition_state;
	unsigned int siren_blink_counter;
};

static volatile unsigned int timectr = 0;
static const struct debounce_config_t default_button_config = {
	.fire_threshold = 10,
};
static struct uistate_t ui = {
	.button_left.config = &default_button_config,
	.button_right.config = &default_button_config,
	.button_siren.config = &default_button_config,
	.button_parent.config = &default_button_config,
	.button_ignition_crank.config = &default_button_config,
	.button_ignition_ccw.config = &default_button_config,
	.ignition_state.config = &default_button_config,
};

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

static enum ignition_state_t determine_ignition_state(void) {
	enum ignition_state_t ignition_state = IGNITION_UNDEFINED;
	bool have_power = pwr_sense_is_active();
//	printf("Crank %d CCW %d Pwr %d\n", state.button_ignition_crank.last_state, state.button_ignition_ccw.last_state, pwr_sense_is_active());
	if (have_power) {
		/* Either "ignition on" or "ignition crank" */
		if ((ui.button_ignition_crank.last_state == 0) && (ui.button_ignition_ccw.last_state == 1)) {
			ignition_state = IGNITION_ON;
		} else if ((ui.button_ignition_crank.last_state == 1) && (ui.button_ignition_ccw.last_state == 0)) {
			ignition_state = IGNITION_CRANK;
		}
	} else {
		/* Either "ignition off" or "ignition CCW" */
		if ((ui.button_ignition_crank.last_state == 0) && (ui.button_ignition_ccw.last_state == 0)) {
			ignition_state = IGNITION_OFF;
		} else if ((ui.button_ignition_crank.last_state == 0) && (ui.button_ignition_ccw.last_state == 1)) {
			ignition_state = IGNITION_CCW;
		}
	}
	return ignition_state;
}

static void audio_update(void) {
	if (ui.engine_state == ENGINE_CRANKING) {
		audio_playback_fileno(FILENO_ENGINE_START, true);
	} else if (ui.engine_state == ENGINE_SHUTTING_OFF) {
		audio_playback_fileno(FILENO_ENGINE_STOP, true);
	} else if (ui.engine_state == ENGINE_ON) {
		if (ui.siren) {
			audio_playback_fileno(FILENO_SIREN_WITH_ENGINE, true);
		} else {
			audio_playback_fileno(FILENO_ENGINE_IDLE, true);
		}
	} else if (ui.engine_state == ENGINE_OFF) {
		if (ui.siren) {
			audio_playback_fileno(FILENO_SIREN_NO_ENGINE, true);
		} else {
			audio_shutoff();
		}
	}
}

void audio_trigger_end_of_sample(unsigned int fileno) {
	if (fileno == FILENO_ENGINE_START) {
		/* Start of engine is finished */
		ui.engine_state = ENGINE_ON;
		audio_update();
	} else if (fileno == FILENO_ENGINE_STOP) {
		ui.engine_state = ENGINE_OFF;
		audio_update();
	}
}

void audio_trigger_point(void) {
}

int main(void) {
	led_green_set_active();
	pwr_keepalive_set_active();
	printf("Device cold start complete.\n");
	while (timectr < 125);		/* Wait 250 ms before flash settles */
	audio_init();

	while (true) {
		/* Execute roughly 100 Hz */
		wait_systick();

		uint32_t voltage_millivolts = adc_get_ext_voltage_millivolts();
		if (voltage_millivolts < 3500 * 3) {
			ui.undervoltage_counter++;
		} else {
			ui.undervoltage_counter = 0;
		}
		if (ui.undervoltage_counter >= 100) {
			/* One second of straight undervoltage, go into error mode. */
			enter_error_mode(0);
		}

		if (debounce_button_active(&ui.button_left, button_left_is_active())) {
			printf("Left\n");
		}
		if (debounce_button_active(&ui.button_right, button_right_is_active())) {
			printf("Right\n");
		}
		if (debounce_button_active(&ui.button_parent, button_parent_is_active())) {
			printf("Parent\n");
		}
		if (debounce_button_active(&ui.button_siren, button_siren_is_active())) {
			ui.siren = !ui.siren;
			if (ui.siren) {
				led_siren_set_active();
			} else {
				ui.siren_blink_counter = 0;
			}
			audio_update();
		}

		debounce_button(&ui.button_ignition_crank, ignition_crank_is_active());
		debounce_button(&ui.button_ignition_ccw, ignition_ccw_is_active());
		enum ignition_state_t current_ignition_state = determine_ignition_state();
		bool ignition_state_changed = debounce_button(&ui.ignition_state, current_ignition_state);
		if (ignition_state_changed) {
			if ((ui.engine_state == ENGINE_OFF) && (ui.ignition_state.last_state == IGNITION_CRANK)) {
				ui.engine_state = ENGINE_CRANKING;
				audio_update();
			} else if (((ui.engine_state == ENGINE_ON) || (ui.engine_state == ENGINE_CRANKING)) && ((ui.ignition_state.last_state == IGNITION_OFF) || (ui.ignition_state.last_state == IGNITION_CCW))) {
				ui.engine_state = ENGINE_SHUTTING_OFF;
				audio_update();
			}
		}

		if (!ui.siren) {
			led_siren_set_inactive();
		} else {
			ui.siren_blink_counter++;
			if (ui.siren_blink_counter > 30) {
				led_siren_toggle();
				ui.siren_blink_counter = 0;
			}
		}

#if 1
		uint8_t foo[6] = { 0xff, 0x0 , 0, 0x0,0xff,0 };
		ws2812_sendbits(ws2812_PORT, ws2812_PIN, 2, foo);
#endif
	}
}
