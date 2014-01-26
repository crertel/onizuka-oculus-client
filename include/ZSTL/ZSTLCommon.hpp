/*
	ZSTLCommon.hpp
	Author: James Russell <jcrussell@762studios.com>
	Created: 9/13/2011

	Purpose: 

	Common include file used by all ZSTL components.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _ZSTLCOMMON_HPP
#define _ZSTLCOMMON_HPP

#include <pstdint.h>			//Need this for our int types
#include <new>					//This gives us (std::nothrow)
#include <assert.h>				//This gives us assert
#include <stdlib.h>				//This gives us a definition for NULL

//Hash Value Types
typedef uint32_t ZHashValue;
typedef uint32_t ZHashValue32;
typedef uint64_t ZHashValue64;

//Used to get rid of the (/W4 or -Wall) warning 'Unreferenced Formal Parameter'
#ifndef URFP
#define URFP(x)								((void)x)
#endif

//Default allocation macro used by ZSTL default allocators
#ifndef ZSTL_NEW
#define ZSTL_NEW(_type)						new (std::nothrow) _type()
#endif

//Default delete macro used by ZSTL default allocators
#ifndef ZSTL_DEL
#define ZSTL_DEL(_object)					delete _object
#endif

//Default array allocation macro used by ZSTL default allocators
#ifndef ZSTL_NEW_ARRAY
#define ZSTL_NEW_ARRAY(_type, _size)		new (std::nothrow) _type[_size]
#endif

//Default array delete macro used by ZSTL default allocators
#ifndef ZSTL_DEL_ARRAY
#define ZSTL_DEL_ARRAY(_ptr, _size)			((void)_size); delete[] _ptr
#endif

//Default assert macro used by ZSTL
#ifndef ZSTL_ASSERT
#define ZSTL_ASSERT(condition, message)		(assert(condition && message))
#endif

//Error condition macro used by ZSTL
#ifndef ZSTL_ERROR
#define ZSTL_ERROR(message)					(assert(0 && message))
#endif

//Swap used in array quicksort (takes array, first index, second index)
#define ZSTL_ARRAY_ELEMENT_SWAP(_a, _i, _j) temp = _a[_j]; _a[_j] = _a[_i]; _a[_i] = temp

//Push used in list mergesort (takes list node and new node)
#define ZSTL_LIST_ELEMENT_PUSH(_l, _n) if (_l == NULL) { _l = _n; } else { _l->Next = _n; _n->Previous = _l; _l = _n; }

/*
public BoundsCheck<I>

Function that checks the provided index against the provided boundary, asserting if an 
out of bounds access occurs.  Calls to this will be compiled out if ZSTL_DISABLE_RUNTIME_CHECKS 
is not zero.

@param I - the type of index
@param _index - index to check
@param _boundary - the (exclusive) boundary to use for bounds checking
@return (size_t) - unsigned index
@assert - if the index is not less than boundary
*/
template <typename I>
size_t BoundsCheck(I _index, I _boundary)
{
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(_index < _boundary, "ZSTL: Out of bounds access!");
	#endif

	return (size_t)_index;
}

/*
Node class, which is used by ZList to contain list data and previous / next pointers.  Could
be used by other classes that also need basic list-like functionality.

The template parameter T is the type contained by the list node.
*/
template <typename T>
struct ZListNode
{
	//Default Constructor
	ZListNode() 
		: Next(NULL), Previous(NULL), Element() { }

	//Parameterized Constructor
	ZListNode(ZListNode<T>* _next, ZListNode<T>* _previous)
		: Next(_next), Previous(_previous), Element() { }

	//Parameterized Constructor
	ZListNode(ZListNode<T>* _next, ZListNode<T>* _previous, const T& _value)
		: Next(_next), Previous(_previous), Element(_value) { }

	//The next node
	ZListNode* Next;

	//The previous node
	ZListNode* Previous;

	//The contained element
	T Element;

};

/*
Comparator functor, used when a delegate is needed to provide comparison information on 
elements.  This implementation uses operator < on elements.

The template parameter T is the type we will be comparing to each other.
*/
template <typename T>
struct ZComparator
{
	ZComparator() { } //Silence Sun C++ warnings about uninitialized structs

	//operator overload which returns -1 if a < b, 0 if a == b, and 1 if a > b
	int operator () (const T& _a, const T& _b) const
	{
		if (_a < _b)
		{
			return -1;
		}
		else if (_b < _a)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
};

/*
Array quicksort functor.  Uses a recursive in-place sort.  This implementation is 
not a stable sort.

The template parameter T is the type contained by the array we will be sorting.
*/
template <typename T>
struct ZArrayQuickSort
{
	ZArrayQuickSort() { } //Silence Sun C++ warnings about uninitialized structs

	//Helper function which partitions the array
	template <typename CF>
	inline size_t Partition(CF& _comparator, T* _array, size_t _left, size_t _right, size_t _pivot)
	{
		size_t i, j;
		T temp;

		//Get the value at the pivot point
		T pivotValue = _array[_pivot];

		//Move pivot to end
		ZSTL_ARRAY_ELEMENT_SWAP(_array, _pivot, _right);
		
		//Check values from left up to pivot
		for (i = _left, j = _left; i < _right; i++) 
		{
			//If less than the pivot value
			if (_comparator(_array[i], pivotValue) < 0)
			{
				//Swap back and increment our 'target' index j
				ZSTL_ARRAY_ELEMENT_SWAP(_array, i, j);
				j++;
			}
		}

		//Move pivot to final location (all values with index < j are < pivotValue)
		ZSTL_ARRAY_ELEMENT_SWAP(_array, j, _right); 

		return j;
	}

	//Helper function which performs the sorting
	template <typename CF>
	void QuickSort(CF& _comparator, T* _array, size_t _left, size_t _right)
	{
		size_t pivot;

		if (_right > _left)
		{
			//Center pivot point (guarded against overflow)
			pivot = _left + (_right - _left) / 2;

			//Get our next pivot after partitioning around the current
			pivot = Partition(_comparator, _array, _left, _right, pivot);

			//Sort the left partition
			if (pivot != 0)
				QuickSort(_comparator, _array, _left, pivot - 1);

			//Sort the right partition
			QuickSort(_comparator, _array, pivot + 1, _right);
		}
	}

	//Functor operator () override
	template <typename CF>
	void operator () (CF& _comparator, T* _array, size_t _size)
	{
		QuickSort(_comparator, _array, 0, _size - 1);
	}
};

/*
List merge sort functor.  This implementation is a 'stable' sort.

The template parameter T is the type contained by the underlying list we will be sorting.
*/
template <typename T>
struct ZListMergeSort
{
	ZListMergeSort() { } //Silence Sun C++ warnings about initialized structs

	//Special 'Len' function which uses no end node
	inline size_t Length(ZListNode<T>* _list)
	{
		size_t i = 0;
		ZListNode<T>* node;

		//for (i = 0, node = _list; node != NULL; i++, node = node->Next);

		node = _list;
		while (node != NULL)
		{
			node = node->Next;
			i++;
		}

		return i;
	}

	//Helper function which merges two lists, returns the last node
	template <typename CF>
	inline ZListNode<T>* Merge(CF& _comparator, ZListNode<T>* _left, ZListNode<T>* _right)
	{
		ZListNode<T>* merged = NULL;

		//While left and right still have elements
		while (_left != NULL && _right != NULL)
		{
			//Compare first elements
			if (_comparator(_left->Element, _right->Element) < 0)
			{
				//Add the left element
				ZSTL_LIST_ELEMENT_PUSH(merged, _left);
				_left = _left->Next;
			}
			else
			{
				//Add the right element
				ZSTL_LIST_ELEMENT_PUSH(merged, _right);
				_right = _right->Next;
			}
		}

		//While the left still has elements
		while (_left != NULL)
		{
			//Add them
			ZSTL_LIST_ELEMENT_PUSH(merged, _left);
			_left = _left->Next;
		}

		//While the right still has elements
		while (_right != NULL)
		{
			//Add them
			ZSTL_LIST_ELEMENT_PUSH(merged, _right);
			_right = _right->Next;
		}

		//Return the end node
		return merged;
	}

	//Helper function which merge sorts a list, returns the last node
	template <typename CF>
	inline ZListNode<T>* MergeSort(CF& _comparator, ZListNode<T>* _start)
	{
		size_t i;
		size_t middle;

		ZListNode<T>* left;
		ZListNode<T>* right;

		//If none or one element
		if (_start == NULL || _start->Next == NULL)
			return _start;

		//Determine midpoint
		middle = Length(_start) / 2;

		//Set our left and right
		left = right = _start;

		//Set right to midpoint
		for (i = 0; i < middle; i++)
			right = right->Next;

		//Seperate the list
		right->Previous->Next = NULL;
		right->Previous = NULL;

		//Sort left and right recursively
		MergeSort(_comparator, left);
		MergeSort(_comparator, right);

		//Set back our left list pointer
		while (left != NULL && left->Previous != NULL)
			left = left->Previous;

		//Set back our right list pointer
		while (right != NULL && right->Previous != NULL)
			right = right->Previous;

		//Return the last node from the merged lists
		return Merge(_comparator, left, right);
	}

	//Functor operator () override
	template <typename CF>
	void operator () (CF& _comparator, ZListNode<T>* _start, ZListNode<T>* _end) 
	{
		ZListNode<T>* prev;

		//Check to see if list is empty
		if (_start == _end)
			return;

		//Get the node before the starting node
		prev = _start->Previous;

		//Split the end node off
		_end->Previous->Next = NULL;
		_start->Previous = NULL;
		//Reset the end node back
		_end->Previous = MergeSort(_comparator, _start);

		//Attach
		_end->Previous->Next = _end;

		//Reattach the node before _start
		if (prev != NULL)
		{
			ZListNode<T>* current = _end;

			while (current->Previous != NULL)
				current = current->Previous;

			current->Previous = prev;
			prev->Next = current;
		}
	}
};

#endif
