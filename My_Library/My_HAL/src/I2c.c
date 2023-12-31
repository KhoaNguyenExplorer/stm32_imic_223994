/*
 * I2c.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#include "I2c.h"

void i2c1_init()
{
    // Set up pins PB6 & PB9 as alternate function 4
    // __HAL_RCC_GPIOB_CLK_ENABLE();
    uint32_t* GPIOB_MODER = (uint32_t*)(0x40020400);
	*GPIOB_MODER &= ~((0b11 << 12) | (0b11 << 18)); 
	*GPIOB_MODER |= (0b10 << 12) | (0b10 << 18);

    uint32_t* GPIOB_AFRL = (uint32_t*)(0x40020420);
	*GPIOB_AFRL &= ~(0b1111 << 24);
	*GPIOB_AFRL |= (4 << 24);

    uint32_t* GPIOB_AFRH = (uint32_t*)(0x40020424);
	*GPIOB_AFRH &= ~(0b1111 << 4);
	*GPIOB_AFRH |= (4 << 4);

    // set up i2c
    // __HAL_RCC_I2C1_CLK_ENABLE();
    uint32_t* I2C1_CR1 = (uint32_t*)(0x40005400);
    uint32_t* I2C1_CR2 = (uint32_t*)(0x40005404);
    uint32_t* I2C1_CCR = (uint32_t*)(0x4000541C);
	
    *I2C1_CR1 &= ~(1 << 0);     // Firstly disable I2C
    *I2C1_CR2 |= 16;            // Declare the frequency used for I2C

    *I2C1_CCR = 500;            /* Declare CCR = 500 so that the frequency for controlling
	                             * i2c is 32Khz (calculated from the datasheet of LSM303AGR,
	                             * and the input clock, which is 16MHz)
	                            */
    
    *I2C1_CR1 |= (1 << 0);      // enable i2c
}

void i2c1_write(uint8_t data_type, uint8_t data_write)
{
    uint32_t* I2C1_CR1 = (uint32_t*)(0x40005400);   
    uint32_t* I2C1_SR1 = (uint32_t*)(0x40005414);   
    uint32_t* I2C1_SR2 = (uint32_t*)(0x40005418);   
    uint32_t* I2C1_DR = (uint32_t*)(0x40005410); 

    while (((*I2C1_SR2 >> 1) & 1) == 1);            // wait until not busy
    *I2C1_CR1 |= (1 << 8);                          // request to generate a start bit
    
    while (((*I2C1_SR1 >> 0) & 1) != 1);            // check if start bit has been generated by waiting
                                                    // until the SB bit has been generated in SR1
                                                    
    *I2C1_DR = (ACCEL_BASE_ADDR << 1) | I2C_WRITE_BIT;  // write 7 bits of slave address | 1 WR bit
    while (((*I2C1_SR1 >> 1) & 1) != 1);            // check if address has been sent
    uint32_t temp = *I2C1_SR2;                      // reading SR1 then SR2 => Clear ADDR bit of SR1
    (void)temp;                                     // fix warning

    if ((*I2C1_SR1 >> 10) & 1)                      // if the transfer is not successful (NACK)
    {
        *I2C1_CR1 |= (1 << 9);  // send STOP bit and exit
        return;
    }
    else 
    {
        /*Do nothing, continue*/
    }

    *I2C1_DR = data_type;                           // send command frame to slave
    while (((*I2C1_SR1 >> 2) & 1) != 1);            // wait until the command frame has been transferred

    if ((*I2C1_SR1 >> 10) & 1)                      // if the transfer is not successful (NACK)
    {
        *I2C1_CR1 |= (1 << 9);  // send STOP bit and exit
        return;
    }
    else 
    {
        /*Do nothing, continue*/
    }    

    *I2C1_DR = data_write;                         	// send data
    while (((*I2C1_SR1 >> 2) & 1) != 1);            // wait until the command frame has been transferred
    *I2C1_CR1 |= (1 << 9);                          // send STOP bit
}

uint8_t i2c1_read(uint8_t data_type, uint8_t* data_read)
{
    uint32_t* I2C1_CR1 = (uint32_t*)(0x40005400);   
    uint32_t* I2C1_SR1 = (uint32_t*)(0x40005414);   
    uint32_t* I2C1_SR2 = (uint32_t*)(0x40005418);   
    uint32_t* I2C1_DR = (uint32_t*)(0x40005410); 

    while (((*I2C1_SR2 >> 1) & 1) == 1);            // wait until not busy

    *I2C1_CR1 |= (1 << 8);                          // request to generate a start bit
    while (((*I2C1_SR1 >> 0) & 1) != 1);            // check if start bit has been generated by waiting
                                                    // until the SB bit has been generated in SR1    

    *I2C1_DR = (ACCEL_BASE_ADDR << 1) | I2C_WRITE_BIT;  // write 7 bits of slave address | 1 WR bit
    while (((*I2C1_SR1 >> 1) & 1) != 1);            // check if address has been sent
    uint32_t temp = *I2C1_SR2;                      // reading SR1 then SR2 => Clear ADDR bit of SR1

    if ((*I2C1_SR1 >> 10) & 1)                      // if the transfer is not successful (NACK)
    {
        *I2C1_CR1 |= (1 << 9);  // send STOP bit and exit
        return -1;
    }
    else 
    {
        /*Do nothing, continue*/
    }

    *I2C1_DR = data_type;                           // send command frame to slave
    while (((*I2C1_SR1 >> 2) & 1) != 1);            // wait until the command frame has been transferred

    if ((*I2C1_SR1 >> 10) & 1)                      // if the transfer is not successful (NACK)
    {
        *I2C1_CR1 |= (1 << 9);  // send STOP bit and exit
        return -1;
    }
    else 
    {
        /*Do nothing, continue*/
    }    

    // REPEAT: request to generate a start bit
    *I2C1_CR1 |= (1 << 8);
    while (((*I2C1_SR1 >> 0) & 1) != 1);            // check if start bit has been generated by waiting
                                                    // until the SB bit has been generated in SR1   

    *I2C1_DR = (ACCEL_BASE_ADDR << 1) | I2C_READ_BIT;  // write 7 bits of slave address | 1 RD bit
    while (((*I2C1_SR1 >> 1) & 1) != 1);            // check if address has been sent
    temp = *I2C1_SR2;                               // reading SR1 then SR2 => Clear ADDR bit of SR1
   (void)temp;                                     // fix warning

    if ((*I2C1_SR1 >> 10) & 1)                      // if the transfer is not successful (NACK)
    {
        *I2C1_CR1 |= (1 << 9);  // send STOP bit and exit
        return -1;
    }
    else 
    {
        /*Do nothing, continue*/
    }   

    *data_read = (uint8_t)*I2C1_DR;                	// Read data from DR register
    *I2C1_CR1 |= (1 << 9);                          // send STOP bit  

    return *data_read;
}
