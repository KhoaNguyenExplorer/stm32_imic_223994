/*
 * Peripheral_LED.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#include "Peripheral_LED.h"

void LED_init()
{
	// enable clock for GPIOD
	// __HAL_RCC_GPIOD_CLK_ENABLE();
	clock_AHB1_enable(GPIOD_CLK);
	// set PD12 -> PD15 as output mode
	uint32_t* GPIOD_MODER = (uint32_t*)(0x40020C00);
	*GPIOD_MODER &= ~(0xff << LED_GREEN * 2);
	*GPIOD_MODER |= (0b01 << LED_GREEN * 2) | (0b01 << LED_ORANGE * 2) \
				 | (0b01 << LED_RED * 2) | (0b01 << LED_BLUE * 2);
}

void LED_ctrl(uint8_t led_color, uint8_t led_state)
{
	uint32_t* GPIOD_ODR = (uint32_t*)(0x40020C14);
	if (led_state == LED_SET)
	{
		*GPIOD_ODR |= (1 << led_color);
	}
	else
	{
		*GPIOD_ODR &= ~(1 << led_color);
	}
}

void LED_ctrl_uart(char* rx_buf, uint32_t rx_index)
{
    if (strstr(rx_buf, "LED ON") != NULL)
    {
        LED_ctrl(LED_ORANGE, LED_SET);
        memset(rx_buf, 0, rx_index);
        rx_index = 0;
    }
    else if (strstr(rx_buf, "LED OFF") != NULL)
    {
        LED_ctrl(LED_ORANGE, LED_RESET);
        memset(rx_buf, 0, rx_index);
        rx_index = 0;
    }
}

void LED_toggle(uint8_t led_color)
{
	uint32_t* GPIOD_IDR = (uint32_t*)(0x40020C10);
	uint32_t* GPIOD_ODR = (uint32_t*)(0x40020C14);
	if ((*GPIOD_IDR >> led_color) & 1)		// reading the GPIOD bit using "1" as mask bit
	{
		*GPIOD_ODR &= ~(1 << led_color);	// turn LED OFF
	}
	else
	{
		*GPIOD_ODR |= (1 << led_color);		// turn LED ON
	}
}

