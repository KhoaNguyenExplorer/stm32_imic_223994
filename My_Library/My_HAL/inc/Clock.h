/*
 * Clock.h
 *
 *  Created on: Oct 18, 2023
 *      Author: Admin
 */

#ifndef MY_LIBRARY_INC_CLOCK_H_
#define MY_LIBRARY_INC_CLOCK_H_

#include "main.h"

#define RCC_ADDR_BASE  0x40023800

typedef enum 
{
    GPIOA_CLK = 0,
    GPIOB_CLK,
    GPIOC_CLK,
    GPIOD_CLK,
    GPIOE_CLK,
    GPIOH_CLK = 7,
    CRC_CLK = 12,
    DMA1_CLK = 21,
    DMA2_CLK
} AHB1_CLK_Peripheral_t;

typedef enum 
{
    TIM2_CLK = 0,
    TIM3_CLK,
    TIM4_CLK,
    TIM5_CLK,
    WWDG_CLK = 11,
    SPI2_CLK = 14,
    SPI3_CLK = 15,
    USART2_CLK = 17,
    I2C1_CLK = 21,
    I2C2_CLK,
    I2C3_CLK,
    PWR_CLK = 28
} APB1_CLK_Peripheral_t;

typedef enum 
{
    TIM1_CLK = 0,
    USART1_CLK = 4,
    USART6_CLK,
    ADC1_CLK = 8,
    SDIO_CLK = 11,
    SPI1_CLK,
    SPI4_CLK,
    SYSCFG_CLK,
    TIM9_CLK = 16,
    TIM10_CLK,
    TIM11_CLK,
    SPI5_CLK = 20
} APB2_CLK_Peripheral_t;

void clock_init(void);
void clock_AHB1_enable(AHB1_CLK_Peripheral_t AHB1_Peripheral);
void clock_APB1_enable(APB1_CLK_Peripheral_t APB1_Peripheral);
void clock_APB2_enable(APB2_CLK_Peripheral_t APB2_Peripheral);

#endif /* MY_LIBRARY_INC_CLOCK_H_ */