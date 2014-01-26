/*
	SST_TLS.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-concurrency thread local storage functions

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_TLS_H
#define _SST_TLS_H

#include <pstdint.h>

/* Opaque type representing a TLS slot */
#if defined(_WIN32) || defined(WIN32) || defined(__WIN32) || defined(__WIN32__)
	typedef uint32_t SST_TLS; /* Win32 API uses DWORD, complains about pointer truncation if this is a typedef for void* */
#else
	typedef void* SST_TLS;
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/*
	SST_Concurrency_CreateTLS

	Allocates a thread local storage handle. If none available, this returns NULL.

	@return - a TLS handle
	*/
	SST_TLS SST_Concurrency_CreateTLS(void);

	/*
	SST_Concurrency_DestroyTLS

	Destroys a thread local storage handle.

	@param tls - The thread local storage handle to destroy.
	*/
	void SST_Concurrency_DestroyTLS(SST_TLS tls);

	/*
	SST_Concurrency_GetTLSValue

	Gets a thread local storage value by handle. If the thread has never set it, defaults to NULL.

	@param tls - The thread local storage handle
	@return - The value stored in the thread storage
	*/
	void* SST_Concurrency_GetTLSValue(SST_TLS tls);

	/*
	SST_Concurrency_SetTLSValue

	Sets a thread local storage value by handle.

	@param tls - The thread local storage handle
	@param value - The value to store
	*/
	void SST_Concurrency_SetTLSValue(SST_TLS tls, void* value);

#ifdef __cplusplus
}
#endif

#endif

