/*
	SST_Alloc.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 1/7/2012

	Purpose: 

	Provides portable implementations for various CRT-like memory functions

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_ALLOC_H
#define _SST_ALLOC_H

#include <stdlib.h>

#include <SST/SST_Build.h>

#if SST_COMPILER == SST_COMPILER_MSVC

	#include <malloc.h>
	#define SST_OS_Alloca(x) _alloca(x)
	
#elif (SST_COMPILER == SST_COMPILER_GCC) || (SST_COMPILER == SST_COMPILER_CLANG)

	//GCC/Clang provide built-in alloca() functions.
	#define SST_OS_Alloca(x) __builtin_alloca(x)	

#elif (SST_COMPILER == SST_COMPILER_SUNPRO)

	//Just need alloca.h
	#include <alloca.h>
	#define SST_OS_Alloca(x) alloca(x)
	
#else //Sane default
	#define SST_OS_Alloca(x) alloca(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	
	/*
		SST_OS_AlignedMalloc

		Like malloc(), but returns pointers aligned to the user-specified amount. Aligning
		the address wastes a bit of memory. This must be freed using SST_OS_AlignedFree().

		@param bytes - The number of bytes to allocate.
		@param boundary - The alignment boundary to allocate on. Must be a power-of-two and at least sizeof(int) and no more than 128.
		@return (void*) - The memory, or NULL on error.
	
	*/
	void* SST_OS_AlignedMalloc(size_t bytes, size_t boundary);

	/*
		SST_OS_AlignedFree

		Frees a block of memory allocated with SST_OS_AlignedMalloc()

		@param ptr - The block of memory to free.
		@return (void)
	*/
	void SST_OS_AlignedFree(void* ptr);
	
	/*
		SST_OS_AlignPtr

		Takes the provided pointer and returns it's value aligned to the byte boundary
		provided (sizeof(void*), for example, would get you word aligned).

		@param ptr - the pointer to align
		@param boundary - the byte boundary to align to
		@return (void*) - the aligned value of the pointer
	*/
	void* SST_OS_AlignPtr(void* ptr, size_t boundary);

	/*
		SST_OS_SafeAlignedMalloc

		Like SST_OS_AlignedMalloc(), except that integer overflows of the
		product of 'nelem' and 'elsize' trigger SST_OS_RuntimeAssert() failures.

		@param nelem - The number of elements to allocate
		@param elsize - The size of each element
		@param boundary - The boundary, exactly the same meaning as in SST_OS_AlignedMalloc()
		@return (void*) - The block of memory
	*/
	void* SST_OS_SafeAlignedMalloc(size_t nelem, size_t elsize, size_t boundary);

	/*
		SST_OS_SafeRealloc
	
		Like realloc(), except that integer overflows of the product of 'nelem'
		and 'elsize' trigger SST_OS_RuntimeAssert() failures. Since this uses
		realloc() internally, the memory should be freed using free(). The
		memory for the 'ptr' parameter must have come from malloc(), realloc(),
		or SST_OS_SafeRealloc().

		@param ptr - The original pointer. Like realloc(), it accepts NULL.
		@param nelem - The number of elements to allocate
		@param elsize - The size of each element
		@return (void*) - The resized block of memory.
	*/
	void* SST_OS_SafeRealloc(void* ptr, size_t nelem, size_t elsize);

	/*
		SST_OS_SafeMalloc
	
		Like malloc(), except that integer overflows of the product of 'nelem'
		and 'elsize' trigger SST_OS_RuntimeAssert() failures. Since this uses
		malloc() internally, the memory should be freed using free(). 

		@param nelem - The number of elements to allocate
		@param elsize - The size of each element		
	*/
	#define SST_OS_SafeMalloc(nelem, elsize)	SST_OS_SafeRealloc(NULL, nelem, elsize)

#ifdef __cplusplus
}
#endif


#endif
