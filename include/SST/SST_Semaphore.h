/*
	SST_Semaphore.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-concurrency semaphore functions

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_SEMAPHORE_H
#define _SST_SEMAPHORE_H

#include <pstdint.h>

typedef void* SST_Semaphore;	/* Semaphore */

#ifdef __cplusplus
extern "C" {
#endif

	/*
	SST_Concurrency_CreateSemaphore

	Creates a semaphore with the given initial count.

	@param initialCount - the initial count value for the semaphore
	@return (SST_Semaphore) - a semaphore handle
	*/
	SST_Semaphore SST_Concurrency_CreateSemaphore(uint32_t initialCount);

	/*
	SST_Concurrency_DestroySemaphore

	Destroys a semaphore. Undefined results if threads are waiting on the semaphore.

	@param sema - the semaphore to destroy
	@return (void)
	*/
	void SST_Concurrency_DestroySemaphore(SST_Semaphore sema);

	/*
	SST_Concurrency_PostSemaphore

	Increases a semaphore's count, waking up threads that might be waiting.

	@param sema - the semaphore to wait on.
	@param count - the count to increase the semaphore by
	@return (void)
	*/
	void SST_Concurrency_PostSemaphore(SST_Semaphore sema, uint32_t count);

	/*
	SST_Concurrency_WaitSemaphore

	Wait for a semaphore's count be non-zero, then atomically decrease it by 1.

	@param sema - the semaphore to wait on.
	@param ticks - the time to wait in milliseconds. 0 = don't wait (test only), SST_WAIT_INFINITE = block indefinitely
	@return (int) - non-zero if the semaphore was signaled when it returns, zero if the semaphore wasn't signaled before
	*/
	int SST_Concurrency_WaitSemaphore(SST_Semaphore sema, uint32_t ticks);

#ifdef __cplusplus
}
#endif

#endif

