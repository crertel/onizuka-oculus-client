/*
	SST_Once.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-concurrency "once" functions. Requires libsst-atomic.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_ONCE_H
#define _SST_ONCE_H

/* Opaque type for "once" initializer which should be statically initialized to SST_ONCE_INIT */
typedef volatile unsigned int SST_Once;

/* Typedef for a function pointer that will be executed by SST_ExecOnce() [(void*) -> void] */
typedef void (*SST_OnceFunc)(void*);

#define SST_ONCE_INIT	0x00FFFF00

#ifdef __cplusplus
extern "C" {
#endif

	/*
	SST_Concurrency_ExecOnce

	Provides a safe way to call a callback function in a multithreaded environment that needs
	to be executed just once. Many threads may call SST_Concurrency_ExecOnce(), but only one
	single call to the user provided callback will be made. This is useful for avoiding race
	conditions for initialization, e.g:

		if(g_Resource == NULL)
		{
			//assume CreateResource() assigns 'g_Resource'
			CreateResource(g_Params);
		}

	Instead, this can be done as:

		SST_Once g_resourceOnce = SST_ONCE_INIT;

		//Only ever gets executed once
		SST_Concurrency_ExecOnce(&g_resourceOnce, CreateResource, g_Params);


	@param once - The "once" handle. It should be initialized to SST_ONCE_INIT statically.
	@param once_func - The function to be called a single time.
	@param arg - The argument provided to the 'once_func' callback. This value not interpreted by libsst-concurrency.
	*/
	void SST_Concurrency_ExecOnce(SST_Once* once, SST_OnceFunc once_func, void* arg);

	/*
		SST_Concurrency_InitOnceHandle
	
		Used to initialize a "once" handle dynamically. This must be done in
		in a serialized manner, i.e. only one thread should call this ever,
		and, ironically, it should be done only once. For example:

		//------------
		//main thread:
		SST_Once* once = malloc(sizeof(SST_Once));
		SST_Concurrency_InitOnceHandle(once);

		CreateBunchesOfThreads(once); //pass once handle to them.
		//------------

		@param once - The "once" handle to initialize.
	*/
	void SST_Concurrency_InitOnceHandle(SST_Once* once);

#ifdef __cplusplus
}
#endif

#endif

