/**
 ******************************************************************************
 * @file    drv_genx320.c
 * @author  PSEE Applications Team
 * @brief	Genx320 RTOS Driver
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
#include "drv_genx320.h"

/* Structure for GenX320 driver */
struct xDrvGenx320 {
	SemaphoreHandle_t xMutexLockHdl;
	bool bInitDone;
};

/* Instance of GenX320 driver */
typedef struct xDrvGenx320 StaticDrvGenx320_t;

/* Current ISSD */
const struct issd *current_issd = NULL;

/**
 * @brief Function to Allocate and initialize a DrvGenx320 driver.
 */
BaseType_t xDrvGenx320Create(
		void* (*allocator) (size_t size),
		DrvGenx320Handle_t *pxDrvGenx320Handle)
{
	assert(allocator && pxDrvGenx320Handle);

	/* Allocate the driver and a semaphore. */
	StaticDrvGenx320_t *pxDrvGenx320 = (StaticDrvGenx320_t *) allocator(sizeof(StaticDrvGenx320_t));
	assert(pxDrvGenx320);

	/* Initialize the lock semaphore. */
	StaticSemaphore_t *xStaticSemphLockHdl = allocator(sizeof(StaticSemaphore_t));
	assert(xStaticSemphLockHdl);
	pxDrvGenx320->xMutexLockHdl = xSemaphoreCreateMutexStatic( xStaticSemphLockHdl );
	assert(pxDrvGenx320->xMutexLockHdl);

	pxDrvGenx320->bInitDone = false;

	*pxDrvGenx320Handle = (DrvGenx320Handle_t) pxDrvGenx320;
	return pdTRUE;
}

/**
 * @brief Function to Initialize the GenX320 driver.
 * @param xDrvGenx320Handle GenX320 Driver Handle
 */
BaseType_t ucDrvGenx320Init(DrvGenx320Handle_t xDrvGenx320Handle)
{
	struct xDrvGenx320 *pxDrvGenx320 = (struct xDrvGenx320 *) xDrvGenx320Handle;
	BaseType_t status;

	/* Acquire the lock. */
	status = xSemaphoreTake( pxDrvGenx320->xMutexLockHdl, portMAX_DELAY );
	if (status != pdTRUE)
		return status;

	/* Initialize the sensor */
	current_issd = psee_open_evt();

	/* Set Standard biases */
	psee_sensor_set_biases(&genx320_default_biases);

	/* Start the sensor */
	psee_sensor_start(current_issd);

	/* Set Flip */
	psee_sensor_set_flip(0,0);

	/* Release the lock. */
	xSemaphoreGive( pxDrvGenx320->xMutexLockHdl );
	return pdTRUE;
}

/**
 * @brief Function to Disable AFK
 * @param xDrvGenx320Handle GenX320 Driver Handle
 */
BaseType_t ucDrvGenx320DisableAfk(DrvGenx320Handle_t xDrvGenx320Handle)
{
	struct xDrvGenx320 *pxDrvGenx320 = (struct xDrvGenx320 *) xDrvGenx320Handle;
	BaseType_t status;

	/* Acquire the lock. */
	status = xSemaphoreTake( pxDrvGenx320->xMutexLockHdl, portMAX_DELAY );
	if (status != pdTRUE)
		return status;

	if (pxDrvGenx320->bInitDone == true)
		psee_disable_afk();
	else
		status = pdFALSE;

	/* Release the lock. */
	xSemaphoreGive( pxDrvGenx320->xMutexLockHdl );
	return status;
}
