/*
        SST_FileSys.h
        Author: Patrick Baggett <ptbaggett@762studios.com>
        Created: 3/29/2012

        Purpose: 

        libsst-os file system querying

        License:

        This program is free software. It comes without any warranty, to
        the extent permitted by applicable law. You can redistribute it
        and/or modify it under the terms of the Do What The Fuck You Want
        To Public License, Version 2, as published by Sam Hocevar. See
        http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_FILESYS_H
#define _SST_FILESYS_H

#include <stdlib.h>
#include <pstdint.h>

#define SST_FILENAME_MAX	256

typedef struct SST_FileInfo
{
	char name[SST_FILENAME_MAX];
	uint64_t size;
	size_t nameLen;
	int isDir;
} SST_FileInfo;


typedef void* SST_Dir;

#ifdef __cplusplus
extern "C" {
#endif

	/*
		SST_OS_OpenDirectory()
		
		Opens a directory for listing the contents.

		@param path - The path to the directory. Must not contain a trailing slash
		@return (SST_Dir) - NULL on error, or handle to the directory on success. To close this handle, use SST_OS_CloseDirectory().
	*/
	SST_Dir SST_OS_OpenDirectory(const char* path);

	/*
		SST_OS_ReadNextDirectoryEntry()

		Attempts to read the next directory entry.

		@param dir - The directory handle
		@param fileInfoReturn - Pointer to structure to receive information about the next file
		@return (int) - Non-zero if a file entry was read, zero if at the end of the directory.
	*/
	int SST_OS_ReadNextDirectoryEntry(SST_Dir dir, SST_FileInfo* fileInfoReturn);

	/*
		SST_OS_CloseDirectory()

		Closes the directory handle opened with SST_OS_OpenDirectory()

		@param dir - The directory handle
	*/
	void SST_OS_CloseDirectory(SST_Dir dir);

	/*
		SST_OS_CreateDirectory()

		Creates a directory with the given path

		@param dir - The directory to create
		@return (int) - Negative on error, zero on success, positive if the directory already exists
	*/
	int SST_OS_CreateDirectory(const char* dir);

	/*
		SST_OS_RemoveDirectory()

		Removes an empty directory with the given path

		@param dir - The directory to remove
		@return (int) - Non-zero on success, zero on failure
	*/
	int SST_OS_RemoveDirectory(const char* dir);

#ifdef __cplusplus
}
#endif

#endif

