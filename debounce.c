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

bool debounce_button(struct debounce_t *button, unsigned int current_state) {
	bool change = false;

	if (button->counter == 0) {
		/* Uninitialized */
		button->last_state = current_state;
		button->counter = button->config->fire_threshold;
	} else {
		if (current_state != button->last_state) {
			if (button->counter > 0) {
				button->counter--;
				if (button->counter == 0) {
					change = true;
					button->counter = button->config->fire_threshold;
					button->last_state = current_state;
				}
			}
		} else {
			if (button->counter < button->config->fire_threshold) {
				button->counter++;
			}
		}
	}
	return change;
}

bool debounce_button_active(struct debounce_t *button, unsigned int current_state) {
	return debounce_button(button, current_state) && button->last_state;
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
		if (!fgets(buffer, sizeof(buffer) - 1, stdin)) {
			break;
		}

		int current_state = atoi(buffer);
		bool change = debounce_button(&button, current_state);
		printf("[%d] -> %d\n", change, button.last_state);
	}
}
#endif
