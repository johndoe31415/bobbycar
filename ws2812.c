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

#include "ws2812.h"
#include "ws2812_delay.h"

void ws2812_sendbits(GPIO_TypeDef *port, unsigned int pin_no, unsigned int led_count, const void *led_data) {
	unsigned int bit_count = led_count * 24;
	const uint32_t pin_mask = 1 << pin_no;

	__disable_irq();
	while (bit_count > 0) {
		uint32_t current_word = *((const uint8_t*)led_data++);
		for (unsigned int word_bits = 0; word_bits < 8; word_bits++) {
			if (current_word & 1) {
				ws2812_emit_1(port, pin_mask);
			} else {
				ws2812_emit_0(port, pin_mask);
			}

			current_word >>= 1;
			bit_count--;
			if (bit_count == 0) {
				break;
			}
		}
	}
	__enable_irq();
}
