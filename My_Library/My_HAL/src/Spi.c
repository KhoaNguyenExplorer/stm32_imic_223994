/*
 * Spi.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#include "Spi.h"

// Function prototypes
/**
 * @brief: control spi device
 * @param: state
 *         0: inactive
 *         1: active
 * @retval: None
*/

uint8_t id;
uint16_t x_result, y_result, z_result = 0;

static void spi_slave_ctrl(spi_slave_state_t state)
{
#if (RC522_FEATURE == RC522_DISABLE)    
    uint32_t* GPIOE_ODR = (uint32_t*)(0x40021014);
    if (state == SLAVE_ACTIVE)
    {
        *GPIOE_ODR &= ~(1 << 3);
    }
    else
    {
        *GPIOE_ODR |= (1 << 3);
    }
#else
    uint32_t* GPIOE_ODR = (uint32_t*)(0x40020C14);
    if (state == SLAVE_ACTIVE)
    {
        *GPIOE_ODR &= ~(1 << 12);
    }
    else
    {
        *GPIOE_ODR |= (1 << 12);
    }
#endif
}

void spi1_init()
{
    clock_AHB1_enable(GPIOA_CLK);
    uint32_t* GPIOA_MODER = (uint32_t*)(0x40020000);
	*GPIOA_MODER &= ~(0b111111 << 10);                      // Clear PA5 to PA7
	*GPIOA_MODER |= (0b10 << 10) | (0b10 << 12) | (0b10 << 14);

	uint32_t* GPIOA_AFRL = (uint32_t*)(0x40020020);
	*GPIOA_AFRL &= ~(0xfff << 20);                          // Clear PA5 to PA7
	*GPIOA_AFRL |= (5 << 20) | (5 << 24) | (5 << 28);       // Set up pin PA5 to PA7 as alternate function 5

    clock_AHB1_enable(GPIOE_CLK);    
    uint32_t* GPIOE_MODER = (uint32_t*)(0x40021000);        
	  *GPIOE_MODER &= ~(0b11 << 6);                         // Clear PE3
	  *GPIOE_MODER |= (0b01 << 6);                          // Configure PE3 as output
      
    uint32_t* GPIOE_ODR = (uint32_t*)(0x40021014);      
    *GPIOE_ODR &= ~(1 << 3);                              // clear CS pin so that the Gyroscope is active
      
#if (RC522_FEATURE == RC522_ENABLE)        
      *GPIOE_ODR |= (1 << 3);								// pull the Gyroscope up so that it is inactive
    // configure for connections with RC522 board     
    clock_AHB1_enable(GPIOD_CLK);        
    uint32_t* GPIOD_MODER = (uint32_t*)(0x40020C00);
    *GPIOD_MODER &= ~(0b11 << 24);                          // Clear PD12
    *GPIOD_MODER |= (0b01 << 24);                           // Configure PD12 as output
#endif

    spi_slave_ctrl(SLAVE_INACTIVE);

    clock_APB2_enable(SPI1_CLK);
    uint32_t* SPI1_CR1 = (uint32_t*)(0x40013000);           
    *SPI1_CR1 |= (1 << 2) | (0b100 << 3) | (1 << 8) | (1 << 9);  // configured STM32 as Master

    *SPI1_CR1 |= (1 << 6);                                  // enable SPI
}

void spi_write(uint8_t data_type, uint8_t data)
{
    uint32_t* SPI1_SR = (uint32_t*)(0x40013008);
    uint32_t* SPI1_DR = (uint32_t*)(0x4001300c);

    spi_slave_ctrl(SLAVE_ACTIVE);

    while (((*SPI1_SR >> 1) & 1) != 1);   // wait until data buffer is empty
    *SPI1_DR = data_type;
    while (((*SPI1_SR >> 7) & 1) == 1);   // wait until BSY = 0
    while (((*SPI1_SR >> 0) & 1) != 1);
    uint32_t temp = *SPI1_DR;             // clear spam data on MISO

    while (((*SPI1_SR >> 1) & 1) != 1);   // wait until data buffer is empty
    *SPI1_DR = data;                      // send data to slave
    while (((*SPI1_SR >> 7) & 1) == 1);   // wait until BSY = 0
    while (((*SPI1_SR >> 0) & 1) != 1);   // clear spam data on MISO
    temp = *SPI1_DR;
    (void)temp;

    spi_slave_ctrl(SLAVE_INACTIVE);
}

uint8_t spi_read(uint8_t data_type)
{
    uint32_t* SPI1_SR = (uint32_t*)(0x40013008);
    uint32_t* SPI1_DR = (uint32_t*)(0x4001300c);
    spi_slave_ctrl(SLAVE_ACTIVE);

    while (((*SPI1_SR >> 1) & 1) != 1);   // wait until data buffer is empty

#if (RC522_FEATURE == RC522_ENABLE)        
    *SPI1_DR = data_type;			// for RC522
#else
    *SPI1_DR = (data_type | 0x80);        // set bit 1 to read data from slave
#endif

    while (((*SPI1_SR >> 7) & 1) == 1);   // wait until BSY = 0
    while (((*SPI1_SR >> 0) & 1) != 1);
    uint32_t temp = *SPI1_DR;             // clear spam data on MISO

    while (((*SPI1_SR >> 1) & 1) != 1);   // wait until data buffer is empty
    *SPI1_DR = 0xff;                      // send "dummy" data to allow slave to transmit
    while (((*SPI1_SR >> 7) & 1) == 1);   // wait until BSY = 0
    while (((*SPI1_SR >> 0) & 1) != 1);   // clear spam data on MISO
    temp = *SPI1_DR;
    (void)temp;                           // fix warning

    spi_slave_ctrl(SLAVE_INACTIVE);
    return temp;
}

void check_spi_active()
{
    id = spi_read(WHO_AM_I);
    custom_printf("ID: %d\r\n", id);  
}

void read_gyro_through_spi()
{
    spi_write(CTRL_REG1, 0b00111111);      
    while ((spi_read(STATUS_REG) & 0b111) != 0b111)
    {
        delay(10);
    }

    uint8_t x_l = spi_read(OUT_X_L);
    uint8_t x_h = spi_read(OUT_X_H);  
    uint8_t y_l = spi_read(OUT_Y_L);
    uint8_t y_h = spi_read(OUT_Y_H);   
    uint8_t z_l = spi_read(OUT_Z_L);
    uint8_t z_h = spi_read(OUT_Z_H);       

    x_result = (x_h << 8) | x_l;
    y_result = (y_h << 8) | y_l;
    z_result = (z_h << 8) | z_l;    
}
