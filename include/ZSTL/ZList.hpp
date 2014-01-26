/*
	ZList.hpp
	Author: James Russell <jcrussell@762studios.com>
	Created: 9/12/2011

	Purpose: 

	Templated doubly-linked list implementation.

	A primary difference between ZList and ZArray is that ZList (rather, the default allocators for
	ZList) make an absolute guarantee about scoped allocation / deallocation of objects.  This 
	means that a ZList using any of the default allocators is guaranteed to construct and destruct 
	list nodes as needed, and this means element constructors and destructors will be called as 
	elements are allocated and deallocated.

	This guarantee goes out the window when a custom allocator is used, and it becomes up to the 
	implementor to meet or disregard this guarantee as desired.  The list will, however, always 
	call out to the allocator whenever a node is needed, guaranteeing that the allocator is still
	called as elements enter and leave scope.

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

#ifndef _ZLIST_HPP
#define _ZLIST_HPP

#include <ZSTL/ZSTLCommon.hpp>
#include <ZSTL/ZSTLInvalidPos.hpp>

//Forward Declaration of ZList
template <typename T, typename A>
class ZList;

//Forward Declaration of ZListIterator
template <typename T>
class ZListIterator;

/* 
Forward Declaration of ZList Method Implementation Structures 

Existence of these structures allows for template specialization of 
individual methods of the ZList class.  In order to specialize
a single method of ZList, simply specialize the corresponding
method implementation structure.
*/

//Forward Declaration of ZList<T, A>::At struct
template <typename T, typename A>
struct ZList_AtImpl {
	inline static ZListIterator<T> Call(const ZList<T, A>& _self, size_t _index);
};

//Forward Declaration of ZList<T, A>::Back struct
template <typename T, typename A>
struct ZList_BackImpl {
	inline static T& Call(const ZList<T, A>& _self);
};

//Forward Declaration of ZList<T, A>::Clear struct
template <typename T, typename A>
struct ZList_ClearImpl {
	inline static void Call(ZList<T, A>& _self, ZListIterator<T>& _itr);
};

//Forward Declaration of ZList<T, A>::Copy struct
template <typename T, typename A>
struct ZList_CopyImpl {
	template <typename B> inline static void Call(ZList<T, A>& _self, const ZList<T, B>& _other);
};

//Forward Declaration of ZList<T, A>::Empty struct
template <typename T, typename A>
struct ZList_EmptyImpl {
	inline static bool Call(const ZList<T, A>& _self);
};

//Forward Declaration of ZList<T, A>::Equals struct
template <typename T, typename A>
struct ZList_EqualsImpl {
	template <typename B> inline static bool Call(const ZList<T, A>& _self, const ZList<T, B>& _other);
};

//Forward Declaration of ZList<T, A>::Erase struct
template <typename T, typename A>
struct ZList_EraseImpl {
	inline static T Call(ZList<T, A>& _self, ZListIterator<T>& _itr);
	inline static void Call(ZList<T, A>& _self, ZListIterator<T>& _start, const ZListIterator<T>& _end);
};

//Forward Declaration of ZList<T, A>::Find struct
template <typename T, typename A>
struct ZList_FindImpl {
	inline static ZListIterator<T> Call(const ZList<T, A>& _self, const T& _elem);
};

//Forward Declaration of ZList<T, A>::Front struct
template <typename T, typename A>
struct ZList_FrontImpl {
	inline static T& Call(const ZList<T, A>& _self);
};

//Forward Declaration of ZList<T, A>::Insert struct
template <typename T, typename A>
struct ZList_InsertImpl {
	inline static void Call(ZList<T, A>& _self, const ZListIterator<T>& _itr, const T& _value);
	inline static void Call(ZList<T, A>& _self, const ZListIterator<T>& _itr, const ZListIterator<T>& _start, const ZListIterator<T>& _end);
};

//Forward Declaration of ZList<T, A>::PopBack struct
template <typename T, typename A>
struct ZList_PopBackImpl {
	inline static T Call(ZList<T, A>& _self);
};

//Forward Declaration of ZList<T, A>::PopFront struct
template <typename T, typename A>
struct ZList_PopFrontImpl {
	inline static T Call(ZList<T, A>& _self);
};

//Forward Declaration of ZList<T, A>::PushBack struct
template <typename T, typename A>
struct ZList_PushBackImpl {
	inline static void Call(ZList<T, A>& _self, const T& _value);
};

//Forward Declaration of ZList<T, A>::PushFront struct
template <typename T, typename A>
struct ZList_PushFrontImpl {
	inline static void Call(ZList<T, A>& _self, const T& _value);
};

//Forward Declaration of ZList<T, A>::Size struct
template <typename T, typename A>
struct ZList_SizeImpl {
	inline static size_t Call(const ZList<T, A>& _self);
};

//Forward Declaration of ZList<T, A>::Splice struct
template <typename T, typename A>
struct ZList_SpliceImpl {
	inline static void Call(ZList<T, A>& _self, const ZListIterator<T>& _itr, ZList<T, A>& _other, const ZListIterator<T>& _start, const ZListIterator<T>& _end);
};

//Forward Declaration of ZList<T, A>::Swap struct
template <typename T, typename A>
struct ZList_SwapImpl {
	inline static void Call(ZList<T, A>& _self, ZList<T, A>& _other);
};

//Forward Declaration of ZList<T, A>::SwapNodes struct
template <typename T, typename A>
struct ZList_SwapNodesImpl {
	inline static void Call(ZList<T, A>& _self, const ZListIterator<T>& _i, const ZListIterator<T>& _j);
};

//////////////////////
/* ZList Allocators */
//////////////////////

/*
Allocator for ZList.  Handles allocation of nodes of the templated type.

The template parameter T is the type contained in the list this allocator is for.
*/
template <typename T>
class ZListAllocator
{
public:
	/*
	public ZListAllocator<T>::Allocate

	Allocator function which allocates a ZListNode<T>.

	@return - an allocated ZListNode<T>
	*/
	ZListNode<T>* Allocate()
	{
		return ZSTL_NEW(ZListNode<T>);
	}

	/*
	public ZListAllocator<T>::Deallocate

	Deallocation function which deallocates a previously allocated ZListNode<T>.

	@param _node - node to deallocate
	*/
	void Deallocate(ZListNode<T>* _node)
	{
		ZSTL_DEL(_node);
	}
};

/*
Alternate allocator for ZList  Handles allocation of nodes of the templated type,
and uses local storage to avoid allocations whenever possible.

Note:	This allocator is NOT suitable for a list that is later spliced using 
		ZList::Splice or has nodes swapped using ZList::Swap or ZList::SwapNodes.  
		It will cause errors when local nodes are deallocated by the other list's
		allocator.

The template parameter T is the type contained in the list this allocator is for.
*/
template <typename T, size_t N>
class ZListPooledAllocator
{
private:
	//Local storage space for nodes
	char			Local[N * sizeof(ZListNode<T>)];

	//Pointer storage for nodes (if available)
	ZListNode<T>*	Nodes[N];

	//Our current node index
	size_t			NodeIndex;

public:

	/*
	Default constructor, which initializes local storage.
	*/
	ZListPooledAllocator()
		: NodeIndex(0)
	{
		uintptr_t ptr = (uintptr_t)&Local[0];
		for (size_t i = 0; i < N; i++)
		{
			Nodes[i] = (ZListNode<T>*)ptr;
			ptr += sizeof(ZListNode<T>);
		}
	}

	/*
	public ZListPooledAllocator<T>::Allocate

	Allocator function which allocates a ZListNode<T>.

	@return - an allocated ZListNode<T>
	*/
	ZListNode<T>* Allocate()
	{
		if (NodeIndex < N)
		{
			ZListNode<T>* node = Nodes[NodeIndex];
			Nodes[NodeIndex++] = NULL;
			return new (node) ZListNode<T>();			
		}

		return ZSTL_NEW(ZListNode<T>);
	}

	/*
	public ZListPooledAllocator<T>::Deallocate

	Deallocation function which deallocates a previously allocated ZListNode<T>.

	@param _node - node to deallocate
	*/
	void Deallocate(ZListNode<T>* _node)
	{
		const uintptr_t nodePtr		= (uintptr_t)_node;
		const uintptr_t poolLower	= (uintptr_t)&Local[0];
		const uintptr_t poolUpper	= poolLower + (N-1)*sizeof(ZListNode<T>);

		if (nodePtr >= poolLower && nodePtr <= poolUpper)
		{
			ZSTL_ASSERT((nodePtr - poolLower) % sizeof(ZListNode<T>) == 0, "Node is not aligned to this list, check for address corruption");

			Nodes[--NodeIndex] = _node;
			_node->~ZListNode<T>();
		}
		else
		{
			ZSTL_DEL(_node);
		}		
	}
};

////////////////////
/* ZList Iterator */
////////////////////

/*
Iterator class for ZList.

The template parameter T is the type contained in the list this iterator is for.
*/
template <typename T>
class ZListIterator
{
public:
	/*
	Default Constructor.
	*/
	ZListIterator() 
		: Node(NULL), EndNode(NULL) { }

	/*
	Useful constructor.

	@param _node - the node we are to begin iterating at
	*/
	ZListIterator(ZListNode<T>* _node, ZListNode<T>* _endNode) 
		: Node(_node), EndNode(_endNode) { }

	/*	
	Copy Constructor.

	@param _other - the other iterator
	*/
	ZListIterator(const ZListIterator& _other) 
		: Node(_other.Node), EndNode(_other.EndNode) { }
	
	/*
	public ZListIterator<T, A>::CheckNodeCurrent
	
	Node check function that does not allow the current node to 
	point to NULL or to 'End'.
	
	@return (void)
	@assert - if the node is NULL
	*/
	void CheckNodeCurrent() const
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(Node != NULL && Node != EndNode, "ZList Iterator Invalid!");
		#endif
	}

	/*
	public ZListIterator<T, A>::CheckNodeNext
	
	Node check function that does not allow Node to be 'End'
	
	@return (void)
	*/
	void CheckNodeNext() const
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(Node != NULL && Node != EndNode, "ZList Iterator Next Invalid!");
		#endif
	}

	/*
	public ZListIterator<T, A>::CheckNodePrevious
	
	Node check function that does not allow Node to be 'Begin'
	
	@return (void)
	*/
	void CheckNodePrevious() const
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(Node != NULL && Node->Previous != EndNode, "ZList Iterator Previous Invalid!");
		#endif
	}

	/*
	public ZListIterator<T, A>::GetNode
	
	Gets the node that this iterator is currently pointed at.
	
	@return (ZListNode<T>*) - the node we are currently pointed at
	*/
	ZListNode<T>* GetNode() const								
		{ return Node; }

	/*
	public ZListIterator<T, A>::SetNode
	
	Sets the current node for this iterator.  Useful for when the 
	underlying list changes state.
	
	@param _node - the node to point this iterator at
	@return (void)
	*/
	void SetNode(ZListNode<T>* _node)							
		{ Node = _node; }

	/*
	public ZListIterator<T>::Get
	
	Gets the element this iterator points to.
	
	@return (T&) - the element pointed to
	*/
	T& Get() const										
		{ return *(*this); }

	/*
	public ZListIterator<T>::HasCurrent
	
	Determines if this iterator currently points to a valid element.
	
	@return (bool) - true if element at current position
	*/
	bool HasCurrent() const								
		{ return Node != NULL && Node != EndNode; }

	/*
	public ZListIterator<T>::HasNext
	
	Determines if this iterator has a valid element after the current element.
	
	@return (bool) - true if element after current, false otherwise
	*/
	bool HasNext() const								
		{ return Node != NULL && Node != EndNode && Node->Next != EndNode; }

	/*
	public ZListIterator<T>::HasPrev
	
	Determines if this iterator has a valid element before the current element.
	
	@return (bool) - true if element before current, false otherwise
	*/
	bool HasPrev() const								
		{ return Node != NULL && Node->Previous != EndNode; }

	/*
	public ZListIterator<T>::Next
	
	Advances this iterator to the next element.
	
	@return (void)
	@assert - if this would advance past end
	*/
	void Next()											
		{ ++(*this); }

	/*
	public ZListIterator<T>::Prev
	
	Returns this iterator to the previous element.
	
	@return (void)
	@assert - if this would advance past begin
	*/
	void Prev()									
		{ --(*this); }

	//Operator overrides
	ZListIterator& operator ++()							{ CheckNodeNext(); Node = Node->Next; return *this; }
	ZListIterator operator ++ (int)							{ ZListNode<T> *node = Node; CheckNodeNext(); Node = Node->Next; return ZListIterator(node, EndNode); }
	ZListIterator operator + (int _value)					{ ZListIterator itr(*this); for (int i = 0; i < _value; i++) ++itr; return itr; }
	ZListIterator& operator += (int _value)					{ for (int i = 0; i < _value; i++) ++(*this); return *this; }
	
	ZListIterator& operator -- ()							{ CheckNodePrevious(); Node = Node->Previous; return *this; }
	ZListIterator operator -- (int)							{ ZListNode<T> *node = Node; CheckNodePrevious(); Node = Node->Previous; return ZListIterator(node, EndNode); }
	ZListIterator operator - (int _value)					{ ZListIterator itr(*this); for (int i = _value; i > 0; i--) --itr; return itr; }
	ZListIterator& operator -=(int _value)					{ for (int i = 0; i < _value; i--) --(*this); return *this; }

	ZListIterator& operator = (const ZListIterator &_other)	{ Node = _other.Node; EndNode = _other.EndNode; return *this; }
	bool operator == (const ZListIterator& _other) const	{ return (Node == _other.Node) && (EndNode == _other.EndNode); }
	bool operator !=(const ZListIterator& _other) const		{ return !( (*this)==_other ); }

	T& operator *() const									{ CheckNodeCurrent(); return Node->Element; }

private:
	//The current node we are pointing to
	ZListNode<T> *Node;

	//The end node of the list
	ZListNode<T> *EndNode;
};

//////////////////////////
/* ZList Implementation */
//////////////////////////

/*
List implementation for ZEngine.

ZList takes a single template parameter, which is the contained type.  The allocator is
passed in at construction, which allows the allocator to be decoupled from the list container.

The template parameter T is the type to be contained in the list.
*/
template <typename T, typename A = ZListAllocator<T> >
class ZList
{
friend struct ZList_AtImpl<T, A>;
friend struct ZList_BackImpl<T, A>;
friend struct ZList_ClearImpl<T, A>;
friend struct ZList_CopyImpl<T, A>;
friend struct ZList_EmptyImpl<T, A>;
friend struct ZList_EqualsImpl<T, A>;
friend struct ZList_EraseImpl<T, A>;
friend struct ZList_FindImpl<T, A>;
friend struct ZList_FrontImpl<T, A>;
friend struct ZList_InsertImpl<T, A>;
friend struct ZList_PopBackImpl<T, A>;
friend struct ZList_PopFrontImpl<T, A>;
friend struct ZList_PushBackImpl<T, A>;
friend struct ZList_PushFrontImpl<T, A>;
friend struct ZList_SizeImpl<T, A>;
friend struct ZList_SpliceImpl<T, A>;
friend struct ZList_SwapImpl<T, A>;
friend struct ZList_SwapNodesImpl<T, A>;

protected:
	/*
	Our empty list node.
	
	EmptyNode.Next is always the first node in the list (itself if empty).
	EmptyNode.Previous is always the last node in the list (itself if empty).
	*/
	ZListNode<T> EmptyNode;

	//Allocator for the list
	A ListAllocator;

	//Allocate function
	inline ZListNode<T>* AllocateNode()
	{
		ZListNode<T>* node = ListAllocator.Allocate();

		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(node != NULL, "ZList allocator failed to allocate node!");
		#endif

		return node;
	}

	//Deallocate function
	inline void DeallocateNode(ZListNode<T> *_node)
	{
		ListAllocator.Deallocate(_node);
	}

	//Integrity Check (used for internal debugging)
	inline void CheckIntegrity() const
	{
		#if ZSTL_CHECK_INTEGRITY

		ZListNode<T>* current;
		ZListNode<T>* previous;

		current = EmptyNode.Next;
		previous = current->Previous;

		ZSTL_ASSERT(previous == &EmptyNode, "First linkage invalid!");

		for(;;) //Stupid MSVC warning
		{
			ZSTL_ASSERT(current != NULL, "ZList Error: Contains invalid linkage pointers!");

			previous = current;
			current = current->Next;

			if (current == &EmptyNode)
				break;
		}

		ZSTL_ASSERT(previous == EmptyNode.Previous, "ZList Error: Last linkage invalid!");

		#endif //ZSTL_CHECK_INTEGRITY
	}

public:
	/*	
	Typedef for ZListIterator (allows for ZList<T, A>::Iterator notation).
	*/
	typedef ZListIterator<T> Iterator;

	/*
	Default Constructor.
	*/
	ZList()
	{
		EmptyNode.Next = &EmptyNode;
		EmptyNode.Previous = &EmptyNode; 

		CheckIntegrity();
	}

	/*
	Sub-List Constructor.  Constructs a list containing the elements between two given iterators.

	@param _begin - the iterator at which to start the list
	@param _end - the iterator at which to end the list (exclusive)
	*/
	ZList(const ZListIterator<T>& _begin, const ZListIterator<T>& _end)
	{
		typename ZList<T, A>::Iterator itr;

		EmptyNode.Next = &EmptyNode;
		EmptyNode.Previous = &EmptyNode; 

		for (itr = _begin; itr != _end; ++itr)
			PushBack((*itr));

		CheckIntegrity();
	}

	/*
	Copy Constructor.  Makes a copy of the other list.  Needed to prevent
	the default copy constructor from being generated.

	@param _other - the other list to copy.
	*/
	ZList(const ZList<T, A>& _other)
	{
		typename ZList<T, A>::Iterator itr;

		EmptyNode.Next = &EmptyNode;
		EmptyNode.Previous = &EmptyNode; 

		_other.CheckIntegrity();

		for (itr = _other.Begin(); itr != _other.End(); ++itr)
			PushBack((*itr));

		CheckIntegrity();
	}

	/*
	Copy constructor.  Makes a copy of the other list.

	@param B - the allocator type of the other list
	@param _other - the other list to copy
	*/
	template <typename B>
	ZList(const ZList<T, B>& _other)
	{
		typename ZList<T, A>::Iterator itr;

		EmptyNode.Next = &EmptyNode;
		EmptyNode.Previous = &EmptyNode; 

		for (itr = _other.Begin(); itr != _other.End(); ++itr)
			PushBack((*itr));

		CheckIntegrity();
	}

	/*
	Destructor.
	*/
	~ZList()
	{
		Clear();
	}

	/*
	[] Operator overload.  Equivalent to a call to At.

	@param _index - index into the list
	@return (ZList<T, A>::Iterator) - iterator to the given index
	*/
	Iterator operator [] (size_t _index) const 
		{ return At(_index); }

	/*
	= Operator overload.  Equivalent to a call to Copy.  Needed to prevent 
	a default assignment operator from being created.

	@param _other - the other list to copy.
	@return (void)
	*/
	ZList<T, A>& operator = (const ZList<T, A>& _other) 
		{ Copy(_other); return *this; }

	/*
	= Operator overload.  Equivalent to a call to Copy.

	@param _other - the other list to copy.
	@return (void)
	*/
	template <typename B>
	ZList<T, A>& operator = (const ZList<T, B>& _other) 
		{ Copy(_other); return *this; }

	/*
	== Operator overload.  Equivalent to a call to Equals.

	@param B - the allocator type of the other list
	@param _other - the list to compare against
	@return (bool) - true if they are equal, false otherwise
	*/
	template <typename B>
	bool operator == (const ZList<T, B>& _other) const 
		{ return Equals(_other); }

	/*
	!= Operator overload.  Equivalent to !Equals.

	@param B - the allocator type of the other list
	@param _other - the list to compare against
	@return (bool) - true if they are not equal, false if they are
	*/
	template <typename B>
	bool operator != (const ZList<T, B>& _other) const 
		{ return !Equals(_other); }

	/*
	public ZList<T, N>::Allocator

	Returns a reference to the current allocator.

	@return (A&) - reference to the held allocator
	*/
	A& Allocator()
		{ return ListAllocator; }

	/*
	public ZList<T, A>::At
	
	Gets an iterator to a specific index in the list.

	@param _index - the index 
	@return (Iterator&) - iterator to the indexed element
	@assert - if the index is past the end of the list
	*/
	Iterator At(size_t _index) const 
		{ return ZList_AtImpl<T, A>::Call(*this, _index); }

	/*
	public ZList<T, A>::Back

	Gets a reference to the value at the back of the list.

	@return (T&) - last element in the list
	@assert - if the list is empty
	*/
	T& Back() const 
		{ return ZList_BackImpl<T, A>::Call(*this); }

	/*
	public ZList<T, A>::Begin

	Gets an iterator to the beginning of the list.

	@return (ZList<T, A>::Iterator) - iterator pointing to the first list node
	*/
	Iterator Begin() const 
		{ return ZListIterator<T>(const_cast<ZListNode<T>*>(EmptyNode.Next), const_cast<ZListNode<T>*>(&EmptyNode)); }

	/*
	public ZList<T, A>::Clear

	Clears the list.

	@return (void)
	*/
	void Clear() 
		{ typename ZList<T, A>::Iterator temp = Begin(); return Clear(temp); }

	/*
	public ZList<T, A>::Clear

	Clears the list after a specific iterator location.  The given iterator is set
	to the end of the list after this call.

	@param - _itr - the iterator
	@return (void)
	*/
	void Clear(Iterator& _itr)
		{ return ZList_ClearImpl<T, A>::Call(*this, _itr); }

	/*
	public ZList<T, A>::Copy<B>
	
	Copies the contents of another list into this list.
	
	@param _other - the list to copy from
	@return (void)
	*/
	template <typename B>
	void Copy(const ZList<T, B>& _other) 
		{ return ZList_CopyImpl<T, A>::template Call<B>(*this, _other); }

	/*
	public ZList<T, A>::Empty

	O(1) operation that determines if the list is empty.

	@return (bool) - true if the list has no elements, false otherwise
	*/
	bool Empty() const 
		{ return ZList_EmptyImpl<T, A>::Call(*this); }

	/*
	public ZList<T, A>::End

	Gets an iterator to the 'end' node, which is one element past
	the last element in the list.

	@return (ZList<T, A>::Iterator) - iterator pointing to the 'end' node
	*/
	Iterator End() const 
		{ return ZListIterator<T>(const_cast<ZListNode<T>*>(&EmptyNode), const_cast<ZListNode<T>*>(&EmptyNode)); }

	/*
	public ZList<T, A>::Equals
	
	Does an element-by-element comparison to determine equivalence.
	
	@param B - the type of the allocator used by the other list
	@param _other - the list to compare against
	@return (bool) - true if the lists are equivalent, false otherwise
	*/
	template <typename B>
	bool Equals(const ZList<T, B>& _other) const
		{ return ZList_EqualsImpl<T, A>::Call(*this, _other); }

	/*
	public ZList<T, A>::Erase

	Removes a value from the list at the specified location.

	@param _itr - the iterator location to remove the value from (incremented after this call)
	@return (T) - the removed value
	@assert - if the iterator is invalid
	*/
	T Erase(Iterator& _itr) 
		{ return ZList_EraseImpl<T, A>::Call(*this, _itr); }

	/*
	public ZList<T, A>::Erase

	Removes a range of values from the list.

	@param _from - the iterator location to start at (set to _to after this call)
	@param _to - the iterator location to end at (exclusive)
	@return (void)
	@assert - if the iterator is invalid
	*/
	void Erase(Iterator& _from, const Iterator& _to)
		{ return ZList_EraseImpl<T, A>::Call(*this, _from, _to); }
	
	/*
	public ZList<T, A>::Find

	Finds the first occurrence of the element in the list, as determined by
	the == operator.

	@param _elem - the element to find
	@return (Iterator) - iterator to first occurrence of the element, or ZSTL::invalidPos if not found
	*/
	Iterator Find(const T& _elem) const 
		{ return ZList_FindImpl<T, A>::Call(*this, _elem); }

	/*
	public ZList<T, A>::Front

	Gets a reference to the value at the front of the list.

	@return (T&) - first value in the list
	@assert - if the list is empty
	*/
	T& Front() const 
		{ return ZList_FrontImpl<T, A>::Call(*this); }	

	/*
	public ZList<T, A>::Insert

	Inserts a value at the specified location in the list.  As _itr is
	not modified, it will point to the first node after the inserted value.

	@param _itr - iterator to the location to insert the value
	@param _value - the value to add to the beginning of the list
	@return (void)
	@assert - if the iterator is invalid
	*/
	void Insert(const ZListIterator<T>& _itr, const T& _value) 
		{ return ZList_InsertImpl<T, A>::Call(*this, _itr, _value); }

	/*
	public ZList<T, A>::Insert

	Inserts the values between the provided iterators at the specified
	location.  As _itr is not modified, it will point to the first node
	after the inserted values.

	@param _itr - iterator to the location to insert the values
	@param _start - the starting iterator to the values to insert
	@param _end - the ending iterator (exclusive)
	@return (void)
	*/
	void Insert(const ZListIterator<T>& _itr, const ZListIterator<T>& _start, const ZListIterator<T>& _end) 
		{ return ZList_InsertImpl<T, A>::Call(*this, _itr, _start, _end); }

	/*
	public ZList<T, A>::PopBack

	Pops a value from the end of the list.

	@return (T) - the value at the back of the list
	@assert - if the list is empty
	*/
	T PopBack() 
		{ return ZList_PopBackImpl<T, A>::Call(*this); }

	/*
	public ZList<T, A>::PopFront

	Removes and returns the value from the beginning of the list.

	@return (T) - the value at the front of the list
	@assert - if the list is empty
	*/
	T PopFront() 
		{ return ZList_PopFrontImpl<T, A>::Call(*this); }

	/*
	public ZList<T, A>::PushBack

	Pushes a value onto the back of the list.  Calls out to the allocator
	to allocate new nodes.

	@param _value - the value to place in the list
	@return (void)
	*/
	void PushBack(const T& _value) 
		{ return ZList_PushBackImpl<T, A>::Call(*this, _value); }

	/*
	public ZList<T, A>::PushFront

	Pushes a value onto the front of the list.  Calls out to the allocator
	to allocate new nodes.

	@param _value - the value to place in the list
	@return (void)
	*/
	void PushFront(const T& _value) 
		{ return ZList_PushFrontImpl<T, A>::Call(*this, _value); }

	/*
	public ZList<T, A>::Size

	O(n) operation that gives the size of the list.

	@return (size_t) - list length  (number of contained elements)
	*/
	size_t Size() const 
		{ return ZList_SizeImpl<T, A>::Call(*this); }

	/*
	public ZList<T, A>::Splice
	
	Splices the contents of another list between the given iterators 
	into this list.  This avoids assignment operator calls on the contained 
	elements, but does modify the list pointed to by the given iterators 
	(removes the nodes) and requires that an allocator of type A be able
	to deallocate nodes made by other allocators of type A.
	
	The passed in iterators are not modified, meaning that _start will point 
	to the first inserted node in this list, the _end iterator will point 
	to the same element in the node provider list, and _itr will point to the 
	first element after the inserted nodes.
	
	@param _itr - the position to insert the values at
	@param _other - the list the nodes are removed from (must be passed to ensure allocator equivalence)
	@param _start - the starting iterator
	@param _end - the ending iterator (exclusive)
	@return (void)
	@assert - if allocator type A cannot deallocate nodes made by another instance of A (determined by Allocator ==)
	*/
	void Splice(const ZListIterator<T>& _itr, ZList<T, A>& _other, const ZListIterator<T>& _start, const ZListIterator<T>& _end) 
		{ return ZList_SpliceImpl<T, A>::Call(*this, _itr, _other, _start, _end); }

	/*
	public ZList<T, A>::Swap

	Swaps the contents of this list with the contents of another.
	
	@param _other - the list to swap contents with
	@return (void)
	*/
	void Swap(ZList<T, A>& _other) 
		{ return ZList_SwapImpl<T, A>::Call(*this, _other); }

	/*
	public ZList<B>::SwapNodes
	
	Swaps the actual nodes at the given positions.  Avoids an assignment operator call
	on the elements contained.  The iterators are not modified, meaning they now point
	to each other's locations after the call.
	
	@param _i - iterator to the first element
	@param _j - iterator to the second element
	@return (void)
	@assert - if the iterators are invalid
	*/
	void SwapNodes(const ZListIterator<T>& _i, const ZListIterator<T>& _j)
		{ return ZList_SwapNodesImpl<T, A>::Call(*this, _i, _j); }
};

////////////////////////////////////////////
/* Non-Specialized Method Implementations */
////////////////////////////////////////////

template <typename T, typename A>
ZListIterator<T> ZList_AtImpl<T, A>::Call(const ZList<T, A>& _self, size_t _index)

{
	size_t i;
	ZListIterator<T> itr;

	for (i = 0, itr = _self.Begin(); i < _index; i++)
			++itr;

	return itr;
}

/*************************************************************************/

template <typename T, typename A>
T& ZList_BackImpl<T, A>::Call(const ZList<T, A>& _self)
{
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(!_self.Empty(), "ZList: No back element present!");
	#endif

	return _self.EmptyNode.Previous->Element;
}

/*************************************************************************/

template <typename T, typename A>
void ZList_ClearImpl<T, A>::Call(ZList<T, A>& _self, ZListIterator<T>& _itr)
{
	ZListNode<T>* next;
	ZListNode<T>* current = _itr.GetNode();

	_self.CheckIntegrity();

	//Early out
	if (_self.Empty())
		return;

	//If we are the starting node, be sure to reset the EmptyNode.Next pointer
	if (current == _self.EmptyNode.Next)
		_self.EmptyNode.Next = &_self.EmptyNode;

	//This is always true
	_self.EmptyNode.Previous = current->Previous;
	current->Previous->Next = &_self.EmptyNode;

	//Iterate and deallocate the nodes
	while (current != &_self.EmptyNode)
	{
		next = current->Next;
		_self.DeallocateNode(current);
		current = next;
	}

	//Set the node on the iterator equal to the end node
	_itr.SetNode(&_self.EmptyNode);

	_self.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A> template <typename B>
void ZList_CopyImpl<T, A>::Call(ZList<T, A>& _self, const ZList<T, B>& _other)
{
	typename ZList<T, A>::Iterator itr1;
	typename ZList<T, B>::Iterator itr2;

	if (&_self == &_other)
		return;

	//Assign as much as we can to avoid allocation calls with PushBack
	for (itr1 = _self.Begin(), itr2 = _other.Begin(); itr2 != _other.End(); ++itr2)
	{
		if (itr1 != _self.End())
		{
			(*itr1++) = (*itr2);
		}
		else
		{
			_self.PushBack((*itr2));			
		}
	}

	_self.Clear(itr1);
}

/*************************************************************************/

template <typename T, typename A>
bool ZList_EmptyImpl<T, A>::Call(const ZList<T, A>& _self)
{
	//Just need to check the EmptyNode.Next pointer
	return (_self.EmptyNode.Next == &_self.EmptyNode);
}

/*************************************************************************/

template <typename T, typename A> template <typename B>
bool ZList_EqualsImpl<T, A>::Call(const ZList<T, A>& _self, const ZList<T, B>& _other)
{
	typename ZList<T, A>::Iterator itr1;
	typename ZList<T, B>::Iterator itr2;

	if (&_self == &_other)
		return true;

	//Make sure we are element-wise equivalent up to the end of either list
	for (itr1 = _self.Begin(), itr2 = _other.Begin(); itr1 != _self.End() && itr2 != _other.End(); ++itr1, ++itr2)
	{
		if ((*itr1) != (*itr2))
			return false;
	}

	//Then make sure we have reached the end of both lists
	if (itr1 == _self.End() && itr2 == _other.End())
		return true;

	return false;
}

/*************************************************************************/

template <typename T, typename A>
T ZList_EraseImpl<T, A>::Call(ZList<T, A>& _self, ZListIterator<T>& _itr)
{
	T elem;
	ZListNode<T> *node = _itr.GetNode();

	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(node != NULL, "ZList: Iterator is invalid!");
	ZSTL_ASSERT(node != &_self.EmptyNode, "ZList: Cannot erase end node!");
	#endif

	//Increment the iterator to the next list node to keep the iterator valid
	++_itr;
	
	//Rearrange the pointers
	node->Previous->Next = node->Next;
	node->Next->Previous = node->Previous;

	elem = node->Element;

	_self.DeallocateNode(node);

	_self.CheckIntegrity();

	return elem;
}

/*************************************************************************/

template <typename T, typename A>
void ZList_EraseImpl<T, A>::Call(ZList<T, A>& _self, ZListIterator<T>& _start, const ZListIterator<T>& _end)
{
	ZListNode<T> *nodeStart = _start.GetNode();
	ZListNode<T> *nodeEnd = _end.GetNode();

	ZListNode<T> *curNode;
	ZListNode<T> *prevNode;

	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(nodeStart != NULL && nodeEnd != NULL, "ZList: Cannot erase with invalid iterator!");
	ZSTL_ASSERT(nodeStart != &_self.EmptyNode, "ZList: Cannot erase end node!");
	#endif

	//Rearrange the pointers
	nodeStart->Previous->Next = nodeEnd;
	nodeEnd->Previous = nodeStart->Previous;

	//Erase each element between from and to
	curNode = nodeStart;
	prevNode = NULL;

	while (curNode != nodeEnd)
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(curNode != &_self.EmptyNode, "ZList: Cannot erase end node!");
		#endif

		prevNode = curNode;
		curNode = curNode->Next;

		_self.DeallocateNode(prevNode);
	}

	_start = _end;
}

/*************************************************************************/

template <typename T, typename A>
ZListIterator<T> ZList_FindImpl<T, A>::Call(const ZList<T, A>& _self, const T& _elem)
{
	ZListIterator<T> localItr = _self.Begin();
	ZListIterator<T> end = _self.End();

	//For each element, insert into the list at the given location
	while (localItr != end)
	{
		if ((*localItr) == _elem) {
			return localItr;
		}
		++localItr;
	}

	return ZSTL::InvalidPos;
}

/*************************************************************************/

template <typename T, typename A>
T& ZList_FrontImpl<T, A>::Call(const ZList<T, A>& _self)
{
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(!_self.Empty(), "ZList: No front element in list!");
	#endif

	return _self.EmptyNode.Next->Element;
}

/*************************************************************************/

template <typename T, typename A>
void ZList_InsertImpl<T, A>::Call(ZList<T, A>& _self, const ZListIterator<T>& _itr, const T& _value)
{
	ZListNode<T> *node = _itr.GetNode();
	ZListNode<T> *newNode = _self.AllocateNode();

	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(node != NULL, "ZList: Iterator is invalid!");
	#endif

	//Set our data
	newNode->Element = _value;

	//Swap the pointers
	newNode->Next = node;
	newNode->Previous = node->Previous;

	node->Previous->Next = newNode;
	node->Previous = newNode;

	_self.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A>
void ZList_InsertImpl<T, A>::Call(ZList<T, A>& _self, const ZListIterator<T>& _itr, const ZListIterator<T>& _start, const ZListIterator<T>& _end)
{
	ZListIterator<T> localItr = _start;

	//For each element, insert into the list at the given location
	while (localItr != _end)
	{
		_self.Insert(_itr, *localItr);
		++localItr;
	}
}

/*************************************************************************/

template <typename T, typename A>
T ZList_PopBackImpl<T, A>::Call(ZList<T, A>& _self)
{
	T elem;
	ZListNode<T> *node;

	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(!_self.Empty(), "ZList: Cannot pop from back of empty list!");
	#endif

	//Grab the element at the back of the list
	node = _self.EmptyNode.Previous;
	
	//Remove the node from the list
	node->Previous->Next = &_self.EmptyNode;	
	_self.EmptyNode.Previous = node->Previous;

	//Get the element value
	elem = node->Element;

	//Deallocate and then return
	_self.DeallocateNode(node);

	_self.CheckIntegrity();

	return elem;
}

/*************************************************************************/

template <typename T, typename A>
T ZList_PopFrontImpl<T, A>::Call(ZList<T, A>& _self)
{
	T elem;
	ZListNode<T> *node;

	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(!_self.Empty(), "ZList: Cannot pop from front of empty list!");
	#endif

	//Grab the element at the front of the list
	node = _self.EmptyNode.Next;

	//Remove the node from the list
	node->Next->Previous = node->Previous;
	node->Previous->Next = node->Next;

	//Get the element value
	elem = node->Element;

	//Deallocate and then return
	_self.DeallocateNode(node);

	_self.CheckIntegrity();

	return elem;
}

/*************************************************************************/

template <typename T, typename A>
void ZList_PushBackImpl<T, A>::Call(ZList<T, A>& _self, const T& _value)
{
	//Get us a new node
	ZListNode<T> *node = _self.AllocateNode();

	//Set the value
	node->Element = _value;

	//Set some pointers
	node->Next = &_self.EmptyNode;
	node->Previous = _self.EmptyNode.Previous;

	_self.EmptyNode.Previous->Next = node;
	_self.EmptyNode.Previous = node;

	_self.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A>
void ZList_PushFrontImpl<T, A>::Call(ZList<T, A>& _self, const T& _value)
{
	//Get us a new node
	ZListNode<T> *node = _self.AllocateNode();

	//Set the value
	node->Element = _value;

	//Set some pointers
	node->Next = _self.EmptyNode.Next;
	node->Previous = &_self.EmptyNode;

	node->Next->Previous = node;
	_self.EmptyNode.Next = node;

	_self.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A>
size_t ZList_SizeImpl<T, A>::Call(const ZList<T, A>& _self)
{
	size_t i;
	ZListNode<T> *node;

	//Iterate and count
	i = 0;
	for (node = _self.EmptyNode.Next; node != &_self.EmptyNode; node = node->Next)
		i++;

	return i;
}

/*************************************************************************/

template <typename T, typename A>
void ZList_SpliceImpl<T, A>::Call(ZList<T, A>& _self, const ZListIterator<T>& _itr, ZList<T, A>& _other, const ZListIterator<T>& _start, const ZListIterator<T>& _end)
{
	ZListNode<T> *node = _itr.GetNode();
	ZListNode<T> *startNode = _start.GetNode();
	ZListNode<T> *endNode = _end.GetNode();

	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(node != NULL, "ZList: Iterator is invalid!");
	ZSTL_ASSERT(startNode != NULL, "ZList: Start Iterator is invalid!");
	ZSTL_ASSERT(endNode != NULL, "ZList: End Iterator is invalid!");
	#endif

	//Look for easy ways out of this
	if (startNode == endNode)
		return;

	if (_other.Empty())	//This has the convenient side effect of checking to ensure allocators are of the same type
		return;

	/*

	Nope, gotta do this the hard way.  So, we have the following.

	This List: 
	? <-> A <-> B <-> ?

	Other List:
	? <-> C <-> D <-?-> E <-> F <-> ?

	node is node B.  startNode is node D.  endNode is node F.  It is entirely possible
	that node E is also node D - but the algorithm should not care.
	
	After Splice, we should have the following.

	This List:
	? <-> A <-> D <-?-> E <-> B <-> ?

	Other List:
	? <-> C <-> F <-> ?
	
	*/

	ZListNode<T> *nodeA, *nodeB, *nodeC, *nodeD, *nodeE, *nodeF;

	nodeB = node;
	nodeD = startNode;
	nodeF = endNode;
	nodeA = nodeB->Previous;
	nodeE = nodeF->Previous;
	nodeC = nodeD->Previous;

	nodeA->Next = nodeD;
	nodeD->Previous = nodeA;

	nodeB->Previous = nodeE;
	nodeE->Next = nodeB;

	nodeF->Previous = nodeC;
	nodeC->Next = nodeF;

	_self.CheckIntegrity();

	return;
}

/*************************************************************************/

template <typename T, typename A>
void ZList_SwapImpl<T, A>::Call(ZList<T, A>& _self, ZList<T, A>& _other)
{
	ZListNode<T>* tempNode;

	if (_self.Empty() && _other.Empty())
	{
		//Nothing to do here
	}
	else if (_self.Empty() && !_other.Empty())
	{
		//Simple swap
		_self.EmptyNode.Next = _other.EmptyNode.Next;
		_self.EmptyNode.Previous = _other.EmptyNode.Previous;
		_self.EmptyNode.Next->Previous = &_self.EmptyNode;
		_self.EmptyNode.Previous->Next = &_self.EmptyNode;

		_other.EmptyNode.Next = &_other.EmptyNode;
		_other.EmptyNode.Previous = &_other.EmptyNode;
	}
	else if (!_self.Empty() && _other.Empty())
	{
		//Again, simple swap
		_other.EmptyNode.Next = _self.EmptyNode.Next;
		_other.EmptyNode.Previous = _self.EmptyNode.Previous;
		_other.EmptyNode.Next->Previous = &_other.EmptyNode;
		_other.EmptyNode.Previous->Next = &_other.EmptyNode;

		_self.EmptyNode.Next = &_self.EmptyNode;
		_self.EmptyNode.Previous = &_self.EmptyNode;
	}
	else
	{
		//Swap the empty node pointers around
		tempNode = _self.EmptyNode.Next;

		_self.EmptyNode.Next = _other.EmptyNode.Next;
		_other.EmptyNode.Next = tempNode;

		tempNode = _self.EmptyNode.Previous;

		_self.EmptyNode.Previous = _other.EmptyNode.Previous;
		_other.EmptyNode.Previous = tempNode;

		//Now correct the element pointers
		_self.EmptyNode.Next->Previous = &_self.EmptyNode;
		_self.EmptyNode.Previous->Next = &_self.EmptyNode;

		_other.EmptyNode.Next->Previous = &_other.EmptyNode;
		_other.EmptyNode.Previous->Next = &_other.EmptyNode;
	}

	_self.CheckIntegrity();
	_other.CheckIntegrity();
}

/*************************************************************************/

template <typename T, typename A>
void ZList_SwapNodesImpl<T, A>::Call(ZList<T, A>& _self, const ZListIterator<T>& _i, const ZListIterator<T>& _j)
{
	URFP(_self);

	ZListNode<T> *tempNode;
	ZListNode<T> *iNode = _i.GetNode();
	ZListNode<T> *jNode = _j.GetNode();

	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(iNode != NULL, "ZList: _i Iterator is invalid!");
	ZSTL_ASSERT(jNode != NULL, "ZList: _j Iterator is invalid!");
	#endif

	//Early out
	if (iNode == jNode)
		return;

	//Swap pointers around
	tempNode = iNode->Next;
	
	iNode->Next = jNode->Next;
	jNode->Next = tempNode;

	iNode->Next->Previous = iNode;
	jNode->Next->Previous = jNode;

	tempNode = iNode->Previous;

	iNode->Previous = jNode->Previous;
	jNode->Previous = tempNode;

	iNode->Previous->Next = iNode;
	jNode->Previous->Next = jNode;
}

#endif
