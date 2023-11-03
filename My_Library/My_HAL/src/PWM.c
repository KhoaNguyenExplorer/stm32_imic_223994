/*
 * PWM.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#include "PWM.h"

int cnt = 0;

void tim4_ch1_pwm()     // For PD12
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	uint32_t* GPIOD_MODER = (uint32_t*)(0x40020c00);
	*GPIOD_MODER &= ~(0b11 << (LED_GREEN * 2));
	*GPIOD_MODER |= (0b10 << (LED_GREEN * 2));  					// Set up pin D as alternate function
	uint32_t* GPIOD_AFRH = (uint32_t*)(0x40020c24);
	*GPIOD_AFRH |= (0b0010 << 16);

	__HAL_RCC_TIM4_CLK_ENABLE();
    uint16_t* TIM4_CR1 = (uint16_t*)(0x40000800);
    uint16_t* TIM4_PSC = (uint16_t*)(0x40000828);
    uint16_t* TIM4_ARR = (uint16_t*)(0x4000082c);

    *TIM4_PSC = 16 - 1;
    *TIM4_ARR = 100;

    uint16_t* TIM4_CCR1 = (uint16_t*)(0x40000834);	//TIM4 outputs CH1
    *TIM4_CCR1 = 0;

    uint16_t* TIM4_CCMR1 = (uint16_t*)(0x40000818);
    *TIM4_CCMR1 &= ~(0b11 << 0);					//TIM4 CH1 is configured as output
    *TIM4_CCMR1 |= (0b110 << 4);					//upcounting mode is selected

    uint16_t* TIM4_CCER = (uint16_t*)(0x40000820);	//TIM4 output compare enabling
    *TIM4_CCER |= 1;

    *TIM4_CR1 |= 1;
}

void tim4_ch2_pwm()     // For PD13
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	uint32_t* GPIOD_MODER = (uint32_t*)(0x40020c00);
	*GPIOD_MODER &= ~(0b11 << (LED_ORANGE * 2));
	*GPIOD_MODER |= (0b10 << (LED_ORANGE * 2));  					// Set up pin D as alternate function
	uint32_t* GPIOD_AFRH = (uint32_t*)(0x40020c24);
	*GPIOD_AFRH |= (0b0010 << 20);

	__HAL_RCC_TIM4_CLK_ENABLE();
    uint16_t* TIM4_CR1 = (uint16_t*)(0x40000800);
    uint16_t* TIM4_PSC = (uint16_t*)(0x40000828);
    uint16_t* TIM4_ARR = (uint16_t*)(0x4000082c);

    *TIM4_PSC = 16 - 1;
    *TIM4_ARR = 100;

    uint16_t* TIM4_CCR2 = (uint16_t*)(0x40000838);
    *TIM4_CCR2 = 0;

    uint16_t* TIM4_CCMR1 = (uint16_t*)(0x40000818);
    *TIM4_CCMR1 &= ~(0b11 << 8);					//TIM4 CH2 is configured as output
    *TIM4_CCMR1 |= (0b110 << 12);					//upcounting mode is selected

    uint16_t* TIM4_CCER = (uint16_t*)(0x40000820);	//TIM4 output compare enabling
    *TIM4_CCER |= (1 << 4);

    *TIM4_CR1 |= 1;
}

void tim4_ch3_pwm()     // For PD14
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	uint32_t* GPIOD_MODER = (uint32_t*)(0x40020c00);
	*GPIOD_MODER &= ~(0b11 << (LED_RED * 2));
	*GPIOD_MODER |= (0b10 << (LED_RED * 2));  					// Set up pin D as alternate function
	uint32_t* GPIOD_AFRH = (uint32_t*)(0x40020c24);
	*GPIOD_AFRH |= (0b0010 << 24);

	__HAL_RCC_TIM4_CLK_ENABLE();
    uint16_t* TIM4_CR1 = (uint16_t*)(0x40000800);
    uint16_t* TIM4_PSC = (uint16_t*)(0x40000828);
    uint16_t* TIM4_ARR = (uint16_t*)(0x4000082c);

    *TIM4_PSC = 16 - 1;
    *TIM4_ARR = 100;

    uint16_t* TIM4_CCR3 = (uint16_t*)(0x4000083C);
    *TIM4_CCR3 = 0;

    uint16_t* TIM4_CCMR2 = (uint16_t*)(0x4000081C);
    *TIM4_CCMR2 &= ~(0b11 << 0);					//TIM4 CH3 is configured as output
    *TIM4_CCMR2 |= (0b110 << 4);					//upcounting mode is selected

    uint16_t* TIM4_CCER = (uint16_t*)(0x40000820);	//TIM4 output compare enabling
    *TIM4_CCER |= (1 << 8);

    *TIM4_CR1 |= 1;
}

void tim4_ch4_pwm()     // For PD15
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	uint32_t* GPIOD_MODER = (uint32_t*)(0x40020c00);
	*GPIOD_MODER &= ~(0b11 << (LED_BLUE * 2));
	*GPIOD_MODER |= (0b10 << (LED_BLUE * 2));  					// Set up pin D as alternate function
	uint32_t* GPIOD_AFRH = (uint32_t*)(0x40020c24);
	*GPIOD_AFRH |= (0b0010 << 28);

	__HAL_RCC_TIM4_CLK_ENABLE();
    uint16_t* TIM4_CR1 = (uint16_t*)(0x40000800);
    uint16_t* TIM4_PSC = (uint16_t*)(0x40000828);
    uint16_t* TIM4_ARR = (uint16_t*)(0x4000082c);

    *TIM4_PSC = 16 - 1;
    *TIM4_ARR = 100;

    uint16_t* TIM4_CCR4 = (uint16_t*)(0x40000840);
    *TIM4_CCR4 = 0;

    uint16_t* TIM4_CCMR2 = (uint16_t*)(0x4000081C);
    *TIM4_CCMR2 &= ~(0b11 << 8);					//TIM4 CH4 is configured as output
    *TIM4_CCMR2 |= (0b110 << 12);					//upcounting mode is selected

    uint16_t* TIM4_CCER = (uint16_t*)(0x40000820);	//TIM4 output compare enabling
    *TIM4_CCER |= (1 << 12);

    *TIM4_CR1 |= 1;
}

void update_pulse_width_ch1(char width)
{
	uint16_t* TIM4_CCR1 = (uint16_t*)(0x40000834);
	*TIM4_CCR1 = width;
}

void update_pulse_width_ch2(char width)
{
	uint16_t* TIM4_CCR2 = (uint16_t*)(0x40000838);
	*TIM4_CCR2 = width;
}

void update_pulse_width_ch3(char width)
{
	uint16_t* TIM4_CCR3 = (uint16_t*)(0x4000083C);
	*TIM4_CCR3 = width;
}

void update_pulse_width_ch4(char width)
{
	uint16_t* TIM4_CCR4 = (uint16_t*)(0x40000840);
	*TIM4_CCR4 = width;
}
