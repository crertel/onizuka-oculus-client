/*
	SST_SmallPRNG.h
	Author: Chris Ertel <crertel@762studios.com>
	Created: 11/20/2011

	Purpose: 

	Implementation of a small noncryptographic PRNG. See http://burtleburtle.net/bob/rand/smallprng.html.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#ifndef _SST_SMALLPRNG_H
#define _SST_SMALLPRNG_H

#include <pstdint.h>
#include <stdlib.h> /* size_t */

/* SmallPRNG Data typedef */
typedef void* SST_SmallPRNG_Instance;

#ifdef __cplusplus
extern "C" {
#endif

	/*
	SST_Random_CreateSmallPRNG

	Creates a new SmallPRNG instance using the current time as the seed.

	@return (SST_SmallPRNG_Instance) - the created randomizer
	*/
	SST_SmallPRNG_Instance SST_Random_CreateSmallPRNG();

	/*
	SST_Random_CreateSmallPRNGFromSeed

	Creates a new SmallPRNG instance with the given seed.

	@param _seed - the seed the randomizer should be seeded with
	@return (SST_SmallPRNG_Instance) - the created randomizer
	*/
	SST_SmallPRNG_Instance SST_Random_CreateSmallPRNGFromSeed(uint32_t _seed);

	/*
	SST_Random_CloneSmallPRNG

	Makes a deep copy of the provided randomizer, and returns a handle to that copy.

	@param _instance - the instance to clone
	@return (SST_SmallPRNG_Instance) - a cloned instance
	*/
	SST_SmallPRNG_Instance SST_Random_CloneSmallPRNG(SST_SmallPRNG_Instance _instance);

	/*
	SST_Random_DestroySmallPRNG

	Deletes a SmallPRNG instance.

	@param _instance - the instance to delete
	@return (void)
	*/
	void SST_Random_DestroySmallPRNG(SST_SmallPRNG_Instance _instance);

	/*
	SST_Random_GetSmallPRNGFloat

	Gets a random float that lies within a range.

	@param _instance - the randomizer instance
	@param _min - the minimum value (inclusive)
	@param _max - the maximum value (exclusive)
	@return (float) - random float in range
	*/
	float SST_Random_GetSmallPRNGFloat(SST_SmallPRNG_Instance _instance, float _min, float _max);

	/*
	SST_Random_GetSmallPRNGFloatArray

	Fills a provided array with random floats that lie within a range.

	@param _instance - the randomizer instance
	@param _out - array to store floats to
	@param _numElements - number of elements to store in array
	@param _min - the minimum value (inclusive)
	@param _max - the maximum value (exclusive)
	@return (void)
	*/
	void SST_Random_GetSmallPRNGFloatArray(SST_SmallPRNG_Instance _instance, float* _out, size_t _numElements, float _min, float _max);

	/*
	SST_Random_GetSmallPRNGInt

	Gets a random integer that lies within a range.

	@param _instance - the randomizer instance
	@param _min - the minimum value (inclusive)
	@param _max - the maximum value (exclusive)
	@return (int) - random integer in range
	*/
	int SST_Random_GetSmallPRNGInt(SST_SmallPRNG_Instance _instance, int _min, int _max);

	/*
	SST_Random_GetSmallPRNGIntArray

	Fills a provided array with random ints that lie within a range.

	@param _instance - the randomizer instance
	@param _out - array to store the ints to
	@param _numElements - number of elements to store in array
	@param _min - the minimum value (inclusive)
	@param _max - the maximum value (exclusive)
	@return (void)
	*/
	void SST_Random_GetSmallPRNGIntArray(SST_SmallPRNG_Instance _instance, int* _out, size_t _numElements, int _min, int _max);

#ifdef __cplusplus
}
#endif

#endif
