/*
	SST_Event.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-concurrency event functions

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_EVENT_H
#define _SST_EVENT_H

#include <pstdint.h>

/* Opaque type representing an event */
typedef void* SST_Event;

#ifdef __cplusplus
extern "C" {
#endif

	/*
	SST_Concurrency_CreateEvent

	Creates an event object, initially unsignaled.

	@return (SST_Event) - The handle to the event. Use DestroyEvent() when done.
	*/
	SST_Event SST_Concurrency_CreateEvent(void);

	/*
	SST_Concurrency_DestroyEvent

	Destroys an event object.

	@param event - the event object to be destroyed
	@return (void)
	*/
	void SST_Concurrency_DestroyEvent(SST_Event event);

	/*
	SST_Concurrency_SignalEvent

	Signals an event. If the event is already signaled, this has no effect. All threads that were
	waiting on this event (by WaitEvent()) are unblocked.

	@param event - the event object to be signaled
	@return (void)
	*/
	void SST_Concurrency_SignalEvent(SST_Event _event);

	/*
	SST_Concurrency_ResetEvent

	Resets the event's state to unsignaled. If the event is already unsignaled, this has no effect.

	@param event - the event object to be reset
	@return (void)
	*/
	void SST_Concurrency_ResetEvent(SST_Event _event);

	/*
	SST_Concurrency_WaitEvent

	Blocks the calling thread until the given event is signaled. If the event was already signaled,
	then this function returns immediately.

	@param event - the event object to wait on
	@param ticks - the time to wait in milliseconds (0 = don't wait (test only), SST_WAIT_INFINITE = block indefinitely)
	@return (int) - non-zero if the event was signaled when it returns, zero if the event wasn't signaled before returning
	*/
	int SST_Concurrency_WaitEvent(SST_Event _event, uint32_t _ticks);

#ifdef __cplusplus
}
#endif

#endif

