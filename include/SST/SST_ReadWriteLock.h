/*
	SST_ReadWriteLock.h
	Author: James Russell <jcrussell@762studios.com>
	Created: 5/16/2012

	Purpose: 

	libsst-concurrency read write lock functions

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_READWRITELOCK_H
#define _SST_READWRITELOCK_H

#include <pstdint.h>

#include "SST_Atomic.h"

/* Read Write Lock Opaque Type */
typedef void* SST_ReadWriteLock;

#ifdef __cplusplus
extern "C" {
#endif

	/*
	public SST_Concurrency_CreateReadWriteLock
	
	Creates an allocated SST_ReadWriteLock instance.
	
	@return (SST_ReadWriteLock) - a new read write lock instance
	*/
	SST_ReadWriteLock SST_Concurrency_CreateReadWriteLock(void);

	/*
	public SST_Concurrency_DestroyReadWriteLock
	
	Destroys an SST_ReadWriteLock allocated via SST_Concurrency_CreateReadWriteLock.  This will
	signal the event before destruction in order to ensure any waiting readers / writers are released.
	
	@param lock - the lock to destroy
	@return (void)
	*/
	void SST_Concurrency_DestroyReadWriteLock(SST_ReadWriteLock lock);

	/*
	public SST_Concurrency_LockForReading
	
	Locks an SST_ReadWriteLock for reading.  Will block until writing is completed.  
	
	@param lock - the lock to lock for reading
	@param ticks - the maximum amount of time to block
	@return (int) - zero if locking failed, non-zero if locking succeeded
	*/
	int SST_Concurrency_LockForReading(SST_ReadWriteLock lock, uint32_t ticks);

	/*
	public SST_Concurrency_LockForWriting
	
	Locks an SST_ReadWriteLock for writing.  Will block until readers have completed.
	Will fail (return 0) if already locked for writing.
	
	@param lock - the lock to lock for writing
	@param ticks - the maximum amount of time to block
	@return (int) - zero if locking failed, non-zero if locking succeeded
	*/
	int SST_Concurrency_LockForWriting(SST_ReadWriteLock lock, uint32_t ticks);

	/*
	public SST_Concurrency_EndReading
	
	Signals that a read operation has completed on an SST_ReadWriteLock.
	
	@param lock - the lock to signal
	@return (void)
	*/
	void SST_Concurrency_EndReading(SST_ReadWriteLock lock);

	/*
	public SST_Concurrency_EndWriting
	
	Signals that a write operation has completed on an SST_ReadWriteLock.
	
	@param lock - the lock to signal
	@return (void)
	*/
	void SST_Concurrency_EndWriting(SST_ReadWriteLock lock);

#ifdef __cplusplus
}
#endif

#endif
