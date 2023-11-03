/*
 * Interrupt.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#include "Interrupt.h"

void exti0_init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	// Don't need to set input mode for PA0 because it's been in INPUT mode when powered on
	// Configure the Trigger selection bits of the interrupt lines by Rising edge via RTSR
	uint32_t* EXTI_RTSR = (uint32_t*)(0x40013C00 + 0x08);
	// Configure the mask bits to enable the interrupt request
	uint32_t* EXTI_IMR = (uint32_t*)(0x40013C00 +  0x00);

	*EXTI_RTSR |= (1 << 0);
	*EXTI_IMR |= (1 << 0);

	// Enabling the interrupt event at the 1st position for the button
	uint32_t* ISER0 = (uint32_t*)(0xE000E100 + 0x00);	//*
	*ISER0 |= (1 << 6); // position 6 in the interrupt vector table
}

void EXTI0_IRQHandler()
{
	__asm("NOP");
	uint32_t* EXTI_PR = (uint32_t*)(0x40013C00 + 0x14);
	*EXTI_PR |= (1 << 0);
}
