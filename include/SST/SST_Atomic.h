/*
	SST_Atomic.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	762 Studios Atomic Operations Library

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.
	
*/

#pragma once

#ifndef _SST_ATOMIC_H
#define _SST_ATOMIC_H

/* Version number constants for libsst-atomic */
#define SST_ATOMIC_VERSION_MAJOR	0x01
#define SST_ATOMIC_VERSION_MINOR	0x01
#define SST_ATOMIC_VERSION_PATCH	0x0000
#define SST_ATOMIC_VERSION			(SST_ATOMIC_VERSION_MAJOR << 24) | (SST_ATOMIC_VERSION_MINOR << 16) | (SST_ATOMIC_VERSION_PATCH)

#define SST_ATOMIC_VERSION_STRING	"1.1.0"

#ifdef __cplusplus
extern "C" {
#endif

	/* Helper macros for incrementing and decrementing */
	#define SST_Atomic_Inc(x)			SST_Atomic_Add((volatile int*)x, 1) 
	#define SST_Atomic_Dec(x)			SST_Atomic_Add((volatile int*)x, -1) 
	#define SST_Atomic_IncReturn(x)		SST_Atomic_AddReturn((volatile int*)x, 1) 
	#define SST_Atomic_DecReturn(x)		SST_Atomic_AddReturn((volatile int*)x, -1) 

	/* Basic atomic operations */
	void SST_Atomic_Add(volatile int* x, int value);
	void SST_Atomic_AddPtr(volatile void** x, int value);
	void SST_Atomic_And(volatile int* x, int value);
	void SST_Atomic_Or(volatile int* x, int value);
	void SST_Atomic_Xor(volatile int* x, int value);
	void SST_Atomic_Not(volatile int* x);

	/* 
	Basic atomic operations that return the value stored in memory 
	after this operation completes.  These are slower on most architectures, 
	use only as needed. 
	*/
	int	SST_Atomic_AddReturn(volatile int* x, int value);
	void* SST_Atomic_AddPtrReturn(volatile void** x, int value);
	int	SST_Atomic_AndReturn(volatile int* x, int value);
	int	SST_Atomic_OrReturn(volatile int* x, int value);
	int	SST_Atomic_XorReturn(volatile int* x, int value);
	int	SST_Atomic_NotReturn(volatile int* x);

	/*
	SST_Atomic_ExchangeAdd

	Adds a value to the given memory location while returning the old one
	Use this to update a variable when the old (unmodified) value is important.

	@param x - pointer to an integer to add value to
	@param value - the value to add
	@return (int) - the old (unmodified) value
	*/
	int SST_Atomic_ExchangeAdd(volatile int* x, int value);

	/*
	SST_Atomic_ExchangeAddPtr

	As above, but used for pointers.

	@param x - pointer to a pointer to add a value to
	@param value - the value to add to the pointer
	@return (void*) - the old (unmodified) pointer
	*/
	void* SST_Atomic_ExchangeAddPtr(volatile void** x, int value);

	/*
	SST_Atomic_Exchange

	Exchanges the given value with the value pointed.
	
	This sets the value of 'x' to 'value', while returning the original value of 'x' before the assignment.

	NOTE: Use AtomicExchangePtr() to exchange pointers. Don't assume sizeof(int) == sizeof(void*).

	@param x - the address of an integer to exchange values with
	@param value - the value to exchange
	@return (int) - the original value of x prior to assignment
	*/
	int SST_Atomic_Exchange(volatile int* x, int value);

	/*
	SST_Atomic_ExchangePtr

	Exchanges the given value with the value pointed.
	
	This sets the value of @a x to @a value, while returning the original value of @a x before the assignment.

	NOTE: Use this function to exchange pointers. Don't assume sizeof(int) == sizeof(void*),

	@param x - The address of a pointer to exchange values with
	@param value - The value to exchange
	@return (void*) - the original value of @a x
	*/
	void* SST_Atomic_ExchangePtr(volatile void** x, void* value);

	/*
	SST_Atomic_CAS

	Performs an atomic compare and swap using integers.

	The value pointed by dest is compared to 'compare'. If they are equal, 'newValue' is stored in 'dest'
	The original value of 'dest' is always returned.

	NOTE: Use AtomicCASPtr to perform atomic compare-and-swaps on pointers. Don't assume sizeof(int) == sizeof(void*)

	@param dest - The address of a value
	@param compare - The value to be compared with @a dest points to
	@param newValue - The value to be exchanged with @a dest point to if @a newValue and @a dest match
	@return (int) - The original value of @a dest, regardless of the comparison's success or failure
	*/
	int SST_Atomic_CAS(volatile int* dest, int compare, int newValue);

	/*
	SST_Atomic_CASPtr

	Performs an atomic compare-and-swap using pointers
	
	The value pointed by dest is compared to 'compare'. If they are equal, 'newValue' is stored in 'dest'
	The original value of 'dest' is always returned.

	NOTE: Use this function to perform atomic compare-and-swaps on pointers. Don't assume sizeof(int) == sizeof(void*)	

	@param dest - The address of a value
	@param compare - The value to be compared with @a dest points to
	@param newValue - The value to be exchanged with @a dest point to if @a newValue and @a dest match
	@return (void*) - The original value of @a dest, regardless of the comparison's success or failure
	*/
	void* SST_Atomic_CASPtr(volatile void** dest, void* compare, void* newValue);
	
	/*
	SST_Atomic_LoadAcquire

	Loads a value with 'acquire' semantics. In SPARC terms, #LoadStore | #LoadLoad, read as all memory loads
	must be completed before any load/store instructions after this barrier will happen.
		
	Consider this example:

	int check = SST_Atomic_LoadAcquire(&flag);
	if(check == 1)
		printf("%p", g_ptr);
		
	This ensures that the value of 'flag' is loaded before the value of 'g_ptr' is loaded, not before. That way, if 'g_ptr' is initialized
	by another thread first, then 'flag' is set to 1, this thread will either print the correct pointer, or nothing at all.
	
	@param src - The address of the value
	@return (int) - The value pointed to by *src
	*/	
	int SST_Atomic_LoadAcquire(volatile int* src);
	void* SST_Atomic_LoadAcquirePtr(volatile void** src);
	
	/*
	SST_Atomic_StoreRelease

	Stores a value with 'release' semantics. In SPARC terms, #LoadStore | #StoreStore, read as all memory load/stores
	must be completed before any store instructions after this barrier occurs.
	
	Consider this example:
	
	//'value' and 'flag' are global variables each initialized to 0xFF
	
	value = 0;
	SST_Atomic_StoreRelease(&flag, 1);
	
	This ensures that no thread ever sees "value == 0" and "flag == 0xFF" since it forces
	"value = 0" to be globally visible to all other CPUs before "flag = 1" is stored.
	
	
	@param dest - The address to store at
	@param value - The value to store
	@return (void)
	*/	
	void SST_Atomic_StoreRelease(volatile int* dest, int value);
	void SST_Atomic_StoreReleasePtr(volatile void** dest, void* value);
	
#ifdef __cplusplus
}
#endif


#endif

