/*
 * Uart.h
 *
 *  Created on: Oct 1, 2023
 *      Author: Admin
 */

#ifndef MY_LIBRARY_INC_UART_H_
#define MY_LIBRARY_INC_UART_H_

#include "main.h"

void uart1_init();
void uart_send_1_byte(char data);
void uart_send_string(char* str);
char uart_receive_data();

// void wifi_init();

void uart2_init();
void uart2_send(char data);
void send_at_command(char* at_command, char* response);

void custom_printf(char* format, ...);

#endif /* MY_LIBRARY_INC_UART_H_ */
