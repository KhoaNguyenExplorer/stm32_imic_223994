/*
 * Input_capture.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#include "Input_capture.h"

void capture_init_pa0()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	uint32_t* GPIOA_MODER = (uint32_t*)(0x40020000);
	*GPIOA_MODER &= ~(0b11 << 0);
	*GPIOA_MODER |= (0b10 << 0);  					// Set up pin A0 as alternate function
	uint32_t* GPIOA_AFRL = (uint32_t*)(0x40020020);
	*GPIOA_AFRL |= (0b0001 << 0);

  __HAL_RCC_TIM2_CLK_ENABLE();
  uint32_t* TIM2_CR1 = (uint32_t*)(0x40000000);
  uint32_t* TIM2_PSC = (uint32_t*)(0x40000028);
  uint32_t* TIM2_ARR = (uint32_t*)(0x4000002C);
  uint32_t* TIM2_CCMR1 = (uint32_t*)(0x40000018);
  uint32_t* TIM2_CCER = (uint32_t*)(0x40000020);
  uint32_t* TIM2_SMCR = (uint32_t*)(0x40000008);

  *TIM2_ARR = 0xffffffff;   // TIM2 is 32-bit
  *TIM2_PSC = 16 - 1;

  *TIM2_CCMR1 = (0b01 << 0);     // set input mode for CH1

  *TIM2_CCER &= ~((1 << 1) | (1 << 3));   // select rising polarity
  *TIM2_CCER |= (1 << 0);                 // enable CH1

  *TIM2_CCMR1 = (0b10 << 8);     // set input mode for CH2

  // select falling polarity for channel 2
  *TIM2_CCER &= ~(1 << 7);
  *TIM2_CCER |= (1 << 5);
  *TIM2_CCER |= (1 << 4);                 // enable CH2

  *TIM2_SMCR |= (0b100 << 0) | (0b101 << 4);     // enable reset on

  *TIM2_CR1 |= (1 << 0);          // enable CNT for CH1
}

void capture_init_pa8()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	uint32_t* GPIOA_MODER = (uint32_t*)(0x40020000);
	*GPIOA_MODER &= ~(0b11 << 8);
	*GPIOA_MODER |= (0b10 << 8);  					// Set up pin A8 as alternate function
	uint32_t* GPIOA_AFRH = (uint32_t*)(0x40020024);
	*GPIOA_AFRH |= (0b0001 << 0);

  __HAL_RCC_TIM1_CLK_ENABLE();
  uint16_t* TIM1_CR1 = (uint16_t*)(0x40010000);
  uint16_t* TIM1_PSC = (uint16_t*)(0x40010028);
  uint16_t* TIM1_ARR = (uint16_t*)(0x4001002C);
  uint16_t* TIM1_CCMR1 = (uint16_t*)(0x40010018);
  uint16_t* TIM1_CCER = (uint16_t*)(0x40010020);
  uint16_t* TIM1_SMCR = (uint16_t*)(0x40010008);

  *TIM1_ARR = 0xffff;   // TIM1 is 16-bit
  *TIM1_PSC = 16 - 1;

  *TIM1_CCMR1 = (0b01 << 0);     // set input mode for CH1

  *TIM1_CCER &= ~((1 << 1) | (1 << 3));   // select rising polarity
  *TIM1_CCER |= (1 << 0);                 // enable CH1

  *TIM1_CCMR1 = (0b10 << 8);     // set input mode for CH2

  // select falling polarity for channel 2
  *TIM1_CCER &= ~(1 << 7);
  *TIM1_CCER |= (1 << 5);
  *TIM1_CCER |= (1 << 4);                 // enable CH2

  *TIM1_SMCR |= (0b100 << 0) | (0b101 << 4);     // enable reset on

  *TIM1_CR1 |= (1 << 0);          // enable CNT for CH1
}

uint32_t tim2_capture_frequency()
{
	uint32_t* TIM2_CCR1 = (uint32_t*)(0x40000034);
  return *TIM2_CCR1;
}

uint32_t tim2_capture_pulse_width()
{
	uint32_t* TIM2_CCR2 = (uint32_t*)(0x40000038);
  return *TIM2_CCR2;
}

uint16_t tim1_capture_frequency()
{
	uint16_t* TIM1_CCR1 = (uint16_t*)(0x40010034);
  return *TIM1_CCR1;
}

uint16_t tim1_capture_pulse_width()
{
	uint16_t* TIM1_CCR2 = (uint16_t*)(0x40010038);
  return *TIM1_CCR2;
}
