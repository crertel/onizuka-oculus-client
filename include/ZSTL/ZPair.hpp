/*
	ZPair.hpp
	Author: James Russell <jcrussell@762studios.com>
	Created: 9/12/2011

	Purpose: 

	Templated tuple implementation.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _ZPAIR_HPP
#define _ZPAIR_HPP

#include <ZSTL/ZSTLCommon.hpp>

//Forward Declaration of ZPair
template <typename T1, typename T2>
class ZPair;

/* 
Forward Declaration of ZPair Method Implementation Structures 

Existence of these structures allows for template specialization of 
individual methods of the ZPair class.  In order to specialize
a single method of ZPair for a function, simply specialize the corresponding
method implementation structure.
*/

//Forward Declaration of ZPair<T1, T2>::operator < struct
template <typename T1, typename T2>
struct ZPair_OperatorLessThanImpl {
	inline static bool Call(const ZPair<T1, T2>& _self, const ZPair<T1, T2>& _other);
};

//Forward Declaration of ZPair<T1, T2>::operator = struct
template <typename T1, typename T2>
struct ZPair_OperatorEqualsAssignImpl {
	inline static ZPair<T1, T2>& Call(ZPair<T1, T2>& _self, const ZPair<T1, T2>& _other);
};

//Forward Declaration of ZPair<T1, T2>::operator == struct
template <typename T1, typename T2>
struct ZPair_OperatorEqualsCompareImpl {
	inline static bool Call(const ZPair<T1, T2>& _self, const ZPair<T1, T2>& _other);
};

//Forward Declaration of ZPair<T1, T2>::operator != struct
template <typename T1, typename T2>
struct ZPair_OperatorNotEqualImpl {
	inline static bool Call(const ZPair<T1, T2>& _self, const ZPair<T1, T2>& _other);
};

//Forward Declaration of ZPair<T1, T2>::Swap struct
template <typename T1, typename T2>
struct ZPair_SwapImpl {
	inline static ZPair<T2, T1> Call(const ZPair<T1, T2>& _self);
};

/*
Dynamic tuple implementation.

The template parameter T1 is the type of the first contained value.

The template parameter T2 is the type of the second contained value.
*/
template <typename T1, typename T2>
class ZPair
{
friend struct ZPair_OperatorLessThanImpl<T1, T2>;
friend struct ZPair_OperatorEqualsAssignImpl<T1, T2>;
friend struct ZPair_OperatorEqualsCompareImpl<T1, T2>;
friend struct ZPair_OperatorNotEqualImpl<T1, T2>;
friend struct ZPair_SwapImpl<T1, T2>;

public:
	//The first value
	T1 First;

	//The second value
	T2 Second;

	/*
	Default constructor.
	*/
	ZPair()
		: First(), Second() { }

	/*
	Copy constructor.

	@param _other - the other pair
	*/
	ZPair(const ZPair<T1, T2>& _other)
		: First(_other.First), Second(_other.Second) { } 

	/*
	Parameterized constructor.

	@param _first - the first value
	@param _second - the second value
	*/	
	ZPair(const T1& _first, const T2& _second)
		: First(_first), Second(_second) { }

	/*	
	operator < override, used to ensure ZPair can be compared using the default ZComparator.
	
	@param _other - the other pair
	@return (bool) - true if this ZPair is less than the other, false otherwise
	*/
	bool operator < (const ZPair<T1, T2>& _other) const
		{ return ZPair_OperatorLessThanImpl<T1, T2>::Call(*this, _other); }

	/*
	operator = override, used to assign the ZPair to another.
	
	@param _other - the other pair
	@return (ZPair<T1, T2>&) - this pair
	*/
	ZPair<T1, T2>& operator = (const ZPair<T1, T2>& _other)
		{ return ZPair_OperatorEqualsAssignImpl<T1, T2>::Call(*this, _other); }

	/*	
	operator == override.
	
	@param _other - the other pair
	@return (bool) - true if this pair is equal to the other, false otherwise
	*/
	bool operator == (const ZPair<T1, T2>& _other) const
		{ return ZPair_OperatorEqualsCompareImpl<T1, T2>::Call(*this, _other); }

	/*
	operator != override.

	@param _other - the other pair
	@return (bool) - true if this pair is not equal to the other, false otherwise
	*/
	bool operator != (const ZPair<T1, T2>& _other) const
		{ return ZPair_OperatorNotEqualImpl<T1, T2>::Call(*this, _other); }

	/*
	public ZPair<T1, T2>::Swap

	Returns another pair that is has swapped the first and second values of this pair.

	@return (ZPair<T2, T1>) - a pair with swapped first/second values
	*/
	ZPair<T2, T1> Swap() const
		{ return ZPair_SwapImpl<T1, T2>::Call(*this); }
};

////////////////////////////////////////////
/* Non-Specialized Method Implementations */
////////////////////////////////////////////

template <typename T1, typename T2>
bool ZPair_OperatorLessThanImpl<T1, T2>::Call( const ZPair<T1, T2>& _self, const ZPair<T1, T2>& _other )
{
	if (_self.First < _other.First)
		return true;

	return (_self.Second < _other.Second);
}

template <typename T1, typename T2>
ZPair<T1, T2>& ZPair_OperatorEqualsAssignImpl<T1, T2>::Call( ZPair<T1, T2>& _self, const ZPair<T1, T2>& _other )
{
	_self.First = _other.First;
	_self.Second = _other.Second;

	return _self;
}

template <typename T1, typename T2>
bool ZPair_OperatorEqualsCompareImpl<T1, T2>::Call( const ZPair<T1, T2>& _self, const ZPair<T1, T2>& _other )
{
	if (_self.First == _other.First && _self.Second == _other.Second)
		return true;

	return false;
}

template <typename T1, typename T2>
bool ZPair_OperatorNotEqualImpl<T1, T2>::Call( const ZPair<T1, T2>& _self, const ZPair<T1, T2>& _other )
{
	return !(_self == _other);
}

template <typename T1, typename T2>
ZPair<T2, T1> ZPair_SwapImpl<T1, T2>::Call( const ZPair<T1, T2>& _self )
{
	return ZPair<T2, T1>(_self.Second, _self.First);
}

#endif
