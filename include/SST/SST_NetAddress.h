/*
	SST_NetAddress.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 8/7/2012

	Purpose: 

	Address storage manipulation

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_NetAddress_H
#define _SST_NetAddress_H


#include <SST/SST_NetResult.h>
#include <SST/SST_NetTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

	SST_NetResult SST_Net_InitAddress(SST_NetAddress* addr, SST_NetAddressFamily family, unsigned short port);

	SST_NetAddressFamily SST_Net_GetAddressFamily(const SST_NetAddress* addr);

	unsigned short SST_Net_GetAddressPort(const SST_NetAddress* addr);

	int SST_Net_AddressCompare(const SST_NetAddress* addr1, const SST_NetAddress* addr2);

	int SST_Net_AddressIsLocalhost(const SST_NetAddress* addr);

	void SST_Net_AddressSetToLocalhost(SST_NetAddress* addr);

	SST_NetResult SST_Net_SetAddressFromAddress(SST_NetAddress* dst, const SST_NetAddress* src);

	SST_NetResult SST_Net_SetAddressFromString(SST_NetAddress* addr, const char* addrString);

#ifdef __cplusplus
}
#endif


#endif


