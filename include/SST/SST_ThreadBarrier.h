/*
	SST_Barrier.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-concurrency thread barrier functions

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_BARRIER_H
#define _SST_BARRIER_H

#include "SST_Event.h"

/*
	SYNOPSIS
	
	Barrier are a simple way to ensure that a number of threads synchronize at a certain point. When created, a
	barrier has a 'threshold' value, and each thread that tries to "enter" the barrier increments a counter. If the
	counter is less than the threshold, then the function call blocks the thread. When that counter meets the threshold
	value, all threads are allowed to proceed.
	
	A good example of this would be if 'N' threads had 'N' variable sized jobs to complete. In order to ensure that the
	main thread blocked until all 'N' threads had completed their 'N' work assignments, a barrier with 'N+1' count could
	be created. The main thread attempts the enter the barrier, and is blocked. Then, as the 'N' threads complete their 
	work, they each enter the barrier as well. When the last thread enters the barrier, all of the threads become unblocked
	because the barrier count has reached the threshold value of N+1.
	
	Barriers can safely be canceled (i.e. wake up all threads without the condition being met) as well if for example a
	long running job needs to be canceled. Barriers may be used multiple times, but after each use, they should be reset after
	their threshold has been exceed, or else threads simply return immediately.
*/

typedef void* SST_ThreadBarrier;

#define SST_BARRIER_SINGLE	0x7000	/* Returned by a single thread from SST_WaitThreadBarrier() */

#ifdef __cplusplus
extern "C" {
#endif

/*
	SST_Concurrency_CreateThreadBarrier
	
	Creates a thread barrier object and initializes it with the given count.
	
	@param count - The threshold value for the barrier.
	@return The handle, or NULL on failure (such as out of memory)
*/
SST_ThreadBarrier SST_Concurrency_CreateThreadBarrier(uint32_t threshold);

/*
	SST_Concurrency_WaitThreadBarrier
	
	Increments the barrier count and waits at the current barrier until the
	threshold value is reached or a call to SST_Concurrency_CancelThreadBarrier() is made.

	Once the threshold value is released, one the threads will return SST_BARRIER_SINGLE and
	rest will return some other positive value, guaranteed not to be SST_BARRIER_SINGLE. The
	SST_BARRIER_SINGLE is also positive, so checks like "retval > 0" are still legal without
	any special-case logic.

	If a barrier has already been released or canceled, then this returns immediately,
	and the return value is as if it succeeded, however, SST_BARRIER_SINGLE is never
	returned (since it would have been the first time).
	
	@param barrier - The barrier to wait on
	@return < 0: error or corrupt state
			0: barrier canceled
			> 0 success -- One thread will return SST_BARRIER_SINGLE.
*/
int SST_Concurrency_WaitThreadBarrier(SST_ThreadBarrier barrier);

/*
	SST_Concurrency_ResetThreadBarrier
	
	Resets the barrier for usage. This is required after SST_Concurrency_WaitThreadBarrier()
	returns (either due to the threshold being exceeded or being canceled). The results
	are undefined if calling this while threads are still waiting.
	
	@param barrier - The barrier to reset
	@param newThreshold - The new threshold value
*/
void SST_Concurrency_ResetThreadBarrier(SST_ThreadBarrier barrier, uint32_t newThreshold);

/*
	SST_Concurrency_CancelThreadBarrier
	
	Wakes up all threads that are waiting on the given barrier. This causes the
	threads who called SST_Concurrency_WaitThreadBarrier() to return zero. Any
	threads who call SST_Concurrency_WaitThreadBarrier() after a cancel but before a reset
	will not be blocked and will return that the barrier was canceled (i.e. zero).
	Canceling an already canceled barrier has no effect.
	
	@param barrier - The barrier to cancel.
	
*/
void SST_Concurrency_CancelThreadBarrier(SST_ThreadBarrier barrier);

/*
	SST_Concurrency_DestroyThreadBarrier
	
	Frees resources associated with the barrier. Undefined results occur if the 
	barrier is currently blocking threads, though due to race conditions, a crash
	is the most likely occurance.
	
	@param barrier - The barrier to destroy
*/
void SST_Concurrency_DestroyThreadBarrier(SST_ThreadBarrier barrier);

#ifdef __cplusplus
}
#endif


#endif

