/*
	SST_WMWindow.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 5/14/2012

	Purpose: 

	Display target and window functions

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_WMWINDOW_H
#define _SST_WMWINDOW_H

#include <SST/SST_WMTypes.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

	/*
		SST_WM_CreateDisplayTarget()
		
		Creates a display target from the given adapter-screen combination. Use SST_MULTIHEAD as the screen
		to support drawing to all attached screens.

		@param adapterIndex - The graphics adapter index
		@param screenIndexOrMultihead - The screen index, or SST_MULTIHEAD to enable multihead support
		@return (SST_DisplayTarget) - The display target, or NULL on error.
	
	*/
	SST_DisplayTarget SST_WM_CreateDisplayTarget(size_t adapterIndex, size_t screenIndexOrMultihead);

	/*
		SST_WM_GetDisplayTargetScreenCount()
		
		Gets the number of screens associated with a display target. Unless the display target was created with SST_MULTIHEAD,
		this will be 1.

		@param target - The display target
		@return (size_t) - The number of screens associated with the display target
	
	*/
	size_t SST_WM_GetDisplayTargetScreenCount(SST_DisplayTarget target);
	
	/*
		SST_WM_CreateWindow()

		Creates a window on the default screen. This is a shortcut when multihead is not enabled.

		@param dispTarget - The display target
		@param x - The window's preferred X position on screen. This may not be honored exactly.
		@param y - The window's preferred Y position on screen. This may not be honored exactly.
		@param width - The width of the window's client area (drawable area). 
		@param height - The height of the window's client area (drawable area).
		@param title - The text for the window titlebar
	*/
	#define SST_WM_CreateWindow(dispTarget, x, y, width, height, title) SST_WM_CreateWindowOnScreen(dispTarget, SST_DEFAULT_SCREEN, x, y, width, height, title)

	/*
		SST_WM_CreateWindowOnScreen()

		Creates a window on the specified screen.

		@param target - The display target
		@param screenIndex - The index of the screen
		@param x - The window's preferred X position on screen. This may not be honored exactly.
		@param y - The window's preferred Y position on screen. This may not be honored exactly.
		@param width - The width of the window's client area (drawable area). 
		@param height - The height of the window's client area (drawable area).
		@param title - The text for the window titlebar
	*/
	SST_Window SST_WM_CreateWindowOnScreen(SST_DisplayTarget target, size_t screenIndex, uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char* title);
	
	/*
		SST_WM_GetWindowDisplayTarget()
		
		Gets the display target that this window was created from.
		
		@param window - The window to get the display target of.
		@return (SST_DisplayTarget) - The display target.
	*/
	SST_DisplayTarget SST_WM_GetWindowDisplayTarget(SST_Window window);
	
	/*
		SST_WM_SetWindowText()

		Sets the window's titlebar text.

		@param window - The window
		@param titleBar - The UTF-8 titlebar text, null-terminated.
	*/
	void SST_WM_SetWindowText(SST_Window window, const char* titleBar);
	
	/*
		SST_WM_GetWindowRect()

		Gets the window's rectangle, which consists of its position on screen and the width/height 
		of its client area.

		@param window - The window
		@param rect - Pointer to an SST_Rect structure to receive these values
	*/
	void SST_WM_GetWindowRect(SST_Window window, SST_Rect* rectReturn);

	/*
		SST_WM_MoveWindow()

		Moves the window to the new X/Y coords, relative to the same screen. The OS may ignore/modify this if they are
		invalid, out of range, or would cause a bad user experience. Users tend to move their own windows, so using
		this function is generally discouraged unless doing something fancy, like attempting to arrange a series
		windows. This results in a SSTWMEVENT_MOVED event being sent with the new position.

		@param window - The window
		@param x - The new X position
		@param y - The new Y position
	*/
	#define SST_WM_MoveWindow(window, x, y) SST_WM_MoveWindowOnScreen(window, SST_SAME_SCREEN, x, y);
	
	/*
		SST_WM_MoveWindowOnScreen()

		Moves the window to the new X/Y coords, relative to the given screen. The OS may ignore/modify this if they are
		invalid, out of range, or would cause a bad user experience. Users tend to move their own windows, so using
		this function is generally discouraged unless doing something fancy, like attempting to arrange a series
		windows. This results in a SSTWMEVENT_MOVED event being sent with the new position.

		@param window - The window
		@param screenIndex - The index of the screen, or SST_SAME_SCREEN for a move relative to the same screen
		@param x - The new X position
		@param y - The new Y position
	*/
	void SST_WM_MoveWindowOnScreen(SST_Window window, size_t screenIndex, uint32_t x, uint32_t y);

	/*
		SST_WM_ResizeWindow()

		Resizes the window. The OS may ignore/modify this if they are invalid, out of range, or would
		cause a bad user experience. This results in a SSTWMEVENT_RESIZED event being sent with the new
		size.

		@param window - The window
		@param width - The new width, at least 1 pixel
		@param height - The new height, at least 1 pixel
	*/
	void SST_WM_ResizeWindow(SST_Window window, uint32_t width, uint32_t height);


	/*
		SST_WM_ShowDialogBox

		Shows a modal dialog box prompting for user input. This does not return until the dialog box is closed.

		@param target - The display target
		@param parent - The parent window, or NULL. Though the 
		@param caption - UTF-8 string to be displayed as the window's caption (usually above window)
		@param message - UTF-8 string to be displayed as the window's content
		@param buttons - Array of UTF-8 strings to be displayed as the buttons
		@param nrButtons - The number of buttons (i.e. size of buttons[] array)

		@return (int) - The zero-based index of the button pushed, or -1 if the window was closed by other means (such as [X] in corner, Alt+F4, etc.)
	*/
	int SST_WM_ShowDialogBox(SST_DisplayTarget target, SST_Window parent, const char* caption, const char* message, const char** buttons, int nrButtons);

	/*
		SST_WM_SetWindowState()

		Changes a window state parameter. See SST_WMWindowState enum for list of parameters.

		@param window - The window
		@param state - The state to change
		@param param - The value to set the state to.
	*/
	void SST_WM_SetWindowState(SST_Window window, SST_WMWindowState state, uint32_t param);
	
	/*
		SST_WM_SetDisplayTargetState()

		Changes a display target state parameter. See SST_WMWDisplayTargetState enum for list of parameters.

		@param target - The display target
		@param state - The state to change
		@param param - The value to set the state to.
	*/
	void SST_WM_SetDisplayTargetState(SST_DisplayTarget target, SST_WMDisplayTargetState state, uint32_t param);

	/*
		SST_WM_DestroyWindow()

		Destroys a window.

		@param window - The window
	*/
	void SST_WM_DestroyWindow(SST_Window window);

	/*
		SST_WM_DestroyDisplayTarget()

		Destroys a display target

		@param target - The display target
	*/
	void SST_WM_DestroyDisplayTarget(SST_DisplayTarget target);

#ifdef __cplusplus
}
#endif

#endif

