/*
	ZListAlgo.hpp
	Author: James Russell <jcrussell@762studios.com>
	Created: 1/12/2012

	Purpose: 

	Generalized algorithm implementations for use with ZList.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _ZLISTALGO_HPP
#define _ZLISTALGO_HPP

#include <ZSTL/ZSTL.hpp>

namespace ZListAlgo
{
	/*
	public ZListAlgo<T, A, B>::Append
	 
	Appends a range of one list's elements to the back of another.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param B - the allocator type used by the other list
	@param _list - list to append elements to
	@param _other - list to get elements from
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (void)
	@assert - if _start, _end don't belong to _other
	*/
	template <typename T, typename A, typename B>
	void Append(ZList<T, A>& _list, const ZList<T, B>& _other, const ZListIterator<T>& _start, const ZListIterator<T>& _end)
	{
		URFP(_other);
		typename ZList<T, B>::Iterator itr;
		for (itr = _start; itr !=_end; ++itr)
			_list.PushBack(*itr);
	}
	
	/*
	public ZListAlgo<T, A, B>::Append
	 
	Appends one list's elements to the back of another.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param B - the allocator type used by the other list
	@param _list - list to append elements to
	@param _other - list to get elements from
	@return (void)
	*/
	template <typename T, typename A, typename B>
	void Append(ZList<T, A>& _list, const ZList<T, B>& _other)
	{
		Append(_list,_other, _other.Begin(), _other.End());
	}


	/*
	public ZListAlgo<T, A, B>::Concatenate
	 
	Function to concatenate ranges of two lists together and returns the result.
	 
	@param _list - the first list to concatenate
	@param _listStart - the starting iterator for _list
	@param _listEnd - the ending iterator for _list (exclusive)
	@param _other - the second list to concatenate
	@param _otherStart - the starting iterator for _other
	@param _otherEnd - the second iterator for _other (exclusive)
	@return (ZList<T, A>) - concatenated list with copies of elements from lists.
	*/
	template <typename T, typename A, typename B>
	ZList<T,A> Concatenate(const ZList<T, A>& _list, const ZListIterator<T>& _listStart, const ZListIterator<T>& _listEnd,
						   const ZList<T, B>& _other, const ZListIterator<T>& _otherStart, const ZListIterator<T>& _otherEnd)
	{
		URFP(_list);	//TODO: Need to check to ensure _listStart, _listEnd iterate _list
		URFP(_other);	//TODO: Need to check to ensure _otherStart, _otherEnd iterate _other

		ZListIterator<T> litr;
		ZListIterator<T> oitr;

		ZList<T,A> ret;

		for (litr = _listStart; litr != _listEnd; ++litr)
			ret.PushBack(*litr);

		for (oitr = _otherStart; oitr != _otherEnd; ++oitr)
			ret.PushBack(*oitr);

		return ret;
	}

	/*
	public ZListAlgo<T, A, B>::Concatenate
	 
	Function to concatenate two lists together.
	 
	@param _list - the first list to concatenate
	@param _other - the second list to concatenate
	@return (ZList<T,A>) - list of form [list, other], made of copies of their elements 
	*/
	template <typename T, typename A, typename B>
	ZList<T,A> Concatenate(const ZList<T, A>& _list, const ZList<T, B>& _other)
	{
		return Concatenate(_list, _list.Begin(), _list.End(), _other, _other.Begin(), _other.End());
	}

	/*
	public ZListAlgo<T, A>::Contains
	
	Determines if the list contains the given value between the given iterators.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to search in
	@param _value - the value to look for
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (bool) - true if the range contains the value, false otherwise
	*/
	template <typename T, typename A>
	bool Contains(const ZList<T, A>& _list, const T& _value, const ZListIterator<T>& _start, const ZListIterator<T>& _end)
	{
		URFP(_list);	//TODO: Need to check to ensure _start, _end iterate _list

		ZListIterator<T> itr;
		
		for (itr = _start; itr != _end; ++itr)
		{
			if (*itr == _value)
				return true;
		}

		return false;
	}

	/*
	public ZListAlgo<T, A>::Count

	Counts the number of occurrences of a value in the list between the given iterators.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to count elements in
	@param _value - the value to look for
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (size_t) - the number of occurrences of the value
	*/
	template <typename T, typename A>
	size_t Count(const ZList<T, A>& _list, const T& _value, const ZListIterator<T>& _start, const ZListIterator<T>& _end)
	{
		URFP(_list);	//TODO: Need to check to ensure _start, _end iterate _list

		size_t i;
		typename ZList<T, A>::Iterator itr;

		for (i = 0, itr = _start; itr != _end; ++itr)
		{
			if ((*itr) == _value)
				i++;
		}

		return i;
	}

	/*
	public ZListAlgo<T, A>::Count

	Counts the number of occurrences of a value in the list.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to count elements in
	@param _value - the value to look for
	@return (size_t) - the number of occurrences of the value
	*/
	template <typename T, typename A>
	size_t Count(const ZList<T, A>& _list, const T& _value) 
	{
		return Count(_list, _value, _list.Begin(), _list.End());
	}


	/*
	public ZListAlgo<T, A>::Excise
	 
	Excises a region from a list and returns it.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to excise a region from
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (ZList<T, A>) - the excised region
	*/
	template <typename T, typename A>
	ZList<T,A> Excise(ZList<T, A>& _list, const ZListIterator<T>& _start, const ZListIterator<T>& _end)
	{
		URFP(_list);	//TODO: Need to check to ensure _start, _end iterate _list
		
		ZList<T, A> ret;
		ZListIterator<T> itr;

		itr = _start;

		while (itr != _end)
			ret.PushBack(_list.Erase(itr));

		return ret;
	}

	/*
	public ZListAlgo<T, A>::FindFirstOf

	Searches for the first occurrence of the specified value in the list between
	the given iterators.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to search in
	@param _value - the value to search for
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (ZListIterator<T>) - the iterator location where the value is found (_end if not found)
	*/
	template <typename T, typename A>
	ZListIterator<T> FindFirst(const ZList<T, A>& _list, const T& _value, const ZListIterator<T>& _start, const ZListIterator<T>& _end) 
	{
		URFP(_list);	//TODO: Need to check to ensure _start, _end iterate _list

		ZListIterator<T> itr;

		for (itr = _start; itr != _end; ++itr)
			if ((*itr) == _value)
				return itr;

		return _list.End();
	}
	
	/*
	public ZListAlgo<T, A>::FindFirstOf

	Searches for the first occurrence of the specified value in the list.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to search in
	@param _value - the value to search for
	@return (ZListIterator<T>) - the iterator location where the value is found (_end if not found)
	*/
	template <typename T, typename A>
	ZListIterator<T> FindFirst(const ZList<T, A>& _list, const T& _value)
	{
		return FindFirst(_list, _value, _list.Begin(), _list.End());
	}


	/*
	public ZListAlgo<T, A>::Contains
	
	Determines if the list contains the given value.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to search in
	@param _value - the value to look for
	@return (bool) - true if the list contains the value, false otherwise
	*/
	template <typename T, typename A>
	bool Contains(const ZList<T, A>& _list, const T& _value)
	{
		return FindFirst(_list, _value) != _list.End();
	}

	/*
	public ZListAlgo<T, A>::FirstNotOf
	 
	Function to find the first occurrence of a value that is not the given value in the given range and 
	returns an iterator to it.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to search in
	@param _value - the value to avoid
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (ZListIterator<T>) - iterator pointing at the first non-match (_end if not found)
	*/
	template <typename T, typename A>
	ZListIterator<T> FirstNotOf(const ZList<T, A>& _list, const T& _value, const ZListIterator<T>& _start, const ZListIterator<T>& _end)
	{
		URFP(_list);	//TODO: Need to check to ensure _start, _end iterate _list

		ZListIterator<T> itr;

		for (itr = _start; itr != _end; ++itr)
			if (!(*itr == _value))
				break;

		return itr;
	}

	/*
	public ZListAlgo<T, A>::FirstNotOf

	Function to find the first occurrence of a value that is not the given value and 
	returns an iterator to it.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to search in
	@param _value - the value to search for the first nonoccurence of
	@return (ZListIterator<T>) - iterator pointing at the first non-match (_end if not found)
	*/
	template <typename T, typename A>
	ZListIterator<T> FirstNotOf(const ZList<T, A>& _list, const T& _value)
	{
		return FirstNotOf(_list, _value, _list.Begin(), _list.End());
	}

	/*
	public ZListAlgo<T, A, B>::Prepend
	 
	Appends copies of a range of a list's elements to the front of another list.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param B - the allocator type of the other list
	@param _list - list to append elements to
	@param _other - list to get elements from
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (void)
	*/
	template <typename T, typename A, typename B>
	void Prepend(ZList<T, A>& _list, const ZList<T, B>& _other, const ZListIterator<T>& _start, const ZListIterator<T>& _end)
	{
		URFP(_other);
		_list.Insert(_list.Begin(), _start, _end);
	}

	/*
	public ZListAlgo<T, A, B>::Prepend
	 
	Appends copies of a list's elements to the front of another list.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param B - the allocator type of the other list
	@param _list - list to append elements to
	@param _other - list to get elements from
	@return (void)
	*/
	template <typename T, typename A, typename B>
	void Prepend(ZList<T, A>& _list, const ZList<T, B>& _other)
	{
		Prepend(_list, _other, _other.Begin(), _other.End());
	}

	/*
	public ZListAlgo<T, A>::Remove

	Removes the first occurrence of the specified value from the list starting at
	the given iterator location.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to remove elements from
	@param _value - the value to remove
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (ZListIterator<T>) - iterator to the position after the removed element
	*/
	template <typename T, typename A>
	ZListIterator<T> Remove(ZList<T, A>& _list, const T& _value, const ZListIterator<T>& _start, const ZListIterator<T>& _end) 
	{
		URFP(_list);	//TODO: Need to check to ensure _start, _end iterate _list

		ZListIterator<T> itr = _start;

		while (itr != _end)
		{
			if ((*itr) == _value)
			{
				_list.Erase(itr);
				return itr;
			}

			++itr;
		}

		return _list.End();
	}

	/*
	public ZListAlgo<T, A>::Remove
	
	Removes the first occurrence of the specified value from the list.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to remove elements from
	@param _value - the value to remove
	@return (ZListIterator<T>) - iterator to the position after the removed element
	*/
	template <typename T, typename A>
	ZListIterator<T> Remove(ZList<T, A>& _list, const T& _value) 
	{
		return Remove(_list, _value, _list.Begin(), _list.End());
	}


	/*
	public ZListAlgo<T, A>::RemoveAll

	Removes all occurrences of the specified value from the list between the given iterators.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to remove elements from
	@param _value - the value to remove
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (size_t) - the number of elements removed
	*/
	template <typename T, typename A>
	size_t RemoveAll(ZList<T, A>& _list, const T& _value, const ZListIterator<T>& _start, const ZListIterator<T>& _end) 
	{
		URFP(_list);	//TODO: Need to check to ensure _start, _end iterate _list

		size_t removed = 0;
		ZListIterator<T> temp = _start;

		while (temp != _end)
		{
			if (*temp == _value)
			{
				_list.Erase(temp);
				removed++;
			}
			else
			{
				++temp;	//Erase will increment above
			}
		}

		return removed;
	}

	/*
	public ZListAlgo<T, A>::RemoveAll

	Removes all occurrences of the specified value from the list.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to remove elements from
	@param _value - the value to remove
	@return (size_t) - the number of elements removed
	*/
	template <typename T, typename A>
	size_t RemoveAll(ZList<T, A>& _list, const T& _value) 
	{
		return RemoveAll(_list, _value, _list.Begin(), _list.End());
	}


	/*
	public ZListAlgo<T, A>::Reverse
	 
	Reverses the elements of a list in a given range.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to reverse a range on
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (void)
	*/
	template <typename T, typename A>
	void Reverse(ZList<T, A>& _list, const ZListIterator<T>& _start, const ZListIterator<T>& _end)
	{
		if (_list.Size() < 2)
			return;

		ZListIterator<T> start = _start;
		ZListIterator<T> end = _end;
		end.Prev();
		T temp;

		while (start != end)
		{
			temp = *start;
			*start = *end;
			*end = temp;

			if (start.GetNode()->Next == end.GetNode() && end.GetNode()->Previous == start.GetNode())
				break;
			if (start.GetNode()->Next == end.GetNode()->Previous)
				break;
			++start;
			--end;
		}		
	}

	/*
	public ZListAlgo<T, A>::Reverse
	 
	Reverses the elements in a list.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to reverse
	@return (void)
	*/
	template <typename T, typename A>
	void Reverse(ZList<T, A>& _list)
	{
		Reverse(_list, _list.Begin(), _list.End());
	}


	/*
	public ZListAlgo<T, A>::ReverseNodes
	 
	Reverses the elements of a list in a given range.  Use when operator = for the 
	contained element is expensive.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to reverse a range on
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (void)
	*/
	template <typename T, typename A>
	void ReverseNodes(ZList<T, A>& _list, const ZListIterator<T>& _start, const ZListIterator<T>& _end)
	{
		URFP(_list);	//TODO: Need to check to ensure _start, _end iterate _list

		ZListNode<T>* startBoundNode = _start.GetNode()->Previous;
		ZListNode<T>* endBoundNode = _end.GetNode();
		ZListNode<T>* currNode = _start.GetNode();

		ZListNode<T>* temp;

		// don't reverse sublist of only 1 element
		if (currNode == endBoundNode->Previous)
			return;

		// setup the start node and end boundary
		startBoundNode->Next = endBoundNode->Previous;
		endBoundNode->Previous = currNode;

		// swap node next and prev pointers
		while (currNode != endBoundNode)
		{
			temp = currNode->Next;
			ZSTL_ASSERT(temp != NULL, "temp null in Reverse!()\n");
			currNode->Next = currNode->Previous;
			currNode->Previous = temp;
			currNode = temp;
		}

		// setup the end node and the first boundary
		currNode->Previous = startBoundNode;		

		// setup the first node and the end boundary
		currNode = _start.GetNode();
		currNode->Next = endBoundNode;		
	}

	/*
	public ZListAlgo<T, A>::ReverseNodes
	 
	Reverses the nodes in a given list.  Use when operator = for the 
	contained element is expensive.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to reverse
	@return (void)
	*/
	template <typename T, typename A>
	void ReverseNodes(ZList<T, A>& _list)
	{
		ReverseNodes(_list, _list.Begin(), _list.End());
	}


	/*
	public ZListAlgo<T, A>::Slice
	 
	Gets a copy of the elements in a subset of a list.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to get a slice from
	@param _start - the beginning (inclusive) of the slice
	@param _end - the end (exclusive) of the slice
	@return (ZList<T, A>) - slice list
	*/
	template <typename T, typename A>
	ZList<T, A> Slice(const ZList<T, A>& _list, const ZListIterator<T>& _start, const ZListIterator<T>& _end)
	{
		URFP(_list);	//TODO: Need to check to ensure _start, _end iterate _list

		return ZList<T, A>(_start, _end);
	}

	/*
	public ZListAlgo<CF, T, A>::Sort

	Sorts the provided list in place between the given iterators using the provided comparator and algorithm.

	@param CF - binary comparator functor to use [(const T&, const T&) -> int] that compares values
				returns negative value if first value < second value
				returns positive value if first value > second value
				returns 0 if first value == second value
	@param AF - ternary algorithm functor to use [(CF&, ZListNode<T>*, ZListNode<T>*) -> void] that sorts the list
				the first argument is a reference to the provided comparator
				the second argument is a pointer to the starting node
				the third argument is a pointer to the ending node (exclusive)
	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to sort
	@param _comparator - the comparator function used to compare the values
	@param _algorithm - the algorithm to use to sort the list
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (void)
	*/
	template <typename CF, typename AF, typename T, typename A>
	void Sort(ZList<T, A>& _list, CF _comparator, AF _algorithm, ZListIterator<T>& _start, ZListIterator<T>& _end)
	{
		URFP(_list); //TODO: ensure that start/end came from list
		_algorithm(_comparator, _start.GetNode(), _end.GetNode());
	}

	/*
	public ZListAlgo<T, A>::Sort

	Sorts the provided list in place using the default comparator (uses operator <) and 
	a non-stable merge sort.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to sort
	@return (void)
	*/
	template <typename T, typename A>
	void Sort(ZList<T, A>& _list)
	{
		ZComparator<T> comparator;
		ZListMergeSort<T> algorithm; 
		
		ZListIterator<T> start = _list.Begin();
		ZListIterator<T> end = _list.End();

		Sort(_list, comparator, algorithm, start, end);
	}

	/*
	public ZListAlgo<T, A>::Sort

	Sorts the provided list in place between the given iterators using the default 
	comparator (uses operator <) and a non-stable merge sort.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to sort
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (void)
	*/
	template <typename T, typename A>
	void Sort(ZList<T, A>& _list, ZListIterator<T>& _start, ZListIterator<T>& _end)
	{
		ZComparator<T> comparator;
		ZListMergeSort<T> algorithm; 
		Sort(_list, comparator, algorithm, _start, _end);
	}

	/*
	public ZListAlgo<CF, T, A>::Sort

	Sorts the provided list in place using the provided comparator and a 
	non-stable merge sort.

	@param CF - binary comparator functor to use [(const T&, const T&) -> int] that compares values
				returns negative value if first value < second value
				returns positive value if first value > second value
				returns 0 if first value == second value
	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to sort
	@param _comparator - the comparator function used to compare the values
	@return (void)
	*/
	template <typename CF, typename T, typename A>
	void Sort(ZList<T, A>& _list, CF _comparator)
	{
		ZListMergeSort<T> algorithm; 
		ZListIterator<T> begin = _list.Begin();
		ZListIterator<T> end = _list.End();
		Sort(_list, _comparator, algorithm, begin, end);
	}

	/*
	public ZListAlgo<CF, T, A>::Sort

	Sorts the provided list in place using the provided comparator and algorithm.

	@param CF - binary comparator functor to use [(const T&, const T&) -> int] that compares values
				returns negative value if first value < second value
				returns positive value if first value > second value
				returns 0 if first value == second value
	@param AF - ternary algorithm functor to use [(CF&, ZListNode<T>*, ZListNode<T>*) -> void] that sorts the list
				the first argument is a reference to the provided comparator
				the second argument is a pointer to the starting node
				the third argument is a pointer to the ending node (exclusive)
	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to sort
	@param _comparator - the comparator function used to compare the values
	@param _algorithm - the algorithm to use to sort the list
	@return (void)
	*/
	template <typename CF, typename AF, typename T, typename A>
	void Sort(ZList<T, A>& _list, CF _comparator, AF _algorithm)
	{
		ZListIterator<T> begin = _list.Begin();
		ZListIterator<T> end = _list.End();
		Sort(_list, _comparator, _algorithm, begin, end);
	}


	/*
	public ZListAlgo<T, A>::Split

	Splits the list at the specified location.  The provided list is modified to be the first half,
	the returned list is the second half.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _list - the list to split
	@param _itr - iterator location to split (invalidated by this call)
	@return (ZList<T, A>) - the split second half of the list
	*/
	template <typename T, typename A>
	ZList<T, A> Split(ZList<T, A>& _list, const ZListIterator<T>& _itr) 
	{
		URFP(_list);	//TODO: Need to check to ensure _itr iterates _list

		ZListIterator<T> itr = _itr;

		ZList<T, A> splitList(itr, _list.End());

		_list.Clear(itr);

		return splitList;
	}

	/*
	public ZListAlgo<T, A>::SwapElements

	Swaps the elements at two locations in this list.  Both iterators are modified
	to point to new nodes, but they remain at the same location.

	@param T - the type held by the list
	@param A - the allocator used by the list
	@param _itr1 - iterator to the first element
	@param _itr2 - iterator to the second element
	@return (void)
	*/
	template <typename T, typename A>
	void SwapElements(ZList<T, A>& _list, const ZListIterator<T>& _itr1, const ZListIterator<T>& _itr2) 
	{
		URFP(_list);	//TODO: Need to check to ensure _itr1, _itr2 iterates _list

		T temp = *_itr2;
		*_itr2 = *_itr1;
		*_itr1 = temp;
	}
}

#endif
 
