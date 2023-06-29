/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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
#define DBG_Pin GPIO_PIN_0
#define DBG_GPIO_Port GPIOA
#define MODE_1_BUTTON_Pin GPIO_PIN_1
#define MODE_1_BUTTON_GPIO_Port GPIOA
#define RS485_NUCLEO_TX_Pin GPIO_PIN_2
#define RS485_NUCLEO_TX_GPIO_Port GPIOA
#define RS485_NUCLEO_RX_Pin GPIO_PIN_3
#define RS485_NUCLEO_RX_GPIO_Port GPIOA
#define FULL_GAS_BUTTON_Pin GPIO_PIN_4
#define FULL_GAS_BUTTON_GPIO_Port GPIOA
#define LED_STS_Pin GPIO_PIN_7
#define LED_STS_GPIO_Port GPIOA
#define HORN_BUTTON_Pin GPIO_PIN_0
#define HORN_BUTTON_GPIO_Port GPIOB
#define MODE_2_BUTTON_Pin GPIO_PIN_1
#define MODE_2_BUTTON_GPIO_Port GPIOB
#define SC_CLOSE_BUTTON_Pin GPIO_PIN_8
#define SC_CLOSE_BUTTON_GPIO_Port GPIOA
#define LCD_TX_Pin GPIO_PIN_9
#define LCD_TX_GPIO_Port GPIOA
#define LCD_RX_Pin GPIO_PIN_10
#define LCD_RX_GPIO_Port GPIOA
#define FUELCELL_RACE_MODE_BUTTON_Pin GPIO_PIN_11
#define FUELCELL_RACE_MODE_BUTTON_GPIO_Port GPIOA
#define HALF_GAS_BUTTON_Pin GPIO_PIN_12
#define HALF_GAS_BUTTON_GPIO_Port GPIOA
#define LED_NUCLEO_Pin GPIO_PIN_3
#define LED_NUCLEO_GPIO_Port GPIOB
#define FUELCELL_OFF_MODE_BUTTON_Pin GPIO_PIN_4
#define FUELCELL_OFF_MODE_BUTTON_GPIO_Port GPIOB
#define FUELCELL_PREPARETORACE_MODE_BUTTON_Pin GPIO_PIN_5
#define FUELCELL_PREPARETORACE_MODE_BUTTON_GPIO_Port GPIOB
#define SUPPLY_BUTTON_Pin GPIO_PIN_6
#define SUPPLY_BUTTON_GPIO_Port GPIOB
#define SPEED_RESET_BUTTON_Pin GPIO_PIN_7
#define SPEED_RESET_BUTTON_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
