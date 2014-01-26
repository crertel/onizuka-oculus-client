/*
	SST_NetSocket.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 8/7/2012

	Purpose: 

	Socket functions resembling POSIX sockets interface

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_NETSOCKET_H
#define _SST_NETSOCKET_H


#include <SST/SST_NetResult.h>
#include <SST/SST_NetTypes.h>
#include <stdlib.h> /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

	SST_NetResult SST_Net_Socket( SST_NetAddressFamily _addressFamily, SST_NetSocketType _socketType,  SST_NetProtocol _protocol, SST_Socket* _createdSocket);
	SST_NetResult SST_Net_Bind(SST_Socket sock, const SST_NetAddress* _address);
	SST_NetResult SST_Net_Listen(SST_Socket sock, int backlog);
	SST_NetResult SST_Net_Connect(SST_Socket sock, const SST_NetAddress* _address);
	SST_NetResult SST_Net_Accept(SST_Socket sock, SST_NetAddress* _address, SST_Socket* _accepted);
	SST_NetResult SST_Net_Send(SST_Socket sock, const void* _data, size_t _dataLength, uint32_t _flags, size_t* _bytesSent);
	SST_NetResult SST_Net_Recv(SST_Socket sock, void* _storage, size_t _storageLength, uint32_t _flags, size_t* _bytesReceived);
	SST_NetResult SST_Net_SendTo(SST_Socket sock, const void* _data, size_t _dataLength, uint32_t _flags, const SST_NetAddress* _destination, size_t* _bytesSent);
	SST_NetResult SST_Net_RecvFrom(SST_Socket sock, void* _storage, size_t _storageLength, uint32_t _flags, SST_NetAddress* _sender, size_t* _bytesReceived);
	void SST_Net_Close(SST_Socket sock);

	SST_NetResult SST_Net_GetSockOpt(SST_Socket sock, SST_NetProtocolLevel level, SST_NetSocketOption option, void* optionStorage, size_t* optionStorageSize);
	SST_NetResult SST_Net_SetSockOpt(SST_Socket sock, SST_NetProtocolLevel level, SST_NetSocketOption option, const void* optionData, size_t optionDataSize);
	SST_NetResult SST_Net_SetNonblock(SST_Socket sock, int nonblock);
#ifdef __cplusplus
}
#endif

#endif


