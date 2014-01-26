/*
	SST_Concurrency.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	762 Studios Concurrency Library

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_CONCURRENCY_H
#define _SST_CONCURRENCY_H

/* Version number constants for libsst-conconcurrency */
#define SST_CONCURRENCY_VERSION_MAJOR	0x01
#define SST_CONCURRENCY_VERSION_MINOR	0x01
#define SST_CONCURRENCY_VERSION_PATCH	0x0000
#define SST_CONCURRENCY_VERSION	(SST_CONCURRENCY_VERSION_MAJOR << 24) | (SST_CONCURRENCY_VERSION_MINOR << 16) | (SST_CONCURRENCY_VERSION_PATCH)

#define SST_CONCURRENCY_VERSION_STRING	"1.1.0"

/* Symbolic constant meaning "wait as long as necessary" */
#define SST_WAIT_INFINITE ((uint32_t)0xFFFFFFFF)

#include "SST_Thread.h"
#include "SST_Mutex.h"
#include "SST_TLS.h"
#include "SST_Event.h"
#include "SST_Semaphore.h"
#include "SST_ReadWriteLock.h"
#include "SST_Once.h"
#include "SST_ThreadBarrier.h"
#include "SST_MemBarrier.h"

#endif
