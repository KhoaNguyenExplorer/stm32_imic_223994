/*
 * Peripheral_LED.h
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#ifndef MY_LIBRARY_INC_PERIPHERAL_LED_H_
#define MY_LIBRARY_INC_PERIPHERAL_LED_H_

#include "main.h"

typedef enum
{
	LED_GREEN = 12,
	LED_ORANGE,
	LED_RED,
	LED_BLUE
} led_postion_t;

typedef enum
{
	LED_RESET,
	LED_SET
} led_state_t;
void LED_init();
void LED_ctrl(uint8_t led_color, uint8_t led_state);
void LED_ctrl_uart(char* rx_buf, uint32_t rx_index);
void LED_toggle(uint8_t led_color);

#endif /* MY_LIBRARY_INC_PERIPHERAL_LED_H_ */
