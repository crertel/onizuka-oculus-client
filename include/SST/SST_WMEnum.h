/*
	SST_WMEnum.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 1/7/2012

	Purpose: 

	API For enumerating graphics adapters and their abilities

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_WMENUM_H
#define _SST_WMENUM_H

#include "SST_WMTypes.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

	/*
		SST_WM_CreateGraphicsEnumerator()

		Creates an opaque enumeration object and returns it. This can subsequently be used
		by other various functions. To free memory associated with this, use SST_WM_DestroyGraphicsEnumerator().

		The function takes a snapshot of the system at the given moment, so all functions that operate on this
		object return cached values. This can lead to seemingly erroneous results if a graphics adapter is removed
		from the system, a monitor is unplugged or disabled, or the video mode is changed AFTER the enumerator has
		read from the system data. In this case, destroy the object and recreate it to get updated values.

		@return (SST_WM_GraphicsEnumerator) - The enumeration object, or NULL on error (no graphics cards, no memory, etc.)
	*/
	SST_GraphicsEnumerator SST_WM_CreateGraphicsEnumerator(void);

	/*
		SST_WM_GetEnumAdapterCount()

		Returns the number of graphics adapters in the system at the time of the enumeration.

		@param enumerator - The graphics enumerator object
		@retrun (size_t) - The number of graphics adapters in the system
	*/
	size_t SST_WM_GetEnumAdapterCount(SST_GraphicsEnumerator enumerator);

	/*
		SST_WM_GetEnumAdapterName()

		Gets the user-friendly name for the graphics adapter, such as "MassiveTurboFx Nausea3000"
		at the time of the enumeration.

		When 'nameReturn' is NULL, this queries the number of characters in the name, otherwise, it
		stores up to *bufferSize - 1 characters into 'nameReturn' and NULL-terminates it.

		@param enumerator - The enumerator object
		@param adapterIndex - The adapter index
		@param nameReturn - Pointer to where the name may be stored, or NULL for a length query.
		@param bufferSize - Pointer to the size of 'nameReturn'. When 'nameReturn' is NULL, this
							receives the number of bytes in the name (including null). When 'nameReturn'
							is not NULL, up to *bufferSize-1 characters are stored, and the string is
							null-terminated.
		@return (void)
	*/
	void SST_WM_GetEnumAdapterName(SST_GraphicsEnumerator enumerator, size_t adapterIndex, char* nameReturn, size_t* bufferSize);

	/*
		SST_WM_GetEnumScreenCount()
		
		Gets the number of screens associated with a graphics adapter at the time of the enumeration.

		@param enumerator - The enumerator object
		@param adapterIndex - The graphics adapter index
		@return (size_t) - The number of screens
	*/
	size_t SST_WM_GetEnumScreenCount(SST_GraphicsEnumerator enumerator, size_t adapterIndex);

	/*
		SST_WM_GetEnumVideoModes()
		
		Gets the video modes associated with this a particular adapter/screen pair at the time of the enumeration.

		@param enumerator - The enumerator object
		@param adapterIndex - The graphics adapter index
		@param screenIndex - The screen index
		@param modesReturn - Array of SST_VideoModes to be filled in, or NULL to get array size
		@param modeCountReturn - Pointer to value containing the size of modesReturn[], or when modesReturn is NULL, receives the size of the array required
		@return (void)
	*/
	void SST_WM_GetEnumVideoModes(SST_GraphicsEnumerator enumerator, size_t adapterIndex, size_t screenIndex, SST_VideoMode* modesReturn, size_t* modeCountReturn);

	/*
		SST_WM_GetEnumCurrentVideoMode()
		
		Gets the current video mode associated with this a particular adapter/screen pair at the time of the enumeration.

		@param enumerator - The enumerator object
		@param adapterIndex - The graphics adapter index
		@param screenIndex - The screen index
		@param mode - Array of SST_VideoModes to be filled in, or NULL to get array size
		@return (void)	
	*/
	void SST_WM_GetEnumCurrentVideoMode(SST_GraphicsEnumerator enumerator, size_t adapterIndex, size_t screenIndex, SST_VideoMode* mode);

	/*
		SST_WM_DestroyGraphicsEnumerator()
		
		Destroys the graphics enumerator object and frees all memory associated with it.

		@param enumerator - The enumerator object
		@return (void)
	*/
	void SST_WM_DestroyGraphicsEnumerator(SST_GraphicsEnumerator enumerator);

#ifdef __cplusplus
}
#endif

#endif
