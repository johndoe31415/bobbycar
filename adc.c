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

#include <string.h>
#include <stm32f10x_adc.h>
#include "adc.h"

static uint16_t adc_sample_value(void) {
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}

static void adc_add_sample(struct adc_result_t *result) {
	const uint32_t sample_time = ADC_SampleTime_239Cycles5;
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, sample_time);
	result->adu_vext += adc_sample_value();
	ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint, 1, sample_time);
	result->adu_vrefint += adc_sample_value();
	result->sample_count += 1;
}

static void adc_average_samples(struct adc_result_t *result, unsigned int count) {
	memset(result, 0, sizeof(struct adc_result_t));
	for (unsigned int i = 0; i < count; i++) {
		adc_add_sample(result);
	}
	result->adu_vext = (result->adu_vext + (count / 2)) / count;
	result->adu_vrefint = (result->adu_vrefint + (count / 2)) / count;
	result->sample_count = 1;
}

uint32_t adc_get_ext_voltage_millivolts(void) {
	struct adc_result_t result;
	adc_average_samples(&result, 4);

	// ./intapprox.py -v -b 32 -m 4095 '1.2*1000/(3.9/(3.9+12))'
	//return result.adu_vext * 63600 / 13 / result.adu_vrefint;

	/* Actual calibration values taken into account */
	// ./intapprox.py -v -b 32 -m 4095 --fast-div '1.2*1000/(3.9/(3.9+12))*12/12.193'
	return result.adu_vext * 616303 / 128 / result.adu_vrefint;
}
