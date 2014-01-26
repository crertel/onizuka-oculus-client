/*
	ZStringUtil.hpp
	Author: James Russell <jcrussell@762studios.com>
	Created: 1/12/2012

	Purpose: 

	Contains additional methods for manipulation and use of ZBasicString<A>.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _ZBASICSTRINGALGO_HPP
#define _ZBASICSTRINGALGO_HPP

#include <ZSTL/ZSTL.hpp>
#include <ZSTL/ZArrayAlgo.hpp>

#include <string.h>	//for strcmp, memset, memmove
#include <stdio.h>	//for sprintf
#include <stdarg.h>	//for va_start, va_end

namespace ZBasicStringAlgo
{
	//Forward Declaration of FindFirst
	template <typename A>
	size_t FindFirst(const ZBasicString<A>& _string, const char& _char, size_t _start, size_t _end);
	
	template <typename A>
	size_t FindFirst(const ZBasicString<A>& _string, const char& _char);

	//Forward Declaration of FindSub
	template <typename A, typename B> 
	size_t FindSub(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const ZBasicString<B>& _substring, size_t _s2, size_t _e2);

	//Forward Declaration of TrimLeft
	template <typename A>
	size_t TrimLeft(ZBasicString<A>& _string, size_t _start, size_t _end);

	//Forward Declaration of TrimRight
	template <typename A>
	size_t TrimRight(ZBasicString<A>& _string, size_t _start, size_t _end);

	/*
	public ZBasicStringAlgo<A, B>::Append

	Appends a range of a string to the provided one.

	@param A - allocator type for this string
	@param B - the allocator type for the other string
	@param _string - the string to append to
	@param _other - string to append
	@param _start - the starting index of the other string
	@param _end - the ending index of the other string (exclusive)
	@return (void)
	@assert - if _end < _start
			  if _start of _end out of bounds			
	*/
	template <typename A, typename B>
	void Append(ZBasicString<A>& _string, const ZBasicString<B>& _other, size_t _start, size_t _end)
	{
		if (_start == _end)
			return;

		const size_t start = _other.BoundsCheck(_start, _other.Length());
		const size_t end = _other.BoundsCheck(_end, _other.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::Append - Cannot append with end < start!");
		#endif

		ZArray<char, A>& stringArray = const_cast< ZArray<char, A>& >(_string.Array());

		stringArray.PopBack();

		ZArrayAlgo::Append(stringArray, _other.Array(), start, end);

		stringArray.PushBack(ZBASICSTRING_NULL_TERMINATOR);
	}

	/*
	public ZBasicStringAlgo<A, B>::Append

	Appends a string to the provided one.

	@param A - allocator type for this string
	@param B - the allocator type for the other string
	@param _string - the string to append to
	@param _other - string to append
	@return (ZBasicString<A>&) - this provided string, after appending
	*/
	template <typename A, typename B>
	void Append(ZBasicString<A>& _string, const ZBasicString<B>& _other)
	{
		return Append(_string, _other, 0, _other.Length());
	}

	//Append C-String Implementation
	template <typename A>
	void Append(ZBasicString<A>& _string, const char *_other)
		{ return Append(_string, ZBasicString<A>(_other)); }
	
	//Append C-String Implementation
	template <typename A>
	void Append(ZBasicString<A>& _string, const char *_other, size_t _start, size_t _end)
		{ return Append(_string, ZBasicString<A>(_other), _start, _end); }

	/*
	public ZBasicStringAlgo<A>::BuildNumeric

	Builds a string from an integer or floating point type using the given format string, so
	this equivalent to calling snprintf with the provided format string and number.  

	@param A - the allocator type for the string
	@param N - the numeric type to build from (int, double, etc.)
	@param _string - the string to build
	@param _fmt - the format string to use (any format string valid for _number)
	@param _number - the number to use
	@param 
	*/
	template <typename A, typename N>
	void BuildNumeric(ZBasicString<A>& _string, const char* _fmt, N _number)
	{
		ZSTL_ASSERT(_fmt != NULL, "BuildNumeric() given null format string!\n");
		_string.Clear();

		int len = -1;
		#ifdef _MSC_VER
		#pragma warning(push)
		#pragma warning(disable:4996)
		len = _snprintf(NULL,0,_fmt,_number);
		#pragma warning(pop)
		#else
        char temp = ' ';
		len = snprintf(&temp,0,_fmt,_number);
		#endif

		ZSTL_ASSERT(len > 0, "BuildNumeric() would've made bad printf!\n");
		_string.Resize(len, ZBASICSTRING_NULL_TERMINATOR);

		#ifdef _MSC_VER
		#pragma warning(push)
		#pragma warning(disable:4996)
		// note that ZString has one more in the data array, set to the terminator, so this is okay.
		_snprintf(_string.Data(), (_string.Length()) * sizeof(char), _fmt, _number);
		#pragma warning(pop)
		#else
		// for GNU et al, though we need to fake out their null-terminator safety
		snprintf(_string.Data(), (_string.Length() + 1) * sizeof(char), _fmt, _number);
		#endif
	}

	/*
	public ZBasicStringAlgo<A>::BuildNumeric
	
	Builds a string from a double-precision float.  This is equivalent to using
	snprintf with a format string of %f.

	@param A - the allocator type for the string
	@param _string - the string to build
	@param _number - the number to use
	@return (void)
	*/
	template <typename A>
	void BuildNumeric(ZBasicString<A>& _string, double _number)
	{
		return BuildNumeric(_string, "%f", _number);
	}

	/*
	public ZBasicStringAlgo<A>::BuildNumeric

	Builds a string from an integer.  This is equivalent to using
	snprintf with a format string of %i.
	
	@param A - the allocator type of the string
	@param _string - the string to build
	@param _number - the number to use
	@return (void)
	*/
	template <typename A>
	void BuildNumeric(ZBasicString<A>& _string, int _number)
	{
		return BuildNumeric(_string, "%i", _number);
	}

	/*
	public ZBasicStringAlgo<A>::BuildPrintf
	
	Builds a basic string using printf-style formatted output.

	@param A - the allocator type of the string
	@param _string - the string to build (cleared by this function before building)
	@param _fmt - the null-terminated printf format string
	@param  ... - the variable arguments for the format string
	@return (void)
	*/
	template <typename A>
	void BuildPrintf(ZBasicString<A>& _string, const char *_fmt, ...)
	{
		ZSTL_ASSERT(_fmt != NULL, "BuildPrintf() given null format string!\n");
		va_list args;
		va_start(args, _fmt);

		_string.Clear();
		
		//MSVC's implementation of vsnprintf() family contradicts C standard on return value. It returns -1 if too small, when it should return the number of characters
		#if defined(_MSC_VER) || defined(_WIN32)	//TODO: Really, we meant MSVCRTxx.dll, i.e. Microsoft VC runtime
		_string.Resize(128);
		int result;
		do 
		{
			result = _vsnprintf_s(_string.Data(), _string.Length()+1,  _TRUNCATE, _fmt, args);	//NOTE: 2nd param is size of buffer, not number chars. Therefore, we give it +1

			//Not enough space? -> Increase string size
			if(result < 0)				
				_string.Resize( _string.Length() * 2);
			else //Success, trim extra characters
				_string.Resize((size_t)result);

		} while (result < 0);

		#else //Rest of the world can follow the C standard thankfully (right?)
		char temp[1];
		int len = vsnprintf(temp, sizeof(temp), _fmt, args);	//C standard says 2nd param is sizeof(buffer), not number of chars
		//Return is -1 on error, OR number of characters in size

		ZSTL_ASSERT(len >= 0, "BuildPrintf() given bad format string!\n");
		_string.Resize((size_t)len, ZBASICSTRING_NULL_TERMINATOR);
	
		//This will succeed now.
		vsnprintf(_string.Data(), _string.Length()+1, _fmt, args);	//Again, need sizeof(buffer) -> sizeof(buffer) == string.Length()+1
		#endif

		va_end(args);
	}

	/*
	public ZBasicString<>::BuildRepeat
	
	Constructs a ZBasicString<A> by repeating the given null-terminated string the given number of times.

	@param A - the allocator type of the string
	@param _string - the string to build (cleared by this function before building)
	@param _repeatString - the string to repeat
	@param _count - the number of times to repeat it
	@return (void)
	*/
	template <typename A>
	void BuildRepeat(ZBasicString<A>& _string, const char *_repeatString, size_t _count)
	{
		size_t i;
		
		
		size_t len = strlen(_repeatString);

		_string.Resize(len * _count);	//Resize() includes space for NULL terminator


		char* data = _string.Data();
		for (i = 0; i < _count; i++)
		{
			memcpy(data, _repeatString, len);
			data += len;
		}
		*data = '\0';
	}

	/*
	public ZBasicStringAlgo<A>::Contains
	
	Determines if the provided string contains the given character between the given
	indices.

	@param A - allocator type for this string
	@param _string - the string to check
	@param _c - the character to look for 
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (bool) - true if the string contains this character, false otherwise
	@assert - if _end < _start
			  if _start of _end out of bounds			
	*/
	template <typename A>
	bool Contains(const ZBasicString<A>& _string, const char _char, size_t _start, size_t _end) 
	{
		if (_start == _end)
			return false;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::Contains - Cannot check with end < start!");
		#endif

		return ZArrayAlgo::Contains(_string.Array(), _char, start, end);
	}

	/*
	public ZBasicStringAlgo<A>::Contains
	
	Determines if the provided string contains the given character.

	@param A - allocator type for this string
	@param _string - the string to check
	@param _c - the character to look for 
	@return (bool) - true if the string contains this character, false otherwise
	*/
	template <typename A>
	bool Contains(const ZBasicString<A>& _string, const char _char) 
	{
		return Contains(_string, _char, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A, B>::ContainsSub

	Determines if the provided string contains the provided substring in the given ranges.

	@param A - allocator type of the string
	@param B - allocator type for the substring
	@param _string - the string to check
	@param _s1 - the starting index in  the string
	@param _e1 - the ending index in the string (exclusive)
	@param _substring - the substring to look for
	@param _s2 - the starting index in the substring
	@param _e2 - the ending index in the string (exclusive)
	@return (bool) - true if the string contains the substring, false otherwise
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename A, typename B>
	bool ContainsSub(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const ZBasicString<B>& _substring, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return false;

		const size_t s1 = _string.BoundsCheck(_s1, _string.Length());
		const size_t e1 = _string.BoundsCheck(_e1, _string.Length() + 1);

		const size_t s2 = _substring.BoundsCheck(_s2, _substring.Length());
		const size_t e2 = _substring.BoundsCheck(_e2, _substring.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZBasicStringAlgo::ContainsSub - Cannot check with end < start!");
		ZSTL_ASSERT(s2 <= e2, "ZBasicStringAlgo::ContainsSub - Cannot check with end < start!");
		#endif

		return ZArrayAlgo::ContainsSub(_string.Array(), s1, e1, _substring.Array(), s2, e2);	
	}

	/*
	public ZBasicStringAlgo<A, B>::ContainsSub

	Determines if the provided string contains the provided substring.

	@param A - allocator type of the string
	@param B - allocator type for the substring
	@param _string - the string to check
	@param _substring - the substring to look for 
	@return (bool) - true if the string contains the substring, false otherwise
	*/
	template <typename A, typename B>
	bool ContainsSub(const ZBasicString<A>& _string, const ZBasicString<B>& _substring)
	{
		return ContainsSub(_string, 0, _string.Length(), _substring, 0, _substring.Length());
	}

	//ContainsSub C-String Implementation
	template <typename A>
	bool ContainsSub(const ZBasicString<A>& _string, const char* _other)
		{ return ContainsSub(_string, ZBasicString<A>(_other)); }

	//ContainsSub C-String Implementation
	template <typename A>
	bool ContainsSub(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const char* _other, size_t _s2, size_t _e2)
		{ return ContainsSub(_string, _s1, _e1, ZBasicString<A>(_other), _s2, _e2); }

	/*
	public ZBasicStringAlgo<A, B>::Copy

	Copies the values in the range provided from the given string into the range 
	provided in another.  The maximum number of characters copied is equal to the 
	minimum of (_e1 - _s1) or (_e2 - _s2).

	@param A - allocator type for the destination string
	@param B - allocator type for the source string
	@param _destination - the destination string
	@param _s1 - the starting index for the destination string
	@param _e1 - the ending index for the destination string (exclusive)
	@param _source - the source string
	@param _s2 - the starting index for the source string
	@param _e2 - the ending index for the source string (exclusive)
	@return (void)
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename A, typename B>
	void Copy(ZBasicString<A>& _destination, size_t _s1, size_t _e1, const ZBasicString<B>& _source, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return;

		const size_t s1 = _destination.BoundsCheck(_s1, _destination.Length());
		const size_t e1 = _destination.BoundsCheck(_e1, _destination.Length() + 1);

		const size_t s2 = _source.BoundsCheck(_s2, _source.Length());
		const size_t e2 = _source.BoundsCheck(_e2, _source.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZBasicStringAlgo::Copy - Cannot copy with end < start!");
		ZSTL_ASSERT(s2 <= e2, "ZBasicStringAlgo::Copy - Cannot copy with end < start!");
		#endif

		ZArray<char, A>& stringArray = const_cast< ZArray<char, A>& >(_destination.Array());

		ZArrayAlgo::Copy(stringArray, s1, e1, _source.Array(), s2, e2);		
	}

	/*
	public ZBasicStringAlgo<A, B>::Copy

	Copies the characters from a source string into a destination string.  The maximum number of 
	characters copied is equal to the length of the shorter string.

	@param A - allocator type for the destination string
	@param B - allocator type for the source string
	@param _destination - the destination string
	@param _source - the source string
	@return (void)
	*/
	template <typename A, typename B>
	void Copy(ZBasicString<A>& _destination, const ZBasicString<B>& _source)
	{
		Copy(_destination, 0, _destination.Length(), _source, 0, _source.Length());
	}

	//Copy C-String Implementation
	template <typename A>
	void Copy(ZBasicString<A>& _destination, size_t _s1, size_t _e1, const char* _source, size_t _s2, size_t _e2)
		{ Copy(_destination, _s1, _e1, ZBasicString<A>(_source), _s2, _e2); }

	//Copy C-String Implementation
	template <typename A>
	void Copy(ZBasicString<A>& _destination, const char* _source)
		{ Copy(_destination, ZBasicString<A>(_source)); }

	/*
	public ZBasicStringAlgo<A, B>::EndsWith

	Determines if the provided range of the given string ends with the values specified in the provided
	range of another string.

	@param A - allocator type for the string
	@param B - allocator type for the other string
	@param _string - the string to check
	@param _s1 - the starting index for the destination string
	@param _e1 - the ending index for the destination string (exclusive)
	@param _other - the string to compare against
	@param _s2 - the starting index for the source string
	@param _e2 - the ending index for the source string (exclusive)
	@return (bool) - true if the string ends with the provided value, false otherwise
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename A, typename B>
	bool EndsWith(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const ZBasicString<B>& _other, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return false;

		const size_t s1 = _string.BoundsCheck(_s1, _string.Length());
		const size_t e1 = _string.BoundsCheck(_e1, _string.Length() + 1);

		const size_t s2 = _other.BoundsCheck(_s2, _other.Length());
		const size_t e2 = _other.BoundsCheck(_e2, _other.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZBasicStringAlgo::EndsWith - Cannot check with end < start!");
		ZSTL_ASSERT(s2 <= e2, "ZBasicStringAlgo::EndsWith - Cannot check with end < start!");
		#endif

		return ZArrayAlgo::EndsWith(_string.Array(), s1, e1, _other.Array(), s2, e2);	
	}

	/*
	public ZBasicStringAlgo<A, B>::EndsWith

	Determines if the provided string ends with the another string.

	@param A - allocator type for the string
	@param B - allocator type for the other string
	@param _string - the string to check
	@param _s1 - the starting index for the destination string
	@param _e1 - the ending index for the destination string (exclusive)
	@param _other - the string to compare against
	@param _s2 - the starting index for the source string
	@param _e2 - the ending index for the source string (exclusive)
	@return (bool) - true if the string ends with the provided value, false otherwise
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename A, typename B>
	bool EndsWith(const ZBasicString<A>& _string, const ZBasicString<B>& _other)
	{
		return EndsWith(_string, 0, _string.Length(), _other, 0, _other.Length());
	}

	//EndsWith C-String Implementation
	template <typename A>
	bool EndsWith(const ZBasicString<A>& _string, const char* _substring)
		{ return EndsWith(_string, ZBasicString<A>(_substring)); }

	//EndsWith C-String Implementation
	template <typename A>
	bool EndsWith(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const char* _substring, size_t _s2, size_t _e2)
		{ return EndsWith(_string, _s1, _e1, ZBasicString<A>(_substring), _s2, _e2); }

	/*
	public ZBasicStringAlgo<A, B>::Equal

	Determines if the given range of the provided string is equal to the given range
	of another string.  The maximum number of characters compared is equal to the 
	minimum of (_e1 - _s1) or (_e2 - _s2).

	If provided a range of zero on both strings, the answer is always true.

	@param A - allocator type for the string
	@param B - allocator type for the other string
	@param _string - the string to check
	@param _s1 - the starting index for the destination string
	@param _e1 - the ending index for the destination string (exclusive)
	@param _other - the string to compare against
	@param _s2 - the starting index for the source string
	@param _e2 - the ending index for the source string (exclusive)
	@return (bool) - true if the string equals the provided value, false otherwise
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename A, typename B>
	bool Equal(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const ZBasicString<B>& _other, size_t _s2, size_t _e2)
	{
		if (_e1 - _s1 == 0 && _e2 - _s2 == 0) {
			return true;
		}

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(_s1 <= _e1, "ZBasicStringAlgo::Equal - Cannot compare with end < start!");
		ZSTL_ASSERT(_s2 <= _e2, "ZBasicStringAlgo::Equal - Cannot compare with end < start!");
		#endif

		const size_t s1 = _string.BoundsCheck(_s1, _string.Length());
		const size_t e1 = _string.BoundsCheck(_e1, _string.Length() + 1);

		const size_t s2 = _other.BoundsCheck(_s2, _other.Length());
		const size_t e2 = _other.BoundsCheck(_e2, _other.Length() + 1);


		return ZArrayAlgo::Equal(_string.Array(), s1, e1, _other.Array(), s2, e2);
	}

	/*
	public ZBasicStringAlgo<A, B>::Equal

	Determines if the given string is equal to the other string.  The maximum number
	of characters compared is equal to the length of the shorter string.

	@param A - allocator type for the string
	@param B - allocator type for the other string
	@param _string - the string to check
	@param _s1 - the starting index for the destination string
	@param _e1 - the ending index for the destination string (exclusive)
	@param _other - the string to compare against
	@param _s2 - the starting index for the source string
	@param _e2 - the ending index for the source string (exclusive)
	@return (bool) - true if the string equals the provided value, false otherwise
	*/
	template <typename A, typename B>
	bool Equal(const ZBasicString<A>& _string, const ZBasicString<B>& _other)
	{
		return Equal(_string, 0, _string.Length(), _other, 0, _other.Length());
	}

	//Equal C-String Implementation
	template <typename A>
	bool Equal(const ZBasicString<A>& _string, const char* _other)
		{ return Equal(_string, ZBasicString<A>(_other)); }

	//Equal C-String Implementation
	template <typename A>
	bool Equal(const ZBasicString<A>& _string, size_t _s1, const char* _other, size_t _s2, size_t _count)
		{ return Equal(_string, _s1, ZBasicString<A>(_other), _s2, _count); }

	/*
	public ZBasicStringAlgo<A, B>::EqualIgnoreCase

	Determines if the given range of the provided string is equal to the given range
	of another string, ignoring the character case.  The maximum number of characters 
	compared is equal to the minimum of (_e1 - _s1) or (_e2 - _s2).

	If provided a range of zero on both strings, the answer is always true.

	@param A - allocator type for the string
	@param B - allocator type for the other string
	@param _string - the string to check
	@param _s1 - the starting index for the destination string
	@param _e1 - the ending index for the destination string (exclusive)
	@param _other - the string to compare against
	@param _s2 - the starting index for the source string
	@param _e2 - the ending index for the source string (exclusive)
	@return (bool) - true if the string equals the provided value ignoring case, false otherwise
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename A, typename B>
	bool EqualIgnoreCase(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const ZBasicString<B>& _other, size_t _s2, size_t _e2)
	{
		if (_e1 - _s1 == 0 && _e2 - _s2 == 0) {
			return true;
		}

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(_s1 <= _e1, "ZBasicStringAlgo::Equal - Cannot compare with end < start!");
		ZSTL_ASSERT(_s2 <= _e2, "ZBasicStringAlgo::Equal - Cannot compare with end < start!");
		#endif

		const size_t s1 = _string.BoundsCheck(_s1, _string.Length());
		const size_t e1 = _string.BoundsCheck(_e1, _string.Length() + 1);

		const size_t s2 = _other.BoundsCheck(_s2, _other.Length());
		const size_t e2 = _other.BoundsCheck(_e2, _other.Length() + 1);

		size_t i, j;

		for (i = s1, j = s2; i < e1 && j < e2; i++, j++)
		{
			if ( tolower(_string.Data()[i]) != tolower(_other.Data()[j]) )
				return false;
		}

		return true;
	}

	/*
	public ZBasicStringAlgo<A, B>::EqualIgnoreCase

	Determines if the given string is equal to the other string ignoring character
	case.  The maximum number of characters compared is equal to the length of the 
	shorter string.

	@param A - allocator type for the string
	@param B - allocator type for the other string
	@param _string - the string to check
	@param _s1 - the starting index for the destination string
	@param _e1 - the ending index for the destination string (exclusive)
	@param _other - the string to compare against
	@param _s2 - the starting index for the source string
	@param _e2 - the ending index for the source string (exclusive)
	@return (bool) - true if the string equals the provided value ignoring character case, false otherwise
	*/
	template <typename A, typename B>
	bool EqualIgnoreCase(const ZBasicString<A>& _string, const ZBasicString<B>& _other)
	{
		return EqualIgnoreCase(_string, 0, _string.Length(), _other, 0, _other.Length());
	}

	//EqualIgnoreCase C-String Implementation
	template <typename A>
	bool EqualIgnoreCase(const ZBasicString<A>& _string, const char* _other)
		{ return EqualIgnoreCase(_string, ZBasicString<A>(_other)); }

	//EqualIgnoreCase C-String Implementation
	template <typename A>
	bool EqualIgnoreCase(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const char* _other, size_t _s2, size_t _e2)
		{ return EqualIgnoreCase(_string, _s1, _e1, ZBasicString<A>(_other), _s2, _e2); }

	/*
	public ZBasicStringAlgo<A>::Fill
	
	Fills the provided region of the given string with the provided value.
	
	@param A - the allocator type for the string
	@param _string - the string to fill
	@param _value - the value to fill with
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (void)
	@assert - if _end < _start
			  if _start of _end out of bounds			
	*/
	template <typename A>
	void Fill(ZBasicString<A>& _string, const char& _value, size_t _start, size_t _end)
	{
		if (_start == _end) {
			return;
		}

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::Fill - Cannot fill with end < start!");
		#endif

		ZArray<char, A>& stringArray = const_cast< ZArray<char, A>& >(_string.Array());

		ZArrayAlgo::Fill(stringArray, _value, start, end);
	}

	/*
	public ZBasicStringAlgo<A>::Fill
	
	Fills the string full of the provided character value.
	
	@param A - the allocator type for the string
	@param _string - the string to fill
	@param _value - the value to fill with
	@return (void)
	*/
	template <typename A>
	void Fill(ZBasicString<A>& _string, const char& _value)
	{
		Fill(_string, _value, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A>::Find
	
	Finds the Nth occurrence of a character in the provided region of the string.
	
	@param A - the allocator type for the string 
	@param _string - the string to search
	@param _value - the value to find
	@param _count - the number of occurrences which would be skipped (meaning an index to the (N+1)th occurrence is returned)
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the index of the Nth occurrence (ZBasicString::InvalidPos if not found)
	@assert - if _end < _start
			  if _start of _end out of bounds			
	*/
	template <typename A>
	size_t Find(const ZBasicString<A>& _string, const char& _value, size_t _count, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::Find - Cannot find with end < start!");
		#endif

		return ZArrayAlgo::Find(_string.Array(), _value, _count, start, end);
	}

	/*
	public ZBasicStringAlgo<A>::Find
	
	Finds the Nth occurrence of a character in the provided string.
	
	@param A - the allocator type for the string 
	@param _string - the string to search
	@param _value - the value to find
	@param _count - the number of occurrences which would be skipped (meaning an index to the (N+1)th occurrence is returned)
	@return (size_t) - the index of the Nth occurrence (ZBasicString::InvalidPos if not found)
	*/
	template <typename A>
	size_t Find(const ZBasicString<A>&_string, const char& _value, size_t _count)
	{
		return Find(_string, _value, _count, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A>::FindAll
	
	Finds all occurrences of the provided character in the given region in the string.
	
	@param A - the allocator type of the string
	@param _string - the string to search
	@param _char - the character to look for
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (ZArray<size_t>) - array of indices to all occurrences of the character
	@assert - if _end < _start
			  if _start of _end out of bounds			
	*/
	template <typename A>
	ZArray<size_t> FindAll(const ZBasicString<A>& _string, const char& _char, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZArray<size_t>();

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::FindAll - Cannot find with end < start!");
		#endif

		return ZArrayAlgo::FindAll(_string.Array(), _char, start, end);
	}

	/*
	public ZBasicStringAlgo<A>::FindAll
	
	Finds all occurrences of the provided character in the given string.
	
	@param A - the allocator type of the string
	@param _string - the string to search
	@param _char - the character to look for
	@return (ZArray<size_t>) - array of indices to all occurrences of the character
	@assert - if _end < _start
			  if _start of _end out of bounds			
	*/
	template <typename A>
	ZArray<size_t> FindAll(const ZBasicString<A>& _string, const char& _char)
	{
		return FindAll(_string, _char, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A, B>::FindAllOf
	
	Finds all occurrences of any of the given characters in the given region.
	
	@param A - the allocator type of the string
	@param B - the allocator type of the values string
	@param _string - the string to search
	@param _s1 - the starting index in the string
	@param _e1 - the ending index in the string (exclusive)
	@param _values - the values to look for
	@param _s2 - the starting index in the values string
	@param _e2 - the ending index in the values string (exclusive)
	@return (ZArray<size_t>) - array of indices to all occurrences of one of the provided characters
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename A, typename B>
	ZArray<size_t> FindAllOf(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const ZBasicString<B>& _values, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return ZArray<size_t>();

		const size_t s1 = _string.BoundsCheck(_s1, _string.Length());
		const size_t e1 = _string.BoundsCheck(_e1, _string.Length() + 1);

		const size_t s2 = _string.BoundsCheck(_s2, _values.Length());
		const size_t e2 = _string.BoundsCheck(_e2, _values.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZBasicStringAlgo::FindAllOf - Cannot find with end < start!");
		ZSTL_ASSERT(s2 <= e2, "ZBasicStringAlgo::FindAllOf - Cannot find with end < start!");
		#endif

		return ZArrayAlgo::FindAllOf(_string.Array(), s1, e1, _values.Array(), s2, e2);
	}

	/*
	public ZBasicStringAlgo<A, B>::FindAllOf
	
	Finds all occurrences of any of the given characters.
	
	@param A - the allocator type of the string
	@param B - the allocator type of the values string
	@param _string - the string to search
	@param _values - the values to look for
	@return (ZArray<size_t>) - array of indices to all occurrences of one of the provided characters
	*/
	template <typename A, typename B>
	ZArray<size_t> FindAllOf(const ZBasicString<A>& _string, const ZBasicString<B>& _values)
	{
		return FindAllOf(_string, 0, _string.Length(), _values, 0, _values.Length());
	}

	//FindAllOf C-String Implementation
	template <typename A>
	ZArray<size_t> FindAllOf(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const char* _values, size_t _s2, size_t _e2)
	{
		return FindAllOf(_string, _s1, _e1, ZBasicString<A>(_values), _s2, _e2);
	}

	//FindAllOf C-String Implementation
	template <typename A>
	ZArray<size_t> FindAllOf(const ZBasicString<A>& _string, const char* _values)
	{
		return FindAllOf(_string, ZBasicString<A>(_values));
	}

	/*
	public ZBasicStringAlgo<A>::FindFirst
	
	Finds the first occurrence of the provided value in the given region.
	
	@param A - the allocator type for the string
	@param _string - the string to search
	@param _char - the character to search for
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the first occurrence of the given value (ZSTL::InvalidPos if not found)
	@assert - if _end < _start
			  if _start of _end out of bounds			
	*/
	template <typename A>
	size_t FindFirst(const ZBasicString<A>& _string, const char& _char, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::FindFirst - Cannot find with end < start!");
		#endif

		return ZArrayAlgo::FindFirst(_string.Array(), _char, start, end);
	}

	/*
	public ZBasicStringAlgo<A>::FindFirst
	
	Finds the first occurrence of the provided value.
	
	@param A - the allocator type for the string
	@param _string - the string to search
	@param _char - the character to search for
	@return (size_t) - the first occurrence of the given value (ZSTL::InvalidPos if not found)
	*/
	template <typename A>
	size_t FindFirst(const ZBasicString<A>& _string, const char& _char)
	{
		return FindFirst(_string, _char, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A, B>::FindFirstOf
	
	Finds the first occurrence of a set of values in the provided region of a string.
	
	@param A - the allocator type for the string
	@param B - the allocator type for the values string
	@param _string - the string to search in
	@param _s1 - the starting index in the string
	@param _e1 - the ending index in the string (exclusive)
	@param _delimiters - the delimiters to search for
	@param _s2 - the starting index in the delimiters string
	@param _e2 - the ending index in the delimiters string (exclusive)
	@return (size_t) - the index of the first occurrence of one of the delimiters (ZBasicString::InvalidPos if not found)
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename A, typename B>
	size_t FindFirstOf(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const ZBasicString<B>& _delimiters, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return ZSTL::InvalidPos;

		const size_t s1 = _string.BoundsCheck(_s1, _string.Length());
		const size_t e1 = _string.BoundsCheck(_e1, _string.Length() + 1);

		const size_t s2 = _string.BoundsCheck(_s2, _delimiters.Length());
		const size_t e2 = _string.BoundsCheck(_e2, _delimiters.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZBasicStringAlgo::FindFirstOf - Cannot find with end < start!");
		ZSTL_ASSERT(s2 <= e2, "ZBasicStringAlgo::FindFirstOf - Cannot find with end < start!");
		#endif

		return ZArrayAlgo::FindFirstOf(_string.Array(), s1, e1, _delimiters.Array(), s2, e2);
	}

	//FindFirstOf C-String Implementation
	template <typename A, typename B>
	size_t FindFirstOf(const ZBasicString<A>& _string, const ZBasicString<B>& _values, size_t _start, size_t _end)
	{ return FindFirstOf(_string, 0, _string.Length(), ZBasicString<A>(_values), _start, _end); }

	/*
	public ZBasicStringAlgo<A, B>::FindFirstOf
	
	Finds the first occurrence of a set of values in the provided region
	
	@param A - the allocator type for the string
	@param B - the allocator type for the values string
	@param _string - the string to search in
	@param _delimiters - the decimeters to search for
	@return (size_t) - the index of the first occurrence of one of the delimiters (ZBasicString::InvalidPos if not found)
	*/
	template <typename A, typename B>
	size_t FindFirstOf(const ZBasicString<A>& _string, const ZBasicString<B>& _delimiters)
	{
		return FindFirstOf(_string, 0, _string.Length(), _delimiters, 0, _delimiters.Length());
	}

	//FindFirstOf C-String Implementation
	template <typename A>
	size_t FindFirstOf(const ZBasicString<A>& _string, const char* _delimiters)
		{ return FindFirstOf(_string, ZBasicString<A>(_delimiters)); }

	//FindFirstOf C-String Implementation
	template <typename A>
	size_t FindFirstOf(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const char* _delimiters, size_t _s2, size_t _e2)
		{ return FindFirstOf(_string, _s1, _e1, ZBasicString<A>(_delimiters), _s2, _e2); }

	/*
	public ZBasicStringAlgo<A>::FindFirstNot
	
	Finds the first occurrence of a value that is not the provided value in the given
	region of the string.
	
	@param A - the allocator type of the string
	@param _string - the string to search
	@param _char - the character to avoid
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the index of the first value that is not the provided value (ZBasicString::InvalidPos if not found)
	@assert - if _end < _start
			  if _start of _end out of bounds			
	*/
	template <typename A>
	size_t FindFirstNot(const ZBasicString<A>& _string, const char& _char, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);
		
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::FindFirstNot - Cannot find with end < start!");
		#endif

		return ZArrayAlgo::FindFirstNot(_string.Array(), _char, start, end);
	}

	/*
	public ZBasicStringAlgo<A>::FindFirstNot
	
	Finds the first occurrence of a value that is not the provided value in the given
	region of the string.
	
	@param A - the allocator type of the string
	@param _string - the string to search
	@param _char - the character to avoid
	@return (size_t) - the index of the first value that is not the provided value (ZBasicString::InvalidPos if not found)
	*/
	template <typename A>
	size_t FindFirstNot(const ZBasicString<A>& _string, const char& _char)
	{
		return FindFirstNot(_string, _char, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A, B>::FindFirstNotOf
	
	Finds the first occurrence of a value that is not one of a set of values 
	in the provided region of a string.
	
	@param A - the allocator type for the string
	@param B - the allocator type for the values string
	@param _string - the string to search in
	@param _s1 - the starting index in the string
	@param _e1 - the ending index in the string (exclusive)
	@param _delimiters - the delimiters to avoid
	@param _s2 - the starting index in the delimiters string
	@param _e2 - the ending index in the delimiters string (exclusive)
	@return (size_t) - the index of the first occurrence of a value that is not one of the delimiters (ZBasicString::InvalidPos if not found)
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename A, typename B>
	size_t FindFirstNotOf(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const ZBasicString<B>& _delimiters, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return ZSTL::InvalidPos;

		const size_t s1 = _string.BoundsCheck(_s1, _string.Length());
		const size_t e1 = _string.BoundsCheck(_e1, _string.Length() + 1);

		const size_t s2 = _string.BoundsCheck(_s2, _delimiters.Length());
		const size_t e2 = _string.BoundsCheck(_e2, _delimiters.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZBasicStringAlgo::FindFirstNotOf - Cannot find with end < start!");
		ZSTL_ASSERT(s2 <= e2, "ZBasicStringAlgo::FindFirstNotOf - Cannot find with end < start!");
		#endif

		return ZArrayAlgo::FindFirstNotOf(_string.Array(), s1, e1, _delimiters.Array(), s2, e2);
	}

	/*
	public ZBasicStringAlgo<A, B>::FindFirstNotOf
	
	Finds the first occurrence of a value that is not one of a set of values 
	in the provided region of a string.
	
	@param A - the allocator type for the string
	@param B - the allocator type for the values string
	@param _string - the string to search in
	@param _delimiters - the delimiters to avoid
	@return (size_t) - the index of the first occurrence of a value that is not one of the delimiters (ZBasicString::InvalidPos if not found)
	*/
	template <typename A, typename B>
	size_t FindFirstNotOf(const ZBasicString<A>& _string, const ZBasicString<B>& _delimiters)
	{
		return FindFirstNotOf(_string, 0, _string.Length(), _delimiters, 0, _delimiters.Length());
	}

	//FindFirstNotOf C-String Implementation
	template <typename A>
	size_t FindFirstNotOf(const ZBasicString<A>& _string, const char* _delimiters)
		{ return FindFirstNotOf(_string, ZBasicString<A>(_delimiters)); }

	//FindFirstNotOf C-String Implementation
	template <typename A>
	size_t FindFirstNotOf(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const char* _delimiters, size_t _s2, size_t _e2)
		{ return FindFirstNotOf(_string, _s1, _e1, ZBasicString<A>(_delimiters), _s2, _e2); }

	/*
	public ZBasicStringAlgo<A>::FindLast
	
	Finds the last occurrence of the specified value in the given range of the string.

	@param A - the allocator type of the string	
	@param _string - the string to search
	@param _char - the value to find
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the index of hte last occurence of the specified value (ZBasicString::InvalidPos if not found)
	@assert - if _end < _start
			  if _start of _end out of bounds			
	*/
	template <typename A>
	size_t FindLast(const ZBasicString<A>& _string, const char& _char, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::FindLast - Cannot find with end < start!");
		#endif

		return ZArrayAlgo::FindLast(_string.Array(), _char, start, end);
	}
	
	/*
	public ZBasicStringAlgo<A>::FindLast
	
	Finds the last occurrence of the specified value in the string.
	
	@param A - the allocator type for the string
	@param _string - the string to search
	@param _char - the value to find
	@return (size_t) - index of the last occurrence (ZBasicString::InvalidPos if not found)
	*/
	template <typename A>
	size_t FindLast(const ZBasicString<A>& _string, const char& _char)
	{
		return FindLast(_string, _char, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A, B>::FindLastOf
	
	Finds the last occurrence of a set of values in the provided region of a string.
	
	@param A - the allocator type for the string
	@param B - the allocator type for the values string
	@param _string - the string to search in
	@param _s1 - the starting index in the string
	@param _e1 - the ending index in the string (exclusive)
	@param _delimiters - the delimiters to search for
	@param _s2 - the starting index in the delimiters string
	@param _e2 - the ending index in the delimiters string (exclusive)
	@return (size_t) - the index of the last occurrence of one of the delimiters (ZBasicString::InvalidPos if not found)
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename A, typename B>
	size_t FindLastOf(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const ZBasicString<B>& _values, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return ZSTL::InvalidPos;

		const size_t s1 = _string.BoundsCheck(_s1, _string.Length());
		const size_t e1 = _string.BoundsCheck(_e1, _string.Length() + 1);	// 1 to allow indexing of end

		const size_t s2 = _values.BoundsCheck(_s2, _values.Length());
		const size_t e2 = _values.BoundsCheck(_e2, _values.Length() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZBasicStringAlgo::FindLastOf - Cannot find with e1 < s1!");
		ZSTL_ASSERT(s2 <= e2, "ZBasicStringAlgo::FindLasttOf - Cannot find with e2 < s2!");
		#endif

		return ZArrayAlgo::FindLastOf(_string.Array(), s1, e1, _values.Array(), s2, e2);
	}

	/*
	public ZBasicStringAlgo<A, B>::FindLastOf
	
	Finds the last occurrence of a set of values in the provided string.
	
	@param A - the allocator type for the string
	@param B - the allocator type for the values string
	@param _string - the string to search in
	@param _delimiters - the delimiters to search for
	@return (size_t) - the index of the last occurrence of one of the delimiters (ZBasicString::InvalidPos if not found)
	*/
	template <typename A, typename B>
	size_t FindLastOf(const ZBasicString<A>& _string, const ZBasicString<B>& _values)
	{
		return FindLastOf(_string, 0, _string.Length(), _values, 0, _values.Length());
	}

	//FindLastOf C-String Implementation
	template <typename A>
	size_t FindLastOf(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const char* _values, size_t _s2, size_t _e2)
	{
		return FindLastOf(_string, _s1, _e1, ZBasicString<A>(_values), _s2, _e2);
	}

	//FindLastOf C-String Implementation
	template <typename A>
	size_t FindLastOf(const ZBasicString<A>& _string, const char* _values)
	{
		return FindLastOf(_string, ZBasicString<A>(_values));
	}

	/*
	public ZBasicStringAlgo<A>::FindLastNot
	
	Finds the last occurrence of a value that is not the provided value in the given
	region of the string.
	
	@param A - the allocator type of the string
	@param _string - the string to search
	@param _char - the character to avoid
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the index of the last value that is not the provided value (ZBasicString::InvalidPos if not found)
	@assert - if _end < _start
			  if _start of _end out of bounds			
	*/
	template <typename A>
	size_t FindLastNot(const ZBasicString<A>& _string, const char& _char, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::FindLastNot - Cannot find with end < start!");
		#endif

		return ZArrayAlgo::FindLastNot(_string.Array(), _char, start, end);
	}

	/*
	public ZBasicStringAlgo<A>::FindLastNot
	
	Finds the last occurrence of a value that is not the provided value in the given string.
	
	@param A - the allocator type of the string
	@param _string - the string to search
	@param _char - the character to avoid
	@return (size_t) - the index of the last value that is not the provided value (ZBasicString::InvalidPos if not found)
	*/
	template <typename A>
	size_t FindLastNot(const ZBasicString<A>& _string, const char& _char)
	{
		return FindLastNot(_string, _char, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A, B>::FindLastNotOf
	
	Finds the last occurrence of a value that is not one of a set of values 
	in the provided region of a string.
	
	@param A - the allocator type for the string
	@param B - the allocator type for the values string
	@param _string - the string to search in
	@param _s1 - the starting index in the string
	@param _e1 - the ending index in the string (exclusive)
	@param _delimiters - the delimiters to avoid
	@param _s2 - the starting index in the delimiters string
	@param _e2 - the ending index in the delimiters string (exclusive)
	@return (size_t) - the index of the last occurrence of a value that is not one of the delimiters (ZBasicString::InvalidPos if not found)
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename A, typename B>
	size_t FindLastNotOf(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const ZBasicString<B>& _values, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return ZSTL::InvalidPos;

		const size_t s1 = _string.BoundsCheck(_s1, _string.Length());
		const size_t e1 = _string.BoundsCheck(_e1, _string.Length() + 1);	// 1 to allow indexing of end

		const size_t s2 = _values.BoundsCheck(_s2, _values.Length());
		const size_t e2 = _values.BoundsCheck(_e2, _values.Length() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZBasicStringAlgo::FindLastNotOf - Cannot find with e1 < s1!");
		ZSTL_ASSERT(s2 <= e2, "ZBasicStringAlgo::FindLastNotOf - Cannot find with e2 < s2!");
		#endif

		return ZArrayAlgo::FindLastNotOf(_string.Array(), s1, e1, _values.Array(), s2, e2);
	}

	/*
	public ZBasicStringAlgo<A, B>::FindLastNotOf
	
	Finds the last occurrence of a value that is not one of a set of values 
	in the provided region of a string.
	
	@param A - the allocator type for the string
	@param B - the allocator type for the values string
	@param _string - the string to search in
	@param _delimiters - the delimiters to avoid
	@return (size_t) - the index of the last occurrence of a value that is not one of the delimiters (ZBasicString::InvalidPos if not found)
	*/
	template <typename A, typename B>
	size_t FindLastNotOf(const ZBasicString<A>& _string, const ZBasicString<B>& _values)
	{
		return FindLastNotOf(_string, 0, _string.Length(), _values, 0, _values.Length());
	}

	//FindLastNotOf C-String Implementation
	template <typename A>
	size_t FindLastNotOf(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const char* _values, size_t _s2, size_t _e2)
	{
		return FindLastNotOf(_string, _s1, _e1, ZBasicString<A>(_values), _s2, _e2);
	}

	//FindLastNotOf C-String Implementation
	template <typename A>
	size_t FindLastNotOf(const ZBasicString<A>& _string, const char* _values)
	{
		return FindLastNotOf(_string, ZBasicString<A>(_values));
	}

	/*
	public ZBasicStringAlgo<A, B>::FindSub
	
	Finds the first occurrence of a substring in the provided region in the string.
	
	@param A - the allocator type for the string
	@param B - the allocator type for the substring
	@param _string - the string to search in
	@param _s1 - the starting index
	@param _e1 - the ending index (exclusive)
	@param _substring - the substring to search for
	@param _s2 - the starting index in the substring
	@param _e2 - the ending index in the substring
	@return (size_t) - the index of the first occurrence of the substring (ZBasicString::InvalidPos if not found)
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename A, typename B>
	size_t FindSub(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const ZBasicString<B>& _substring, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return ZSTL::InvalidPos;

		const size_t s1 = _string.BoundsCheck(_s1, _string.Length());
		const size_t e1 = _string.BoundsCheck(_e1, _string.Length() + 1);

		const size_t s2 = _string.BoundsCheck(_s2, _substring.Length());
		const size_t e2 = _string.BoundsCheck(_e2, _substring.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZBasicStringAlgo::FindSub - Cannot find with end < start!");
		ZSTL_ASSERT(s2 <= e2, "ZBasicStringAlgo::FindSub - Cannot find with end < start!");
		#endif

		return ZArrayAlgo::FindSub(_string.Array(), s1, e1, _substring.Array(), s2, e2);
	}

	/*
	public ZBasicStringAlgo<A, B>::FindSub
	
	Finds the first occurrence of a substring in the provided string.
	
	@param A - the allocator type for the string
	@param B - the allocator type for the substring
	@param _string - the string to search in
	@param _substring - the substring to search for
	@return (size_t) - the index of the first occurrence of the substring (ZBasicString::InvalidPos if not found)
	*/
	template <typename A, typename B>
	size_t FindSub(const ZBasicString<A>& _string, const ZBasicString<B>& _substring)
	{
		return FindSub(_string, 0, _string.Length(), _substring, 0, _substring.Length());
	}

	//FindSub C-String Implementation
	template <typename A>
	size_t FindSub(const ZBasicString<A>& _string, const char* _substring)
		{ return FindSub(_string, ZBasicString<A>(_substring)); }

	//FindSub C-String Implementation
	template <typename A>
	size_t FindSub(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const char* _substring, size_t _s2, size_t _e2)
		{ return FindSub(_string, _s1, _e1, ZBasicString<A>(_substring), _s2, _e2); }

	/*
	public ZBasicString<A>::IsNumeric

	Determines if a range of this string represents a numeric quantity.

	@param A - the allocator type for the string
	@param _string - the string to check
	@param _start - the starting point
	@param _end - the ending point (exclusive)
	@return (bool) - true if this string is numeric, false otherwise
	@assert - if _end < _start
			  if _start of _end out of bounds			
	*/
	template <typename A>
	bool IsNumeric(const ZBasicString<A>& _string, size_t _start, size_t _end)
	{
		if (_start == _end)
			return false;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		bool digitSeen = false;
		bool decimalPointSeen = false;

		const char* string  = _string.Array().Data();

		//Ensure we see [-]?[0-9]+[.]?[0-9]*
		// *  := 0 or more times
		// +  := 1 or more times
		// ?  := 0 or 1 times

		//May begin with '-' or digit.
		if(string[start] != '-')
		{
			if(!isdigit(string[start]))
				return false;

			digitSeen = true;
		}

		/*
		Algorithm: Since we know we have either seen a digit or negative sign,
		scan for more digits and an optional decimal point, allowing only
		a single decimal point. If we terminate the loop before seeing a single
		digit, and we didn't see one before checking for the negative sign, then
		return false.
		*/
		for(size_t i = start+ 1; i < end; i++)
		{
			//Not a digit?
			if(!isdigit(string[i]))
			{
				//Only allow 1 decimal point
				if(!decimalPointSeen && string[i] == '.')
				{
					decimalPointSeen = true;
					continue;
				}

				//Not numeric
				return false;
			}
			else
				digitSeen = true;
		}

		//Must have seen at least one digit for this to be considered numeric.
		return digitSeen;
	}

	/*
	public ZBasicStringAlgo<A>::IsNumeric

	Determines if this string represents a numeric quantity.

	@param A - the allocator type for the string
	@param _string - the string to check
	@return (bool) - true if this string is numeric, false otherwise
	*/
	template <typename A>
	bool IsNumeric(const ZBasicString<A>& _string)
	{
		return IsNumeric(_string, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A>::IsNumeric

	Returns the numeric integer value of this string.  Empty strings are treated as
	zero.

	@param A - the allocator type for the string
	@param _string - the string to convert to an integer value
	@param _start - the starting index for the string
	@param _end - the ending index for the string
	@return (int) - the integer value of the string
	*/
	template <typename A>
	int NumericInt(const ZBasicString<A>& _string, size_t _start, size_t _end)
	{
		if (_start == _end) {
			return 0;
		}

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		char tmp = _string.Data()[end];

		_string.Data()[end] = ZBASICSTRING_NULL_TERMINATOR; //Don't worry, we'll put it back

		int val = atoi(_string.Data() + start);

		_string.Data()[end] = tmp;

		return val;
	}

	/*

	*/
	template <typename A>
	int NumericInt(const ZBasicString<A>& _string)
	{
		return NumericInt(_string, 0, _string.Length());
	}

	/*

	*/
	template <typename A>
	double NumericFloat(const ZBasicString<A>& _string, size_t _start, size_t _end)
	{
		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		char tmp = _string.Data()[end];

		_string.Data()[end] = ZBASICSTRING_NULL_TERMINATOR; //Don't worry, we'll put it back

		double val = atof(_string.Data() + start);

		_string.Data()[end] = tmp;

		return val;
	}

	/*

	*/
	template <typename A>
	double NumericFloat(const ZBasicString<A>& _string)
	{
		return NumericFloat(_string, 0, _string.Length());
	}
	
	/*
	public ZBasicStringAlgo<A>::Remove
	
	Removes the first occurrence of a provided character from the given region in this string.
	
	@param A - the allocator type for the string
	@param _string - the string to remove characters from
	@param _char - the character to remove
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the index of the first removed character (ZBasicString::InvalidPos if none removed)
	@assert - if _end < _start
			  if _start of _end out of bounds			
	*/
	template <typename A>
	size_t Remove(ZBasicString<A>& _string, const char& _char, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::Remove - Cannot remove with end < start!");
		#endif

		ZArray<char, A>& stringArray = const_cast< ZArray<char, A>& >(_string.Array());

		return ZArrayAlgo::Remove(stringArray, _char, start, end);
	}

	/*
	public ZBasicStringAlgo<A>::Remove
	
	Removes the first occurrence of a provided character from this string.
	
	@param A - the allocator type for the string
	@param _string - the string to remove characters from
	@param _char - the character to remove
	@return (size_t) the index of the first removed character (ZBasicString::InvalidPos if none removed)
	*/
	template <typename A>
	size_t Remove(ZBasicString<A>& _string, const char& _char)
	{
		return Remove(_string, _char, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A>::RemoveAll
	
	Removes all occurrences of a provided character from this string in the given region.
	
	@param A - the allocator type for the string
	@param _string - the string to remove characters from
	@param _char - the character to remove
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the number of characters removed
	@assert - if _end < _start
			  if _start of _end out of bounds			
	*/
	template <typename A>
	size_t RemoveAll(ZBasicString<A>& _string, const char& _char, size_t _start, size_t _end)
	{
		if (_start == _end)
			return 0;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::RemoveAll - Cannot remove with end < start!");
		#endif

		ZArray<char, A>& stringArray = const_cast< ZArray<char, A>& >(_string.Array());

		return ZArrayAlgo::RemoveAll(stringArray, _char, start, end);
	}


	/*
	public ZBasicStringAlgo<A>::RemoveAll
	
	Removes all occurrences of a provided character from this string.
	
	@param A - the allocator type for the string
	@param _string - the string to remove characters from
	@param _char - the character to remove
	@return (size_t) - the number of characters removed
	*/
	template <typename A>
	size_t RemoveAll(ZBasicString<A>& _string, const char& _char)
	{
		return RemoveAll(_string, _char, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A>::RemoveUpTo
	
	Removes up to the provided number of occurrences of a character from the string in
	the specified range.

	@param A - the allocator type of the string
	@param _string - the string to remove elements from
	@param _char - the char to look for
	@param _count - the maximum number of times to remove the char
	@param _start - the index to start at
	@param _end - the ending index (exclusive)
	@return (size_t) - the number of occurrences removed
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename A>
	size_t RemoveUpTo(ZBasicString<A>& _string, const char& _char, size_t _count, size_t _start, size_t _end)
	{
		if (_start == _end || _count == 0)
			return 0;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::RemoveUpTo - Cannot remove with end < start!");
		#endif

		ZArray<char, A>& stringArray = const_cast< ZArray<char, A>& >(_string.Array());

		return ZArrayAlgo::RemoveUpTo(stringArray, _char, _count, start, end);
	}
	
	/*
	public ZBasicStringAlgo<A>::RemoveUpTo
	
	Removes up to the provided number of occurrences of a character from the string.

	@param A - the allocator type of the string
	@param _string - the _string to remove elements from
	@param _char - the character to look for
	@param _count - the maximum number of times to remove the char
	@return (size_t) - the number of occurrences removed
	*/
	template <typename A>
	size_t RemoveUpTo(ZBasicString<A>& _string, const char& _char, size_t _count)
	{
		return RemoveUpTo(_string, _char, _count, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A>::Replace
	
	Finds and replaces all occurrences of the provided character with another in the given region.

	@param A - the allocator type of the string
	@param _string - the string to replace chars in
	@param _char - the char to look for
	@param _newValue - the char to replace with
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the number of chars replaced
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename A>
	size_t Replace(ZBasicString<A>& _string, const char& _char, const char& _newChar, size_t _start, size_t _end)
	{
		if (_start == _end)
			return 0;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::Replace - Cannot replace with end < start!");
		#endif

		ZArray<char, A>& stringArray = const_cast< ZArray<char, A>& >(_string.Array());

		return ZArrayAlgo::Replace(stringArray, _char, _newChar, start, end);
	}

	/*
	public ZBasicStringAlgo<A>::Replace
	
	Finds and replaces all occurrences of the provided character with another.

	@param T - the type held by the string
	@param A - the allocator type of the string
	@param _string - the string to replace chars in
	@param _char - the char to look for
	@param _newValue - the char to replace with
	@return (size_t) - the number of chars replaced
	*/
	template <typename A>
	size_t Replace(ZBasicString<A>& _string, const char& _char, const char& _newChar)
	{
		return Replace(_string, _char, _newChar, 0, _string.Length());		
	}

	/*
	public ZBasicStringAlgo<A>::Reverse
	
	Reverses an string in place between the given indices.

	@param A - the allocator type of the string
	@param _string - the string to reverse
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (void)
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename A>
	void Reverse(ZBasicString<A>& _string, size_t _start, size_t _end)
	{
		if (_start == _end)
			return;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::Reverse - Cannot reverse with end < start!");
		#endif

		ZArray<char, A>& stringArray = const_cast< ZArray<char, A>& >(_string.Array());

		ZArrayAlgo::Reverse(stringArray, start, end);
	}

	/*
	public ZBasicStringAlgo<A>::Reverse
	
	Reverses an string in place.

	@param A - the allocator type of the string
	@param _string - the string to reverse
	@return (void)
	*/
	template <typename A>
	void Reverse(ZBasicString<A>& _string)
	{
		Reverse(_string, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A>::Slice
	
	Removes and returns a given region of a string as a new string.
	
	@param A - the allocator type of the string
	@param _string - the string to slice
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (ZBasicString<A>) - the removed region
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename A>
	ZBasicString<A> Slice(ZBasicString<A>& _string, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZBasicString<A>();

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::Slice - Cannot slice with end < start!");
		#endif

		ZArray<char, A>& stringArray = const_cast<ZArray<char, A>&>(_string.Array());

		ZArray<char, A> slice = ZArrayAlgo::Slice(stringArray, start, end);
		slice.PushBack('\0');
		return ZBasicString<A>(slice);
	}

	/*
	public ZBasicStringAlgo<A, B>::Split
	
	Splits the provided range of the given string into a set of strings anywhere a 
	delimiter occurs up to a maximum number of times. This will omit empty strings,
	such as those caused by having two or more delimiters adjacent.
	
	@param A - the allocator type of the string
	@param B - the allocator type for the delimiter string
	@param _string - the string to split
	@param _delimiters - the delimiters to split on
	@param _count - the maximum number of times to split
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (ZArray< ZBasicString<A> >) - array of strings
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename A, typename B>
	ZArray< ZBasicString<A> > Split(const ZBasicString<A>& _string, const ZBasicString<B>& _delimiters, size_t _count, size_t _start, size_t _end)
	{
		if (_start == _end || _count == 0)
			return ZArray< ZBasicString<A> >();

		ZArray< ZBasicString<A> > sections(_string.Length() + 1);

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::Split - Cannot split with end < start!");
		#endif

		size_t previous = start;

		for (size_t i = start; i < end ; i++)
		{
			for (size_t j = 0; j < _delimiters.Length(); j++)
			{
				if ( _string.Data()[i] == _delimiters.Data()[j] )
				{
					if (i  > previous)	//If we have more than just one of the delimiters
					{
						ZBasicString<A> section = ZBasicString<A>(_string, previous, i);
						sections.PushBack( section );
					}

					if (sections.Size() == _count )	//Break out if we have reached our count limit
					{
						previous = i + 1;
						goto breakOut; // we need to leave at this point
						break;
					}
					previous = i + 1;
					break;
				}
			}
		}

breakOut: // breakout from hitting our split count
		if (previous != end)	//Add the final section if we don't end on a delimiter
			sections.PushBack( ZBasicString<A>(_string, previous, end) );

		return sections;
	}

	/*
	public ZBasicStringAlgo<A, B>::Split

	Splits the given string into a set of strings anywhere a delimiter occurs up 
	to a maximum number of times. This will omit empty strings, such as those
	caused by having two or more delimiters adjacent.
	
	@param A - the allocator type of the string
	@param B - the allocator type for the delimiter string
	@param _string - the string to split
	@param _delimiters - the delimiters to split on
	@param _count - the maximum number of times to split
	@return (ZArray< ZBasicString<A> >) - array of strings
	*/
	template <typename A, typename B>
	ZArray< ZBasicString<A> > Split(const ZBasicString<A>& _string, const ZBasicString<B>& _delims, size_t _count)
	{
		return Split(_string, _delims, _count, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A, B>::Split

	Splits the given string into a set of strings anywhere a delimiter occurs.
	This will omit empty strings, such as those caused by having two or more
	delimiters adjacent.

	@param A - the allocator type of the string
	@param B - the allocator type for the delimiter string
	@param _string - the string to split
	@param _delimiters - the delimiters to split on
	@param _count - the maximum number of times to split
	@return (ZArray< ZBasicString<A> >) - array of strings
	*/
	template <typename A, typename B>
	ZArray< ZBasicString<A> > Split(const ZBasicString<A>& _string, const ZBasicString<B>& _delims)
	{
		return Split(_string, _delims, _string.Length(), 0, _string.Length());
	}

	//Split C-String Implementation
	template <typename A>
	ZArray< ZBasicString<A> > Split(const ZBasicString<A>& _string, const char* _delims)
		{ return Split(_string, ZBasicString<A>(_delims)); }

	//Split C-String Implementation
	template <typename A>
	ZArray< ZBasicString<A> > Split(const ZBasicString<A>& _string, const char* _delims, size_t _count)
		{ return Split(_string, ZBasicString<A>(_delims), _count); }

	//Split C-String Implementation
	template <typename A>
	ZArray< ZBasicString<A> > Split(const ZBasicString<A>& _string, const char* _delims, size_t _count, size_t _start, size_t _end)
		{ return Split(_string, ZBasicString<A>(_delims), _count, _start, _end); }
	
	/*
	public ZBasicStringAlgo<A, B>::StartsWith
	
	Determines if the provided region of a string starts with the given region of a substring.
	
	@param A - the allocator type for the string
	@param B - the allocator type for the substring
	@param _string - the string to check
	@param _s1 - the starting index for the string
	@param _e1 - the ending index for the string (exclusive)
	@param _substring - the substring to check
	@param _s2 - the starting index for the substring
	@param _e2 - the ending index for the substring (exclusive)
	@return (bool) - true if the string starts with the given substring, false otherwise
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename A, typename B>
	bool StartsWith(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const ZBasicString<B>& _substring, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return false;

		const size_t s1 = _string.BoundsCheck(_s1, _string.Length());
		const size_t e1 = _string.BoundsCheck(_e1, _string.Length() + 1);

		const size_t s2 = _string.BoundsCheck(_s2, _substring.Length());
		const size_t e2 = _string.BoundsCheck(_e2, _substring.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZBasicStringAlgo::StartsWith - Cannot check with end < start!");
		ZSTL_ASSERT(s2 <= e2, "ZBasicStringAlgo::StartsWith - Cannot check with end < start!");
		#endif

		// easy check: can substring fit into string?
		if (e1 - s1 < e2 - s2)
			return false;

		return ZArrayAlgo::StartsWith(_string.Array(), s1, e1, _substring.Array(), s2, e2);
	}
	
	/*
	public ZBasicStringAlgo<A, B>::StartsWith
	
	Determines if the provided string starts with the given substring.
	
	@param A - the allocator type for the string
	@param B - the allocator type for the substring
	@param _string - the string to check
	@param _substring - the substring to check
	@return (bool) - true if the string starts with the given substring, false otherwise
	*/
	template <typename A, typename B>
	bool StartsWith(const ZBasicString<A>& _string, const ZBasicString<B>& _substring)
	{ 
		return StartsWith(_string, 0, _string.Length(), _substring, 0, _substring.Length());
	}

	//StartsWith C-String Implementation
	template <typename A>
	bool StartsWith(const ZBasicString<A>& _string, const char *_substring)
		{ return StartsWith(_string, ZBasicString<A>(_substring)); }

	//StartsWith C-String Implementation
	template <typename A>
	bool StartsWith(const ZBasicString<A>& _string, size_t _s1, size_t _e1, const char* _substring, size_t _s2, size_t _e2)
		{ return StartsWith(_string, _s1, _e1, ZBasicString<A>(_substring), _s2, _e2); }

	/*
	public ZBasicStringAlgo<A>::Strip
	
	Removes all whitespace from the provided region of a string.

	@param A - the allocator type of the string
	@param _string - the string to strip
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the number of whitespace characters removed
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename A>
	size_t Strip(ZBasicString<A>& _string, size_t _start, size_t _end)
	{
		if (_start == _end)
			return 0;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::Strip - Cannot strip whitepsace with end < start!");
		#endif

		ZArray<char, A>& stringArray = const_cast< ZArray<char, A>& >(_string.Array());

		size_t removed = 0;

		// remove spaces
		removed += ZArrayAlgo::RemoveAll(stringArray, ' ', start, end);

		// remove tabs (remember that you have to decrement the end counter to stay in bounds)
		removed += ZArrayAlgo::RemoveAll(stringArray, '\t', start, end - removed);

		return removed;
	}

	/*
	public ZBasicStringAlgo<A>::Strip
	
	Removes all whitespace from the provided string.

	@param A - the allocator type of the string
	@param _string - the string to strip
	@return (size_t) - the number of whitespace characters removed
	*/
	template <typename A>
	size_t Strip(ZBasicString<A>& _string)
	{

		return Strip(_string, 0, _string.Length());
	}

	/*
	public ZBasicString<A>::Tokenize

	Tokenize function, which returns the next token in this string up to 
	the delimiter, consuming the occurrence of the delimiter in the process.
	This may emit empty strings, such as those caused by having two or
	more delimiters adjacent.

	@param A - the allocator type of the string
	@param _delims - the delimiters to look for
	@return (ZBasicString<A>) - the token
	*/
	template <typename A, typename B>
	ZBasicString<A> Tokenize(ZBasicString<A>& _string, const ZBasicString<B>& _delims)
	{
		if (_string.Empty())
			return ZBasicString<A>();

		if (_delims.Empty())
		{
			ZBasicString<A> ret = _string;
			_string.Clear();
			return ret;
		}

		size_t tokend = ZSTL::InvalidPos;
		size_t i, j;
		ZBasicString<A> token;

		for(i = 0; i < _string.Length(); i++)
		{
			for (j = 0; j < _delims.Length(); j++)
			{
				if (_string.At(i) == _delims.At(j))
				{
					tokend = i;
					i = _string.Length() - 1; // forces us to break out of the outer loop
					break;
				}
			}
		}

		// we found no delimiters :(
		if (tokend == ZSTL::InvalidPos)
		{
			token = _string;
			_string.Clear();
			return token;
		}

		// we found a delimiter first
		if (tokend == 0)
		{
			_string.Erase(0);
			return ZBasicString<A>();
		}

		//Grab that token
		token = ZBasicString<A>(_string,0,tokend);

		//Erase that token from this string
		_string.Erase(0, tokend + 1);

		//Return the token
		return token;
	}

	//Tokenize C-String Implementation
	template <typename A>
	ZBasicString<A> Tokenize(ZBasicString<A>& _string, const char *_delims)
		{ return Tokenize(_string, ZBasicString<A>(_delims)); }

	/*
	public ZBasicStringAlgo<A>::ToLower
	
	Changes the provided region of the given string into all lowercase letters.
	
	@param A - the allocator type of the string
	@param _string - the string to mutate
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (void)
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename A>
	void ToLower(ZBasicString<A>& _string, size_t _start, size_t _end)
	{
		if (_start == _end)
			return;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::ToLower - Cannot lower with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
			_string.Data()[i] = (char)tolower(_string.Data()[i]);
	}

	/*
	public ZBasicStringAlgo<A>::ToLower
	
	Changes the provided string into all lowercase letters.
	
	@param A - the allocator type of the string
	@param _string - the string to mutate
	@return (void)
	*/
	template <typename A>
	void ToLower(ZBasicString<A>& _string)
	{
		ToLower(_string, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A>::ToUpper
	
	Changes the provided region of the given string into all uppercase letters.
	
	@param A - the allocator type of the string
	@param _string - the string to mutate
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (void)
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename A>
	void ToUpper(ZBasicString<A>& _string, size_t _start, size_t _end)
	{
		if (_start == _end)
			return;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::ToUpper - Cannot upper with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
			_string.Data()[i] = (char)toupper(_string.Data()[i]);
	}

	/*
	public ZBasicStringAlgo<A>::ToUpper
	
	Changes the provided region of the given string into all uppercase letters.
	
	@param A - the allocator type of the string
	@param _string - the string to mutate
	@return (void)
	*/
	template <typename A>
	void ToUpper(ZBasicString<A>& _string)
	{
		ToUpper(_string, 0, _string.Length());
	}
	
	/*
	public ZBasicStringAlgo<A>::TrimLeft
	
	Trims all the whitespace from the left (front) of the provided region of a string.
	
	@param A - the allocator type for the string
	@param _string - the string to trim
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the number of whitespace characters removed
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename A>
	size_t TrimLeft(ZBasicString<A>& _string, size_t _start, size_t _end)
	{
		if (_start == _end)
			return 0;

		size_t i;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::TrimLeft - Cannot trim with end < start!");
		#endif

		for (i = start; i < end; i++) 
		{
			if (!isspace(_string.Data()[i])) 
				break;
		}

		_string.Erase(start, i);

		return (i - start);
	}

	/*
	public ZBasicString<A>::TrimLeft
	
	Removes all whitespace from the left side (front) of this string.
	
	@param A - the allocator type for the string
	@param _string - the string to trim whitespace from
	@return (size_t) - the number of whitespace characters removed
	*/
	template <typename A>
	size_t TrimLeft(ZBasicString<A>& _string)
	{
		return TrimLeft(_string, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A>::TrimLeft
	
	Trims all the whitespace from the right (back) of the provided region of a string.
	
	@param A - the allocator type for the string
	@param _string - the string to trim
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the number of whitespace characters removed
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename A>
	size_t TrimRight(ZBasicString<A>& _string, size_t _start, size_t _end)
	{
		if (_start == _end)
			return 0;

		size_t i;

		const size_t start = _string.BoundsCheck(_start, _string.Length());
		const size_t end = _string.BoundsCheck(_end, _string.Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicStringAlgo::TrimRight - Cannot trim with end < start!");
		#endif

		//Remove any character that is a space character as determined by isspace
		for (i = end; i > start; i--) 
		{
			if (!isspace(_string.Data()[i - 1])) 
				break;
		}

		_string.Erase(i, end);

		return (end - i);
	}

	/*
	public ZBasicString<A>::TrimRight
	
	Removes all whitespace from the right side (back) of this string.
	
	@param _string - the string to trim whitespace from
	@return (size_t) - the number of whitespace characters removed
	*/
	template <typename A>
	size_t TrimRight(ZBasicString<A>& _string)
	{
		return TrimRight(_string, 0, _string.Length());
	}

	/*
	public ZBasicStringAlgo<A>::Trim
	
	Trims all the whitespace from both the left (front) and right (back) sides of the
	given region of a string.
	
	@param A - the allocator type for the string
	@param _string - the string to trim
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the number of whitespace characters removed
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename A>
	size_t Trim(ZBasicString<A>& _string, size_t _start, size_t _end)
	{
		if (_start == _end)
			return 0;

		size_t removed = TrimRight(_string, _start, _end);

		return removed + TrimLeft(_string, _start, _end - removed);
	}
	
	/*
	public ZBasicString<A>::Trim

	Removes all whitespace from both left and right side of this string.

	@param A - the allocator type of the string
	@param _string - the string to trim whitespace from
	@return (size_t) - the number of whitespace characters removed
	*/
	template <typename A>
	size_t Trim(ZBasicString<A>& _string)
	{
		if (_string.Empty())
			return 0;

		return TrimLeft(_string) + TrimRight(_string);
	}


};

#endif
