/*
	SST_CPUCache.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-os CPU cache manipulation

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_CPUCACHE_H
#define _SST_CPUCACHE_H

#ifdef __cplusplus
extern "C" {
#endif

	/*
		SST_OS_GetCacheLineSize()

		Gets the L1 data cache line size.

		@return The cache line size in bytes, or 0 if unknown.
	*/
	uint32_t SST_OS_GetCacheLineSize();

	/*
		SST_OS_FlushDCRange

		Signals the CPU to begin flushing (write-back) a block of the data cache
		to main memory.This does not guarantee that the line is fully written
		back to memory by the time it returns, use SST_OS_SyncCache() to enforce
		this.

		@param base - The base address of the block to write back to main memory
		@param range - The number of bytes to invalidate
	*/
	void SST_OS_FlushDCRange(void* base, size_t range);

	/*
		SST_OS_InvalidateICRange

		Invalidates a range of instructions in the instruction cache, forcing it be re-read from
		memory. Note that this is re-read from MEMORY, not from DATA CACHE. This has
		important implications: to correctly modify code and then execute it, the following
		sequence should be made:

		
		SST_OS_FlushDCRange(code_ptr, code_size); //force DC to be written back to memory
		SST_OS_SyncCache(); //ensure DC is written back to memory
		SST_OS_InvalidateICRange(code_ptr, code_size); //Invalidate IC range
		SST_OS_SyncCache(); //ensure invalidation-then-refetch is complete
		execute_new_code(code_ptr);

		@param base - The base address of the block to write back to main memory
		@param range - The number of bytes to invalidate
	*/
	void SST_OS_InvalidateICRange(void* base, size_t range);

	/*
		SST_OS_SyncCache

		Guarantees that all outstanding that all outstanding SST_OS_InvalidateICRange() and
		SST_OS_FlushDCRange() calls are completed for the CPU that calls it.

		NOTE: If you have one thread generating code and another executing it,
		this is not sufficient to protect the other thread from executing invalid
		code -- SST_OS_SyncCache() synchronizes the *caller*, not *all threads*.

	*/
	void SST_OS_SyncCache();

#ifdef __cplusplus
}
#endif

#endif

