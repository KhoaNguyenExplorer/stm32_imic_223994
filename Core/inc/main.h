/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "Peripheral_LED.h"
#include "Peripheral_button.h"
#include "MFRC522.h"
#include "Timer.h"  
#include "Uart.h"
#include "Spi.h"
#include "I2c.h"
#include "AD_converter.h"
#include "Clock.h"
#include "Firmware_update.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void SystemInit(void);

/* Private defines -----------------------------------------------------------*/
#define FREERTOS_ENABLE   1
#define FREERTOS_DISABLE  0
#define FREERTOS_FEATURE  FREERTOS_ENABLE

#define VOLTAGE_ADC       0
#define CPU_TEMP          1
#define GYRO_READ         2
#define CARD_READ         3
#define PROGRAM_SELECT    CARD_READ

// Configure the SPI to use the MFRC522 library
#define RC522_ENABLE    1
#define RC522_DISABLE   0

#if (PROGRAM_SELECT == CARD_READ)
#define RC522_FEATURE   RC522_ENABLE
#else
#define RC522_FEATURE   RC522_DISABLE
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
