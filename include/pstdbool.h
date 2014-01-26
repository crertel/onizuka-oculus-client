/*
	pstdbool.h
	Author: Chris Ertel <crertel@762studios.com>
	
	Purpose: <stdbool.h> replacement for data types. NOT C99 COMPLIANT YET.

	Notes: Macros were derived from spec at ( http://pubs.opengroup.org/onlinepubs/007904875/basedefs/stdbool.h.html ).

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.
*/

#ifndef _PSTDBOOL_H
#define _PSTDBOOL_H

/*
	To make this file a no-op, simply do #define PSTDBOOL_OK. On platforms that have <stdbool.h>,
	best efforts are made to include them.
*/

/* If the user is calling this from a C++ compiler, they probably don't need to be using this. */
#ifndef __cplusplus


#if !defined(PSTDBOOL_OK) && __STDC__ && (__STDC_VERSION__ >= 199901L)

	/* Compiler reports C99 available, so just use <stdbool.h> */
	#include <stdbool.h>

#elif !defined(PSTDBOOL_OK) && (defined(_MSC_VER) || (defined(__SUNPRO_C) && !defined(_STDC_C99)))
	/* Visual Studio on Windows */
	/* SunC in C90 mode gives an error if stdbool.h is included */ 
	#undef true
	#undef false
	#undef bool

	/* SunC 12.3 treats _Bool as a keyword, not a typedef, even in C90 mode */	
	#if !defined(__SUNPRO_C)
	typedef int _Bool;
	#endif

	#define bool _Bool
	#define true (1)
	#define false (0)
	#define __bool_true_false_are_defined 1
	
#elif !defined(PSTDBOOL_OK) && (defined(__GNUC__) || defined(__SUNPRO_C))

	/* Definitely provided by the compiler */
	#include <stdbool.h>

#endif

#endif

#endif

