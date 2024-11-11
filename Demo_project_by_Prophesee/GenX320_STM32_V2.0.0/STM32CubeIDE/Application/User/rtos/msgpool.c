/**
 ******************************************************************************
 * @file    msgpool.c
 * @author  PSEE Applications Team
 * @brief	Message Pool Driver
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
#include "msgpool.h"

/* Structure for MsgPool driver */
struct xMsgPool {
	StaticQueue_t xEmptyMsgs;
	StaticQueue_t xFilledMsgs;
};

/* Instance of MsgPool driver */
typedef struct xMsgPool StaticMsgPool_t;
typedef struct xMsgPool MsgPool_t;

/**
 * @brief Function to return the Handle of the Empty Queue.
 * @param xPoolHandle Message Pool Handle
 */
static inline QueueHandle_t xEmptyQueueHandle(const MsgPoolHandle_t xPoolHandle)
{
	MsgPool_t *pxMsgPool = (MsgPool_t *) xPoolHandle;
	return (QueueHandle_t) &(pxMsgPool->xEmptyMsgs);
}

/**
 * @brief Function to return the Handle of the Filled Queue.
 * @param xPoolHandle Message Pool Handle
 */
static inline QueueHandle_t xFilledQueueHandle(const MsgPoolHandle_t xPoolHandle)
{
	MsgPool_t *pxMsgPool = (MsgPool_t *) xPoolHandle;
	return (QueueHandle_t) &(pxMsgPool->xFilledMsgs);
}

/**
 * @brief Function to get a new message to fill it before posting. Invoked by the producer.
 * @param xPoolHandle Message Pool Handle
 * @param xTimeout Ticks to wait
 */
MsgHandle_t xMsgPoolAllocate(const MsgPoolHandle_t xPoolHandle, TickType_t xTimeout)
{
	MsgPool_t *pxMsgPool = (MsgPool_t *) xPoolHandle;
	assert(pxMsgPool);


	MsgHandle_t xMsgHandle = NULL;
	xQueueReceive( xEmptyQueueHandle(pxMsgPool), &xMsgHandle, xTimeout );
	return xMsgHandle;
}

/**
 * @brief Function to get a new message to fill it before posting from ISR. Invoked by the producer.
 * @param xPoolHandle Message Pool Handle
 */
MsgHandle_t xMsgPoolAllocateFromISR(const MsgPoolHandle_t xPoolHandle, BaseType_t *pxHigherPriorityTaskWoken)
{
	MsgPool_t *pxMsgPool = (MsgPool_t *) xPoolHandle;
	assert(pxMsgPool);

	MsgHandle_t xMsgHandle = NULL;
	BaseType_t status = xQueueReceiveFromISR( xEmptyQueueHandle(pxMsgPool), &xMsgHandle, pxHigherPriorityTaskWoken );
	if (status != pdPASS)
		return NULL;
	return xMsgHandle;
}

/**
 * @brief Function to Receive the Filled message from a queue.
 * @param xPoolHandle Message Pool Handle.
 * @param xTimeout Ticks to wait
 */
MsgHandle_t xMsgPoolGet(const MsgPoolHandle_t xPoolHandle, TickType_t xTimeout)
{
	MsgPool_t *pxMsgPool = (MsgPool_t *) xPoolHandle;
	assert(pxMsgPool);

	MsgHandle_t xMsg = NULL;
	xQueueReceive( xFilledQueueHandle(pxMsgPool), &xMsg, xTimeout );

	return xMsg;
}

/**
 * @brief Function to Post an message to be consumed. Invoked from the producer task.
 * @param xPoolHandle Message Pool Handle.
 * @param xMsg Pointer to the item that is to be placed on the queue.
 * @Note: xFilledMsgs is supposed to be large enough to hold all messages.
 */
BaseType_t xMsgPoolPost(const MsgPoolHandle_t xPoolHandle, MsgHandle_t xMsg)
{
	MsgPool_t *pxMsgPool = (MsgPool_t *) xPoolHandle;
	assert(pxMsgPool);

	return xQueueSend( xFilledQueueHandle(pxMsgPool), &xMsg, 0);
}

/**
 * @brief Function to Post an message to be consumed from ISR. Invoked from the producer task.
 * @param xPoolHandle Message Pool Handle.
 * @param xMsg Pointer to the item that is to be placed on the queue.
 */
BaseType_t xMsgPoolPostFromISR(const MsgPoolHandle_t xPoolHandle, MsgHandle_t xMsg, BaseType_t *pxHigherPriorityTaskWoken)
{
	MsgPool_t *pxMsgPool = (MsgPool_t *) xPoolHandle;
	assert(pxMsgPool);

	return xQueueSendFromISR( xFilledQueueHandle(pxMsgPool), &xMsg, pxHigherPriorityTaskWoken);
}

/**
 * @brief Function to release the consumed message. Invoked by the consumer task.
 * @param xPoolHandle Message Pool Handle.
 * @param xMsg Pointer to the item that is to be placed on the queue.
 * @Note xEmptyMsgs is supposed to be large enough to hold all messages.
 */
BaseType_t xMsgPoolRelease( const MsgPoolHandle_t xPoolHandle, MsgHandle_t xMsg )
{
	MsgPool_t *pxMsgPool = (MsgPool_t *) xPoolHandle;
	assert(pxMsgPool);

	return xQueueSend( xEmptyQueueHandle(pxMsgPool), &xMsg, 0 );
}

/**
 * @brief Function to release the consumed message from ISR. Invoked by the consumer task.
 * @param xPoolHandle Message Pool Handle.
 * @param xMsg Pointer to the item that is to be placed on the queue.
 */
BaseType_t xMsgPoolReleaseFromISR( const MsgPoolHandle_t xPoolHandle, MsgHandle_t xMsg, BaseType_t *pxHigherPriorityTaskWoken )
{
	MsgPool_t *pxMsgPool = (MsgPool_t *) xPoolHandle;
	assert(pxMsgPool);

	return xQueueSendFromISR( xEmptyQueueHandle(pxMsgPool), &xMsg, pxHigherPriorityTaskWoken );
}

/**
 * @brief Function to create the Message Pool
 * @param pxMsgs Pointer to an array of messages previously allocated.
 * @param ulNbMsgs Number of messages.
 * @param ulMsgSize Message size.
 * @param allocator Allocator invoked to create Msg handles and Queues.
 * @param pxMsgPoolHandle Handle of the created MsgPool.
 */
BaseType_t xMsgPoolCreate(
		uint8_t *pxMsgs,
		const size_t ulNbMsgs,
		const size_t ulMsgSize,
		void* (*allocator) (size_t size),
		MsgPoolHandle_t *pxMsgPoolHandle)
{
	assert(pxMsgPoolHandle);

	/* Allocate a new private message pool instance. */
	MsgPool_t *pxMsgPool = (MsgPool_t *) allocator(sizeof(MsgPool_t));
	if(pxMsgPool == NULL)
		return pdFALSE;

	/* Allocate a new message queues holding pointers to messages. */
	MsgHandle_t *pxEmptyMsgsHdl     = (MsgHandle_t *) allocator(ulNbMsgs * sizeof(MsgHandle_t));
	MsgHandle_t *pxFilledMsgsHdl   = (MsgHandle_t *) allocator(ulNbMsgs * sizeof(MsgHandle_t));

	if (!pxMsgs || !pxEmptyMsgsHdl || !pxFilledMsgsHdl)
		return pdFALSE;

	/* Initialize message queues. */
	QueueHandle_t pxEmptyMsgsQueue = xQueueCreateStatic(
			ulNbMsgs, sizeof(MsgHandle_t),
			(uint8_t *) pxEmptyMsgsHdl, &(pxMsgPool->xEmptyMsgs));
	if (pxEmptyMsgsQueue == NULL)
		return pdFALSE;

	QueueHandle_t pxFilledMsgsQueue = xQueueCreateStatic(
			ulNbMsgs, sizeof(MsgHandle_t),
			(uint8_t *) pxFilledMsgsHdl, &(pxMsgPool->xFilledMsgs));
	if (pxFilledMsgsQueue == NULL)
		return pdFALSE;

	/* Post allocated messages to the Empty message queue. */
	for (size_t i = 0; i < ulNbMsgs; ++i) {
		MsgHandle_t pMsgHdl = (MsgHandle_t) &(pxMsgs[i * ulMsgSize]);
		xQueueSend(xEmptyQueueHandle(pxMsgPool), &pMsgHdl, 0);
	}

	/* Return a pointer to a newly allocated message pool. */
	*pxMsgPoolHandle = (MsgPoolHandle_t) pxMsgPool;
	return pdTRUE;
}
