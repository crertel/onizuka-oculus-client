/*
	SST_WMWindow.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 5/30/2012

	Purpose: 

	Query/set video mode on a display target / screen

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_WMVIDEOMODE_H
#define _SST_WMVIDEOMODE_H

#include <SST/SST_WMTypes.h> /* For definition of SST_DEFAULT_SCREEN */
/*
	These functions are somewhat like the ones in SST_WMEnum.h, but operate "right now" and do not
	cache information.
*/

#ifdef __cplusplus
extern "C" {
#endif

	/*
		SST_WM_SetVideoModeOnScreen()

		Attempts to change the video mode on a specific screen. This is useful for changing the video
		mode of one screen in a multihead environment.

		@param target - The display target
		@param screenIndex - The screen index
		@param vmode - The video mode to set
		@return (int) - Non-zero if successful, zero if failure (not enough video memory, mode not supported, etc.)
	*/
	int SST_WM_SetVideoModeOnScreen(SST_DisplayTarget target, size_t screenIndex, const SST_VideoMode* vmode);

	/*
		SST_WM_GetVideoModeOnScreen()

		Gets the video mode on a specific screen. This is useful for getting the video
		mode of one screen in a multihead environment.

		@param target - The display target
		@param screenIndex - The screen index
		@param vmode - Pointer to storage for video mode information
		@return (int) - Non-zero if successful, zero if failure (screen disconnected, graphics adapter disconnected, etc.)
	*/
	int SST_WM_GetVideoModeOnScreen(SST_DisplayTarget target, size_t screenIndex, SST_VideoMode* vmodeReturn);

	/*
		SST_WM_SetVideoMode()

		Attempts to change the video mode on the default screen.

		@param target - The display target
		@param vmode - The video mode to set
		@return (int) - Non-zero if successful, zero if failure (not enough video memory, mode not supported, etc.)
	*/
	#define SST_WM_SetVideoMode(displayTarget, vmode) SST_WM_SetVideoModeOnScreen(displayTarget, SST_DEFAULT_SCREEN, vmode) 

	/*
		SST_WM_GetVideoMode()

		Gets the video mode on the default screen.

		@param target - The display target
		@param vmode - Pointer to storage for video mode information
		@return (int) - Non-zero if successful, zero if failure (screen disconnected, graphics adapter disconnected, etc.)
	*/
	#define SST_WM_GetVideoMode(displayTarget, vmode)	SST_WM_GetVideoModeOnScreen(displayTarget, SST_DEFAULT_SCREEN, vmode)
	


#ifdef __cplusplus
}
#endif


#endif
