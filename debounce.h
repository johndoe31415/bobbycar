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

#ifndef __DEBOUNCE_H__
#define __DEBOUNCE_H__

#include <stdint.h>
#include <stdbool.h>

enum debounce_result_t {
	DEBOUNCE_NO_CHANGE,
	DEBOUNCE_PRESSED,
	DEBOUNCE_RELEASED,
};

enum debounce_state_t {
	DEBOUNCE_UNINITIALIZED = 0,
	DEBOUNCE_OPEN = 1,
	DEBOUNCE_CLOSED = 2,
};

struct debounce_config_t {
	unsigned int fire_threshold;
};

struct debounce_t {
	const struct debounce_config_t *config;
	uint8_t last_state;
	uint8_t fired_state;
	unsigned int counter;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
enum debounce_result_t debounce_button(struct debounce_t *button, bool current_state);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
