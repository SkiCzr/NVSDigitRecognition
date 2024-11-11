/**
 ******************************************************************************
 * @file    drv_i2c.c
 * @author  PSEE Applications Team
 * @brief	I2C Driver
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
#include "drv_i2c.h"

/* Instance of I2C driver */
typedef struct xDrvI2c StaticDrvI2c_t;

/**
 * @brief Function to Allocate and initialize a DrvI2c driver.
 */
BaseType_t xDrvI2cCreate(
		I2C_HandleTypeDef *pxHdlI2c,
		void* (*allocator) (size_t size), DrvI2cHandle_t *pxDrvI2cHandle)
{
	assert(pxHdlI2c && allocator && pxDrvI2cHandle);

	/* Allocate the driver and a semaphore. */
	StaticDrvI2c_t *pxDrvI2c = (StaticDrvI2c_t *) allocator(sizeof(StaticDrvI2c_t));
	StaticSemaphore_t *xStaticSemphIsrHdl  = allocator(sizeof(StaticSemaphore_t));
	StaticSemaphore_t *xStaticSemphLockHdl = allocator(sizeof(StaticSemaphore_t));
	assert(pxDrvI2c && xStaticSemphIsrHdl && xStaticSemphLockHdl);

	/* Initialize the semaphore with ISR and take it. */
	pxDrvI2c->xSemphIsrHdl = xSemaphoreCreateBinaryStatic( xStaticSemphIsrHdl );
	assert(pxDrvI2c->xSemphIsrHdl);
	/* The sematphore shall be given before the first take. */

	/* Initialize the lock semaphore. */
	pxDrvI2c->xMutexLockHdl = xSemaphoreCreateMutexStatic( xStaticSemphLockHdl );
	assert(pxDrvI2c->xSemphIsrHdl);
	pxDrvI2c->pxHdlI2c = pxHdlI2c;

	*pxDrvI2cHandle = (DrvI2cHandle_t) pxDrvI2c;
	return pdTRUE;
}

/**
 * @brief Function to read data in non-blocking mode with Interrupt
 * @param xDrvI2cHdl I2C Driver Handle
 * @param uhDevAddr Target device address
 * @param ucRegAddr Sensor register address
 * @param pucData Pointer to data buffer
 * @param ulNbBytes Amount of data to be received
 */
BaseType_t ucDrvI2cReadBytes(DrvI2cHandle_t xDrvI2cHdl, uint16_t uhDevAddr, uint16_t ucRegAddr, uint8_t *pucData, size_t ulNbBytes)
{
	BaseType_t status;
	struct xDrvI2c *pxDrvI2c = (struct xDrvI2c *) xDrvI2cHdl;
	assert(pxDrvI2c && pucData);

	uint16_t regAddr = ucRegAddr;

	/* Acquire the lock. */
	status = xSemaphoreTake( pxDrvI2c->xMutexLockHdl, portMAX_DELAY );
	if (status != pdTRUE)
		return status;

	HAL_I2C_Master_Transmit_IT( pxDrvI2c->pxHdlI2c, uhDevAddr, (uint8_t *)&regAddr, 2);
	status = xSemaphoreTake( pxDrvI2c->xSemphIsrHdl, I2C_MAX_WAIT_TIME_PER_BYTE );
	if (status != pdTRUE)
		goto label_abort;
	HAL_I2C_Master_Receive_IT( pxDrvI2c->pxHdlI2c, uhDevAddr, pucData, ulNbBytes);
	status = xSemaphoreTake( pxDrvI2c->xSemphIsrHdl, I2C_MAX_WAIT_TIME_PER_BYTE * ulNbBytes);

	label_abort:
	/* Release the lock. */
	xSemaphoreGive( pxDrvI2c->xMutexLockHdl );
	return status;
}

/**
 * @brief Function to Write data in non-blocking mode with Interrupt
 * @param xDrvI2cHdl I2C Driver Handle
 * @param uhDevAddr Target device address
 * @param pucData Pointer to data buffer
 * @param nb_bytes Amount of data to be sent
 */
BaseType_t ucDrvI2cWriteBytes(DrvI2cHandle_t xDrvI2cHdl, uint16_t uhDevAddr, uint8_t *pucData, size_t nb_bytes)
{
	BaseType_t status;
	struct xDrvI2c *pxDrvI2c = (struct xDrvI2c *) xDrvI2cHdl;
	assert(pxDrvI2c);

	/* Acquire the lock. */
	status = xSemaphoreTake( pxDrvI2c->xMutexLockHdl, portMAX_DELAY );
	if (status != pdTRUE)
		return status;

	HAL_I2C_Master_Transmit_IT( pxDrvI2c->pxHdlI2c, uhDevAddr, pucData, nb_bytes);
	status = xSemaphoreTake( pxDrvI2c->xSemphIsrHdl, I2C_MAX_WAIT_TIME_PER_BYTE );

	/* Release the lock. */
	xSemaphoreGive( pxDrvI2c->xMutexLockHdl );
	return status;

}

/**
 * @brief Call back function for I2Cs transfer complete.
 * @param xDrvI2cHdl I2C Driver Handle
 */
BaseType_t ucDrvI2cCallback(DrvI2cHandle_t xDrvI2cHdl)
{
	struct xDrvI2c *pxDrvI2c = (struct xDrvI2c *) xDrvI2cHdl;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR( pxDrvI2c->xSemphIsrHdl, &xHigherPriorityTaskWoken);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	return xHigherPriorityTaskWoken;
}
