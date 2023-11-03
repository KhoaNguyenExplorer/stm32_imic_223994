/*
 * Timer.h
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#ifndef MY_LIBRARY_INC_TIMER_H_
#define MY_LIBRARY_INC_TIMER_H_

#include "main.h"

#define TIM1_ADDR_BASE 0x40010000

void tim1_init();
void delay(uint64_t time_ms);

#endif /* MY_LIBRARY_INC_TIMER_H_ */
