/*
	SST_DynLib.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-os dynamic linking functions module

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_DYNLIB_H
#define _SST_DYNLIB_H

typedef void* SST_DynLib;
typedef void (*SST_DynLibFunc)(void);

#ifdef __cplusplus
extern "C" {
#endif

/*
	SST_OS_LoadDynLib

	Loads a dynamic link library / shared object.

	@param path - Path to the library
	@return - Handle to the library, or NULL on error (e.g: library not found, dependent library not found,
	          wrong CPU architecture, etc.)
*/
SST_DynLib SST_OS_LoadDynLib(const char* path);

/*
	SST_OS_GetLibSymbol

	Gets a pointer to a function with the given name.

	@param lib - The dynamic link library / shared object
	@param symbol - The ASCII name for the symbol
	@return - Pointer to the function's symbol, or NULL if not found.
*/
SST_DynLibFunc SST_OS_GetLibSymbol(SST_DynLib lib, const char* symbol);

/*
	SST_OS_CloseDynLib

	Closes the dynamic link library / shared object

	@param lib - The dynamic link library to close.
*/
void SST_OS_CloseDynLib(SST_DynLib lib);

/*
	SST_OS_DynLibName
	
	Constructions an OS-specific library name for the given library.
	For example a library named "mylib" on Windows would become "mylib.dll"
	and on UNIX-like operating systems, it would become "libmylib.so". To
	query the number of characters required, call the function with the 'libnameOut'
	parameter set to NULL.
	
	@param libnameOut - Pointer to buffer to store the library's OS-specific name, or NULL to query.
	@param libnameIn - Name of the library, without any extensions, such as "mylib" or "foo". This should NOT
	                   be a path, e.g: "/usr/local/foo"
	@return - If 'libnameOut' is NULL, then this returns the number of characters in the modified name,
	          i.e. the size of the buffer required for libnameOut without the NULL terminating character.
			  If 'libnameOut' is non-NULL, then this returns 0.
*/
int SST_OS_DynLibName(char* libnameOut, const char* libnameIn);

#ifdef __cplusplus
}
#endif

#endif

