/*
 * AD_converter.c
 *
 *  Created on: Oct 27, 2023
 *      Author: Admin
 */

#include "AD_converter.h"

void adc1_init()
{
    // enable clock for ADCx
    clock_APB2_enable(ADC1_CLK);

    uint32_t* ADC1_JSQR = (uint32_t*)(ADC1_ADDR_BASE + 0x38);
    uint32_t* ADC1_CR2  = (uint32_t*)(ADC1_ADDR_BASE + 0x08);
    uint32_t* ADC1_SMPR1 = (uint32_t*)(ADC1_ADDR_BASE + 0x0C);
    uint32_t* ADC1_CCR = (uint32_t*)(ADC1_ADDR_BASE + 0x300 + 0x04);

    *ADC1_CCR &= ~(1 << 22);		// disable VBAT channel
    *ADC1_CCR |= (1 << 23);         // enable temperature sensor

    *ADC1_SMPR1 |= 0b111 << 18;     // set 480 cycles sampling rate for channel 16
    *ADC1_JSQR &= ~(0b11 << 20);    // 00: 1 conversion
    *ADC1_JSQR |= 16 << 15;         // select channel 16 (temperature sensor)

    *ADC1_CR2 |= 1 << 0;            // enable ADC
}

int adc1_vin_measure()
{
    uint32_t* ADC1_CR2  = (uint32_t*)(ADC1_ADDR_BASE + 0x08);
    uint32_t* ADC1_SR   = (uint32_t*)(ADC1_ADDR_BASE + 0x00);
    uint32_t* ADC1_JDR1 = (uint32_t*)(ADC1_ADDR_BASE + 0x3C);
    uint16_t ADC1_DR;

    *ADC1_CR2 |= 1 << 22;               // start injected conversion
    while (((*ADC1_SR >> 2) & 1) != 1); // wait till the end of injected conversion
    ADC1_DR = (*ADC1_JDR1 & 0xfff);    // get first 12 bits only

    return (ADC1_DR * 3000) / 4095;
}

int adc1_get_temp()
{
    int Vin = adc1_vin_measure();
    // Temperature (in °C) = {(VSENSE – V25) / Avg_Slope} + 25
    int temperature = (int)((Vin - V25) / Avg_Slope) + 25;

    return temperature;
}
