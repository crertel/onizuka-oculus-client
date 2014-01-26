/*
	ZSTL.hpp
	Author: James Russell <jcrussell@762studios.com>
	Created: 12/26/2011

	Purpose: 

	Header file for ZSTL that includes all ZSTL containers.  Container utility libraries
	are not included - these must be included individually.

	Defining the following features to 1 enables the feature on all ZSTL containers.  Defining 
	to 0 disables the feature (default behavior if undefined).

	ZSTL_CHECK_INTEGRITY
	Checks integrity of the ZSTL containers after allocations or if non-const functions are called.  
	Used for debugging ZSTL.  Useful if new methods are added.

	ZSTL_DISABLE_RUNTIME_CHECKS
	Disables runtime bounds and error checking on ZSTL containers, iterators, and algorithms.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _ZSTL_HPP
#define _ZSTL_HPP

/* Version number constants for ZSTL */
#define ZSTL_VERSION_MAJOR	0x01
#define ZSTL_VERSION_MINOR	0x01
#define ZSTL_VERSION_PATCH	0x0000
#define ZSTL_VERSION		(ZSTL_VERSION_MAJOR << 24) | (ZSTL_VERSION_MINOR << 16) | (ZSTL_VERSION_PATCH)

#define ZSTL_VERSION_STRING	"1.1.0"

/* The ZSTL headers */
#include "ZSTLInvalidPos.hpp"
#include "ZSTLCommon.hpp"
#include "ZPair.hpp"
#include "ZArray.hpp"
#include "ZList.hpp"
#include "ZHashMap.hpp"
#include "ZRingBuffer.hpp"
#include "ZString.hpp"


#endif
