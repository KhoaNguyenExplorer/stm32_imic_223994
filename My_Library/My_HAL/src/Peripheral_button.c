/*
 * Peripheral_button.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#include "Peripheral_button.h"

void button_init()
{
	// enable clock for GPIOA
	// __HAL_RCC_GPIOA_CLK_ENABLE();
	clock_AHB1_enable(GPIOA_CLK);
	// set PA0 as input mode
	uint32_t* GPIOA_MODER = (uint32_t*)(0x40020000);
	*GPIOA_MODER &= ~(0b11 << 0);
}

char button_read()
{
	uint32_t* GPIOA_IDR = (uint32_t*)(0x40020010);
	char buttonState = *GPIOA_IDR & (1 << 0);

	return buttonState;
}

