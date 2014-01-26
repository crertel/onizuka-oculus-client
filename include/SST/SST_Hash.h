/*
	SST_Hash.h
	Author: James Russell <jcrussell@762studios.com>
	Created: 11/06/2011

	Purpose: 

	Defines a set of string hash functions.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#ifndef _SST_HASH_H
#define _SST_HASH_H

#include <pstdint.h>
#include <stdlib.h>		/* size_t */

/* hash value data types */
typedef uint32_t SST_HashValue32;
typedef uint64_t SST_HashValue64;

#ifdef __cplusplus
extern "C" {
#endif

	/*
	SST_Crypto_HashDJB2

	djb2 Hash Function, which is a variant of the J6 Hash function (or vice versa, not sure really).
	Found it at http://www.cse.yorku.ca/~oz/hash.html

	@param _string - the string to hash
	@param _len - the length of the string
	@return (SST_HashValue64) - hash value for the string
	*/
	extern SST_HashValue64 SST_Crypto_HashDJB2(const char* _string, size_t _len);

	/*
	SST_Crypto_HashJ6

	Hashes a string as defined by the Java 6 String hash method.

	@param _string - the string to hash
	@param _len - the length of the string
	@return (SST_HashValue32) - hash value for the string
	*/
	extern SST_HashValue32 SST_Crypto_HashJ6(const char* _string, size_t _len);

	/*
	SST_Crypto_HashJCR2

	64-bit Hash function which gives very good distribution for ascii strings of the
	same length (guaranteed no collisions up to length 8).

	@param _string - the string to hash
	@return (SST_HashValue64) - the hash value for the string
	*/
	extern SST_HashValue64 SST_Crypto_HashJCR2(const char* _string, size_t _len);

	/*
	SST_Crypto_HashSDBM

	sdbm hash method, which is used in Berkeley db (among other places).
	Found it at http://www.cse.yorku.ca/~oz/hash.html

	@param _string - the string to hash
	@param _len - the length of the string
	@return (SST_HashValue64) - hash value for the string
	*/
	extern SST_HashValue64 SST_Crypto_HashSDBM(const char* _string, size_t _len);

	/*
	SST_Crypto_HashCRC32

	CRC32 hash of a string.

	See chapter 14 of "Hacker's Delight" by Henr S. Warren, http://en.wikipedia.org/wiki/Cyclic_redundancy_check
	for choice of polynomial, and http://opensource.apple.com/source/xnu/xnu-1456.1.26/bsd/libkern/crc32.c for table.

	@param _string - the string to hash
	@param _len - the length of the string
	@return (SST_HashValue32) - hash value for the string
	*/
	extern SST_HashValue32 SST_Crypto_HashCRC32(const char* _string, size_t _len);

#ifdef __cplusplus
}
#endif

#endif
 
