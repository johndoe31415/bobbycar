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

#include <stdio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_crc.h>
#include "usart.h"
#include "system.h"
#include "usart_terminal.h"

void usart_transmit_char(char character) {
	led_orange_toggle();
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, character);
}

void USART1_Handler(void) {
	if (USART_GetITStatus(USART1, USART_IT_RXNE)) {
		led_green_toggle();
		char received_char = USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		usart_terminal_rx(received_char);
#if 0
		CRC_ResetDR();
		uint32_t crc = CRC_CalcBlockCRC((uint32_t*)"ABCD", 1);
		printf("ABCD = 0x%lx\n", crc);
#endif
	}
}
