/*
 * Flash.h
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#ifndef MY_LIBRARY_INC_FLASH_H_
#define MY_LIBRARY_INC_FLASH_H_

#include "main.h"

#define FLASH_ADDR_BASE 0x40023C00

void flash_erase_sector(uint8_t sector);
void flash_program(char* address, char* buff, uint32_t size);

#endif /* MY_LIBRARY_INC_FLASH_H_ */
