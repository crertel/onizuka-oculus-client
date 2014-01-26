/*
	ZBasicString.hpp
	Author: James Russell <jcrussell@762studios.com>
	Created: 12/25/2011

	Purpose: 

	Basic ASCII string implementation.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _ZBASICSTRING_HPP
#define _ZBASICSTRING_HPP

#include <ZSTL/ZArray.hpp>

#include <string.h>	//for strcmp, memset, memmove
#include <ctype.h>	//for isspace, tolower
#include <stdio.h>	//for sprintf

#include <iostream>	//for ostream

/*
 * Turns out that _stricmp is a Windows specific function.  
 * strcasecmp() appears to be POSIX compliant.
 */
#ifndef _WIN32
#include <strings.h> 
#endif

//Null Terminator Definition
#define ZBASICSTRING_NULL_TERMINATOR ((char)'\0')

//Default Capacity for a ZBasicString (not including the null terminator)
#ifndef ZBASICSTRING_DEFAULT_CAPACITY
#define ZBASICSTRING_DEFAULT_CAPACITY (127)
#endif

//Default buffer size for a ZBasicString
#ifndef ZBASICSTRING_DEFAULT_ALLOCATOR_STORAGE
#define ZBASICSTRING_DEFAULT_ALLOCATOR_STORAGE (128)
#endif

/*
Dynamic ASCII string implementation.

The template parameter A is the array allocator that will be used by the underlying array
that stores our characters.  It should allocate arrays of type 'char'.
*/
template <typename A = ZArrayAllocator<char, ZBASICSTRING_DEFAULT_ALLOCATOR_STORAGE> >
class ZBasicString
{
protected:
	//Array used to hold the characters
	ZArray<char, A> StringArray;

	//Integrity check
	inline void CheckIntegrity()
	{
		#if ZSTL_CHECK_INTEGRITY

		ZSTL_ASSERT(StringArray.Size() > 0, "StringArray is empty with no null terminator!");
		ZSTL_ASSERT(StringArray[Length()] == ZBASICSTRING_NULL_TERMINATOR, "StringArray has no null terminator!");

		#endif //ZSTL_CHECK_INTEGRITY
	}

public:
	/*
	Default constructor.  Constructs an empty string.
	*/
	ZBasicString()
		: StringArray(ZBASICSTRING_DEFAULT_CAPACITY + 1)
	{
		//Make sure we have our null terminator
		StringArray.PushBack(ZBASICSTRING_NULL_TERMINATOR);
		CheckIntegrity();
	}

	/*
	Parameterized constructor.  Constructs an empty string with the 
	specified capacity (not including the null terminator).
	*/
	explicit ZBasicString(size_t _capacity)
		: StringArray(_capacity + 1)
	{
		//Make sure we have our null terminator
		StringArray.PushBack(ZBASICSTRING_NULL_TERMINATOR);
		CheckIntegrity();
	}

	/*
	Parameterized constructor.  This constructor will initialize a copy of 
	the provided null-terminated string.

	@param _string - the null-terminated string to initialize to
	*/
	ZBasicString(const char *_string)
		: StringArray( _string != NULL ? strlen(_string) + 1 : 1)
	{
		//Set the string array size equal to the starting capacity, copy, and set null terminator
		StringArray.Resize(StringArray.Capacity());
		memmove(StringArray.Data(), _string, Length() * sizeof(char));
		StringArray.Data()[Length()] = ZBASICSTRING_NULL_TERMINATOR;
		CheckIntegrity();
	}

	/*
	Parameterized constructor.  This constructor will initialize a copy of the provided
	string, but it will only copy in as many characters as defined.  The null terminator
	at the end is not required.

	If a null terminator is present before the end of the string, the string will be 
	shortened such that the first encountered null terminator is at the end of the string.

	@param _string - the string to initialize to
	@param _count - the number of chars in the string
	*/
	ZBasicString(const char *_string, const size_t _count)
		: StringArray(_string, _count, _count + 1)
	{
		StringArray.PushBack(ZBASICSTRING_NULL_TERMINATOR);
		StringArray.Resize(strlen(StringArray.Data()) + 1);

		CheckIntegrity();
	}

	/*
	Parameterized constructor.  This constructor will initialize a string from the character
	data contained in the provided array.  The null terminator at the end is not required.

	If a null terminator is present before the end of the string, the string will be 
	shortened such that the first encountered null terminator is at the end of the string.

	@param _array - the provided character array to use as a string
	*/
	template <typename B>
	explicit ZBasicString(const ZArray<char, B>& _array)
		: StringArray(_array.Data(), _array.Size(), _array.Capacity() + 1)
	{
		StringArray.PushBack(ZBASICSTRING_NULL_TERMINATOR);
		StringArray.Resize(strlen(StringArray.Data()) + 1);

		CheckIntegrity();
	}

	/*
	Copy constructor.

	@param _other - the other string
	*/
	ZBasicString(const ZBasicString<A>& _other)
		: StringArray(_other.Array())
	{
		CheckIntegrity();
	}

	/*
	Copy constructor.  Used when the allocator type for the 
	other string is different.

	@param B - the allocator type of the other string
	@param _other - the other string
	*/
	template <typename B>
	ZBasicString(const ZBasicString<B>& _other)
		: StringArray(_other.Array())
	{
		CheckIntegrity();
	}
	
	/*
	Slice Copy constructor.  Constructs this string as a slice of another.

	@param B - the allocator type of the other string
	@param _other - the other string
	*/
	template <typename B>
	ZBasicString(const ZBasicString<B>& _other, size_t _start, size_t _end)
		: StringArray(_other.Array(), _start, _end)
	{
		if (_start == _end || StringArray.Back() != ZBASICSTRING_NULL_TERMINATOR)
			StringArray.PushBack(ZBASICSTRING_NULL_TERMINATOR);

		CheckIntegrity();
	}

	/*
	Destructor.
	*/
	~ZBasicString()
	{ 
		CheckIntegrity(); 
	}

	/*
	[] operator overload.  Allows indexing into the characters of this string.
	Equivalent to a call to At.

	@param _index - integer index into the string
	@return (char&) - character at the given index
	@assert - if the index is out of bounds
	*/
	char& operator [] (const size_t _index) const
		{ return At(_index); }

	/*
	= operator overload.  Makes this string equivalent to the other.  Equivalent
	to a call to Copy.

	@param _other - the string to set this equal to
	@return (ZBasicString<A>&) - this string
	*/
	ZBasicString<A>& operator = (const ZBasicString<A>& _other) 
		{ Copy(_other); return *this; }

	/*
	= operator overload.  Makes this string equivalent to the other. Used when the underlying
	allocator type is different.  Equivalent to a call to Copy.

	@param B - the allocator type for the other string
	@param _other - the string to set this equal to
	@return (ZBasicString<A>&) - this string
	*/
	template <typename B>
	ZBasicString<A>& operator = (const ZBasicString<B>& _other) 
		{ Copy(_other); return *this; }

	//C-String Implementation of operator =
	ZBasicString<A>& operator = (const char* _other) 
		{ Copy(_other); return *this; }

	/*
	== operator overload.  Determines if this string is equal to another.  Equivalent
	to a call to Equals.

	@param _other - the string to compare to
	@return (bool) - true if equal, false otherwise
	*/
	template <typename B>
	bool operator == (const ZBasicString<B>& _other) const 
		{ return Equals(_other); }

	//C-String Implementation of operator ==
	bool operator == (const char* _other) const 
		{ return Equals(_other); }

	/*
	!= operator overload.  Determines if this string is not equal to another.
	Equivalent to a call to !Equals.

	@param B - the allocator type for the other string
	@param _other - the string to compare to
	@return (bool) - true if not equal, false otherwise
	*/
	template <typename B>
	bool operator != (const ZBasicString<B>& _other) const 
		{ return !Equals(_other); }

	//C-String Implementation of operator !=
	bool operator != (const char* _other) const 
		{ return !Equals(_other); }

	/*
	< operator overload.  Lexographically compares strings.

	@param B - the allocator type for the other string
	@param _other - the string to compare to
	@return (bool) - true if less than, false otherwise
	*/
	template <typename B>
	bool operator < (const ZBasicString<B>& _other) const 
		{ return Compare(_other) < 0; }

	//C-String implementation of operator <
	bool operator < (const char* _other) const 
		{ return Compare(_other); }

	/*
	< operator overload.  Lexographically compares strings.  Equivalent to a call
	to Compare.

	@param B - the allocator type for the other string
	@param _other - the string to compare to
	@return (bool) - true if less than, false otherwise
	*/
	template <typename B>
	bool operator <= (const ZBasicString<B>& _other) const 
		{ return Compare(_other) <= 0; }

	/*
	+ operator overload.  Returns a string that is the concatenation of this
	string and the provided string.

	@param B - the allocator type for the other string
	@param _other - the string to append
	@return (ZBasicString<A>) - a string that is this string appended with _other
	*/
	template <typename B>
	ZBasicString<A> operator + (const ZBasicString<B>& _other) const 
		{ ZBasicString<A> ret(*this); ret.Insert(ret.Length(), _other); return ret; }

	//C-String implementation of operator +
	ZBasicString<A> operator + (const char *_other) const 
		{ ZBasicString<A> ret(*this); ret.Insert(ret.Length(), _other); return ret; }

	/*
	+ operator overload.  Returns a string that is the concatenation of this 
	string and the provided char.

	@param _c - the character to append
	@return (ZBasicString<A>) - a string that is this string appended with _c
	*/
	ZBasicString<A> operator + (const char& _c) const 
		{ ZBasicString<A> ret(*this); ret.PushBack(_c); return ret; }
	
	/*
	+= operator overload.  Inserts the given string at the end of this string.  Equivalent
	to a call to Insert.

	@param B - the allocator type for the other string
	@param _other - the string to append
	@return (ZBasicString<A>&) - this string
	*/
	template <typename B>
	ZBasicString<A>& operator += (const ZBasicString<B>& _other) 
		{ Insert(Length(), _other); return *this; }

	//C-String Implementation of operator +=
	ZBasicString<A>& operator += (const char *_other) 
		{ Insert(Length(), _other); return *this; }

	/*
	+= operator overload.  Pushes a character onto this string.  Equivalent to a call to
	PushBack.

	@param _c - the character to append
	@return (ZBasicString<A>&) - this string
	*/
	ZBasicString<A>& operator += (const char& _c) 
		{ PushBack(_c); return *this; }

	/*
	ZHashValue value override.  Uses the Java 6 string hash function.  Equivalent to a call
	to Hash.

	@return (ZHashValue) - hash code for this string
	*/
	operator ZHashValue () const 
		{ return Hash(); }

	/*
	public ZBasicString<A>::Allocator
	
	Gets the allocator set on this string.
	
	@return (A&) - the allocator instance
	*/
	A& Allocator() 
	{ 
		return StringArray.Allocator(); 
	}

	/*
	public ZBasicString<A>::Array
	
	Gets this string's backing ZArray.  The last character in the array is guaranteed to
	be ZBASICSTRING_NULL_TERMINATOR.  Try to keep it that way.
	
	@return (ZArray<char, A>&) - the string as a ZArray.
	*/
	const ZArray<char, A>& Array() const
	{ 
		return StringArray; 
	}

	/*
	public ZBasicString<A>::At
	
	Gets the character at the given signed index.  Passing in an index of -1 will return
	the last character, not the null terminator.
	
	@param _index - the index 
	@return (char) - the character at the provided index
	@assert - if the index is out of bounds
	*/
	char& At(const size_t _index) const
	{ 
		return StringArray.Data()[BoundsCheck(_index, Length())]; 
	}

	/*
	public ZBasicString<A>::Back
	
	Gets a reference to the character at the back of the string.
	
	@return (char&) - the character at the end of the string
	@assert - if the string is empty
	*/
	char& Back() const 
	{ 
		return StringArray.Data()[BoundsCheck(Length() - 1, Length())]; 
	}
	
	/*
	public ZBasicString<A>::BoundsCheck
	
	Bounds checks the provided index against the provided boundary, ensuring it is less
	than and  asserting if an out of bounds access occurs.  Calls to this will be compiled 
	out if ZSTL_DISABLE_RUNTIME_CHECKS is not zero.
	
	@param _index - the index to convert
	@param _boundary - the boundary to bounds check against
	@return (size_t) - the unsigned index
	@assert - if the index is not less than boundary
	*/
	size_t BoundsCheck(const size_t _index, const size_t _boundary) const 
	{ 
		return StringArray.BoundsCheck( _index, _boundary ); 
	}

	/*
	public ZBasicString<A>::Capacity
	
	Returns the capacity of this string.
	
	@return (size_t) - capacity of the string
	*/
	size_t Capacity() const 
	{ 
		return StringArray.Capacity() - 1; 
	}

	/*
	public ZBasicString<A>::Clear

	Clears out the string to the empty string.

	@return (void)
	*/
	void Clear()
	{ 
		StringArray.Clear(); 
		StringArray.PushBack(ZBASICSTRING_NULL_TERMINATOR); 
	}

	/*
	public ZBasicString<A>::Clear
	
	Clears out the string to the empty string and ensures the capacity of
	the string to the provided capacity.  Will reallocate if necessary.
	
	@param _newCapacity - the new capacity to use
	@return (void)
	*/
	void Clear(size_t _newCapacity) 
	{ 
		StringArray.Clear(_newCapacity + 1); 
		StringArray.PushBack(ZBASICSTRING_NULL_TERMINATOR); 
	}

	/*
	public ZBasicString<A>::Compare
	
	Lexicographically compares this string with another.
	
	@param B - the other string allocator type
	@param _other - the other string
	@return (int) - zero if lexicographically equivalent, less than zero if this < _other, 
					greater than zero if this > _other 
	*/
	template <typename B>
	int Compare(const ZBasicString<B>& _other) const
	{
		return strcmp(StringArray.Data(), _other.Data()); 
	}

	//C-String Implementation of Compare
	int Compare(const char *_other) const 
	{
		return strcmp(StringArray.Data(), _other);
	}

	/*
	public ZBasicString<B>::Copy
	
	Copies the given string data into this string.
	
	@param B - the other string allocator type
	@param _other - the other string
	@return (void)
	*/
	template <typename B>
	void Copy(const ZBasicString<B>& _other) 
	{ 
		StringArray.Resize(_other.Length() + 1); 
		StringArray.Copy(_other.Array()); 
	}

	//C-String Implementation of Copy
	void Copy(const char* _other)
		{ Copy(ZBasicString<A>(_other)); }

	/*
	public ZBasicString<A>::Data
	
	Gets the character data from this string (NULL terminated C-style string).
	
	@return (char*) - this string as a char*
	*/
	char* Data() const 
	{ 
		return StringArray.Data(); 
	}

	/*
	public ZBasicString<A>::Empty

	Determines if the string is the empty string.

	@return (bool) - true if the string is empty string, false otherwise
	*/
	bool Empty() const
	{ 
		return StringArray.Data()[0] == ZBASICSTRING_NULL_TERMINATOR; 
	}

	/*
	public ZBasicString<A>::Equals
	
	Determines if this string is lexicographically equivalent to another.
	
	@param B - the allocator type of the other
	@param _other - the other string
	@return (bool) - true if equivalent, false otherwise
	*/
	template <typename B>
	bool Equals(const ZBasicString<B>& _other) const  
	{ 
		return Compare(_other) == 0; 
	}

	//C-String Implementation of Equals
	bool Equals(const char *_other) const 
		{ return Compare(_other) == 0; }

	/*
	public ZBasicString<A>::Erase

	Erase function.  Erases the character at the provided index.

	@param _index - the index of the character to erase
	@return (void)
	*/
	void Erase(const size_t _index)
	{
		size_t index = BoundsCheck(_index, Length());
		Erase(index, index + 1);
	}

	/*
	public ZBasicString<A>::Erase

	Erase function.  Erases characters between the given indices.

	@param _start - the starting index
	@param __end - the ending index index (exclusive)
	@return (void)
	@assert - if _start or _end out of bounds
			  if _end < _start
	*/
	void Erase(const size_t _start, const size_t _end)
	{
		if (_start == _end)
			return;

		size_t start = BoundsCheck(_start, Length());
		size_t end = BoundsCheck(_end, Length() + 1);

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(start <= end, "ZBasicString: Cannot erase with _end < _start!");
		#endif

		StringArray.Erase(start, end);

		CheckIntegrity();
	}

	/*
	public ZBasicString<A>::Front
	
	Gets a reference to the character at the front of the string.
	
	@return (char&) - the character at the front of the string
	@assert - if the string is empty
	*/
	char& Front() const 
	{ 
		return StringArray.Data()[BoundsCheck(0, Length())]; 
	}

	/*
	public ZBasicString<A>::Hash
	
	Returns a hash value for the string.  Uses the Java 6 string hashing method.
	This does not include the null terminator of the string.
	
	@return (ZHashValue) - the hash value for the string
	*/
	ZHashValue Hash() const 
	{
		size_t i;
		ZHashValue hash;

		for (hash = 0, i = 0; i < Length(); i++) 
			hash = (hash << 5) - hash + (StringArray.Data())[i];

		return hash;
	}

	/*
	public ZBasicString<A>::Insert

	Insert function.  Inserts a character into this string.

	@param _index - index to insert at
	@param _char - character to insert
	@return (void)
	@assert - if index is invalid
	*/
	void Insert(const size_t _index, const char& _char)
	{  
		size_t index = BoundsCheck(_index, Length() + 1);

		StringArray.Insert(index, _char);

		CheckIntegrity(); 
	}

	/*
	public ZBasicString<A>::Insert

	Insert function.  Inserts a string into this string.

	@param _index - index to insert at
	@param _other - string to insert
	@return (void)
	@assert - if index is invalid
	*/
	void Insert(const size_t _index, const ZBasicString<A>& _other)
	{  
		size_t index = BoundsCheck(_index, Length() + 1);

		StringArray.Insert(index, _other.Array(), 0, _other.Length());

		CheckIntegrity(); 
	}

	/*
	public ZBasicString<A>::Length

	Gives the length of the string, not including the null terminator.
	
	@return (size_t) - length of the string
	*/
	size_t Length() const
	{ 
		return StringArray.Size() - 1; 
	}

	/*
	public ZBasicString<A>::Pop

	Pops a character off the string.

	@return (char) - the character removed from the string
	@assert - if the string is empty
	*/
	char PopBack()
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(!Empty(), "ZBasicString: PopBack called on empty string!");
		#endif

		return StringArray.Erase(StringArray.Size() - 2);
	}

	/*
	public ZBasicString<A>::PopFront
	
	Pops a character off of the beginning of the string.
	
	@return (char)
	@assert - if the string is empty
	*/
	char PopFront()
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(!Empty(), "ZBasicString: PopBack called on empty string!");
		#endif

		return StringArray.PopFront();
	}

	/*
	public ZBasicString<A>::PushBack

	Pushes a character onto the string.  Will allocate storage if necessary.

	@param _char - character to push onto the string
	@return (void)
	*/
	void PushBack(const char& _char)
	{
		StringArray.PushBack(ZBASICSTRING_NULL_TERMINATOR);
		StringArray.At(StringArray.Size() - 2) = _char;
	}

	/*
	public ZBasicString<A>::PushFront
	
	Pushes a character onto the beginning of the string.  Will allocate storage
	if necessary.
	
	@param _char - the character to push onto the string
	@return (void)
	*/
	void PushFront(const char& _char)
	{ 
		StringArray.PushFront(_char); 
	};

	/*
	public ZBasicString<A>::Reserve
	
	Changes the capacity of this string, reserving space in the underlying array for the
	provided number of characters and the null terminator.
	
	@param _capacity - the capacity to use
	@return (void)
	@assert - if the capacity is less than current size
	*/
	void Reserve(size_t _capacity)
	{ 
		StringArray.Reserve(_capacity + 1); 
	}

	/*
	public ZBasicString<A>::Resize
	
	Resizes this string.  If the string size is shrunk, removes the excess characters.  If
	the string size is grown, the whitespace character is appended that many times.  Will
	allocate storage if necessary.
	
	@param _newSize - the new size of the string
	@return (void)
	*/
	void Resize(size_t _newSize) 
	{ 
		Resize(_newSize, ' '); 
	}

	/*
	public ZBasicString<A>::Resize
	
	Resizes this string.  If the string size is shrunk, removes the excess characters.  If
	the string size is grown, the provided character is appended that many times.  Will 
	allocate storage if necessary.
	
	@param _newSize - the new size of this string
	@param _value - the value to append if size increases
	@return (void)
	*/
	void Resize(size_t _newSize, const char& _value) 
	{ 
		StringArray.PopBack();						//Remove null terminator
		StringArray.Resize(_newSize + 1, _value);	//Include room to replace the null terminator 
		StringArray.At(this->Length()) = ZBASICSTRING_NULL_TERMINATOR;
		CheckIntegrity(); 
	}

	/*
	public ZBasicString<A>::Swap

	Swap operation, which swaps string content.

	@param B - the allocator type of the other string
	@param _other - string to swap contents with
	@return (void)
	*/
	template <typename B>
	void Swap(ZBasicString<B>& _other)
	{ 
		StringArray.Swap(_other.StringArray); 
		CheckIntegrity(); 
	}
};

///////////////////////////////////////////
/* Non-Member Functions for ZBasicString */
///////////////////////////////////////////

/*
Non member addition function.  Allows adding a const char* to a ZBasicString<A> 
with the CString as the right side of the operator.

@param _lhs - string to append to the left side
@param _rhs - char * to append to
@return - a string that is the concatenation of _lhs with _rhs
*/
template <typename A>
ZBasicString<A> operator + (const ZBasicString<A>& _lhs, const char *_rhs)
{
	return _lhs + ZBasicString<A>(_rhs);
}

/*
Non member addition function.  Allows adding a ZBasicString<A> to a const char * 
with the CString as the left side of the operator.

@param _lhs - char * to append to
@param _rhs - string to append to the left side
@return - a string that is the concatenation of _lhs with _rhs
*/
template <typename A>
ZBasicString<A> operator + (const char *_lhs, const ZBasicString<A>& _rhs)
{
	return ZBasicString<A>(_lhs) + _rhs;
}

/*
Output stream operator overload for ZBasicString.  Needed to ensure ZBasicString 
behaves as expected when trying to use std::cout or std::cerr.

@param _os - the output stream
@param _str - the basic string
@return - the output stream provided
*/
template <typename A>
std::ostream& operator << (std::ostream& _os, const ZBasicString<A>& _str)
{
	_os << _str.Data();	return _os;
}

#endif
