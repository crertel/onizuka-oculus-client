/*
	SST_CMWC.h
	Author: James Russell <jcrussell@762studios.com>
	Created: 11/13/2011

	Purpose: 

	Complementary Multiply with carry randomizer interface.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#ifndef _SST_CMWC_H
#define _SST_CMWC_H

#include <pstdint.h>
#include <stdlib.h>		/* size_t */

/* CMWC Data typedef */
typedef void* SST_CMWC_Instance;

#ifdef __cplusplus
extern "C" {
#endif

	/*
	SST_Random_CreateCMWC

	Creates a new randomizer instance using the current time as the seed.

	@return (SST_CMWC_Instance) - the created randomizer
	*/
	SST_CMWC_Instance SST_Random_CreateCMWC();

	/*
	SST_Random_CreateCMWCFromSeed

	Creates a new multiply with carry randomizer instance with the given seed.

	@param _seed - the seed the randomizer should be seeded with
	@return (SST_CMWC_Instance) - the created randomizer
	*/
	SST_CMWC_Instance SST_Random_CreateCMWCFromSeed(uint32_t _seed);

	/*
	SST_Random_CloneCMWC

	Makes a deep copy of the provided randomizer, and returns a handle to that copy.

	@param _instance - the instance to clone
	@return (SST_CMWC_Instance) - a cloned instance
	*/
	SST_CMWC_Instance SST_Random_CloneCMWC(SST_CMWC_Instance _instance);

	/*
	SST_Random_DestroyCMWC

	Deletes a CMWC instance.

	@param _instance - the instance to delete
	@return (void)
	*/
	void SST_Random_DestroyCMWC(SST_CMWC_Instance _instance);

	/*
	SST_Random_GetCMWCFloat

	Gets a random float that lies within the provided range.

	@param _instance - the randomizer instance
	@param _min - the minimum value (inclusive)
	@param _max - the maximum value (exclusive)
	@return (float) - random float in range
	*/
	float SST_Random_GetCMWCFloat(SST_CMWC_Instance _instance, float _min, float _max);

	/*
	SST_Random_GetCMWCFloatArray

	Fills a provided array with random floats that lie within the provided range.

	@param _instance - the randomizer instance
	@param _out - array to store floats to
	@param _numElements - number of elements to store in array
	@param _min - the minimum value (inclusive)
	@param _max - the maximum value (exclusive)
	@return (void)
	*/
	void SST_Random_GetCMWCFloatArray(SST_CMWC_Instance _instance, float* _out, size_t _numElements, float _min, float _max);

	/*
	SST_Random_GetCMWCInt

	Gets a random integer that lies within the provided range.

	@param _instance - the randomizer instance
	@param _min - the minimum value (inclusive)
	@param _max - the maximum value (exclusive)
	@return (int) - random integer in range
	*/
	int SST_Random_GetCMWCInt(SST_CMWC_Instance _instance, int _min, int _max);

	/*
	SST_Random_GetCMWCIntArray

	Fills a provided array with random ints that lie within the provided range.

	@param _instance - the randomizer instance
	@param _out - array to store ints to
	@param _numElements - number of elements to store in array
	@param _min - the minimum value (inclusive)
	@param _max - the maximum value (exclusive)
	@return (void)
	*/
	void SST_Random_GetCMWCIntArray(SST_CMWC_Instance _instance, int* _out, size_t _numElements, int _min, int _max);

#ifdef __cplusplus
}
#endif

#endif
