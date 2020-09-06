/**
 *	bobbycar - Hello world program for Cortex-M3.
 *	Copyright (C) 2020-2020 Johannes Bauer
 *
 *	This file is part of bobbycar.
 *
 *	bobbycar is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; this program is ONLY licensed under
 *	version 3 of the License, later versions are explicitly excluded.
 *
 *	bobbycar is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with bobbycar; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	Johannes Bauer <JohannesBauer@gmx.de>
**/

#include "ws2812.h"
#include "ws2812_delay.h"

/* 1 tick approximately 100ns */

static void ws2812_emit_one(GPIO_TypeDef *port, const uint32_t pin_mask) {
	port->BRR = pin_mask;
	ws2812_delay(5);		// 700ns
	port->BSRR = pin_mask;
	ws2812_delay(3);		// 600ns
}

static void ws2812_emit_zero(GPIO_TypeDef *port, const uint32_t pin_mask) {
	port->BRR = pin_mask;
	ws2812_delay(1);		// 350ns
	port->BSRR = pin_mask;
	ws2812_delay(4);		// 800ns
}

void ws2812_sendbits(GPIO_TypeDef *port, unsigned int pin_no, unsigned int led_count, const void *led_data) {
	unsigned int bit_count = led_count * 24;
	const uint32_t pin_mask = 1 << pin_no;

	while (bit_count > 0) {
		uint32_t current_word = *((const uint8_t*)led_data++);
		for (unsigned int word_bits = 0; word_bits < 8; word_bits++) {
			if (current_word & 1) {
				ws2812_emit_one(port, pin_mask);
			} else {
				ws2812_emit_zero(port, pin_mask);
			}

			current_word >>= 1;
			bit_count--;
			if (bit_count == 0) {
				break;
			}
		}
	}
}
