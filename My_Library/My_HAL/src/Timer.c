/*
 * Timer.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#include "Timer.h"

uint64_t tim1_cnt = 0;

void tim1_init()
{
    // 1 milisec
    // auto-reload: 1000
    // pre-scaler: 16 (15)

    // Implementation step (for timer):
    // Provide clock source for timer, in this case, time source is RCC: 16MHz
    // Declare timer control register
    // Declare timer prescaler register
    // Declare timer auto-reload register
    // Enable counter
    // __HAL_RCC_TIM1_CLK_ENABLE();
    clock_APB2_enable(TIM1_CLK);
    uint16_t* TIM1_CR1 = (uint16_t*)(TIM1_ADDR_BASE + 0x00);
    uint16_t* TIM1_PSC = (uint16_t*)(TIM1_ADDR_BASE + 0x28);
    uint16_t* TIM1_ARR = (uint16_t*)(TIM1_ADDR_BASE + 0x2C);

    *TIM1_CR1 |= (1 << 0);
    *TIM1_PSC = 16 - 1;
    *TIM1_ARR = 1000;

    // Implementation step (for timer-generated interrupt):
    // Declare interrupt enable register
    // Register an interrupt event in NVIC

    uint16_t* TIM1_DIER = (uint16_t*)(TIM1_ADDR_BASE + 0x0C);
    *TIM1_DIER |= (1 << 0); // send signal to NVIC when interrupt event occurs

    uint32_t* ISER0 = (uint32_t*)(0xE000E100);
    *ISER0 |= (1 << 25);
}

void TIM1_UP_TIM10_IRQHandler()
{
    // Declare timer status flag (in interrupt)
    // increment the counter value by 1
    // clear update flag

    uint16_t* TIM1_SR = (uint16_t*)(TIM1_ADDR_BASE + 0x10);
    tim1_cnt++;
    *TIM1_SR &= ~(1 << 0);
}

void delay(uint64_t time_ms)
{
	while (tim1_cnt < time_ms);
	tim1_cnt = 0;
}

