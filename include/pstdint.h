/*
	pstdint.h
	Author: Patrick Baggett
	
	Purpose: <stdint.h> replacement for data types. NOT C99 COMPLIANT YET.

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.
*/

#ifndef _PSTDINT_H
#define _PSTDINT_H

/*
	To make this file a no-op, simply do #define PSTDINT_OK. On platforms that have <stdint.h>,
	best efforts are made to include them.
*/
#if !defined(PSTDINT_OK)
	/*
		Most implementations require this macro to explicitly define UINT8_MAX and similar 
		in stdint.h for C++ compilers.  Evidently they need to be explicitly requested.
	*/
	#define __STDC_LIMIT_MACROS

	#if (defined(__STDC__) && (__STDC_VERSION__ >= 199901L)) || \
		(defined(__GNUC__) || (__SUNPRO_C) || defined(__SUNPRO_CC)) || \
		(defined(_MSC_VER) && _MSC_VER >= 1700) /* Definitely C99 or provided by the compiler */

		/* Just use <stdint.h> */
		#include <stdlib.h> /* For size_t */
		#include <stdint.h>

	#elif defined(_MSC_VER) && _MSC_VER < 1700 /* Visual Studio on Windows. VS 2012 and later offers stdint.h */
		
		typedef signed __int8 int8_t;
		#define INT8_MIN ((int8_t)0x80)
		#define INT8_MAX ((int8_t)0x7F)

		typedef signed __int16 int16_t;
		#define INT16_MIN ((int16_t)0x8000)
		#define INT16_MAX ((int16_t)0x7FFF)

		typedef signed __int32 int32_t;
		#define INT32_MIN ((int32_t)0x80000000)
		#define INT32_MAX ((int32_t)0x7FFFFFFF)

		typedef signed __int64 int64_t;
		#define INT64_MIN ((int64_t)0x8000000000000000LL)
		#define INT64_MAX ((int64_t)0x7FFFFFFFFFFFFFFFLL)

		typedef unsigned __int8 uint8_t;
		#define UINT8_MIN ((uint8_t)0x00U)
		#define UINT8_MAX ((uint8_t)0xFFU)

		typedef unsigned __int16 uint16_t;
		#define UINT16_MIN ((uint16_t)0x0000U)
		#define UINT16_MAX ((uint16_t)0xFFFFU)

		typedef unsigned __int32 uint32_t;
		#define UINT32_MIN ((uint32_t)0x00000000U)
		#define UINT32_MAX ((uint32_t)0xFFFFFFFFU)

		typedef unsigned __int64 uint64_t;
		#define UINT64_MIN ((uint64_t)0x0000000000000000ULL)
		#define UINT64_MAX ((uint64_t)0xFFFFFFFFFFFFFFFFULL)
		
		/* __w64 is used to mark types that change their size */
		#if defined(_WIN64) || defined(_M_IA64) || defined(_M_AMD64) || defined(_M_X64)
			typedef __w64 __int64 intptr_t;
			typedef __w64 unsigned __int64 uintptr_t;
		#else
			typedef __w64 __int32 intptr_t;
			typedef __w64 unsigned __int32 uintptr_t;
		#endif

		/* "Least" types -- identical to normal counterparts */
		typedef int8_t    int_least8_t;
		typedef int16_t   int_least16_t;
		typedef int32_t   int_least32_t;
		typedef int64_t   int_least64_t;
		typedef uint8_t   uint_least8_t;
		typedef uint16_t  uint_least16_t;
		typedef uint32_t  uint_least32_t;
		typedef uint64_t  uint_least64_t;

		/* "Fast" types -- identical to normal counterparts */
		typedef int8_t    int_fast8_t;
		typedef int16_t   int_fast16_t;
		typedef int32_t   int_fast32_t;
		typedef int64_t   int_fast64_t;
		typedef uint8_t   uint_fast8_t;
		typedef uint16_t  uint_fast16_t;
		typedef uint32_t  uint_fast32_t;
		typedef uint64_t  uint_fast64_t;

	#endif

#endif /* !defined(PSTDINT_OK) */


#endif

