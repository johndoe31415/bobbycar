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

#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <stdbool.h>

enum audio_fileno_t {
	FILENO_ENGINE_START = 0,
	FILENO_ENGINE_IDLE = 1,
	FILENO_ENGINE_STOP = 2,
	FILENO_SIREN_NO_ENGINE = 3,
	FILENO_SIREN_WITH_ENGINE = 4,
	FILENO_TURN_SIGNAL_NO_ENGINE = 5,
	FILENO_TURN_SIGNAL_WITH_ENGINE = 6,
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void TIM2_Handler(void);
void audio_playback(unsigned int disk_offset, unsigned int file_length, bool discard_nextbuffer);
void audio_playback_fileno(unsigned int fileno, bool discard_nextbuffer);
int audio_current_fileno(void);
uint8_t audio_next_sample(void);
void audio_shutoff(void);
void audio_init(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
