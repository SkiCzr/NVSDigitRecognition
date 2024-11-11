/**
 ******************************************************************************
 * @file    task_frame_buffer.c
 * @author  PSEE Applications Team
 * @brief	Frame buffer specific Functions
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
#include "task_frame_buffer.h"
#include "predictor.h"
#include <time.h>
/* Private Defines */
#define DISPLAY_RESOLUTION (320*272)

/* 3 Display Frame Buffers */
static uint8_t frame_buffer_l1_one[DISPLAY_RESOLUTION] __attribute__((section("L1_Frame_Buffers"))) __attribute__((aligned(4)));
static uint8_t frame_buffer_l1_two[DISPLAY_RESOLUTION] __attribute__((section("L1_Frame_Buffers"))) __attribute__((aligned(4)));
static uint8_t frame_buffer_l1_three[DISPLAY_RESOLUTION] __attribute__((section("L1_Frame_Buffers"))) __attribute__((aligned(4)));
long unsigned int clock_counter = 1;
float buffer_number[28][28];
uint8_t* volatile fb_cpi = frame_buffer_l1_three;
static int prediction = 0;

/* This semaphore is used to notify the application that the new
 * frame buffer is displayed, a new one can be prepared...
 */
static SemaphoreHandle_t xSemphFbUpdtHdl = NULL;

/**
 * @brief Color look-up table for Events streaming mode
 */
static const uint32_t clut_event[] = {
		((30UL << 16) | (37UL << 8) | (52UL)), 		// Background
		((216UL << 16) | (223UL << 8) | (236UL)), 	// ON event
		((64UL << 16) | (126UL << 8) | (201UL)) 	// OFF event
};

/**
 * @brief Function to enable the LCD layer
 * @param hltdc Pointer to a LTDC_HandleTypeDef structure
 * @param layer LTDC Layer index
 * @param address Pointer to the address of the frame buffer
 */
static void enable_layer(LTDC_HandleTypeDef *hltdc, unsigned int layer, void *address) {

	LTDC_LayerCfgTypeDef pLayerCfg = {0};

	pLayerCfg.WindowX0 = 160;
	pLayerCfg.WindowX1 = 480;
	pLayerCfg.WindowY0 = 0;
	pLayerCfg.WindowY1 = 272;
	pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_L8; //LTDC_PIXEL_FORMAT_RGB565;
	pLayerCfg.Alpha = 255;
	pLayerCfg.Alpha0 = 0;
	pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
	pLayerCfg.FBStartAdress = (uint32_t)address;
	pLayerCfg.ImageWidth = 320;
	pLayerCfg.ImageHeight = 272;
	pLayerCfg.Backcolor.Blue = 0;
	pLayerCfg.Backcolor.Green = 0;
	pLayerCfg.Backcolor.Red = 0;
	HAL_LTDC_ConfigLayer(hltdc, &pLayerCfg, layer);
	HAL_LTDC_ConfigCLUT(hltdc, (uint32_t *)clut_event, sizeof(clut_event)/sizeof(uint32_t), layer);
	HAL_LTDC_EnableCLUT(hltdc, layer);
}

/**
 * @brief Function to Reset the frame buffer. It must be done before scheduling starts.
 * It can not be done by DMA.
 */
void reset_frame_buffers() {

	memset(frame_buffer_l1_one, 0 , sizeof(frame_buffer_l1_one));
	memset(frame_buffer_l1_two, 0 , sizeof(frame_buffer_l1_two));
	memset(frame_buffer_l1_three, 0 , sizeof(frame_buffer_l1_three));

}

/**
 * @brief Function implementing the Update frame buffer task.
 */
extern SemaphoreHandle_t tskCtlUpdateFbHandle;
void task_update_fb(const args_update_fb_t *args) {
	uint32_t time_taken;
	uint32_t end_time;
	uint32_t start_time;
	uint32_t start_time2;
	uint8_t* fb_display = frame_buffer_l1_one;
	uint8_t* fb_reset = frame_buffer_l1_two;
	uint8_t* old_fb_cpi;
	DrvDmaHandle_t drv_dma = *(args->drv_dma);
	BaseType_t status;
	StaticSemaphore_t xSemaphoreBuffer;

	xSemphFbUpdtHdl = xSemaphoreCreateBinaryStatic( &xSemaphoreBuffer );
	assert(xSemphFbUpdtHdl);

	enable_layer(args->hltdc, 1, fb_display);

	uint8_t starting_x = 0; //   x and y coordinates for selecting the
			uint8_t starting_y = 0; //   region of the display to be used in recognition
	/* Infinite loop */
	for(;;)
	{

		start_time2 = HAL_GetTick();
		fb_display = frame_buffer_l1_one;

		/* This semaphore is used to suspend the task. */
		status = xSemaphoreTake(tskCtlUpdateFbHandle, portMAX_DELAY  );
		assert(status == pdTRUE);

		/* Swap CPI Buffer, Reset Buffer and display buffer */
		old_fb_cpi = fb_cpi;
		fb_cpi = fb_reset;

		fb_reset = fb_display;
		fb_display = old_fb_cpi;

		/* Wait for register reload interrupt ( Generated when shadow register reload is performed ) */
		HAL_LTDC_Reload(args->hltdc, LTDC_RELOAD_VERTICAL_BLANKING);

		/* Notify the display and change fb address */
		LTDC_LAYER(args->hltdc, 1)->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
		LTDC_LAYER(args->hltdc, 1)->CFBAR = (uint32_t)(fb_display);

		status = xSemaphoreTake( xSemphFbUpdtHdl, 50 / portTICK_PERIOD_MS );
		assert(status == pdTRUE);

		status = xSemaphoreGive(tskCtlUpdateFbHandle);
		assert(status == pdTRUE);

		/* Start Reset */
		ucDrvDmaMemsetTo0(drv_dma, fb_reset, sizeof(frame_buffer_l1_one));

		vTaskDelay( 12 / portTICK_PERIOD_MS);

		// Start CNN cycle, every 30 loops, a frame is pre-processed and fed to the CNN.
		if (clock_counter % 30 == 0) {
			start_time = HAL_GetTick();
			for (int8_t i = 0; i < 28; i++){ //This nested for loop takes a 252 by 252 square region from the display starting given starting_x and starting_y coordinates. It's set to start from top-left corner of the screen.
								for (int8_t j = 0; j < 28; j++){
									//starting from pixel (0,0) and going up to (251,251). We are down sizing to 28x28.
									//2nd index is 27-j instead of j, so that the image is mirrored before being processed. A threshold of 30 is applied in order to remove noise.
									buffer_number[starting_x + i][starting_y + (27-j)] = (calculate_the_weight_of_a_9by9_block(frame_buffer_l1_one, 9 * i, 9 * j) < 30) ? 0 : 1;
								}
							}
			end_time = HAL_GetTick();
			time_taken = end_time - start_time;
			printf("Time PREPROCESSING: %lu ms \n", time_taken);
			prediction = predict_main(buffer_number);
			end_time = HAL_GetTick();
			time_taken = end_time - start_time;
			printf("Time taken to execute: %lu ms \n", time_taken);
		}
		clock_counter += 1;

	}
	end_time = HAL_GetTick();
	printf("TOTAL TIME: %lu ms \n",end_time - start_time2);
}

/**
 * @brief Call back function for register reload
 */
void update_fb_callback()
{
	BaseType_t xHigherPriorityTaskWoken_RR = pdFALSE;
	xSemaphoreGiveFromISR( xSemphFbUpdtHdl, &xHigherPriorityTaskWoken_RR);

	portYIELD_FROM_ISR( xHigherPriorityTaskWoken_RR );
}

int get_prediction() {
	return prediction;
}


