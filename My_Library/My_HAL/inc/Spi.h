/*
 * Spi.h
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#ifndef MY_LIBRARY_INC_SPI_H_
#define MY_LIBRARY_INC_SPI_H_

#include "main.h"

#define WHO_AM_I        0x0f
#define CTRL_REG1       0x20
#define STATUS_REG      0x27
#define OUT_X_L       	0x28
#define OUT_X_H         0x29
#define OUT_Y_L       	0x2A
#define OUT_Y_H         0x2B
#define OUT_Z_L       	0x2C
#define OUT_Z_H         0x2D

typedef enum
{
	SLAVE_ACTIVE,
  SLAVE_INACTIVE
} spi_slave_state_t;

void spi1_init();
void spi_write(uint8_t data_type, uint8_t data);
uint8_t spi_read(uint8_t data_type);
void check_spi_active();
void read_gyro_through_spi();

#endif /* MY_LIBRARY_INC_SPI_H_ */
