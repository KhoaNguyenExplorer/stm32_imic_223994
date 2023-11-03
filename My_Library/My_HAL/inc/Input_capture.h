/*
 * Input_capture.h
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#ifndef MY_LIBRARY_INC_INPUT_CAPTURE_H_
#define MY_LIBRARY_INC_INPUT_CAPTURE_H_

#include "main.h"

void capture_init_pa0();
void capture_init_pa8();
uint32_t tim2_capture_frequency();
uint32_t tim2_capture_pulse_width();
uint16_t tim1_capture_frequency();
uint16_t tim1_capture_pulse_width();

#endif /* MY_LIBRARY_INC_INPUT_CAPTURE_H_ */
