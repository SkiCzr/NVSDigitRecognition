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
#ifndef APPLICATION_USER_CORE_TASK_FRAME_BUFFER_H_
#define APPLICATION_USER_CORE_TASK_FRAME_BUFFER_H_

/* Standard Includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* HAL Includes */
#include "stm32f7xx_hal.h"

/* RTOS Includes */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "drv_dma.h"

/* Structure for Update FB */
typedef struct {
	LTDC_HandleTypeDef *hltdc;
	DrvDmaHandle_t *drv_dma;
} args_update_fb_t;

extern uint8_t* volatile fb_cpi;

/* Exported Function Prototypes */
extern void reset_frame_buffers();
extern void task_update_fb(const args_update_fb_t *args);
extern void update_fb_callback();
extern int get_prediction();

#endif /* APPLICATION_USER_CORE_TASK_FRAME_BUFFER_H_ */
