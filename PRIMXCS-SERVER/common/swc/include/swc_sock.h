/*****************************************************************************
 *                                                                           *
 * File Name : swc_sock.h                                                    *
 * Theme     : Berkeley socket class                                         *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 11. 24                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_sock.h
/// @author Park, sung soo
/// @date 2009-11-24
/// @brief Berkeley socket class

#include <swc_base.h>

#ifndef __SWC_SOCK_H__
#define __SWC_SOCK_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

/// socket object class
class WCSock
{
public :
	SOCKET   Listen          (IN USHORT      nport, 
                              IN INT         option, 
                              IN const CHAR* strip);
	INT      Read            (IN SOCKET      sfd, 
                              IN CHAR*       buf, 
                              IN INT         len, 
                              IN INT         timeout);
	INT      Read            (IN CHAR*       buf, 
                              IN INT         len, 
                              IN INT         timeout);
	INT      Receive         (IN SOCKET      sfd, 
                              OUT CHAR*      buf, 
                              IN INT         len, 
                              IN INT         timeout);
	INT      Receive         (OUT CHAR*      buf, 
                              IN INT         len, 
                              IN INT         timeout);
	INT      Write           (IN SOCKET      sfd, 
                              IN CHAR*       buf, 
                              IN INT         len, 
                              IN INT         timeout);
	INT      Write           (IN CHAR*       buf, 
                              IN INT         len, 
                              IN INT         timeout);
	INT      Send            (IN SOCKET      sfd, 
                              IN CHAR*       buf, 
                              IN INT         len, 
                              IN INT         timeout);
	INT      Send            (IN CHAR*       buf, 
                              IN INT         len, 
                              IN INT         timeout);
	SOCKET   Connect         (IN const CHAR* dstaddr, 
                              IN USHORT      dstport);
	SOCKET   ConnectEx       (IN const CHAR* dstaddr, 
                              IN USHORT      dstport, 
                              IN const CHAR* bndaddr, 
                              IN UINT        ltimeout);
	VOID     Close           (IN SOCKET      sfd);
	VOID     CloseNoForce    (IN SOCKET      sfd);
	VOID     Close           ();
	VOID     CloseNoFprce    ();
	VOID     SetNonBlockMode (IN SOCKET      sfd);
	VOID     SetNonBlockMode ();
	VOID     SetBlockMode    (IN SOCKET      sfd);
	SWC_BOOL IsConnected     ();
	SWC_BOOL IsNonBlockMode  () { return m_bNonBlockMode; };
	SWC_BOOL IsListenSocket  () { return m_bListenSock; };
	SOCKET   GetSockDes      () { return m_SockFd; };
	SOCKET   GetListenSockDes() { return m_ListenFd; };
	INT      GetLastError    () { return m_nErrNo; };

	WCSock();
	virtual ~WCSock();

public :
	SOCKET   m_SockFd;
	SOCKET   m_ListenFd;

private :
	SWC_BOOL m_bNonBlockMode;
	SWC_BOOL m_bListenSock;
	INT      m_nErrNo;
};

//-----------------------------------------------------------------------------
#endif // __SWC_SOCK_H__
 
