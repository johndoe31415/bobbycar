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

.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

.section .text
.set BRR_OFFSET, 20
.set BSRR_OFFSET, 16
.set overhead, 4

.type ws2812_emit_0, %function
ws2812_emit_0:
	str r1, [r0, #BRR_OFFSET]

	movs  r2, #4					// 350ns
	0:
	subs  r2, r2, #1
	bne   0b

	str r1, [r0, #BSRR_OFFSET]

	movs  r2, #(10 - overhead)		// 800ns
	1:
	subs  r2, r2, #1
	bne   1b

	bx lr
.size ws2812_emit_0, .-ws2812_emit_0
.global ws2812_emit_0



.type ws2812_emit_1, %function
ws2812_emit_1:
	str r1, [r0, #BRR_OFFSET]

	movs  r2, #8					// 700ns
	0:
	subs  r2, r2, #1
	bne   0b

	str r1, [r0, #BSRR_OFFSET]

	movs  r2, #(7 - overhead)		// 600ns
	1:
	subs  r2, r2, #1
	bne   1b

	bx lr
.size ws2812_emit_1, .-ws2812_emit_1
.global ws2812_emit_1
