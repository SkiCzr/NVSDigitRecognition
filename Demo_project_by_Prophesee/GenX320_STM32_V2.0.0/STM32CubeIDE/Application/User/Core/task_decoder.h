/**********************************************************************************************************************
 * Copyright (c) Prophesee S.A.                                                                                       *
 *                                                                                                                    *
 * Licensed under the Apache License, Version 2.0 (the "License");                                                    *
 * you may not use this file except in compliance with the License.                                                   *
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0                                 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed   *
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.                      *
 * See the License for the specific language governing permissions and limitations under the License.                 *
 **********************************************************************************************************************/

#ifndef APPLICATION_USER_CORE_TASK_DECODER_H_
#define APPLICATION_USER_CORE_TASK_DECODER_H_

/* Standard Includes */
#include <stdio.h>
#include <stdint.h>
#include <math.h>

/* HAL Includes */
#include "stm32f7xx_hal.h"

/* RTOS Includes */
#include <FreeRTOS.h>
#include <task.h>
#include "drv_dma.h"
#include "drv_genx320.h"

/* User Includes */
#include "task_frame_buffer.h"
#include "genx320.h"
#include "evt_2_0.h"

/* Structure for Event decoder */
typedef struct {
	DCMI_HandleTypeDef *hdcmi;
	DMA_HandleTypeDef  *hdma_dcmi;
	DrvGenx320Handle_t *drv_genx320;
} args_evt_decoder_t;

/* Exported Function Prototypes */
extern void task_evt_decoder(const args_evt_decoder_t *args);

#endif /* APPLICATION_USER_CORE_TASK_DECODER_H_ */
