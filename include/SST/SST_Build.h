/*
	SST_Build.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/23/2011

	Purpose: 

	Common compiler-specific bits for C/C++ code.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_BUILD_H
#define _SST_BUILD_H

#undef RESTRICT
#undef INLINE

#define SST_COMPILER_MSVC	0		/* Microsoft Visual C++ */
#define SST_COMPILER_GCC	1		/* GNU Compiler Collection */
#define SST_COMPILER_SUNPRO	2		/* SunPro compilers (Sun Studio / Solaris Studio) */
#define SST_COMPILER_CLANG	3		/* clang */
#define SST_COMPILER_INTEL	4		/* Intel C/C++ compiler (icc) */

#if defined(_MSC_VER)								/* Microsoft Visual C/C++ compiler */
	#define	SST_ALIGN_PREFIX(n)		/*__declspec(align(n))*/
	#define SST_ALIGN_SUFFIX(n)		/* empty */
	#define SST_ASSUME_ALIGNED(a,n) /* empty */
	#define SST_FUNCNAME			__FUNCTION__	
	#define INLINE					__forceinline
	#define RESTRICT				__restrict
	#define WHILE_NOP				(void)0,0
	#define WHILE_TRUE				(void)1,1
	#define SST_COMPILER			SST_COMPILER_MSVC
	#define CONSTFUNC				/* empty */
#elif defined(__ICC) || defined(__INTEL_COMPILER)	/* Intel C/C++ compiler (ICC defines __GNUC__...grr) */
	#define SST_ALIGN_PREFIX(n)		/* empty */
	#define SST_ALIGN_SUFFIX(n)		/* empty */
	#define SST_ASSUME_ALIGNED(a,n) __assume_aligned(a,n)
	#define SST_FUNCNAME			__PRETTY_FUNCTION__ /* Supposedly, ICC supports this... */
	#define INLINE					__inline__
	#define RESTRICT				__restrict__
	#define WHILE_NOP				0
	#define WHILE_TRUE				1
	#define SST_COMPILER			SST_COMPILER_INTEL
	#define CONSTFUNC				/* empty */
#elif defined(__clang__)							/* clang */
	#define SST_ALIGN_PREFIX(n)		/* empty */
	#define SST_ALIGN_SUFFIX(n)		__attribute__ ((aligned(n)))
	#define SST_ASSUME_ALIGNED(a,n) /* empty */
	#define SST_FUNCNAME			__FUNCTION__	
	#define INLINE					__inline__
	#define RESTRICT				__restrict__
	#define WHILE_NOP				0
	#define WHILE_TRUE				1
	#define SST_COMPILER			SST_COMPILER_CLANG
	#define CONSTFUNC				__attribute__((const))
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)	/* Sun C/C++ compiler */
	#define SST_ALIGN_PREFIX(n)		/* empty */
	#define SST_ALIGN_SUFFIX(n)		__attribute__ ((aligned(n)))
	#define SST_ASSUME_ALIGNED(a,n)	/* Not supported by Sun C/C++ */
	#define SST_FUNCNAME			__PRETTY_FUNCTION__
	#define INLINE					__inline
	#ifdef __SUNPRO_C
		#define RESTRICT			restrict
	#else
		#define RESTRICT			/* Sun C++ doesn't do restrict */
	#endif
	#define WHILE_NOP				0
	#define WHILE_TRUE				1
	#define SST_COMPILER			SST_COMPILER_SUNPRO
	#define CONSTFUNC				__attribute__((const)) /* SunPro 12.2 implements GCC's const attribute */
#elif defined(__GNUC__)								/* GNU C/C++ compiler */
	#define SST_ALIGN_PREFIX(n)		/* empty */
	#define SST_ALIGN_SUFFIX(n)		__attribute__ ((aligned(n)))

	/* GCC >= 4.7.0 adds __builtin_assume_aligned() */
	#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)
		#define SST_ASSUME_ALIGNED(a,n) a = __builtin_assume_aligned(a,n)
	#else
		#define SST_ASSUME_ALIGNED(a,n) /* need GCC 4.7.0+ */
	#endif
	#define SST_FUNCNAME			__PRETTY_FUNCTION__
	#define INLINE					__inline__
	#define RESTRICT				__restrict__
	#define WHILE_NOP				0
	#define WHILE_TRUE				1
	#define SST_COMPILER			SST_COMPILER_GCC
	#define CONSTFUNC				__attribute__((const))
#endif

#if !defined(SST_COMPILER)
#error SST_Build.h: Your compiler was not recognized.
#endif

#include <string.h>		/* memcpy() */
#include <pstdint.h>	/* int types */
#include <pstdbool.h>	/* boolean types */

/* Correctly cast raw integer bits to floating point values and back */
static INLINE float cast_i2f(uint32_t x)
{
	float out;
	memcpy(&out, &x, sizeof(float));
	return out;
}

static INLINE double cast_i2d(uint64_t x)
{
	double out;
	memcpy(&out, &x, sizeof(double));
	return out;
}

static INLINE uint32_t cast_f2i(float x)
{
	uint32_t out;
	memcpy(&out, &x, sizeof(uint32_t));
	return out;
}

static INLINE uint64_t cast_d2i(double x)
{
	uint64_t out;
	memcpy(&out, &x, sizeof(uint64_t));
	return out;
}

#endif

