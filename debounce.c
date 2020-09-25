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

#include "debounce.h"

enum debounce_result_t debounce_button(struct debounce_t *button, bool current_state) {
	enum debounce_result_t result = DEBOUNCE_NO_CHANGE;

	if (button->last_state == DEBOUNCE_UNINITIALIZED) {
		button->last_state = current_state ? DEBOUNCE_CLOSED : DEBOUNCE_OPEN;
	} else {
		bool might_fire = false;
		if (!current_state) {
			if (button->counter > 0) {
				button->counter--;
				if (button->counter == 0) {
					might_fire = true;
				}
			}
		} else {
			if (button->counter < button->config->fire_threshold) {
				button->counter++;
				if (button->counter == button->config->fire_threshold) {
					might_fire = true;
				}
			}
		}

		if (might_fire) {
			if ((button->last_state == DEBOUNCE_OPEN) && current_state) {
				button->last_state = DEBOUNCE_CLOSED;
				result = DEBOUNCE_PRESSED;
			} else if ((button->last_state == DEBOUNCE_CLOSED) && (!current_state)) {
				button->last_state = DEBOUNCE_OPEN;
				result = DEBOUNCE_RELEASED;
			}
		}
	}
	return result;
}

#ifdef __MAIN__
// gcc -D__MAIN__ -fsanitize=address -fsanitize=leak -fsanitize=undefined -std=c11 -O2 -o debounce debounce.c && ./debounce
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	struct debounce_t button = {
		.config = &(const struct debounce_config_t){
			.fire_threshold = 3,
		},
	};
	while (true) {
		char buffer[32];
		printf("0 or 1: ");
		fflush(stdout);
		fgets(buffer, sizeof(buffer) - 1, stdin);

		bool current_state = atoi(buffer);
		printf("[%d] -> %d\n", current_state, debounce_button(&button, current_state));
	}
}
#endif
