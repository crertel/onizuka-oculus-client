/*
	SST_Sockets.h
	Author: Chris Ertel <crertel@762studios.com>
	Created: 7/8/2012

	Purpose: 

	Cross-platform BSD/POSIX-style sockets wrapper functions.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_SOCKETS_H
#define _SST_SOCKETS_H

#include <stddef.h>
#include <SST/SST_NetResult.h>
#include <SST/SST_NetTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

int SST_Net_Init( void );
void SST_Net_Shutdown( void );

SST_NetResult SST_Net_CreateSocketSet( SST_NetSocketSet* _out );
void SST_Net_DestroySocketSet( SST_NetSocketSet _set );
void SST_Net_ClearSocketFromSocketSet( SST_NetSocketSet _set, SST_Socket _sock);
void SST_Net_AddSocketToSocketSet( SST_NetSocketSet _set, SST_Socket _sock);
int SST_Net_IsSocketInSocketSet( SST_NetSocketSet _set, SST_Socket _sock);
void SST_Net_ZeroSocketSet( SST_NetSocketSet _set);
SST_NetResult SST_Net_Select( size_t _numSockets, SST_NetSocketSet _readSockets, SST_NetSocketSet _writeSockets, SST_NetSocketSet _errorSockets, SST_NetTimeval* _timeout, size_t* _numEvented);

SST_NetResult SST_Net_CreatePollDescriptorSet(size_t _numDescriptors, SST_NetPollDescriptorSet* _out);
void SST_Net_DestroyPollDescriptorSet(SST_NetPollDescriptorSet _toDestroy);
void SST_Net_SetPollDescriptorSocket(SST_NetPollDescriptorSet _descriptors, size_t _which, SST_Socket _socket);
SST_Socket SST_Net_GetPollDescriptorSocket(SST_NetPollDescriptorSet _descriptors, size_t _which);
void SST_Net_SetPollDescriptorFlags(SST_NetPollDescriptorSet _descriptors, size_t _which, SST_NetPollFlag _flags);
SST_NetPollFlag SST_Net_GetPollDescriptorFlags(SST_NetPollDescriptorSet _descriptors, size_t _which);
void SST_Net_ClearPollDescriptorFlags(SST_NetPollDescriptorSet _descriptors, size_t _which);
SST_NetPollFlag SST_Net_GetPollDescriptorEvents(SST_NetPollDescriptorSet _descriptors, size_t _which);
SST_NetResult SST_Net_Poll( SST_NetPollDescriptorSet _toPoll, int _timeout, size_t* _numEvented);

SST_NetResult SST_Net_GetSockOpt( SST_Socket _socket, SST_NetProtocolLevel _level, SST_NetSocketOption _option, void* _optionStorage, size_t* _optionStorageSize);
SST_NetResult SST_Net_SetSockOpt( SST_Socket _socket, SST_NetProtocolLevel _level, SST_NetSocketOption _option, const void* _optionData, size_t* _optionDataSize);

SST_NetResult SST_Net_PToN( SST_NetAddressFamily _family, const char* _addressText, SST_NetAddress _storage);
SST_NetResult SST_Net_NToP( SST_NetAddressFamily _family, SST_NetAddress _data, char* _addressTextStorage, size_t _addressTextStorageLength);

SST_NetResult SST_Net_GetAddrInfo( const char* _node, const char* _service, const SST_NetAddrHints* _hints, SST_NetAddrInfo* _results);
void SST_Net_FreeAddrInfo( SST_NetAddrInfo _toFree );
SST_NetResult SST_Net_GetNameInfo(SST_NetAddress _address, char* _hostName, size_t _hostNameLength, char* _serviceName, size_t _serviceNameLength, SST_NetNameInfoFlags _flags);

/*
RESERVED FOR LATER VERSIONS OF LIBSST-NET

SST_NetResult SST_Net_IOCtlSocket( SST_Socket _socket, SST_NetIOCommand _command, void* _args);
*/

#ifdef __cplusplus
};
#endif

#endif