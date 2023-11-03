/*
 * Dma.h
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#ifndef MY_LIBRARY_INC_DMA_H_
#define MY_LIBRARY_INC_DMA_H_

#include "main.h"

#define DMA2_ADDR_BASE  0x40026400

void dma2_init(uint32_t size);

#endif /* MY_LIBRARY_INC_DMA_H_ */
