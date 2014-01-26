/*
	ZRingBuffer.hpp
	Author: Chris Ertel <crertel@762studios.com>
			James Russell <jcrussell@762studios.com>
	Created: 12/01/2011

	Purpose: 

	Templated array-backed resizable circular buffer implementation.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _ZRINGBUFFER_HPP
#define _ZRINGBUFFER_HPP

#include <ZSTL/ZArray.hpp>

//Default local storage for ring buffer
#ifndef ZRINGBUFFER_DEFAULT_LOCAL_STORAGE
#define ZRINGBUFFER_DEFAULT_LOCAL_STORAGE (100)
#endif

//Default capacity for ring buffer
#ifndef ZRINGBUFFER_DEFAULT_CAPACITY
#define ZRINGBUFFER_DEFAULT_CAPACITY (100)
#endif

//Resize factor for ZRingBuffer when operations are performed that require an increase in capacity
#ifndef ZRINGBUFFER_CAPACITY_RESIZE_FACTOR
#define ZRINGBUFFER_CAPACITY_RESIZE_FACTOR (2.0)
#endif

////////////////////////////////
/* ZRingBuffer Implementation */
////////////////////////////////

//Forward declaration of ZRingBuffer
template <typename T, typename P, typename A>
class ZRingBuffer;

/* 
Forward Declaration of ZRingBuffer Method Implementation Structures 

Existence of these structures allows for template specialization of 
individual methods of the ZRingBuffer class.  In order to specialize
a single method of ZRingBuffer, simply specialize the corresponding 
method implementation structure.
*/

//Forward declaration of ZRingBuffer<T, P, A>::At struct
template <typename T, typename P, typename A>
struct ZRingBuffer_AtImpl {
	inline static T& Call( const ZRingBuffer<T, P, A>& _self, const size_t _index);
};

//Forward declaration of ZRingBuffer<T, P, A>::Back struct
template <typename T, typename P, typename A>
struct ZRingBuffer_BackImpl {
	inline static T& Call( const ZRingBuffer<T, P, A>& _self );
};

//Forward declaration of ZRingBuffer<T, P, A>::Capacity struct
template <typename T, typename P, typename A>	
struct ZRingBuffer_CapacityImpl {
	inline static size_t Call( const ZRingBuffer<T, P, A>& _self );
};

//Forward declaration of ZRingBuffer<T, P, A>::Clear struct
template <typename T, typename P, typename A>
struct ZRingBuffer_ClearImpl {
	inline static void Call( ZRingBuffer<T, P, A>& _self );
	inline static void Call( ZRingBuffer<T, P, A>& _self, size_t _newCapacity );
};

//Forward declaration of ZRingBuffer<T, P, A>::Copy struct
template <typename T, typename P, typename A>
struct ZRingBuffer_CopyImpl {
	template <typename O, typename B> inline static void Call( ZRingBuffer<T, P, A>& _self, const ZRingBuffer<T, O, B>& _other );
};

//Forward declaration of ZRingBuffer<T, P, A>::Empty struct
template <typename T, typename P, typename A>
struct ZRingBuffer_EmptyImpl {
	inline static bool Call( const ZRingBuffer<T, P, A>& _self );
};

//Forward declaration of ZRingBuffer<T, P, A>::Equals struct
template <typename T, typename P, typename A>
struct ZRingBuffer_EqualsImpl {
	template <typename O, typename B> inline static bool Call( const ZRingBuffer<T, P, A>& _self, const ZRingBuffer<T, O, B>& _other );
};

//Forward declaration of ZRingBuffer<T, P, A>::Erase struct
template <typename T, typename P, typename A>
struct ZRingBuffer_EraseImpl {
	inline static T Call( ZRingBuffer<T, P, A>& _self, size_t _index);
	inline static void Call( ZRingBuffer<T, P, A>& _self, size_t _i, size_t _j );
};

//Forward declaration of ZRingBuffer<T, P, A>::Front struct
template <typename T, typename P, typename A>
struct ZRingBuffer_FrontImpl {
	inline static T& Call( const ZRingBuffer<T, P, A>& _self );
};

//Forward declaration of ZRingBuffer<T, P, A>::Full struct
template <typename T, typename P, typename A>	
struct ZRingBuffer_FullImpl {
	inline static bool Call( const ZRingBuffer<T, P, A>& _self );
};

//Forward declaration of ZRingBuffer<T, P, A>::Insert struct
template <typename T, typename P, typename A>
struct ZRingBuffer_InsertImpl {
	inline static void Call ( ZRingBuffer<T, P, A>& _self, size_t _index, const T& _value, size_t _count );
	template <typename B> inline static void Call( ZRingBuffer<T, P, A>& _self, size_t _index, const ZArray<T, B>& _array, size_t _start, size_t _count );
	template <typename O, typename B> inline static void Call( ZRingBuffer<T, P, A>& _self, size_t _index, const ZRingBuffer<T, O, B>& _other, size_t _start, size_t _count );
};

//Forward declaration of ZRingBuffer<T, P, A>::PopBack struct
template <typename T, typename P, typename A>
struct ZRingBuffer_PopBackImpl {
	inline static T Call( ZRingBuffer<T, P, A>& _self );
};

//Forward declaration of ZRingBuffer<T, P, A>::PopFront struct
template <typename T, typename P, typename A>
struct ZRingBuffer_PopFrontImpl {
	inline static T Call( ZRingBuffer<T, P, A>& _self );
};

//Forward declaration of ZRingBuffer<T, P, A>::PushBack struct
template <typename T, typename P, typename A>
struct ZRingBuffer_PushBackImpl {
	inline static void Call( ZRingBuffer<T, P, A>& _self, const T& _value );
};

//Forward declaration of ZRingBuffer<T, P, A>::PushFront struct
template <typename T, typename P, typename A>
struct ZRingBuffer_PushFrontImpl {
	inline static void Call( ZRingBuffer<T, P, A>& _self, const T& _value );
};

//Forward declaration of ZRingBuffer<T, P, A>::Reserve struct
template <typename T, typename P, typename A>	
struct ZRingBuffer_ReserveImpl {
	inline static void Call( ZRingBuffer<T, P, A>& _self, size_t _newCapacity );
};

//Forward declaration of ZRingBuffer<T, P, A>::Size struct
template <typename T, typename P, typename A>	
struct ZRingBuffer_SizeImpl {
	inline static size_t Call( const ZRingBuffer<T, P, A>& _self );
};

//Forward declaration of ZRingBuffer<T, P, A>::TryPushBack struct
template <typename T, typename P, typename A>
struct ZRingBuffer_TryPushBackImpl {
	inline static bool Call( ZRingBuffer<T, P, A>& _self, const T& _value );
};

//Forward declaration of ZRingBuffer<T, P, A>::TryPushFront struct
template <typename T, typename P, typename A>
struct ZRingBuffer_TryPushFrontImpl {
	inline static bool Call( ZRingBuffer<T, P, A>& _self, const T& _value );
};

/////////////////////////////////////////
/* ZRingBuffer Overflow Policy Structs */
/////////////////////////////////////////

//When used for template parameter P, then overflow is never checked for
struct ZRingBuffer_OverflowUnsafe {};

//When used for template parameter P, then overflow causes an assert 
struct ZRingBuffer_OverflowAssert {};

//When used for template parameter P, then overflow is ignored and the new element is dropped
struct ZRingBuffer_OverflowIgnore {};

//When used for template parameter P, then overflow causes the front element to be dropped
struct ZRingBuffer_OverflowDropFront {};

//When used for template parameter P, then overflow causes the back element to be dropped
struct ZRingBuffer_OverflowDropBack {};

//When used for template parameter P, then overflow causes the current element to be overwritten
struct ZRingBuffer_OverflowOverwrite {};

//When used for template parameter P, then overflow evicts the first element (PushBack) or last element (PushFront)
struct ZRingBuffer_OverflowEvict {};

//When used for template parameter P, then overflow causes the storage to grow
struct ZRingBuffer_OverflowGrow {};

/////////////////////////////
/* ZRingBuffer Declaration */
/////////////////////////////

/*
Templated array-backed circular buffer implementation.

The template parameter T is the type contained in the buffer.

The template parameter P is the overflow policy for the ring buffer.  Overflow only occurs during
push and pop operations - other methods of adding (such as insert) to the ring buffer will always grow the
buffer.

The template parameter A is the allocator that will be used by the underlying array.
*/
template <typename T, typename P = ZRingBuffer_OverflowGrow, typename A = ZArrayAllocator<T, ZRINGBUFFER_DEFAULT_LOCAL_STORAGE> >
class ZRingBuffer
{
friend struct ZRingBuffer_AtImpl<T, P, A>;
friend struct ZRingBuffer_BackImpl<T, P, A>;
friend struct ZRingBuffer_CapacityImpl<T, P, A>;
friend struct ZRingBuffer_ClearImpl<T, P, A>;
friend struct ZRingBuffer_CopyImpl<T, P, A>;
friend struct ZRingBuffer_EmptyImpl<T, P, A>;
friend struct ZRingBuffer_EqualsImpl<T, P, A>;
friend struct ZRingBuffer_EraseImpl<T, P, A>;
friend struct ZRingBuffer_FrontImpl<T, P, A>;
friend struct ZRingBuffer_FullImpl<T, P, A>;
friend struct ZRingBuffer_InsertImpl<T, P, A>;
friend struct ZRingBuffer_PopBackImpl<T, P, A>;
friend struct ZRingBuffer_PopFrontImpl<T, P, A>;
friend struct ZRingBuffer_PushBackImpl<T, P, A>;
friend struct ZRingBuffer_PushFrontImpl<T, P, A>;
friend struct ZRingBuffer_ReserveImpl<T, P, A>;
friend struct ZRingBuffer_SizeImpl<T, P, A>;
friend struct ZRingBuffer_TryPushFrontImpl<T, P, A>;
friend struct ZRingBuffer_TryPushBackImpl<T, P, A>;

protected:
	//Backing array of the ring buffer
	ZArray<T, A> Buffer;

	//Current number of elements in the ring buffer
	size_t BufferSize;

	//Index into the backing array of the front element of the ring buffer
	size_t FrontIndex;

	//Index into the backing array of the back element of the ring buffer
	size_t BackIndex;

	//Aligns the buffer with front index at zero
	inline void AlignBuffer()
	{
		//If FrontIndex is set to zero, then we are aligned
		if (FrontIndex == 0)
			return;

		ZArray<T, A> copy(Buffer.Capacity());

		copy.Resize(Buffer.Size());

		for (size_t num = 0; num < (size_t)BufferSize; num++)
			copy.Data()[num] = At(num);

		Buffer = copy;
		FrontIndex = 0;
		BackIndex = BufferSize - 1;
	}
	
	//Integrity Check
	inline void CheckIntegrity() const
	{
		#if ZSTL_CHECK_INTEGRITY
		ZSTL_ASSERT(FrontIndex < Buffer.Size(), "ZRingBuffer Error: Front index exceeds capacity!");
		ZSTL_ASSERT(BackIndex < Buffer.Size(), "ZRingBuffer Error: Back index exceeds capacity!");
		#endif
	}

	//Decrements the BackIndex (decreases size)
	inline ZRingBuffer<T, P, A>& DecrementBack()
	{
		//The back index does not move if we remove the last element
		if (BufferSize != 1)
		{
			if (BackIndex == 0)
				BackIndex = Buffer.Size() - 1;
			else
				BackIndex--;
		}

		BufferSize--;

		return *this;
	}

	//Decrements the FrontIndex (increases size)
	inline ZRingBuffer<T, P, A>& DecrementFront()
	{
		//The first element push does not require moving the back index
		if (BufferSize != 0)
		{
			if (FrontIndex == 0)
				FrontIndex = Buffer.Size() - 1;
			else
				FrontIndex--;
		}

		BufferSize++;

		return *this;
	}

	//Increments the BackIndex (increases size)
	inline ZRingBuffer<T, P, A>& IncrementBack()
	{
		//The first element push does not require moving the back index
		if (BufferSize != 0)
		{
			if (BackIndex == Buffer.Size() - 1)
				BackIndex = 0;
			else
				BackIndex++;
		}

		BufferSize++;

		return *this;
	}

	//Increments the FrontIndex (decreases size)
	inline ZRingBuffer<T, P, A>& IncrementFront()
	{
		//The front index does not move if we remove the last element
		if (BufferSize != 1)
		{
			if (FrontIndex == Buffer.Size() - 1)
				FrontIndex = 0;
			else
				FrontIndex++;
		}

		BufferSize--;

		return *this;
	}

public:
	/*	 
	Default constructor.  Initializes an empty ring buffer with a default capacity.
	*/
	ZRingBuffer()
		: Buffer(ZRINGBUFFER_DEFAULT_CAPACITY), 
		  BufferSize(0), FrontIndex(0), BackIndex(0) 
	{ Buffer.Resize(ZRINGBUFFER_DEFAULT_CAPACITY); }

	/*	 
	Parameterized constructor for ZRingBuffer. Initializes empty, but allows specification of capacity.
	 
	@param _capacity - initial capacity for the ring buffer
	*/
	ZRingBuffer(size_t _capacity)
		: Buffer(_capacity), 
		  BufferSize(0), FrontIndex(0), BackIndex(0) 
	{ Buffer.Resize(_capacity); }

	/*	 
	Parameterized constructor to use for ZRingBuffer. Allows default assignment of 
	contained elements.  The starting size of the buffer is equal to the array size.  The
	starting capacity of the buffer is equal to the array capacity.
	 
	@param _storage - ZArray containing default set of elements
	*/
	ZRingBuffer(const ZArray<T, A>& _storage)
		: Buffer(_storage), 
		  BufferSize(_storage.Size()), FrontIndex(0), BackIndex(_storage.Size() - 1) 
	{ Buffer.Resize(_storage.Capacity()); }

	/*
	Destructor.
	*/
	~ZRingBuffer() { }

	/*
	[] Operator overload.  Gets a value from the buffer.  Equivalent to
	a call to At.

	@param _index - the integer index into the buffer
	@return (T&) - reference to a value contained in the buffer
	*/
	T& operator [] (const size_t _index) const
		{ return At(_index); }

	/*
	= Operator overload.  Sets this buffer equal to the other by making
	a copy.  Equivalent to a call to Copy.
	
	Even though the code is the same as the more generic version below,
	this is needed to prevent a default assignment operator from being constructed.

	@param _other - the buffer to be set equal to
	@return (void)
	*/
	ZRingBuffer<T, P, A>& operator = (const ZRingBuffer<T, P, A>& _other) 
		{ Copy(_other); return *this; }

	/*
	= Operator overload.  Sets this buffer equal to the other by making
	a copy.  Equivalent to a call to Copy.

	@param _other - the buffer to be set equal to
	@return (void)
	*/
	template <typename B>
	ZRingBuffer<T, P, A>& operator = (const ZRingBuffer<T, B>& _other)
		{ Copy(_other); return *this; }

	/*
	== Operator overload.  Performs an element comparison on the two buffers.
	Equivalent to a call to Equals.

	@param B - the type of allocator for the other buffer
	@param _other - the buffer to compare to
	@return (bool) - true if this buffer is equivalent to the other
	*/
	template <typename B>
	bool operator == (const ZRingBuffer<T, B>& _other) const 
		{ return Equals(_other); }
	
	/*
	!= Operator overload.  Performs an element comparison on the two buffers.
	Equivalent to a call to !Equals.

	@param B - the type of the allocator for the other buffer
	@param _other - the buffer to compare to
	@return (bool) - false if this buffer is equivalent to the other
	*/
	template <typename B>
	bool operator != (const ZRingBuffer<T, B>& _other) const 
		{ return !Equals(_other); }

	/*
	public ZRingBuffer<T, P, A>::Actual

	Function that gives an actual index into the underlying array given an index into
	the ring buffer.  Will also perform bounds checking and assert if out of bounds.

	@param _index - index
	@return (size_t) - unsigned index into the backing array
	*/
	size_t ActualIndex(size_t _index) const
	{
		size_t index;

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(_index < BufferSize, "ZRingBuffer: Out of bounds access!");
		#endif

		index = ((size_t)_index + FrontIndex) % Buffer.Size();

		#if !ZSTL_DISABLE_RUNTIME_CHECKS

			if (FrontIndex < BackIndex)
			{
				ZSTL_ASSERT(((index >= FrontIndex) && (index <= BackIndex)), "ZRingBuffer: Out of bounds access!");
			}
			else //FrontIndex >= BackIndex
			{
				ZSTL_ASSERT(((index >= FrontIndex) || (index <= BackIndex)), "ZRingBuffer: Out of bounds access!");
			}
		
		#endif

		return index;
	}

	/*
	public ZRingBuffer<T, P, A>::Allocator

	Gets a reference to the allocator used by this buffer.

	@return (A&) - the allocator used by this buffer
	*/
	A& Allocator()
		{ return Buffer.Allocator(); }

	/*
	public ZRingBuffer<T, P, A>::Array

	Gets a reference to the array that backs this buffer.  The elements are not guaranteed to
	be stored in order.

	@return (ZArray<T, P, A>&) - the array backing this buffer	
	*/
	const ZArray<T, A>& Array() const
		{ return Buffer; }

	/*
	public ZRingBuffer<T, P, A>::At

	Gets a reference to the element indexed by the provided index.  The index 0 refers
	to the front element in the buffer, the index 1 the following, and so on up to the back
	element.  This allows random access of the buffer, though more overhead is involved than
	random access of ZArray.

	@param _index - index into the buffer
	@return (T&) - element at the indexed location
	@assert - if _index is out of bounds
	*/
	T& At(const size_t _index) const
		{ return ZRingBuffer_AtImpl<T, P, A>::Call(*this, _index); }

	/*
	public ZRingBuffer<T, P, A>::Back
	 
	Gets a reference to the back element of the buffer.
	 
	@return (T&) - reference to back element in buffer
	@assert - if the buffer is empty
	*/
	T& Back() const
		{ return ZRingBuffer_BackImpl<T, P, A>::Call(*this); }

	/*
	public ZRingBuffer<T, P, A>::Capacity
	 
	Gets the capacity of the ring buffer, which is the maximum number of elements that
	can be stored before a resize is required.
	 
	@return (size_t) - the capacity of the ring buffer
	*/
	size_t Capacity() const
		{ return ZRingBuffer_CapacityImpl<T, P, A>::Call(*this); }

	/*
	public ZRingBuffer<T, P, A>::Clear

	Clears out all elements from the buffer, keeping the currently allocated storage.

	@return (void)
	*/
	void Clear() 
		{ ZRingBuffer_ClearImpl<T, P, A>::Call(*this); }

	/*
	public ZRingBuffer<T, P, A>::Clear

	Clears out all elements from the buffer and will ensure that the capacity is
	at least as much as the provided value.  Reallocates the internal storage if necessary.

	@param _newCapacity - the new capacity value
	@return (void)
	*/
	void Clear(size_t _newCapacity) 
		{ ZRingBuffer_ClearImpl<T, P, A>::Call(*this, _newCapacity); }

	/*
	public ZRingBuffer<T, P, A>::Copy

	Copies the contents of another buffer into this one.

	@param O - overflow policy for the other buffer
	@param B - allocator type for the other buffer
	@param _other - the ring buffer to copy
	@return (void)
	*/
	template <typename O, typename B>
	void Copy(const ZRingBuffer<T, O, B>& _other)
		{ ZRingBuffer_CopyImpl<T, P, A>::template Call<O, B>(*this, _other); }

	/*
	public ZRingBuffer<T, P, A>::Empty
	 
	Tests whether or not the ring buffer is empty.
	 
	@return (bool) - true is empty, false if not empty
	*/
	bool Empty() const
		{ return ZRingBuffer_EmptyImpl<T, P, A>::Call(*this); }

	/*
	public ZRingBuffer<T, P, A>::Equals

	Tests whether or not this buffer is equivalent to another.  The two are considered 
	equivalent if they contain equivalent elements in the same order.

	@param O - overflow policy for the other buffer
	@param B - allocator type for the other buffer
	@param _other - the other buffer
	@return (bool) - true if equal, false otherwise
	*/
	template <typename O, typename B>
	bool Equals(const ZRingBuffer<T, O, B>& _other) 
		{ return ZRingBuffer_EqualsImpl<T, P, A>::template Call<O, B>(*this, _other); }

	/*
	public ZRingBuffer<T, P, A>::Erase

	Erases a value present in the buffer at the given index.

	@param _index - the index at which to remove the element from the array
	@return (T) - the element removed
	@assert - if the index is out of range
	*/
	T Erase(size_t _index) 
		{ return ZRingBuffer_EraseImpl<T, P, A>::Call(*this, _index); }

	/*
	public ZRingBuffer<T, P, A>::Erase

	Erase function.  Removes elements from the buffer between the given indices.

	@param _i - first index
	@param _j - second index (exclusive)
	@return (void)
	@assert - if either index is out of range or if _j < _i
	*/
	void Erase(size_t _i, size_t _j)
		{ ZRingBuffer_EraseImpl<T, P, A>::Call(*this, _i, _j); }

	/*
	public ZRingBuffer<T, P, A>::Front
	 
	Gets a reference to the front element of the buffer.
	 
	@return (T&) - reference to front element in buffer.
	@assert - if the buffer is empty
	*/
	T& Front() const
		{ return ZRingBuffer_FrontImpl<T, P, A>::Call(*this); }

	/*
	public ZRingBuffer<T, P, A>::Full
	 
	Tests whether or not the ring buffer is full.  If full, the overflow policy
	determines what is done whenever any operation that adds elements to the 
	buffer.
	 
	@return (bool) - true if full, false if not full
	*/
	bool Full() const
		{ return ZRingBuffer_FullImpl<T, P, A>::Call(*this); }

	/*
	public ZRingBuffer<T, P, A>::Insert

	Inserts the given value at the specified location.  Ring buffers are 
	not well suited to perform this operation quickly, so use sparingly.

	@param _index - the index at which to perform the insertion
	@param _value - the value to insert
	@return (void)
	@assert - if _index is out of range
	*/
	void Insert(size_t _index, const T& _value) 
		{ ZRingBuffer_InsertImpl<T, P, A>::Call(*this, _index, _value, 1); }

	/*
	public ZRingBuffer<T, P, A>::Insert

	Inserts the given value at the specified location.  Ring buffers are 
	not well suited to perform this operation quickly, so use sparingly.

	@param _index - the index at which to perform the insertion
	@param _value - the value to insert
	@param _count - the number of times to insert _value
	@return (void)
	@assert - if _index is out of range
	*/
	void Insert(size_t _index, const T& _value, size_t _count) 
		{ ZRingBuffer_InsertImpl<T, P, A>::Call(*this, _index, _value, _count); }
	
	/*
	public ZRingBuffer<T, P, A>::Insert

	Inserts the given array at the specified location.  Copies
	the values into the buffer.  Ring buffers are not well suited 
	to perform this operation quickly, so use sparingly.

	@param _index - the index at which to perform the insertion
	@param _array - the array to insert
	@return (void)
	@assert - if _index is out of range
	*/
	template <typename B>
	void Insert(size_t _index, const ZArray<T, B>& _array)
		{ ZRingBuffer_InsertImpl<T, P, A>::template Call<B>(*this, _index, _array, 0, _array.Size()); }
	
	/*
	public ZRingBuffer<T, P, A>::Insert

	Inserts data from the given array at the specified location.  Copies
	the values into the buffer.  Ring buffers are not well suited 
	to perform this operation quickly, so use sparingly.

	@param _index - the index at which to perform the insertion
	@param _array - the array to insert
	@param _start - the index to start getting values from
	@param _count - the number of values to get
	@return (void)
	@assert - if _index is out of range
	*/
	template <typename B>
	void Insert(size_t _index, const ZArray<T, B>& _array, size_t _start, size_t _count)
		{ ZRingBuffer_InsertImpl<T, P, A>::template Call<B>(*this, _index, _array, _start, _count); }

	/*
	public ZRingBuffer<T, P, A>::Insert

	Inserts the given buffer at the specified location.  Copies
	the values into the buffer.  Ring buffers are not well suited 
	to perform this operation quickly, so use sparingly.

	@param O - overflow policy for the other buffer
	@param B - allocator type for the other buffer
	@param _index - the index at which to perform the insertion
	@param _other - the buffer to insert
	@return (void)
	@assert - if _index is out of range
	*/
	template <typename O, typename B>
	void Insert(size_t _index, const ZRingBuffer<T, O, B>& _other)
		{ ZRingBuffer_InsertImpl<T, P, A>::template Call<O, B>(*this, _index, _other, 0, _other.Size()); }
	
	/*
	public ZRingBuffer<T, P, A>::Insert

	Inserts data from the given buffer at the specified location.  Copies
	the values into the buffer.  Ring buffers are not well suited 
	to perform this operation quickly, so use sparingly.

	@param O - overflow policy for the other buffer
	@param B - allocator type for the other buffer
	@param _index - the index at which to perform the insertion
	@param _other - the buffer to insert
	@param _start - the index to start getting values from
	@param _count - the number of values to get
	@return (void)
	@assert - if _index is out of range
	*/
	template <typename O, typename B>
	void Insert(size_t _index, const ZRingBuffer<T, O, B>& _other, size_t _start, size_t _count)
		{ ZRingBuffer_InsertImpl<T, P, A>::template Call<O, B>(*this, _index, _other, _start, _count); }

	/*
	public ZRingBuffer<T, P, A>::PopBack
	 
	Function to pop the back element off of the ring buffer.
	 
	@return (T) - the back element in the buffer
	@assert - if the buffer is empty
	*/
	T PopBack()
		{ return ZRingBuffer_PopBackImpl<T, P, A>::Call(*this); }

	/*
	public ZRingBuffer<T, P, A>::PopFront
	 
	Function to pop the front element off of the ring buffer.
	 
	@return (T) - the front element in the buffer
	@assert - if the buffer is empty
	*/
	T PopFront()
		{ return ZRingBuffer_PopFrontImpl<T, P, A>::Call(*this); }
	
	/*
	public ZRingBuffer<T, P, A>::PushBack
	 
	Pushes an element onto the back of this ring buffer.
	 
	@param _value - element to push onto back of ring buffer
	@return (void)
	@assert - if the buffer is full
	*/
	void PushBack(const T& _value)
		{ ZRingBuffer_PushBackImpl<T, P, A>::Call(*this, _value); }

	/*
	public ZRingBuffer<T, P, A>::PushFront
	 
	Pushes an element onto the front of this ring buffer.
	
	@param _value - element to push onto front of ring buffer
	@return (void)
	@assert - if the buffer is full
	*/
	void PushFront(const T& _value)
		{ ZRingBuffer_PushFrontImpl<T, P, A>::Call(*this, _value); }

	/*
	public ZRingBuffer<T, P, A>::Reserve
	 
	Changes the capacity of the backing array to accommodate the given size.  Existing items in the 
	buffer remain in order and unaffected, though copy assignment may be required.
	 
	@param _newCapacity - new capacity to reserve in ring buffer
	@return (void)
	@assert - if the new capacity is smaller than the current size
	*/
	void Reserve(size_t _newCapacity)
		{ ZRingBuffer_ReserveImpl<T, P, A>::Call(*this, _newCapacity); }

	/*
	public ZRingBuffer<T, P, A>::Size
	 
	Gets the size of the ring buffer, which is the current number of contained elements.

	@return (size_t) - the number of elements in the ring buffer.
	*/
	size_t Size() const
		{ return ZRingBuffer_SizeImpl<T, P, A>::Call(*this); }

	/*
	public ZRingBuffer<T, P, A>::TryPushFront
	 
	Tries to push an element onto the ring buffer, returning success or failure.

	If the element is unable to be pushed onto the buffer, no change is made to the buffer.
	 
	@param _value - element to push onto front of ring buffer.
	@return (bool) - true if push was successful, false if not.
	*/
	bool TryPushFront(const T& _value)
		{ return ZRingBuffer_TryPushFrontImpl<T, P, A>::Call(*this, _value); }

	/*
	public ZRingBuffer<T, P, A>::TryPushBack
	 
	Tries to push an element onto the ring buffer, returning success or failure.

	If the element is unable to be pushed onto the buffer, no change is made to the buffer.
	 
	@param _value - element to push onto the back of the ring buffer.
	@return (bool) - true if push was successful, false if not.
	*/
	bool TryPushBack(const T& _value)
		{ return ZRingBuffer_TryPushBackImpl<T, P, A>::Call(*this, _value); }
};

////////////////////////////////////////////
/* Non-Specialized Method Implementations */
////////////////////////////////////////////

template <typename T, typename P, typename A>
T& ZRingBuffer_AtImpl<T, P, A>::Call(const ZRingBuffer<T, P, A>& _self, const size_t _index)
{
	return _self.Buffer.Data()[_self.ActualIndex(_index)]; //Bounds checking will be done by ActualIndex
}

/*************************************************************************/

template <typename T, typename P, typename A>
T& ZRingBuffer_BackImpl<T, P, A>::Call(const ZRingBuffer<T, P, A>& _self)
{
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT( _self.BufferSize > 0, "ZRingBuffer::Back() called with no elements present!");
	#endif

	return _self.Buffer.Data()[_self.BackIndex];
}

/*************************************************************************/

template <typename T, typename P, typename A>
size_t ZRingBuffer_CapacityImpl<T, P, A>::Call(const ZRingBuffer<T, P, A>& _self)
{
	return _self.Buffer.Size();
}

/*************************************************************************/

template <typename T, typename P, typename A>
void ZRingBuffer_ClearImpl<T, P, A>::Call( ZRingBuffer<T, P, A>& _self )
{
	_self.BufferSize = 0;
	_self.FrontIndex = 0;
	_self.BackIndex = 0;
}

/*************************************************************************/

template <typename T, typename P, typename A>
void ZRingBuffer_ClearImpl<T, P, A>::Call( ZRingBuffer<T, P, A>& _self, size_t _newCapacity )
{
	_self.Buffer.Resize(_newCapacity);

	_self.BufferSize = 0;
	_self.FrontIndex = 0;
	_self.BackIndex = 0;
}

/*************************************************************************/

template <typename T, typename P, typename A> template <typename O, typename B>
void ZRingBuffer_CopyImpl<T, P, A>::Call( ZRingBuffer<T, P, A>& _self, const ZRingBuffer<T, O, B>& _other )
{
	_self.Buffer.Copy(_other.Buffer);

	_self.BufferSize = _other.BufferSize;
	_self.FrontIndex = _other.FrontIndex;
	_self.BackIndex = _other.BackIndex;
}

/*************************************************************************/

template <typename T, typename P, typename A>
bool ZRingBuffer_EmptyImpl<T, P, A>::Call(const ZRingBuffer<T, P, A>& _self)
{
	return _self.BufferSize == 0;
}

/*************************************************************************/

template <typename T, typename P, typename A> template <typename O, typename B>
bool ZRingBuffer_EqualsImpl<T, P, A>::Call( const ZRingBuffer<T, P, A>& _self, const ZRingBuffer<T, O, B>& _other )
{
	size_t i;

	//Easy out #1
	if (&_self == &_other)
		return true;

	//Easy out #2
	if (_self.Size() != _other.Size())
		return false;

	//Element by element comparison
	for (i = 0; i < _self.Size(); i++)
	{
		if (_self.At(i) != _other.At(i))
			return false;
	}

	return true;
}

/*************************************************************************/

template <typename T, typename P, typename A>
T ZRingBuffer_EraseImpl<T, P, A>::Call(ZRingBuffer<T, P, A>& _self, const size_t _index)
{
	//We are going to do this in a very naive fashion, as this container is not intended for these
	//kinds of operations anyhow.  Anyone who wishes to optimize this at a later date is welcome.

	_self.AlignBuffer();

	_self.BufferSize--;
	_self.BackIndex--;

	return _self.Buffer.Erase(_index);
}

/*************************************************************************/

template <typename T, typename P, typename A>
void ZRingBuffer_EraseImpl<T, P, A>::Call( ZRingBuffer<T, P, A>& _self, size_t _i, size_t _j )
{
	//We are going to do this in a very naive fashion, as this container is not intended for these
	//kinds of operations anyhow.  Anyone who wishes to optimize this at a later date is welcome.

	size_t count;

	_self.AlignBuffer();

	count = (size_t)(_j - _i);

	_self.BufferSize = _self.BufferSize - count;
	_self.BackIndex = _self.BackIndex - count;

	_self.Buffer.Erase(_i, _j);
}

/*************************************************************************/

template <typename T, typename P, typename A>
T& ZRingBuffer_FrontImpl<T, P, A>::Call(const ZRingBuffer<T, P, A>& _self)
{
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT( _self.BufferSize > 0, "ZRingBuffer::Front() called with no elements present!");
	#endif

	return _self.Buffer.Data()[_self.FrontIndex];
}

/*************************************************************************/

template <typename T, typename P, typename A>
bool ZRingBuffer_FullImpl<T, P, A>::Call(const ZRingBuffer<T, P, A>& _self)
{
	return _self.BufferSize == _self.Buffer.Size();
}

/*************************************************************************/

template <typename T, typename P, typename A>
void ZRingBuffer_InsertImpl<T, P, A>::Call( ZRingBuffer<T, P, A>& _self, size_t _index, const T& _value, size_t _count )
{
	//We are going to do this in a very naive fashion, as this container is not intended for these
	//kinds of operations anyhow.  Anyone who wishes to optimize this at a later date is welcome.

	_self.AlignBuffer();

	_self.Buffer.Insert(_index, _value, _count);
	_self.Buffer.Resize(_self.Buffer.Capacity());	//In case the insert caused a grow

	_self.BackIndex = _self.BackIndex + _count;
	_self.BufferSize = _self.BufferSize + _count;
}

/*************************************************************************/

template <typename T, typename P, typename A> template <typename B>
void ZRingBuffer_InsertImpl<T, P, A>::Call( ZRingBuffer<T, P, A>& _self, size_t _index, const ZArray<T, B>& _array, size_t _start, size_t _count )
{
	//We are going to do this in a very naive fashion, as this container is not intended for these
	//kinds of operations anyhow.  Anyone who wishes to optimize this at a later date is welcome.

	_self.AlignBuffer();

	_self.Buffer.Insert(_index, _array, _start, _count);
	_self.Buffer.Resize(_self.Buffer.Capacity());	//In case the insert caused a grow

	_self.BackIndex = _self.BackIndex + _count;
	_self.BufferSize = _self.BufferSize + _count;
}

/*************************************************************************/

template <typename T, typename P, typename A> template <typename O, typename B>
void ZRingBuffer_InsertImpl<T, P, A>::Call( ZRingBuffer<T, P, A>& _self, size_t _index, const ZRingBuffer<T, O, B>& _other, size_t _start, size_t _count )
{
	//We are going to do this in a very naive fashion, as this container is not intended for these
	//kinds of operations anyhow.  Anyone who wishes to optimize this at a later date is welcome.

	ZRingBuffer<T, P, A> copy(_other);

	copy.AlignBuffer();
	_self.AlignBuffer();

	_self.Buffer.Insert(_index, copy.Array(), _start, _count);
	_self.Buffer.Resize(_self.Buffer.Capacity());	//In case the insert caused a grow

	_self.BackIndex = _self.BackIndex + _count;
	_self.BufferSize = _self.BufferSize + _count;
}

/*************************************************************************/

template <typename T, typename P, typename A>
T ZRingBuffer_PopBackImpl<T, P, A>::Call(ZRingBuffer<T, P, A>& _self)
{
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT( _self.BufferSize > 0, "ZRingBuffer::PopBack() caused underflow!");
	#endif

	size_t index = _self.BackIndex;
	
	_self.DecrementBack();

	_self.CheckIntegrity();

	return _self.Buffer.Data()[index];
}

/*************************************************************************/

template <typename T, typename P, typename A>
T ZRingBuffer_PopFrontImpl<T, P, A>::Call(ZRingBuffer<T, P, A>& _self)
{
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT( _self.BufferSize > 0, "ZRingBuffer::PopFront() caused underflow!");
	#endif

	size_t index = _self.FrontIndex;

	_self.IncrementFront();

	_self.CheckIntegrity();

	return _self.Buffer.Data()[index];
}

/*************************************************************************/

template <typename T, typename P, typename A>
void ZRingBuffer_PushBackImpl<T, P, A>::Call(ZRingBuffer<T, P, A>& _self, const T& _value)
{
	URFP(_self); URFP(_value);
	P::UnimplementedMethod;	//This will cause a compiler error if P is of invalid type
}

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushBackImpl <T, ZRingBuffer_OverflowUnsafe, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowUnsafe, A>& _self, const T& _value)
	{
		_self.IncrementBack().Buffer.Data()[_self.BackIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushBackImpl <T, ZRingBuffer_OverflowAssert, A>{
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowAssert, A>& _self, const T& _value)
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT( _self.BufferSize < _self.Buffer.Size(), "ZRingBuffer::PushBack() caused overflow!");
		#endif

		_self.IncrementBack().Buffer.Data()[_self.BackIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushBackImpl <T, ZRingBuffer_OverflowIgnore, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowIgnore, A>& _self, const T& _value)
	{
		//See if we should be ignoring this call
		if (_self.Full())
			return;

		_self.IncrementBack().Buffer.Data()[_self.BackIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushBackImpl <T, ZRingBuffer_OverflowDropBack, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowDropBack, A>& _self, const T& _value)
	{
		//See if we need to drop the back element
		if (_self.Full())
			_self.DecrementBack();

		_self.IncrementBack().Buffer.Data()[_self.BackIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushBackImpl <T, ZRingBuffer_OverflowDropFront, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowDropFront, A>& _self, const T& _value)
	{
		//See if we need to drop the front element
		if (_self.Full())
			_self.IncrementFront();

		_self.IncrementBack().Buffer.Data()[_self.BackIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushBackImpl <T, ZRingBuffer_OverflowOverwrite, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowOverwrite, A>& _self, const T& _value)
	{
		//See if we need to overwrite the current element
		if (_self.Full())
			_self.Buffer.Data()[_self.BackIndex] = _value;
		else
			_self.IncrementBack().Buffer.Data()[_self.BackIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushBackImpl <T, ZRingBuffer_OverflowEvict, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowEvict, A>& _self, const T& _value)
	{
		//See if we need to evict the opposite element
		if (_self.Full())
			_self.IncrementFront();

		_self.IncrementBack().Buffer.Data()[_self.BackIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushBackImpl <T, ZRingBuffer_OverflowGrow, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowGrow, A>& _self, const T& _value)
	{
		//See if we need to reserve more space
		if (_self.Full())
			_self.Reserve((size_t)(_self.BufferSize * ZRINGBUFFER_CAPACITY_RESIZE_FACTOR));

		_self.IncrementBack().Buffer.Data()[_self.BackIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename P, typename A>
void ZRingBuffer_PushFrontImpl<T, P, A>::Call(ZRingBuffer<T, P, A>& _self, const T& _value)
{
	URFP(_self); URFP(_value);
	P::UnimplementedMethod;	//This will cause a compiler error if P is of invalid type
}

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushFrontImpl <T, ZRingBuffer_OverflowUnsafe, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowUnsafe, A>& _self, const T& _value)
	{
		_self.DecrementFront().Buffer.Data()[_self.FrontIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushFrontImpl <T, ZRingBuffer_OverflowAssert, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowAssert, A>& _self, const T& _value)
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT( _self.BufferSize < _self.Buffer.Size(), "ZRingBuffer::PushFront() caused overflow!");
		#endif

		_self.DecrementFront().Buffer.Data()[_self.FrontIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushFrontImpl <T, ZRingBuffer_OverflowIgnore, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowIgnore, A>& _self, const T& _value)
	{
		//See if we should be ignoring this call
		if (_self.Full())
			return;

		_self.DecrementFront().Buffer.Data()[_self.FrontIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushFrontImpl <T, ZRingBuffer_OverflowDropBack, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowDropBack, A>& _self, const T& _value)
	{
		//See if we need to drop the back element
		if (_self.Full())
			_self.DecrementBack();

		_self.DecrementFront().Buffer.Data()[_self.FrontIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushFrontImpl <T, ZRingBuffer_OverflowDropFront, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowDropFront, A>& _self, const T& _value)
	{
		//See if we need to drop the front element
		if (_self.Full())
			_self.IncrementFront();

		_self.DecrementFront().Buffer.Data()[_self.FrontIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushFrontImpl <T, ZRingBuffer_OverflowOverwrite, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowOverwrite, A>& _self, const T& _value)
	{
		//See if we need to overwrite the current element
		if (_self.Full())
			_self.Buffer.Data()[_self.FrontIndex] = _value;
		else
			_self.DecrementFront().Buffer.Data()[_self.FrontIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushFrontImpl <T, ZRingBuffer_OverflowEvict, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowEvict, A>& _self, const T& _value)
	{
		//See if we need to evict the opposite element
		if (_self.Full())
			_self.DecrementBack();

		_self.DecrementFront().Buffer.Data()[_self.FrontIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename A>
struct ZRingBuffer_PushFrontImpl <T, ZRingBuffer_OverflowGrow, A> {
	inline static void Call(ZRingBuffer<T, ZRingBuffer_OverflowGrow, A>& _self, const T& _value)
	{
		//See if we need to reserve more space
		if (_self.Full())
			_self.Reserve((size_t)(_self.BufferSize * ZRINGBUFFER_CAPACITY_RESIZE_FACTOR));

		_self.DecrementFront().Buffer.Data()[_self.FrontIndex] = _value;
	}
};

/*************************************************************************/

template <typename T, typename P, typename A>
void ZRingBuffer_ReserveImpl<T, P, A>::Call(ZRingBuffer<T, P, A>& _self, size_t _newCapacity)
{
	//If the new and current capacities are equal, do nothing.
	if (_self.Buffer.Size() == _newCapacity)
		return;

	//If the new capacity is smaller than the current size, assert.
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT( _newCapacity >= _self.BufferSize, "ZRingBuffer::Reserve() called with new capacity below size!");
	#endif

	//Align the buffer
	_self.AlignBuffer();

	//Resize the buffer
	_self.Buffer.Resize(_newCapacity);
}

/*************************************************************************/

template <typename T, typename P, typename A>
size_t ZRingBuffer_SizeImpl<T, P, A>::Call(const ZRingBuffer<T, P, A>& _self)
{
	return _self.BufferSize;
}

/*************************************************************************/

template <typename T, typename P, typename A>
bool ZRingBuffer_TryPushFrontImpl<T, P, A>::Call(ZRingBuffer<T, P, A>& _self, const T& _value)
{
	if (_self.Full())
		return false;
	
	_self.PushFront(_value);
	
	return true;
}

/*************************************************************************/

template <typename T, typename P, typename A>
bool ZRingBuffer_TryPushBackImpl<T, P, A>::Call(ZRingBuffer<T, P, A>& _self, const T& _value)
{
	if (_self.Full())
		return false;
	
	_self.PushBack(_value);
	
	return true;
}

#endif
