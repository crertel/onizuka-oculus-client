/*
	SST_Mutex.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-concurrency mutex functions.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_MUTEX_H
#define _SST_MUTEX_H

/* Opaque type representing a mutex */
typedef void* SST_Mutex;

#ifdef __cplusplus
extern "C" {
#endif

	/*
	SST_Concurrency_CreateMutex

	Creates a mutex, unlocked initially.

	@return (SST_Mutex) - A mutex handle
	*/
	SST_Mutex SST_Concurrency_CreateMutex(void);

	/*
	SST_Concurrency_DestroyMutex

	Destroys a mutex. The results are undefined if the mutex is already in use

	@param mutex - the mutex handle
	@return (void)
	*/
	void SST_Concurrency_DestroyMutex(SST_Mutex mutex);

	/*
	SST_Concurrency_LockMutex

	Locks a mutex. Recursive locking is not allowed and results in undefined behavior.

	@param mutex - the mutex handle
	@return (void)
	*/
	void SST_Concurrency_LockMutex(SST_Mutex mutex);

	/*
	SST_Concurrency_TryLockMutex

	Attempt to lock a mutex. Recursive locking is not allowed and results in undefined behavior.

	@param mutex - the mutex handle
	@return (int) - non-zero if the mutex was successfully locked, zero if another thread already owns it
	*/
	int SST_Concurrency_TryLockMutex(SST_Mutex mutex);

	/*
	SST_Concurrency_UnlockMutex

	Unlocks a mutex previously locked mutex. The results are undefined if the mutex was not
	owned by this thread already.

	@param mutex - the mutex handle
	@return (void)
	*/
	void SST_Concurrency_UnlockMutex(SST_Mutex mutex);

#ifdef __cplusplus
}
#endif

#endif

