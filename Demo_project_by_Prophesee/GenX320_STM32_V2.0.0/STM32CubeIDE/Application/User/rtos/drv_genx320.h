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
#ifndef APPLICATION_USER_RTOS_DRV_GENX320_H_
#define APPLICATION_USER_RTOS_DRV_GENX320_H_

/* Standard Includes */
#include <assert.h>
#include <stdbool.h>

/* RTOS Includes */
#include <FreeRTOS.h>
#include <semphr.h>

/* User Includes */
#include "genx320.h"
#include "firmware.h"

#ifndef INC_FREERTOS_H
#error "include FreeRTOS.h" must appear in source files before "include freertos_i2c.h"
#endif

typedef void *DrvGenx320Handle_t;

/* Exported Function Prototypes */
extern BaseType_t xDrvGenx320Create(
		void* (*allocator) (size_t size),
		DrvGenx320Handle_t *pxDrvGenx320Handle);
extern BaseType_t ucDrvGenx320Init(DrvGenx320Handle_t xDrvGenx320Handle);
extern BaseType_t ucDrvGenx320DisableAfk(DrvGenx320Handle_t xDrvGenx320Handle);

#endif /* APPLICATION_USER_RTOS_DRV_GENX320_H_ */
