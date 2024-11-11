/**
 ******************************************************************************
 * @file    psee_sensor_wrapper.c
 * @author  PSEE Applications Team
 * @brief	Platform specific wrapper functions
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

/* RTOS Includes */
#include <FreeRTOS.h>
#include "drv_i2c.h"

/* HAL Includes */
#include "stm32f7xx_hal.h"

/* User Includes */
#include "genx320.h"

/* Private Defines */
#define TIO_start		100
#define Tana_start		100
#define Tclk_start		200
#define trstn_start		100

#define tclk_stop		100
#define Tana_stop		100
#define TIO_stop		100
#define Tdig_stop		100

#define DCMI_PWR_EN_Pin GPIO_PIN_13
#define DCMI_PWR_EN_GPIO_Port GPIOH

/* Imported Handles */
extern DrvI2cHandle_t drv_i2c;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

/**
 * @brief Platform dependent sleep function for delay in us.
 * @param duration Time delay in microseconds
 */
static void psee_sleep_us_imp(uint32_t duration) {

	/* Get the current value of the counters */
	uint32_t tickstart = __HAL_TIM_GET_COUNTER(&htim2);

	/* Get the value of the delay */
	uint32_t delay = duration;

	/* Wait for the delay */
	while ((__HAL_TIM_GET_COUNTER(&htim2) - tickstart) < delay);

}

/**
 * @brief Platform dependent sleep function for delay in ms.
 * @param duration Time delay in milliseconds
 */
void psee_sleep_ms_imp(uint32_t duration) {
	HAL_Delay(duration);
}

/**
 * @brief Platform dependent function to perform a single-write operation to the sensor's register.
 * @param reg Sensor's register to which the data needs to be written
 * @param data Data to be written
 */
void psee_sensor_write(uint16_t reg, uint32_t data) {
	uint16_t buf[3];

	/* Write reg addr */
	buf[0] = __builtin_bswap16 (reg);

	/* Write value */
	buf[1] = __builtin_bswap16 ((data >> 16));
	buf[2] = __builtin_bswap16 ((data & 0xFFFF));

	BaseType_t status = ucDrvI2cWriteBytes(drv_i2c, (I2C_ADDRESS << 1), (uint8_t *) buf, sizeof(buf));
	assert(status == pdTRUE);
}

/**
 * @brief Platform dependent function to perform a single-read operation from the sensor's register.
 * @param reg Sensor's register from which the data needs to be read
 * @param data Pointer to the variable where the data needs to be stored
 */
void psee_sensor_read(uint16_t reg, uint32_t* data) {
	uint16_t recv[2] = {0};

	uint16_t swap_reg = __builtin_bswap16 (reg);
	BaseType_t status = ucDrvI2cReadBytes(drv_i2c, (I2C_ADDRESS << 1),
			swap_reg, (uint8_t *)recv, sizeof(uint32_t));
	assert(status == pdTRUE);

	*data = (__builtin_bswap16(recv[0]) << 16) + (__builtin_bswap16 (recv[1]));
}

/**
 * @brief Platform dependent function to perform a sequential-read operation from the sensor's register.
 * @param reg Sensor's starting register address from which the data needs to be read
 * @param data Pointer to the array where the data needs to be stored
 * @param n_word Total number of bytes that needs to be read
 */
void psee_sensor_sequential_read(uint16_t reg, uint32_t* data, uint16_t n_word) {

	BaseType_t status = ucDrvI2cReadBytes(drv_i2c, (I2C_ADDRESS << 1),
			__builtin_bswap16 (reg), (uint8_t *) data, n_word * sizeof(uint32_t));
	assert(status == pdTRUE);

	for(uint32_t d_index = 0; d_index < n_word; d_index ++ ) {
		uint16_t *p_data = (uint16_t *) &data[d_index];
		data[d_index] = (__builtin_bswap16 (p_data[0]) << 16) + (__builtin_bswap16 (p_data[1]));
	}

}

/**
 * @brief Platform dependent function to perform a sequential-write operation to the sensor's register.
 * @param register_data Address of the data array that needs to be written
 * @param n_bytes Total number of bytes that needs to be written
 */
void psee_sensor_sequential_write(uint8_t *register_data, uint16_t n_bytes) {

	BaseType_t status = ucDrvI2cWriteBytes(drv_i2c, (I2C_ADDRESS << 1),
			register_data, n_bytes);
	assert(status == pdTRUE);
}

/**
 * @brief Platform dependent initialization routine.
 */
void psee_sensor_platform_init() {

	/*-------------------- Without Modified Hardware-----------------------------*/

	HAL_GPIO_WritePin(DCMI_PWR_EN_GPIO_Port, DCMI_PWR_EN_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);

	/* Turn on power supplies of Saphir (PSU_EN) */
	HAL_GPIO_WritePin(DCMI_PWR_EN_GPIO_Port, DCMI_PWR_EN_Pin, GPIO_PIN_SET);
	HAL_Delay(1);


	/*-------------------- With Modified Hardware-----------------------------*/

	/*
	 * @note ULP_ENB would be already logic high.
	 */

	/* Switch ON PSU_EN_1V1 */
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET);
	psee_sleep_us_imp(TIO_start);

	/* Switch ON PSU_EN_1V8 */
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET);
	psee_sleep_us_imp(Tana_start);

	/* Switch ON PSU_EN_2V5 */
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET);

	/* Toggle ON ULP_ENB */
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET);
	psee_sleep_us_imp(Tclk_start);

	/* Start the 10MHz PWM signal */
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	psee_sleep_us_imp(trstn_start);

	/* Toggle ON RSTN */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
}


