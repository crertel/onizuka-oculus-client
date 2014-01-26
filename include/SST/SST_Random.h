/*
	SST_Random.h
	Author: Chris Ertel <crertel@762studios.com>
	Created: 2/26/2012

	Purpose: 

	Primary header for libsst-random, which includes all other libsst-random headers.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_RANDOM_H
#define _SST_RANDOM_H

/* Version number constants for libsst-random */
#define SST_RANDOM_VERSION_MAJOR	0x01
#define SST_RANDOM_VERSION_MINOR	0x01
#define SST_RANDOM_VERSION_PATCH	0x0000
#define SST_RANDOM_VERSION			(SST_RANDOM_VERSION_MAJOR << 24) | (SST_RANDOM_VERSION_MINOR << 16) | (SST_RANDOM_VERSION_PATCH)

#define SST_RANDOM_VERSION_STRING	"1.1.0"

/* Pseudo Random Number Generator functions */
#include "SST_PRNG.h"

/* Noise Functions */
#include "SST_SimplexNoise.h"

#endif
