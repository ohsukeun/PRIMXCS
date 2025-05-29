/*****************************************************************************
 *                                                                           *
 * File Name : swc_base.h                                                    *
 * Theme     : std wrapper library                                           *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 10. 14                     *
 *             2. modified by Park, sung soo '12. 11. 28                     *
 *                SOCKET, TRUE, FALSE, LONG type redefined                   *
 *             3. modified by Park, sung soo '18. 01. 24                     *
 *                IN / OUT macro define                                      *
 *                                                                           *
 *****************************************************************************/

/// @file swc_base.h
/// @author Park, sung soo
/// @date 2018-01-24
/// @brief std wrapper library

#ifndef __SWC_BASE_H__
#define __SWC_BASE_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

#ifdef __linux__
	#define __SWC_OS_LINUX__
#elif __unix__ 
	#define __SWC_OS_UNIX__
#elif __APPLE__
	#define __SWC_OS_MAC__
#elif (_WIN32 || _WIN64)
	#define __SWC_WINDOWS__
#endif

#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_UNIX__) || defined(__SWC_OS_MAC__)
	#define __SWC_BSD_SERIES__
#endif

#if defined(__SWC_BSD_SERIES__)
	#include <pthread.h>
#elif defined(__SWC_WINDOWS__)
	#include <afxmt.h>
#endif 

// length definitions =========================================================
#ifndef SWC_MAX_PATH_LEN
	#define SWC_MAX_PATH_LEN          1024    // max. path length
#endif
#ifndef SWC_MAX_LINE_BUF_LEN
	#define SWC_MAX_LINE_BUF_LEN      1024    // max. line buffer length
#endif
#ifndef SWC_EXC_MAX_COUNT
	#define SWC_EXC_MAX_COUNT      1000000    // exceptional value for list
#endif
#ifndef SWC_MAX_RECV_BUF_LEN
	#define SWC_MAX_RECV_BUF_LEN 100000000    // max. socket receiving buffer length
#endif
#ifndef SWC_MAX_RW_BUF_LEN
	#define SWC_MAX_RW_BUF_LEN   100000000    // max. socket receiving buffer length
#endif
//=============================================================================

#ifndef SWC_USE_CPU_CORE_NUM
	#define SWC_USE_CPU_CORE_NUM         1    // using cpu core number
#endif

// type definitions ===========================================================
#ifndef IN
	#define IN
#endif
#ifndef OUT
	#define OUT
#endif

// socket related types
#ifndef SOCKET
	typedef int SOCKET;
#endif

#ifndef INVALID_SOCKET
	#define INVALID_SOCKET        -1
#endif

// variable related types
#ifndef uint64_t
	#if defined(__LP64__) || defined(_LP64)
		typedef unsigned long         uint64_t;
	#else
		typedef unsigned long long    uint64_t;
	#endif	
#endif

#ifndef DWORD
	#if defined(__LP64__) || defined(_LP64)
		typedef unsigned int          DWORD;
	#else
		typedef unsigned long         DWORD;
	#endif
#endif

#ifndef PDWORD
	typedef DWORD*                PDWORD;
#endif

#ifndef LPDWORD
	typedef DWORD*                LPDWORD;
#endif

#ifndef DWORD32
	typedef unsigned int          DWORD32;
#endif

#ifndef FLOAT
	typedef float                 FLOAT;
#endif

#ifndef HANDLE
	typedef void*                 HANDLE;
#endif

#ifndef HCALL
	typedef DWORD                 HCALL;
#endif

#ifndef HRESULT
	typedef long                  HRESULT;
#endif

#ifndef WORD
	typedef unsigned short        WORD;
#endif

#ifndef VOID
	typedef void                  VOID;
#endif


//#ifndef PVOID
	typedef void*                 PVOID;
//#endif

#ifndef LPVOID
	typedef void*                 LPVOID;
#endif

#ifndef INT
	typedef int                   INT;
#endif

#ifndef INT8
	typedef signed char           INT8;
#endif

#ifndef INT16
	typedef signed short          INT16;
#endif

#ifndef INT32
	typedef signed int            INT32;
#endif

#ifndef UINT
	typedef unsigned int          UINT;
#endif

#ifndef UINT8
	typedef unsigned char         UINT8;
#endif

#ifndef UINT16
	typedef unsigned short        UINT16;
#endif

#ifndef UINT32
	typedef unsigned int          UINT32;
#endif

#ifndef UINT64
	#if defined(__LP64__) || defined(_LP64)
		typedef unsigned long         UINT64;
	#else
		typedef unsigned long long    UINT64;
	#endif
#endif

#ifndef INT64
	#if defined(__LP64__) || defined(_LP64)
		typedef long              INT64;
	#else
		typedef long long         INT64;
	#endif
#endif

#ifndef LONG_PTR
	typedef INT32                 LONG_PTR;
#endif

#ifndef ULONG_PTR
	typedef UINT32                ULONG_PTR;
#endif

#ifndef SIZE_T
	typedef ULONG_PTR             SIZE_T;
#endif

#ifndef DOUBLE
	typedef double                DOUBLE;
#endif

#ifndef LONG
	#if defined(__LP64__) || defined(_LP64)
		typedef int                   LONG;
	#else
		typedef long                  LONG;
	#endif
#endif

#ifndef LONG32
    typedef signed int            LONG32;
#endif

#ifndef LONG32
	typedef signed int            LONG32;
#endif

#ifndef LONG64
	#if defined(__LP64__) || defined(_LP64)
		typedef long                  LONG64;
	#else
		typedef long long             LONG64;
	#endif
#endif

#ifndef ULONG
	#if defined(__LP64__) || defined(_LP64)
		typedef unsigned int      ULONG;
	#else
		typedef unsigned long     ULONG;
	#endif
#endif 

#ifndef ULONG32
	typedef unsigned int          ULONG32;
#endif

#ifndef ULONG64
	#if defined(__LP64__) || defined(_LP64)
		typedef unsigned long         ULONG64;
	#else
		typedef unsigned long long    ULONG64;
	#endif
#endif

#ifndef SHORT
	typedef short                 SHORT;
#endif

#ifndef USHORT
	typedef unsigned short        USHORT;
#endif

#ifndef CHAR
	typedef char                  CHAR;
#endif

#ifndef PCHAR
	typedef char*                 PCHAR;
#endif

#ifndef UCHAR
	typedef unsigned char         UCHAR;
#endif

#ifndef STRING
	typedef UCHAR*                STRING;
#endif

#ifndef BYTE
	typedef unsigned char         BYTE;
#endif

#ifndef PBYTE
	typedef BYTE*                 PBYTE;
#endif

#ifndef LPBYTE
	typedef BYTE*                 LPBYTE;
#endif

// boolean related types
#ifndef BOOL
	typedef bool                  BOOL;
#endif

#ifndef SWC_BOOL
	typedef unsigned int          SWC_BOOL;
#endif

#ifndef PBOOL
	typedef BOOL*                 PBOOL;
#endif

#ifndef LPBOOL
	typedef BOOL*                 LPBOOL;
#endif

#ifndef BOOLEAN
	typedef BYTE                  BOOLEAN;
#endif

#ifndef PBOOLEAN
	typedef BOOLEAN*              PBOOLEAN;
#endif

#ifndef TRUE
	#define TRUE                  1
#endif

#ifndef FALSE
	#define FALSE                 0
#endif

#ifndef YES
	#define YES                   1
#endif

#ifndef NO
	#define NO                    0
#endif

#ifndef IN
	#define IN
#endif

#ifndef OUT
	#define OUT
#endif

//-----------------------------------------------------------------------------
#endif // __SWC_BASE_H__
