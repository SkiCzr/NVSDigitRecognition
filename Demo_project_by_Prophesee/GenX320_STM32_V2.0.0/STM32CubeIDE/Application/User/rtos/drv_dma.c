/**
 ******************************************************************************
 * @file    drv_dma.c
 * @author  PSEE Applications Team
 * @brief	DMA Driver
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
#include "drv_dma.h"

/* Private Defines */
#define DMA_DATA_ALIGN (1)
#define DMA_INC_SIZE   (4)
#define DMA_BURST_SIZE (DMA_DATA_ALIGN * DMA_INC_SIZE)

/* Structure for DMA driver */
struct xDrvDma {
	SemaphoreHandle_t xSemphIsrHdl;
	SemaphoreHandle_t xMutexLockHdl;
	DMA_HandleTypeDef *pxHdlDma;
};

/* Instance of DMA driver */
typedef struct xDrvDma StaticDrvDma_t;

/* Buffer definition */
static const uint8_t pxZeroBuffer[DMA_BURST_SIZE] __ALIGNED(DMA_BURST_SIZE) = {0};

/**
 * @brief Function to Allocate and initialize a DrvDma driver.
 */
BaseType_t xDrvDmaCreate(
		DMA_HandleTypeDef *pxHdlDma,
		void* (*allocator) (size_t size), DrvDmaHandle_t *pxDrvDmaHandle)
{
	assert(pxHdlDma && allocator && pxDrvDmaHandle);

	/* Allocate the driver and a semaphore. */
	StaticDrvDma_t *pxDrvDma = (StaticDrvDma_t *) allocator(sizeof(StaticDrvDma_t));
	StaticSemaphore_t *xStaticSemphIsrHdl  = allocator(sizeof(StaticSemaphore_t));
	StaticSemaphore_t *xStaticSemphLockHdl = allocator(sizeof(StaticSemaphore_t));
	assert(pxDrvDma && xStaticSemphIsrHdl && xStaticSemphLockHdl);

	/* Initialize the semaphore with ISR and take it. */
	pxDrvDma->xSemphIsrHdl = xSemaphoreCreateBinaryStatic( xStaticSemphIsrHdl );
	assert(pxDrvDma->xSemphIsrHdl);
	/* The semaphore shall be given before the first take. */

	/* Initialize the lock semaphore. */
	pxDrvDma->xMutexLockHdl = xSemaphoreCreateMutexStatic( xStaticSemphLockHdl );
	assert(pxDrvDma->xSemphIsrHdl);
	pxDrvDma->pxHdlDma = pxHdlDma;

	*pxDrvDmaHandle = (DrvDmaHandle_t) pxDrvDma;
	return pdTRUE;
}

/**
 * @brief Function to reset the buffer to zero using DMA.
 * @param xDrvDmaHandle DMA Driver Handle
 * @param pucData The destination memory Buffer address
 * @param ulNbBytes The length of data to be transferred from source to destination
 */
BaseType_t ucDrvDmaMemsetTo0(DrvDmaHandle_t xDrvDmaHandle, void *pucData, size_t ulNbBytes)
{
	BaseType_t status;

	assert(xDrvDmaHandle && pucData);
	struct xDrvDma *pxDrvDma = (struct xDrvDma *) xDrvDmaHandle;

	if ((uint32_t)pucData % DMA_BURST_SIZE)
		return pdFALSE;

	/* Acquire the lock. */
	status = xSemaphoreTake( pxDrvDma->xMutexLockHdl, portMAX_DELAY );
	if (status != pdTRUE)
		return status;

	status = HAL_DMA_Start_IT(pxDrvDma->pxHdlDma, (uint32_t) pxZeroBuffer, (uint32_t) pucData, ulNbBytes / DMA_BURST_SIZE);
	if (status != pdTRUE)
		goto label_abort;

	status = xSemaphoreTake( pxDrvDma->xSemphIsrHdl, portMAX_DELAY  );

	label_abort:
	/* Release the lock. */
	xSemaphoreGive( pxDrvDma->xMutexLockHdl );
	return status;
}

/**
 * @brief Call back function for DMA transfer complete.
 * @param xDrvDmaHandle DMA Driver Handle
 */
BaseType_t ucDrvDmaCallback(DrvDmaHandle_t xDrvDmaHandle)
{
	struct xDrvDma *pxDrvDma = (struct xDrvDma *) xDrvDmaHandle;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR( pxDrvDma->xSemphIsrHdl, &xHigherPriorityTaskWoken);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	return xHigherPriorityTaskWoken;
}
