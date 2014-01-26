/*
	SST_Crypto.h
	Author: James Russell <jcrussell@762studios.com>
	Created: 11/06/2011

	Purpose: 

	Primary header for libsst-crypto, which includes all other libsst-crypto headers.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#ifndef _SST_CRYPTO_H
#define _SST_CRYPTO_H

/* Version number constants for libsst-crypto */
#define SST_CRYPTO_VERSION_MAJOR	0x01
#define SST_CRYPTO_VERSION_MINOR	0x01
#define SST_CRYPTO_VERSION_PATCH	0x0000
#define SST_CRYPTO_VERSION	(SST_CRYPTO_VERSION_MAJOR << 24) | (SST_CRYPTO_VERSION_MINOR << 16) | (SST_CRYPTO_VERSION_PATCH)

#define SST_CRYPTO_VERSION_STRING	"1.1.0"

/* Hashing functions */
#include "SST_Hash.h"			//Useful 32 and 64-bit hashing functions for strings

#endif
 
