/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "flash.h"
#include "led.h"
#include "tim3.h"
#include "uart1.h"
#include "uart2.h"
//#include "usbd_gp_handle.h"
//#include "user_function_handle.h"
//#include "user_handle.h"
//#include "user_ringbuffer_handle.h"
//#include "user_script_handle.h"
#include "EasyCon_API.h"
#include "Hid.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LEDB_Pin GPIO_PIN_13
#define LEDB_GPIO_Port GPIOC
#define UMAIN_TX_Pin GPIO_PIN_2
#define UMAIN_TX_GPIO_Port GPIOA
#define UMAIN_RX_Pin GPIO_PIN_3
#define UMAIN_RX_GPIO_Port GPIOA
#define DEBUG_TX_Pin GPIO_PIN_9
#define DEBUG_TX_GPIO_Port GPIOA
#define DEBUG_RX_Pin GPIO_PIN_10
#define DEBUG_RX_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
#define PROJ_NAME "NANcon"
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
