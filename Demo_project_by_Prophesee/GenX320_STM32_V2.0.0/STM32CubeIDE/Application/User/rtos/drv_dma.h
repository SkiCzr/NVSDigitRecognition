/*
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

typedef void *DrvDmaHandle_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Exported Function Prototypes */
extern BaseType_t xDrvDmaCreate(
		DMA_HandleTypeDef *pxHdlDma,
		void* (*allocator) (size_t size), DrvDmaHandle_t *pxDrvDmaHandle);
extern BaseType_t ucDrvDmaMemsetTo0(DrvDmaHandle_t xDrvDmaHandle, void *pucData, size_t ulNbBytes);
extern BaseType_t ucDrvDmaCallback(DrvDmaHandle_t xDrvI2cHdl);

#ifdef __cplusplus
}
#endif
