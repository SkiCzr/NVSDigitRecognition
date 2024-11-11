/**
 ******************************************************************************
 * @file    task_decoder.c
 * @author  PSEE Applications Team
 * @brief	Decoder task specific Functions
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
#include "task_decoder.h"

/* Configuration */
#define configCALIBRATE_SENSOR  0

/* Private Defines */
#define WIDTH 					320
#define HEIGHT 					272
#define TX_SIZE 				2048*4 // EVT2.0
#define COLOR_ON  				(1)
#define COLOR_OFF 				(2)

/* Statistics Variables */
volatile uint32_t evt_statistics[2] = {0};

/* Calibration Variables */
volatile uint32_t ev_threshold_to_calibrate = 100000;

/* Event Buffer */
static uint32_t event_buffer[TX_SIZE] __ALIGNED(32) = {0};

/* 8 bit Histogram Buffer */
uint8_t histogram[320*320] = {0};

/* Global Variables */
uint32_t nbev_rate = 0;
uint32_t ev_processed = 0;
uint32_t calib_done = 0;

/**
 * @brief Function to disable Hot pixels
 * @param histogram Pointer to the histogram buffer
 */
void disable_hot_pixels(uint8_t *histogram) {

	/* Compute average */
	uint32_t val = 0;
	uint8_t max = 0;
	for(uint32_t j = 0 ; j < 320 ; j++) {
		for(uint32_t i = 0 ; i < 320 ; i++) {
			val += histogram[i+j*320];
			if(histogram[i+j*320] > max) {
				max = histogram[i+j*320];
			}
		}
	}
	val /= (320*320);

	/* Compute std */
	uint32_t std = 0;
	for(uint32_t j = 0 ; j < 320 ; j++) {
		for(uint32_t i = 0 ; i < 320 ; i++) {
			std += (histogram[i+j*320]-val/100)*(histogram[i+j*320]-val);
		}
	}
	std /= (320*320);
	std = sqrt(std);

	const uint32_t threshold = val+std/2;

	uint32_t disable_cnt = 0;
	for(uint32_t j = 0 ; j < 320 ; j++) {

		/* Reset all blocks */
		for(uint8_t i = 0 ; i < 10 ; i++) {
			psee_write_ROI_X(4*i, 0x00000000);
		}

		/* Select line */
		uint32_t offset = j/32;
		psee_write_ROI_Y(4*offset, 1 << (j%32));

		/* Trigger shadow */
		psee_write_ROI_CTRL(0x420);

		uint32_t tmp[10] = {0};
		for(uint32_t i = 0 ; i < 320 ; i++) {
			if( histogram[i+j*320] > threshold) {
				uint32_t offset_x = i/32;
				tmp[offset_x] |= 1 << (i%32);
				disable_cnt++;
			}
		}

		/* Write x values to disable */
		for(uint32_t xpos = 0 ; xpos < 10 ; xpos++) {
			psee_write_ROI_X(4*xpos, tmp[xpos]);
		}

		/* Activate block */
		psee_write_ROI_CTRL(0x422);

		/* Disable roi block */
		psee_write_ROI_CTRL(0x400);
		psee_write_ROI_Y(4*offset, 0);

	}
}

/**
 * @brief Function to decode EVT2.0.
 * @param Value to be decoded
 */
static void decode_evt2(const uint32_t val) {
	static uint32_t x,y;

	switch( __EVT20_TYPE(val)) {
	case TD_LOW:
	{
		y = __EVT20_X(val);
		x = __EVT20_Y(val);
#if( configCALIBRATE_SENSOR == 1 )
		if(histogram[y+x*320] < 255) histogram[y+x*320]++;
#endif
		if( x < WIDTH && y < HEIGHT) {
			fb_cpi[x+y*WIDTH] = COLOR_OFF;
		}
		evt_statistics[0] ++;
		break;
	}
	case TD_HIGH:
	{
		y = __EVT20_X(val);
		x =  __EVT20_Y(val);
#if( configCALIBRATE_SENSOR == 1 )
		if(histogram[y+x*320] < 255) histogram[y+x*320]++;
#endif
		if( x < WIDTH && y < HEIGHT) {
			fb_cpi[x+y*WIDTH] = COLOR_ON;
		}
		evt_statistics[1] ++;
		break;
	}

	case (EV_TIME_HIGH):
			{
		break;
			}
	case (EXT_TRIGGER):
			{
		break;
			}
	default:
		break;
	}
}

/**
 * @brief Function implementing Event decoding task.
 */
void task_evt_decoder(const args_evt_decoder_t *args) {

	uint32_t dma_read_idx = 0;
	uint32_t now = 0, last = 0;
	uint32_t ev_processed_old = 0;
	DrvGenx320Handle_t drv_genx320 = *(args->drv_genx320);

	/* Initialize the sensor */
	ucDrvGenx320Init(drv_genx320);

	/* Start DCMI_DMA */
	while(HAL_DCMI_Start_DMA(args->hdcmi, DCMI_MODE_CONTINUOUS , (uint32_t)event_buffer, TX_SIZE) != HAL_OK) {
		HAL_Delay(10);
	}

	/* Infinite loop */
	for(;;)	{

		/* Computing Event Rate */
		now = HAL_GetTick();

		if(now - last > 50) {

			ev_processed = (evt_statistics[0] + evt_statistics[1]);
			uint32_t nbev = ev_processed - ev_processed_old;
			nbev_rate = nbev/(now-last);
			ev_processed_old = ev_processed;
			last = now;

		}

		/* Event Decoding */
		uint32_t dma_idx = TX_SIZE - args->hdma_dcmi->Instance->NDTR;

		if (dma_read_idx == dma_idx) {
			vTaskDelay( 3 / portTICK_PERIOD_MS);
			continue;
		}

		if(dma_read_idx > dma_idx) {
			while (dma_read_idx < TX_SIZE) {
				decode_evt2(event_buffer[dma_read_idx++]);
			}
			dma_read_idx = 0;
		}

		while (dma_read_idx < dma_idx) {
			decode_evt2(event_buffer[dma_read_idx++]);
		}

#if( configCALIBRATE_SENSOR == 1 )

		/* Calibration */
		if((ev_processed > ev_threshold_to_calibrate) && (!calib_done)) {

			disable_hot_pixels(histogram);
			calib_done = 1;
		}
#endif

	}

}



