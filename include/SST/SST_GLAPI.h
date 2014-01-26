/*
	SST_GLAPI.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/12/2012

	Purpose: 

	libsst-glapi main include

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_GLAPI_H
#define _SST_GLAPI_H

#include <SST/SST_GLAPIDefs.h>
#include <SST/SST_GLAPIStruct.h>
#include <SST/SST_GLAPIContext.h>
#include <SST/SST_GLAPIMacros.h>

/* Version number constants for libsst-glapi */
#define SST_GLAPI_VERSION_MAJOR		0x01
#define SST_GLAPI_VERSION_MINOR		0x00
#define SST_GLAPI_VERSION_PATCH		0x0000
#define SST_GLAPI_VERSION			(SST_GLAPI_VERSION_MAJOR << 24) | (SST_GLAPI_VERSION_MINOR << 16) | (SST_GLAPI_VERSION_PATCH)

#define SST_GLAPI_VERSION_STRING	"1.0.0"


#endif

