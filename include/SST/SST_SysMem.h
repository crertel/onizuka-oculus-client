/*
	SST_SysMem.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-os system memory functions module

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_SYSMEM_H
#define _SST_SYSMEM_H

#include <pstdint.h>
#include <stdlib.h>

/* Flags for use by SST_OS_ProtectPages() */
#define SST_PROTECT_READ	0x01 /**< The page may be read from */
#define SST_PROTECT_WRITE	0x02 /**< The page may be written to */
#define SST_PROTECT_EXEC	0x04 /**< The page may be executed by the CPU */

#ifdef __cplusplus
extern "C" {
#endif

	/*
	SST_OS_GetMemorySize

	Gets the size of the physical memory installed in the computer, in bytes.

	@return - The size of the free memory, in bytes.
	*/
	uint64_t SST_OS_GetMemorySize();

	/*
	SST_OS_GetPageSize

	Gets the size of a system page, in bytes.

	@return - The size of a page of memory, in bytes.
	*/
	uint32_t SST_OS_GetPageSize();

	/*
	SST_OS_AllocPages

	Directly allocates a contiguous region of pages. If the parameter
	'allocBytes' is not a multiple of the page size, the function rounds
	up the allocation request to the nearest multiple of the page size.
	Due to the high cost of calling this function, this should really only be
	used by custom memory management code.

	Naturally, the pointer returned (if successful) is aligned to the page
	boundary.

	By default, the pages are allocated with read/write permissions. To change
	the access, use SST_OS_ProtectPages().

	Attempts are made to commit memory immediately, but the possibility of this
	can vary from platform to platform.

	@param allocBytes - The number of bytes to allocate.
	@return Pointer to contiguous region of pages, or NULL if unable to allocate pages.
	        Note that being unable to allocate a *contiguous* region of pages is not the
			same as a being out of memory. This is more of a problem for 32-bit processes.
	*/
	void* SST_OS_AllocPages(size_t allocBytes);

	/*
	SST_OS_FreePages

	Frees a contiguous region of pages. The address 'pageBase' must be
	aligned to the page boundary (e.g. returned from SST_OS_AllocPages()),
	and if the number of bytes isn't a multiple of the page size, it is 
	rounded up. Note that when these pages are freed, access to them undefined,
	but will likely cause a crash as the OS usually obliterates them from the
	virtual address space of the process.

	Double-freeing pages also has undefined results, but likely is to cause trouble.

	@param pageBase - The base address of the pages to begin freeing. This must be
	                  aligned to the page boundary.
	@param nrBytes - The number of bytes in the region to free. This is rounded
	                 to the nearest page if it is not a multple of the page size.
	@return >0 on success, <= on failure.
	*/
	int SST_OS_FreePages(void* pageBase, size_t nrBytes);

	/*
	SST_OS_ProtectPages

	Changes the memory protections for a range of pages. The address 'pageBase'
	must be aligned to the page boundary (e.g. returned from SST_OS_AllocPages()),
	and if the number of bytes isn't a multiple of the page size, it is 
	rounded up.

	Some CPU architectures don't have all of the protection flags (e.g. x86 originally
	did not have a per-page execute bit), but to be portable, never assume that one
	protection bit implies the other.

	As a best practice, software that generates or modifies its own code should
	remove the 'write' flag when it is not needed.

	@param pageBase - The base address of the pages to change protections on.
	@param nrBytes - The number of bytes in the region to change. This is rounded
	                 to the nearest page if it is not a multple of the page size.
	@param protectFlags - A bitwise OR of the SST_PROTECT_XXXXXX flags, or 0 if no access
	                      should be allowed (e.g. a so called "redzone")
	@return Non-zero on success, zero on failure.

	*/
	int SST_OS_ProtectPages(void* pageBase, size_t nrBytes, int protectFlags);

#ifdef __cplusplus
}
#endif

#endif


