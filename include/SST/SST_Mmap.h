/*
	SST_Mmap.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-os memory mapped file I/O module

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_MMAP_H
#define _SST_MMAP_H

#include <SST/SST_File.h>
#include <SST/SST_SysMem.h>

/* Memory map */
typedef void* SST_MemoryMap;

#ifdef __cplusplus
extern "C" {
#endif

	/*
		SST_OS_GetMmapGranularity

		Gets the granularity of the 'offset' parameter for SST_OS_MmapCreate(). For example,
		if this value was 0x1000, then 'offset' must be a multiple 0x1000.

		@return The granularity (in bytes) that the 'offset' parameter must be a multiple of
				when calling SST_OS_MmapCreate().
	*/
	uint32_t SST_OS_GetMmapGranularity(void);

	/*
		SST_OS_CreateMmap

		Maps a region of a file into the virtual address space of the process. The file
		must have been opened with SST_OS_OpenFile() and with either SST_OPEN_READ or
		SST_OPEN_WRITE (i.e. SST_OPEN_APPEND not allowed) and not in asynchronous mode.
		Overlapping maps of a file results in undefined results.

		@param file - The file handle
		@param offset - The offset, must be a multiple of SST_OS_MmapGetGranularity()
		@param mapLength - The length of a the memory map, in bytes. When offset and mapLength are 0, this is interpreted as "the whole file"
		@param mode - The method of memory mapping. Must be a combination of SST_PROTECT_XXXX flags (see SST_SysMem.h)
		@return - Handle to a memory map object
	*/
	SST_MemoryMap SST_OS_CreateMmap(SST_File file, uint64_t offset, size_t mapLength, int mode);

	/*
		SST_OS_DestroyMmap

		Destroys a memory map and frees all resources associated with it. This also
		synchronizes the memory with the file.
	*/
	void SST_OS_DestroyMmap(SST_MemoryMap mappedRegion);

	/*
		SST_OS_GetMmapBase

		Gets the base address of the memory map.

		@param mappedRegion - The memory map handle
		@return - The base address of the memory mapped region.
	*/
	void* SST_OS_GetMmapBase(SST_MemoryMap mappedRegion);

	/*
		SST_OS_GetMmapSize

		Gets the size of the memory mapped region, in bytes.

		@param mappedRegion - The memory map handle
		@return - The size of the memory mapped region, in bytes.
	*/
	size_t SST_OS_GetMmapSize(SST_MemoryMap mappedRegion);

	/*
		SST_OS_GetMmapOffset

		Gets the offset from the beginning of the file to where
		the memory map begins.

		@param mappedRegion - The memory map handle
		@return - The offset from the beginning of the file to the beginning of the memory map, in bytes.
	*/
	uint64_t SST_OS_GetMmapOffset(SST_MemoryMap mappedRegion);

	/*
		SST_OS_SyncMmap

		Synchronizes the memory map with the file. This initiates the
		process of writing the pages, but does not wait for its completion.

		@param mappedRegion - The memory map handle
	*/
	void SST_OS_SyncMmap(SST_MemoryMap mappedRegion);

#ifdef __cplusplus
}
#endif

#endif


