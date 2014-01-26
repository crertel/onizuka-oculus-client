/*
	SST_WMOpenGL.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 5/30/2012

	Purpose: 

	OpenGL creation

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_WMOPENGL_H
#define _SST_WMOPENGL_H

#include <SST/SST_WMTypes.h>

/* Supported OpenGL APIs */
typedef enum SST_WMOpenGLType
{
	SSTGL_OPENGL,
	SSTGL_OPENGL_ES
} SST_WMOpenGLType;

/* OpenGL context information */
typedef struct SST_OpenGLContextAttributes
{
	uint8_t contextVersionMajor;	/**< OpenGL major version, e.g. 3 in "3.1" */
	uint8_t contextVersionMinor;	/**< OpenGL minor version, e.g. 1 in "3.1" */
	uint8_t colorBits;				/**< Number of color bits in the framebuffer. Typically 24 */
	uint8_t alphaBits;				/**< Number of alpha bits in the framebuffer. Typically 0 or 8 */
	uint8_t depthBits;				/**< Number of depth bits in the framebuffer. Typically 24 */
	uint8_t stencilBits;			/**< Number of stencil bits in the framebuffer. Typically 8 */
	uint8_t stereoEnabled;			/**< If true, enable quad-buffered stereoscopic rendering */
	uint8_t multisampleFactor;		/**< The multisample factor. 0 or 1 is disabled, 2 = 2x MSAA, 4 = 4x MSAA and so on */
	uint8_t debugEnabled;			/**< Enable GL debugging, if possible. */
	uint8_t legacyEnabled;			/**< Target GL < 3.0 (compatibility profile), otherwise use forward-compatible/core */
} SST_OpenGLContextAttributes;

#ifdef __cplusplus
extern "C" {
#endif

	/*
		SST_WM_CreateOpenGLContext()
		
		Creates an OpenGL context of the specified type with the given attributes. If API or context parameters
		aren't supported, then this fails. The resulting OpenGL context will only function on windows created by
		the given display target. The context is not initially bound to the given thread.
		
		@param target - The display target that wants to drive the OpenGL context
		@param apiType - The API type.
		@param attribs - The OpenGL context attributes to use. They are matched "equal or better" or else this fails.
		@param selectedAttribsReturn - The OpenGL context attributes that were picked and set. This can be NULL if not interested.
		@return (SST_OpenGLContext) - The OpenGL context, or NULL on error.
	*/
	SST_OpenGLContext SST_WM_CreateOpenGLContext(SST_DisplayTarget target, SST_WMOpenGLType apiType, const SST_OpenGLContextAttributes* attribs, SST_OpenGLContextAttributes* selectedAttribsReturn);

	/*
		SST_WM_CreateSlaveOpenGLContext()
		
		Creates a second "slave" OpenGL context that can be used for resource creation. It shares
		resources with the other context, but is otherwise a separate OpenGL context. It shouldn't
		be used except for doing resource creation/loading.
		
		Dev Note: Since this feature requires mature/advanced OpenGL drivers in the host system,
		it is possible that driver bugs will cause invalid rendering/crashes for older drivers.
		You should always have a way to run the application without making use of this API at the
		user's choice.
		
		@param masterGLContext - The "master" OpenGL context. It must be unbound (not used by any thread) and must
		                         have been created with SST_WM_CreateOpenGLContext() (i.e. must not be a "slave" context)
		@return (SST_OpenGLContext) - The slave context, or NULL on error / not supported
	*/
	SST_OpenGLContext SST_WM_CreateSlaveOpenGLContext(SST_OpenGLContext masterGLContext);

	/*
		SST_WM_SwapOpenGLBuffers()
		
		Swaps front and back buffers of the window that the context is bound to. In stereoscopic rendering,
		this swaps both the left and right eye buffers.
		
		@param ctx - The OpenGL context to swap.
		@return (void)
	*/
	void SST_WM_SwapOpenGLBuffers(SST_OpenGLContext ctx);

	/*
		SST_WM_BindOpenGLContext()
		
		Instructs the thread to take ownership of a context and to bind its rendering commands
		to the given window's buffers. If this is a slave context, then the window must be NULL
		since it isn't used for drawing. The window must be created on the same display target
		as the window passed to SST_WM_CreateOpenGLContext().

		On failure, the state of the current context is left unchanged.

		NOTE: Due to a limitation of the Windows operating system, this function may only be called
		with contexts whose SSTOpenGLContextAttributes returned from SST_WM_CreateOpenGLContext() are
		identical. For example:

			SSTOpenGLContextAttributes attrs = { ... };
			ctx1 = SST_WM_CreateOpenGLContext(..., &attrs, ...);
			ctx2 = SST_WM_CreateOpenGLContext(..., &attrs, ...);

			SST_WM_BindOpenGLContext(ctx1, someWindow);
			SST_WM_BindOpenGLContext(ctx2, someWindow); //OK, created with same attributes.

		If 'ctx1' and 'ctx2' were not created with the same attributes, then the results are undefined.

		@param ctx - The OpenGL context to bind, or NULL to unbind all contexts from the thread.
		@param window - The window to bind to. This must be NULL if it is a slave context or when 'ctx' is NULL.
		@return (int) - Non-zero if successful, zero on failure.
	*/	
	int SST_WM_BindOpenGLContext(SST_OpenGLContext ctx, SST_Window window);
	
	/*
		SST_WM_DestroyOpenGLContext()
		
		Destroys an OpenGL unbound context. If context is bound to any thread, including the caller,
		then the results are undefined.
		
		@param ctx - The OpenGL context to destroy.
		@return (void)
	*/
	void SST_WM_DestroyOpenGLContext(SST_OpenGLContext ctx);

#ifdef __cplusplus
}
#endif

#endif


