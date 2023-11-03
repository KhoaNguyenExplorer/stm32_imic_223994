/*
 * Clock.c
 *
 *  Created on: Oct 18, 2023
 *      Author: Admin
 */

#include "Clock.h"

void clock_init()
{
    uint32_t* RCC_CR        = (uint32_t*)(RCC_ADDR_BASE + 0x00);
    uint32_t* RCC_PLLCFGR   = (uint32_t*)(RCC_ADDR_BASE + 0x04);    
    uint32_t* RCC_CFGR      = (uint32_t*)(RCC_ADDR_BASE + 0x08);      

    *RCC_CR |= (1 << 16);                                   // 1. enable HSE
    while (((*RCC_CR << 17) & 1) != 1);                     // 2. wait until HSE is ready

    *RCC_PLLCFGR |= (1 << 22);                              // 3. select HSE as PLL source
    *RCC_PLLCFGR |= (8 << 0) | (200 << 6) | (0b00 << 16);   // 4. set M = 8, N = 200, P = 2

    *RCC_CR |= (1 << 24);                                   // 5. enable PLL
    while (((*RCC_CR << 25) & 1) != 1);                     // 6. wait until PLL is ready    
    
    /* WARNING!!! */
    /* The FLASH's LATENCY must be configured according to fSYSCLK  */
    uint32_t* FLASH_ACR = (uint32_t*)(FLASH_ADDR_BASE + 0x00);    
    *FLASH_ACR |= (3 << 0);

    *RCC_CFGR |= (0b100 << 10);                             // set APB1 prescaler = /2 so that its input frequency will be 50MHz

    *RCC_CFGR |= (0b10 << 0);                               // 7. select PLL as SYSCLK
}

void clock_AHB1_enable(AHB1_CLK_Peripheral_t AHB1_Peripheral)
{
    uint32_t* RCC_AHB1ENR = (uint32_t*)(RCC_ADDR_BASE + 0x30);
    *RCC_AHB1ENR |= (1 << AHB1_Peripheral);
}

void clock_APB1_enable(APB1_CLK_Peripheral_t APB1_Peripheral)
{
    uint32_t* RCC_APB1ENR = (uint32_t*)(RCC_ADDR_BASE + 0x40);
    *RCC_APB1ENR |= (1 << APB1_Peripheral);
}

void clock_APB2_enable(APB2_CLK_Peripheral_t APB2_Peripheral)
{
    uint32_t* RCC_APB2ENR = (uint32_t*)(RCC_ADDR_BASE + 0x44);
    *RCC_APB2ENR |= (1 << APB2_Peripheral);
}
