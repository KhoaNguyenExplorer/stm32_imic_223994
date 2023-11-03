/*
 * Uart.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */
#include <stdarg.h>
#include <stdio.h>
#include "Uart.h"
char rx_buf[5792];
uint32_t rx_index = 0;

uint8_t wifi_rx_buf[1024];
uint32_t wifi_rx_index = 0;

void uart1_init()
{
    // Baudrate: 115200bps
    // Parity: even
    // Data length: 8 bits

	// __HAL_RCC_GPIOB_CLK_ENABLE();
    clock_AHB1_enable(GPIOB_CLK);
	uint32_t* GPIOB_MODER = (uint32_t*)(0x40020400);
	*GPIOB_MODER &= ~((0b11 << 12) | (0b11 << 14));     // Clear PB6 & PB7
	*GPIOB_MODER |= (0b10 << 12) | (0b10 << 14);

	uint32_t* GPIOB_AFRL = (uint32_t*)(0x40020420);
	*GPIOB_AFRL &= ~((1 << 24) | (1 << 28));            // Clear PB6 & PB7
	*GPIOB_AFRL |= (7 << 24) | (7 << 28);;  		        // Set up pin B6 & B7 as alternate function

    // __HAL_RCC_USART1_CLK_ENABLE();
    clock_APB2_enable(USART1_CLK);    
    uint32_t* UART1_BRR = (uint32_t*)(0x40011008);
    uint32_t* UART1_CR1 = (uint32_t*)(0x4001100c);

    // *BRR |= (104 << 4) | (3 << 0);         // set baudrate = 9600 (mantissa + fraction)
    *UART1_BRR |= (8 << 4) | (139 << 0);            // set baudrate = 115200 (mantissa + fraction)
    *UART1_CR1 |= (1 << 12) | (1 << 10);            // enable even parity and configure data length to be 8-bit

    // For USART interrupt
    uint32_t* ISER1 = (uint32_t*)(0xE000E104);

    *UART1_CR1 |= (1 << 5);                         // enable interrupt, when RXNE = 1
    *ISER1 |= (1 << (37 - 32));               // position 37 in the interrupt vector table

    // enable UART and its transmit & receive functions
    *UART1_CR1 |= (1 << 13) | (1 << 3) | (1 << 2);
}

void uart_send_1_byte(char data)
{
    uint32_t* UART1_DR = (uint32_t*)(0x40011004);
    uint32_t* UART1_SR = (uint32_t*)(0x40011000);

    while (((*UART1_SR >> 7) & 1) != 1);            // this line and the following line will do a clearing sequence
    *UART1_DR = data;
}

void uart_send_string(char* str)
{
    int str_len = strlen(str);
    for (int i = 0; i < str_len; i++)
    {
        uart_send_1_byte(str[i]);
    }
}

char uart_receive_data()
{
    uint32_t* UART1_DR = (uint32_t*)(0x40011004);
    uint32_t* UART1_SR = (uint32_t*)(0x40011000);

    while (((*UART1_SR >> 5) & 1) != 1);            // check if read data register is empty or not
    char data = *UART1_DR;

    return data;
}

void custom_printf(char* format, ...)
{
    char buf[256] = {0};
	int buf_len = 0;
	va_list ap;
    va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);
	buf_len = strlen(buf);
	for(int i = 0; i < buf_len; i++)
	{
		uart_send_1_byte(buf[i]);
	}
}

void USART1_IRQHandler()
{
    uint32_t* UART1_DR = (uint32_t*)(0x40011004);
    uint32_t* UART1_SR = (uint32_t*)(0x40011000);
    rx_buf[rx_index++] = *UART1_DR;
    *UART1_SR &= ~(1 << 5);
}

void uart2_init()
{
    // Baudrate: 115200bps
    // Parity: even
    // Data length: 8 bits

	//   __HAL_RCC_GPIOD_CLK_ENABLE();
    clock_AHB1_enable(GPIOD_CLK);    
	  uint32_t* GPIOD_MODER = (uint32_t*)(0x40020C00);
	  *GPIOD_MODER &= ~((0b11 << 10) | (0b11 << 12));   // Clear PD5 & PD6
	  *GPIOD_MODER |= (0b10 << 10) | (0b10 << 12);

	  uint32_t* GPIOD_AFRL = (uint32_t*)(0x40020C20);
	  *GPIOD_AFRL &= ~((0xf << 20) | (0xf << 24));      // Clear PD5 & PD6
	  *GPIOD_AFRL |= (7 << 20) | (7 << 24);  		    // Set up pin D5 (Tx) & D6 (Rx) as alternate function

    // __HAL_RCC_USART2_CLK_ENABLE();
    clock_APB1_enable(USART2_CLK);
    uint32_t* UART2_BRR = (uint32_t*)(0x40044008);
    uint32_t* UART2_CR1 = (uint32_t*)(0x4004400c);

    // BRR_val = fCLK/(8*(2-OVER8)*baudrate)  
    // *BRR |= (104 << 4) | (3 << 0);                   // set baudrate = 9600 (mantissa + fraction)
    *UART2_BRR |= (8 << 4) | (11 << 0);                 // set baudrate = 115200 (mantissa + fraction)
    *UART2_CR1 |= (1 << 12) | (1 << 10);                // enable even parity and configure data length to be 8-bit

    // For USART interrupt
    uint32_t* ISER1 = (uint32_t*)(0xE000E104);

    *UART2_CR1 |= (1 << 5);                             // enable interrupt, when RXNE = 1
    *ISER1 |= (1 << (38 - 32));                         // position 37 in the interrupt vector table

    *UART2_CR1 |= (1 << 13) | (1 << 3) | (1 << 2);      // enable UART and its transmit & receive functions
}

void uart2_send(char data)
{
    uint32_t* UART2_DR = (uint32_t*)(0x40044004);
    uint32_t* UART2_SR = (uint32_t*)(0x40044000);

    while (((*UART2_SR >> 7) & 1) != 1);                // this line and the following line will do a clearing sequence
    *UART2_DR = data;
}

void USART2_IRQHandler()
{
    uint32_t* UART2_DR = (uint32_t*)(0x40044004);
    uint32_t* UART2_SR = (uint32_t*)(0x40044000);

    wifi_rx_buf[wifi_rx_index++] = *UART2_DR;
    *UART2_SR &= ~(1 << 5);

    // prevent overflow
    if (wifi_rx_index > sizeof(wifi_rx_buf))
    {
        wifi_rx_index = 0;
    }
}

// void wifi_init()
// {
//     uart2_init();
// }

// void send_at_command(char* at_command, char* response)
// {
//     uint32_t at_cmd_len = strlen(at_command);
//     wifi_rx_index = 0;
//     memset(wifi_rx_buf, 0, sizeof(wifi_rx_buf));

//     for (int i = 0; i <at_cmd_len; i++)
//     {
//         uart2_send(at_command[i]);
//     }
//     uart2_send('\r');
//     uart2_send('\n');

//     while(1)
//     {
//         if (strstr(wifi_rx_buf, "OK\r\n") != -1)
//         {
//             __asm("NOP");
//             memset(wifi_rx_buf, 0, sizeof(wifi_rx_buf));
//             wifi_rx_index = 0;
//             // TODO: rewrite this part
//         }
//     }
// }

// void wifi_setup(char* ssid, char* pass)
// {   
//     if(send_at_command("AT+CWMODE=1", NULL) != WIFI_OK);
//         return WIFI_ERR;
//     else(send_at_command("AT+CWJAP=1", NULL) != WIFI_OK)
// }

// void wifi_get_database()
// {
    
// }
