/*
	SST_Assert.h
	Author: Patrick Baggett <ptbaggett@762studios.com>, James Russell <jcrussell@762studios.com>
	Created: 12/05/2011

	Purpose: 

	libsst-os runtime assertion functions.

	By default, SST_OS_RuntimeAssert() will produce a message that has references to
	the failed condition, the file, line number, and function name. For companies where
	confidentiality of these details are necessary, #define'ing SST_ASSERT_NOINFO will
	result in calls to SST_OS_RuntimeAssert() that exclude any references to file/line/function.

	This is also useful if you wish to reduce the size of the executable image but have
	many uses of SST_OS_RuntimeAssert().

	The option is not all-or-nothing. It can be enabled for some files, disabled for others.
	For example, if the entire project is compiled with SST_ASSERT_NOINFO, then to enable extra
	assert information for a single *.c / *.cpp file, simply do:

	#undef SST_ASSERT_NOINFO
	#include <SST/SST_Assert.h>

	Debugging asserts can be removed (e.g. for optimized release builds) by #define'ing SST_ASSERT_NODEBUG.

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#ifndef _SST_ASSERT_H
#define _SST_ASSERT_H

#include <SST/SST_Build.h> /* WHILE_NOP definition */

/* typedef for assert handler functions */
typedef int (*SST_AssertHandlerFunc)(const char*, void*);

/************************************************************************************/
/* Compiler magic. It defines PORTABLE_TRAP(). This is a compiler/architecture      *
specific function to signal the debugger to break/trap.								*/
/************************************************************************************/

#ifdef _MSC_VER /* MSVC */
	#define PORTABLE_TRAP()		__debugbreak()
#elif defined(__GNUC__) /* GNU C/C++ >= 3.3 */
	#if defined(__i386__) || defined(__amd64__) || defined(__x86_64__) /* x86, x86-64 */
		#define PORTABLE_TRAP() asm("int $3\n")
	#else /* Not sure about architecture, just use GCC's builtin trap */
		#define PORTABLE_TRAP() __builtin_trap()
	#endif
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
	#if defined(__sparc) || defined(__sparcv9)
		#define PORTABLE_TRAP() asm("ta 0x05\n")
	#elif defined(__i386) || defined(__amd64)
		#define PORTABLE_TRAP() asm("int $3\n")
	#else
		#error Unknown CPU arch for Solaris
	#endif
#else
	#include <stdlib.h> /* abort() */
	#define PORTABLE_TRAP() abort()
#endif

/****************************************/
/* Macro definitions (The exposed API)	*/
/****************************************/

#ifndef SST_ASSERT_NODEBUG
	/*
	SST_OS_DebugAssert

	In debug mode (_DEBUG defined), it tests if 'cond' is zero, calls the debug handler,
	and traps so that the debugger can intervene. NOTE: when compiling in release mode,
	the ENTIRE expression is removed. Thus, your expression should NEVER have
	side effects.

	Bad:
		SST_OS_DebugAssert( (fp = fopen("file.txt", "rb")) ) //whole line gets removed in release builds!

	Good:
		fp = fopen("file.txt", "rb");
		SST_OS_DebugAssert(fp != NULL, "File is NULL!"); // OK if this line removed, program behavior unchanged.

	@param cond - condition to test. If non-zero, then nothing happens. When zero, a
	              compiler/architecture/OS dependent debug event occurs.
	@param msg - debug message to pass to the handler
	@return (void)
	*/
	#define SST_OS_DebugAssert(cond, msg) do { if(!(cond)) { if(sst_debug_assert_handler != NULL) { if(!sst_debug_assert_handler(msg, sst_debug_assert_handler_arg)) PORTABLE_TRAP(); } else PORTABLE_TRAP();} } while(WHILE_NOP)

	/*
	SST_OS_DebugError

	In debug mode (_DEBUG defined), it calls the debug assert handler and then traps
	so that the debugger can intervene.

	@param msg - debug message to pass to the handler
	@return (void)
	*/
	#define SST_OS_DebugError(msg) do { if(sst_debug_assert_handler != NULL) { if(!sst_debug_assert_handler(msg, sst_debug_assert_handler_arg)) PORTABLE_TRAP(); } else PORTABLE_TRAP(); } while(WHILE_NOP)

	/*
	SST_OS_DebugBreak

	In debug mode (_DEBUG defined), it traps so that the debugger can intervene.  
	When compiling in release mode, the expression is a no-op.

	@return (void)
	*/
	#define SST_OS_DebugBreak() do { PORTABLE_TRAP(); } while(WHILE_NOP)

#else /* release mode */
	#define SST_OS_DebugAssert(cond, msg)
	#define SST_OS_DebugBreak()
	#define SST_OS_DebugError(msg)
#endif

	/*
	SST_OS_RuntimeAssert

	Checks the value of 'cond', and if zero, raises a runtime error with potentially useful
	information for debugging it.

	@param cond - the conditional
	@param msg - the error message to display
	@return (void)
	*/
#ifdef SST_ASSERT_NOINFO
	#define SST_OS_RuntimeAssert(cond, msg)		SST_OS_RuntimeAssertImplNoInfo(cond, msg)
#else /* we want the info */
	#define SST_OS_RuntimeAssert(cond, msg)		SST_OS_RuntimeAssertImpl(cond, #cond, msg, __FILE__, SST_FUNCNAME, __LINE__)
#endif

	/*
	SST_OS_RuntimeError

	Raises a runtime error with potentially useful information for debugging.

	@param msg - the error message to display
	@return (void)
	*/
#ifdef SST_ASSERT_NOINFO
	#define SST_OS_RuntimeError(msg)			SST_OS_RuntimeAssertImplNoInfo(0, msg)
#else /* give us the info */
	#define SST_OS_RuntimeError(msg)			SST_OS_RuntimeAssertImpl(0, "", msg, __FILE__, SST_FUNCNAME, __LINE__)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/* Begin non-public stuff */
	extern SST_AssertHandlerFunc sst_debug_assert_handler; 
	extern void* sst_debug_assert_handler_arg;
	extern SST_AssertHandlerFunc sst_runtime_assert_handler;
	extern void* sst_runtime_assert_handler_arg;

	void SST_OS_RuntimeAssertImpl(int condition, const char* condString, const char* msgString, const char* file, const char* func, int line);
	void SST_OS_RuntimeAssertImplNoInfo(int cond, const char* msgString);
	/* End non-public symbols */
	
	/*************************************************************************/
	/* Handler function setters.  Use these functions to set a 'handler' function
	that will be called when a debug assert fails or a runtime assert fails.	  */
	/*************************************************************************/

	/*
	SST_OS_SetDebugHandlerFunc
	
	Sets the handler function for a failed debug assert.  This function is called when
	a debug assert fails before the breakpoint is triggered.  If the handler returns 0,
	then the breakpoint is triggered, otherwise, it is ignored. If no handler has ever been
	installed or it is set to NULL, then the breakpoint is triggered.
	
	@param _func - the function to call
	@param _arg - the argument to pass to the handler function
	@return (void)
	*/
	void SST_OS_SetDebugAssertHandler(SST_AssertHandlerFunc _func, void *_arg);

	/*
	SST_OS_SetRuntimeAssertHandler
	
	Sets the handler function for a failed runtime assert.  This function is called when
	a runtime assert fails before the runtime error is raised. If the handler returns 0, then
	the runtime error is raised. If no handler has ever been installed or it is set to NULL,
	then the runtime error is raised, otherwise, it is ignored.  If no handler has ever been
	installed or it is set to NULL, then the runtime error is raised.
	
	@param _func - the function to call
	@param _arg - the argument to pass to the handler function
	@return (void)
	*/
	void SST_OS_SetRuntimeAssertHandler(SST_AssertHandlerFunc _func, void *_arg);

#ifdef __cplusplus
}
#endif

#endif

