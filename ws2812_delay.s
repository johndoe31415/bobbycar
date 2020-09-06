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
.type ws2812_delay, %function
ws2812_delay:
	cbz r0, exit_delay
	subs r0, 1
	b ws2812_delay


	exit_delay:
	bx lr
.size ws2812_delay, .-ws2812_delay
.global ws2812_delay
