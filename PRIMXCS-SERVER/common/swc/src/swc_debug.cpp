/*****************************************************************************
 *                                                                           *
 * File Name : swc_debug.cpp                                                 *
 * Theme     : debugging messaging & etc ...                                 *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 11. 24                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_debug.cpp
/// @author Park, sung soo
/// @date 2009-11-24
/// @brief debugging messaging & etc

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <ctime>
#include <unistd.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <swc_debug.h>
#include <swc_tool.h>

#ifndef __SWC_DEBUG_CPP__
#define __SWC_DEBUG_CPP__

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// GLOVAL DEFINITIONS
//-----------------------------------------------------------------------------
CHAR  SWC_G_LOG_FILE_PATH[256];
FILE* SWC_G_LOG_FILE            = NULL;
UINT  SWC_G_LOG_FILE_SIZE_LIMIT = (10 * 1024 * 1024);
unsigned int  SWC_G_MODE_DEBUG          = FALSE;
unsigned int  SWC_G_MODE_MSG_ON         = TRUE;

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------
unsigned int OpenDebugFile  (const CHAR* spath, UINT nlimit);
VOID         CloseDebugFile ();
VOID         DMsg           (INT option, const CHAR* comments, ...);
VOID         SockErrMsg     (INT errnum);
VOID         SetDebugModeOn () { SWC_G_MODE_DEBUG  = TRUE; }
VOID         SetDebugModeOff() { SWC_G_MODE_DEBUG  = FALSE; }
VOID         SetMsgOn       () { SWC_G_MODE_MSG_ON = TRUE; }
VOID         SetMsgOff      () { SWC_G_MODE_MSG_ON = FALSE; }
unsigned int IsDMsgOn       (void) { return SWC_G_MODE_MSG_ON; }

static INT _is_file_size_over(const CHAR* sparh, UINT lsize);

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------
/* ----------------------------------------------------------------------------
    description : open debug file
    parameters  :
		- (const CHAR*) debug file path 
		- (u_int)       debug file max. file size limit (from 1 to 100 mega bytes)
    return      : (unsigned int return result
    history     :
        - 2010. 01. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
unsigned int OpenDebugFile(const CHAR* spath, UINT nlimit)
{
	if(NULL == spath) return FALSE;
	if(NULL == (SWC_G_LOG_FILE = fopen(spath, "a+"))) return FALSE;
	if((0 == nlimit) || ((100 * 1024 * 1024) < nlimit)) nlimit = (10 * 1024 * 1024);
	SWC_G_LOG_FILE_SIZE_LIMIT = nlimit;
	snprintf(SWC_G_LOG_FILE_PATH, 256, "%s", spath);
	return TRUE;
}

/* ----------------------------------------------------------------------------
    description : close debug file
    parameters  :
		- (const CHAR*) debug file path 
    return      : (unsigned int return result
    history     :
        - 2010. 01. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CloseDebugFile()
{
	if(NULL != SWC_G_LOG_FILE) 
	{
		fflush(SWC_G_LOG_FILE);
		fclose(SWC_G_LOG_FILE);
		SWC_G_LOG_FILE = NULL;
	}
}

/* ----------------------------------------------------------------------------
    description : debug messaging function
    parameters  :
        - (int)         output option
		- (const CHAR*) comments, ...
    return      : ()
    history     :
        - 2009. 11. 24 created by Park. Sung soo
		- 2012. 11. 26 modified by Park, Sung soo
			DebugMsg() -> DMsg()
---------------------------------------------------------------------------- */
VOID DMsg(INT option, const CHAR* comments, ...)
{
/*
	va_list      ap;
	struct timeb curtm;
	struct tm    time_ptr;
	CHAR         msg_time[30] = {0}, shead[10] = {0};

	ftime(&curtm);
	memcpy(&time_ptr, localtime(&curtm.time), sizeof(struct tm));
	strftime(msg_time, 30, "%H:%M:%S", &time_ptr);
*/
	va_list ap;
	time_t rawtime;
	struct tm time_ptr;
	struct timespec ts;
	int    nMilliSec = 0;
	CHAR   msg_time[30] = {0}, shead[10] = {0};

	time(&rawtime);
	memcpy(&time_ptr, localtime(&rawtime), sizeof(struct tm));
	strftime(msg_time, 30, "%H:%M:%S", &time_ptr);

	clock_gettime(CLOCK_MONOTONIC, &ts);
	nMilliSec = (int)(ts.tv_nsec / 1000000);

//	if(!(DMSG_W & option)) option |= DMSG_W;
	if(TRUE != SWC_G_MODE_MSG_ON) 
	{
		if(DMSG_M & option) option ^= DMSG_M;
		if(DMSG_E & option) option ^= DMSG_E;
		if(DMSG_P & option) option ^= DMSG_P;
	}
	if(DMSG_W & option)
	{
		if(NULL != SWC_G_LOG_FILE)
		{
			INT ncheck = _is_file_size_over(SWC_G_LOG_FILE_PATH, SWC_G_LOG_FILE_SIZE_LIMIT);
			if(1 == ncheck)
			{
				fclose(SWC_G_LOG_FILE);
				unlink(SWC_G_LOG_FILE_PATH);
				SWC::swc_sleep(0, 10000);
				SWC_G_LOG_FILE = NULL;
				if(NULL == (SWC_G_LOG_FILE = fopen(SWC_G_LOG_FILE_PATH, "a+"))) SWC_G_LOG_FILE = NULL;
			}
			else if(-1 == ncheck)
			{
				if(NULL == (SWC_G_LOG_FILE = fopen(SWC_G_LOG_FILE_PATH, "a+"))) SWC_G_LOG_FILE = NULL;
			}
		}
		else
		{
			if(NULL == (SWC_G_LOG_FILE = fopen(SWC_G_LOG_FILE_PATH, "a+"))) SWC_G_LOG_FILE = NULL;
		}
	}
	if(DMSG_M & option) snprintf(shead, 10, "msg");
	if(DMSG_E & option) snprintf(shead, 10, "err");

	if((DMSG_S & option) && (TRUE == SWC_G_MODE_DEBUG))
		fprintf(stdout, "[%s.%03d] %s : ", msg_time, nMilliSec, shead);
	if(DMSG_W & option)
	{
		if(NULL != SWC_G_LOG_FILE)
		{
			strftime(msg_time, 30, "%Y/%m/%d %H:%M:%S", &time_ptr);
			fprintf(SWC_G_LOG_FILE, "[%s.%03d] %s : ", msg_time, nMilliSec, shead);
		}
	}
	if((DMSG_S & option) && (TRUE == SWC_G_MODE_DEBUG)) 
	{
		va_start(ap, comments);
		vprintf(comments, ap);
		va_end(ap);
	}
	if(DMSG_W & option) 
	{
		va_start(ap, comments);
		if(NULL != SWC_G_LOG_FILE) vfprintf(SWC_G_LOG_FILE, comments, ap);
		va_end(ap);
	}
	if(DMSG_W & option)
	{
		if(NULL != SWC_G_LOG_FILE) fflush(SWC_G_LOG_FILE);
	}
	if((DMSG_P & option) && (TRUE == SWC_G_MODE_DEBUG)) perror("|=>");
}

/* ----------------------------------------------------------------------------
    description : socket error messaging function
    parameters  :
		- (int) error number
    return      : ()
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID SockErrMsg(INT errnum)
{
	if(TRUE != SWC_G_MODE_MSG_ON) return;
	switch(errnum)
	{
		case D_SOCK_ECREATESOCK :
#ifndef NDEBUG
			fprintf(stdout, "Can't Create Listen Sock on port (%s)\n", strerror(errno));
#endif
			break;
		case D_SOCK_ESETSOCKOPT :
#ifndef NDEBUG
			fprintf(stdout, "Can't Set Listen Sock option on port (%s)\n", strerror(errno));
#endif
			break;
		case D_SOCK_EBIND :    
#ifndef NDEBUG
			fprintf(stdout, "Can't Bind Listen Sock on port (%s)\n", strerror(errno));
#endif
			break;
		case D_SOCK_ELISTEN :
#ifndef NDEBUG
			fprintf(stdout, "Can't Listen Listen Sock on port (%s)\n", strerror(errno));
#endif
			break;
		case D_SOCK_ECONN :
#ifndef NDEBUG
			fprintf(stdout, "Can't Connect to Manager on port (%s)\n", strerror(errno));
#endif
			break;
		default :
#ifndef NDEBUG
			fprintf(stdout, "Unknown socket error occured\n");
#endif
			break;
	}
}

/* ----------------------------------------------------------------------------
	description : check file size given file size limit
	parameters  :
		- (const CHAR*) file path
		- (UINT)       file size limit
	return      : (int) return result
				-1 -> error coorued. just reopen
				0  -> less than limit
				1  -> over than limit
	history     :
		- 2007. 07. 01 created by Park. Sung soo
		- 2011. 12. 05 modified by Park, Sung soo
---------------------------------------------------------------------------- */
INT _is_file_size_over(const CHAR* spath, UINT lsize)
{
	INT nret = 0;
	struct stat st;

	if(NULL == spath) return -1;
	memset(&st, 0x00, sizeof(struct stat));
	if(0 != stat(spath, &st))
	{
		if((EBADF == errno) || (EFAULT == errno) || (ENOENT == errno)) return -1;
	}
	if(lsize < (UINT)st.st_size) nret = 1;	
	else if(0 == (UINT)st.st_size) nret = -1;
	else nret = 0;
	return nret;
}

//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif // __SWC_DEBUG_CPP__
