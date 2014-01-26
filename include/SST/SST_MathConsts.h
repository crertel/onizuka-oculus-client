/*
	SST_Math.h
	Author: Patrick Baggett <ptbaggett@762studios.com
	Created: 8/30/2012

	Purpose: 
	
	Simple math constants

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.
	
*/
#ifndef _SST_MATHCONSTS_H
#define _SST_MATHCONSTS_H

/* Just kill all previous definitions, who knows how accurate they are or aren't */
#undef M_PI
#undef M_PI_2
#undef M_PI_4
#undef M_1_PI
#undef M_2_PI
#undef M_2_SQRTPI
#undef M_E
#undef M_LOG2E
#undef M_LOG10E
#undef M_LN2
#undef M_LN10
#undef M_SQRT2
#undef M_SQRT1_2


#define M_PI		3.14159265358979323846		/* pi */
#define M_PI_2		1.57079632679489661923		/* pi / 2.0 */
#define M_PI_4		0.785398163397448309616		/* pi / 4.0 */
#define M_1_PI		0.318309886183790671538		/* 1.0 / pi */
#define M_2_PI		0.636619772367581343076		/* 2.0 / pi */
#define M_2_SQRTPI	1.12837916709551257390		/* 2.0 / sqrt(pi) */
#define M_E			2.71828182845904523536		/* e */
#define M_LOG2E		1.44269504088896340736		/* log base 2 of e */
#define M_LOG10E	0.434294481903251827651		/* log base 10 of e */
#define M_LN2		0.693147180559945309417		/* natural log of 2 */
#define M_LN10		2.30258509299404568402		/* natural log of 10 */
#define M_SQRT2		1.41421356237309504880		/* sqrt(2.0) */
#define M_SQRT1_2	0.707106781186547524401		/* 1.0 / sqrt(2.0) */

#endif