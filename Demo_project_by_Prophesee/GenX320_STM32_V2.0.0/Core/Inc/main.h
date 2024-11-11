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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* Standard Includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* RTOS Includes */
#include "cmsis_os.h"
#include <semphr.h>
#include <queue.h>
#include "msgpool.h"
#include "drv_i2c.h"
#include "drv_dma.h"
#include "drv_genx320.h"

/* User Includes */
#include "genx320.h"
#include "gui_com.h"
#include "task_decoder.h"
#include "task_frame_buffer.h"
#include "firmware.h"
#include "tasks.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* Exported Thread Handles */
extern osThreadId_t Update_FbHandle;
extern osThreadId_t Event_DecodingHandle;
extern osThreadId_t Activity_MapHandle;
extern osThreadId_t StatisticsHandle;
extern osThreadId_t ESP_LPWHandle;
extern osThreadId_t Led_TrackingHandle;

/* Exported Semaphore Handles */
extern osSemaphoreId_t Vsync_SemaphoreHandle;
extern osSemaphoreId_t ESP_LPW_SemphHandle;
extern osSemaphoreId_t Activity_Map_SemphHandle;
extern osSemaphoreId_t Statistics_SemphHandle;
extern osSemaphoreId_t Led_Tracking_SemphHandle;

/* Exported Driver Handles */
extern LTDC_HandleTypeDef hltdc;
extern DMA_HandleTypeDef hdma_dcmi;
extern TIM_HandleTypeDef htim1;
extern DCMI_HandleTypeDef hdcmi;
extern I2C_HandleTypeDef hi2c1;
extern DrvGenx320Handle_t drv_genx320;
extern DrvI2cHandle_t drv_i2c;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */


/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern void psee_sleep_us_imp(uint32_t duration);
extern void ULP_standby_sequence();
extern void ULP_wakeup_sequence();

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ARDUINO_SCL_D15_Pin GPIO_PIN_8
#define ARDUINO_SCL_D15_GPIO_Port GPIOB
#define VSYNC_FREQ_Pin GPIO_PIN_4
#define VSYNC_FREQ_GPIO_Port GPIOB
#define ARDUINO_SDA_D14_Pin GPIO_PIN_9
#define ARDUINO_SDA_D14_GPIO_Port GPIOB
#define LCD_BL_CTRL_Pin GPIO_PIN_3
#define LCD_BL_CTRL_GPIO_Port GPIOK
#define LCD_DISP_Pin GPIO_PIN_12
#define LCD_DISP_GPIO_Port GPIOI
#define DCMI_PWR_EN_Pin GPIO_PIN_13
#define DCMI_PWR_EN_GPIO_Port GPIOH
#define PSU_EN_1V8____D5_Pin GPIO_PIN_0
#define PSU_EN_1V8____D5_GPIO_Port GPIOI
#define FRAME_RATE_Pin GPIO_PIN_7
#define FRAME_RATE_GPIO_Port GPIOC
#define RENDER_TIME_Pin GPIO_PIN_6
#define RENDER_TIME_GPIO_Port GPIOC
#define PSU_EN_1V1____D4_Pin GPIO_PIN_7
#define PSU_EN_1V1____D4_GPIO_Port GPIOG
#define MCU_ACTIVE_Pin GPIO_PIN_6
#define MCU_ACTIVE_GPIO_Port GPIOG
#define ULP_TH_Pin GPIO_PIN_7
#define ULP_TH_GPIO_Port GPIOF
#define ULP_RSTn_Pin GPIO_PIN_6
#define ULP_RSTn_GPIO_Port GPIOF
#define WAKEUP_Pin GPIO_PIN_9
#define WAKEUP_GPIO_Port GPIOF
#define ULP_ENb_Pin GPIO_PIN_8
#define ULP_ENb_GPIO_Port GPIOF
#define PSU_EN_2V5____D6_Pin GPIO_PIN_6
#define PSU_EN_2V5____D6_GPIO_Port GPIOH

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
