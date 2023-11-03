/*
 * Dma.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#include "Dma.h"

char firmware_transmit_complete_flag = 0;

extern char rx_buf[5792];

void dma2_init(uint32_t size)
{
    // For DMA2
    uint32_t* UART1_CR3 = (uint32_t*)(0x40011014);
    *UART1_CR3 |= (1 << 6);                         // enable DMA mode for reception

  	__HAL_RCC_DMA2_CLK_ENABLE();
    int stream_number = 2;
    uint32_t* DMA2_SxCR = (uint32_t*)(DMA2_ADDR_BASE + 0x10 + 0x18 * stream_number);
    uint32_t* DMA2_SxNDTR = (uint32_t*)(DMA2_ADDR_BASE + 0x14 + 0x18 * stream_number);
    uint32_t* DMA2_SxPAR = (uint32_t*)(DMA2_ADDR_BASE + 0x18 + 0x18 * stream_number);
    uint32_t* DMA2_SxM0AR = (uint32_t*)(DMA2_ADDR_BASE + 0x1C + 0x18 * stream_number);

    *DMA2_SxCR &= ~(1 << 0);   // we have to firstly disable the DMA before configuration
    *DMA2_SxCR |= (4 << 25) | (1 << 10)  | (1 << 4);  // we use channel 4, according to the DMA table
                                                      // use memory increment mode
                                                      // enable circular mode (write 1 to 8th bit)

    *DMA2_SxNDTR = size;                                // when it has receives this number of byte, an interrupt will be triggered
    *DMA2_SxPAR = 0x40011004;                           // address of the USART data register (DR) - source
    *DMA2_SxM0AR = (uint32_t)rx_buf;                    // address of the destination, this is the variable which will be stored in RAM

    // for interrupt
    uint32_t* ISER1 = (uint32_t*)(0xE000E104);
    *ISER1 |= (1 << (58 - 32));

    // enable DMA
    *DMA2_SxCR |= (1 << 0);
}

void DMA2_Stream2_IRQHandler()
{
    firmware_transmit_complete_flag = 1;
    uint32_t* DMA2_LIFCR = (uint32_t*)(DMA2_ADDR_BASE + 0x08);
    *DMA2_LIFCR |= (1 << 21);    // clear transfer complete interrupt flag for DMA stream 2
}
