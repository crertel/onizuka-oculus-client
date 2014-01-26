/*
	SST_Time.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 5/29/2013

	Purpose: 

	libsst-os timing functions

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_TIME_H
#define _SST_TIME_H

#include <pstdint.h>

#ifdef __cplusplus
extern "C" {
#endif
	/*
		SST_OS_GetMicroTime()

		Gets the time in microseconds since some fixed, arbitrary point. Subtracting
		two invocations of this can be used to find time deltas.

		@return (uint64_t) - The number of microseconds.
	*/
	uint64_t SST_OS_GetMicroTime();

	/*
		SST_OS_GetMilliTime()

		Gets the time in milliseconds since some fixed, arbitrary point. Subtracting
		two invocations of this can be used to find time deltas.

		@return (uint64_t) - The number of milliseconds.
	*/
	uint64_t SST_OS_GetMilliTime();


	/*
		SST_OS_GetFloatingTime()

		Gets the time in seconds since some fixed, arbitrary point. Subtracting
		two invocations of this can be used to find time deltas. Value are represented
		using double-precision floating point values, so a value of 0.25 means that
		250 milliseconds has elapsed. Note that the difference between two relatively close (few minutes)
		timestamps can be safely represented as a 32-bit floating point value.

		@return (double) - The number of seconds.
	*/
	double SST_OS_GetFloatingTime();

#ifdef __cplusplus
}
#endif


#endif


