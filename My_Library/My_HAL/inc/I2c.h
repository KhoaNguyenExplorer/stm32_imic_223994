/*
 * I2c.h
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#ifndef MY_LIBRARY_INC_I2C_H_
#define MY_LIBRARY_INC_I2C_H_

#include "main.h"

#define ACCEL_BASE_ADDR     0b0011001
#define LSM303AGR_ID        0x0F

#define I2C_WRITE_BIT       0
#define I2C_READ_BIT        1

void i2c1_init();
void i2c1_write(uint8_t data_type, uint8_t data_write);
uint8_t i2c1_read(uint8_t data_type, uint8_t* data_read);

#endif /* MY_LIBRARY_INC_I2C_H_ */
