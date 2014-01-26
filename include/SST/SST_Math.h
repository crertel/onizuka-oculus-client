/*
	SST_Math.h
	Author: James Russell <jcrussell@762studios.com>
	Created: 12/04/2011

	Purpose: 
	
	Includes all relevant libsst-math header files.

	License:

	TODO
*/

#ifndef _SST_MATH_H
#define _SST_MATH_H

/* Version number constants for libsst-math */
#define SST_MATH_VERSION_MAJOR	0x01
#define SST_MATH_VERSION_MINOR	0x00
#define SST_MATH_VERSION_PATCH	0x0000
#define SST_MATH_VERSION	(SST_MATH_VERSION_MAJOR << 24) | (SST_MATH_VERSION_MINOR << 16) | (SST_MATH_VERSION_PATCH)

#define SST_MATH_VERSION_STRING	"1.0.0"

#include <SST/SST_Build.h>

#if SST_COMPILER == SST_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable:4201)
#endif

#include <SST/SST_Vec2.h>
#include <SST/SST_Vec3.h>
#include <SST/SST_Vec4.h>

#include <SST/SST_Mat22.h>
#include <SST/SST_Mat33.h>
#include <SST/SST_Mat44.h>
#include "SST_Transform.h"
#include "SST_Geo.h"

#if SST_COMPILER == SST_COMPILER_MSVC
#pragma warning(pop)
#endif

#include <SST/SST_MathConsts.h>

#endif
 
