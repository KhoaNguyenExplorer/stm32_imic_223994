/*
 * PWM.h
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#ifndef PWM_H_
#define PWM_H_

#include "main.h"
#include "Peripheral_LED.h"

void tim4_ch1_pwm();
void tim4_ch2_pwm();
void tim4_ch3_pwm();
void tim4_ch4_pwm();

void update_pulse_width_ch1(char width);
void update_pulse_width_ch2(char width);
void update_pulse_width_ch3(char width);
void update_pulse_width_ch4(char width);


#endif /* PWM_H_ */
