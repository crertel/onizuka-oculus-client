/*
	SST_PRNG.h
	Author: James Russell <jcrussell@762studios.com>
	Created: 4/29/2012

	Purpose: 

	Interface for pseudo random number generation for libsst.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_PRNG_H
#define _SST_PRNG_H

#include <stdlib.h>		/* size_t */
#include <pstdint.h>	/* uint32_t */

#ifdef __cplusplus
extern "C" {
#endif

	/*
	Enumeration for our type of random number generator.
	*/
	typedef enum SST_PRNG_TYPE {
		SST_PRNG_MERSENNE,
		SST_PRNG_CMWC,
		SST_PRNG_SMALLPRNG
	} SST_PRNG_TYPE;

	/*
	Typedef used for a handle to a PRNG instance.
	*/
	typedef void* SST_PRNG;

	/*
	SST_Random_CreatePRNG

	Creates a new randomizer instance using the current time as the seed.

	@param type - the type of algorithm to use for the PRNG
	@return (SST_PRNG_Instance) - the created randomizer
	*/
	SST_PRNG SST_Random_CreatePRNG(SST_PRNG_TYPE type);

	/*
	SST_Random_CreatePRNGFromSeed

	Creates a new pseudo random number generator instance with the given seed.

	@param type - the type of algorithm to use for the PRNG
	@param seed - the seed the randomizer should be seeded with
	@return (SST_PRNG_Instance) - the created randomizer
	*/
	SST_PRNG SST_Random_CreatePRNGFromSeed(SST_PRNG_TYPE type, uint32_t seed);

	/*
	SST_Random_ClonePRNG

	Makes a deep copy of the provided randomizer, and returns a handle to that copy.

	@param instance - the instance to clone
	@return (SST_PRNG_Instance) - a cloned instance
	*/
	SST_PRNG SST_Random_ClonePRNG(SST_PRNG instance);

	/*
	SST_Random_DestroyPRNG

	Deletes a PRNG instance.

	@param instance - the instance to delete
	@return (void)
	*/
	void SST_Random_DestroyPRNG(SST_PRNG _instance);

	/*
	SST_Random_GetPRNGFloat

	Gets a random float that lies within a range.

	@param instance - the randomizer instance
	@param min - the minimum value (inclusive)
	@param max - the maximum value (exclusive)
	@return (float) - random float in range
	*/
	float SST_Random_GetPRNGFloat(SST_PRNG instance, float min, float max);

	/*
	SST_Random_GetPRNGFloatArray

	Fills a provided array with random floats that lie within a range.

	@param instance - the randomizer instance
	@param out - array to store floats to
	@param num - number of floats to store in array
	@param min - the minimum value (inclusive)
	@param max - the maximum value (exclusive)
	@return (void)
	*/
	void SST_Random_GetPRNGFloatArray(SST_PRNG instance, float* out, size_t num, float min, float max);

	/*
	SST_Random_GetPRNGInt

	Gets a random integer that lies within a range.

	@param instance - the randomizer instance
	@param min - the minimum value (inclusive)
	@param max - the maximum value (exclusive)
	@return (int) - random integer in range
	*/
	int SST_Random_GetPRNGInt(SST_PRNG instance, int min, int max);

	/*
	SST_Random_GetPRNGIntArray

	Fills a provided array with random ints that lie within a range.

	@param instance - the randomizer instance
	@param out - array to store ints to
	@param num - number of integers to store in array
	@param min - the minimum value (inclusive)
	@param max - the maximum value (exclusive)
	@return (void)
	*/
	void SST_Random_GetPRNGIntArray(SST_PRNG instance, int* out, size_t num, int min, int max);

#ifdef __cplusplus
}
#endif

#endif
 
