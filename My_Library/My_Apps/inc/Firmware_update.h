/*
 * Firmware_update.h
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#ifndef MY_LIBRARY_MY_APPS_INC_FIRMWARE_UPDATE_H_
#define MY_LIBRARY_MY_APPS_INC_FIRMWARE_UPDATE_H_

#include "main.h"
#include "Flash.h"
#include "Uart.h"
#include "Peripheral_button.h"
#include "Dma.h"

#define APP_START_ADDR  0x08008000  // sector 2 of FLASH memory

extern char rx_buf[5792];

void bootloader();
void update_firmware();
void jump_to_app();

#endif /* MY_LIBRARY_MY_APPS_INC_FIRMWARE_UPDATE_H_ */
