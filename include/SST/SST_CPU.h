/*
	SST_CPU.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 12/20/2011

	Purpose: 

	libsst-os CPU query module

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_CPU_H
#define _SST_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

	/*
		SST_OS_GetNumberCPUChips

		Gets the number of CPU chips in the machine. This is not the number
		of cores or threads, but actual sockets that have CPUs in them. Most
		PCs have a single CPU socket, while workstations and servers tend to
		have 2 or more.

		@return (int) - > 0: The number of CPU chips, 0: unknown, < 0: error
	*/
	int SST_OS_GetNumberCPUChips(void);

	/*
		SST_OS_GetNumberPhysicalCPUs

		Gets the number of physical CPUs in the system. For example, a single socket
		quad core system would return 4 here, and a dual socket 6-core system would
		return 12 here.

		@return (int) - > 0: The number of CPU cores, 0: unknown, < 0: error
	*/
	int SST_OS_GetNumberPhysicalCPUs(void);

	/*
		SST_OS_GetNumberLogicalCPUs

		Gets the number of logical CPUs in the system. Logical CPUs are scheduled
		by the OS as CPUs but can be just hardware threads that share execution
		resources (e.g. Intel's HyperThreading).

		@return (int) - > 0: The number of logical CPUs, 0: unknown, < 0: error
	*/
	int SST_OS_GetNumberLogicalCPUs(void);

	/*
		SST_OS_MapPhysicalToLogicalCPU

		Maps a physical CPU to one or more logical CPUs. For systems where 
		the number of physical CPUs is equal to the number of logical CPUs,
		this is an identity transformation. If SST_OS_GetNumberPhysicalCPUs()
		or SST_OS_GetNumberLogicalCPUs() returns 0 (i.e. no information provided
		for this platform), then this function does nothing and returns 0 as well.

		This is useful for mapping similar threads to one physical CPU.

		@param physCpuId - The physical CPU ID, between 0 and SST_OS_GetNumberPhysicalCPUs()-1
		@param logCpuIds - Pointer to an array of integers to fill in with the logical CPU IDs.
						   This can vary per physical CPU (rarely), so 'physical / logical' is not
						   necessarily safe. To be completely safe, it should be SST_OS_GetNumberLogicalCPUs()
						   in size.
		@return (int) - > 0: The number of logical CPUs returned, 0: API not available, < 0: error.
	*/
	int SST_OS_MapPhysicalToLogicalCPU(int physCpuId, int* logCpuIds);

	/*
		SST_OS_GetCPUAffinity

		Gets the current thread's logical CPU affinity as bitmask.

		Logical CPU 'n' is represented by bit (n%8) of byte (n/8). In C, this is the expression
		"(cpuMaskReturn[n/8] & (1 << (n%8))) >> n".

		The number of bytes required to fully hold the CPU mask given 'k' logical CPUs
		is given by the expression (k+7)/8. Unused bits are initialized to zero.

		@param cpuMaskReturn - An array of bytes representing the logical CPU bits
		@param cpuMaskSize - The size (in bytes) of cpuMaskReturn.
		@return (int) - > 0: success, 0: API not available, < 0: error.
	*/
	int SST_OS_GetCPUAffinity(unsigned char* cpuMaskReturn, unsigned int cpuMaskSize);

	/*
		SST_OS_SetCPUAffinity

		Sets the current thread's logical CPU affinity as bitmask. Though a hint; it is generally
		strongly enforced by the OS unless exceptional events occur.

		Logical CPU 'n' is represented by bit (n%8) of byte (n/8). In C, this is the expression
		"(cpuMaskReturn[n/8] & (1 << (n%8))) >> n".

		The number of bytes required to fully hold the CPU mask given 'k' logical CPUs
		is given by the expression (k+7)/8. Unused bits are ignored.

		@param cpuMask - An array of bytes representing the logical CPU bits
		@param cpuMaskSize - The size (in bytes) of cpuMask.
		@return (int) - > 0: success, 0: API not available, < 0: error.
	*/
	int SST_OS_SetCPUAffinity(const unsigned char* cpuMask, unsigned int cpuMaskSize);

	/*
		SST_OS_SetPreferredCPU

		Hints to the OS that the currently bound thread would prefer to run on a given CPU.
		This is less strong of a hint than SST_OS_SetCPUAffinity()

		@param logCpuId - The logical CPU ID
	*/
	void SST_OS_SetPreferredCPU(int logCpuId);

#ifdef __cplusplus
}
#endif

#endif


