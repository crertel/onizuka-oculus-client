/*
	SST_Thread.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-concurrency thread functions

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_THREAD_H
#define _SST_THREAD_H

#include <pstdint.h>

/* Opaque type representing a thread */
typedef void* SST_Thread;

/* Typedef for a function pointer that is executed by a thread [(void*) -> int] */
typedef int(*SST_ThreadFunc)(void*);

#ifdef __cplusplus
extern "C" {
#endif

	/*
	SST_Concurrency_CreateThread

	Creates a thread instance that will begin running the provided function.
	The thread context should be cleaned up with DestroyThread() after the
	thread exits (use WaitThread() to ensure it is exited).

	@param func - function that the thread will execute
	@param arg - The argument that is passed to the function.
	@return (SST_Thread) - handle to the thread
	*/
	SST_Thread SST_Concurrency_CreateThread(SST_ThreadFunc func, void *arg);

	/*
	SST_Concurrency_DestroyThread

	Frees memory associated with a thread context. This should only be called
	after SST_Concurrency_WaitThread() is called. This does NOT act like a thread
	kill function. After this is called, the thread ID from this thread may be
	reused, see SST_Concurrency_GetThreadId(). Failure to call this thread after
	creating and exiting a large number of threads can result in exhaustion of OS
	resources.

	@param thr - the thread handle
	@return (void)
	*/
	void SST_Concurrency_DestroyThread(SST_Thread thr);

	/*
	SST_Concurrency_SleepThread

	Causes the calling thread to sleep for a minimum of the specified time (in ms). Due to
	OS scheduling, the actual amount of time slept can be much larger.

	@param ms - the specified time to sleep (in ms)
	@return (void)
	*/
	void SST_Concurrency_SleepThread(uint32_t ms);

	/*
	SST_Concurrency_GetThreadId

	Gets the thread ID of the calling thread. This is guaranteed to be unique among all
	threads; however, if the thread is destroyed, then the ID may be recycled.

	@return (uint32_t) -  the thread's ID value
	*/
	uint32_t SST_Concurrency_GetThreadId(void);
		
	/*
	SST_Concurrency_GetTicks

	Gets the tick count since some arbitrary point in the past.

	The only guarantee is that the value monotonically increases, though it
	may wrap back to zero before the full 64 bits are used.

	@return (uint64_t) - the count (in ms) since the aforementioned arbitrary point
	*/
	uint64_t SST_Concurrency_GetTicks(void);
		
	/*
	SST_Concurrency_WaitThread

	Waits for a thread to terminate.  This is the only way to ensure thread resources are
	returned. This restriction may be lifted in the future.

	@param _thr - the thread handle of the thread to wait on
	@param _retval - pointer to integer to take the thread's return value, which may be NULL if the
					 return value is not desired.
	@return (int) - non-zero if successful, zero if an error occurred
	*/
	int SST_Concurrency_WaitThread(SST_Thread thr, int* retval);

	/*
	SST_Concurrency_YieldThread

	Causes the calling thread to yield execution to another thread that is
	ready to run. If no threads are ready to run, this function returns immediately
	(and therefore, is not suitable for entering a power-optimized state, e.g. laptops
	will still burn significant battery power if this is called in a loop).

	@return (void)
	*/
	void SST_Concurrency_YieldThread(void);

#ifdef __cplusplus
}
#endif

#endif

