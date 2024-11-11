/**
 ******************************************************************************
 * @file    tasks.c
 * @author  PSEE Applications Team
 * @brief	Application specific Tasks
 *
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

/* Includes ------------------------------------------------------------------*/
#include "tasks.h"

/* Global variables for TouchGFX communication */
volatile uint32_t lp_counter[16] = {0};
volatile uint16_t total_ro_evt_cd_cnt = 0;
volatile uint16_t afk_flickering_ratio = 0;
volatile uint8_t me_direction = 0;
volatile uint32_t led_coordinates[2] = {0};

/**
 * @brief Function implementing the Low power wake up task.
 */
void psee_esp_lpw_task() {

	/* Infinite loop */
	for(;;)
	{

		/* Acquire the Semaphore */
		osSemaphoreAcquire(ESP_LPW_SemphHandle, portMAX_DELAY);

		uint32_t val = psee_get_mbx_misc() >> 16;
		if((val & 0b10) == 0b10) {
			/* Update the device_state based on the output from the algo */
			me_direction = (val & 0b01);
		}
		else {
			/* Update the device_state to sleep state */
			me_direction = 2;
		}

		/* Reset mailbox misc register */
		psee_set_mbx_misc(0);

		/* Update the values of the event counters */
		psee_read_ro_lp_evt_cnt((uint32_t *)lp_counter);

		/* Release the Semaphore */
		osSemaphoreRelease(ESP_LPW_SemphHandle);

		vTaskDelay( 15 / portTICK_PERIOD_MS);
	}
}

/**
 * @brief Function implementing the Statistics task.
 */
void psee_statistics_task() {

	static volatile uint16_t ro_evt_cd_cnt = 0;
	static uint32_t afk_flicker_evt_cnt = 0;
	static uint32_t afk_total_evt_cnt = 0;
	static double flicker_ratio = 0;

	/* Infinite loop */
	for(;;)
	{
		/* Acquire the Semaphore */
		osSemaphoreAcquire(Statistics_SemphHandle, portMAX_DELAY);

		/*********************** Total Pixel's CD Events Statistics **********************/
		ro_evt_cd_cnt = psee_read_ro_evt_cd_cnt();

		/* Convert to Pixel's CD events per second (Kevts/sec) */
		total_ro_evt_cd_cnt = (ro_evt_cd_cnt * 40) / 1000;

		/********************************  AFK Statistics ********************************/
		afk_flicker_evt_cnt = psee_read_afk_flicker_evt_cnt();
		afk_total_evt_cnt = psee_read_afk_total_evt_cnt();

		/* Ratio of total flickering events and total valid events */
		flicker_ratio = ((double)afk_flicker_evt_cnt / (double)afk_total_evt_cnt) * 100 ;
		afk_flickering_ratio = (uint32_t)flicker_ratio;

		/* Release the Semaphore */
		osSemaphoreRelease(Statistics_SemphHandle);

		vTaskDelay( 15 / portTICK_PERIOD_MS);

	}

}

/**
 * @brief Function implementing the Activity map task.
 */
void psee_activity_map_task() {

	/* Infinite loop */
	for(;;)
	{
		/* Acquire the Semaphore */
		osSemaphoreAcquire(Activity_Map_SemphHandle, portMAX_DELAY);

		/* Read the values of the 16 ro_lp_evt counters */
		psee_read_ro_lp_evt_cnt((uint32_t *)lp_counter);

		/* Release the Semaphore */
		osSemaphoreRelease(Activity_Map_SemphHandle);

		vTaskDelay( 15 / portTICK_PERIOD_MS);

	}

}

/**
 * @brief Function implementing the Led tracking task.
 */
void psee_led_tracking_task() {

	/* Infinite loop */
	for(;;)
	{
		/* Acquire the Semaphore */
		osSemaphoreAcquire(Led_Tracking_SemphHandle, portMAX_DELAY);

		/* Read the value of the mailbox misc register */
		uint32_t mbx_value = psee_get_mbx_misc();

		/* Fetch the value of the X coordinate */
		led_coordinates[0] = mbx_value >> 16;

		/* Fetch the value of the Y coordinate */
		led_coordinates[1] = mbx_value & 0xFFFF;

		/* Release the Semaphore */
		osSemaphoreRelease(Led_Tracking_SemphHandle);

		vTaskDelay( 3 / portTICK_PERIOD_MS);
	}

}
