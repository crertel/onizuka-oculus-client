/*
	SST_WM.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 1/5/2012

	Purpose: 

	762 Studios Window Management Library

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_WM_H
#define _SST_WM_H

/* Version number constants for libsst-wm */
#define SST_WM_VERSION_MAJOR	0x01
#define SST_WM_VERSION_MINOR	0x00
#define SST_WM_VERSION_PATCH	0x0000
#define SST_WM_VERSION			(SST_WM_VERSION_MAJOR << 24) | (SST_WM_VERSION_MINOR << 16) | (SST_WM_VERSION_PATCH)

#define SST_WM_VERSION_STRING	"1.0.0"

#include "SST_WMEnum.h"
#include "SST_WMEvent.h"
#include "SST_WMKeys.h"
#include "SST_WMNonPortable.h"
#include "SST_WMOpenGL.h"
#include "SST_WMRender.h"
#include "SST_WMTypes.h"
#include "SST_WMVideoMode.h"
#include "SST_WMWindow.h"

#endif


