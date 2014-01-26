/*
	ZHashMap.hpp
	Author: James Russell <jcrussell@762studios.com>
	Created: 12/24/2011

	Purpose: 

	Templated hash map implementation.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _ZHASHMAP_HPP
#define _ZHASHMAP_HPP

#include <ZSTL/ZPair.hpp>
#include <ZSTL/ZArray.hpp>
#include <ZSTL/ZList.hpp>

//Default number of buckets
#ifndef ZHASHMAP_DEFAULT_BUCKETS
#define ZHASHMAP_DEFAULT_BUCKETS (31)
#endif

//Default load factor for the HashMap (expressed as an integer percentage LF / 100)
#ifndef ZHASHMAP_DEFAULT_LOADFACTOR
#define ZHASHMAP_DEFAULT_LOADFACTOR (75)
#endif

//Factor by which HashMap grows when loadfactor is exceeded
#ifndef ZHASHMAP_DEFAULT_GROWFACTOR
#define ZHASHMAP_DEFAULT_GROWFACTOR (2.0)
#endif

//Number of nodes for the lists stored locally (safe since we never splice)
#ifndef ZHASHMAP_DEFAULT_LOCAL_NODES
#define ZHASHMAP_DEFAULT_LOCAL_NODES (10)
#endif

//Forward Declaration of ZHashMap
template < typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA >
class ZHashMap;

//Forward Declaration of ZHashMapIterator
template <typename K, typename V, typename HT>
class ZHashMapIterator;

/* 
Forward Declaration of ZHashMap Method Implementation Structures 

Existence of these structures allows for template specialization of 
individual methods of the ZHashMap class.  In order to specialize
a single method of ZHashMap, simply specialize the corresponding
method implementation structure.
*/

//Forward Declaration of ZHashMap<K, V, HT, H, LF, LA, AA>::Clear struct
template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
struct ZHashMap_ClearImpl {
	inline static void Call(ZHashMap<K, V, HT, H, LF, LA, AA>& _self);
};

//Forward Declaration of ZHashMap<K, V, HT, H, LF, LA, AA>::ContainsKey struct
template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
struct ZHashMap_ContainsKeyImpl {
	inline static bool Call(const ZHashMap<K, V, HT, H, LF, LA, AA>& _self, const K& _key);
};

//Forward Declaration of ZHashMap<K, V, HT, H, LF, LA, AA>::ContainsValue struct
template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
struct ZHashMap_ContainsValueImpl {
	inline static bool Call(const ZHashMap<K, V, HT, H, LF, LA, AA>& _self, const V& _value);
};

//Forward Declaration of ZHashMap<K, V, HT, H, LF, LA, AA>::Empty struct
template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
struct ZHashMap_EmptyImpl {
	inline static bool Call(const ZHashMap<K, V, HT, H, LF, LA, AA>& _self);
};

//Forward Declaration of ZHashMap<K, V, HT, H, LF, LA, AA>::Erase struct
template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
struct ZHashMap_EraseImpl {
	inline static void Call(ZHashMap<K, V, HT, H, LF, LA, AA>& _self, const K& _key);
	inline static void Call(ZHashMap<K, V, HT, H, LF, LA, AA>& _self, ZHashMapIterator<K, V, HT>& _itr);
};

//Forward Declaration of ZHashMap<K, V, HT, H, LF, LA, AA>::Get struct
template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
struct ZHashMap_GetImpl {
	inline static V& Call(const ZHashMap<K, V, HT, H, LF, LA, AA>& _self, const K& _key);
};

//Forward Declaration of ZHashMap<K, V, HT, H, LF, LA, AA>::Put struct
template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
struct ZHashMap_PutImpl {
	inline static void Call(ZHashMap<K, V, HT, H, LF, LA, AA>& _self, const K& _key);
	inline static void Call(ZHashMap<K, V, HT, H, LF, LA, AA>& _self, const K& _key, const V& _value);
};

//Forward Declaration of ZHashMap<K, V, HT, H, LF, LA, AA>::Resize struct
template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
struct ZHashMap_ResizeImpl {
	inline static void Call(ZHashMap<K, V, HT, H, LF, LA, AA>& _self, size_t _buckets);
};

//Forward Declaration of ZHashMap<K, V, HT, H, LF, LA, AA>::Size struct
template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
struct ZHashMap_SizeImpl {
	inline static size_t Call(const ZHashMap<K, V, HT, H, LF, LA, AA>& _self);
};

///////////////////////////////////////
/* Hash Comparator, Hasher, HashNode */
///////////////////////////////////////

/*
Hash comparator class, which compares elements with the == operator.
*/
template <typename T>
struct ZHashComparator
{
	ZHashComparator() { } //Silence Sun C++ warnings about uninitialized structs

	//operator overload which returns true if equivalent (as per ==), false otherwise
	bool operator () (const T& _a, const T& _b) const
	{
		if ((T)_a == (T)_b)
			return true;
		else
			return false;
	}
};

/*
Hasher class, which produces hash values of objects for use in ZHashMap.

The template parameter T is the type we will be hashing.
The template parameter HT is the hash type that is computed.
*/
template <typename T, typename HT>
class ZHasher
{
public:
	//Comparator for use with equals and less than
	ZHashComparator<T> Comp;

	//Default hash function, which attempts to cast the object as a hash value
	HT Hash(const T& _obj) const
	{
		return (HT)_obj;
	}

	//Default comparison function, which compares objects (should return 0 if not equal, !0 otherwise)
	int Equals(const T& _first, const T& _second) const
	{
		return Comp(_first, _second);
	}
};

/*
Hash node structure, which contains a key, a value, and the pre-computed hash for 
the key as well as the hash mod (bucket).  This node is stored within a ZListNode
for hash chaining.

The template parameter K is the key type.
The template parameter V is the value type.
The template parameter HT is the hash type.
*/
template <typename K, typename V, typename HT>
struct ZHashNode
{
	ZHashNode() { }	//Silence Sun C++ warnings about uninitialized structs

	K			Key;		//The contained key
	V			Value;		//The contained value
	HT			Hash;		//The hash of the key
	size_t		HashMod;	//The hash mod number of buckets (gives bucket)
};

///////////////////////
/* ZHashMap Iterator */
///////////////////////

/*
Iterator for ZHashMap.

The template parameter K is the key type.
The template parameter V is the value type.
The template parameter HT is the hash type.
*/
template <typename K, typename V, typename HT>
class ZHashMapIterator
{
private:
	//The current node we are pointing to
	ZListNode< ZHashNode<K, V, HT> > *Node;

	//The end node of the map
	ZListNode< ZHashNode<K, V, HT> > *EndNode;

public:
	/*
	Default Constructor.
	*/
	ZHashMapIterator() 
		: Node(NULL), EndNode(NULL) { }

	/*
	Useful constructor.

	@param _node - the node we are to begin iterating at
	*/
	ZHashMapIterator(ZListNode< ZHashNode<K, V, HT> >* _node, ZListNode< ZHashNode<K, V, HT> >* _endNode) 
		: Node(_node), EndNode(_endNode) { }

	/*	
	Copy Constructor.

	@param _other - the other iterator
	*/
	ZHashMapIterator(const ZHashMapIterator& _other) 
		: Node(_other.Node), EndNode(_other.EndNode) { }
	
	/*
	public ZHashMapIterator<K, V, HT>::CheckNodeCurrent
	
	Node check function that does not allow the current node to point to NULL.
	
	@return (void)
	*/
	void CheckNodeCurrent() const
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(Node != NULL, "ZHashMap Iterator Invalid!");
		#endif
	}

	/*
	public ZHashMapIterator<K, V, HT>::CheckNodeNext
	
	Node check function that does not allow Node to be 'End'
	
	@return (void)
	*/
	void CheckNodeNext() const
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(Node != NULL && Node != EndNode, "ZHashMap Iterator Next Invalid!");
		#endif
	}

	/*
	public ZHashMapIterator<K, V, HT>::CheckNodePrevious
	
	Node check function that does not allow Node to be 'Begin'
	
	@return (void)
	*/
	void CheckNodePrevious() const
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(Node != NULL && Node->Previous != EndNode, "ZHashMap Iterator Previous Invalid!");
		#endif
	}
	
	/*
	public ZHashMapIterator<K, V, HT>::GetNode
	
	Gets the node that this iterator is currently pointed at.
	
	@return (ZListNode<T>*) - the node we are currently pointed at
	*/
	ZListNode< ZHashNode<K, V, HT> >* GetNode() const								
		{ return Node; }

	/*
	public ZHashMapIterator<K, V, HT>::SetNode
	
	Sets the current node for this iterator.  Useful for when the 
	underlying list changes state.
	
	@param _node - the node to point this iterator at
	@return (void)
	*/
	void SetNode(ZListNode< ZHashNode<K, V, HT> >* _node)							
		{ Node = _node; }

	/*
	public ZHashMapIterator<K, V, HT>::GetKey
	
	Gets the key this iterator points to.
	
	@return (const K&) - the key pointed to
	*/
	const K& GetKey() const										
		{ CheckNodeCurrent(); return Node->Element.Key; }

	/*
	public ZHashMapIterator<K, V, HT>::GetValue

	Gets the value this iterator points to.

	@return (V&) - the value pointed to
	*/
	V& GetValue() const
		{ CheckNodeCurrent(); return Node->Element.Value; }

	/*
	public ZHashMapIterator<K, V, HT>::HasCurrent
	
	Determines if this iterator currently points to a valid element.
	
	@return (bool) - true if element at current position
	*/
	bool HasCurrent() const								
		{ return Node != NULL && Node != EndNode; }

	/*
	public ZHashMapIterator<K, V, HT>::HasNext
	
	Determines if this iterator has a valid element after the current element.
	
	@return (bool) - true if element after current, false otherwise
	*/
	bool HasNext() const								
		{ return Node != NULL && Node != EndNode && Node->Next != EndNode; }

	/*
	public ZHashMapIterator<K, V, HT>::HasPrev
	
	Determines if this iterator has a valid element before the current element.
	
	@return (bool) - true if element before current, false otherwise
	*/
	bool HasPrev() const								
		{ return Node != NULL && Node->Previous != EndNode; }

	/*
	public ZHashMapIterator<K, V, HT>::Next
	
	Advances this iterator to the next element.
	
	@return (void)
	@assert - if this would advance past end
	*/
	void Next()											
		{ ++(*this); }

	/*
	public ZHashMapIterator<K, V, HT>::Prev
	
	Returns this iterator to the previous element.
	
	@return (void)
	@assert - if this would advance past begin
	*/
	void Prev()									
		{ --(*this); }

	/*
	public ZHashMapIterator<K, V, HT>::SetValue

	Sets the value mapped to the given key.

	@return (void)
	@assert - if this is the end node
	*/
	void SetValue(const V& _value)
		{  CheckNodeCurrent(); Node->Element.Value = _value; }


	//Operator overrides
	ZHashMapIterator& operator ++ ()								{ CheckNodeNext(); Node = Node->Next; return *this; }
	ZHashMapIterator  operator ++ (int)								{ ZListNode< ZHashNode<K, V, HT> > *node = Node; CheckNodeNext(); Node = Node->Next; return ZHashMapIterator(node, EndNode); }
	ZHashMapIterator  operator +  (int _value)						{ ZHashMapIterator itr(*this); for (int i = 0; i < _value; i++) ++itr; return itr; }
	ZHashMapIterator& operator += (int _value)						{ for (int i = 0; i < _value; i++) ++(*this); return *this; }
	
	ZHashMapIterator& operator -- ()								{ CheckNodePrevious(); Node = Node->Previous; return *this; }
	ZHashMapIterator  operator -- (int)								{ ZListNode< ZHashNode<K, V, HT> > *node = Node; CheckNodePrevious(); Node = Node->Previous; return ZHashMapIterator(node, EndNode); }
	ZHashMapIterator  operator -  (int _value)						{ ZHashMapIterator itr(*this); for (int i = _value; i > 0; i--) --itr; return itr; }
	ZHashMapIterator& operator -= (int _value)						{ for (int i = 0; i < _value; i--) --(*this); return *this; }

	ZHashMapIterator& operator = (const ZHashMapIterator &_other)	{ Node = _other.Node; EndNode = _other.EndNode; return *this; }
	bool operator == (const ZHashMapIterator& _other) const			{ return (Node == _other.Node) && (EndNode == _other.EndNode); }
	bool operator != (const ZHashMapIterator& _other) const			{ return !( (*this)==_other ); }

	const ZHashNode<K, V, HT>& operator * () const					{ CheckNodeCurrent(); return Node->Element; }
};

/////////////////////////////
/* ZHashMap Implementation */
/////////////////////////////

/*
Templated dynamic hash map implementation.  Uses chained hashing and (optional) load factor
balancing to ensure it behaves gracefully even under heavy load.  This implementation does
not maintain order between keys.

The template parameter K is the key type, which must be hashable.

The template parameter V is the value type, which need not be hashable.

The template parameter HT is the hash type.  This should generally correspond to something
like a 32-bit or 64-bit unsigned integer.  It must be compatible with the modulo operator.

The template parameter H is the hasher type, which defaults to the ZHasher for K using hash type
HT.  The hasher must expose the same methods as ZHasher (Hash, Equals).

The template parameter LF is the load factor of the hash map given as an integral value between
0 (hashmap will not resize) and 100.  Load factor is then computed as (double)(LF / 100).  Whenever 
the current load factor exceeds this value, the number of buckets is increased (map is resized).

The template parameter LA is the allocator type to use for list nodes.  It must allocate ZListNode
instances containing type ZHashNode<K, V, HT>.

The template parameter AA is the allocator type to pass along to the contained ZArray.  It must allocate
arrays of type ZListNode< ZHashNode< K, V > >*.
*/
template <typename K, typename V, 
		  typename HT = ZHashValue32, typename H = ZHasher<K, HT>, size_t LF = ZHASHMAP_DEFAULT_LOADFACTOR,
		  typename LA = ZListPooledAllocator< ZHashNode<K, V, HT>, ZHASHMAP_DEFAULT_LOCAL_NODES >,
		  typename AA = ZArrayAllocator< ZListNode< ZHashNode<K, V, HT> >*, ZHASHMAP_DEFAULT_BUCKETS> >
class ZHashMap
{
friend struct ZHashMap_ClearImpl<K, V, HT, H, LF, LA, AA>;
friend struct ZHashMap_ContainsKeyImpl<K, V, HT, H, LF, LA, AA>;
friend struct ZHashMap_ContainsValueImpl<K, V, HT, H, LF, LA, AA>;
friend struct ZHashMap_EmptyImpl<K, V, HT, H, LF, LA, AA>;
friend struct ZHashMap_EraseImpl<K, V, HT, H, LF, LA, AA>;
friend struct ZHashMap_GetImpl<K, V, HT, H, LF, LA, AA>;
friend struct ZHashMap_PutImpl<K, V, HT, H, LF, LA, AA>;
friend struct ZHashMap_ResizeImpl<K, V, HT, H, LF, LA, AA>;
friend struct ZHashMap_SizeImpl<K, V, HT, H, LF, LA, AA>;

private:
	H		Hasher;			//The hasher for our keys
	LA		NodeAllocator;	//Allocator for chained hashing
	size_t	ElementCount;	//The element count

	ZArray< ZListNode< ZHashNode<K, V, HT> >*, AA>	Map;		//The map
	ZListNode< ZHashNode<K, V, HT> >				EmptyNode;	//The end node for the hash map

	//Resizes and copies the HashMap if we exceed LoadFactor (LF of 0 indicates we should not resize)
	inline void CheckLoadFactor()
	{
		//This will evaluate to false if LF == 0, true if LF > 0 (done to avoid C4127)
		if ((void)LF,LF)
		{
			double currentLoadFactor = LoadFactor();

			if (currentLoadFactor > ((double)LF / (double)100.0))
				Resize((size_t)((double)Map.Size() * ZHASHMAP_DEFAULT_GROWFACTOR));
		}
	}

	//Gets a bucket given a hash code
	inline size_t GetBucket(HT _hash) const
	{
		return _hash % Map.Size();
	}

public:
	/*
	Iterator type for ZHashMap.  Allows ZHashMap::Iterator notation.
	*/
	typedef ZHashMapIterator< K, V, HT > Iterator;

	/*
	Default Constructor.
	*/
	ZHashMap()
		: ElementCount(0), Map(ZHASHMAP_DEFAULT_BUCKETS)
	{
		EmptyNode.Next = &EmptyNode;
		EmptyNode.Previous = &EmptyNode;

		Map.Resize(ZHASHMAP_DEFAULT_BUCKETS, NULL);
	}

	/*
	Parameterized Constructor.

	@param _buckets - the number of buckets the hashmap should use
	*/
	ZHashMap(size_t _buckets)
		: ElementCount(0), Map(_buckets)
	{
		#if !ZSTL_DISABLE_RUNTIME_CHECKS
		ZSTL_ASSERT(_buckets > 0, "Cannot make ZHashMap with no buckets!");
		#endif
		
		EmptyNode.Next = &EmptyNode;
		EmptyNode.Previous = &EmptyNode;

		Map.Resize(_buckets, NULL);
	}

	/*
	Copy constructor.

	@param _other - the other hash map
	*/
	ZHashMap(const ZHashMap<K, V, HT, H, LF, LA, AA>& _other)
		: ElementCount(_other.Size()), Map(_other.Size())
	{
		EmptyNode.Next = &EmptyNode;
		EmptyNode.Previous = &EmptyNode;

		Map.Resize(_other.Size(), NULL);

		for (typename ZHashMap<K, V, HT, H, LF, LA, AA>::Iterator itr = _other.Begin(); itr != _other.End(); ++itr)
			Put((*itr).Key, (*itr).Value);
	}

	/*
	Copy constructor that can copy hash maps with other template types.  Key
	and Value type must be the same.

	@param _other - the other hash map
	*/
	template <typename OHT, typename OH, size_t OLF, typename OLA, typename OAA>
	ZHashMap(const ZHashMap<K, V, OHT, OH, OLF, OLA, OAA>& _other)
		: ElementCount(_other.Size()), Map(_other.Size())
	{
		EmptyNode.Next = &EmptyNode;
		EmptyNode.Previous = &EmptyNode;

		Map.Resize(_other.Map.Size(), NULL);
		
		for (typename ZHashMap<K, V, HT, H, LF, LA, AA>::Iterator itr = _other.Begin(); itr != _other.End(); ++itr)
			Put((*itr).Key, (*itr).Value);
	}

	/*
	= operator overload.  Makes a copy of another hash map.  This version is needed to
	override the default version.

	@param _other - the other hash map
	@return (ZHashMap<K, V, HT, H, LF, LA, AA>) - this hash map
	*/
	ZHashMap<K, V, HT, H, LF, LA, AA>& operator = (const ZHashMap<K, V, HT, H, LF, LA, AA>& _other) 
	{
		Clear();

		ElementCount = _other.ElementCount;
		Map.Resize(_other.Map.Size());
		
		EmptyNode.Next = &EmptyNode;
		EmptyNode.Previous = &EmptyNode;
				
		for (typename ZHashMap<K, V, HT, H, LF, LA, AA>::Iterator itr = _other.Begin(); itr != _other.End(); ++itr) {
			Put((*itr).Key, (*itr).Value);
		}

		return *this;
	}
	
	/*
	= operator overload.

	@param _other - the other hash map
	@return (ZHashMap<K, V, HT, H, LF, LA, AA>) - this hash map
	*/
	template <typename OHT, typename OH, size_t OLF, typename OLA, typename OAA>
	ZHashMap<K, V, HT, H, LF, LA, AA>& operator = (const ZHashMap<K, V, OHT, OH, OLF, OLA, OAA>& _other) 
	{
		Clear();

		ElementCount = _other.ElementCount;
		Map.Resize(_other.Map.Size()());

		EmptyNode.Next = &EmptyNode;
		EmptyNode.Previous = &EmptyNode;
				
		for (typename ZHashMap<K, V, HT, H, LF, LA, AA>::Iterator itr = _other.Begin(); itr != _other.End(); ++itr) {
			Put((*itr).Key, (*itr).Value);
		}

		return *this;
	}

	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::operator[]
	
	Gets a value from the map.  Functionally the same as Get.
	
	@param _key - the key to lookup
	@return (V&) - the value mapped to _key
	@assert - if the key is not mapped
	*/
	V& operator [] (const K& _key) 
		{ return Get(_key); }

	/*
	public ZHashMap<K, V, H, LF, lA, AA>::Begin

	Gets an iterator to the first element in the hash map.  Keep in mind
	that hash map elements are unordered.

	@return (ZHashMap<K, V, HT, H, LF, LA, AA>::Iterator) - iterator to the first element
	*/
	Iterator Begin() const
		{ return ZHashMapIterator< K, V, HT >(const_cast<ZListNode< ZHashNode<K, V, HT> >* >(&EmptyNode)->Next, 
											  const_cast<ZListNode< ZHashNode<K, V, HT> >* >(&EmptyNode)); }

	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::Clear

	Clears the hash map of all keys and values.
	
	@return (void)
	*/
	void Clear() 
		{ ZHashMap_ClearImpl<K, V, HT, H, LF, LA, AA>::Call(*this);  }

	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::ContainsKey

	Determines if the hash map contains the given key.
	
	@param _key - the key to check
	@return (bool) - boolean indicating if the hash map contains the key
	*/
	bool ContainsKey(const K& _key) const 
		{ return ZHashMap_ContainsKeyImpl<K, V, HT, H, LF, LA, AA>::Call(*this, _key); }

	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::ContainsValue

	Determines if the hash map contains the given value.

	@param _value - the value to check for
	@return (bool) - boolean indicating if the hash map contains the value
	*/
	bool ContainsValue(const V& _value) const 
		{ return ZHashMap_ContainsValueImpl<K, V, HT, H, LF, LA, AA>::Call(*this, _value); }

	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::Empty

	Indicates whether or not the hash map is empty.

	@return (bool) - boolean indicating the hash map is empty (free of keys and values)
	*/
	bool Empty() const 
		{ return ZHashMap_EmptyImpl<K, V, HT, H, LF, LA, AA>::Call(*this); }
	
	/*
	public ZHashMap<K, V, H, LF, lA, AA>::End

	Gets an iterator to the end node in the hash map.  Keep in mind
	that hash map elements are unordered.

	@return (ZHashMap<K, V, HT, H, LF, LA, AA>::Iterator) - iterator to the end node
	*/
	Iterator End() const
		{ return ZHashMapIterator< K, V, HT >(const_cast<ZListNode< ZHashNode<K, V, HT> >* >(&EmptyNode), 
		  								 	  const_cast<ZListNode< ZHashNode<K, V, HT> >* >(&EmptyNode)); }

	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::Erase

	Removes the associated key and mapped value.

	@param _key - the key to lookup
	@return (void) - this hash map
	@assert - if the key is not mapped
	*/
	void Erase(const K& _key) 
		{ ZHashMap_EraseImpl<K, V, HT, H, LF, LA, AA>::Call(*this, _key); }

	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::Erase

	Removes the key and mapped value given by the iterator.  This version is more
	efficient than the key-only version because no hash lookup is performed.

	@param _itr - iterator to the hash node to remove
	@return (void)
	@assert - if it is the end iterator
	*/
	void Erase(Iterator& _itr)
		{ ZHashMap_EraseImpl<K, V, HT, H, LF, LA, AA>::Call(*this, _itr); }

	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::Find

    Returns an iterator to the element mapped to the given key.  Will return
	the End() iterator if not mapped.

	@param _key - the key to look up
	@return (ZHashMap<K, V, HT, H, LF, LA, AA>::Iterator) - iterator to the element, 
															End() if not found
	*/
	Iterator Find(const K& _key) const
	{
		HT hash = Hasher.Hash(_key);
		size_t bucket = GetBucket(hash);

		ZListNode< ZHashNode<K, V, HT> >* node = Map.Data()[bucket]; 

		while (node != NULL && node != &EmptyNode && node->Element.HashMod == bucket)
		{
			if (node->Element.Hash == hash && Hasher.Equals(_key, node->Element.Key) != 0)
				return Iterator(node, const_cast< ZListNode< ZHashNode<K, V, HT> > *>(&EmptyNode));

			node = node->Next;
		}

		return End();
	}
	
	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::Get
	
	Gets the value mapped to the given key.

	@param _key - the key to look up
	@return (V&) - the value mapped to _key
	@assert - if the key is not present in the map
	*/
	V& Get(const K& _key) const 
		{ return ZHashMap_GetImpl<K, V, HT, H, LF, LA, AA>::Call(*this, _key); }

	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::Keys

	Gets a list of keys in the map.  Uses the method 'PushBack' to add the 
	keys.

	@param C - the container type to push the keys into, which must contain
			   type K and have a 'PushBack' method
	@param _container - the container to put the keys into
	@return - ZList containing all the keys in the map
	*/
	template <typename C>
	void Keys(C& _container) const 
	{
		Iterator itr = Begin();

		while (itr.HasCurrent()) 
		{
			_container.PushBack((*itr).Key);
			itr.Next();
		}
	}

	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::LoadFactor

	Returns the current load factor of the hash map, as determined by number
	of elements over number of buckets.

	@return (double) - the load factor
	*/
	double LoadFactor() const
	{
		return (double)ElementCount / (double)Map.Size();
	}

	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::Mappings

	Gets a list of all the key value mappings in the hash map.  Adds pairs of
	mappings using 'PushBack'.
	
	@param C - the container type to push the keys into, which must contain
			   type ZPair<K, V> and have a 'PushBack' method
	@param _container - the container to put the keys into
	@return (ZList<ZPair<K, V>>)- list of key-value pairs
	*/
	template <typename C>
	void Mappings(C& _container) const 
	{
		Iterator itr = Begin();

		while (itr.HasCurrent()) 
		{
			_container.PushBack(ZPair<K, V>((*itr).Key, (*itr).Value));
			itr.Next();
		}
	}
	
	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::Put

	Puts the given key into the map with a default constructed value of type V.

	@param _key - the key to place in the map
	*/
	void Put(const K& _key)
		{ return ZHashMap_PutImpl<K, V, HT, H, LF, LA, AA>::Call(*this, _key); }

	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::Put

	Puts the given key and value combination into the hash map.

	@param _key - the key to associate with _value
	@param _value - the value to lookup using _key
	*/
	void Put(const K& _key, const V& _value) 
		{ return ZHashMap_PutImpl<K, V, HT, H, LF, LA, AA>::Call(*this, _key, _value); }
	
	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::Resize
	
	Sets the number of buckets the hashmap will use.  Existing elements will be re-hashed into the map.
	
	@param _buckets - number of buckets to use
	@return (ZHashMap<K, V, HT, H, LF, LA, AA>&) - this map
	*/
	void Resize(size_t _buckets) 
		{ ZHashMap_ResizeImpl<K, V, HT, H, LF, LA, AA>::Call(*this, _buckets); }

	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::Size

	Returns the number of mapped key-value pairs in the map.

	@return (size_t) - the number of key value pairs
	*/
	size_t Size() const 
		{ return ZHashMap_SizeImpl<K, V, HT, H, LF, LA, AA>::Call(*this); }
	
	/*
	public ZHashMap<K, V, HT, H, LF, LA, AA>::Values

	Gets a list of values in the map.
	
	@param C - the container type to push the keys into, which must contain
			   type V and have a 'PushBack' method
	@param _container - the container to put the keys into
	@return (ZList<V>)- ZList containing all the values in the map
	*/
	template <typename C>
	void Values(C& _container) const 
	{
		Iterator itr = Begin();

		while (itr.HasCurrent())
		{
			_container.PushBack((*itr).Value);
			itr.Next();
		}
	}
};

////////////////////////////////////////////
/* Non-Specialized Method Implementations */
////////////////////////////////////////////

template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
void ZHashMap_ClearImpl<K, V, HT, H, LF, LA, AA>::Call(ZHashMap<K, V, HT, H, LF, LA, AA>& _self)
{
	ZListNode< ZHashNode<K, V, HT> >* node = _self.EmptyNode.Next;
	
	while (node != &_self.EmptyNode)
	{
		node = node->Next;
		_self.NodeAllocator.Deallocate(node->Previous);
	} 

	for (size_t i = 0; i < _self.Map.Size(); i++)
		_self.Map.Data()[i] = NULL;

	_self.EmptyNode.Next = &_self.EmptyNode;
	_self.EmptyNode.Previous = &_self.EmptyNode;

	_self.ElementCount = 0;
}

/*************************************************************************/

template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
bool ZHashMap_ContainsKeyImpl<K, V, HT, H, LF, LA, AA>::Call(const ZHashMap<K, V, HT, H, LF, LA, AA>& _self, const K& _key)
{
	HT hash = _self.Hasher.Hash(_key);
	size_t bucket = _self.GetBucket(hash);

	ZListNode< ZHashNode<K, V, HT> >* node = _self.Map.Data()[bucket]; 

	while (node != NULL && node != &_self.EmptyNode && node->Element.HashMod == bucket)
	{
		if (node->Element.Hash == hash && _self.Hasher.Equals(_key, node->Element.Key) != 0)
			return true;

		node = node->Next;
	}
	
	return false;
}

/*************************************************************************/

template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
bool ZHashMap_ContainsValueImpl<K, V, HT, H, LF, LA, AA>::Call(const ZHashMap<K, V, HT, H, LF, LA, AA>& _self, const V& _value)
{
	typename ZHashMap<K, V, HT, H, LF, LA, AA>::Iterator itr = _self.Begin();

	while (itr.HasCurrent()) 
	{
		if ((*itr).Value == _value)
			return true;
		
		itr.Next();
	}

	return false;
}

/*************************************************************************/

template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
bool ZHashMap_EmptyImpl<K, V, HT, H, LF, LA, AA>::Call(const ZHashMap<K, V, HT, H, LF, LA, AA>& _self)
{
	return _self.ElementCount == 0;
}

/*************************************************************************/

template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
void ZHashMap_EraseImpl<K, V, HT, H, LF, LA, AA>::Call(ZHashMap<K, V, HT, H, LF, LA, AA>& _self, const K& _key)
{
	HT hash = _self.Hasher.Hash(_key);
	size_t bucket = _self.GetBucket(hash);
	
	ZListNode< ZHashNode<K, V, HT> >* node = _self.Map.Data()[bucket]; 

	while (node != NULL && node != &_self.EmptyNode && node->Element.HashMod == bucket)
	{
		if (node->Element.Hash == hash && _self.Hasher.Equals(_key, node->Element.Key) != 0)
		{
			ZListNode< ZHashNode<K, V, HT> >* prevNode = node->Previous;
			ZListNode< ZHashNode<K, V, HT> >* nextNode = node->Next;

			prevNode->Next = nextNode;
			nextNode->Previous = prevNode;

			//If the bucket maps to this node, it must change (either Next or NULL)
			if (_self.Map.Data()[bucket] == node) 
			{
				if (nextNode->Element.HashMod == bucket)
					_self.Map.Data()[bucket] = nextNode;
				else
					_self.Map.Data()[bucket] = NULL;
			}

			_self.NodeAllocator.Deallocate(node);
			_self.ElementCount--;

			return;
		}

		node = node->Next;
	}
}

/*************************************************************************/

template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
void ZHashMap_EraseImpl<K, V, HT, H, LF, LA, AA>::Call(ZHashMap<K, V, HT, H, LF, LA, AA>& _self, ZHashMapIterator<K, V, HT>& _itr)
{
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(_itr != _self.End(), "ZHashMap: Cannot Erase End iterator!");
	#endif

	ZListNode< ZHashNode<K, V, HT> >* node = _itr.GetNode();
	ZListNode< ZHashNode<K, V, HT> >* prevNode = node->Previous;
	ZListNode< ZHashNode<K, V, HT> >* nextNode = node->Next;

	size_t bucket = node->Element.HashMod;

	prevNode->Next = nextNode;
	nextNode->Previous = prevNode;

	//If the bucket maps to this node, it must change (either Next or NULL)
	if (_self.Map.Data()[node->Element.HashMod] == node) 
	{
		if (nextNode->Element.HashMod == bucket)
			_self.Map.Data()[bucket] = nextNode;
		else
			_self.Map.Data()[bucket] = NULL;
	}

	_self.NodeAllocator.Deallocate(node);
	_self.ElementCount--;
}


/*************************************************************************/

template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
V& ZHashMap_GetImpl<K, V, HT, H, LF, LA, AA>::Call(const ZHashMap<K, V, HT, H, LF, LA, AA>& _self, const K& _key)
{
	HT hash = _self.Hasher.Hash(_key);
	size_t bucket = _self.GetBucket(hash);
	
	ZListNode< ZHashNode<K, V, HT> >* node = _self.Map.Data()[bucket]; 

	while (node != NULL && node != &_self.EmptyNode && node->Element.HashMod == bucket)
	{
		if (node->Element.Hash == hash && _self.Hasher.Equals(_key, node->Element.Key) != 0)
			return node->Element.Value;

		node = node->Next;
	}

	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ERROR("ZHashMap: Get could not find value!");
	#endif

	//If the runtime assert is ignored and we continue, return ref to a static local variable
	//This is effectively 'undefined' behavior
	static V val;

	return val;
}

/*************************************************************************/

template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
void ZHashMap_PutImpl<K, V, HT, H, LF, LA, AA>::Call(ZHashMap<K, V, HT, H, LF, LA, AA>& _self, const K& _key)
{
	_self.CheckLoadFactor();

	HT hash = _self.Hasher.Hash(_key);
	size_t bucket = _self.GetBucket(hash);

	ZListNode< ZHashNode<K, V, HT> >* node = _self.Map.Data()[bucket]; 

	if (node == NULL)
	{
		//Empty bucket, so search for the next node
		for (size_t i = bucket + 1; i < _self.Map.Size(); i++)
		{
			node = _self.Map.Data()[i];

			if (node != NULL)
				break;
		}

		//No node was found, so use the empty node
		if (node == NULL)
			node = &_self.EmptyNode;
	}
	else
	{
		//Non-empty bucket, so see if we are already mapped
		while (node->Element.HashMod == bucket)
		{
			if (node->Element.Hash == hash && _self.Hasher.Equals(_key, node->Element.Key) != 0)
				return;

			node = node->Next;
		}
	}

	//Not mapped, so insert (node is currently one past where we should be)
	ZListNode< ZHashNode<K, V, HT> >* newNode = _self.NodeAllocator.Allocate();

	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(newNode != NULL, "ZHashMap: Unable to allocate hash node!");
	#endif

	//If the bucket has NULL linkage, set it
	if (_self.Map.Data()[bucket] == NULL)
		_self.Map.Data()[bucket] = newNode;

	newNode->Next = node;
	newNode->Previous = node->Previous;
	newNode->Element.Key = _key;
	newNode->Element.Value = V();		//Be sure to default construct the value instance
	newNode->Element.Hash = hash;
	newNode->Element.HashMod = bucket;

	newNode->Previous->Next = newNode;
	newNode->Next->Previous = newNode;	

	_self.ElementCount++;
}

/*************************************************************************/

template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
void ZHashMap_PutImpl<K, V, HT, H, LF, LA, AA>::Call(ZHashMap<K, V, HT, H, LF, LA, AA>& _self, const K& _key, const V& _value)
{
	_self.CheckLoadFactor();
	
	HT hash = _self.Hasher.Hash(_key);
	size_t bucket = _self.GetBucket(hash);
	
	ZListNode< ZHashNode<K, V, HT> >* node = _self.Map.Data()[bucket]; 

	if (node == NULL)
	{
		//Empty bucket, so search for the next node
		for (size_t i = bucket + 1; i < _self.Map.Size(); i++)
		{
			node = _self.Map.Data()[i];

			if (node != NULL)
				break;
		}

		//No node was found, so use the empty node
		if (node == NULL)
			node = &_self.EmptyNode;
	}
	else
	{
		//Non-empty bucket, so see if we are already mapped
		while (node->Element.HashMod == bucket)
		{
			if (node->Element.Hash == hash && _self.Hasher.Equals(_key, node->Element.Key) != 0)
			{
				node->Element.Value = _value;
				return;
			}

			node = node->Next;
		}
	}
	
	//Not mapped, so insert (node is currently one past where we should be)
	ZListNode< ZHashNode<K, V, HT> >* newNode = _self.NodeAllocator.Allocate();
	
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(newNode != NULL, "ZHashMap: Unable to allocate hash node!");
	#endif
	
	//If the bucket has NULL linkage, set it
	if (_self.Map.Data()[bucket] == NULL)
		_self.Map.Data()[bucket] = newNode;
	
	newNode->Next = node;
	newNode->Previous = node->Previous;
	newNode->Element.Key = _key;
	newNode->Element.Value = _value;
	newNode->Element.Hash = hash;
	newNode->Element.HashMod = bucket;

	newNode->Previous->Next = newNode;
	newNode->Next->Previous = newNode;	
	
	_self.ElementCount++;
}

/*************************************************************************/

template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
void ZHashMap_ResizeImpl<K, V, HT, H, LF, LA, AA>::Call(ZHashMap<K, V, HT, H, LF, LA, AA>& _self, size_t _buckets)
{
	#if !ZSTL_DISABLE_RUNTIME_CHECKS
	ZSTL_ASSERT(_buckets > 0, "ZHashMap: Unable to resize to zero buckets!");
	#endif	
	
	//Get a pointer to the first non-empty element (which may itself be the empty element)
	ZListNode< ZHashNode<K, V, HT> >* node = _self.EmptyNode.Next;

	//Clear out the map completely 
	for (size_t i = 0; i < _self.Map.Size(); i++)
		_self.Map.Data()[i] = NULL;

	_self.EmptyNode.Next = &_self.EmptyNode;
	_self.EmptyNode.Previous = &_self.EmptyNode;

	_self.Map.Resize(_buckets, NULL);

	//Loop through our previous nodes and place them back in the map in their new spots
	while (node != &_self.EmptyNode)
	{
		//Compute new bucket
		node->Element.HashMod = node->Element.Hash % _buckets;
		
		//Determine where we should be placing this node
		ZListNode< ZHashNode<K, V, HT> >* curNode = node;
		ZListNode< ZHashNode<K, V, HT> >* nextNode = _self.Map.Data()[node->Element.HashMod];

		//Get the next node to place now, before we scribble the pointers
		node = curNode->Next;

		//If the bucket has NULL linkage, next node
		if (nextNode == NULL)
		{			
			//Empty bucket, so search for the next nextNode
			for (size_t i = curNode->Element.HashMod + 1; i < _self.Map.Size(); i++)
			{
				nextNode = _self.Map.Data()[i];

				if (nextNode != NULL)
					break;
			}

			//No nextNode was found, so use the empty nextNode
			if (nextNode == NULL)
				nextNode = &_self.EmptyNode;
		}
	
		//Reestablish linkage
		curNode->Next = nextNode;
		curNode->Previous = nextNode->Previous;

		curNode->Previous->Next = curNode;
		curNode->Next->Previous = curNode;	
		
		//Since we are pushing front, this always means we set the pointer
		_self.Map.Data()[curNode->Element.HashMod] = curNode;
	}
}

/*************************************************************************/

template <typename K, typename V, typename HT, typename H, size_t LF, typename LA, typename AA>
size_t ZHashMap_SizeImpl<K, V, HT, H, LF, LA, AA>::Call(const ZHashMap<K, V, HT, H, LF, LA, AA>& _self)
{
	return _self.ElementCount;
}

#endif
