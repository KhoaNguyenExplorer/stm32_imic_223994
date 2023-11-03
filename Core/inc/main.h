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
// typedef enum
// {
// 	VOLTAGE_TEMP,
// 	GYRO_READ,
//   CARD_READ
// } program_select_t;
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void SystemInit(void);

/* Private defines -----------------------------------------------------------*/
#define VOLTAGE_TEMP    0
#define GYRO_READ       1
#define CARD_READ       2
#define PROGRAM_SELECT  GYRO_READ

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
