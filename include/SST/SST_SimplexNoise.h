/*
	SST_SimplexNoise.h
	Author: Chris Ertel <crertel@762studios.com>
	Created: 11/27/2011

	Purpose: 

	Implementation of simplex noise for libsst.

	Stefan Gustavson's paper "Simplex Noise Demystified" was an invaluable resource and the algorithms
	presented here are ported from his Java code.

	(paper available at http://www.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf)

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#ifndef _SST_SIMPLEXNOISE_H
#define _SST_SIMPLEXNOISE_H

#include <pstdint.h>
#include <stdlib.h> /* size_t */

/* Noise Instance */
typedef void* SST_SimplexNoise;

#ifdef __cplusplus
extern "C" {
#endif

	/*
	SST_Random_CreateSimplexNoise
	 
	Creates a new simplex noise instance using the current time as a seed.

	@return (SST_Simplex_Instance) - new simplex noise generator
	*/
	SST_SimplexNoise SST_Random_CreateSimplexNoise();

	/*
	SST_Random_CreateSimplexNoiseFromSeed
	 
	Creates a new simplex noise instance using the provided seed.
	 
	@param _seed - seed to use for generating noise
	@return (SST_Simplex_Instance) - new simplex noise instance
	*/
	SST_SimplexNoise SST_Random_CreateSimplexNoiseFromSeed(uint32_t _seed);

	/*
	SST_Random_ReseedSimplexNoise
	 
	Reseeds a given simplex noise instance.

	@param _source - noise instance to reseed.
	@param _seed - seed to seed noise with
	@return (void)
	*/
	void SST_Random_ReseedSimplexNoise(SST_SimplexNoise _source, uint32_t _seed);

	/*
	SST_SimplexNoise_Clone
	 
	Duplicates a given noise instance, making a deep copy of state.
	 
	@param _source - noise instance to duplicate
	@return (SST_Simplex_Instance) - new simplex noise instance
	*/
	SST_SimplexNoise SST_Random_CloneSimplexNoise(SST_SimplexNoise _source);

	/*
	SST_Random_DestroySimplexNoise
	 
	Frees the resources of a given simplex noise instance.
	 
	@param _instance - noise instance to free the resources of
	@return (void)
	*/
	void SST_Random_DestroySimplexNoise(SST_SimplexNoise _instance);

	/*
	SST_Random_MapSimplexNoise1D
	 
	Gets a value from a noise instance given a 1D coordinate.
	 
	@param _instance - noise instance to get value from
	@param _x - first coordinate on interval [0,1]
	@return (float) - value of noise at that point on interval [-1,1]
	*/
	float SST_Random_MapSimplexNoise1D( SST_SimplexNoise _instance, float _x);

	/*
	SST_Random_MapSimplexNoise1DFromArray
	 
	Fills an array of floats using coordinates provided in another array.
	 
	@param _instance - noise instance to get value from
	@param _destination - array to store floats
	@param _sourceX - array to read first coordinates from sampling in fro
	@param _numElements - size of both destination and source array.
	@return (void)
	*/
	void SST_Random_MapSimplexNoise1DFromArray( SST_SimplexNoise _instance, float* _destination, float* _sourceX, size_t _numElements);

	/*
	SST_Random_MapSimplexNoise2D
	 
	Gets a value from a noise instance given a 1D coordinate.
	 
	@param _instance - noise instance to get value from
	@param _x - first coordinate on interval [0,1]
	@param _y - second coordinate on interval [0,1]
	@return (float) - value of noise at that point on interval [-1,1]
	*/
	float SST_Random_MapSimplexNoise2D( SST_SimplexNoise _instance, float _x, float _y);

	/*
	SST_Random_MapSimplexNoise2DFromArray
	 
	Fills an array of floats using coordinates provided in other arrays.
	 
	@param _instance - noise instance to get value from
	@param _destination - array to store floats
	@param _sourceX - array to read first coordinates from sampling in from
	@param _sourceY - array to read second coordinates from sampling in from
	@param _numElements - size of both destination and source array
	@return (void)
	*/
	void SST_Random_MapSimplexNoise2DFromArray( SST_SimplexNoise _instance, float* _destination, float* _sourceX, float* _sourceY, size_t _numElements);

	/*
	SST_Random_MapSimplexNoise3D

	Gets a value from a noise instance given a 1D coordinate.

	@param _instance - noise instance to get value from
	@param _x - first coordinate on interval [0,1]
	@param _y - second coordinate on interval [0,1]
	@param _z - third coordinate on interval [0,1]
	@return (float) - value of noise at that point on interval [-1,1]
	*/
	float SST_Random_MapSimplexNoise3D( SST_SimplexNoise _instance, float _x, float _y,  float _z);

	/*
	SST_Random_MapSimplexNoise3DFromArray
	 
	Fills an array of floats using coordinates provided in other arrays.

	@param _instance - noise instance to get value from
	@param _destination - array to store floats
	@param _sourceX - array to read first coordinates from sampling in from
	@param _sourceY - array to read second coordinates from sampling in from
	@param _sourceZ - array to read third coordinates from sampling in from
	@param _numElements - size of both destination and source array
	@return (void)
	*/
	void SST_Random_MapSimplexNoise3DFromArray( SST_SimplexNoise _instance, float* _destination, float* _sourceX, float* _sourceY, float* _sourceZ, size_t _numElements);

	/*
	SST_Random_MapSimplexNoise4D

	Gets a value from a noise instance given a 1D coordinate.

	@param _instance - noise instance to get value from.
	@param _x - first coordinate on interval [0,1]
	@param _y - second coordinate on interval [0,1]
	@param _z - third coordinate on interval [0,1]
	@param _w - fourth coordinate on interval [0,1]
	@return (float) - value of noise at that point on interval [-1,1]
	*/
	float SST_Random_MapSimplexNoise4D( SST_SimplexNoise _instance, float _x, float _y,  float _z, float _w);

	/*
	SST_Random_MapSimplexNoise4DFromArray
	 
	Fills an array of floats using coordinates provided in other arrays.

	@param _instance - noise instance to get value from
	@param _destination - array to store floats
	@param _sourceX - array to read first coordinates from sampling in from
	@param _sourceY - array to read second coordinates from sampling in from
	@param _sourceZ - array to read third coordinates from sampling in from
	@param _sourceW - array to read fourth coordinates from sampling in from
	@param _numElements - size of both destination and source array
	@return (void)
	*/
	void SST_Random_MapSimplexNoise4DFromArray( SST_SimplexNoise _instance, float* _destination, float* _sourceX, float* _sourceY, float* _sourceZ, float* _sourceW, size_t _numElements);

#ifdef __cplusplus
}
#endif

#endif 
