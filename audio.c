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
#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x_tim.h>
#include "audio.h"
#include "winbond25q64.h"
#include "main.h"
#include "crc32.h"
#include "time.h"

#define AUDIO_BUFFER_SIZE 		256
#define MAX_FILE_COUNT			8

struct audio_toc_entry_t {
	uint32_t begin_disk_offset;
	uint32_t file_length;
	uint8_t filename[52];
	uint32_t crc32;
} __attribute__ ((packed));

struct active_audio_file_t {
	int fileno;
	unsigned int playback_offset;
	unsigned int begin_disk_offset;
	unsigned int file_length;
};

struct audio_buffer_t {
	bool valid;
	unsigned int samples_total;
	unsigned int offset;
	unsigned int absolute_offset;
	uint8_t data[4 + AUDIO_BUFFER_SIZE];
};

static struct active_audio_file_t audio_file = {
	.fileno = 0,
	.playback_offset = 0,
	.begin_disk_offset = 0,
	.file_length = 	0,
};

enum filling_action_t {
	IDLE,
	FILLING,
};

static struct audio_buffer_t audio_buffers[2];
static unsigned int buffer_index = 0;
static enum filling_action_t filling_action = IDLE;
static struct {
	unsigned int begin_disk_offset;
	unsigned int file_length;
} present_files[MAX_FILE_COUNT];
static int trigger_point = -1;
static unsigned int trigger_point_index = 0;


static struct audio_buffer_t* get_current_audio_buffer(void) {
	return &audio_buffers[buffer_index];
}

static struct audio_buffer_t* get_next_audio_buffer(void) {
	return &audio_buffers[1 - buffer_index];
}

void TIM2_Handler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)   {
		TIM1->CCR1 = audio_next_sample() >> 3;
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	}
}

static void switch_audio_buffers(void) {
	buffer_index = 1 - buffer_index;
}

void audio_playback(unsigned int disk_offset, unsigned int file_length, bool discard_nextbuffer) {
	if (discard_nextbuffer) {
		struct audio_buffer_t *next_buffer = get_next_audio_buffer();
		next_buffer->valid = false;
	}
	audio_file.playback_offset = 0;
	audio_file.begin_disk_offset = disk_offset;
	audio_file.file_length = file_length;
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
}

void audio_playback_fileno(unsigned int fileno, bool discard_nextbuffer) {
	if ((fileno > MAX_FILE_COUNT) || (present_files[fileno].begin_disk_offset == 0xffffffff)) {
		audio_shutoff();
	} else {
		if (audio_file.fileno != fileno) {
			audio_playback(present_files[fileno].begin_disk_offset, present_files[fileno].file_length, discard_nextbuffer);
			audio_file.fileno = fileno;
			if ((fileno == FILENO_TURN_SIGNAL_WITH_ENGINE) || (fileno == FILENO_TURN_SIGNAL_NO_ENGINE)) {
				trigger_point_index = 0;
				trigger_point = 65 + 4079 * trigger_point_index;
			} else {
				trigger_point = -1;
			}
		}
	}
}

static void audio_check_audio_buffers(void) {
	struct audio_buffer_t *next_buffer = get_next_audio_buffer();
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
			next_buffer->absolute_offset = audio_file.playback_offset;
			next_buffer->valid = true;
			audio_file.playback_offset += next_buffer->samples_total - 4;
			if (audio_file.playback_offset >= audio_file.file_length) {
				audio_file.playback_offset = 0;
				audio_trigger_end_of_sample(audio_file.fileno);
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

int audio_current_fileno(void) {
	return audio_file.fileno;
}

static void audio_execute_trigger_point(void) {
	trigger_point_index += 1;
	if (trigger_point_index >= 12) {
		trigger_point_index = 0;
	}
	trigger_point = 65 + 4079 * trigger_point_index;

	audio_trigger_point();
}

uint8_t audio_next_sample(void) {
	audio_check_audio_buffers();

	struct audio_buffer_t *current_buffer = get_current_audio_buffer();
	if (!current_buffer->valid) {
		struct audio_buffer_t *next_buffer = get_next_audio_buffer();
		if (!next_buffer->valid) {
			return 0;
		} else {
			switch_audio_buffers();
			current_buffer = next_buffer;
		}
	}

	uint8_t returned_sample = current_buffer->data[current_buffer->offset];
	current_buffer->offset++;
	current_buffer->absolute_offset++;
	if (current_buffer->absolute_offset == trigger_point) {
		audio_execute_trigger_point();
	}
	if (current_buffer->offset >= current_buffer->samples_total) {
		/* Buffer at end. Switch to next */
		current_buffer->valid = false;
		switch_audio_buffers();
	}

	return returned_sample;
}

void audio_shutoff(void) {
	/* Disable audio "next_sample" IRQ and set output to 0 */
	TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
	TIM1->CCR1 = 0;
	audio_file.fileno = -1;
}

void audio_init(void) {
	/* Read audio TOC */
	for (unsigned int i = 0; i < MAX_FILE_COUNT; i++) {
		const unsigned int offset = sizeof(struct audio_toc_entry_t) * i;
		struct audio_toc_entry_t entry;
		present_files[i].begin_disk_offset = 0xffffffff;
		for (unsigned int try = 0; try < 100; try++) {
			spiflash_read(offset, &entry, sizeof(entry));
			if (entry.begin_disk_offset != 0xffffffff) {
				uint32_t computed_crc = compute_crc32(&entry, sizeof(entry) - 4);
				if (computed_crc == entry.crc32) {
					printf("File %d: \"%s\", offset 0x%lx, length %lu, CRC32 0x%lx OK\n", i, entry.filename, entry.begin_disk_offset, entry.file_length, entry.crc32);
					present_files[i].begin_disk_offset = entry.begin_disk_offset;
					present_files[i].file_length = entry.file_length;
					break;
				} else {
					printf("File %d: offset 0x%lx, length %lu, CRC32 ERR 0x%lx computed 0x%lx. Retrying (try #%d).\n", i, entry.begin_disk_offset, entry.file_length, entry.crc32, computed_crc, try + 1);
					systick_wait();
				}
			}
		}
	}
}
