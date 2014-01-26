/*
	SST_NetTypes.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 8/7/2012

	Purpose: 

	Data types used by libsst-net

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_NETTYPES_H
#define _SST_NETTYPES_H

#include <pstdint.h>

/*=============================================================================
Typedefs
=============================================================================*/
typedef uintptr_t SST_Socket;
typedef void* SST_NetSocketSet;
typedef void* SST_NetPollDescriptorSet;
typedef void* SST_NetAddrInfo;
typedef void* SST_NetLingerInfo;


/*=============================================================================
Enumerations
=============================================================================*/
typedef enum SST_NetAddressFamily
{
	SSTNETADDRFAM_IPV4,
	SSTNETADDRFAM_IPV6,
	SSTNETADDRFAM_UNKNOWN,
	SSTNETADDRFAM_MAX
} SST_NetAddressFamily;

typedef enum SST_NetSocketType
{
	SSTNETSOCKTYPE_STREAM,
	SSTNETSOCKTYPE_DATAGRAM,
	SSTNETSOCKTYPE_UNKNOWN,
	SSTNETSOCKTYPE_SIZE
} SST_NetSocketType;

typedef enum SST_NetProtocol
{
	SSTNETPROTOCOL_TCP,
	SSTNETPROTOCOL_UDP,
	SSTNETPROTOCOL_UNKNOWN,
	SSTNETPROTOCOL_SIZE
} SST_NetProtocol;

typedef enum SST_NetNameInfoFlags
{
	SSTNETNINFOFLAGS_NOFQDN,
	SSTNETNINFOFLAGS_NUMBERICHOST,
	SSTNETNINFOFLAGS_NAMEREQD,
	SSTNETNINFOFLAGS_NUMERICSERVICE,
	SSTNETNINFOFLAGS_DATAGRAM
} SST_NetNameInfoFlags;

typedef enum SST_NetProtocolLevel
{
	SSTNETPROTOLEVEL_SOCKET,
	
	/* We'll add TCP, UDP, etc. levels later */

	SSTNETPROTOLEVEL_SIZE
} SST_NetProtocolLevel;

typedef enum SST_NetSocketOption
{
	/* Socket-level options */
	SSTNETSOCKOPT_DEBUG,
	SSTNETSOCKOPT_ACCEPTCONN,
	SSTNETSOCKOPT_BROADCAST,
	SSTNETSOCKOPT_REUSEADDR,
	SSTNETSOCKOPT_KEEPALIVE,
	SSTNETSOCKOPT_LINGER,
	SSTNETSOCKOPT_OOBINLINE,
	SSTNETSOCKOPT_SENDBUFFERSIZE,
	SSTNETSOCKOPT_RECVBUFFERSIZE,
	SSTNETSOCKOPT_ERRORSTATUS,
	SSTNETSOCKOPT_SOCKETTYPE,
	SSTNETSOCKOPT_DONTROUTE,

	/* We'll add other option levels later */

	SSTNETSOCKOT_SIZE
} SST_NetSocketOption;

typedef enum SST_NetPollFlag
{
	SSTNETPOLLFLAG_POLLPRI = 0x01,
	SSTNETPOLLFLAG_POLLRDBAND = 0x02,
	SSTNETPOLLFLAG_POLLRDNORM = 0x04,
	SSTNETPOLLFLAG_POLLWRNORM = 0x08,
	SSTNETPOLLFLAG_POLLERR = 0x10,
	SSTNETPOLLFLAG_POLLHUP = 0x20,
	SSTNETPOLLFLAG_POLLNVAL = 0x40,
	SSTNETPOLLFLAG_POLLIN = SSTNETPOLLFLAG_POLLRDNORM | SSTNETPOLLFLAG_POLLRDBAND,
	SSTNETPOLLFLAG_POLLOUT = SSTNETPOLLFLAG_POLLWRNORM,
	SSTNETPOLLFLAG_SIZE
} SST_NetPollFlag;

typedef enum SST_NetAddrInfoFlags
{
	SSTNETAINFOFLAG_PASSIVE = 0x01,
	SSTNETAINFOFLAG_CANONICALNAME = 0x02,
	SSTNETAINFOFLAG_NUMERICHOST = 0x04,
	SSTNETAINFOFLAG_ADDRESSCONFIG = 0x0400,
	SSTNETAINFOFLAG_NONAUTHORATATIVE = 0x04000,
	SSTNETAINFOFLAG_SECURE = 0x08000,
	SSTNETAINFOFLAG_RETURNPREFERREDNAME = 0x010000,
	SSTNETAINFOFLAG_FILESERVER = 0x00040000
} SST_NetAddrInfoFlags;

/*=============================================================================
Defines
=============================================================================*/

#define SSTNET_INVALIDSOCKET ((uintptr_t)NULL)

#define SSTNET_DONTROUTE	(1 << 0)
#define SSTNET_OOB			(1 << 1)
#define SSTNET_PEEK			(1 << 2)
#define SSTNET_WAITALL		(1 << 3)


/*=============================================================================
Structures
=============================================================================*/

/* Structure representing a network address. It intentionally mimics struct sockaddr_storage. */
typedef struct SST_NetAddress
{
	uint16_t ss_family;		/* 2 bytes, total 2 */	
	uint8_t __ss_pad1[6];	/* 6 bytes, total 8 */
	uint64_t __ss_align;	/* 8 bytes, total 16 */
	uint8_t __ss_pad2[112];	/* 112 bytes, total 128 */
} SST_NetAddress;

typedef struct SST_NetTimeval
{
    long seconds;
    long microseconds;
} SST_NetTimeval;

typedef struct SST_NetAddrHints
{
	SST_NetAddrInfoFlags flags;
	SST_NetAddressFamily addressFamily;
	SST_NetSocketType socketType;
	SST_NetProtocol protocolType;
} SST_NetAddrHints;

#endif

