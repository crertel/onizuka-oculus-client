/*
	SST_File.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-os file I/O module

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_FILE_H
#define _SST_FILE_H

#include <pstdint.h>

/* Open types for SST_OS_FileOpen() */
#define SST_OPEN_READ		0x00000001
#define SST_OPEN_WRITE		0x00000002
#define SST_OPEN_APPEND		0x00000003

/* Combined with one of the above flags */
#define SST_OPEN_ASYNC		0x80000000	/* Open for asynchronous I/O (not yet supported) */
#define SST_OPEN_HINTRAND	0x40000000	/* Hint: accesses will be random. Mutually exclusive with SST_OPEN_HINTSEQ */
#define SST_OPEN_HINTSEQ	0x20000000	/* Hint: operations will be sequential-forward. Mutually exclusive with SST_OPEN_HINTRAND */


#define SST_SEEK_START		0x00	/* Used with SST_OS_SeekFile() to represent an offset from the start of the file */
#define SST_SEEK_CUR		0x01	/* Used with SST_OS_SeekFile() to represent an offset from the current position in the file */
#define SST_SEEK_END		0x02	/* Used with SST_OS_SeekFile() to represent an offset from the end of the file */

/* Opaque type for file handle */
typedef void* SST_File;

#ifdef __cplusplus
extern "C" {
#endif

	/*
		SST_OS_OpenFile

		Opens a file for reading/writing/appending.

		@param path - The path to the file, relative or absolute.
		@param mode - Bitwise OR of one of SST_OPEN_READ/WRITE/APPEND and zero or more of:
					  SST_OPEN_ASYNC, SST_OPEN_HINTRAND, SST_OPEN_HINTSEQ.
		@return - The file handle, or NULL on failure.
	*/
	SST_File SST_OS_OpenFile(const char* path, uint32_t mode);

	/*
		SST_OS_WriteFile

		Writes a block of data to the file. If the file was opened with SST_OPEN_ASYNC,
		then the asynchronous I/O functions must be used.

		@param file - The file handle
		@param buf - Pointer to block of data
		@param bytes - Number of bytes to write
		@return - The number of bytes written (> 0), or 0 on error. This can be less than 'bytes'
	*/
	uint64_t SST_OS_WriteFile(SST_File file, const void* buf, uint64_t bytes);

	/*
		SST_OS_ReadFile

		Reads a block of data from the file. If the file was opened with SST_OPEN_ASYNC,
		then the asynchronous I/O functions must be used.

		@param file - The file handle
		@param buf - Pointer to buffer to receive data read
		@param bytes - Number of bytes to read
		@return - The number of bytes read (> 0), or 0 on error. This can be less than 'bytes'
	*/
	uint64_t SST_OS_ReadFile(SST_File file, void* buf, uint64_t bytes);

	/*
		SST_OS_GetFilePointer

		Gets the file's current read/write pointer.

		@param file - The file handle
		@param fpReturn - Pointer to value to receive the file pointer
		@return > 0: success, <= 0: failure
	*/
	int SST_OS_GetFilePointer(SST_File file, uint64_t* fpReturn);

	/*
		SST_OS_SetFilePointer

		Sets the file's current read/write pointer.

		@param file - The file handle
		@param ptr - The file pointer
		@return > 0: success, <= 0: failure, e.g. out of range.
	*/
	int SST_OS_SetFilePointer(SST_File file, uint64_t ptr);

	/*
		SST_OS_FileEOF

		Returns whether the file is at the EOF or not.

		@param file - The file handle
		@return >0: file is at EOF, 0: not at EOF, <0: error.
	*/
	int SST_OS_FileEOF(SST_File file);

	/*
		SST_OS_FlushFile

		Tells the underlying OS to flush buffered I/O. No guarantee about
		atomicity is made, as this is just a hint to the OS.

		@param file - The file handle
	*/
	void SST_OS_FlushFile(SST_File file);

	/*
		SST_OS_CloseFile

		Closes the specified file handle. Note that any memory mapped regions must
		be unmapped first or the results are undefined.

		@param file - The file handle
	*/
	void SST_OS_CloseFile(SST_File file);

	/*
		SST_OS_SeekFile

		Adjusts the file read/write pointer by doing a seek relative
		to the beginning, end, or current pointer.

		@param file - The file handle
		@param offset - The signed offset from
		@param fromWhere - One of SST_SEEK_START, SST_SEEK_CUR, SST_SEEK_END
		@return - Non-zero: success, 0: failure
	*/
	int SST_OS_SeekFile(SST_File file, int64_t offset, int fromWhere);

	/*
		SST_OS_GetFileSize

		Gets the size of the file in bytes. If there is an error UINT64_MAX is returned.

		@param file - The file handle
		@return (uint64_t) - File size
	*/
	uint64_t SST_OS_GetFileSize(SST_File file);

#ifdef __cplusplus
}
#endif

#endif


