/*****************************************************************************
 *                                                                           *
 * File Name : swc_ionode.h                                                  *
 * Theme     : recv / write object definitions                               *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '12. 11. 22                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_ionode.h
/// @author Park, sung soo
/// @date 2012-11-22
/// @brief recv / write object definitions

#include <pthread.h>
#include <stdio.h>

#include <swc_base.h>
#include "swc_string.h"

#ifndef __SWC_IONODE_H__
#define __SWC_IONODE_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

#define SOCK_IO_NODE_KEY_LEN          50
#define SOCK_RECV_TIMEOUT             20

#define SOCK_MAX_RECV_LEN      104857600 // (100 X 1024 X 1024 = 100 M bytes)

#define SOCK_IO_ERR_INVALID_SFD   -20000
#define SOCK_IO_ERR_INVALID_ARG   -20001
#define SOCK_IO_ERR_SEND_RUNNING  -20101
#define SOCK_IO_ERR_RECV_RUNNING  -20102

/// I/O node class (NO SSL)
class CIONode 
{
public :
	// socket I/O related
	VOID     Close              ();
	VOID     Reset              ();
	VOID     ResetRecv          ();
	VOID     ResetSend          ();

	VOID     SetFd              (IN SOCKET nsfd);
	SOCKET   GetFd              ();

	VOID     SetRecvLen         (IN INT nlen) { m_nRecvLen = nlen; };
	INT      GetRecvLen         () const { return m_nRecvLen; };
	VOID     SetSendLen         (IN INT nlen) { m_nSendLen = nlen; };
	INT      GetSendLen         () const { return m_nSendLen; };

	SWC_BOOL SetRecvMode        (IN INT nlen);
	SWC_BOOL IsRecvMode         ();
	SWC_BOOL SetSendMode        (IN VOID* pbuf, IN INT nlen);
	SWC_BOOL IsSendMode         ();

	VOID     SetRecvPtr         (IN CHAR* pbuf);
	CHAR*    GetRecvPtr         ();
	VOID     SetSendPtr         (IN CHAR* pbuf);
	CHAR*    GetSendPtr         ();

	VOID     SetRecvPos         (IN INT npos);
	INT      GetRecvPos         ();
	VOID     SetSendPos         (IN INT npos);
	INT      GetSendPos         ();

	VOID     SetRecvPosAdd      (IN INT npos);
	VOID     SetSendPosAdd      (IN INT npos);

	VOID     SetBusy            ();
	VOID     SetIdle            (); 
	SWC_BOOL IsBusy             (); 
	SWC_BOOL IsIdle             (); 

    VOID     StartJob           (); 
	VOID     CompleteJob        (); 
	SWC_BOOL HasJob             (); 

	VOID     SetConnectTime     () { time  (&m_tConnect); };
	VOID     SetConnectTime     (IN time_t tconn){ m_tConnect = tconn; };
	time_t   GetConnectTime     () const  { return m_tConnect; };

	SWC_BOOL SetTempBufAlloc    (IN INT nlen);
	CHAR*    GetTempBuf         () const  { return m_pTempBuf; };
	SWC_BOOL InitAsRecv         (IN INT nlen);

	CHAR*    CopyTempBuf        ();
	CHAR*    CopyRecvBuf        ();

	VOID     SetHeadRcvd        ();
	VOID     SetHeadEmpty       ();
	SWC_BOOL IsHeadRcvd         ();

	time_t   GetTimeLastIO      () { return m_tLastIO; };
	VOID     UpdateTimeLastIO   () { time  (&m_tLastIO); };
	VOID     SetTimeZeroLastIO  () { m_tLastIO = 0; };

    VOID     ResetBuffer        ();
	VOID     InitForIdleMode    ();
	SWC_BOOL CheckRecvTimeOut   ();

	INT      Write              (IN VOID* buf, IN INT len);
	INT      Write              (IN VOID* buf, IN INT len, IN INT timeout, IN SWC_BOOL bFlexTimeOut = FALSE);
	INT      Read               (OUT VOID* buf, IN INT len);
	INT      Read               (OUT VOID* buf, IN INT len, IN INT timeout, IN SWC_BOOL bFlexTimeOut = FALSE);

	// attached value related
	VOID     SetLID             (IN UINT   lid)        { m_lID = lid; };
	UINT     GetLID             () const               { return m_lID; };
	VOID     SetLLID            (IN UINT64 llid)       { m_llID = llid; };
	UINT64   GetLLID            () const               { return m_llID; };
	VOID     SetIP              (IN UINT   lip)        { m_lIP = lip; };
	UINT     GetIP              () const               { return m_lIP; };
	VOID     SetCMAC            (IN UCHAR  cmac[])     { memcpy  (m_cMAC, cmac, 6); };
	VOID     SetSMAC            (IN const  CHAR* smac) { snprintf  (m_sMAC, 13, "%s", smac); };
	VOID     SetKey             (IN const  CHAR* skey) { snprintf  (m_sKey, SOCK_IO_NODE_KEY_LEN, "%s", skey); };
	VOID     SetPtr             (IN VOID*  pptr)       { m_pPtr = pptr; };
	VOID*    GetPtr             () const               { return m_pPtr; };
	VOID     SetUserValue       (IN UINT   lval)       { m_lUserVal = lval; };
	UINT     GetUserValue       ()                     { return m_lUserVal; };

	// file I/O related
	SWC_BOOL SetFileSendMode    (IN const CHAR* spath, IN UINT nbsize);
	VOID     ClrFileSendMode    ();
	SWC_BOOL SetFileSendNext    ();

	SWC_BOOL IsFileSendMode     () { return m_bIsFileSendMode; };
	FILE*    GetFDesc           ();
	UINT     GetFTotalSize      () { return m_nFTotalSize; };
	UINT     GetFTotalBlock     () { return m_nFTotalBlock; };
	UINT     GetFBlockSize      () { return m_nFBlockSize; };
	UINT     GetFRestSize       () { return m_nFRestSize; };
	UINT     GetFSentBytes      ();
	UINT     GetSendingBytes    ();
	SWC_BOOL WaitSendingComplete(IN INT nsfd);

	VOID     Lock               () { pthread_mutex_lock(&m_Lock); };
	VOID     Unlock             () { pthread_mutex_unlock(&m_Lock); };
	SWC_BOOL IsLocked           ()
	{
		if(1 <= m_Lock.__data.__lock) return TRUE;
		return FALSE;
	}

	// xml buffer related
	WCString GetXmlBuf          ();
	VOID     SetXmlBuf          (IN WCString sxml);
	VOID     SetXmlBuf          (IN std::string sxml);
	void     SetXmlBuf          (IN const CHAR* sxml);
	VOID     ClearXmlBuf        () { m_pXmlBuf.Empty(); };

	VOID     Init               ();

	CIONode();
	CIONode(SOCKET nsfd);
	CIONode(SOCKET nsfd, IN UINT nbuflen);
	virtual ~CIONode();

public :
	UINT            m_lID;
	UINT64          m_llID;
	UINT            m_lIP;
	UCHAR           m_cMAC[6];
	CHAR            m_sMAC[13];                   // AABBCCDDEEFF
	CHAR            m_sKey[SOCK_IO_NODE_KEY_LEN]; // unique key
	VOID*           m_pPtr;
	UINT            m_lUserVal;

private :
	INT             m_nRecvLen;
	CHAR*           m_pRecv;
	CHAR*           m_pRecvBack;
	INT             m_nRecvPos;
	size_t          m_nRecvAllocLen;
	SWC_BOOL        m_bRecvMode;

	INT             m_nSendLen;
	CHAR*           m_pSend;
	CHAR*           m_pSendBack;
	INT             m_nSendPos;
	size_t          m_nSendAllocLen;
	SWC_BOOL        m_bSendMode;

	CHAR*           m_pTempBuf; 
	INT             m_nTempBufLen;
	SWC_BOOL        m_bHeadRcvd;

	time_t          m_tConnect;
	time_t          m_tLastIO;
	SWC_BOOL        m_bWorking;
	SWC_BOOL        m_bHasJob;

	SOCKET          m_nSfd;

	SWC_BOOL        m_bIsFileSendMode;
	FILE*           m_pFile;
	UINT            m_nFTotalSize;
	UINT            m_nFTotalBlock; 
	UINT            m_nFBlockSize;
	UINT            m_nFRestSize;
	UINT            m_nFSentBytes;
	UINT            m_nFSendingBytes;
	CHAR*           m_pFBuf;

	WCString        m_pXmlBuf;

	INT             m_nRecvTimeOut;
	INT             m_nRecvTimeSpend;
	time_t          m_tRecvPrev;

	pthread_mutex_t m_Lock;
};

//-----------------------------------------------------------------------------
#endif // __SWC_IONODE_H__
