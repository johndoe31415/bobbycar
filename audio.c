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
#include "audio.h"
#include "winbond25q64.h"

#define AUDIO_BUFFER_SIZE 		256

struct active_audio_file_t {
	unsigned int playback_offset;
	unsigned int begin_disk_offset;
	unsigned int file_length;
};

struct audio_buffer_t {
	bool valid;
	unsigned int samples_total;
	unsigned int offset;
	uint8_t data[4 + AUDIO_BUFFER_SIZE];
};

static struct active_audio_file_t audio_file = {
	.playback_offset = 0,
	.begin_disk_offset = 0,
	.file_length = 	64563,
};

enum filling_action_t {
	IDLE,
	FILLING,
};

static struct audio_buffer_t buffers[2];
static unsigned int buffer_index = 0;
static enum filling_action_t filling_action = IDLE;

static void switch_buffers(void) {
	buffer_index = 1 - buffer_index;
}
/*
static void fire_fill_dma_buffer(struct audio_buffer_t *buffer, unsigned int disk_offset, unsigned int sample_count) {
}*/

void audio_file_new(unsigned int disk_offset, unsigned int file_length) {
	struct audio_buffer_t *next_buffer = &buffers[1 - buffer_index];
	next_buffer->valid = false;
	audio_file.playback_offset = 0;
	audio_file.begin_disk_offset = disk_offset;
	audio_file.file_length = file_length;
}

static void audio_check_buffers(void) {
	struct audio_buffer_t *next_buffer = &buffers[1 - buffer_index];
	if (next_buffer->valid) {
		return;
	}

	if (filling_action == IDLE) {
 		/* Not filling right now, start a fill */
		/* How many bytes has the sample left? */
		unsigned int remaining_bytes = audio_file.file_length - audio_file.playback_offset;

		/* How many should we fetch for the next buffer? */
		unsigned int fetch_bytes = (remaining_bytes > AUDIO_BUFFER_SIZE) ? AUDIO_BUFFER_SIZE : remaining_bytes;

		unsigned int disk_offset = audio_file.begin_disk_offset + audio_file.playback_offset;
		next_buffer->samples_total = 4 + fetch_bytes;
		next_buffer->data[0] = SPIFLASH_READ_DATA;
		next_buffer->data[1] = (disk_offset >> 16) & 0xff;
		next_buffer->data[2] = (disk_offset >> 8) & 0xff;
		next_buffer->data[3] = (disk_offset >> 0) & 0xff;

		/* Then fire off the DMA request */
		spiflash_txrx_dma(next_buffer->data, next_buffer->samples_total);

		/* And memorize we're trying to fill the next buffer */
		filling_action = FILLING;
	} else {
		/* Was filling, check if DMA is finished */
		enum dma_state_t dma_state = spiflash_get_dma_state();
		if (dma_state == DMA_SUCCESS) {
			/* Finished successfully. */
			next_buffer->offset = 4;
			next_buffer->valid = true;
			audio_file.playback_offset += next_buffer->samples_total - 4;
			if (audio_file.playback_offset >= audio_file.file_length) {
				audio_file.playback_offset = 0;
			}
			filling_action = IDLE;
		} else if (dma_state == DMA_IN_PROGRESS) {
			/* Just keep waiting. */
		} else {
			/* DMA seems to have errored, retry next time. */
			filling_action = IDLE;
		}
	}
}

uint8_t audio_next_sample(void) {
	audio_check_buffers();

	struct audio_buffer_t *current_buffer = &buffers[buffer_index];
	if (!current_buffer->valid) {
		struct audio_buffer_t *next_buffer = &buffers[1 - buffer_index];
		if (!next_buffer->valid) {
			return 0;
		} else {
			switch_buffers();
			current_buffer = next_buffer;
		}
	}

	uint8_t returned_sample = current_buffer->data[current_buffer->offset];
	current_buffer->offset++;
	if (current_buffer->offset >= current_buffer->samples_total) {
		/* Buffer at end. Switch to next */
		current_buffer->valid = false;
		switch_buffers();
	}

	return returned_sample;
}

void audio_playback(void) {
}
