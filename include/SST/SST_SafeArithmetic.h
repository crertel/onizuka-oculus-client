/*
	SST_SafeArithmetic.h
	Author: Chris Ertel <crertel@762studios.com>
	Created: 1/22/2012

	Purpose: 

	Functions to help perform integer arithmetic while catching overflow conditions.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_SAFEARITHMETIC_H
#define _SST_SAFEARITHMETIC_H

#include <stdlib.h>
#include <pstdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
SST_OS_SafeAddI8
 
Adds two signed 8-bit integers and signals if overflow occurred.
 
@param _a - first integer to add
@param _b - second integer to add
@param _out - storage for sum
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddI8(int8_t _a, int8_t _b, int8_t* _out);

/*
SST_OS_SafeAddI8ToSizeT
 
Adds a signed 8-bit integer to a size_t and signals if overflow occurred.
 
@param _a - first integer to add
@param _size - size_t to add to
@param _out - size_t to store result to
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddI8ToSizeT(int8_t _a, size_t _size, size_t* _out);

/*
SST_OS_SafeAddU8
 
Adds two unsigned 8-bit integers and signals if overflow occurred.

@param _a - first integer to add
@param _b - second integer to add
@param _out - storage for sum
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddU8(uint8_t _a, uint8_t _b, uint8_t* _out);

/*
SST_OS_SafeMultiplyU8
 
Multiplies two 8-bit integers and signals if overflow occurred.
 
@param _a - first integer to multiply
@param _b - second integer to multiply
@param _out - storage for product
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeMultiplyU8(uint8_t _a, uint8_t _b, uint8_t* _out);

/*
SST_OS_SafeAddU8ToSizeT
 
 Adds an unsigned 8-bit integer to a size_t and signals if overflow occurred.

 @param _a - first integer to add
 @param _size - size_t to add to
 @param _out - size_t to store result to
 @return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddU8ToSizeT(uint8_t _a, size_t _size, size_t* _out);

/*
SST_OS_SafeAddI16
 
Adds two signed 16-bit integers and signals if overflow occurred.

@param _a - first integer to add
@param _b - second integer to add
@param _out - storage for sum
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddI16(int16_t _a, int16_t _b, int16_t* _out);

/*
SST_OS_SafeMultiplyU16
 
Multiplies two 16-bit integers and signals if overflow occurred.
 
@param _a - first integer to multiply
@param _b - second integer to multiply
@param _out - storage for product
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeMultiplyU16(uint16_t _a, uint16_t _b, uint16_t* _out);

/*
SST_OS_SafeAddI16ToSizeT
 
 Adds a signed 16-bit integer to a size_t and signals if overflow occurred.

 @param _a - first integer to add
 @param _size - size_t to add to
 @param _out - size_t to store result to
 @return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddI16ToSizeT(int16_t _a, size_t _size, size_t* _out);


/*
SST_OS_SafeAddU16
 
Adds two unsigned 16-bit integers and signals if overflow occurred.

@param _a - first integer to add
@param _b - second integer to add
@param _out - storage for sum
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddU16(uint16_t _a, uint16_t _b, uint16_t* _out);

/*
SST_OS_SafeAddU16ToSizeT
 
Adds an unsigned 16-bit integer to a size_t and signals if overflow occurred.

@param _a - first integer to add
@param _size - size_t to add to
@param _out - size_t to store result to
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddU16ToSizeT(uint16_t _a, size_t _size, size_t* _out);

/*
SST_OS_SafeAddI32
 
Adds two signed 32-bit integers and signals if overflow occurred.

@param _a - first integer to add
@param _b - second integer to add
@param _out - storage for sum
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddI32(int32_t _a, int32_t _b, int32_t* _out);

/*
SST_OS_SafeAddI32ToSizeT
 
Adds a signed 32-bit integer to a size_t and signals if overflow occurred.

@param _a - first integer to add
@param _b - size_t to add to
@param _out - size_t to store result to
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddI32ToSizeT(int32_t _a, size_t _b, size_t* _out);

/*
SST_OS_SafeAddU32
 
Adds two unsigned 32-bit integers and signals if overflow occurred.

@param _a - first integer to add
@param _b - second integer to add
@param _out - storage for sum
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddU32(uint32_t _a, uint32_t _b, uint32_t* _out);

/*
SST_OS_SafeMultiplyU32
 
Multiplies two 32-bit integers and signals if overflow occurred.
 
@param _a - first integer to multiply
@param _b - second integer to multiply
@param _out - storage for product
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeMultiplyU32(uint32_t _a, uint32_t _b, uint32_t* _out);

/*
SST_OS_SafeAddU32ToSizeT
 
Adds an unsigned 32-bit integer to a size_t and signals if overflow occurred.

@param _a - first integer to add
@param _b - size_t to add to
@param _out - size_t to store result to
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddU32ToSizeT(uint32_t _a, size_t _b, size_t* _out);

/*
SST_OS_SafeAddI64
 
Adds two signed 64-bit integers and signals if overflow occurred.

@param _a - first integer to add
@param _b - second integer to add
@param _out - storage for sum
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddI64(int64_t _a, int64_t _b, int64_t* _out);

/*
SST_OS_SafeAddI64ToSizeT
 
Adds a signed 64-bit integer to a size_t and signals if overflow occurred.

@param _a - first integer to add
@param _b - size_t to add to
@param _out - size_t to store result to
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddI64ToSizeT(int64_t _a, size_t _b, size_t* _out);

/*
SST_OS_SafeAddU64
 
Adds two unsigned 64-bit integers and signals if overflow occurred.

@param _a - first integer to add
@param _b - second integer to add
@param _out - storage for sum
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddU64(uint64_t _a, uint64_t _b, uint64_t* _out);

/*
SST_OS_SafeMultiplyU64
 
Multiplies two 64-bit integers and signals if overflow occurred.
 
@param _a - first integer to multiply
@param _b - second integer to multiply
@param _out - storage for product
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeMultiplyU64(uint64_t _a, uint64_t _b, uint64_t* _out);

/*
SST_OS_SafeAddU64ToSizeT
 
Adds an unsigned 64-bit integer to a size_t and signals if overflow occurred.

@param _a - first integer to add
@param _b - size_t to add to
@param _out - size_t to store result to
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddU64ToSizeT(uint64_t _a, size_t _b, size_t* _out);

/*
SST_OS_SafeAddSizeTToSizeT
 
Adds a size_t to a size_t and signals if overflow occurred.

@param _a - first size_t to add
@param _b - size_t to add to
@param _out - size_t to store result to
@return (int) - 1 on success, 0 on overflow
*/
int SST_OS_SafeAddSizeTToSizeT( size_t _a, size_t _b, size_t* _out);

#ifdef __cplusplus
}
#endif

#endif

