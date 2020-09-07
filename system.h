/*
 *  WARNING: THIS FILE IS AUTO-GENERATED. CHANGES WILL BE OVERWRITTEN.
 *  Generated at 2020-09-06 18:39:00
 *  Generated by https://github.com/johndoe31415/mcuconfig
 */

/**
 *	mcuconfig - Generation of microcontroller build setups.
 *	Copyright (C) 2019-2020 Johannes Bauer
 *
 *	This file is part of mcuconfig.
 *
 *	mcuconfig is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; this program is ONLY licensed under
 *	version 3 of the License, later versions are explicitly excluded.
 *
 *	mcuconfig is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with mcuconfig; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	Johannes Bauer <JohannesBauer@gmx.de>
**/

#ifndef __STM32_F103_SYSTEM_H__
#define __STM32_F103_SYSTEM_H__

#include <stm32f10x_gpio.h>

// w25qxx_cs: PA4, mode = OutputPushPull, inverted, alternate function SPI1
#define w25qxx_cs_PORT				GPIOA
#define w25qxx_cs_PIN				4
#define w25qxx_cs_MASK				(1 << w25qxx_cs_PIN)
#define w25qxx_cs_set_high()		w25qxx_cs_PORT->BSRR = w25qxx_cs_MASK
#define w25qxx_cs_set_low()		w25qxx_cs_PORT->BRR = w25qxx_cs_MASK
#define w25qxx_cs_set_active()		w25qxx_cs_set_low()
#define w25qxx_cs_set_inactive()	w25qxx_cs_set_high()
#define w25qxx_cs_toggle()			w25qxx_cs_PORT->ODR ^= w25qxx_cs_MASK
#define w25qxx_cs_get()			((w25qxx_cs_PORT->IDR >> w25qxx_cs_PIN) & 1)
#define w25qxx_cs_is_high()		(w25qxx_cs_get() != 0)
#define w25qxx_cs_is_low()			(w25qxx_cs_get() == 0)
#define w25qxx_cs_is_active()		w25qxx_cs_is_low()
#define w25qxx_cs_is_inactive()	w25qxx_cs_is_high()

// w25qxx_sck: PA5, mode = AlternateFunction, alternate function SPI1
#define w25qxx_sck_PORT				GPIOA
#define w25qxx_sck_PIN				5
#define w25qxx_sck_MASK				(1 << w25qxx_sck_PIN)
#define w25qxx_sck_get()			((w25qxx_sck_PORT->IDR >> w25qxx_sck_PIN) & 1)
#define w25qxx_sck_is_high()		(w25qxx_sck_get() != 0)
#define w25qxx_sck_is_low()			(w25qxx_sck_get() == 0)
#define w25qxx_sck_is_active()		w25qxx_sck_is_high()
#define w25qxx_sck_is_inactive()	w25qxx_sck_is_low()

// w25qxx_miso: PA6, mode = InputFloat, alternate function SPI1
#define w25qxx_miso_PORT				GPIOA
#define w25qxx_miso_PIN				6
#define w25qxx_miso_MASK				(1 << w25qxx_miso_PIN)
#define w25qxx_miso_get()			((w25qxx_miso_PORT->IDR >> w25qxx_miso_PIN) & 1)
#define w25qxx_miso_is_high()		(w25qxx_miso_get() != 0)
#define w25qxx_miso_is_low()			(w25qxx_miso_get() == 0)
#define w25qxx_miso_is_active()		w25qxx_miso_is_high()
#define w25qxx_miso_is_inactive()	w25qxx_miso_is_low()

// w25qxx_mosi: PA7, mode = AlternateFunction, alternate function SPI1
#define w25qxx_mosi_PORT				GPIOA
#define w25qxx_mosi_PIN				7
#define w25qxx_mosi_MASK				(1 << w25qxx_mosi_PIN)
#define w25qxx_mosi_get()			((w25qxx_mosi_PORT->IDR >> w25qxx_mosi_PIN) & 1)
#define w25qxx_mosi_is_high()		(w25qxx_mosi_get() != 0)
#define w25qxx_mosi_is_low()			(w25qxx_mosi_get() == 0)
#define w25qxx_mosi_is_active()		w25qxx_mosi_is_high()
#define w25qxx_mosi_is_inactive()	w25qxx_mosi_is_low()

// rs232_tx: PA9, mode = AlternateFunction, alternate function USART1
#define rs232_tx_PORT				GPIOA
#define rs232_tx_PIN				9
#define rs232_tx_MASK				(1 << rs232_tx_PIN)
#define rs232_tx_get()			((rs232_tx_PORT->IDR >> rs232_tx_PIN) & 1)
#define rs232_tx_is_high()		(rs232_tx_get() != 0)
#define rs232_tx_is_low()			(rs232_tx_get() == 0)
#define rs232_tx_is_active()		rs232_tx_is_high()
#define rs232_tx_is_inactive()	rs232_tx_is_low()

// rs232_rx: PA10, mode = InputFloat, alternate function USART1
#define rs232_rx_PORT				GPIOA
#define rs232_rx_PIN				10
#define rs232_rx_MASK				(1 << rs232_rx_PIN)
#define rs232_rx_get()			((rs232_rx_PORT->IDR >> rs232_rx_PIN) & 1)
#define rs232_rx_is_high()		(rs232_rx_get() != 0)
#define rs232_rx_is_low()			(rs232_rx_get() == 0)
#define rs232_rx_is_active()		rs232_rx_is_high()
#define rs232_rx_is_inactive()	rs232_rx_is_low()

// led_red: PB0, mode = OutputPushPull
#define led_red_PORT				GPIOB
#define led_red_PIN				0
#define led_red_MASK				(1 << led_red_PIN)
#define led_red_set_high()		led_red_PORT->BSRR = led_red_MASK
#define led_red_set_low()		led_red_PORT->BRR = led_red_MASK
#define led_red_set_active()		led_red_set_high()
#define led_red_set_inactive()	led_red_set_low()
#define led_red_toggle()			led_red_PORT->ODR ^= led_red_MASK
#define led_red_get()			((led_red_PORT->IDR >> led_red_PIN) & 1)
#define led_red_is_high()		(led_red_get() != 0)
#define led_red_is_low()			(led_red_get() == 0)
#define led_red_is_active()		led_red_is_high()
#define led_red_is_inactive()	led_red_is_low()

// led_orange: PB1, mode = OutputPushPull
#define led_orange_PORT				GPIOB
#define led_orange_PIN				1
#define led_orange_MASK				(1 << led_orange_PIN)
#define led_orange_set_high()		led_orange_PORT->BSRR = led_orange_MASK
#define led_orange_set_low()		led_orange_PORT->BRR = led_orange_MASK
#define led_orange_set_active()		led_orange_set_high()
#define led_orange_set_inactive()	led_orange_set_low()
#define led_orange_toggle()			led_orange_PORT->ODR ^= led_orange_MASK
#define led_orange_get()			((led_orange_PORT->IDR >> led_orange_PIN) & 1)
#define led_orange_is_high()		(led_orange_get() != 0)
#define led_orange_is_low()			(led_orange_get() == 0)
#define led_orange_is_active()		led_orange_is_high()
#define led_orange_is_inactive()	led_orange_is_low()

// led_green: PB2, mode = OutputPushPull
#define led_green_PORT				GPIOB
#define led_green_PIN				2
#define led_green_MASK				(1 << led_green_PIN)
#define led_green_set_high()		led_green_PORT->BSRR = led_green_MASK
#define led_green_set_low()		led_green_PORT->BRR = led_green_MASK
#define led_green_set_active()		led_green_set_high()
#define led_green_set_inactive()	led_green_set_low()
#define led_green_toggle()			led_green_PORT->ODR ^= led_green_MASK
#define led_green_get()			((led_green_PORT->IDR >> led_green_PIN) & 1)
#define led_green_is_high()		(led_green_get() != 0)
#define led_green_is_low()			(led_green_get() == 0)
#define led_green_is_active()		led_green_is_high()
#define led_green_is_inactive()	led_green_is_low()

// ws2812: PB13, mode = OutputPushPull
#define ws2812_PORT				GPIOB
#define ws2812_PIN				13
#define ws2812_MASK				(1 << ws2812_PIN)
#define ws2812_set_high()		ws2812_PORT->BSRR = ws2812_MASK
#define ws2812_set_low()		ws2812_PORT->BRR = ws2812_MASK
#define ws2812_set_active()		ws2812_set_high()
#define ws2812_set_inactive()	ws2812_set_low()
#define ws2812_toggle()			ws2812_PORT->ODR ^= ws2812_MASK
#define ws2812_get()			((ws2812_PORT->IDR >> ws2812_PIN) & 1)
#define ws2812_is_high()		(ws2812_get() != 0)
#define ws2812_is_low()			(ws2812_get() == 0)
#define ws2812_is_active()		ws2812_is_high()
#define ws2812_is_inactive()	ws2812_is_low()


void default_fault_handler(void);
void early_system_init(void);

#endif