/*****************************************************************************
 *                                                                           *
 * File Name : swc_udsock.h                                                  *
 * Theme     : unix domain socket class                                      *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 11. 23                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_udsock.h
/// @author Park, sung soo
/// @date 2009-11-23
/// @brief unix domain socket class

#include <swc_base.h>

#ifndef __SWC_UDSOCK_H__
#define __SWC_UDSOCK_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
/// UNIX domain socket object class
class WCUDSock
{
public :
	VOID   SetIOFile      (IN const CHAR* siofile);
	CHAR*  GetIOFile      () { return m_sIoFile; };
	SOCKET GetConnectSfd  () { return m_ConnectSfd; };
	SOCKET GetListenSfd   () { return m_ListenSfd; };
	VOID   Destroy        ();
	SOCKET Listen         ();
	SOCKET Connect        ();
	VOID   CloseConnectSfd();

	static VOID Destroy        (IN const CHAR* str_file) { sock_destory_uds(str_file); };
	static VOID Close          (IN SOCKET nsfd);
	static INT  Send           (IN SOCKET nsfd, IN VOID* pbuf, IN INT nlen, IN INT ntmout);
	static INT  Recv           (IN SOCKET nsfd, IN VOID* pbuf, IN INT nlen, IN INT ntmout);
	static VOID SetNonBlockMode(IN SOCKET nsfd);

	WCUDSock();
	WCUDSock(IN const CHAR* siofile);
	virtual ~WCUDSock();

private :
    CHAR   m_sIoFile[SWC_MAX_PATH_LEN]; // unix socket pipe file path
    SOCKET m_ConnectSfd;                // connect() return socket descriptor
    SOCKET m_ListenSfd;                 // listen() return socket descriptor

private :
	static VOID sock_close      (IN INT fd);
	static VOID sock_destory_uds(IN const CHAR* str_file);
	static INT  sock_listen_uds (IN const CHAR* str_file);
	static INT  sock_connect_uds(IN const CHAR* str_file);
	static INT  sock_write      (IN INT sfd, IN CHAR *buf, IN INT len, IN INT timeout);
	static INT  sock_read       (IN INT sfd, OUT CHAR *buf, IN INT len, IN INT timeout);
};

//-----------------------------------------------------------------------------
#endif // __SWC_UDSOCK_H__
 
