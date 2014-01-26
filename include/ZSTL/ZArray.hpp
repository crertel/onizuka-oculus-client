/*
	ZArray.hpp
	Author: James Russell <jcrussell@762studios.com>
			Chris Ertel <crertel@762studios.com>
	Created: 9/12/2010

	Purpose:	
	
	Templated dynamic array implementation.

	Because ZArray is a dynamic array implementation (not a dynamic vector implementation), it
	makes no guarantees about the constructor / destructor behavior of individual elements.  This is
	to say that as elements enter and leave scope, the underlying array may not be modified and 
	constructs and destructors may not be called.  For example, if space is reserved for 20 elements,
	then 20 elements are constructed in an array of size 20.  The array will not be deleted until
	Reserve(0) is called on an array of size 0, which even then only guarantees that the allocator
	is called and the array provided for a 'Deallocate' function call.  Whether or not this deletes
	the array is up to the allocator.

	Note that the default allocator stores space for a certain number of elements and does not 
	destruct these until the allocator itself leaves scope.

	Note:

	A visualizer for Visual Studio 2008 for this container is available in vs_visualizers.txt.
	
	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _ZARRAY_HPP
#define _ZARRAY_HPP

#include <ZSTL/ZSTLCommon.hpp>
#include <ZSTL/ZSTLInvalidPos.hpp>

//Default capacity for ZArray when no capacity is defined (recommended that this be 
//less than or equal to local storage count, user can define their own)
#ifndef ZARRAY_DEFAULT_CAPACITY
#define ZARRAY_DEFAULT_CAPACITY (10)
#endif

//Resize factor for ZArray when operations are performed that require an increase in capacity
//and capacity is not specified (user can define their own, should be greater than 1.0)
#ifndef ZARRAY_CAPACITY_RESIZE_FACTOR
#define ZARRAY_CAPACITY_RESIZE_FACTOR (2.0)
#endif

//Local storage number of elements (user can define their own)
#ifndef ZARRAY_DEFAULT_ALLOCATOR_STORAGE
#define ZARRAY_DEFAULT_ALLOCATOR_STORAGE (10)
#endif

//Forward Declaration of ZArray
template <typename T, typename A>
class ZArray;

//Forward Declaration of ZArrayIterator
template <typename T>
class ZArrayIterator;

/* 
Forward Declaration of ZArray Method Implementation Structures 

Existence of these structures allows for template specialization of 
individual methods of the ZArray class.  In order to specialize
a single method of ZArray, simply specialize the corresponding
method implementation structure.
*/

//Forward Declaration of ZArray<T, A>::At struct
template <typename T, typename A>
struct ZArray_AtImpl {
	inline static T& Call(const ZArray<T, A>& _self, size_t _index);
};

//Forward Declaration of ZArray<T, A>::Back struct
template <typename T, typename A>
struct ZArray_BackImpl {
	inline static T& Call(const ZArray<T, A>& _self);
};

//Forward Declaration of ZArray<T, A>::Capacity struct
template <typename T, typename A>
struct ZArray_CapacityImpl {
	inline static size_t Call(const ZArray<T, A>& _self);
};

//Forward Declaration of ZArray<T, A>::Clear struct
template <typename T, typename A>
struct ZArray_ClearImpl {
	inline static void Call(ZArray<T, A>& _self);
	inline static void Call(ZArray<T, A>& _self, size_t _newCapacity);
};

//Forward Declaration of ZArray<T, A>::Copy struct
template <typename T, typename A>
struct ZArray_CopyImpl {
	template <typename B> inline static void Call(ZArray<T, A>& _self, const ZArray<T, B>& _other);
};

//Forward Declaration of ZArray<T, A>::Data struct
template <typename T, typename A>
struct ZArray_DataImpl {
	inline static T* Call(const ZArray<T, A>& _self);
};

//Forward Declaration of ZArray<T, A>::Empty struct
template <typename T, typename A>
struct ZArray_EmptyImpl {
	inline static bool Call(const ZArray<T, A>& _self);
};

//Forward Declaration of ZArray<T, A>::Equals struct
template <typename T, typename A>
struct ZArray_EqualsImpl {
	template <typename B> inline static bool Call(const ZArray<T, A>& _self, const ZArray<T, B>& _other);
};

//Forward Declaration of ZArray<T, A>::Erase struct
template <typename T, typename A>
struct ZArray_EraseImpl {
	inline static T Call(ZArray<T, A>& _self, size_t _index);
	inline static void Call(ZArray<T, A>& _self, size_t _start, size_t _end);
};

//Forward Declaration of ZArray<T, A>::Front struct
template <typename T, typename A>
struct ZArray_FrontImpl {
	inline static T& Call(const ZArray<T, A>& _self);
};

//Forward Declaration of ZArray<T, A>::Find struct
template <typename T, typename A>
struct ZArray_FindImpl {
	inline static size_t Call(const ZArray<T, A>& _self, const T& elem);
};

//Forward Declaration of ZArray<T, A>::Insert struct
template <typename T, typename A>
struct ZArray_InsertImpl {
	inline static void Call(ZArray<T, A>& _self, size_t _index, const T& _value, size_t _count);
	template <typename B> inline static void Call(ZArray<T, A>& _self, size_t _index, const ZArray<T, B>& _other, size_t _start, size_t _count);
};

//Forward Declaration of ZArray<T, A>::PopBack struct
template <typename T, typename A>
struct ZArray_PopBackImpl {
	inline static T Call(ZArray<T, A>& _self);
};

//Forward Declaration of ZArray<T, A>::PopFront struct
template <typename T, typename A>
struct ZArray_PopFrontImpl {
	inline static T Call(ZArray<T, A>& _self);
};

//Forward Declaration of ZArray<T, A>::PushBack struct
template <typename T, typename A>
struct ZArray_PushBackImpl {
	inline static void Call(ZArray<T, A>& _self, const T& _value);
};

//Forward Declaration of ZArray<T, A>::PushFront struct
template <typename T, typename A>
struct ZArray_PushFrontImpl {
	inline static void Call(ZArray<T, A>& _self, const T& _value);
};

//Forward Declaration of ZArray<T, A>::Reserve struct
template <typename T, typename A>
struct ZArray_ReserveImpl {
	inline static void Call(ZArray<T, A>& _self, size_t _capacity, bool _reallocate);
};

//Forward Declaration of ZArray<T, A>::Resize struct
template <typename T, typename A>
struct ZArray_ResizeImpl {
	inline static void Call(ZArray<T, A>& _self, size_t _size);
	inline static void Call(ZArray<T, A>& _self, size_t _size, const T& _value);
};

//Forward Declaration of ZArray<T, A>::Size struct
template <typename T, typename A>
struct ZArray_SizeImpl {
	inline static size_t Call(const ZArray<T, A>& _self);
};

//Forward Declaration of ZArray<T, A>::Swap struct
template <typename T, typename A>
struct ZArray_SwapImpl {
	template <typename B> inline static void Call(ZArray<T, A>& _self, ZArray<T, B>& _other);
};

//////////////////////
/* ZArray Allocator */
//////////////////////

/*
ZArray Allocator class.  Used to allocate arrays of a templated type 
when requested by a ZArray instance.

NOTE: This allocator keeps a set amount (N) of locally stored objects of type T.  These
objects are allocated when the array is created and deallocated when the array is
destructed, and there is no way (other than specializing based on type) to cause these
local elements to destruct other than destructing the array.  Reserve(0) will call
'Deallocate', but that will not destruct the elements. So, in summary, do not use the 
default allocator when you have a strict requirement that Reserve(0) should deallocate 
the array and call contained destructors.

The template parameter T is the type contained the array this allocator is for.

The template parameter N is the amount of local storage this allocator will keep
on hand.  Allocations of size less than N (in number of elements) will use the locally
allocated array.

*/
template <typename T, size_t N = ZARRAY_DEFAULT_ALLOCATOR_STORAGE>
class ZArrayAllocator
{
public:

	/*
	public ZArrayAllocator<T>::Allocate

	Allocation method.

	@param _size - size of the array to allocate
	@return - pointer to an array which can store at least _size values
	*/
	T* Allocate(size_t _size)
	{
		if (_size == 0)
			return NULL;
		
		if (_size > N)
		{
			return ZSTL_NEW_ARRAY(T, _size);
		}
		else
		{
			return Local;
		}
	}
	
	/*
	public ZArrayAllocator<T>::Deallocate

	Deallocation method.

	@param _ptr - pointer to previously allocated memory by this allocator
	@param _size - size of the memory
	*/
	void Deallocate(T* _ptr, size_t _size) 
	{
		if (_ptr == NULL)
			return;
		
		if (_ptr != &(Local[0]))
		{
			ZSTL_DEL_ARRAY(_ptr, _size);
		}
	}

protected:

	//The local storage for the allocator
	T Local[N];
};

/*
Specialization of ZArrayAllocator for a 0 size local storage.  Will
always allocate using ZSTL_NEW_ARRAY.
*/
template <typename T>
class ZArrayAllocator <T, 0>
{
public:
	//Allocates the array using ZSTL_NEW_ARRAY
	T* Allocate(size_t _size)
	{
		return ZSTL_NEW_ARRAY(T, _size);
	}
	
	//Deallocates the array using ZSTL_DEL_ARRAY
	void Deallocate(T* _ptr, size_t _size) 
	{
		ZSTL_DEL_ARRAY(_ptr, _size);
	}
};

/*
Heap allocator implementation of an allocator for ZArray.  Will
always allocate on the heap.
*/
template <typename T>
class ZArrayHeapAllocator
{
public:
	//Allocate on the heap
	T* Allocate(size_t _size)
	{
		return new (std::nothrow) T[_size];
	}

	//Deallocate the heap array
	void Deallocate(T* _ptr, size_t _size) 
	{
		URFP(_size);

		delete[] _ptr;
	}
};


/////////////////////
/* ZArray Iterator */
/////////////////////

/*
Iterator class for ZArray.  Can also be used to iterate a raw array of type T.

The template parameter T is the type contained in the list this iterator is for.
*/
template <typename T>
class ZArrayIterator
{
public:

	/*
	Default constructor.
	*/
	ZArrayIterator() 
		: Array(NULL), Index(0), Size(0) 
	{ }

	/*
	Copy constructor.
	
	@param _other - the other iterator
	*/
	ZArrayIterator(const ZArrayIterator& _other) 
		: Array(_other.Array), Index(_other.Index), Size(_other.Size) 
	{ CheckCurrent(); }

	/*
	Parameterized Constructor.

	@param _array - the array this iterator points to
	@param _index - the index into the array at which to start the iterator.
	@param _size - the size of the array we are pointed to
	*/
	ZArrayIterator(T *_array, size_t _index, size_t _size) 
		: Array(_array), Index(_index), Size(_size) 
	{ CheckCurrent(); }
	
	/*
	public ZArrayIterator<T>::CheckCurrent
	
	Check function that determines if the iterator is valid at it's current location. 

	@param _endIsValid - indicates that the iterator can be at the 'end' location
	@return (void)
	*/
	void CheckCurrent(bool _endIsValid = true) const
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(Array != NULL,								
			"Uninitialized ZArray Iterator used!");
		ZSTL_ASSERT(Index < Size + (_endIsValid ? 1 : 0),	
			"ZArray Iterator has gone past end of array!");
		#else
		URFP(_endIsValid);
		#endif
	}

	/*
	public ZArrayIterator<T>::CheckNext
	
	Check function that determines if incrementing the iterator would be valid,
	assuming it was already valid.
	
	@param _inc - the amount by which the iterator will be incremented
	@return (void)
	*/
	void CheckNext(size_t _inc) const
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		//Integer overflow possible here, but not likely
		ZSTL_ASSERT(Index + _inc <= Size, "ZArray Iterator has gone past end of array!");
		#else
		URFP(_inc);
		#endif
	}

	/*
	public ZArrayIterator<T>::CheckPrevious
	
	Check function that determines if decrementing the iterator would be valid,
	assuming it was already valid.

	@param _dec - the amount by which the iterator will be decremented
	@return (void)
	*/
	void CheckPrevious(size_t _dec) const
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		//i.e. check that "Index - _dec >= 0", but carefully since
		//these are unsigned numbers and underflow results in SIZE_MAX
		ZSTL_ASSERT(Index >= _dec, "ZArray Iterator has gone past beginning of array!");
		#else
		URFP(_dec);
		#endif
	}

	/*
	public ZArrayIterator<T>::Get
	
	Gets the element this iterator points to.
	
	@return (T&) - the element pointed to
	*/
	T& Get() const												
	{ 
		return *(*this); 
	}

	/*
	public ZArrayIterator<T>::HasCurrent
	
	Determines if this iterator currently points to a valid element.
	
	@return (bool) - true if element at current position
	*/
	bool HasCurrent() const										
	{ 
		return Index < Size; 
	}

	/*
	public ZArrayIterator<T>::HasNext
	
	Determines if this iterator has a valid element after the current element.
	
	@return (bool) - true if element after current, false otherwise
	*/
	bool HasNext() const										
	{ 
		return Size != 0 && Index < Size - 1; 
	}

	/*
	public ZArrayIterator<T>::HasPrev
	
	Determines if this iterator has a valid element before the current element.
	
	@return (bool) - true if element before current, false otherwise
	*/
	bool HasPrev() const
	{ 
		return Index > 0; 
	}

	/*
	public ZArrayIterator<T>::Next
	
	Advances this iterator to the next element.
	
	@return (void)
	@assert - if this would advance past end
	*/
	void Next()
	{ 
		++(*this); 
	}

	/*
	public ZArrayIterator<T>::Prev
	
	Returns this iterator to the previous element.
	
	@return (void)
	@assert - if this would advance past begin
	*/
	void Prev()													
	{ 
		--(*this); 
	}

	//Operator Overrides
	ZArrayIterator& operator ++()								{ CheckNext(1); Index++; return *this; }
	ZArrayIterator operator ++ (int)							{ CheckNext(1); return ZArrayIterator(Array, Index++, Size); }
	ZArrayIterator operator + (int _value)						{ return ZArrayIterator(Array, Index + _value, Size); }
	ZArrayIterator& operator += (int _value)					{ CheckNext(_value); Index += _value; return *this; }

	ZArrayIterator& operator -- ()								{ CheckPrevious(1); Index--; return *this; }
	ZArrayIterator operator -- (int)							{ CheckPrevious(1); return ZArrayIterator(Array, Index--, Size); }
	ZArrayIterator operator - (int _value)						{ return ZArrayIterator(Array, Index - _value, Size); }
	ZArrayIterator& operator -= (int _value)					{ CheckPrevious(_value); Index -= _value; return *this; }
	
	ZArrayIterator& operator = (const ZArrayIterator &_other)	{ Array = _other.Array; Index = _other.Index; Size = _other.Size; CheckCurrent(); return *this; }
	bool operator == (const ZArrayIterator &_other) const		{ return Index == _other.Index; }
	bool operator != (const ZArrayIterator &_other) const		{ return !(Index == _other.Index); }
	
	bool operator < (const ZArrayIterator &_other) const		{ return (Index < _other.Index); }
	bool operator <= (const ZArrayIterator &_other) const		{ return (Index <= _other.Index); }
	bool operator > (const ZArrayIterator &_other) const		{ return (Index > _other.Index); }
	bool operator >= (const ZArrayIterator &_other) const		{ return (Index >= _other.Index); }
	
	T& operator *() const										{ CheckCurrent(false); return Array[Index]; }
	
	operator size_t () const									{ return Index; }
	
protected:

	//The array
	T* Array;

	//The current index into the array
	size_t Index;

	//The size of the array (at construction)
	size_t Size;

};

///////////////////////////
/* ZArray Implementation */
///////////////////////////

/*
Templated dynamic array implementation.

The template parameter T is the type contained in the array.

The template parameter A is the type of allocator to use, which must allocate
arrays of type T.
*/
template <typename T, typename A = ZArrayAllocator<T, ZARRAY_DEFAULT_ALLOCATOR_STORAGE> >
class ZArray
{
friend struct ZArray_AtImpl<T, A>;
friend struct ZArray_BackImpl<T, A>;
friend struct ZArray_CapacityImpl<T, A>;
friend struct ZArray_ClearImpl<T, A>;
friend struct ZArray_CopyImpl<T, A>;
friend struct ZArray_DataImpl<T, A>;
friend struct ZArray_EmptyImpl<T, A>;
friend struct ZArray_EqualsImpl<T, A>;
friend struct ZArray_EraseImpl<T, A>;
friend struct ZArray_FrontImpl<T, A>;
friend struct ZArray_FindImpl<T, A>;
friend struct ZArray_InsertImpl<T, A>;
friend struct ZArray_PopBackImpl<T, A>;
friend struct ZArray_PopFrontImpl<T, A>;
friend struct ZArray_PushBackImpl<T, A>;
friend struct ZArray_ReserveImpl<T, A>;
friend struct ZArray_ResizeImpl<T, A>;
friend struct ZArray_SizeImpl<T, A>;
friend struct ZArray_SwapImpl<T, A>;
public:
	/*
	Typedef for ZArrayIterator (Allows ZArray<T, A>::Iterator notation).
	*/
	typedef ZArrayIterator<T> Iterator;

	/*
	Default Constructor.
	*/
	ZArray()
		: Array(NULL), 
		  ArrayCapacity(ZARRAY_DEFAULT_CAPACITY), 
		  ArraySize(0)
	{
		Allocate(ArrayCapacity);
		CheckIntegrity();
	}

	/*
	Parameterized Constructor.

	@param _capacity - the starting capacity of the array
	*/	
	explicit ZArray(size_t _capacity)
		: Array(NULL), 
		  ArrayCapacity(_capacity), 
		  ArraySize(0)
	{
		Allocate(ArrayCapacity);

		CheckIntegrity();
	}

	/*
	Parameterized Constructor.  Given a reference to an array with a 
	size that can be deduced by the compiler, will create the array
	using that data.

	@param C - the length of the array
	@param _data - the data to initialize with
	@param _count - the length of the data
	*/
	template <size_t C>
	explicit ZArray( const T (&_data)[C])
		: Array(NULL), 
		  ArrayCapacity((size_t)(C * ZARRAY_CAPACITY_RESIZE_FACTOR)), 
		  ArraySize(C)
	{
		Allocate(ArrayCapacity);

		for (size_t i = 0; i < C; i++)
			Array[i] = _data[i];

		CheckIntegrity();
	}

	/*
	Parameterized Constructor.

	@param _capacity - the starting capacity of the array
	@param _size - the starting size of the array
	*/	
	ZArray(size_t _capacity, size_t _size)
		: Array(NULL), 
		  ArrayCapacity(_capacity), 
		  ArraySize(_size)
	{
		Allocate(ArrayCapacity);
		Resize(ArraySize);

		CheckIntegrity();
	}

	/*
	Parameterized Constructor.  Given a raw array and a size, will
	create this array using that data.

	@param _data - the data to initialize with
	@param _count - the length of the data
	*/
	ZArray(const T* _data, size_t _count)
		: Array(NULL), 
		  ArrayCapacity((size_t)(_count * ZARRAY_CAPACITY_RESIZE_FACTOR)), 
		  ArraySize(_count)
	{
		Allocate(ArrayCapacity);

		for (size_t i = 0; i < _count; i++)
			Array[i] = _data[i];

		CheckIntegrity();
	}

	/*
	Parameterized Constructor.  Given a raw array and a size, will
	create this array using that data.

	@param _data - the data to initialize with
	@param _count - the length of the data
	@param _capacity - the capacity to initialize with (must be greater than size)
	*/
	ZArray(const T* _data, size_t _count, size_t _capacity)
		: Array(NULL), 
		  ArrayCapacity(_capacity), 
		  ArraySize(_count)
	{
		Allocate(ArrayCapacity);

		for (size_t i = 0; i < _count; i++)
			Array[i] = _data[i];

		CheckIntegrity();
	}

	/*
	Copy Constructor.  Makes a deep copy of the array.  Even though the code
	is the same as the more generic copy constructor below, the code is needed to
	ensure a default copy constructor is not generated.

	@param _other - the array to copy from
	*/
	ZArray(const ZArray<T, A>& _other)
		: Array(NULL),
		  ArrayCapacity(_other.Capacity()), 
		  ArraySize(_other.Size())
	{
		T* data;
		size_t i;

		Allocate(ArrayCapacity);
		CheckIntegrity();

		for (i = 0, data = _other.Data(); i < ArraySize; i++)
			Array[i] = data[i];
	}

	/*
	Copy Constructor.  Makes a deep copy of the array from an array with
	a different allocator type.

	@param B - the allocator type of the other array
	@param _other - the array to copy from
	*/
	template <typename B>
	ZArray(const ZArray<T, B>& _other)
		: Array(NULL), 
		  ArrayCapacity(_other.Capacity()), 
		  ArraySize(_other.Size())
	{
		T* data;
		size_t i;

		Allocate(ArrayCapacity);
		CheckIntegrity();

		for (i = 0, data = _other.Data(); i < ArraySize; i++)
			Array[i] = data[i];
	}

	/*
	Slice Copy Constructor.  Constructs this array as a slice of another array.

	@param B - the allocator type of the other array
	@param _other - the array to copy from
	*/
	template <typename B>
	ZArray(const ZArray<T, B>& _other, size_t _start, size_t _end)
		: Array(NULL), 
		  ArrayCapacity(_other.Capacity()), 
		  ArraySize(_end - _start)
	{
		Allocate(ArrayCapacity);

		CheckIntegrity();

		_other.BoundsCheck(_start, _other.Size());
		_other.BoundsCheck(_end, _other.Size() + 1);

		for (size_t i = _start; i < _end; i++)
			Array[i - _start] = _other.Data()[i];
	}

	/*
	Destructor.
	*/
	~ZArray()
	{
		CheckIntegrity();
		Deallocate(Array, ArrayCapacity);
	}

	/*
	[] Operator overload.  Gets a value from the array.  Functionally equivalent to 
	a call to At.

	@param _index - the integer index into the array
	@return (T&) - reference to a value contained in the array
	@assert - if the index is out of bounds
	*/
	T& operator [] (size_t _index) const
		{ return At(_index); }

	/*
	= Operator overload.  Sets this array equal to the other by making
	a copy.  Even though the code is the same as the more generic version below,
	this is needed to prevent a default assignment operator from being constructed.

	@param _other - the array to be set equal to
	@return (ZArray<T, A>&) - this array
	*/
	ZArray<T, A>& operator = (const ZArray<T, A>& _other) 
		{ Copy(_other); return *this; }

	/*
	= Operator overload.  Sets this array equal to the other by making
	a copy.

	@param _other - the array to be set equal to
	@return (ZArray<T, A>&) - this array
	*/
	template <typename B>
	ZArray<T, A>& operator = (const ZArray<T, B>& _other)
		{ Copy(_other); return *this; }

	/*
	== Operator overload.  Performs an element comparison on the two arrays.

	@param B - the type of allocator for the other array
	@param _other - the array to compare to
	@return (bool) - true if this array is equivalent to the other
	*/
	template <typename B>
	bool operator == (const ZArray<T, B>& _other) const 
		{ return Equals(_other); }
	
	/*
	!= Operator overload.  Performs an element comparison on the two arrays.

	@param B - the type of the allocator for the other array
	@param _other - the array to compare to
	@return (bool) - false if this array is equivalent to the other
	*/
	template <typename B>
	bool operator != (const ZArray<T, B>& _other) const 
		{ return !Equals(_other); }

	/*
	public ZArray<T, A>::Allocator

	Returns a reference to the installed allocator.

	@return (A&) - the array allocator
	*/
	A& Allocator()
		{ return ArrayAllocator; }

	/*
	public ZArray<T, A>::At
	
	Gets a value from the array at a given position.
	
	@param _index - index into the array
	@return (T&) - the value at the given index
	@assert - if the index is out of bounds
	*/
	T& At(size_t _index) const 
		{ return ZArray_AtImpl<T, A>::Call(*this, _index); }

	/*
	public ZArray<T, A>::Back
	
	Gets a reference to the value at the back of the array.
	
	@return (T&) - the value at the end of the array
	@assert - if the array is empty
	*/
	T& Back() const 
		{ return ZArray_BackImpl<T, A>::Call(*this); }

	/*
	public ZArray<T, A>::Begin

	Returns an iterator to the beginning of the array.

	@return (ZArray<T, A>::Iterator) - Iterator with index zero
	*/
	Iterator Begin() 
		{ return ZArrayIterator<T>(Array, 0, ArraySize); }

	/*
	public ZArray<T, A>::BoundsCheck

	Function that checks the provided index against the provided boundary, asserting if an 
	out of bounds access occurs.  Calls to this will be compiled out if ZSTL_DISABLE_RUNTIME_CHECKS 
	is not zero.

	@param _index - index
	@param _boundary - the (exclusive) boundary to use for bounds checking
	@return (size_t) - unsigned index into the array
	@assert - if the index is not less than boundary
	*/
	size_t BoundsCheck(size_t _index, size_t _boundary) const
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(_index < _boundary, "ZArray: Out of bounds access!");
		#else
		(void)_boundary;
		#endif

		return _index;
	}

	/*
	public ZArray<T, A>::Capacity

	Returns the capacity of the array, which is the number of values it can contain
	before allocation is required to increase storage size.

	@return (size_t) - array capacity
	*/
	size_t Capacity() const 
		{ return ZArray_CapacityImpl<T, A>::Call(*this); }

	/*
	public ZArray<T, A>::Clear

	Clears out the array of all contained elements, keeping the currently allocated 
	storage.

	@return (void)
	*/
	void Clear() 
		{ ZArray_ClearImpl<T, A>::Call(*this); }
	
	/*
	public ZArray<T, A>::Clear

	Clears out the array of all contained elements and will ensure that the capacity is
	at least as much as the provided value.  Reallocates the internal storage if necessary.

	@param _newCapacity - the (minimum) capacity of the array after clearing
	@return (void)
	*/
	void Clear(size_t _newCapacity) 
		{ ZArray_ClearImpl<T, A>::Call(*this, _newCapacity); }

	/*
	public ZArray<B>::Copy
	
	Makes a deep copy of the provided array.
	
	@param _other - the array to copy from
	@return (void)
	*/
	template <typename B>
	void Copy(const ZArray<T, B>& _other) 
		{ ZArray_CopyImpl<T, A>::template Call<B>(*this, _other); }

	/*
	public ZArray<T, A>::Data
	
	Gets a pointer to the underlying array data.
	
	@return (T*) - pointer to the first element in the array
	*/
	T* Data() const
		{ return ZArray_DataImpl<T, A>::Call(*this); }
	
	/*	
	public ZArray<T, A>::Empty

	Returns true if the array is empty (size 0), false otherwise.
	
	@return (bool) - true if empty, false otherwise
	*/
	bool Empty() const 
		{ return ZArray_EmptyImpl<T, A>::Call(*this); }

	/*
	public ZArray<T, A>::End

	Returns an iterator to the end of the array.

	@return (ZArray<T, A>::Iterator) - Iterator with index set to capacity
	*/
	Iterator End() 
		{ return ZArrayIterator<T>(Array, ArraySize, ArraySize); }

	/*
	public ZArray<T, A>::Equals
	
	Determines, with an element by element comparison, if this array is equivalent
	to another.
	
	@param _other - the array to compare with
	@return (bool) - true if equal, false otherwise
	*/
	template <typename B>
	bool Equals(const ZArray<T, B>& _other) const
		{ return ZArray_EqualsImpl<T, A>::template Call<B>(*this, _other); }

	/*
	public ZArray<T, A>::Erase

	Erase function.  Removes an element from the array at the given index.

	@param _index - the index at which to remove the element from the array.
	@return (T) - the element removed
	@assert - if the index is invalid
	*/
	T Erase(size_t _index) 
		{ return ZArray_EraseImpl<T, A>::Call(*this, _index); }

	/*
	public ZArray<T, A>::Erase

	Erase function.  Removes elements from the array between the given indices.

	@param _start - the starting index
	@param _end - the ending index (exclusive)
	@return (void)
	@assert - if the indices are invalid
	*/
	void Erase(size_t _start, size_t _end)
		{ ZArray_EraseImpl<T, A>::Call(*this, _start, _end); }
	
	/*
	public ZArray<T, A>::Front
	
	Gets a reference to the value at the front of the array.
	
	@return (T&) - the value at the front of the array
	@assert - if the array is empty
	*/
	T& Front() const 
		{ return ZArray_FrontImpl<T, A>::Call(*this); }

	/*
	public ZArray<T, A>::Find

	Find function.  Finds the first element in the array that is equal to elem,
	determined via == operator.

	@param _elem - the elem to find
	@return (size_t) - the index of the first occurence of elem, ZSTL::InvalidPos if not found
	*/
	size_t Find(const T& _elem)
		{ return ZArray_FindImpl<T, A>::Call(*this, _elem); }

	/*
	public ZArray<T, A>::Insert

	Inserts the given value at the specified location.

	@param _index - the index at which to perform the insertion
	@param _value - the value to insert
	@return (void)
	@assert - if the index is invalid
	*/
	void Insert(size_t _index, const T& _value) 
		{ ZArray_InsertImpl<T, A>::Call(*this, _index, _value, 1); }
	
	/*
	public ZArray<T, A>::Insert

	Inserts the given value at the specified location the given number of times.

	@param _index - the index at which to perform the insertion
	@param _value - the value to insert
	@param _count - the number of times to insert _value
	@return (void)
	@assert - if the index is invalid
	*/
	void Insert(size_t _index, const T& _value, size_t _count) 
		{ ZArray_InsertImpl<T, A>::Call(*this, _index, _value, _count); }
	
	/*
	public ZArray<T, A>::Insert

	Inserts the entirety of the given array at the specified location. 

	@param _index - the index at which to perform the insertion
	@param _other - the array to insert values from
	@return (void)
	@assert - if the index is invalid
	*/
	template <typename B>
	void Insert(size_t _index, const ZArray<T, B>& _other)
		{ ZArray_InsertImpl<T, A>::template Call<B>(*this, _index, _other, 0, _other.Size()); }
	
	/*
	public ZArray<T, A>::Insert

	Inserts data from the given array at the specified location. 

	@param _index - the index at which to perform the insertion
	@param _other - the array to insert values from
	@param _start - the index to start getting values from
	@param _count - the number of elements to insert starting at index
	@return (void)
	@assert - if the index is invalid or the start index is invalid
	*/
	template <typename B>
	void Insert(size_t _index, const ZArray<T, B>& _other, size_t _start, size_t _count)
		{ ZArray_InsertImpl<T, A>::template Call<B>(*this, _index, _other, _start, _count); }

	/*
	public ZArray<T, A>::PopBack

	Pop function.  Removes and returns the last element in the array.

	@return (T) - the last element in the array
	@assert - if the array is empty
	*/
	T PopBack() 
		{ return ZArray_PopBackImpl<T, A>::Call(*this); }

	/*
	public ZArray<T, A>::PopFront

	Pop function.  Removes and returns the first element in the array.  This operation 
	requires shifting all elements in the array forward.

	@return (T) - the first element in the array
	@assert - if the array is empty
	*/
	T PopFront() 
		{ return ZArray_PopFrontImpl<T, A>::Call(*this); }

	/*
	public ZArray<T, A>::PushBack

	Push function.  Attaches an element to the end of the array.  Will allocate
	storage if necessary.

	@param _value - the element to place at the end of the array
	@return (void)
	*/
	void PushBack(const T& _value) 
		{ ZArray_PushBackImpl<T, A>::Call(*this, _value); }

	/*
	public ZArray<T, A>::PushFront

	Push function.  Attaches an element to the beginning of the array.  This operation
	requires shifting all elements in the array back.  Will allocate storage if necessary.

	@param _value - the element to place at the beginning of the array
	@return (void)
	*/
	void PushFront(const T& _value) 
		{ ZArray_PushFrontImpl<T, A>::Call(*this, _value); }

	/*
	public ZArray<T, A>::Reserve

	Reserves an amount of space in the vector.  Allocates space if 
	necessary (capacity > current capacity).  The array capacity cannot 
	be reduced in size below the number of contained elements.

	Reserve(0) is guaranteed to always deallocate the array using
	the provided deallocator.

	@param _capacity - the new capacity requested
	@return (void)
	@assert - if capacity is less than size
	*/
	void Reserve(size_t _capacity) 
		{ ZArray_ReserveImpl<T, A>::Call(*this, _capacity, false); }

	/*
	public ZArray<T, A>::Reserve

	Reserves an amount of space in the vector.  Allocates space if 
	necessary (capacity > current capacity).  The array capacity cannot 
	be reduced in size below the number of contained elements.
	
	Reserve(0) is guaranteed to always deallocate the array using
	the provided deallocator, regardless of the value of _reallocate.

	@param _capacity - the new capacity requested
	@param _reallocate - if true, will always reallocate storage
	@return (void)
	@assert - if capacity is less than size
	*/
	void Reserve(size_t _capacity, bool _reallocate) 
		{ ZArray_ReserveImpl<T, A>::Call(*this, _capacity, _reallocate); }

	/*
	public ZArray<T, A>::Resize

	Resize function.  Increases or Decreases the size of the array.
	
	@param _size - the new size to grow the array to
	@return (void)
	*/
	void Resize(size_t _size) 
		{ ZArray_ResizeImpl<T, A>::Call(*this, _size); }

	/*
	public ZArray<T, A>::Resize

	Resize function.  Increases or Decreases the size of the array and
	sets new values equal to the given value.  
	
	@param _size - the new size to grow (or shrink) the array to
	@param _value - the new value to set all added values to
	@return (void)
	*/
	void Resize(size_t _size, const T& _value) 
		{ ZArray_ResizeImpl<T, A>::Call(*this, _size, _value); }

	/*
	public ZArray<T, A>::Resize

	Resize function.  Increases or decreases the size of the array and
	reserves a new capacity (must be greater than size).  Sets new values 
	equal to the given value.

	@param _size - the new size to grow (or shrink) the array to
	@param _capacity - the new capacity to reserve (must be greater than or equal to size)
	@param _value - the new value to set all added values to
	@return (void)
	*/
	void Resize(size_t _size, size_t _capacity, const T& _value)
		{ ZArray_ResizeImpl<T, A>::Call(*this, _size, _capacity, _value); }

	/*
	public ZArray<T, A>::Size

	Size function.  Give the size of the array.

	@return (size_t) - the size of this array
	*/
	size_t Size() const 
		{ return ZArray_SizeImpl<T, A>::Call(*this); }

	/*
	public ZArray<T, A>::Swap

	Swaps the array contents with another array with any
	local storage size.

	@param _other - the array to swap contents with
	@return (void)
	*/
	template <typename B>
	void Swap(ZArray<T, B>& _other) 
		{ ZArray_SwapImpl<T, A>::Call(*this, _other); }
		
protected:

	//The array pointer
	T *Array;

	//Allocator for the Array
	A ArrayAllocator;

	//The 'size' of the raw array pointer
	size_t ArrayCapacity;

	//The current number of contained elements in the array
	size_t ArraySize;

	//Makes a call to the allocator and checks the result
	inline void Allocate(size_t _size)
	{
		Array = ArrayAllocator.Allocate(_size);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(!(Array == NULL && _size > 0), "ZArrayAllocator failed to properly allocate array! (returned NULL with size > 0)");
		#endif
	}

	//Makes a call to the allocator
	inline void Deallocate(T* _array, size_t _size)
	{
		ArrayAllocator.Deallocate(_array, _size);
	}

	//Shifts values up in the array from _start by _count
	inline void ShiftUp(size_t _index, size_t _count)
	{
		/*
		Shift elements up, but do so in reverse so that we don't just copy Array[i] to all of the higher addresses.
		If you do:

			Array[i+1] = Array[i];

		...in a loop, then the first iteration overwrites the second value with the first, but it doesn't preserve
		the second so it can be copied into the third. If you run this code on [1, 2, 3, 0], you get [1, 1, 3, 0] after the first
		iteration, and then [1, 1, 1, 0] after the second iteration, and so on.

		The solution is to copy backwards. Consider an array of 100 elements:

		[1, 2, 3, ..., 100]

		...that is to by shifted by 3 elements, starting at index 5. Then, we know that
		the final array should look like:

		[1, 2, 3, 4, X, X, X, ..., 5, 6, 7, ... 100].

		where X is undefined (in this function, it is "_value"). The number of elements that
		will be shifted is 100 - 5 = 95 elements. Further more, each element will be shifted 3
		units. Let S be the size of the array (100), I be the index to start at (5), and N
		be the number to shift by (3). Then,

		Since we are copying backwards, consider the final element: Array[S-1]. This element
		is to be moved N units, so:

			Array[S-1 + N] = Array[S-1];
		
		Add in a counter, K, that decreases the index value each loop iteration, and loop S-I times.

			Array[S-1 + N - K] = ArraySize[S-1 - K]
		*/

		size_t i;
		const size_t nShift = ArraySize - _index;

		//Shift elements up
		for(i = 0; i < nShift; i++)
			Array[ArraySize - 1 + _count - i] = Array[ArraySize - 1 - i];
	}

	//Integrity Check (used for internal debugging)
	inline void CheckIntegrity() const
	{
		#if ZSTL_CHECK_INTEGRITY
		ZSTL_ASSERT(!(Array == NULL && ArrayCapacity > 0), "ZArray Error: Array is invalid!");
		ZSTL_ASSERT(ArrayCapacity >= ArraySize, "ZArray Error: Array capacity less than size!");
		#endif
	}
};

////////////////////////////////////////////
/* Non-Specialized Method Implementations */
////////////////////////////////////////////

template <typename T, typename A>
T& ZArray_AtImpl<T, A>::Call( const ZArray<T, A>& _self, size_t _index )
{
	size_t index = _self.BoundsCheck(_index, _self.ArraySize);

	return _self.Array[index];
}

/*************************************************************************/

template <typename T, typename A>
T& ZArray_BackImpl<T, A>::Call( const ZArray<T, A>& _self )
{
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(_self.ArraySize > 0, "ZArray: Cannot get back element from empty array!");
	#endif

	return _self.Array[_self.ArraySize - 1];
}

/*************************************************************************/

template <typename T, typename A>
size_t ZArray_CapacityImpl<T, A>::Call(const ZArray<T, A>& _self)
{ 
	//Give back our current capacity
	return _self.ArrayCapacity;	
}

/*************************************************************************/

template <typename T, typename A>
void ZArray_ClearImpl<T, A>::Call(ZArray<T, A>& _self)
{
	//Reset size and check integrity
	_self.ArraySize = 0;
	_self.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A>
void ZArray_ClearImpl<T, A>::Call(ZArray<T, A>& _self, size_t _newCapacity)
{ 
	//Reset size, reserve new capacity, then check integrity
	_self.ArraySize = 0;
	_self.Reserve(_newCapacity, false);

	_self.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A> template <typename B>
void ZArray_CopyImpl<T, A>::Call(ZArray<T, A>& _self, const ZArray<T, B>& _other)
{
	size_t i;

	//If both arrays point to the same data, we're done here
	if (_self.Data() == _other.Data())
		return;

	//Make sure we have room, write data, and check integrity
	_self.Resize(_other.Size());

	for (i = 0; i < _other.Size(); i++)
		_self.Array[i] = _other.Data()[i];

	_self.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A>
T* ZArray_DataImpl<T, A>::Call(const ZArray<T, A>& _self)
{
	return _self.Array;
}

/*************************************************************************/

template <typename T, typename A>
bool ZArray_EmptyImpl<T, A>::Call(const ZArray<T, A>& _self)
{
	return _self.ArraySize == 0;
}

/*************************************************************************/

template <typename T, typename A> template <typename B>
bool ZArray_EqualsImpl<T, A>::Call(const ZArray<T, A>& _self, const ZArray<T, B>& _other)
{
	size_t i;

	//First see if we have the same size
	if (_self.ArraySize == _other.Size())
	{
		//Element wise comparison
		for (i = 0; i < _self.ArraySize; i++)
		{
			if (!(_self.Array[i] == _other.Array[i]))
				return false;
		}
	}		
	else
	{
		//Nope, so false
		return false;
	}

	return true;
}

/*************************************************************************/

template <typename T, typename A>
T ZArray_EraseImpl<T, A>::Call(ZArray<T, A>& _self, size_t _index)
{ 
	size_t index = _self.BoundsCheck(_index, _self.ArraySize);

	//Grab the currently held element, shift the array down, reduce size, and check integrity
	T element = _self.Array[index];

	for (size_t i = index; i + 1 < _self.ArraySize; i++)
		_self.Array[i] = _self.Array[i + 1];

	_self.ArraySize--;
	_self.CheckIntegrity();

	return element;
}

/*************************************************************************/

template <typename T, typename A>
void ZArray_EraseImpl<T, A>::Call(ZArray<T, A>& _self, size_t _start, size_t _end)
{
	if (_start == _end)
		return;

	size_t start = _self.BoundsCheck(_start, _self.ArraySize);
	size_t end = _self.BoundsCheck(_end, _self.ArraySize + 1);

	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(start <= end, "ZArray: cannot erase with start < end!");
	#endif

	//Copy the elements down, compute new size, and check integrity
	for (size_t idx = start; idx + (end - start) < _self.ArraySize; idx++)
		_self.Array[idx] = _self.Array[idx + (end - start)];

	_self.ArraySize = _self.ArraySize - (end - start);
	_self.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A>
T& ZArray_FrontImpl<T, A>::Call( const ZArray<T, A>& _self )
{
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(_self.ArraySize > 0, "ZArray: Cannot get front element from empty array!");
	#endif

	return _self.Array[0];
}

/*************************************************************************/

template <typename T, typename A>
size_t ZArray_FindImpl<T, A>::Call( const ZArray<T, A>& _self, const T& _elem )
{
	for (size_t i = 0; i < _self.ArraySize; i++) {
		if (_self.Array[i] == _elem) {
			return i;
		}
	}
	
	return ZSTL::InvalidPos;
}

/*************************************************************************/

template <typename T, typename A>
void ZArray_InsertImpl<T, A>::Call(ZArray<T, A>& _self, size_t _index, const T& _value, size_t _count)
{ 
	//Get our insertion index (we allow past end to indicate insert at 'end')
	size_t index = _self.BoundsCheck(_index, _self.ArraySize + 1);

	const size_t newSize = _self.ArraySize + _count;

	//Reserve enough space for the new elements
	if (newSize > _self.ArrayCapacity)
		_self.Reserve(newSize);

	//Shift up existing elements
	_self.ShiftUp(index, _count);

	//Insert new values
	for (size_t i = 0; i < _count; i++)
		_self.Array[index + i] = _value;

	//Increase our array size
	_self.ArraySize += _count;

	//We are non const, so check
	_self.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A> template <typename B>
void ZArray_InsertImpl<T, A>::Call(ZArray<T, A>& _self, size_t _index, const ZArray<T, B>& _other, size_t _start, size_t _count)
{
	//Get our indices (once again, allowed to go past end to indicate 'append')
	size_t index = _self.BoundsCheck(_index, _self.ArraySize + 1);
	size_t start = _other.BoundsCheck(_start, _other.Size() + 1);

	_other.BoundsCheck(_start + _count, _other.Size() + 1);	//This will do a bounds check on other

	const size_t newSize = _self.ArraySize + _count;

	//Make sure we have enough space
	if (newSize > _self.ArrayCapacity)
		_self.Reserve( newSize );

	//Shift elements up
	_self.ShiftUp(index, _count);

	//Copy in the data from the other
	for (size_t i = 0; i < _count; i++)
		_self.Array[index + i] = _other.Data()[start + i];

	//Set our new size
	_self.ArraySize = newSize;

	//We are non-const, so check
	_self.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A>
T ZArray_PopBackImpl<T, A>::Call(ZArray<T, A>& _self)
{ 
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(_self.ArraySize > 0, "ZArray: Cannot pop from array with no elements!");
	#endif

	//Grab the last element in the array and decrease our array size
	return _self.Array[--(_self.ArraySize)];
}

/*************************************************************************/

template <typename T, typename A>
T ZArray_PopFrontImpl<T, A>::Call(ZArray<T, A>& _self)
{ 
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(_self.ArraySize > 0, "ZArray: Cannot pop from array with no elements!");
	#endif

	//Just use our erase function
	return _self.Erase(0);
}

/*************************************************************************/

template <typename T, typename A>
void ZArray_PushBackImpl<T, A>::Call(ZArray<T, A>& _self, const T& _value)
{ 
	//See if we need more space
	if (_self.ArraySize >= _self.ArrayCapacity)
		_self.Reserve((size_t)((float)_self.ArrayCapacity * ZARRAY_CAPACITY_RESIZE_FACTOR) + 1 ); //Calls CheckIntegrity

	//Add in the element
	_self.Array[(_self.ArraySize)++] = _value;
}

/*************************************************************************/

template <typename T, typename A>
void ZArray_PushFrontImpl<T, A>::Call(ZArray<T, A>& _self, const T& _value)
{ 
	//Just use the insert function
	return _self.Insert(0, _value, 1);
}

/*************************************************************************/

template <typename T, typename A>
void ZArray_ReserveImpl<T, A>::Call(ZArray<T, A>& _self, size_t _capacity, bool _reallocate)
{ 
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(_capacity >= _self.ArraySize, "ZArray: Cannot reserve capacity less than array size!");
	#endif

	//Special case for reserve 0
	if (_capacity == 0)
	{
		_self.Deallocate(_self.Array, _self.ArrayCapacity);
		_self.Array = NULL;
		_self.ArrayCapacity = 0;
		return;
	}

	//See if we actually need to increase our capacity
	if (!_reallocate && _capacity <= _self.ArrayCapacity)
		return;
		
	//Set our temp array
	T* temp = _self.Array;

	//Allocate an array
	_self.Allocate(_capacity);

	//Make sure we aren't doing something pointless here
	if (temp == _self.Array)
	{
		_self.ArrayCapacity = _capacity;
		return;
	}

	//Copy the data
	for (size_t i = 0; i < _self.ArraySize; i++)
		_self.Array[i] = temp[i];

	//Deallocate the temporary (old) array
	_self.Deallocate(temp, _self.ArrayCapacity);

	//Set our new capacity
	_self.ArrayCapacity = _capacity;

	//non-const, so check integrity
	_self.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A>
void ZArray_ResizeImpl<T, A>::Call(ZArray<T, A>& _self, size_t _size)
{ 
	//Check to see if we need more space, change our size, and check integrity
	if (_size > _self.ArrayCapacity)
		_self.Reserve(_size);

	_self.ArraySize = _size;
	_self.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A>
void ZArray_ResizeImpl<T, A>::Call(ZArray<T, A>& _self, size_t _size, const T& _value)
{
	//See if we need more space, copy in the new value, change our size, and check integrity
	if (_size > _self.ArrayCapacity)
		_self.Reserve(_size);

	for (size_t i = _self.ArraySize; i < _size; i++)
		_self.Array[i] = _value;

	_self.ArraySize = _size;
	_self.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A>
size_t ZArray_SizeImpl<T, A>::Call(const ZArray<T, A>& _self)
{
	return _self.ArraySize;
}

/*************************************************************************/

template <typename T, typename A> template <typename B>
void ZArray_SwapImpl<T, A>::Call(ZArray<T, A>& _self, ZArray<T, B>& _other)
{	
	T tempElement;
	size_t i;
	size_t smallerSize;

	// Arrays are same size.
	if (_self.ArraySize == _other.ArraySize)
	{
		for (i = 0; i < _self.ArraySize; i++)
		{
			tempElement = _other[i];
			_other[i] = _self[i];
			_self[i] = tempElement;
		}
		return;
	}
	// Self is larger than other.
	else if (_self.ArraySize > _other.ArraySize)
	{
		smallerSize = _other.ArraySize;

		// Swap all of the smaller array's elements
		for (i = 0; i < smallerSize; i++)
		{
			tempElement = _other[i];
			_other[i] = _self[i];
			_self[i] = tempElement;
		}

		// Resize smaller array to be larger.
		_other.Reserve(_self.ArraySize);
		for (i = smallerSize; i < _self.ArraySize; i++)
			_other.PushBack(_self[i]);

		// Truncate our array.
		_self.Resize(smallerSize);
	}
	else
	{
		smallerSize = _self.ArraySize;

		// Swap all of the smaller array's elements
		for (i = 0; i < smallerSize; i++)
		{
			tempElement = _other[i];
			_other[i] = _self[i];
			_self[i] = tempElement;
		}

		// Resize smaller array to be larger.
		_self.Reserve(_other.ArraySize);
		for (i = smallerSize; i < _other.ArraySize; i++)
			_self.PushBack(_other[i]);

		// Truncate our array.
		_other.Resize(smallerSize);
	}

	//Check integrity on both arrays
	_self.CheckIntegrity();
	_other.CheckIntegrity();
}

#endif
