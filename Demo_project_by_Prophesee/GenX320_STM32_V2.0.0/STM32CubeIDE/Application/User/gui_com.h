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

#ifndef APPLICATION_USER_GUI_COM_H_
#define APPLICATION_USER_GUI_COM_H_

/* Standard Includes */
#include <stdint.h>
#include <stdio.h>

/* RTOS Includes */
#include "cmsis_os.h"
#include "semphr.h"
#include "drv_i2c.h"

/* HAL Includes */
#include "stm32f7xx_hal.h"

/* TouchGFX Buffers */
#define GUI_BUF_ADD_1		0xC0000000
#define GUI_BUF_ADD_2		0xC003FC00

/* Power Consumption Interpolate Defines */
#define MIN_MEVTS 			0
#define MAX_MEVTS 			10000
#define	MIN_PC 				2920
#define MAX_PC 				8680

/* Color Interpolate Defines */
#define MIN_EVTS			0
#define MAX_EVTS			7000

/* LTDC Layer Defines */
#define ALPHA_150 			150
#define ALPHA_0 			0
#define LAYER_ONE 			0
#define LAYER_TWO 			1

/* ESP Defines */
#define LOCKED 				1
#define UNLOCKED 			0
#define TRUE_WAKE_UP 		0
#define FALSE_WAKE_UP 		1
#define SLEEP_STATE 		2

/* Structure for Display Color */
typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}Display_Color;

/* Exported Function Prototypes */
extern void enable_lcd_layer_event(LTDC_HandleTypeDef *hltdc, unsigned int layer, void *address);
extern void enable_lcd_layer_histo(LTDC_HandleTypeDef *hltdc, unsigned int layer, void *address);
extern void enable_lcd_layer_diff(LTDC_HandleTypeDef *hltdc, unsigned int layer, void *address);
extern UBaseType_t get_lock_sem_count(DrvI2cHandle_t xDrvI2cHdl);
extern HAL_StatusTypeDef set_layer_transparency(uint32_t Alpha, uint32_t LayerIdx);
extern uint16_t pc_interpolate(uint16_t input, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
extern Display_Color color_interpolate(uint16_t input, uint16_t x1, uint16_t x2, const Display_Color color1, const Display_Color color2);

#endif /* APPLICATION_USER_GUI_COM_H_ */
