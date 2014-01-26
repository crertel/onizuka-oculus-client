/*
	SST_Net.h
	Authors: Chris Ertel <crertel@762studios.com>, Patrick Baggett <ptbaggett@762studios.com>
	Created: 7/8/2012

	Purpose: 

	libsst-net main include

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_NET_H
#define _SST_NET_H

/* Version number constants for libsst-net */
#define SST_NET_VERSION_MAJOR	0x01
#define SST_NET_VERSION_MINOR	0x00
#define SST_NET_VERSION_PATCH	0x0000
#define SST_NET_VERSION			(SST_NET_VERSION_MAJOR << 24) | (SST_NET_VERSION_MINOR << 16) | (SST_NET_VERSION_PATCH)

#define SST_NET_VERSION_STRING	"1.0.0"

#include <SST/SST_NetAddress.h>
#include <SST/SST_NetResult.h>
#include <SST/SST_NetSocket.h>
#include <SST/SST_NetTypes.h>


#ifdef __cplusplus
extern "C" {
#endif

	int SST_Net_Init( void );
	void SST_Net_Shutdown( void );

#ifdef __cplusplus
}
#endif


#endif

