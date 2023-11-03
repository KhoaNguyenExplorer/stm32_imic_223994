/*
 * AD_converter.h
 *
 *  Created on: Oct 27, 2023
 *      Author: Admin
 */

#ifndef MY_LIBRARY_INC_AD_CONVERTER_H_
#define MY_LIBRARY_INC_AD_CONVERTER_H_

#include "main.h"

#define ADC1_ADDR_BASE  0x40012000
#define V25             760
#define Avg_Slope       2.5

void adc1_init();
int adc1_vin_measure();
float adc1_get_temp();

#endif /* MY_LIBRARY_INC_AD_CONVERTER_H_ */