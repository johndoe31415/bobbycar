/**
 *	bobbycar - Modded Bobby Car toy for toddlers
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

#include <stdint.h>
#include <stdbool.h>
#include "crc32.h"

#define CRC_POLY		0x04c11db7

static uint8_t reflect_byte(uint8_t value) {
	uint8_t reflected = 0;
	for (int i = 0; i < 8; i++) {
		reflected = (reflected << 1) | (value & 1);
		value >>= 1;
	}
	return reflected;
}

static uint32_t reflect_uint32(uint32_t value) {
	/* Switch byteorder to big endian */
	value = (((value >> 0) & 0xff) << 24) | (((value >> 8) & 0xff) << 16) | (((value >> 16) & 0xff) << 8) | (((value >> 24) & 0xff) << 0);

	/* Reflect bytes individually */
	uint8_t *array = (uint8_t*)&value;
	for (int i = 0; i < 4; i++) {
		array[i] = reflect_byte(array[i]);
	}
	return value;
}

static uint32_t update_crc32(uint32_t crc, uint8_t next_byte) {
	for (int bit = 0; bit < 8; bit++) {
		bool crc_bit = crc & 0x80000000;
		bool data_bit = (next_byte >> bit) & 1;
		crc <<= 1;
		if (crc_bit ^ data_bit) {
			crc ^= CRC_POLY;
		}
	}
	return crc;
}

uint32_t compute_crc32(const void *data, uint32_t length) {
	uint32_t crc = 0xffffffff;
	for (unsigned int i = 0; i < length; i++) {
		crc = update_crc32(crc, ((const uint8_t*)data)[i]);
	}
	return reflect_uint32(crc ^ 0xffffffff);
}

#ifdef __MAIN__
#include <stdio.h>
#include <string.h>

static void check_crc(const char *string, uint32_t expect_value) {
	uint32_t computed_value = compute_crc32(string, strlen(string));
	printf("CRC-32 of \"%s\": 0x%lx ", string, computed_value);
	if (computed_value == expect_value) {
		printf("OK\n");
	} else {
		printf("FAIL (expected 0x%lx)\n", expect_value);
	}
}

void crc_selftest(void) {
	check_crc("", 0);
	check_crc("\x01", 0xA505DF1B);
	check_crc("\x80", 0x3fba6cad);
	check_crc("foo", 0x8C736521);
}

int main(void) {
	crc_selftest();
	return 0;
}

#endif
