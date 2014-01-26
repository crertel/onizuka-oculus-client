/*
	SST_Build.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/23/2011

	Purpose: 

	libsst-os user account information module

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_USER_H
#define _SST_USER_H

#ifdef __cplusplus
extern "C" {
#endif

	/*
		SST_OS_GetUserName

		Gets the current username for the user executing this process. This
		is the system username, e.g. "root" "jsmith".
		
		A typical usage scenario is to query then fetch the username:

		nrChars = SST_OS_GetUserName(NULL, 0);
		name = malloc(nrChars + 1);
		SST_OS_GetUserName(name, nrChars+1);

		@param usernameReturn - Buffer to hold username. Can be NULL to query size of username buffer required.
		@param bufferSize - The size in bytes of usernameReturn[], including the null terminator.
		@return - The number of characters in the name, excluding the null terminator. If < 0, then an error occurred,
				  or if zero, then no user name is available.
	*/
	int SST_OS_GetUserName(char* usernameReturn, int bufferSize);

	/*
		SST_OS_GetUserRealName

		Acts identically to SST_GetUserName(), except that it gets the user friendly name for the user.
		For example the username "jsmith" might have the user friendly name of "John Smith".

		@param realnameReturn - Buffer to hold the real name. Can be NULL to query size of username buffer required.
		@param bufferSize - The size in bytes of realnameReturn[], including the null terminator.
		@return - The number of characters in the name, excluding the null terminator. If < 0, then an error occurred,
				  or if zero, then no full name is available.
	*/
	int SST_OS_GetUserRealName(char* realnameReturn, int bufferSize);

	/*
		SST_OS_GetUserHomeDirectory

		Acts identically to SST_GetUserName(), except that it gets the path of the user's home directory. The
		directory does NOT end with a trailing slash.

		@param homedirReturn - Buffer to hold the home directory. Can be NULL to query size of username buffer required.
		@param bufferSize - The size in bytes of homedirReturn[], including the null terminator.
		@return - The number of characters in the path, excluding the null terminator. If < 0, then an error occurred,
				  or if zero, then no home directory is available.
	*/
	int SST_OS_GetUserHomeDirectory(char* homedirReturn, int bufferSize);

#ifdef __cplusplus
}
#endif

#endif


