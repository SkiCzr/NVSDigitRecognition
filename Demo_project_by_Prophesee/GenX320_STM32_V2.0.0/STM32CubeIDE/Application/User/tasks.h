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

#ifndef APPLICATION_USER_TASKS_H_
#define APPLICATION_USER_TASKS_H_

/* Standard Includes */
#include <stdio.h>
#include <stdint.h>

/* RTOS Includes */
#include "cmsis_os.h"

/* User Includes */
#include "main.h"
#include "genx320.h"

/* Time Delays us */
#define TIO_start		100
#define Tana_start		100
#define Tclk_start		200
#define trstn_start		100

#define tclk_stop		100
#define Tana_stop		100
#define TIO_stop		100
#define Tdig_stop		100

#define Tulp_init		2000
#define Tulp_rstn_low	10
#define Tfunc_start		100
#define Tulp_release	100

#define Tulp_set		100

/* Time Delays ms */
#define Tulp_init		2000
#define Tulp_retention	1000

/* Exported Variables for TouchGFX */
extern volatile uint32_t lp_counter[16];
extern volatile uint16_t total_ro_evt_cd_cnt;
extern volatile uint16_t afk_flickering_ratio;
extern volatile uint8_t me_direction;
extern volatile uint32_t led_coordinates[2];

/* Exported Function Prototypes */
extern void psee_esp_lpw_task();
extern void psee_statistics_task();
extern void psee_activity_map_task();
extern void psee_led_tracking_task();

#endif /* APPLICATION_USER_TASKS_H_ */
