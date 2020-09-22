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

#include <stdbool.h>
#include "audio.h"
#include "audio_hl.h"
#include "system.h"

enum audio_fileno_t {
	FILENO_ENGINE_START = 0,
	FILENO_ENGINE_IDLE = 1,
	FILENO_ENGINE_STOP = 2,
	FILENO_SIREN_NO_ENGINE = 3,
	FILENO_SIREN_WITH_ENGINE = 4,
	FILENO_TURN_SIGNAL_NO_ENGINE = 5,
	FILENO_TURN_SIGNAL_WITH_ENGINE = 6,
};

static enum audio_mode_t {
	OTHER,
	TURN_SIGNAL,
	ENGINE_START,
	ENGINE_STOP,
	SIREN,
} audio_mode = OTHER;

static unsigned int trigger_point_index = 0;
static bool engine_running;
static bool blinker_state;

void audio_trigger_end_of_sample(void) {
	if (audio_mode == ENGINE_START) {
		audio_playback_fileno(FILENO_ENGINE_IDLE, false);
		audio_mode = OTHER;
	} else if (audio_mode == ENGINE_STOP) {
		audio_shutoff();
		audio_mode = OTHER;
	}
}

void audio_trigger_point(void) {
	blinker_state = !blinker_state;
	trigger_point_index = trigger_point_index + 1;
	if (trigger_point_index >= 12) {
		trigger_point_index = 0;
	}
	audio_set_trigger_point(65 + 4079 * trigger_point_index);
}

void audio_hl_start_engine(void) {
	audio_mode = ENGINE_START;
	audio_playback_fileno(FILENO_ENGINE_START, true);
	engine_running = true;
}

void audio_hl_stop_engine(void) {
	audio_mode = ENGINE_STOP;
	audio_playback_fileno(FILENO_ENGINE_STOP, true);
	engine_running = false;
}

static void audio_restore_engine(void) {
	if (!engine_running) {
		audio_shutoff();
	} else {
		audio_playback_fileno(FILENO_ENGINE_IDLE, false);
	}
	audio_mode = OTHER;
}

void audio_hl_turn_signal_on(void) {
	audio_mode = TURN_SIGNAL;
	audio_playback_fileno(engine_running ? FILENO_TURN_SIGNAL_WITH_ENGINE : FILENO_TURN_SIGNAL_NO_ENGINE, true);
	blinker_state = false;
	trigger_point_index = 0;
	audio_set_trigger_point(65 + 4079 * trigger_point_index);
}

void audio_hl_turn_signal_off(void) {
	blinker_state = false;
	audio_set_trigger_point(-1);
	audio_restore_engine();
}

void audio_hl_siren_on(void) {
	audio_mode = OTHER;
	audio_playback_fileno(engine_running ? FILENO_SIREN_WITH_ENGINE : FILENO_SIREN_NO_ENGINE, true);
}

void audio_hl_siren_off(void) {
	audio_restore_engine();
}
