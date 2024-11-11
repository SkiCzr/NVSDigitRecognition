/**
 ******************************************************************************
 * @file    app_state.c
 * @author  PSEE Applications Team
 * @brief	RTOS Scheduling
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
#include "app_state.h"

/* Private Defines */
#define DISPLAY_RESOLUTION 	(320*272)
#define TRUE 				1
#define FALSE 				0

/*
 * @brief Imported Handle's
 * @note The following references are defined in main.c
 */
extern osThreadId_t ESP_LPWHandle;
extern osThreadId_t TouchGFXTaskHandle;
extern osThreadId_t Update_FbHandle;
extern osThreadId_t Event_DecodingHandle;
extern osThreadId_t Activity_MapHandle;
extern osThreadId_t StatisticsHandle;
extern osThreadId_t Led_TrackingHandle;
extern osSemaphoreId_t ESP_LPW_SemphHandle;
extern osSemaphoreId_t Activity_Map_SemphHandle;
extern osSemaphoreId_t Statistics_SemphHandle;
extern osSemaphoreId_t Led_Tracking_SemphHandle;
extern osMutexId_t tskCtlUpdateFbHandle;
extern LTDC_HandleTypeDef hltdc;

/* Firmware Instance */
static const Firmware *active_firmware = NULL;

/* Private variables */
static uint8_t init_done = 0;

/* Frame Buffer */
static uint8_t frame_buffer_l1_default[DISPLAY_RESOLUTION] __attribute__((section("L1_Frame_Buffers"))) __attribute__((aligned(4)));

/* Task control Instances for all tasks */
task_control_t esp_lpw_tsk_ctrl;
task_control_t led_tsk_ctrl;
task_control_t statistics_tsk_ctrl;
task_control_t activity_map_tsk_ctrl;
task_control_t event_decoding_tsk_ctrl;
task_control_t update_fb_tsk_ctrl;

/**
 * @brief Function to initialize the task control for each task.
 * @param tsk_ctrl Pointer to task control structure
 * @param tsk_hdl Task handle
 * @param tsk_semph_hdl Task semaphore handle
 */
static void init_task_control(task_control_t* tsk_ctrl, osThreadId_t tsk_hdl, osSemaphoreId_t tsk_semph_hdl) {

	/* Initialize task control */
	tsk_ctrl->xTskHdl = tsk_hdl;
	tsk_ctrl->xTskSemphHdl = tsk_semph_hdl;
	tsk_ctrl->xTskState = resumed;

	if(tsk_semph_hdl != NULL) {

		/* Add initial value to the Semaphore */
		osSemaphoreRelease(tsk_ctrl->xTskSemphHdl);
	}

}

/**
 * @brief Function to resume the RTOS task.
 * @param tsk_ctrl Pointer to task control structure
 */
static void resume_rtos_task(task_control_t *tsk_ctrl) {

	/* Check if semaphore_id is NULL */
	if (tsk_ctrl->xTskSemphHdl == NULL) {

		/* Resume the task */
		osThreadResume(tsk_ctrl->xTskHdl );

	}
	else if (tsk_ctrl->xTskSemphHdl != NULL) {

		/* Check if task is in suspended state */
		if(tsk_ctrl->xTskState == supended) {

			/* Release the Semaphore */
			osSemaphoreRelease(tsk_ctrl->xTskSemphHdl);

			/* Update task state */
			tsk_ctrl->xTskState = resumed;

		}
	}
}

/**
 * @brief Function to suspend the RTOS task.
 * @param tsk_ctrl Pointer to task control structure
 */
static void suspend_rtos_task(task_control_t *tsk_ctrl) {

	/* Check if semaphore_id is NULL */
	if (tsk_ctrl->xTskSemphHdl == NULL) {

		/* Suspend the task */
		osThreadSuspend(tsk_ctrl->xTskHdl );

	}
	else if (tsk_ctrl->xTskSemphHdl  != NULL) {

		/* Check if task is in resumed state */
		if(tsk_ctrl->xTskState == resumed) {

			/* Acquire the Semaphore */
			osSemaphoreAcquire(tsk_ctrl->xTskSemphHdl, portMAX_DELAY);

			/* Update task state */
			tsk_ctrl->xTskState = supended;

		}
	}
}

/**
 * @brief Configuration function for Streaming mode screen.
 */
void enter_view()
{
	/* Enable and Update the transparency for the layer 2*/
	enable_lcd_layer_event(&hltdc, LAYER_TWO, frame_buffer_l1_default);

	xSemaphoreGive(tskCtlUpdateFbHandle);

	if(init_done == FALSE) {

		/* Initialize all task control structures */
		init_task_control(&esp_lpw_tsk_ctrl, ESP_LPWHandle, ESP_LPW_SemphHandle);
		init_task_control(&led_tsk_ctrl, Led_TrackingHandle, Led_Tracking_SemphHandle);
		init_task_control(&statistics_tsk_ctrl, StatisticsHandle, Statistics_SemphHandle);
		init_task_control(&activity_map_tsk_ctrl, Activity_MapHandle, Activity_Map_SemphHandle);
		init_task_control(&event_decoding_tsk_ctrl, Event_DecodingHandle, NULL);
		init_task_control(&update_fb_tsk_ctrl, Update_FbHandle, NULL);

		init_done = TRUE;
	}

	/* Resume the RTOS tasks that are required for this Screen */
	resume_rtos_task(&event_decoding_tsk_ctrl);
	resume_rtos_task(&update_fb_tsk_ctrl);

	/* Suspend the RTOS tasks that are not required for this Screen */
	suspend_rtos_task(&activity_map_tsk_ctrl);
	suspend_rtos_task(&statistics_tsk_ctrl);
	suspend_rtos_task(&esp_lpw_tsk_ctrl);
	suspend_rtos_task(&led_tsk_ctrl);

	/* Disable the Anti-FlicKering filter */
	psee_disable_afk();

	/* Operation Mode Configuration */
	psee_PM3C_config();

	/* Set the default border for the Activity map */
	psee_set_default_XY_borders(&genx320mp_default_am_borders);

	/* Configure the Activity map */
	psee_configure_activity_map();

	/* Reset the RISC-V Firmware */
	if (active_firmware != NULL) {

		psee_reset_firmware(genx320_boot_mode);
		active_firmware = NULL;

	}

}

/**
 * @brief Configuration function for Statistics screen.
 */
void enter_statistics_view()
{
	/* Update the transparency for the layer 2 */
	set_layer_transparency(ALPHA_0, LAYER_TWO);

	/* Resume the RTOS tasks that are required for this Screen */
	resume_rtos_task(&statistics_tsk_ctrl);

	/* Suspend the RTOS tasks that are not required for this Screen */
	suspend_rtos_task(&event_decoding_tsk_ctrl);
	suspend_rtos_task(&update_fb_tsk_ctrl);
	suspend_rtos_task(&activity_map_tsk_ctrl);
	suspend_rtos_task(&esp_lpw_tsk_ctrl);
	suspend_rtos_task(&led_tsk_ctrl);

	/* Enable the Anti-FlicKering filter */
	psee_enable_afk(50,200);

	/* Operation Mode Configuration */
	psee_PM2_config();
}

/**
 * @brief Configuration function for Low power statistics screen.
 */
void enter_LP_statistics_view()
{
	/* Update the transparency for the layer 2 */
	set_layer_transparency(ALPHA_150, LAYER_TWO);

	/* Resume the RTOS tasks that are required for this Screen */
	resume_rtos_task(&activity_map_tsk_ctrl);
	resume_rtos_task(&event_decoding_tsk_ctrl);
	resume_rtos_task(&update_fb_tsk_ctrl);

	/* Suspend the RTOS tasks that are not required for this Screen */
	suspend_rtos_task(&statistics_tsk_ctrl);
	suspend_rtos_task(&esp_lpw_tsk_ctrl);
	suspend_rtos_task(&led_tsk_ctrl);

	/* Disable the Anti-FlicKering filter */
	psee_disable_afk();

	/* Operation Mode Configuration */
	psee_PM3C_config();

}

/**
 * @brief Configuration function for Mode selection screen.
 */
void enter_view2()
{
	/* Update the transparency for the layer 2 */
	set_layer_transparency(ALPHA_150, LAYER_TWO);

	/* Resume the RTOS tasks that are required for this Screen */
	resume_rtos_task(&event_decoding_tsk_ctrl);
	resume_rtos_task(&update_fb_tsk_ctrl);

	/* Suspend the RTOS tasks that are not required for this Screen */
	suspend_rtos_task(&activity_map_tsk_ctrl);
	suspend_rtos_task(&statistics_tsk_ctrl);
	suspend_rtos_task(&esp_lpw_tsk_ctrl);
	suspend_rtos_task(&led_tsk_ctrl);

	/* Operation Mode Configuration */
	psee_PM3C_config();

	/* Set Default Bias */
	psee_sensor_set_biases(&genx320_default_biases);

	/* Set the default border for the Activity map */
	psee_set_default_XY_borders(&genx320mp_default_am_borders);
}

/**
 * @brief Configuration function for PM2 mode screen.
 */
void enter_PM2_view()
{
	/* Update the transparency for the layer 2 */
	set_layer_transparency(ALPHA_0, LAYER_TWO);

	/* Resume the RTOS tasks that are required for this Screen */
	resume_rtos_task(&esp_lpw_tsk_ctrl);

	/* Suspend the RTOS tasks that are not required for this Screen */
	suspend_rtos_task(&event_decoding_tsk_ctrl);
	suspend_rtos_task(&update_fb_tsk_ctrl);
	suspend_rtos_task(&activity_map_tsk_ctrl);
	suspend_rtos_task(&statistics_tsk_ctrl);
	suspend_rtos_task(&led_tsk_ctrl);

	/* Set the default border for the Activity map */
	psee_set_default_XY_borders(&genx320mp_default_am_borders);

	/* Flash the RISC-V Firmware */
	if (active_firmware != fw_esp_wakeup) {

	    /* Reset the CPU if it is a new firmware to be flashed */
		psee_reset_firmware(genx320_boot_mode);

	    /* Flash the RISC-V Firmware */
		FW_StatusTypeDef status = psee_write_firmware(fw_esp_wakeup, fw_esp_wakeup_size);

		/* Update active firmware */
		active_firmware = fw_esp_wakeup;

		if(status == fw_OK) {
			/* Start the RISC-V Firmware */
			psee_start_firmware(genx320_boot_mode);
		}
	}

	/* Operation Mode Configuration */
	psee_PM2_config();

}

/**
 * @brief Configuration function for PM0 mode screen.
 */
void enter_PM0_view()
{

	/* Update the transparency for the layer 2 */
	set_layer_transparency(ALPHA_0, LAYER_TWO);

	/* Suspend the RTOS tasks that are not required for this Screen */
	suspend_rtos_task(&event_decoding_tsk_ctrl);
	suspend_rtos_task(&update_fb_tsk_ctrl);
	suspend_rtos_task(&activity_map_tsk_ctrl);
	suspend_rtos_task(&statistics_tsk_ctrl);
	suspend_rtos_task(&led_tsk_ctrl);
	suspend_rtos_task(&event_decoding_tsk_ctrl);

	/* Update active firmware */
	active_firmware = NULL;

}

/**
 * @brief Configuration function for LED tracking application screen.
 */
void enter_LT_view()
{
	/* Update the transparency for the layer 2 */
	set_layer_transparency(ALPHA_150, LAYER_TWO);

	/* Resume the RTOS tasks that are required for this Screen */
	resume_rtos_task(&event_decoding_tsk_ctrl);
	resume_rtos_task(&update_fb_tsk_ctrl);
	resume_rtos_task(&led_tsk_ctrl);

	/* Suspend the RTOS tasks that are not required for this Screen */
	suspend_rtos_task(&activity_map_tsk_ctrl);
	suspend_rtos_task(&statistics_tsk_ctrl);
	suspend_rtos_task(&esp_lpw_tsk_ctrl);

	/* Disable the Anti-FlicKering filter */
	psee_disable_afk();

	/* Operation Mode Configuration */
	psee_PM3C_config();

	/* Flash the RISC-V Firmware */
	if (active_firmware != fw_led_tracking) {

	    /* Reset the CPU if it is a new firmware to be flashed */
		psee_reset_firmware(genx320_boot_mode);

	    /* Flash the RISC-V Firmware */
		FW_StatusTypeDef status = psee_write_firmware(fw_led_tracking, fw_led_tracking_size);

		/* Update active firmware */
		active_firmware = fw_led_tracking;

		if(status == fw_OK) {
			/* Start the RISC-V Firmware */
			psee_start_firmware(genx320_boot_mode);
		}
	}

	/* Change Bias */
	psee_sensor_set_bias(HPF, 127);

}
