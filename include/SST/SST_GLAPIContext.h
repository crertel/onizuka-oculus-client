/*
	SST_GLAPIContext.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/12/2012

	Purpose: 

	GL API context init/shutdown

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_GLAPICONTEXT_H
#define _SST_GLAPICONTEXT_H

#include <SST/SST_Build.h>
#include <SST/SST_GLAPIStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

	/* Thread local variable to hold API pointer list */
	#if SST_COMPILER == SST_MSVC
		extern __declspec(thread) SST_GLAPI* __sstglctx;
	#else
		extern __thread SST_GLAPI* __sstglctx;
	#endif

	/*
		SST_GLAPI_InitForThread()
		
		Loads an OpenGL library by name, retrieves function pointers, and stores them in a thread-local SST_GLAPI structure. Passing NULL for the library name means "use the system default", which correct for 
		most users. On Win32, pointers are //context-dependent//, so an OpenGL context //must// be active in order for this to work correctly. That means
		you should specify the //same// library name here that is used to initialize the context. This works well in conjunction with libsst-wm. Most likely,
		you want to pass NULL here, unless you specifically allow users to provide the name of an OpenGL implementation library (e.g. Mesa 3D).

		@param libGLName - The name of the OpenGL library to load, or NULL for default (e.g. "opengl32.dll" or "libGL.so.1")
		@return (int) - Zero on internal error, non-zero otherwise. Not finding all OpenGL symbols is not an error, e.g. GL 3.3 context vs GL 4.3 SST_GLAPI structure.
	*/
	int SST_GLAPI_InitForThread(const char* libGLName);
	
	/*
		SST_GLAPI_ShutdownForThread()
		
		Releases all memory associated with GL API pointer table. If the thread
		does not have any memory allocated, then this is a no-op. This memory is 
		NOT automatically freed -- if you use SST_GLAPI_InitForThread() or SST_GLAPI_CopyForThread(),
		you must call this in order to prevent a memory leak!
	*/
	void SST_GLAPI_ShutdownForThread();

	/*
		SST_GLAPI_GetThreadGLAPI()
		
		Gets the thread's current SST_GLAPI structure. This allows direct dispatch
		if desired. If SST_GLAPI_InitForThread() has never been called, or if
		SST_GLAPI_ShutdownForThread() was called after the last invocation of
		SST_GLAPI_InitForThread(), then this will return NULL.
		
		This pointer should not be freed; it is freed using SST_GLAPI_ShutdownForThread()
		
		@return (SST_GLAPI*) - The thread's SST_GLAPI structure.
	*/
	SST_GLAPI* SST_GLAPI_GetThreadGLAPI();
	
	/*
		SST_GLAPI_CopyForThread()
		
		Copies the given SST_GLAPI structure to the calling thread's API pointer. If you have a
		single thread switching between two GL contexts, copying the API pointers can be
		significantly faster than looking them up each frame.

		This can also be used to install your own OpenGL driver functions. If so, the libGL and libGLName
		fields should be NULL.
				
		@param api - Pointer to SST_GLAPI structure to copy the API from.
		@return (int) - Zero if unable to allocate memory (first time called), non-zero after that.
	*/
	int SST_GLAPI_CopyForThread(const SST_GLAPI* api);

#ifdef __cplusplus
}
#endif

#endif

