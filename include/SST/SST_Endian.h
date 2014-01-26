/*
	SST_Endian.h
	Author: Chris Ertel <crertel@762studios.com>
	Created: 1/16/2012

	Purpose: 

	libsst-os endian and byte reordering routines.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_ENDIAN_H
#define _SST_ENDIAN_H

#include <pstdint.h>

typedef int SST_ByteOrder;
#define SST_LITTLE_ENDIAN 0
#define SST_BIG_ENDIAN 1
#define SST_PDP_ENDIAN 2	/* Not supported */

#ifdef __cplusplus
extern "C" {
#endif

	
	/*
	public SST_OS_GetHostEndianness
	 
	This is a function to get the endianness (byte-ordering) of the host machine.

	@return (SST_ByteOrder) - The endian type of the machine the routine is running on.
	*/
	SST_ByteOrder SST_OS_GetHostEndianness( void );

	/*
	public SST_OS_ByteSwap16
	 
	Swaps the byte order of a 16-bit value.
	 
	@param _toSwap - 16-bit word to swap endianness of.
	@return (uint16_t) - swapped endian 16-bit word.
	*/
	uint16_t SST_OS_ByteSwap16( uint16_t _toSwap);

	/*
	public SST_OS_ByteSwap32
	 
	Swaps the byte order of a 32-bit value.
	 
	@param _toSwap - 32-bit value to swap the byte order of
	@return (uint32_t) - swapped endian 32-bit word.
	*/
	uint32_t SST_OS_ByteSwap32( uint32_t _toSwap);

	/*
	public SST_OS_ByteSwap64
	 
	Swaps the byte order of a 64-bit value.
	 
	@param _toSwap - 64-bit word to swap endianness of.
	@return (uint64_t) - swapped endian 64-bit word.
	*/
	uint64_t SST_OS_ByteSwap64( uint64_t _toSwap);

#ifdef __cplusplus
}
#endif

/*
	The functions act like compile-time evaluations of whether a
	byte-reorder is needed. They are in the form of SST_HostTo{LE,BE}{16,32,64},
	and for additional contextual clarity, SST_{BE,LE}{16,32,64}ToHost().
	Obviously SST_HostToLE32() and SST_LE32ToHost() do the same thing in
	the case of only endian choices: big or little, but theoretically it
	would be required if running on a so-called middle endian machine.
	
	For example, when the symbol SST_BYTEORDER_BIG_ENDIAN is defined, then any
	function that converts to BE becomes a no-op, while any function that
	converts to LE becomes a swap. The reverse condition is true when
	SST_BYTEORDER_LITTLE_ENDIAN is defined. Due to this, you can greatly
	simplify both written and generated code dealing with a known byte order.
	
	(Note that this header will appropriately define SST_BYTEORDER_BIG_ENDIAN
	or SST_BYTEORDER_LITTLE_ENDIAN as necessary.)

	Generally, these functions are to avoid this mess:

	Case #1, lots of if statements scattered in the codebase
		if(cpu_is_bigendian) //global flag computed at runtime
			BitsPerPixel = Swap32(bmp.bpp);
		else
			BitsPerPixel = bmp.bpp;

	Case #2, function pointers and unnecessary overhead
		uint32_t (*swapfn)(uint32_t);

		//one time set up
		if(cpu_is_bigendian)
			swapfn = Swap32;
		else
			swapfn = ReturnSameValueFn; //e.g. uint32_t ReturnSameValueFn(uint32_t x) { return x; }

		//call through function pointer
		BitsPerPixel = swapfn(bmp.bpp);


	The recommended style is the #3

		BitsPerPixel = SST_LE32ToHost(bmp.bpp);

	This helps the code base because:

	1) It is clear that the 'bmp' structure is in LE byte order.
	2) It is clear that the programmer wants to convert that to host order.
	3) The compiler can omit code on LE machines and generate code on BE machines (absolutely optimal performance)
	4) Theoretically could be extended to middle-endian formats


	The behavior can be summarized by this table

	                   Macro is...
                   +---------+---------+
	               | ToBE*() | ToLE*() |
	Host is... +---+---------+---------+
	           |LE | swap    | no-op   |
	           +---+---------+---------+
	           |BE | no-op   | swap    |
	           +---+---------+---------+
	    
*/

/* Remove previous definitions */
#undef SST_BYTEORDER_BIG_ENDIAN
#undef SST_BYTEORDER_LITTLE_ENDIAN
#include <SST/SST_Build.h>
#if (SST_COMPILER == SST_COMPILER_MSVC) || (SST_COMPILER == SST_COMPILER_INTEL) /* Visual C++/Intel C/C++ */

	/* MSVC/Intel always in little-endian mode. Even Windows NT 4 ran PPC/MIPS in LE mode. */
	#define SST_BYTEORDER_LITTLE_ENDIAN

#elif (SST_COMPILER == SST_COMPILER_SUNPRO) /* SunPro C/C++ */

	#if defined(__sparc)
		/* Solaris/SPARC is always big-endian */
		#define SST_BYTEORDER_BIG_ENDIAN
	#elif defined(__x86)
		/* x86 is always little-endian */
		#define SST_BYTEORDER_LITTLE_ENDIAN
	#else
		#error Unknown  CPU architecture for SunPro
	#endif
	
#elif (SST_COMPILER == SST_COMPILER_GCC) || (SST_COMPILER == SST_COMPILER_CLANG) /* GCC/clang */

	#if defined(__APPLE__)
		#if defined(__i386__) || defined(__x86_64__)
			#define SST_BYTEORDER_LITTLE_ENDIAN
		#else
			#error Your CPU is not yet supported!
		#endif
	#elif !defined(__BYTE_ORDER__)
		 /* TODO:  The below code is slightly Linux specific.*/ 
		#include <endian.h>
		#if (__BYTE_ORDER == __LITTLE_ENDIAN)
			#define SST_BYTEORDER_LITTLE_ENDIAN
		#elif (__BYTE_ORDER == __BIG_ENDIAN)
			#define SST_BYTEORDER_BIG_ENDIAN
		#else
			#error Compiler may be tool old, please open this file and add a workaround.
		#endif
	#else
		#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
			#define SST_BYTEORDER_LITTLE_ENDIAN
		#elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
			#define SST_BYTEORDER_BIG_ENDIAN
		#else
			#error Compiler may be too old, please open this file and add a workaround.
		#endif
	#endif
	
#endif

/* Check for multiple definitions */
#if defined(SST_BYTEORDER_BIG_ENDIAN) && defined(SST_BYTEORDER_LITTLE_ENDIAN)
	#error Both SST_BYTEORDER_BIG_ENDIAN and SST_BYTEORDER_LITTLE_ENDIAN are defined!
#endif

/* Check for no definitions */
#if !defined(SST_BYTEORDER_BIG_ENDIAN) && !defined(SST_BYTEORDER_LITTLE_ENDIAN)
	#error Neither of SST_BYTEORDER_BIG_ENDIAN or SST_BYTEORDER_LITTLE_ENDIAN are defined!
#endif

#if defined(SST_BYTEORDER_BIG_ENDIAN)
	
	#define	SST_BYTEORDER_HOST	SST_BIG_ENDIAN

	/* Host <-> BE */
	#define SST_OS_HostToBE16(x) (x)
	#define SST_OS_HostToBE32(x) (x)
	#define SST_OS_HostToBE64(x) (x)
	#define SST_OS_BEToHost16(x) (x)
	#define SST_OS_BEToHost32(x) (x)
	#define SST_OS_BEToHost64(x) (x)

	/* Host <-> LE */
	#define SST_OS_HostToLE16(x) SST_OS_ByteSwap16(x)
	#define SST_OS_HostToLE32(x) SST_OS_ByteSwap32(x)
	#define SST_OS_HostToLE64(x) SST_OS_ByteSwap64(x)
	#define SST_OS_LEToHost16(x) SST_OS_ByteSwap16(x)
	#define SST_OS_LEToHost32(x) SST_OS_ByteSwap32(x)
	#define SST_OS_LEToHost64(x) SST_OS_ByteSwap64(x)
#endif

#if defined(SST_BYTEORDER_LITTLE_ENDIAN)

	#define	SST_BYTEORDER_HOST	SST_LITTLE_ENDIAN

	/* Host <-> BE */
	#define SST_OS_HostToBE16(x) SST_OS_ByteSwap16(x)
	#define SST_OS_HostToBE32(x) SST_OS_ByteSwap32(x)
	#define SST_OS_HostToBE64(x) SST_OS_ByteSwap64(x)
	#define SST_OS_BEToHost16(x) SST_OS_ByteSwap16(x)
	#define SST_OS_BEToHost32(x) SST_OS_ByteSwap32(x)
	#define SST_OS_BEToHost64(x) SST_OS_ByteSwap64(x)

	/* Host <-> LE */
	#define SST_OS_HostToLE16(x) (x)
	#define SST_OS_HostToLE32(x) (x)
	#define SST_OS_HostToLE64(x) (x)
	#define SST_OS_LEToHost16(x) (x)
	#define SST_OS_LEToHost32(x) (x)
	#define SST_OS_LEToHost64(x) (x)

#endif

#endif


