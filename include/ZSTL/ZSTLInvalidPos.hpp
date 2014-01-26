/*
	ZSTLInvalidPos.hpp
	Author: James Russell <jcrussell@762studios.com>
	Created: 12/27/2012

	Purpose: 

	Header file for ZSTL::InvalidPos, which includes only a single definition for
	the 'InvalidPos' structure.  This acts as a common return value for most containers
	wherein the return would be an invalid index or iterator.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _ZSTLINVALIDPOS_HPP
#define _ZSTLINVALIDPOS_HPP

#include <stdlib.h> // size_t, NULL

//Predeclare ZListIterator type
template <typename T> class ZListIterator;

/* The ZSTL namespace */
namespace ZSTL
{
	//Invalid Object structure, used to indicate an invalid position on many different types of containers
	struct _InvalidObject
	{
		//Implicit cast to size_t
		operator size_t () const 
			{ return ((size_t)-1); }

		//Implicit (and templated) cast to ZListIterator<T>
		template <typename T>
		operator ZListIterator<T> () const
			{ return ZListIterator<T>(NULL, NULL); }
	};

	/*
	Indicator used to check when an algorithm has returned an invalid position or iterator.  
	Many algorithms return an index or iterator that needs to be checked against an 'invalid'
	return, such as 'FindFirstOf' returning a size_t value for arrays and an iterator
	for lists.

	This structure can be used to check against all types of returns. 

		if (ZArrayAlgo::FindFirstOf(...) != ZSTL::InvalidPos)
			...

		if (ZListAlgo::FindFirstOf(...) != ZSTL::InvalidPos)
			...
	*/
	const static _InvalidObject InvalidPos = _InvalidObject();
}

#endif

