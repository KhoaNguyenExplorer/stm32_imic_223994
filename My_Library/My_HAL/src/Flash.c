/*
 * Flash.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#include "Flash.h"

__attribute__((section(".FunctionsOnRAM"))) void flash_erase_sector(uint8_t sector)
{
    uint32_t* FLASH_SR = (uint32_t*)(FLASH_ADDR_BASE + 0x0C);
    uint32_t* FLASH_CR = (uint32_t*)(FLASH_ADDR_BASE + 0x10);
    uint32_t* FLASH_KEYR = (uint32_t*)(FLASH_ADDR_BASE + 0x04);

    // 0. Unlock the CR register
    if(((*FLASH_CR >> 31) & 1) == 1)
    {
        // 1. Write KEY1 = 0x45670123 in the Flash key register (FLASH_KEYR)
        *FLASH_KEYR = 0x45670123;

        // 2. Write KEY2 = 0xCDEF89AB in the Flash key register (FLASH_KEYR)
        *FLASH_KEYR = 0xCDEF89AB;
    }

    // 1.Check that no Flash memory operation is ongoing by checking the BSY bit in the
    // FLASH_SR register
    while(((*FLASH_SR >> 16) & 1) == 1);      // wait until BSY bit in SR register is 0 - empty

    // 2. Set the SER bit and select the sector out of the 7 sectors (STM32F411xC/E) in the
    // main memory block you wish to erase (SNB) in the FLASH_CR register
    *FLASH_CR |= (1 << 1);
	*FLASH_CR |= (sector << 3);

    // 3. Set the STRT bit in the FLASH_CR register
    *FLASH_CR |= (1 << 16);

    // 4. Wait for the BSY bit to be cleared
    while(((*FLASH_SR >> 16) & 1) != 1);

    // 5. 
    *FLASH_CR &= ~(1 << 1);
}

__attribute__((section(".FunctionsOnRAM"))) void flash_program(char* address, char* buff, uint32_t size)
{
    uint32_t* FLASH_SR = (uint32_t*)(FLASH_ADDR_BASE + 0x0C);
    uint32_t* FLASH_CR = (uint32_t*)(FLASH_ADDR_BASE + 0x10);
    uint32_t* FLASH_KEYR = (uint32_t*)(FLASH_ADDR_BASE + 0x04);

    // 0. Unlock the CR register
    if(((*FLASH_CR >> 31) & 1) == 1)
    {
        // 1. Write KEY1 = 0x45670123 in the Flash key register (FLASH_KEYR)
        *FLASH_KEYR = 0x45670123;

        // 2. Write KEY2 = 0xCDEF89AB in the Flash key register (FLASH_KEYR)
        *FLASH_KEYR = 0xCDEF89AB;
    }

    // 1. Check that no main Flash memory operation is ongoing by checking the BSY bit in the
    // FLASH_SR register.
    while(((*FLASH_SR >> 16) & 1) == 1);      // wait until BSY bit in SR register is 0 - empty

    // 2. Set the PG bit in the FLASH_CR register
    *FLASH_CR |= (1 << 0);

    // 3. Perform the data write operation(s) to the desired memory address (inside main
    // memory block or OTP area):
    // – Byte access in case of x8 parallelism
    // – Half-word access in case of x16 parallelism
    // – Word access in case of x32 parallelism
    // – Double word access in case of x64 parallelism
    for (int i = 0; i < size; i++)
    {
        address[i] = buff[i];     // write from the indicated memory into flash
    }

    // 4. Wait for the BSY bit to be cleared.
    while(((*FLASH_SR >> 16) & 1) == 1);
}


