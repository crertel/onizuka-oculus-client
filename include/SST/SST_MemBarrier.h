/*
	SST_MemBarrier.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-concurrency memory barriers

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_MEMBARRIER_H
#define _SST_MEMBARRIER_H

/* Miscellaneous stuff related to concurrency. The following 4 functions are defined: */

/*
	SST_Concurrency_StoreBarrier()
	
	Prevents the compiler and CPU from reordering stores relative to other stores.
	For example, the sequence:

		*a = 1;
		*b = 2;
		*c = 3;

	...can be done in any order. Either the CPU or the compiler can rearrange these.
	However, the sequence:

		*a = 1;
		SST_Concurrency_StoreBarrier();
		*b = 2;
		*c = 3;

	produces a store sequence that must be either {a, b, c} or {a, c, b}. In other words, 'a' is
	written to memory and visible to other CPUs before 'b' or 'c' will be. On x86, every store
	is ordered relative to every other store.

	@return void
*/

/*
	SST_Concurrency_LoadBarrier()

	Prevents the compiler and CPU from reordering loads relative to other loads.
	For example, the sequence:

		int x = *a;
		int y = *b;
		int z = *c;
	
	...can be done in any order. Either the CPU or the compiler can rearrange these.
	This is useful to prevent loads from occurring until after another load is finished, e.g:

		int x, y; //assume globals

		if(x == 3) //load x, compare to 3
		{
			SST_Concurrency_LoadBarrier();
			int count = y; //load y
		}
		else count = 0;
	
	This prevents 'y' from being loaded speculatively before 'x', and also inhibits compiler
	optimization of 'y' being loaded ahead of time. This ensures 'x' really is 3 before the
	instructions to load 'y' are executed. On x86, every load is ordered relative to every other
	load.

	@return void
*/

/*
	SST_Concurrency_MemoryBarrier()

	Enforces a full memory barrier. The compiler/CPU will not reorder any memory operation
	across this. It also ensures that all previous loads/stores have fully completed before
	continuing. As you can imagine, this inhibits aggressive load/stores by both the compiler
	and CPU, so should be used sparingly.

	@return void
*/

/*
	SST_Concurrency_Spin()
	
	Enters a CPU-optimized spin state. This is useful for reducing physical power consumption while
	spinning and also for giving more execution resources to another hardware thread, e.g: Intel's
	HyperThreading. Otherwise, it acts a no-op.

	@return - void
*/

#if defined(_MSC_VER)
	#if _MSC_VER < 1400 /* MSVC 2003 */
		#ifdef __cplusplus
			extern "C"
		#endif
		 void _ReadWriteBarrier(void);
	#else /* MSVC 2005 or later */
		#include <intrin.h> /* Has _ReadWriteBarrier() */
	#endif
	#pragma intrinsic(_ReadWriteBarrier)

	#if defined(_M_IX86) || defined(_M_X64) || defined(_M_AMD64)
		#include <emmintrin.h>
		#define SST_Concurrency_StoreBarrier()		_ReadWriteBarrier() /* no-op, x86 always orders stores relative to other stores */
		#define SST_Concurrency_LoadBarrier()		_ReadWriteBarrier() /* no-op, x86 always orders stores relative to other stores */
		#define SST_Concurrency_MemoryBarrier()		_mm_mfence()
		#define SST_Concurrency_Spin()				_mm_pause()
	#elif defined(_M_IA64) /* TODO: These fences are overly strong and will likely hurt performance + mask bugs */
		#define SST_Concurrency_StoreBarrier()		__mf()
		#define SST_Concurrency_LoadBarrier()		__mf()
		#define SST_Concurrency_MemoryBarrier()		__mf()
		#define SST_Concurrency_Spin()				__yield()
	#else	
		#error Your CPU architecture isnt yet supported!
	#endif

#elif defined(__GNUC__)

	#if defined(__i386__) || defined(__amd64__) /* x86 CPUs, 32/64 bit */
		#define SST_Concurrency_StoreBarrier()		__asm__ volatile("\n" ::: "memory")
		#define SST_Concurrency_LoadBarrier()		__asm__ volatile("\n" ::: "memory")
		#define SST_Concurrency_MemoryBarrier()		__asm__ volatile("mfence\n" ::: "memory")
		#define SST_Concurrency_Spin()				__asm__("pause")
	#elif defined(__sparc) /* SPARCv9 CPUs, 32/64-bit */
		/*
			SPARC membar #XxxYyy: All Xxx before 'membar' must complete before Yyy can begin.
			In SPARC TSO, only #StoreLoad is needed since only loads can be moved ahead of stores.
		*/
		#define SST_Concurrency_StoreBarrier()		__asm__ volatile("membar #StoreStore\n" ::: "memory")
		#define SST_Concurrency_LoadBarrier()		__asm__ volatile("membar #LoadLoad\n" ::: "memory")
		#define SST_Concurrency_MemoryBarrier()		__asm__ volatile("membar #StoreLoad | #StoreStore | #LoadLoad | #LoadStore\n" ::: "memory")
		#define SST_Concurrency_Spin()
	#elif defined(__ia64__) /* Itanium */
		#define SST_Concurrency_StoreBarrier()		__asm__ volatile("mf\n" ::: "memory")
		#define SST_Concurrency_LoadBarrier()		__asm__ volatile("mf\n" ::: "memory")
		#define SST_Concurrency_MemoryBarrier()		__asm__ volatile("mf\n" ::: "memory")
		#define SST_Concurrency_Spin()				__asm__("hint @pause\n")	
	#elif defined(__arm__)
		/* ARMv7 supports 'dmb' instruction */
		#if defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__)
			#define SST_Concurrency_StoreBarrier()	__asm__ volatile("dmb st\n" ::: "memory")
			#define SST_Concurrency_LoadBarrier()	__asm__ volatile("dmb\n", ::: "memory")
			#define SST_Concurrency_MemoryBarrier() __asm__ volatile("dmb\n" ::: "memory")
			#define SST_Concurrency_Spin()			__asm__("yield\n")
		#else
			#define SST_Concurrency_StoreBarrier()	SST_Concurrency_MemoryBarrier() 
			#define SST_Concurrency_LoadBarrier()	SST_Concurrency_MemoryBarrier()
			#define SST_Concurrency_MemoryBarrier() __asm__ volatile("mcr p15, 0, r0, c7, c10, 5" ::: "memory")

			/* 'yield' instruction not supported until ARMv6K */
			#if defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6ZK__)
				#define SST_Concurrency_Spin()		__asm__("yield\n")
			#else
				#define SST_Concurrency_Spin()		/* no-op */
			#endif
		#endif
	#elif defined(__powerpc__) || defined(__ppc__) || defined(__powerpc64__) || defined(__ppc64__) /* Power Architecture */
		#define SST_Concurrency_StoreBarrier()		__asm__ volatile("lwsync\n" ::: "memory")
		#define SST_Concurrency_LoadBarrier()		__asm__ volatile("lwsync\n" ::: "memory")
		#define SST_Concurrency_MemoryBarrier()		__asm__ volatile("sync\n" ::: "memory")
		#define SST_Concurrency_Spin()				/* no-op */
	#elif defined(__mips__)
		/* TODO MIPS32/64 (most common) vs SGI MIPS I - IV have different sync insn. */
		#define SST_Concurrency_StoreBarrier()			__asm__ volatile("sync\n" ::: "memory")                                                                                                                                       
		#define SST_Concurrency_LoadBarrier()			__asm__ volatile("sync\n" ::: "memory")                                                                                                                                       
		#define SST_Concurrency_MemoryBarrier()			__asm__ volatile("sync\n" ::: "memory")                                                                                                                                         
		#define SST_Concurrency_Spin()                          /* no-op */         
	#else
		#error Your CPU architecture isnt yet supported!
	#endif

#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
	#include <mbarrier.h>
	#include <SST/SST_Build.h>
	#define SST_Concurrency_StoreBarrier()			__machine_w_barrier()
	#define SST_Concurrency_LoadBarrier()			__machine_r_barrier()
	#define SST_Concurrency_MemoryBarrier()			__machine_rw_barrier()

	#if defined(__i386__) || defined(__amd64__) /* x86 CPUs, 32/64 bit */
		#define SST_Concurrency_Spin()				asm("pause\n")
	#elif defined(__sparc)
		#define SST_Concurrency_Spin() do{ }		while(WHILE_NOP)
	#endif
#else
	#error Your compiler isnt yet supported
#endif


#endif
