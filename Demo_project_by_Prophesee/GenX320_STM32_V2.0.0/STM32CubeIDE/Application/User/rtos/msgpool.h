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

/* RTOS Includes */
#include "FreeRTOS.h"
#include "queue.h"

#ifndef INC_FREERTOS_H
#error "include FreeRTOS.h" must appear in source files before "include msgpool.h"
#endif

typedef void *MsgPoolHandle_t;
typedef void *MsgHandle_t;

#define MSG_POOL_SIZE(ulNbMsgs, ulMsgSize) ( \
		ALIGN_CEIL(ulNbMsgs * ulMsgSize + \
				2 * sizeof(StaticQueue_t) + \
				2 * ulNbMsgs * sizeof(MsgHandle_t), uint32_t))

#ifdef __cplusplus
extern "C" {
#endif

/* Exported Function Prototypes */
extern MsgHandle_t xMsgPoolAllocate (const MsgPoolHandle_t xPoolHandle, TickType_t xTimeout);
extern MsgHandle_t xMsgPoolAllocateFromISR(const MsgPoolHandle_t xPoolHandle, BaseType_t *pxHigherPriorityTaskWoken);
extern BaseType_t  xMsgPoolPost     (const MsgPoolHandle_t xPoolHandle, MsgHandle_t xMsg);
extern BaseType_t  xMsgPoolPostFromISR     (const MsgPoolHandle_t xPoolHandle, MsgHandle_t xMsg, BaseType_t *pxHigherPriorityTaskWoken);
extern MsgHandle_t xMsgPoolGet      (const MsgPoolHandle_t xPoolHandle, TickType_t xTimeout);
extern BaseType_t  xMsgPoolRelease ( const MsgPoolHandle_t xPoolHandle, MsgHandle_t xMsg );
extern BaseType_t xMsgPoolReleaseFromISR( const MsgPoolHandle_t xPoolHandle, MsgHandle_t xMsg, BaseType_t *pxHigherPriorityTaskWoken );
extern BaseType_t xMsgPoolCreate(
		uint8_t *pxMsgs,
		const size_t ulNbMsgs,
		const size_t ulMsgSize,
		void* (*allocator) (size_t size),
		MsgPoolHandle_t *pxMsgPoolHandle);

#ifdef __cplusplus
}
#endif
