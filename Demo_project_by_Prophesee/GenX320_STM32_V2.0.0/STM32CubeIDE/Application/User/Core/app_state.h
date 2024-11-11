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
#ifndef APPLICATION_USER_CORE_APP_STATE_H_
#define APPLICATION_USER_CORE_APP_STATE_H_

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "semphr.h"
#include "cmsis_os.h"

/* User Includes */
#include "gui_com.h"
#include "genx320.h"
#include "firmware.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Task states
 */
typedef enum {
	supended       			=  0,
	resumed           		=  1,
} thread_state;

/**
 * @brief Structure that encapsulates task identifiers, semaphore handles, and states in an RTOS.
 */
typedef struct {
	osThreadId_t xTskHdl;
	osSemaphoreId_t xTskSemphHdl;
	thread_state xTskState;
} task_control_t;

/* Exported functions prototypes */
extern void enter_view();
extern void enter_histogram_view();
extern void enter_statistics_view();
extern void enter_LP_statistics_view();
extern void enter_view2();
extern void enter_PM2_view();
extern void enter_PM0_view();
extern void enter_LT_view();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_USER_CORE_APP_STATE_H_ */
