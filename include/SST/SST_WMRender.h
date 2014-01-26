/*
	SST_WMRender.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 9/19/2012

	Purpose: 

	Software rendering support for windows

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_WMRENDER_H
#define _SST_WMRENDER_H

#include <SST/SST_WMTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

	/*
		SST_WM_EnableSoftwareRendering()
		
		Enbales software rendering on this window.

		@param window - The window
		@return (int) - Zero on failure, non-zero on success
	*/
	int SST_WM_EnableSoftwareRendering(SST_Window window);

	/*
		SST_WM_DisableSoftwareRendering()
		
		Disables software rendering on this window.

		@param window - The window
		@return (SST_Window) - The window
	*/
	void SST_WM_DisableSoftwareRendering(SST_Window window);

	/*
		SST_WM_LockBackbuffer()
		
		Locks the backbuffer and returns a pointer to it.

		@param window - The window
		@param pitchReturn - The pitch, in bytes, of a row of pixels
		@return (void*) - The backbuffer pointer
	*/
	void* SST_WM_LockBackbuffer(SST_Window window, size_t* pitchReturn);

	/*
		SST_WM_UnlockBackbuffer()
		
		Unlocks the backbuffer and signals the window's contents to be updated

		@param window - The window
		@return (void)
	*/
	void SST_WM_UnlockBackbuffer(SST_Window window);

#ifdef __cplusplus
}
#endif

#endif


