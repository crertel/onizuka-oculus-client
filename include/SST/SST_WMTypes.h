/*
	SST_WMTypes.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 1/7/2012

	Purpose: 

	Provides datatypes for libsst-wm

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_WMTYPES_H
#define _SST_WMTYPES_H

#include <pstdint.h>

typedef void* SST_DisplayTarget; /**< Represents an adapter/screen pair, or adapter/MULTIHEAD constant */
typedef void* SST_OpenGLContext;
typedef void* SST_RenderContext;
typedef void* SST_Window;
typedef void* SST_GraphicsEnumerator;

#define SST_MULTIHEAD	SIZE_MAX			/**< Special constant to represent multihead mode */
#define SST_DEFAULT_ADAPTER ((size_t)0)	/**< Use the default adapter */
#define SST_DEFAULT_SCREEN ((size_t)0)	/**< Use the default screen */
#define SST_DEFAULT_REFRESHRATE ((uint32_t)0) /**< Use the default refresh rate */
#define SST_SAME_SCREEN	SIZE_MAX

#define SST_MBLEFT		0
#define SST_MBMIDDLE	1
#define SST_MBRIGHT		2

#define SST_MB1	SST_MBLEFT		/**< Alias for left mouse button */
#define SST_MB2	SST_MBMIDDLE		/**< Alias for middle mouse button */
#define SST_MB3 SST_MBRIGHT		/**< Alias for right mouse button */
#define SST_MB4 3				/**< 4th button on mouse */
#define SST_MB5 4				/**< 5th button on mouse */

#define SSTKEYMOD_REPEAT	(1 << 0)	/**< Key press is a repeat */
#define SSTKEYMOD_CONTROL	(1 << 1)	/**< Key was pressed with control down */
#define SSTKEYMOD_ALT		(1 << 2)	/**< Key was pressed with alt down */
#define SSTKEYMOD_SHIFT		(1 << 3)	/**< Key was pressed with shift down */

/** Structure describing a video mode */
typedef struct SST_VideoMode
{
	uint32_t width;	/**< Width, in pixels */
	uint32_t height; /**< Height, in pixels */
	uint32_t bpp; /**< Bits per pixel. */
	uint32_t refreshRate; /**< Refresh rate, in Hz, or SSTWM_DEFAULT_REFRESHRATE if "info not available" or "don't care" */
} SST_VideoMode;

typedef struct SST_Rect
{
	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
} SST_Rect;

typedef enum SST_WMWindowState
{
	SSTWS_SHOWN,		/**< Show / hide a window */
	SSTWS_RESIZEABLE,	/**< Enable/disable resizing */
	SSTWS_FULLSCREEN,	/**< Map a window as fullscreen or not */
	SSTWS_MINIMIZED		/**< Minimized or not */
} SST_WMWindowState;

typedef enum SST_WMDisplayTargetState
{
	SSTDTS_RELMOUSE		/**< XY Mouse movement reported at relative, cursor disappears */
} SST_WMDisplayTargetState;

#endif
