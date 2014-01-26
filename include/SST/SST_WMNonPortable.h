/*
	SST_WMNonPortable.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 5/30/2012

	Purpose: 

	Non-portable bits of libsst-wm

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_WMNONPORTABLE_H
#define _SST_WMNONPORTABLE_H

#include <SST/SST_WMTypes.h>

/* Win32 API based systems. TODO: replace with some constant in SST_Build.h */
#if defined(_WIN32) || defined(__WIN32) || defined(WIN32) || defined(__WIN32__)
	#include <windows.h>
	#define WMPLAT_WIN32
	#endif

/* X11-based systems. TODO: replace with some constant in SST_Build.h */
#if defined(__linux) || defined(__sun)
	#include <X11/Xlib.h>
	#define WMPLAT_X11
#endif

#ifdef __cplusplus
extern "C" {
#endif

	#ifdef WMPLAT_WIN32
	/*
		SST_WM_GetHWNDWin32()
		
		Gets the Win32 HWND for a specific window.
		
		@param window - The Window created by SST_WM_CreateWindow()
	*/
	HWND SST_WM_GetHWNDWin32(SST_Window window);
	#endif
	
	#ifdef WMPLAT_X11
	/*
		SST_WM_GetDisplayX11()
		
		Gets the Xlib Display* associated with a window
		
		@param window - The window created by SST_WM_CreateWindow()
		@return (Display*) - The Xlib Display* 
	*/	
	Display* SST_WM_GetDisplayX11(SST_Window window);
	
	/*
		SST_WM_GetWindowX11()
		
		Gets the Xlib Window associated with a window
		
		@param window - The window created by SST_WM_CreateWindow()
		@return (Window) - The Xlib Window handle
	*/	
	Window SST_WM_GetWindowX11(SST_Window window);
	#endif
	
#ifdef __cplusplus
}
#endif

#endif


