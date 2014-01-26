/*
	SST_OS.h
	Author: Patrick Baggett

	Purpose: Includes all relevant libsst-os header files.

	Changelog
	2011/12/04 - creation (ptbaggett)
*/

#ifndef _SST_OS_H
#define _SST_OS_H

/* Version number constants for libsst-os */
#define SST_OS_VERSION_MAJOR	0x01
#define SST_OS_VERSION_MINOR	0x01
#define SST_OS_VERSION_PATCH	0x0000
#define SST_OS_VERSION	(SST_OS_VERSION_MAJOR << 24) | (SST_OS_VERSION_MINOR << 16) | (SST_OS_VERSION_PATCH)

#define SST_OS_VERSION_STRING	"1.1.0"

#include "SST_Alloc.h"
#include "SST_Assert.h"
#include "SST_CPU.h"
#include "SST_CPUCache.h"
#include "SST_DynLib.h"
#include "SST_File.h"
#include "SST_FileSys.h"
#include "SST_Mmap.h"
#include "SST_SysMem.h"
#include "SST_User.h"
#include "SST_Endian.h"
#include "SST_Time.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*
		SST_OS_Init()

		Initializes the OS library

		@return (int) - Non-zero if successful, zero on failure.
	*/
	int SST_OS_Init();

#ifdef __cplusplus
}
#endif

#endif


