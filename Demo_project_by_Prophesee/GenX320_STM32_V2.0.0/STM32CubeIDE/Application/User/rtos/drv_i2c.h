/**
 ******************************************************************************
 * @attention
 * Copyright (c) Prophesee S.A.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 *
 ******************************************************************************
 */
#pragma once

/* Standard Includes */
#include <assert.h>

/* HAL Includes */
#include "stm32f7xx_hal.h"

/* RTOS Includes */
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

#ifndef INC_FREERTOS_H
#error "include FreeRTOS.h" must appear in source files before "include freertos_i2c.h"
#endif

/* Private defines */
#define I2C_MAX_WAIT_TIME_PER_BYTE (( TickType_t ) 100 / portTICK_PERIOD_MS)
#define DRV_FreeRtos_I2C_STORAGE (12 + 2 * sizeof(StaticSemaphore_t))

typedef void *DrvI2cHandle_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Structure for I2C driver */
struct xDrvI2c {
	SemaphoreHandle_t xSemphIsrHdl;
	SemaphoreHandle_t xMutexLockHdl;
	I2C_HandleTypeDef *pxHdlI2c;
};

/* Exported Function Prototypes */
extern BaseType_t xDrvI2cCreate(
		I2C_HandleTypeDef *pxHdlI2c,
		void* (*allocator) (size_t size), DrvI2cHandle_t *pxDrvI2cHandle);
extern BaseType_t ucDrvI2cReadBytes(DrvI2cHandle_t xDrvI2cHdl, uint16_t uhDevAddr, uint16_t ucRegAddr, uint8_t *pucData, size_t ulNbBytes);
extern BaseType_t ucDrvI2cWriteBytes(DrvI2cHandle_t xDrvI2cHdl, uint16_t uhDevAddr, uint8_t *pucData, size_t nb_bytes);
extern BaseType_t ucDrvI2cCallback(DrvI2cHandle_t xDrvI2cHdl);

#ifdef __cplusplus
}
#endif
