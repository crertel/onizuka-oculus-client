/*
	ZArrayAlgo.hpp
	Author: James Russell <jcrussell@762studios.com>
	Created: 1/12/2012

	Purpose: 

	Generalized algorithm implementations for use with ZArray.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _ZARRAYALGO_HPP
#define _ZARRAYALGO_HPP

#include <ZSTL/ZArray.hpp>
#include <ZSTL/ZSTLInvalidPos.hpp>

namespace ZArrayAlgo
{
	//Forward Declaration of FindFirst
	template <typename T, typename A>
	size_t FindFirst(const ZArray<T, A>& _array, const T& _value, size_t _start, size_t _end);

	//Forward Declaration of FindSub
	template <typename T, typename A, typename B>
	size_t FindSub(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _other, size_t _s2, size_t _e2);

	//Forward Declaration of Unique
	template <typename T, typename A>
	size_t Unique(ZArray<T, A>& _array, size_t _start, size_t _end);
	
	/*
	public ZArrayAlgo<T, A, B>::Append
	
	Appends the specified range of one array to another.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the array to append to
	@param _other - the array to append to the end of _array
	@param _start - the starting index of _other
	@param _end - the ending index of _other (exclusive)
	@return (void)
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A, typename B>
	void Append(ZArray<T, A>& _array, const ZArray<T, B>& _other, size_t _start, size_t _end)
	{ 
		if (_start == _end)
			return;

		const size_t start = _other.BoundsCheck(_start, _other.Size());
		const size_t end = _other.BoundsCheck(_end, _other.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Append - Cannot append with end < start!");
		#endif

		const size_t initialSize = _array.Size();
		const size_t delta = end - start;

		_array.Resize(_array.Size() + delta);

		for (size_t i = 0; i < delta; i++)
			_array.Data()[initialSize + i] = _other.Data()[start + i];
	}

	/*
	public ZArrayAlgo<T, A, B>::Append
	
	Appends one array to another.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the array to append to
	@param _other - the array to append to the end of _array
	@return (void)
	*/
	template <typename T, typename A, typename B>
	void Append(ZArray<T, A>& _array, const ZArray<T, B>& _other)
	{ 
		Append(_array, _other, 0, _other.Size());
	}

	/*
	public ZArrayAlgo<F, T, A>::Apply
	
	Maps the provided unary functor over the array between the given indices, mutating in place.

	@param F - unary functor [(T&) -> void] used to operate on array[i]
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to map the function over
	@param _functor - instance of the unary functor F
	@param _start - the index to start looking at
	@param _end - the end index (exclusive)
	@return (void)
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename F, typename T, typename A>
	void Apply(ZArray<T, A>& _array, F _functor, size_t _start, size_t _end)
	{ 
		if (_start == _end)
			return;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Map - Cannot map with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
			_functor(_array.Data()[i]);
	}

	/*
	public ZArrayAlgo<F, T, A>::Apply
	
	Maps the provided unary functor over the array, mutating in place.

	@param F - unary functor [(T&) -> void] used to operate on array[i]
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to map the function over
	@param _functor - instance of the unary functor F
	@return (void)
	*/
	template <typename F, typename T, typename A>
	void Apply(ZArray<T, A>& _array, F _functor)
	{
		Apply(_array, _functor, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A, B>::Concatenate
	
	Concatenates sections of two arrays and returns a new array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the first array
	@param _s1 - the start index on the first array
	@param _e1 - the ending index on the first array (exclusive)
	@param _other - the array that will be 
	@param _s2 - the start index on the second array
	@param _e2 - the end index on the second array (exclusive)
	@return (ZArray<T, A>)
	@assert - if _s1 < _e1 or _s2 < _e2
			  if _s1, _e1, _s2, or _e2 out of bounds 
	*/
	template <typename T, typename A, typename B>
	ZArray<T, A> Concatenate(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _other, size_t _s2, size_t _e2) 
	{
		if (_s1 == _e1)
			return _other;
		else if (_s2 == _e2)
			return _array;

		const size_t s1 = _array.BoundsCheck(_s1, _array.Size());
		const size_t e1 = _array.BoundsCheck(_e1, _array.Size() + 1);	// 1 to allow indexing of end

		const size_t s2 = _other.BoundsCheck(_s2, _other.Size());
		const size_t e2 = _other.BoundsCheck(_e2, _other.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZArrayAlgo::Merge - Cannot merge with e1 < s1!");
		ZSTL_ASSERT(s2 <= e2, "ZArrayAlgo::Merge - Cannot merge with e2 < s2!");
		#endif

		const size_t delta1 = e1 - s1;
		const size_t delta2 = e2 - s2;

		const size_t newSize = delta1 + delta2;

		if (newSize == 0)
			return ZArray<T, A>();

		ZArray<T, A> ret(&_array.Data()[s1], delta1, newSize);

		ret.Resize(newSize);

		for (size_t i = 0; i < delta2; i++)
			ret.Data()[delta1 + i] = _other.Data()[s2 + i];

		return ret;
	}

	/*
	public ZArrayAlgo<T, A, B>::Concatenate
	
	Concatenates an array to another and returns a new array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the first array
	@param _other - the second array 
	@return (ZArray<T, A>) - the result of concatenation
	*/
	template <typename T, typename A, typename B>
	ZArray<T, A> Concatenate(const ZArray<T, A>& _array, const ZArray<T, B>& _other) 
	{
		return Concatenate(_array, 0, _array.Size(), _other, 0, _other.Size());
	}
	
	/*
	public ZArrayAlgo<T, A>::Contains

	Determines if the array contains the given value between the given indices.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to check for the value
	@param _value - the value to search for
	@param _start - the index to start searching at
	@param _end - the end index (exclusive)
	@return (bool) - true if found, false otherwise
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A>
	bool Contains(const ZArray<T, A>& _array, const T& _value, size_t _start, size_t _end) 
	{
		return FindFirst(_array, _value, _start, _end) != ZSTL::InvalidPos;
	}

	/*
	public ZArrayAlgo<T, A>::Contains

	Determines if the array contains the given value.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to check for the value
	@param _value - the value to search for
	@return (bool) - true if found, false otherwise
	*/
	template <typename T, typename A>
	bool Contains(const ZArray<T, A>& _array, const T& _value) 
	{
		return Contains(_array, _value, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::ContainsSub

	Determines if the array contains the given sub array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the array to check for the value
	@param _other - the sub array to search for
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (bool) - true if found, false otherwise
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A, typename B>
	bool ContainsSub(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _other, size_t _s2, size_t _e2)
	{
		return FindSub(_array, _s1, _e1, _other, _s2, _e2) != ZSTL::InvalidPos;
	}

	/*
	public ZArrayAlgo<T, A>::ContainsSub

	Determines if the array contains the given sub array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the array to check for the value
	@param _other - the sub array to search for
	@return (bool) - true if found, false otherwise
	*/
	template <typename T, typename A, typename B>
	bool ContainsSub(const ZArray<T, A>& _array, const ZArray<T, B>& _other)
	{
		return ContainsSub(_array, 0, _array.Size(), _other, 0, _other.Size());
	}

	/*
	public ZArrayAlgo<T, A, B>::Copy
	
	Copies a number of given elements into the given range of the provided array,
	from the provided range in the other array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the array to copy data into
	@param _s1 - the starting index to copy to
	@param _e1 - the ending index to copy to (exclusive)
	@param _other - the array to copy data from
	@param _s2 - the starting index to copy from
	@param _e2 - the ending index to copy to (exclusive)
	@return (void)
	@assert - if _s1, _e1, _s2, or _e2 out of bounds
			  if _e1 < _s1 or _e2 < _s2
	*/
	template <typename T, typename A, typename B>
	void Copy(ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _other, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return;

		const size_t s1 = _array.BoundsCheck(_s1, _array.Size());
		const size_t e1 = _array.BoundsCheck(_e1, _array.Size() + 1);	// 1 to allow indexing of end

		const size_t s2 = _other.BoundsCheck(_s2, _other.Size());
		const size_t e2 = _other.BoundsCheck(_e2, _other.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZArrayAlgo::Copy - Cannot copy with e1 < s1!");
		ZSTL_ASSERT(s2 <= e2, "ZArrayAlgo::Copy - Cannot copy with e2 < s2!");
		#endif

		size_t i, j;

		for (i = s1, j = s2; i < e1 && j < e2; i++, j++)
			_array.Data()[i] = _other.Data()[j];
	}

	/*
	public ZArrayAlgo<T, A, B>::Copy
	
	Copies the elements of one array into the other.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the array to copy data into
	@param _other - the array to copy data from
	@param _count - the number of elements to copy
	@return (void)
	@assert - if _count > array size or other size
	*/
	template <typename T, typename A, typename B>
	void Copy(ZArray<T, A>& _array, const ZArray<T, B>& _other)
	{
		return Copy(_array, 0, _array.Size(), _other, 0, _other.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Count

	Returns the number of occurrences of the given value in the given range of the 
	provided array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to count in
	@param _value - the value to search for 
	@param _start - the index to start at
	@param _end - the ending index (exclusive)
	@return (size_t) - the number of occurrences of _value at and after _index
	@assert - if _end < _start
			  if _start or _end out of bounds
	*/
	template <typename T, typename A>
	size_t Count(const ZArray<T, A>& _array, const T& _value, size_t _start, size_t _end) 
	{ 
		if (_start == _end)
			return 0;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Count - Cannot count with end < start!");
		#endif

		size_t count = 0;

		for (size_t i = start; i < end; i++)
		{
			if (_array.Data()[i] == _value)
				count++;
		}

		return count;
	}

	/*
	public ZArrayAlgo<T, A>::Count

	Returns the number of occurrences of the given value in the array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to count in
	@param _value - the value to search for 
	@return (size_t) - the number of occurrences of _value at and after _index
	*/
	template <typename T, typename A>
	size_t Count(const ZArray<T, A>& _array, const T& _value) 
	{
		return Count(_array, _value, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A, B>::EndsWith

	Used to determine if the given region of the provided array ends with the contents of the
	given region of the other array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the array to check
	@param _s1 - the starting index to check against
	@param _e1 - the ending index to check against (exclusive)
	@param _other - the array to check from
	@param _s2 - the starting index to check from
	@param _e2 - the ending index to check to (exclusive)
	@return (bool) - true if the array ends with the provided values, false otherwise
	@assert - if _s1, _e1, _s2, or _e2 out of bounds
			  if _e1 < _s1 or _e2 < _s2
	*/
	template <typename T, typename A, typename B>
	bool EndsWith(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _other, size_t _s2, size_t _e2)
	{
		return FindSub(_array, _s1, _e1, _other, _s2, _e2) == (_e1 - (_e2 - _s2));
	}


	/*
	public ZArrayAlgo<T, A, B>::EndsWith

	Used to determine if the provided array ends with the values contained in the other array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the array to check
	@param _other - the array to check from
	@return (bool) - true if the array ends with the provided values, false otherwise
	*/
	template <typename T, typename A, typename B>
	bool EndsWith(const ZArray<T, A>& _array, const ZArray<T, B>& _other)
	{
		return EndsWith(_array, 0, _array.Size(), _other, 0, _other.Size());
	}

	/*
	public ZArrayAlgo<T, A, B>::Equal
	
	Determines if the arrays are equivalent in the given subsections, up to the length of the shortest
	subsection.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the array to compare
	@param _s1 - the starting index to compare
	@param _e1 - the ending index to compare (exclusive)
	@param _other - the array to compare against
	@param _s2 - the starting index to compare against
	@param _e2 - the ending index to compare against (exclusive)
	@return (bool) - true if ranges are equivalent, false otherwise
	@assert - if _s1, _e1, _s2, or _e2 out of bounds
			  if _e1 < _s1 or _e2 < _s2
	*/
	template <typename T, typename A, typename B>
	bool Equal(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _other, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return false;

		const size_t s1 = _array.BoundsCheck(_s1, _array.Size());
		const size_t e1 = _array.BoundsCheck(_e1, _array.Size() + 1);	// 1 to allow indexing of end

		const size_t s2 = _other.BoundsCheck(_s2, _other.Size());
		const size_t e2 = _other.BoundsCheck(_e2, _other.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZArrayAlgo::Equal - Cannot compare with e1 < s1!");
		ZSTL_ASSERT(s2 <= e2, "ZArrayAlgo::Equal - Cannot compare with e2 < s2!");
		#endif

		size_t i, j;

		for (i = s1, j = s2; i < e1 && j < e2; i++, j++)
			if (!(_array.Data()[i] == _other.Data()[j]))
				return false;

		return true;
	}

	/*
	public ZArrayAlgo<T, A, B>::Equal
	
	Determines if the arrays are equal in both size and contents.
	
	@param _array - the array to check
	@param _other - the array to check against
	@return (bool) - true if equivalent, false otherwise
	*/
	template <typename T, typename A, typename B>
	bool Equal(const ZArray<T, A>& _array, const ZArray<T, B>& _other)
	{
		return _array.Equals(_other);
	}

	/*
	public ZArrayAlgo<T, A>::Fill
	
	Fills a section of the array full of a given value.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to fill full of values
	@param _value - the value to fill into the array
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (void)
	@assert - if _end < _start
			  if _start or _end out of bounds
	*/
	template <typename T, typename A>
	void Fill(ZArray<T, A>& _array, const T& _value, size_t _start, size_t _end)
	{
		if (_start == _end)
			return;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Fill - Cannot fill with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
			_array.Data()[i] = _value;		
	}

	/*
	public ZArrayAlgo<T, A>::Fill
	
	Fills the array full of the given value.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to fill full of values
	@param _value - the value to fill into the array
	@return (void)
	*/
	template <typename T, typename A>
	void Fill(ZArray<T, A>& _array, const T& _value)
	{
		Fill(_array, _value, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Find

	Finds the Nth occurrence of a value in the provided region of the array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to search
	@param _value - the value to find
	@param _count - the number of occurrences which should be skipped (meaning an index to the (N+1)th occurrence is returned)
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - index of the (N+1)th occurrence
	@assert - if _end < _start
			  if _start or _end out of bounds
	*/
	template <typename T, typename A>
	size_t Find(const ZArray<T, A>& _array, const T& _value, size_t _count, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Find - Cannot find with end < start!");
		#endif

		size_t j = _count;

		for (size_t i = start; i < end; i++)
		{
			if(_array.Data()[i] == _value)
			{
				if (j == 0)
					return i;
				j--;
			}
		}

		return ZSTL::InvalidPos;
	}

	/*
	public ZArrayAlgo<T, A>::Find

	Finds the Nth occurrence of a value in the array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to search
	@param _value - the value to find
	@param _count - the number of occurrences which should be skipped (meaning an index to the (N+1)th occurrence is returned)
	@return (size_t) - index of the (N+1)th occurrence
	*/
	template <typename T, typename A>
	size_t Find(const ZArray<T, A>&_array, const T& _value, size_t _count)
	{
		return Find(_array, _value, _count, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::FindAll
	 
	Finds all the things in the array of the provided value and returns their indices.
	Note that this will return the indices in order of appearance in the searched array.
	 
	@param _array - the list to search through
	@param _value - the value to search for
	@param _start - the starting index (inclusive) to search from
	@param _end - the ending index (exclusive) to search from
	@return (ZArray<size_t>) - array of indices where values appeared (empty list of none found)
	@assert - if _end < _start
			  if _start or _end out of bounds
	*/
	template <typename T, typename A>
	ZArray<size_t> FindAll(const ZArray<T, A>& _array, const T& _value, size_t _start, size_t _end)
	{
		ZArray<size_t> locations(_array.Size() + 1);

		if (_start == _end)
			return locations;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Fill - Cannot fill with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
		{
			if (_array.Data()[i] == _value)
				locations.PushBack(i);
		}

		return locations;
	}

	/*
	public ZArrayAlgo<T, A>::FindAll
	 
	Finds all the things in the array of the provided value and returns their indices.
	Note that this will return the indices in order of appearance in the searched array.
	 
	@param _array - the list to search through
	@param _value - the value to search for
	@return (ZArray<size_t>) - array of indices where values appeared (empty list of none found)
	*/
	template <typename T, typename A>
	ZArray<size_t> FindAll(const ZArray<T, A>& _array, const T& _value)
	{
		return ZArrayAlgo::FindAll(_array, _value, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::FindAllOf
	 
	Finds all the things in the given region of the array that are equal to one 
	of the provided values and returns their indices. Note that this will return
	the indices in order of appearance in the searched array.
	 
	@param _array - the list to search through
	@param _value - the values to search for
	@param _array - the array to compare
	@param _s1 - the starting index to search
	@param _e1 - the ending index to search (exclusive)
	@param _other - the array to compare against
	@param _s2 - the starting index to find
	@param _e2 - the ending index to find (exclusive)
	@return (ZArray<size_t, A>) - array of indices where values appeared (empty of none found)
	@assert - if _s1, _e1, _s2, or _e2 out of bounds
			  if _e1 < _s1 or _e2 < _s2
	*/
	template <typename T, typename A, typename B>
	ZArray<size_t> FindAllOf(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _values, size_t _s2, size_t _e2)
	{
		ZArray<size_t> locations(_array.Size() + 1);

		if (_s1 == _e1 || _s2 == _e2)
			return locations;

		const size_t s1 = _array.BoundsCheck(_s1, _array.Size());
		const size_t e1 = _array.BoundsCheck(_e1, _array.Size() + 1);	// 1 to allow indexing of end

		const size_t s2 = _values.BoundsCheck(_s2, _values.Size());
		const size_t e2 = _values.BoundsCheck(_e2, _values.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZArrayAlgo::FindAllOf - Cannot find with e1 < s1!");
		ZSTL_ASSERT(s2 <= e2, "ZArrayAlgo::FindAllOf - Cannot find with e2 < s2!");
		#endif

		for (size_t i = s1; i < e1; i++)
		{
			for (size_t j = s2; j < e2; j++)
			{
				if ( _array.Data()[i] == _values.Data()[j] )
				{
					locations.PushBack(i);
					break;
				}
			}
		}

		return locations;
	}

	/*
	public ZArrayAlgo<T, A>::FindAllOf
	 
	Finds all the things in the array that are equal to one of the provided values and returns their indices.
	Note that this will return the indices in order of appearance in the searched array.	 
	@param _array - the list to search through
	@param _value - the values to search for
	@return (ZArray<size_t>) - array of indices where values appeared (empty of none found)
	*/
	template <typename T, typename A, typename B>
	ZArray<size_t> FindAllOf(const ZArray<T, A>& _array, const ZArray<T, B>& _values)
	{
		return FindAllOf(_array, 0, _array.Size(), _values, 0, _values.Size());
	}

	/*
	public ZArrayAlgo<F, T, A>::FindIf
	
	Finds a value in the provided range of the array that evaluates a unary functor to true.

	@param F - unary functor [(const T&) -> bool] used to test array[i]
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to find elements in
	@param _functor - instance of the unary functor F to test with
	@param _end - the end index (exclusive)
	@return (size_t) - the first instance that evaluates to true from _functor (ZSTL::InvalidPos if not found)
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename F, typename T, typename A>
	size_t FindIf(const ZArray<T, A>& _array, F _functor, size_t _start, size_t _end)
	{
		if (_start == _end)
			return 0;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Find - Cannot find with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
		{
			if (_functor(_array.Data()[i]))
				return i;
		}

		return ZSTL::InvalidPos;
	}

	/*
	public ZArrayAlgo<F, T, A>::FindIf
	
	Finds a value in the array that evaluates a unary functor to true.

	@param F - unary functor [(const T&) -> bool] used to test array[i]
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to find elements in
	@param _functor - instance of the unary functor F to test with
	@return (size_t) - the first instance that evaluates to true from _functor (ZSTL::InvalidPos if not found)
	*/
	template <typename F, typename T, typename A>
	size_t FindIf(const ZArray<T, A>& _array, F _functor)
	{
		return FindIf(_array, _functor, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::FindFirst

	Looks for the first occurrence of the value between the specified indices.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to find elements in
	@param _value - the value to look for
	@param _start - the index to start looking at
	@param _end - the end index (exclusive)
	@return (size_t) - the index of the first match (ZSTL::InvalidPos if not found)
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A>
	size_t FindFirst(const ZArray<T, A>& _array, const T& _value, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::FindFirstOf - Cannot find with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
		{
			if (_array.Data()[i] == _value)
				return i;
		}

		return ZSTL::InvalidPos;
	}

	/*
	public ZArrayAlgo<T, A>::FindFirst

	Looks for the first occurrence of the specified value in the given array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to find elements in
	@param _value - the value to look for
	@return (size_t) - the index of the first match (ZSTL::InvalidPos if not found)
	*/
	template <typename T, typename A>
	size_t FindFirst(const ZArray<T, A>& _array, const T& _value)
	{
		return FindFirst(_array, _value, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A, B>::FindFirstOf

	Looks for the first occurrence of one of the given values between the specified indices.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type for the values array
	@param _array - the array to find elements in
	@param _s1 - the starting index in array
	@param _e1 - the ending index in array (exclusive)
	@param _values - the values to look for
	@param _s2 - the starting index in values
	@param _e2 - the ending index in values (exclusive)
	@return (size_t) - the index of the first match (ZSTL::InvalidPos if not found)
	@assert - if _s1, _e1, _s2, or _e2 out of bounds
			  if _e1 < _s1 or _e2 < _s2
	*/
	template <typename T, typename A, typename B>
	size_t FindFirstOf(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _values, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return ZSTL::InvalidPos;

		const size_t s1 = _array.BoundsCheck(_s1, _array.Size());
		const size_t e1 = _array.BoundsCheck(_e1, _array.Size() + 1);	// 1 to allow indexing of end

		const size_t s2 = _values.BoundsCheck(_s2, _values.Size());
		const size_t e2 = _values.BoundsCheck(_e2, _values.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZArrayAlgo::FindFirstOf - Cannot find with e1 < s1!");
		ZSTL_ASSERT(s2 <= e2, "ZArrayAlgo::FindFirstOf - Cannot find with e2 < s2!");
		#endif

		for (size_t i = s1; i < e1; i++)
		{
			for (size_t j = s2; j < e2; j++)
			{
				if ( _array.Data()[i] == _values.Data()[j] )
					return i;
			}
		}

		return ZSTL::InvalidPos;
	}

	/*
	public ZArrayAlgo<T, A, B>::FindFirstOf

	Looks for the first occurrence of one of the specified values in the given array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type for the values array
	@param _array - the array to find elements in
	@param _values - the values to look for
	@return (size_t) - the index of the first match (ZSTL::InvalidPos if not found)
	*/
	template <typename T, typename A, typename B>
	size_t FindFirstOf(const ZArray<T, A>& _array, const ZArray<T, B>& _values)
	{
		return FindFirstOf(_array, 0, _array.Size(), _values, 0, _values.Size());
	}

	/*
	public ZArrayAlgo<T, A>::FindFirstNot

	Looks for the first occurrence of a value that is not the provided value between the specified 
	indices.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to find elements in
	@param _value - the value to avoid
	@param _start - the index to start looking at
	@param _end - the end index (exclusive)
	@return (size_t) - the index of the first non-match (ZSTL::InvalidPos if not found)
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A>
	size_t FindFirstNot(const ZArray<T, A>& _array, const T& _value, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::FindFirstNot - Cannot find with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
		{
			if (!(_array.Data()[i] == _value))
				return i;
		}

		return ZSTL::InvalidPos;
	}
	
	/*
	public ZArrayAlgo<T, A>::FindFirstNot

	Looks for the first occurrence of a value that is not the provided value in the given array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to find elements in
	@param _value - the value to avoid
	@return (size_t) - the index of the first non-match (ZSTL::InvalidPos if not found)
	*/
	template <typename T, typename A>
	size_t FindFirstNot(const ZArray<T, A>& _array, const T& _value)
	{
		return FindFirstNot(_array, _value, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A, B>::FindFirstNotOf

	Looks for the first occurrence of a value that is not one of the provided values between the 
	specified indices.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type for the values array
	@param _array - the array to find elements in
	@param _s1 - the starting index in array
	@param _e1 - the ending index in array (exclusive)
	@param _values - the values to avoid
	@param _s2 - the starting index in values
	@param _e2 - the ending index in values (exclusive)
	@return (size_t) - the index of the first non-match (ZSTL::InvalidPos if not found)
	@assert - if _s1, _e1, _s2, or _e2 out of bounds
			  if _e1 < _s1 or _e2 < _s2
	*/
	template <typename T, typename A, typename B>
	size_t FindFirstNotOf(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _values, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return ZSTL::InvalidPos;

		const size_t s1 = _array.BoundsCheck(_s1, _array.Size());
		const size_t e1 = _array.BoundsCheck(_e1, _array.Size() + 1);	// 1 to allow indexing of end

		const size_t s2 = _values.BoundsCheck(_s2, _values.Size());
		const size_t e2 = _values.BoundsCheck(_e2, _values.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZArrayAlgo::FindFirstNotOf - Cannot find with e1 < s1!");
		ZSTL_ASSERT(s2 <= e2, "ZArrayAlgo::FindFirstNotOf - Cannot find with e2 < s2!");
		#endif

		for (size_t i = s1; i < e1; i++)
		{
			for (size_t j = s2; j < e2; j++)
			{
				if ( _array.Data()[i] == _values.Data()[j] )
					break;

				if (j == e2 - 1)
					return i;
			}
		}

		return ZSTL::InvalidPos;
	}

	/*
	public ZArrayAlgo<T, A, B>::FindFirstNotOf

	Looks for the first occurrence of a value that is not one of the provided values in the given array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type for the values array
	@param _array - the array to find elements in
	@param _values - the values to avoid
	@return (size_t) - the index of the first non-match (ZSTL::InvalidPos if not found)
	*/
	template <typename T, typename A, typename B>
	size_t FindFirstNotOf(const ZArray<T, A>& _array, const ZArray<T, B>& _values)
	{
		return FindFirstNotOf(_array, 0, _array.Size(), _values, 0, _values.Size());
	}

	/*
	public ZArrayAlgo<T, A>::FindLast
	
	Finds the last occurrence of the specified value in the array between the given
	indices.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to find elements in
	@param _value - the value to find 
	@param _start - the index to start looking at
	@param _end - the end index (exclusive)
	@return (size_t) - the final occurrence of _value in the array (ZSTL::InvalidPos if not found)
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A>
	size_t FindLast(const ZArray<T, A>& _array, const T& _value, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::FindLast - Cannot find with end < start!");
		#endif

		for (size_t i = end; i > start; i--)
		{
			if (_array.Data()[i - 1] == _value)
				return i - 1;
		}

		return ZSTL::InvalidPos;
	}
	
	/*
	public ZArrayAlgo<T, A>::FindLast
	
	Finds the last occurrence of the specified value in the array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to find elements in
	@param _value - the value to find 
	@return (size_t) - the final occurrence of _value in the array (ZSTL::InvalidPos if not found)
	*/
	template <typename T, typename A>
	size_t FindLast(const ZArray<T, A>& _array, const T& _value)
	{
		return FindLast(_array, _value, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A, B>::FindLastOf
	
	Finds the last occurrence of one of the specified values in the array between the given
	indices.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type for the values array
	@param _array - the array to find elements in
	@param _s1 - the starting index in array
	@param _e1 - the ending index in array (exclusive)
	@param _values - the values to look for
	@param _s2 - the starting index in values
	@param _e2 - the ending index in values (exclusive)
	@return (size_t) - the index of the last match (ZSTL::InvalidPos if not found)
	@assert - if _s1, _e1, _s2, or _e2 out of bounds
			  if _e1 < _s1 or _e2 < _s2
	*/
	template <typename T, typename A, typename B>
	size_t FindLastOf(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _values, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return ZSTL::InvalidPos;

		const size_t s1 = _array.BoundsCheck(_s1, _array.Size());
		const size_t e1 = _array.BoundsCheck(_e1, _array.Size() + 1);	// 1 to allow indexing of end

		const size_t s2 = _values.BoundsCheck(_s2, _values.Size());
		const size_t e2 = _values.BoundsCheck(_e2, _values.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZArrayAlgo::FindFirstNotOf - Cannot find with e1 < s1!");
		ZSTL_ASSERT(s2 <= e2, "ZArrayAlgo::FindFirstNotOf - Cannot find with e2 < s2!");
		#endif

		for (size_t i = e1; i > s1; i--)
		{
			for (size_t j = s2; j < e2; j++)
			{
				if (_array.Data()[i - 1] == _values.Data()[j])
					return i - 1;
			}
		}

		return ZSTL::InvalidPos;
	}

	/*
	public ZArrayAlgo<T, A, B>::FindLastOf
	
	Finds the last occurrence of one of the specified values in the array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type for the values array
	@param _array - the array to find elements in
	@param _value - the value to find 
	@return (size_t) - the final occurrence of _value in the array (ZSTL::InvalidPos if not found)
	*/
	template <typename T, typename A, typename B>
	size_t FindLastOf(const ZArray<T, A>& _array, const ZArray<T, B>& _values)
	{
		return FindLastOf(_array, 0, _array.Size(), _values, 0, _values.Size());
	}

	/*
	public ZArrayAlgo<T, A>::FindLastNot
	
	Finds the last occurrence of a value that is not the specified value in the array. between the given
	indices.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to find elements in
	@param _value - the value to avoid 
	@param _start - the index to start looking at
	@param _end - the end index (exclusive)
	@return (size_t) - the final occurrence of  a non-match in the array (ZSTL::InvalidPos if not found)
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A>
	size_t FindLastNot(const ZArray<T, A>& _array, const T& _value, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::FindLastNotOf - Cannot find with end < start!");
		#endif

		for (size_t i = end; i > start; i--)
		{
			if (!(_array.Data()[i - 1] == _value))
				return i - 1;
		}

		return ZSTL::InvalidPos;
	}

	/*
	public ZArrayAlgo<T, A>::FindLastNot
	
	Finds the last occurrence of a value that is not the specified value in the array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to find elements in
	@param _value - the value to avoid 
	@return (size_t) - the final occurrence of  a non-match in the array (ZSTL::InvalidPos if not found)
	*/
	template <typename T, typename A>
	size_t FindLastNot(const ZArray<T, A>& _array, const T& _value)
	{
		return FindLastNot(_array, _value, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A, B>::FindLastNotOf
	
	Finds the last occurrence of a value that is not one of the specified values in the array between 
	the given indices.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type for the values array
	@param _array - the array to find elements in
	@param _s1 - the starting index in array
	@param _e1 - the ending index in array (exclusive)
	@param _values - the values to avoid
	@param _s2 - the starting index in values
	@param _e2 - the ending index in values (exclusive)
	@return (size_t) - the index of the last non0match (ZSTL::InvalidPos if not found)
	@assert - if _s1, _e1, _s2, or _e2 out of bounds
			  if _e1 < _s1 or _e2 < _s2
	*/
	template <typename T, typename A, typename B>
	size_t FindLastNotOf(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _values, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return ZSTL::InvalidPos;

		const size_t s1 = _array.BoundsCheck(_s1, _array.Size());
		const size_t e1 = _array.BoundsCheck(_e1, _array.Size() + 1);	// 1 to allow indexing of end

		const size_t s2 = _values.BoundsCheck(_s2, _values.Size());
		const size_t e2 = _values.BoundsCheck(_e2, _values.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZArrayAlgo::FindFirstNotOf - Cannot find with e1 < s1!");
		ZSTL_ASSERT(s2 <= e2, "ZArrayAlgo::FindFirstNotOf - Cannot find with e2 < s2!");
		#endif

		for (size_t i = e1; i > s1; i--)
		{
			for (size_t j = s2; j < e2; j++)
			{
				if (_array.Data()[i - 1] == _values.Data()[j])
					break;
				
				if (j == e2 - 1)
					return i - 1;
			}
		}

		return ZSTL::InvalidPos;
	}

	/*
	public ZArrayAlgo<T, A, B>::FindLastNotOf
	
	Finds the last occurrence of a value that is not one of the specified values in the array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the values array
	@param _array - the array to find elements in
	@param _values - the values to avoid 
	@return (size_t) - the final occurrence of a non-match in the array (ZSTL::InvalidPos if not found)
	*/
	template <typename T, typename A, typename B>
	size_t FindLastNotOf(const ZArray<T, A>& _array, const ZArray<T, B>& _values)
	{
		return FindLastNotOf(_array, 0, _array.Size(), _values, 0, _values.Size());
	}

	/*
	public ZArrayAlgo<T, A, B>::FindSub
	
	Looks for the first occurrence of a sub array within the given array in the given region.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the array to search in
	@param _s1 - the starting index in _array
	@param _e1 - the ending index in _array (exclusive)
	@param _other - the values to search for
	@param _s2 - the starting index in _other
	@param _e2 - the ending index in _other (exclusive)
	@return (size_t) - index to the first occurrence of the sub array
	@assert - if _s1, _e1, _s2, or _e2 out of bounds
			  if _e1 < _s1 or _e2 < _s2
	*/
	template <typename T, typename A, typename B>
	size_t FindSub(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _other, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return ZSTL::InvalidPos;

		const size_t s1 = _array.BoundsCheck(_s1, _array.Size());
		const size_t e1 = _array.BoundsCheck(_e1, _array.Size() + 1);	// 1 to allow indexing of end

		const size_t s2 = _other.BoundsCheck(_s2, _other.Size());
		const size_t e2 = _other.BoundsCheck(_e2, _other.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZArrayAlgo::FindSub - Cannot compare with e1 < s1!");
		ZSTL_ASSERT(s2 <= e2, "ZArrayAlgo::FindSub - Cannot compare with e2 < s2!");
		#endif

		// easy case: subarray is empty
		if (_other.Empty())
			return ZSTL::InvalidPos;

		// easy case: subarray can't fit into array
		if ( _array.Size() <= e2 - s2)
			return ZSTL::InvalidPos;

		// easy case: subarray can't fit into region of array
		if ( e1 - s1 < e2 - s2)
			return ZSTL::InvalidPos;

		for (size_t i = s1; i <= e1 - (e2 - s2); i++)
		{
			if (_array.Data()[i] == _other.Data()[s2])
			{
				// short-circuit if matching just one thing
				if (e2 - s2 == 1)
					return i;

				size_t arrayOffset = i + 1;
				// matched first element, so now try the ccccombo
				for (size_t j = s2 + 1; j < e2; j++)
				{
					if (_array.Data()[arrayOffset] != _other.Data()[j])
						break;

					if ((arrayOffset+1) - i == e2 - s2)
						return i;
					arrayOffset++; // won't overrun due to (_array.Size() <= e2-s2) constraint
				}
			}
		}

		return ZSTL::InvalidPos;		
	}

	/*
	public ZArrayAlgo<T, A, B>::FindSub
	
	Looks for the first occurrence of a sub array within the given array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the array to look in
	@param _other - the sub array to look for
	@return (size_t) - index to the first occurrence of the sub array
	*/
	template <typename T, typename A, typename B>
	size_t FindSub(const ZArray<T, A>& _array, const ZArray<T, B>& _other)
	{
		return FindSub(_array, 0, _array.Size(), _other, 0, _other.Size());
	}

	/*
	public ZArrayAlgo<V, T, A>::FoldLeft

	Executes an iterative fold left on the provided array in the given region.

	Given an initial value V, an array region containing [ 1, 2, 3, 4 ] and a functor that emulates the 
	+ operation, fold left produces the following result:

		FoldLeft( [ 1, 2, 3, 4 ], (+), V ) -> ( ( (V + 1) + 2 ) + 3 ) + 4 

	@param F - binary functor [(const T&, const T&) -> T] for the fold operation
	@param V - the initial value type and accumulated type
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to accumulate on
	@param _functor - instance of the binary functor F to test with
	@param _initialValue - the initial value for the fold operation
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (V) - the accumulated value
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename F, typename V, typename T, typename A>
	V FoldLeft(const ZArray<T, A>& _array, F _functor, V _initialValue, size_t _start, size_t _end )
	{
		if (_start == _end)
			return _initialValue;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::FoldLeft - Cannot fold with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
			_initialValue = _functor(_initialValue, _array.Data()[i]);

		return _initialValue;
	}
	
	/*
	public ZArrayAlgo<V, T, A>::FoldLeft

	Executes an iterative fold left on the provided array.

	Given an initial value V, an array containing [ 1, 2, 3, 4 ] and a functor that emulates the 
	+ operation, fold left produces the following result:

		FoldLeft( [ 1, 2, 3, 4 ], (+), V ) -> ( ( (V + 1) + 2 ) + 3 ) + 4 

	@param F - binary functor [(const T&, const T&) -> T] used for the fold operation
	@param V - the initial value type and accumulated type
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to accumulate on
	@param _functor - instance of the binary functor F to test with
	@param _initialValue - the initial value for the fold operation
	@return (V) - the accumulated value
	*/
	template <typename F, typename V, typename T, typename A>
	V FoldLeft(const ZArray<T, A>& _array, F _functor, V _initialValue)
	{
		return FoldLeft(_array, _functor, _initialValue, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<F, V, T, A>::FoldRight

	Executes an iterative fold right on the provided array in the given region.

	Given an initial value V, an array region containing [ 1, 2, 3, 4 ] and a functor that emulates the 
	+ operation, fold right produces the following result:

		FoldRight( [ 1, 2, 3, 4 ], (+), V ) -> 1 + ( 2 + ( 3 + ( 4 + V ) ) ) 

	@param F - binary functor [(const T&, const T&) -> T] for the fold operation
	@param V - the initial value type and accumulated type
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to accumulate on
	@param _functor - instance of the binary functor F to test with
	@param _initialValue - the initial value for the fold operation
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (V) - the accumulated value
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename F, typename V, typename T, typename A>
	V FoldRight(const ZArray<T, A>& _array, F _functor, V _initialValue, size_t _start, size_t _end )
	{
		if (_start == _end)
			return _initialValue;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::FoldLeft - Cannot fold with end < start!");
		#endif

		for (size_t i = end; i > start; i--)
			_initialValue = _functor(_array.Data()[i - 1], _initialValue);

		return _initialValue;
	}

	/*
	public ZArrayAlgo<F, V, T, A>::FoldRight

	Executes an iterative fold right on the provided array.

	Given an initial value V, an array containing [ 1, 2, 3, 4 ] and a functor that emulates the 
	+ operation, fold right produces the following result:

		FoldRight( [ 1, 2, 3, 4 ], (+), V ) -> 1 + ( 2 + ( 3 + ( 4 + V ) ) ) 

	@param F - binary functor [(const T&, const T&) -> T] used for the fold operation
			     the first argument is the value at the current point in the array
				 the second argument is the current accumulated value
	@param V - the initial value type and accumulated type
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to accumulate on
	@param _functor - instance of the binary functor F to test with
	@param _initialValue - the initial value for the fold operation
	@return (V) - the accumulated value
	*/
	template <typename F, typename V, typename T, typename A>
	V FoldRight(const ZArray<T, A>& _array, F _functor, V _initialValue)
	{
		return FoldRight(_array, _functor, _initialValue, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<GF, T, A>::Generate

	Fills the given region of the array with values generated from the provided generator functor.

	@param F - binary functor [(size_t, size_t) -> T] used to generate
			     the first argument is the current number in the generated sequence
				 the second argument is the number of generations remaining
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to generate values for
	@param _functor - instance of the binary functor GF to generate values with
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (void)
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename GF, typename T, typename A>
	void Generate(ZArray<T, A>& _array, GF _generator, size_t _start, size_t _end)
	{
		if (_start == _end)
			return;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::FindLastNotOf - Cannot find with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
			_array.Data()[i] = _generator( i - start, end - start);
	}

	/*
	public ZArrayAlgo<GF, T, A>::Generate

	Fills the array with values generated from the provided generator functor.

	@param F - binary functor [(size_t, size_t) -> T] used to generate
			     the first argument is the current number in the generated sequence
				 the second argument is the number of generations remaining
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to generate values for
	@param _functor - instance of the binary functor GF to generate values with
	@return (void)
	*/
	template <typename GF, typename T, typename A>
	void Generate(ZArray<T, A>& _array, GF _generator)
	{
		ZArrayAlgo::Generate( _array, _generator, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<F, T, A>::Map

	Maps the provided functor over the array between the given indices, returning a 
	transformed array and leaving the original array intact.

	@param F - the unary functor ([T&] -> void) to transform the array with
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to transform
	@param _functor - instance of the unary functor F to operate on 
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (ZArray<T, A>) the transformed array
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename F, typename T, typename A>
	ZArray<T, A> Map(const ZArray<T, A>& _array, F _functor, size_t _start, size_t _end)
	{
		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Map - Cannot transform with end < start!");
		#endif

		ZArray<T, A> ret(&_array.Data()[start], end - start);

		for (size_t i = 0; i < end - start ; i++)
			_functor(ret.Data()[i]);

		return ret;
	}

	/*
	public ZArrayAlgo<F, T, A>::Map
	
	Maps the provided functor over the array, returning a transformed array and leaving the
	original array intact.

	@param F - the unary functor ([T&] -> void) to transform the array with
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to transform
	@param _functor - instance of the unary functor F to operate on 
	@return (ZArray<T, A>) the transformed array
	*/
	template <typename F, typename T, typename A>
	ZArray<T, A> Map(const ZArray<T, A>& _array, F _functor)
	{
		return Map(_array, _functor, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Max
	 
	Function to find the index of the first instance of the max value of an array.
	 
	@param _array - the array to find the max of
	@param _comparator - the comparator to use when determining maximum
	@param _start - the starting index (inclusive) to begin searching for the maximum from
	@param _end - the ending index (exclusive) for the search
	@return (size_t) - the index of the maximum value
	*/
	template <typename CF, typename T, typename A>
	size_t Max(const ZArray<T, A>& _array, CF _comparator, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
			ZSTL_ASSERT(start <= end, "ZArrayAlgo::Max - Cannot remove with end < start!");
			ZSTL_ASSERT(_array.Empty() == false, "ZArrayAlgo::Max - Cannot find min of empty array!\n");
		#endif

		size_t maxIndex = start;
		for (size_t i = start; i < end; i++)
		{
			if ( _comparator( _array[maxIndex], _array[i]) < 0)
				maxIndex = i;
		}

		return maxIndex;
	}

	/*
	public ZArrayAlgo<T, A>::Max
	 
	Function to find the index of the first instance of the max value of an array.
	 
	@param _array - the array to find the max of
	@return (size_t) - the index of the maximum value
	*/
	template <typename T, typename A>
	size_t Max(const ZArray<T, A>& _array)
	{
		return Max(_array, ZComparator<T>(), 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Max
	 
	Function to find the index of the first instance of the max value of an array.
	 
	@param _array - the array to find the max of
	@param _comparator - the comparator to use when determining maximum
	@return (size_t) - the index of the maximum value
	*/
	template <typename CF, typename T, typename A>
	size_t Max(const ZArray<T, A>& _array, CF _comparator)
	{
		return Max(_array, _comparator, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Max
	 
	Function to find the index of the first instance of the max value of an array.
	 
	@param _array - the array to find the max of
	@param _start - the starting index (inclusive) to begin searching for the maximum from
	@param _end - the ending index (exclusive) for the search
	@return (size_t) - the index of the maximum value
	*/
	template <typename T, typename A>
	size_t Max(const ZArray<T, A>& _array, size_t _start, size_t _end)
	{
		return Max(_array, ZComparator<T>(), _start, _end);
	}

	/*
	public ZArrayAlgo<T, A>::Min
	 
	Function to find the index of the first instance of the min value of an array.
	 
	@param _array - the array to find the min of
	@param _comparator - the comparator to use when determining minimum
	@param _start - the starting index (inclusive) to begin searching for the minimum from
	@param _end - the ending index (exclusive) for the search
	@return (size_t) - the index of the minimum value
	*/
	template <typename CF, typename T, typename A>
	size_t Min(const ZArray<T, A>& _array, CF _comparator, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
				ZSTL_ASSERT(start <= end, "ZArrayAlgo::Min - Cannot remove with end < start!");
				ZSTL_ASSERT(_array.Empty() == false, "ZArrayAlgo::Min - Cannot find min of empty array!\n");
		#endif

		size_t maxIndex = start;
		for (size_t i = start; i < end; i++)
		{
			if ( _comparator( _array[maxIndex], _array[i]) > 0)
				maxIndex = i;
		}

		return maxIndex;
	}

	/*
	public ZArrayAlgo<T, A>::Min
	 
	Function to find the index of the first instance of the min value of an array.
	 
	@param _array - the array to find the min of
	@return (size_t) - the index of the minimum value
	*/
	template <typename T, typename A>
	size_t Min(const ZArray<T, A>& _array)
	{
		return Min(_array, ZComparator<T>(), 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Min
	 
	Function to find the index of the first instance of the min value of an array.
	 
	@param _array - the array to find the min of
	@param _comparator - the comparator to use when determining minimum
	@return (size_t) - the index of the minimum value
	*/
	template <typename CF, typename T, typename A>
	size_t Min(const ZArray<T, A>& _array, CF _comparator)
	{
		return Min(_array, _comparator, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Min
	 
	Function to find the index of the first instance of the min value of an array.
	 
	@param _array - the array to find the min of
	@param _start - the starting index (inclusive) to begin searching for the minimum from
	@param _end - the ending index (exclusive) for the search
	@return (size_t) - the index of the minimum value
	*/
	template <typename T, typename A>
	size_t Min(const ZArray<T, A>& _array, size_t _start, size_t _end)
	{
		return Min(_array, ZComparator<T>(), _start, _end);
	}

	/*
	public ZArrayUtil::Remove

	Removes the first occurrence of the given element in the given range.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to remove elements from
	@param _value - the value to remove
	@param _start - the index to start at
	@param _end - the ending index (exclusive)
	@return (size_t) - the index at which the first occurrence was removed from, ZSTL::InvalidPos if no occurrence
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A>
	size_t Remove(ZArray<T, A>& _array, const T& _value, size_t _start, size_t _end) 
	{
		if (_start == _end)
			return ZSTL::InvalidPos;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Remove - Cannot remove with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
		{
			if (_array.Data()[i] == _value)
			{
				_array.Erase(i);
				return i;
			}
		}

		return ZSTL::InvalidPos;
	}

	/*
	public ZArrayUtil::Remove

	Removes the first occurrence of the given element in the array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to remove elements from
	@param _element - the element to remove
	@return (size_t) - the index at which the first occurrence was removed from, ZSTL::InvalidPos if no occurrence
	*/
	template <typename T, typename A>
	size_t Remove(ZArray<T, A>& _array, const T& _element) 
	{
		return Remove(_array, _element, 0, _array.Size());
	}

	/*
	public ZArrayUtil::RemoveAll

	Removes all occurrences of the given element in the given range.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to remove elements from
	@param _value - the value to remove
	@param _start - the index to start at
	@param _end - the ending index (exclusive)
	@return (size_t) - number of occurrences removed
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A>
	size_t RemoveAll(ZArray<T, A>& _array, const T& _value, size_t _start, size_t _end) 
	{ 
		if (_start == _end)
			return 0;

		size_t count = 0;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::RemoveAll - Cannot remove with end < start!");
		#endif

		for (size_t i = end; i > start; i--)
		{
			if (_array.Data()[i - 1] == _value)
			{
				_array.Erase(i - 1);
				count++;
			}
		}

		return count;
	}

	/*
	public ZArrayUtil::RemoveAll

	Removes all occurrences of the given element from the array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to remove elements from
	@param _value - the value to remove
	@return (size_t) - number of occurrences removed
	*/
	template <typename T, typename A>
	size_t RemoveAll(ZArray<T, A>& _array, const T& _value)
	{
		return RemoveAll(_array, _value, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<F, T, A>::RemoveIf
	
	Removes elements from the array in the given range when the provided unary functor evaluates 
	to true.
	
	@param F - unary functor ([const T&] -> bool) used to test elements
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to remove elements from
	@param _functor - functor instance of F used to evaluate elements
	@param _start - the index to start at
	@param _end - the ending index (exclusive)
	@return (size_t) - the number of elements removed
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename F, typename T, typename A>
	size_t RemoveIf(ZArray<T, A>& _array, F _functor, size_t _start, size_t _end)
	{
		if (_start == _end)
			return 0;

		size_t count = 0;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::RemoveIf - Cannot remove with end < start!");
		#endif

		size_t toCheck = end - start;
		size_t i = end-1;
		while (toCheck > 0)
		{
			if (_functor(_array.Data()[i]))
			{
				_array.Erase(i);
				count++;
			}
			i--;
			toCheck--;
		}

		return count;		
	}

	/*
	public ZArrayAlgo<F, T, A>::RemoveIf
	
	Removes elements from the array when the provided unary functor evaluates 
	to true.
	
	@param F - unary functor ([const T&] -> bool) used to test elements
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to remove elements from
	@param _functor - functor instance of F used to evaluate elements
	@return (size_t) - the number of elements removed
	*/
	template <typename F, typename T, typename A>
	size_t RemoveIf(ZArray<T, A>& _array, F _functor)
	{
		return RemoveIf(_array, _functor, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::RemoveUpTo
	
	Removes up to the provided number of occurrences of a value from the array in
	the specified range.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to remove elements from
	@param _value - the value to look for
	@param _count - the maximum number of times to remove the value
	@param _start - the index to start at
	@param _end - the ending index (exclusive)
	@return (size_t) - the number of occurrences removed
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A>
	size_t RemoveUpTo(ZArray<T, A>& _array, const T& _value, size_t _count, size_t _start, size_t _end)
	{
		if (_start == _end)
			return 0;

		size_t count = 0;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::RemoveUpTo - Cannot remove with end < start!");
		#endif

		for (size_t i = end - 1; i < end && i >= start&& count < _count; i--)
		{
			if (_array.Data()[i] == _value)
			{
				_array.Erase(i);
				count++;
			}
		}

		return count;
	}
	
	/*
	public ZArrayAlgo<T, A>::RemoveUpTo
	
	Removes up to the provided number of occurrences of a value from the array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to remove elements from
	@param _value - the value to look for
	@param _count - the maximum number of times to remove the value
	@return (size_t) - the number of occurrences removed
	*/
	template <typename T, typename A>
	size_t RemoveUpTo(ZArray<T, A>& _array, const T& _value, size_t _count)
	{
		return RemoveUpTo(_array, _value, _count, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Replace
	
	Finds and replaces all occurrences of the provided element with another in the given region.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to replace values in
	@param _value - the value to look for
	@param _newValue - the value to replace with
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the number of values replaced
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A>
	size_t Replace(ZArray<T, A>& _array, const T& _value, const T& _newValue, size_t _start, size_t _end)
	{
		if (_start == _end)
			return 0;

		size_t count = 0;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Replace - Cannot replace with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
		{
			if (_array.Data()[i] == _value)
			{
				_array.Data()[i] = _newValue;
				count++;
			}
		}

		return count;
	}

	/*
	public ZArrayAlgo<T, A>::Replace
	
	Finds and replaces all occurrences of the provided element with another.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to replace values in
	@param _value - the value to look for
	@param _newValue - the value to replace with
	@return (size_t) - the number of values replaced
	*/
	template <typename T, typename A>
	size_t Replace(ZArray<T, A>& _array, const T& _value, const T& _newValue)
	{
		return Replace(_array, _value, _newValue, 0, _array.Size());		
	}

	/*
	public ZArrayAlgo<F, T, A>::ReplaceIf
	
	Replaces values in the array when the provided unary functor evaluates to true within
	the given range.
	
	@param F - unary functor ([const &] -> bool) to use to evaluate elements 
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to replace elements in
	@param _functor - instance of unary functor F to use
	@param _newValue - the value to replace with
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the number of replaced elements
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename F, typename T, typename A>
	size_t ReplaceIf(ZArray<T, A>& _array, F _functor, const T& _newValue, size_t _start, size_t _end)
	{
		if (_start == _end)
			return 0;

		size_t count = 0;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::ReplaceIf - Cannot replace with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
		{
			if (_functor(_array.Data()[i]))
			{
				_array.Data()[i] = _newValue;
				count++;
			}
		}

		return count;
	}

	/*
	public ZArrayAlgo<F, T, A>::ReplaceIf
	
	Replaces values in the array when the provided unary functor evaluates to true.
	
	@param F - unary functor ([const &] -> bool) to use to evaluate elements 
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to replace elements in
	@param _functor - instance of unary functor F to use
	@param _newValue - the value to replace with
	@return (size_t) - the number of replaced elements
	*/
	template <typename F, typename T, typename A>
	size_t ReplaceIf(ZArray<T, A>& _array, F _functor, const T& _newValue)
	{
		return ReplaceIf(_array, _functor, _newValue, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Reverse
	
	Reverses an array in place between the given indices.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to reverse
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (void)
	*/
	template <typename T, typename A>
	void Reverse(ZArray<T, A>& _array, size_t _start, size_t _end)
	{
		if (_start == _end)
			return;

		T temp;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
			ZSTL_ASSERT(start <= end, "ZArrayAlgo::Reverse - Cannot reverse with end < start!");
		#endif

		if (start == end)
			return;

		// note that the assert above and the _start == _end protect this from crashing for start = end = 0
		for (size_t i = start, j = end - 1; i < j; i++, j--)
		{
			temp = _array.Data()[i];
			_array.Data()[i] = _array.Data()[j];
			_array.Data()[j] = temp;
		}
	}

	/*
	public ZArrayAlgo<T, A>::Reverse
	
	Reverses an array in place.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to reverse
	@return (void)
	*/
	template <typename T, typename A>
	void Reverse(ZArray<T, A>& _array)
	{
		Reverse(_array, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Rotate
	 
	Function to shift an array so that a given element is at the front.

	[ 0 1 2 3 4 5] --> [ 3 4 5 0 1 2 3]
			^ pivot point
	 
	@param _array - array to pivot
	@param _pivot - index to pivot (inclusive)	
	@param _start - starting index (inclusive) of array slice to rotate
	@param _end - ending index (exclusive) of array slice to rotate
	@return (void)
	@assert - if _pivot, _start, or _end out of bounds
			  if _pivot >= end
			  if _pivot < _start
	*/
	template <typename T, typename A>
	void Rotate(ZArray<T, A>& _array, size_t _pivot, size_t _start, size_t _end)
	{
		if (_start == _end)
			return;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t pivot = _array.BoundsCheck(_pivot, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
			ZSTL_ASSERT(start <= end, "ZArrayAlgo::Reverse - Cannot reverse with end < start!");
			ZSTL_ASSERT(pivot >= start, "ZArrayAlgo::Rotate - Cannot rotate with pivot before start!");
			ZSTL_ASSERT(pivot < end, "ZArrayAlgo::Rotate - Cannot rotate with pivot past end!");
		#endif

		ZArray<T,A> temp(_array.Size());
		
		// copy everything before the part to rotate
		for (size_t i = 0; i < _start; i++)
			temp.PushBack(_array.Data()[i]);

		// copy everything including and after the pivot
		for (size_t i = pivot; i < end ; i++)
			temp.PushBack(_array.Data()[i]);

		// copy everything up to the pivot
		for (size_t i = start; i < pivot; i++)
			temp.PushBack(_array.Data()[i]);

		// copy everything left after the end section
		for (size_t i = _end; i < _array.Size(); i++)
			temp.PushBack(_array.Data()[i]);

		_array.Copy(temp);
	}

	/*
	public ZArrayAlgo<T, A>::Rotate
	 
	Function to shift an array so that a given element is at the front.

	[ 0 1 2 3 4 5] --> [ 3 4 5 0 1 2 3]
			^ pivot point
	 
	@param _array - array to pivot
	@param _pivot - index to pivot (inclusive)	
	@return (void)
	@assert - if _pivot out of bounds
	*/
	template <typename T, typename A>
	void Rotate(ZArray<T, A>& _array, size_t _pivot)
	{
		return ZArrayAlgo::Rotate(_array, _pivot, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A, B>::SetIntersection
	 
	Function to compute the intersection of two arrays.

	[ A B C ] ^ [ C D E ] -> [ C ]
	 
	@param _array - first array
	@param _s1 - starting index (inclusive) of first array
	@param _e1 - ending index (exclusive) of first array
	@param _other - second array
	@param _s2 - starting index (inclusive) of second array
	@param _e2 - starting index (exclusive) of second array
	@return (ZArray<T,A>) - array with unique elements from the intersection of the two arrays
	@assert - if s1 > e1
			  if s2 > e2
			  if s1, e1, s2, e2 out of bounds
	*/
	template <typename T, typename A, typename B>
	ZArray<T, A> SetIntersection(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _other, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1 || _s2 == _e2)
			return ZArray<T,A>();

		const size_t s1 = _array.BoundsCheck(_s1, _array.Size());
		const size_t e1 = _array.BoundsCheck(_e1, _array.Size() + 1);	// 1 to allow indexing of end
		const size_t s2 = _other.BoundsCheck(_s2, _other.Size());
		const size_t e2 = _other.BoundsCheck(_e2, _other.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
				ZSTL_ASSERT(s1 <= e1, "ZArrayAlgo::SetUnion - Cannot accumulate with first list end < start!");
				ZSTL_ASSERT(s2 <= e2, "ZArrayAlgo::SetUnion - Cannot accumulate with second list end < start!");
		#endif

		ZArray<T,A> ret(e1-s1,e1-s1);
		Copy(ret, 0, e1-s1, _array, s1, e1);
		Unique(ret, 0, ret.Size());

		for (size_t i = ret.Size(); i > 0; i--)
		{
			if (Contains(_other, ret.Data()[(i-1)], s2, e2) == false)
				ret.Erase(i-1);
		}

		return ret;
	}

	/*
	public ZArrayAlgo<T, A, B>::SetIntersection
	 
	Function to compute the intersection of two arrays.

	[ A B C ] ^ [ C D E ] -> [ C ]
	 
	@param _array - first array
	@param _other - second array
	@return (ZArray<T,A>) - array with unique elements from the intersection of the two arrays
	*/
	template <typename T, typename A, typename B>
	ZArray<T, A> SetIntersection(const ZArray<T, A>& _array, const ZArray<T, B>& _other)
	{
		return SetIntersection(_array, 0, _array.Size(), _other, 0, _other.Size());
	}

	/*
	public ZArrayAlgo<T, A, B>::SetUnion
	 
	Function to compute the union of two arrays.

	[ A B C ] U [ C D E ] -> [ A B C D E]
	 
	@param _array - first array
	@param _s1 - starting index (inclusive) of first array
	@param _e1 - ending index (exclusive) of first array
	@param _other - second array
	@param _s2 - starting index (inclusive) of second array
	@param _e2 - starting index (exclusive) of second array
	@return (ZArray<T,A>) - array with unique elements from the union of the two arrays
	@assert - if s1 > e1
			  if s2 > e2
			  if s1, e1, s2, e2 out of bounds
	*/
	template <typename T, typename A, typename B>
	ZArray<T, A> SetUnion(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _other, size_t _s2, size_t _e2)
	{
		if (_s1 == _e1)
			return _other;
		else if (_s2 == _e2)
			return _array;

		ZArray<T,A> ret;

		const size_t s1 = _array.BoundsCheck(_s1, _array.Size());
		const size_t e1 = _array.BoundsCheck(_e1, _array.Size() + 1);	// 1 to allow indexing of end
		const size_t s2 = _other.BoundsCheck(_s2, _other.Size());
		const size_t e2 = _other.BoundsCheck(_e2, _other.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(s1 <= e1, "ZArrayAlgo::SetUnion - Cannot accumulate with first list end < start!");
		ZSTL_ASSERT(s2 <= e2, "ZArrayAlgo::SetUnion - Cannot accumulate with second list end < start!");
		#endif

		for (size_t i = _s1; i < _e1; i++)
		{
			if (Contains(ret, _array[i]) == false)
				ret.PushBack(_array[i]);
		}

		for (size_t i = _s2; i < _e2; i++)
		{
			if (Contains(ret, _other[i]) == false)
				ret.PushBack(_other[i]);
		}

		return ret;
	}
	
	/*
	public ZArrayAlgo<T, A, B>::SetUnion
	 
	Function to compute the union of two arrays.

	[ A B C ] U [ C D E ] -> [ A B C D E]
	 
	@param _array - first array
	@param _other - second array
	@return (ZArray<T,A>) - array with unique elements from the union of the two arrays
	*/
	template <typename T, typename A, typename B>
	ZArray<T, A> SetUnion(const ZArray<T, A>& _array, const ZArray<T, B>& _other)
	{
		return SetUnion(_array, 0, _array.Size(), _other, 0, _other.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Slice
	
	Removes a section of the provided array and returns a new array containing the 
	data.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to excise data from
	@param _start - the index to start at
	@param _end - the ending index (exclusive)
	@return (ZArray<T, A>) - the array slice
	@assert - if _end < _start
			  if _start or _end out of bounds
	*/
	template <typename T, typename A>
	ZArray<T, A> Slice(ZArray<T, A>& _array, size_t _start, size_t _end)
	{
		if (_start == _end)
			return ZArray<T, A>();

		ZArray<T, A> ret(_array, _start, _end);

		_array.Erase(_start, _end);

		return ret;
	}

	/*
	public ZArrayAlgo<CF, AF, T, A>::Sort

	Sorts an array in place using the provided comparator and algorithm between the given indices.

	@param CF - the comparator functor to use [(const T&, const T&) -> int] that compares values
				returns negative value if first value < second value
				returns positive value if first value > second value
				returns 0 if first value == second value
	@param AF - the algorithm functor to use [(CF&, T*, size_t) -> void] that sorts the array
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to sort
	@param _comparator - the comparator used to sort the contained values
	@param _algorithm - the array sort algorithm to use to sort the array
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (void)
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename CF, typename AF, typename T, typename A>
	void Sort(ZArray<T, A>& _array, CF _comparator, AF _algorithm, size_t _start, size_t _end)
	{ 
		if (_start == _end)
			return;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Sort - Cannot sort with end < start!");
		#endif

		const size_t sliceSize = end - start;

		_algorithm(_comparator, &_array.Data()[start], sliceSize);
	}

	/*
	public ZArrayAlgo<T, A>::Sort

	Sorts an array in place using the default comparator (uses operator <) and a non-stable 
	quick sort.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to sort
	@return (void)
	*/
	template <typename T, typename A>
	void Sort(ZArray<T, A>& _array)
	{
		ZComparator<T> comparator;
		ZArrayQuickSort<T> algorithm;

		Sort(_array, comparator, algorithm, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Sort

	Sorts an array in place using the default comparator (uses operator <) and 
	uses a non-stable quick sort between the given indices.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to sort
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (void)
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A>
	void Sort(ZArray<T, A>& _array, size_t _start, size_t _end)
	{
		ZComparator<T> comparator;
		ZArrayQuickSort<T> algorithm;

		Sort(_array, comparator, algorithm, _start, _end);
	}

	/*
	public ZArrayAlgo<CF, T, A>::Sort

	Sorts an array in place using the provided comparator and a non-stable quick sort algorithm.

	@param CF - the comparator functor to use [(const T&, const T&) -> int] that compares values
				returns negative value if first value < second value
				returns positive value if first value > second value
				returns 0 if first value == second value
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to sort
	@param _comparator - the comparator used to sort the contained values
	@return (void)
	*/
	template <typename CF, typename T, typename A>
	void Sort(ZArray<T, A>& _array, CF _comparator) 
	{ 
		ZArrayQuickSort<T> algorithm;

		Sort(_array, _comparator, algorithm, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Split
	
	Splits a range of the array into a set of arrays at locations that have a value contained in the provided
	set of delimiters.  The delimiters are consumed from the array in the process.  The array is split
	only up to the provided number of times.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the delimiter array
	@param _array - the array to split
	@param _delimiters - the set of delimiter values to split on
	@param _count - the maximum number of times to split the array
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (ZArray< ZArray<T, A> >) - array of subsections of the original array
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A, typename B>
	ZArray< ZArray<T, A> > Split(const ZArray<T, A>& _array, const ZArray<T, B>& _values, size_t _count, size_t _start, size_t _end)
	{
		ZArray< ZArray<T, A> > sections(_array.Size() + 1);

		if (_start == _end)
			return sections;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Split - Cannot split with end < start!");
		#endif

		//Location of the previous delimiter
		size_t previous = start;

		for (size_t i = start; i < end; i++)
		{
			if (sections.Size() == _count)	//Break out if we have reached our count limit
				break;

			for (size_t j = 0; j < _values.Size(); j++)
			{
				if ( _array.Data()[i] == _values.Data()[j] )
				{
					if (i  > previous)	//If we have more than just one of the delimiters
						sections.PushBack( ZArray<T, A>(_array, previous, i) );

					previous = i + 1;
					break;
				}
			}
		}

		if (previous != end)	//Add the final section
			sections.PushBack( ZArray<T, A>(_array, previous, end) );

		return sections;
	}

	/*
	public ZArrayAlgo<T, A>::Split
	
	Splits the array into a set of arrays at locations that have a value contained in the provided
	set of delimiters.  The delimiters are consumed from the array in the process.  The array is split
	only up to the provided number of times.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the delimiter array
	@param _array - the array to split
	@param _delimiters - the set of delimiter values to split on
	@param _count - the maximum number of times to split the array
	@return (ZArray< ZArray<T, A> >) - array of subsections of the original array
	*/
	template <typename T, typename A, typename B>
	ZArray< ZArray<T, A> > Split(const ZArray<T, A>& _array, const ZArray<T, B>& _delimiters, size_t _count)
	{
		return Split(_array, _delimiters, _count, 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A>::Split
	
	Splits the array into a set of arrays at locations that have a value contained in the provided
	set of delimiters.  The delimiters are consumed from the array in the process.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the delimiter array
	@param _array - the array to split
	@param _delimiters - the set of delimiter values to split on
	@return (ZArray< ZArray<T, A> >) - array of subsections of the original array
	*/
	template <typename T, typename A, typename B>
	ZArray< ZArray<T, A> > Split(const ZArray<T, A>& _array, const ZArray<T, B>& _delimiters)
	{
		return Split(_array, _delimiters, _array.Size(), 0, _array.Size());
	}

	/*
	public ZArrayAlgo<T, A, B>::StartsWith

	Used to determine if the given region of the provided array starts with the contents of the
	given region of the other array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the array to check
	@param _s1 - the starting index to check against
	@param _e1 - the ending index to check against (exclusive)
	@param _other - the array to check from
	@param _s2 - the starting index to check from
	@param _e2 - the ending index to check to (exclusive)
	@return (bool) - true if the array starts with the provided values, false otherwise
	@assert - if _s1, _e1, _s2, or _e2 out of bounds
			  if _e1 < _s1 or _e2 < _s2
	*/
	template <typename T, typename A, typename B>
	bool StartsWith(const ZArray<T, A>& _array, size_t _s1, size_t _e1, const ZArray<T, B>& _other, size_t _s2, size_t _e2)
	{
		return FindSub(_array, _s1, _e1, _other, _s2, _e2) == _s1;
	}

	/*
	public ZArrayAlgo<T, A, B>::StartsWith

	Used to determine if the provided array starts with the contents of the other array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param B - the allocator type of the other array
	@param _array - the array to check
	@param _other - the array to check from
	@return (bool) - true if the array ends with the provided values, false otherwise
	*/
	template <typename T, typename A, typename B>
	bool StartsWith(const ZArray<T, A>& _array, const ZArray<T, B>& _other)
	{
		return StartsWith(_array, 0, _array.Size(), _other, 0, _other.Size());
	}

	/*
	public ZArrayAlgo<V, T, A>::Sum
	
	Returns the result of summing (via operator +) all the elements within a range in the array.
	
	@param V - the initial value type and sum type
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to accumulate on
	@param _initialValue - the initial value for the accumulation
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (V) - the accumulated value
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename V, typename T, typename A>
	V Sum(const ZArray<T, A>& _array, V _initialValue, size_t _start, size_t _end)
	{
		if (_start == _end)
			return _initialValue;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Accumulate - Cannot accumulate with end < start!");
		#endif

		for (size_t i = start; i < end; i++)
			_initialValue = _initialValue + _array.Data()[i];

		return _initialValue;
	}

	/*
	public ZArrayAlgo<V, T, A>::Sum
	
	Returns the result of summing (via operator +) all the elements in the array.
	
	@param V - the initial value type and sum type
	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to accumulate on
	@param _initialValue - the initial value for the accumulation
	@return (V) - the accumulated value
	*/
	template <typename V, typename T, typename A>
	V Sum(const ZArray<T, A>& _array, V _initialValue)
	{
		return Sum(_array, _initialValue, 0, _array.Size());
	}

	/*
	public ZArrayUtil::SwapElements

	Swaps the values in two indices to this array.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to swap elements in
	@param _i - the first value
	@param _j - the second value
	@return (void)
	@assert - if _i or _j are out of bounds
	*/
	template <typename T, typename A>
	void SwapElements(ZArray<T, A>& _array, size_t _start, size_t _end)
	{
		if (_start == _end)
			return;

		const size_t i = _array.BoundsCheck(_start, _array.Size());
		const size_t j = _array.BoundsCheck(_end, _array.Size());

		T temp = _array.Data()[i];
		_array.Data()[i] = _array.Data()[j];
		_array.Data()[j] = temp;
	}

	/*
	public ZArrayAlgo<T, A>::Unique
	
	Makes all the values in an array unique within the given range.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to make full of unique values
	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (size_t) - the number of values removed
	@assert - if _end < _start
			  if _start or _end out of bounds 
	*/
	template <typename T, typename A>
	size_t Unique(ZArray<T, A>& _array, size_t _start, size_t _end)
	{
		if (_start == _end)
			return 0;

		const size_t start = _array.BoundsCheck(_start, _array.Size());
		const size_t end = _array.BoundsCheck(_end, _array.Size() + 1);	// 1 to allow indexing of end

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZArrayAlgo::Unique - Cannot make unique with end < start!");
		#endif

		size_t removed = 0;

		for (size_t i = start; i < end - removed; i++)
		{
			for (size_t j = end - removed - 1; j > i; j--)
			{
				if (_array.Data()[i] == _array.Data()[j])
				{
					removed++;
					_array.Erase(j);
				}
			}
		}
		return removed;		
	}

	/*
	public ZArrayAlgo<T, A>::Unique
	
	Makes all the values in an array unique.

	@param T - the type held by the array
	@param A - the allocator type of the array
	@param _array - the array to make full of unique values
	@return (size_t) - the number of values removed
	*/
	template <typename T, typename A>
	size_t Unique(ZArray<T, A>& _array)
	{
		return Unique(_array, 0, _array.Size());
	}
}

#endif
