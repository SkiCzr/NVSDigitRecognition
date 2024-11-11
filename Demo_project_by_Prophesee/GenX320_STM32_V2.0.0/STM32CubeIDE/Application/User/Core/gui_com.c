/**
 ******************************************************************************
 * @file    gui_com.c
 * @author  PSEE Applications Team
 * @brief   GenX320 GUI communication driver.
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
#include "gui_com.h"

/* Imported Handles */
extern LTDC_HandleTypeDef hltdc;

/**
 * @brief Color look-up table for Event streaming mode.
 */
static const uint32_t clut_event[] = {
		((30UL << 16) | (37UL << 8) | (52UL)), 		// Background
		((216UL << 16) | (223UL << 8) | (236UL)), 	// ON event
		((64UL << 16) | (126UL << 8) | (201UL)) 	// OFF event
};

/**
 * @brief Function to configure and enable the specified lcd layer.
 * @param hltdc Pointer to a LTDC_HandleTypeDef structure
 * @param layer LTDC Layer index
 * @param address Pointer to the address of the frame buffer
 * @param p_lut Pointer to the color lookup table address
 * @param lut_entries Size of the color lookup table
 */
static void enable_lcd_layer(LTDC_HandleTypeDef *hltdc, unsigned int layer, void *address, const uint32_t *p_lut, size_t lut_entries)
{
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
	HAL_LTDC_ConfigCLUT(hltdc, p_lut, lut_entries, layer);
	HAL_LTDC_EnableCLUT(hltdc, layer);
}

/**
 * @brief Function to set transparency for the specified layer.
 * @param Alpha New transparency value
 * @param LayerIdx LTDC Layer index
 */
HAL_StatusTypeDef set_layer_transparency(uint32_t Alpha, uint32_t LayerIdx) {

	HAL_StatusTypeDef status;
	status = HAL_LTDC_SetAlpha(&hltdc, Alpha, LayerIdx);

	return status;

}

/**
 * @brief Function to enable the specified lcd layer for event streaming.
 * @param hltdc Pointer to a LTDC_HandleTypeDef structure
 * @param layer LTDC Layer index
 * @param address Pointer to the address of the frame buffer
 */
void enable_lcd_layer_event(LTDC_HandleTypeDef *hltdc, unsigned int layer, void *address)
{
	enable_lcd_layer(hltdc, layer, address, clut_event,  sizeof(clut_event)/sizeof(uint32_t));
	set_layer_transparency(ALPHA_150, layer);
}

/**
 * @brief Function to get the Semaphore count.
 * @param xDrvI2cHdl I2c driver handle
 * @retval Semaphore count
 */
UBaseType_t get_lock_sem_count(DrvI2cHandle_t xDrvI2cHdl) {
	UBaseType_t count;

	struct xDrvI2c *pxDrvI2c = (struct xDrvI2c *) xDrvI2cHdl;
	assert(pxDrvI2c);

	/* Get the semaphore count of the lock */
	count = uxSemaphoreGetCount(pxDrvI2c->xMutexLockHdl);

	return count;
}

/**
 * @brief Function to interpolate the power consumption based on the event rate.
 * @param input Input event rate
 * @param x1 Minimum event rate in Mevt/s
 * @param x2 Maximum event rate in Mevt's
 * @param y1 Minimum power consumption in mW
 * @param y2 Maximum power consumption in mW
 * @retval Interpolated power consumption in mW
 */
uint16_t pc_interpolate(uint16_t input, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2) {

	/* Perform linear interpolation */
	double slope = (double)(y2 - y1) / (x2 - x1);
	uint16_t output = (uint16_t)(y1 + (slope * (input - x1)));

	return output;

}

/**
 * @brief Function to interpolate the counter color based on the event count.
 * @param input Input event count
 * @param x1 Minimum event count
 * @param x2 Maximum event count
 * @param y1 RGB code for no activity
 * @param y2 RGB code for maximum activity
 * @retval Interpolated RGB code
 */
Display_Color color_interpolate(uint16_t input, uint16_t x1, uint16_t x2, const Display_Color color1, const Display_Color color2) {

	Display_Color result;
	double slope_red = (double)(color2.red - color1.red) / (x2 - x1);
	double slope_green = (double)(color2.green - color1.green) / (x2 - x1);
	double slope_blue = (double)(color2.blue - color1.blue) / (x2 - x1);

	result.red = (uint16_t)(color1.red + (slope_red * (input - x1)));
	result.green = (uint16_t)(color1.green + (slope_green * (input - x1)));
	result.blue = (uint16_t)(color1.blue + (slope_blue * (input - x1)));

	return result;

}
