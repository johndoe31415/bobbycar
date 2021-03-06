/*
 *  WARNING: THIS FILE IS AUTO-GENERATED. CHANGES WILL BE OVERWRITTEN.
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

// battery_sense: PA0, mode = Analog
#define battery_sense_PORT					GPIOA
#define battery_sense_PIN					0
#define battery_sense_MASK					(1 << battery_sense_PIN)
#define battery_sense_get()				((battery_sense_PORT->IDR >> battery_sense_PIN) & 1)
#define battery_sense_is_high()			(battery_sense_get() != 0)
#define battery_sense_is_low()				(battery_sense_get() == 0)
#define battery_sense_is_active()			battery_sense_is_high()
#define battery_sense_is_inactive()		battery_sense_is_low()

// uln2003_ledleft: PA1, mode = OutputPushPull
#define uln2003_ledleft_PORT					GPIOA
#define uln2003_ledleft_PIN					1
#define uln2003_ledleft_MASK					(1 << uln2003_ledleft_PIN)
#define uln2003_ledleft_set_high()			uln2003_ledleft_PORT->BSRR = uln2003_ledleft_MASK
#define uln2003_ledleft_set_low()			uln2003_ledleft_PORT->BRR = uln2003_ledleft_MASK
#define uln2003_ledleft_set_active()			uln2003_ledleft_set_high()
#define uln2003_ledleft_set_inactive()		uln2003_ledleft_set_low()
#define uln2003_ledleft_toggle()				uln2003_ledleft_PORT->ODR ^= uln2003_ledleft_MASK
#define uln2003_ledleft_set_to(value)		if (value) { uln2003_ledleft_set_active(); } else { uln2003_ledleft_set_inactive(); }
#define uln2003_ledleft_set_logic_to(value)	if (value) { uln2003_ledleft_set_high(); } else { uln2003_ledleft_set_low(); }
#define uln2003_ledleft_get()				((uln2003_ledleft_PORT->IDR >> uln2003_ledleft_PIN) & 1)
#define uln2003_ledleft_is_high()			(uln2003_ledleft_get() != 0)
#define uln2003_ledleft_is_low()				(uln2003_ledleft_get() == 0)
#define uln2003_ledleft_is_active()			uln2003_ledleft_is_high()
#define uln2003_ledleft_is_inactive()		uln2003_ledleft_is_low()

// uln2003_ledright: PA2, mode = OutputPushPull
#define uln2003_ledright_PORT					GPIOA
#define uln2003_ledright_PIN					2
#define uln2003_ledright_MASK					(1 << uln2003_ledright_PIN)
#define uln2003_ledright_set_high()			uln2003_ledright_PORT->BSRR = uln2003_ledright_MASK
#define uln2003_ledright_set_low()			uln2003_ledright_PORT->BRR = uln2003_ledright_MASK
#define uln2003_ledright_set_active()			uln2003_ledright_set_high()
#define uln2003_ledright_set_inactive()		uln2003_ledright_set_low()
#define uln2003_ledright_toggle()				uln2003_ledright_PORT->ODR ^= uln2003_ledright_MASK
#define uln2003_ledright_set_to(value)		if (value) { uln2003_ledright_set_active(); } else { uln2003_ledright_set_inactive(); }
#define uln2003_ledright_set_logic_to(value)	if (value) { uln2003_ledright_set_high(); } else { uln2003_ledright_set_low(); }
#define uln2003_ledright_get()				((uln2003_ledright_PORT->IDR >> uln2003_ledright_PIN) & 1)
#define uln2003_ledright_is_high()			(uln2003_ledright_get() != 0)
#define uln2003_ledright_is_low()				(uln2003_ledright_get() == 0)
#define uln2003_ledright_is_active()			uln2003_ledright_is_high()
#define uln2003_ledright_is_inactive()		uln2003_ledright_is_low()

// uln2003_emergencylights: PA3, mode = OutputPushPull
#define uln2003_emergencylights_PORT					GPIOA
#define uln2003_emergencylights_PIN					3
#define uln2003_emergencylights_MASK					(1 << uln2003_emergencylights_PIN)
#define uln2003_emergencylights_set_high()			uln2003_emergencylights_PORT->BSRR = uln2003_emergencylights_MASK
#define uln2003_emergencylights_set_low()			uln2003_emergencylights_PORT->BRR = uln2003_emergencylights_MASK
#define uln2003_emergencylights_set_active()			uln2003_emergencylights_set_high()
#define uln2003_emergencylights_set_inactive()		uln2003_emergencylights_set_low()
#define uln2003_emergencylights_toggle()				uln2003_emergencylights_PORT->ODR ^= uln2003_emergencylights_MASK
#define uln2003_emergencylights_set_to(value)		if (value) { uln2003_emergencylights_set_active(); } else { uln2003_emergencylights_set_inactive(); }
#define uln2003_emergencylights_set_logic_to(value)	if (value) { uln2003_emergencylights_set_high(); } else { uln2003_emergencylights_set_low(); }
#define uln2003_emergencylights_get()				((uln2003_emergencylights_PORT->IDR >> uln2003_emergencylights_PIN) & 1)
#define uln2003_emergencylights_is_high()			(uln2003_emergencylights_get() != 0)
#define uln2003_emergencylights_is_low()				(uln2003_emergencylights_get() == 0)
#define uln2003_emergencylights_is_active()			uln2003_emergencylights_is_high()
#define uln2003_emergencylights_is_inactive()		uln2003_emergencylights_is_low()

// w25qxx_cs: PA4, mode = OutputPushPull, inverted, alternate function SPI1
#define w25qxx_cs_PORT					GPIOA
#define w25qxx_cs_PIN					4
#define w25qxx_cs_MASK					(1 << w25qxx_cs_PIN)
#define w25qxx_cs_set_high()			w25qxx_cs_PORT->BSRR = w25qxx_cs_MASK
#define w25qxx_cs_set_low()			w25qxx_cs_PORT->BRR = w25qxx_cs_MASK
#define w25qxx_cs_set_active()			w25qxx_cs_set_low()
#define w25qxx_cs_set_inactive()		w25qxx_cs_set_high()
#define w25qxx_cs_toggle()				w25qxx_cs_PORT->ODR ^= w25qxx_cs_MASK
#define w25qxx_cs_set_to(value)		if (value) { w25qxx_cs_set_active(); } else { w25qxx_cs_set_inactive(); }
#define w25qxx_cs_set_logic_to(value)	if (value) { w25qxx_cs_set_high(); } else { w25qxx_cs_set_low(); }
#define w25qxx_cs_get()				((w25qxx_cs_PORT->IDR >> w25qxx_cs_PIN) & 1)
#define w25qxx_cs_is_high()			(w25qxx_cs_get() != 0)
#define w25qxx_cs_is_low()				(w25qxx_cs_get() == 0)
#define w25qxx_cs_is_active()			w25qxx_cs_is_low()
#define w25qxx_cs_is_inactive()		w25qxx_cs_is_high()

// w25qxx_sck: PA5, mode = AlternateFunction, alternate function SPI1
#define w25qxx_sck_PORT					GPIOA
#define w25qxx_sck_PIN					5
#define w25qxx_sck_MASK					(1 << w25qxx_sck_PIN)
#define w25qxx_sck_get()				((w25qxx_sck_PORT->IDR >> w25qxx_sck_PIN) & 1)
#define w25qxx_sck_is_high()			(w25qxx_sck_get() != 0)
#define w25qxx_sck_is_low()				(w25qxx_sck_get() == 0)
#define w25qxx_sck_is_active()			w25qxx_sck_is_high()
#define w25qxx_sck_is_inactive()		w25qxx_sck_is_low()

// w25qxx_miso: PA6, mode = InputFloat, alternate function SPI1
#define w25qxx_miso_PORT					GPIOA
#define w25qxx_miso_PIN					6
#define w25qxx_miso_MASK					(1 << w25qxx_miso_PIN)
#define w25qxx_miso_get()				((w25qxx_miso_PORT->IDR >> w25qxx_miso_PIN) & 1)
#define w25qxx_miso_is_high()			(w25qxx_miso_get() != 0)
#define w25qxx_miso_is_low()				(w25qxx_miso_get() == 0)
#define w25qxx_miso_is_active()			w25qxx_miso_is_high()
#define w25qxx_miso_is_inactive()		w25qxx_miso_is_low()

// w25qxx_mosi: PA7, mode = AlternateFunction, alternate function SPI1
#define w25qxx_mosi_PORT					GPIOA
#define w25qxx_mosi_PIN					7
#define w25qxx_mosi_MASK					(1 << w25qxx_mosi_PIN)
#define w25qxx_mosi_get()				((w25qxx_mosi_PORT->IDR >> w25qxx_mosi_PIN) & 1)
#define w25qxx_mosi_is_high()			(w25qxx_mosi_get() != 0)
#define w25qxx_mosi_is_low()				(w25qxx_mosi_get() == 0)
#define w25qxx_mosi_is_active()			w25qxx_mosi_is_high()
#define w25qxx_mosi_is_inactive()		w25qxx_mosi_is_low()

// audio_pwm: PA8, mode = AlternateFunction, speed 50 MHz, alternate function TIM1
#define audio_pwm_PORT					GPIOA
#define audio_pwm_PIN					8
#define audio_pwm_MASK					(1 << audio_pwm_PIN)
#define audio_pwm_get()				((audio_pwm_PORT->IDR >> audio_pwm_PIN) & 1)
#define audio_pwm_is_high()			(audio_pwm_get() != 0)
#define audio_pwm_is_low()				(audio_pwm_get() == 0)
#define audio_pwm_is_active()			audio_pwm_is_high()
#define audio_pwm_is_inactive()		audio_pwm_is_low()

// usart_tx: PA9, mode = AlternateFunction, alternate function USART1
#define usart_tx_PORT					GPIOA
#define usart_tx_PIN					9
#define usart_tx_MASK					(1 << usart_tx_PIN)
#define usart_tx_get()				((usart_tx_PORT->IDR >> usart_tx_PIN) & 1)
#define usart_tx_is_high()			(usart_tx_get() != 0)
#define usart_tx_is_low()				(usart_tx_get() == 0)
#define usart_tx_is_active()			usart_tx_is_high()
#define usart_tx_is_inactive()		usart_tx_is_low()

// usart_rx: PA10, mode = InputFloat, alternate function USART1
#define usart_rx_PORT					GPIOA
#define usart_rx_PIN					10
#define usart_rx_MASK					(1 << usart_rx_PIN)
#define usart_rx_get()				((usart_rx_PORT->IDR >> usart_rx_PIN) & 1)
#define usart_rx_is_high()			(usart_rx_get() != 0)
#define usart_rx_is_low()				(usart_rx_get() == 0)
#define usart_rx_is_active()			usart_rx_is_high()
#define usart_rx_is_inactive()		usart_rx_is_low()

// ignition_crank: PA11, mode = InputPullup, inverted
#define ignition_crank_PORT					GPIOA
#define ignition_crank_PIN					11
#define ignition_crank_MASK					(1 << ignition_crank_PIN)
#define ignition_crank_get()				((ignition_crank_PORT->IDR >> ignition_crank_PIN) & 1)
#define ignition_crank_is_high()			(ignition_crank_get() != 0)
#define ignition_crank_is_low()				(ignition_crank_get() == 0)
#define ignition_crank_is_active()			ignition_crank_is_low()
#define ignition_crank_is_inactive()		ignition_crank_is_high()

// led_red: PB0, mode = OutputPushPull
#define led_red_PORT					GPIOB
#define led_red_PIN					0
#define led_red_MASK					(1 << led_red_PIN)
#define led_red_set_high()			led_red_PORT->BSRR = led_red_MASK
#define led_red_set_low()			led_red_PORT->BRR = led_red_MASK
#define led_red_set_active()			led_red_set_high()
#define led_red_set_inactive()		led_red_set_low()
#define led_red_toggle()				led_red_PORT->ODR ^= led_red_MASK
#define led_red_set_to(value)		if (value) { led_red_set_active(); } else { led_red_set_inactive(); }
#define led_red_set_logic_to(value)	if (value) { led_red_set_high(); } else { led_red_set_low(); }
#define led_red_get()				((led_red_PORT->IDR >> led_red_PIN) & 1)
#define led_red_is_high()			(led_red_get() != 0)
#define led_red_is_low()				(led_red_get() == 0)
#define led_red_is_active()			led_red_is_high()
#define led_red_is_inactive()		led_red_is_low()

// led_yellow: PB1, mode = OutputPushPull
#define led_yellow_PORT					GPIOB
#define led_yellow_PIN					1
#define led_yellow_MASK					(1 << led_yellow_PIN)
#define led_yellow_set_high()			led_yellow_PORT->BSRR = led_yellow_MASK
#define led_yellow_set_low()			led_yellow_PORT->BRR = led_yellow_MASK
#define led_yellow_set_active()			led_yellow_set_high()
#define led_yellow_set_inactive()		led_yellow_set_low()
#define led_yellow_toggle()				led_yellow_PORT->ODR ^= led_yellow_MASK
#define led_yellow_set_to(value)		if (value) { led_yellow_set_active(); } else { led_yellow_set_inactive(); }
#define led_yellow_set_logic_to(value)	if (value) { led_yellow_set_high(); } else { led_yellow_set_low(); }
#define led_yellow_get()				((led_yellow_PORT->IDR >> led_yellow_PIN) & 1)
#define led_yellow_is_high()			(led_yellow_get() != 0)
#define led_yellow_is_low()				(led_yellow_get() == 0)
#define led_yellow_is_active()			led_yellow_is_high()
#define led_yellow_is_inactive()		led_yellow_is_low()

// led_green: PB2, mode = OutputPushPull
#define led_green_PORT					GPIOB
#define led_green_PIN					2
#define led_green_MASK					(1 << led_green_PIN)
#define led_green_set_high()			led_green_PORT->BSRR = led_green_MASK
#define led_green_set_low()			led_green_PORT->BRR = led_green_MASK
#define led_green_set_active()			led_green_set_high()
#define led_green_set_inactive()		led_green_set_low()
#define led_green_toggle()				led_green_PORT->ODR ^= led_green_MASK
#define led_green_set_to(value)		if (value) { led_green_set_active(); } else { led_green_set_inactive(); }
#define led_green_set_logic_to(value)	if (value) { led_green_set_high(); } else { led_green_set_low(); }
#define led_green_get()				((led_green_PORT->IDR >> led_green_PIN) & 1)
#define led_green_is_high()			(led_green_get() != 0)
#define led_green_is_low()				(led_green_get() == 0)
#define led_green_is_active()			led_green_is_high()
#define led_green_is_inactive()		led_green_is_low()

// led_siren: PB4, mode = OutputPushPull
#define led_siren_PORT					GPIOB
#define led_siren_PIN					4
#define led_siren_MASK					(1 << led_siren_PIN)
#define led_siren_set_high()			led_siren_PORT->BSRR = led_siren_MASK
#define led_siren_set_low()			led_siren_PORT->BRR = led_siren_MASK
#define led_siren_set_active()			led_siren_set_high()
#define led_siren_set_inactive()		led_siren_set_low()
#define led_siren_toggle()				led_siren_PORT->ODR ^= led_siren_MASK
#define led_siren_set_to(value)		if (value) { led_siren_set_active(); } else { led_siren_set_inactive(); }
#define led_siren_set_logic_to(value)	if (value) { led_siren_set_high(); } else { led_siren_set_low(); }
#define led_siren_get()				((led_siren_PORT->IDR >> led_siren_PIN) & 1)
#define led_siren_is_high()			(led_siren_get() != 0)
#define led_siren_is_low()				(led_siren_get() == 0)
#define led_siren_is_active()			led_siren_is_high()
#define led_siren_is_inactive()		led_siren_is_low()

// button_siren: PB5, mode = InputPullup, inverted
#define button_siren_PORT					GPIOB
#define button_siren_PIN					5
#define button_siren_MASK					(1 << button_siren_PIN)
#define button_siren_get()				((button_siren_PORT->IDR >> button_siren_PIN) & 1)
#define button_siren_is_high()			(button_siren_get() != 0)
#define button_siren_is_low()				(button_siren_get() == 0)
#define button_siren_is_active()			button_siren_is_low()
#define button_siren_is_inactive()		button_siren_is_high()

// button_left: PB6, mode = InputPullup, inverted
#define button_left_PORT					GPIOB
#define button_left_PIN					6
#define button_left_MASK					(1 << button_left_PIN)
#define button_left_get()				((button_left_PORT->IDR >> button_left_PIN) & 1)
#define button_left_is_high()			(button_left_get() != 0)
#define button_left_is_low()				(button_left_get() == 0)
#define button_left_is_active()			button_left_is_low()
#define button_left_is_inactive()		button_left_is_high()

// button_right: PB7, mode = InputPullup, inverted
#define button_right_PORT					GPIOB
#define button_right_PIN					7
#define button_right_MASK					(1 << button_right_PIN)
#define button_right_get()				((button_right_PORT->IDR >> button_right_PIN) & 1)
#define button_right_is_high()			(button_right_get() != 0)
#define button_right_is_low()				(button_right_get() == 0)
#define button_right_is_active()			button_right_is_low()
#define button_right_is_inactive()		button_right_is_high()

// button_parent: PB8, mode = InputPullup, inverted
#define button_parent_PORT					GPIOB
#define button_parent_PIN					8
#define button_parent_MASK					(1 << button_parent_PIN)
#define button_parent_get()				((button_parent_PORT->IDR >> button_parent_PIN) & 1)
#define button_parent_is_high()			(button_parent_get() != 0)
#define button_parent_is_low()				(button_parent_get() == 0)
#define button_parent_is_active()			button_parent_is_low()
#define button_parent_is_inactive()		button_parent_is_high()

// ignition_on: PB9, mode = InputPullup, inverted
#define ignition_on_PORT					GPIOB
#define ignition_on_PIN					9
#define ignition_on_MASK					(1 << ignition_on_PIN)
#define ignition_on_get()				((ignition_on_PORT->IDR >> ignition_on_PIN) & 1)
#define ignition_on_is_high()			(ignition_on_get() != 0)
#define ignition_on_is_low()				(ignition_on_get() == 0)
#define ignition_on_is_active()			ignition_on_is_low()
#define ignition_on_is_inactive()		ignition_on_is_high()

// ignition_ccw: PB10, mode = InputPullup, inverted
#define ignition_ccw_PORT					GPIOB
#define ignition_ccw_PIN					10
#define ignition_ccw_MASK					(1 << ignition_ccw_PIN)
#define ignition_ccw_get()				((ignition_ccw_PORT->IDR >> ignition_ccw_PIN) & 1)
#define ignition_ccw_is_high()			(ignition_ccw_get() != 0)
#define ignition_ccw_is_low()				(ignition_ccw_get() == 0)
#define ignition_ccw_is_active()			ignition_ccw_is_low()
#define ignition_ccw_is_inactive()		ignition_ccw_is_high()

// sleep: PB11, mode = OutputPushPull
#define sleep_PORT					GPIOB
#define sleep_PIN					11
#define sleep_MASK					(1 << sleep_PIN)
#define sleep_set_high()			sleep_PORT->BSRR = sleep_MASK
#define sleep_set_low()			sleep_PORT->BRR = sleep_MASK
#define sleep_set_active()			sleep_set_high()
#define sleep_set_inactive()		sleep_set_low()
#define sleep_toggle()				sleep_PORT->ODR ^= sleep_MASK
#define sleep_set_to(value)		if (value) { sleep_set_active(); } else { sleep_set_inactive(); }
#define sleep_set_logic_to(value)	if (value) { sleep_set_high(); } else { sleep_set_low(); }
#define sleep_get()				((sleep_PORT->IDR >> sleep_PIN) & 1)
#define sleep_is_high()			(sleep_get() != 0)
#define sleep_is_low()				(sleep_get() == 0)
#define sleep_is_active()			sleep_is_high()
#define sleep_is_inactive()		sleep_is_low()

// ws2812: PB13, mode = OutputPushPull, inverted
#define ws2812_PORT					GPIOB
#define ws2812_PIN					13
#define ws2812_MASK					(1 << ws2812_PIN)
#define ws2812_set_high()			ws2812_PORT->BSRR = ws2812_MASK
#define ws2812_set_low()			ws2812_PORT->BRR = ws2812_MASK
#define ws2812_set_active()			ws2812_set_low()
#define ws2812_set_inactive()		ws2812_set_high()
#define ws2812_toggle()				ws2812_PORT->ODR ^= ws2812_MASK
#define ws2812_set_to(value)		if (value) { ws2812_set_active(); } else { ws2812_set_inactive(); }
#define ws2812_set_logic_to(value)	if (value) { ws2812_set_high(); } else { ws2812_set_low(); }
#define ws2812_get()				((ws2812_PORT->IDR >> ws2812_PIN) & 1)
#define ws2812_is_high()			(ws2812_get() != 0)
#define ws2812_is_low()				(ws2812_get() == 0)
#define ws2812_is_active()			ws2812_is_low()
#define ws2812_is_inactive()		ws2812_is_high()

// turn_off: PB15, mode = OutputPushPull
#define turn_off_PORT					GPIOB
#define turn_off_PIN					15
#define turn_off_MASK					(1 << turn_off_PIN)
#define turn_off_set_high()			turn_off_PORT->BSRR = turn_off_MASK
#define turn_off_set_low()			turn_off_PORT->BRR = turn_off_MASK
#define turn_off_set_active()			turn_off_set_high()
#define turn_off_set_inactive()		turn_off_set_low()
#define turn_off_toggle()				turn_off_PORT->ODR ^= turn_off_MASK
#define turn_off_set_to(value)		if (value) { turn_off_set_active(); } else { turn_off_set_inactive(); }
#define turn_off_set_logic_to(value)	if (value) { turn_off_set_high(); } else { turn_off_set_low(); }
#define turn_off_get()				((turn_off_PORT->IDR >> turn_off_PIN) & 1)
#define turn_off_is_high()			(turn_off_get() != 0)
#define turn_off_is_low()				(turn_off_get() == 0)
#define turn_off_is_active()			turn_off_is_high()
#define turn_off_is_inactive()		turn_off_is_low()


void default_fault_handler(void);
void early_system_init(void);
void clock_switch_hse_pll(void);
void clock_switch_hsi(void);

#endif
