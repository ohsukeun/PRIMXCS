/*****************************************************************************
 *                                                                           *
 * File Name : swc_ssl_handler.h                                             *
 * Theme     : socket handler object using 'epoll' & 'ssl'                   *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by PSS '10. 04. 13                                 *
 *                                                                           *
 *****************************************************************************/

/// @file swc_ssl_handler.h 
/// @author Park, sung soo
/// @date 2010-04-13
/// @brief socket handler object using 'epoll' & 'ssl'

#include <pthread.h>
#include <sched.h>
#include <sys/epoll.h>

#include "swc_ssl_func.h"
#include "swc_array.h"
#include "swc_ptr_queue.h"
#include "swc_ssl_ionode.h"
#include "swc_map_str_to_ptr.h"
#include "swc_sock.h"

#ifndef __SWC_SSL_HANDLER_H__
#define __SWC_SSL_HANDLER_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

#ifndef EPOLLRDHUP
	#define EPOLLRDHUP    0x2000
#endif

#define SOCK_MAX_WORK_TH_CNT                         5
#define SOCK_MAX_JOB_Q_CNT                       10000
#define SOCK_MAX_CLIENT_CNT                        100
#define SOCK_MAX_EPOLL_STRUCT_SIZE                1024
#define SOCK_MAX_EPOLL_FD_CNT                    65000
#define SOCK_MAX_FILE_SEND_BLOCK_SIZE    (1024 * 1024)
#define SOCK_IO_TIMEOUT                            450  // (7 minutes 30 seconds)
#define SOCK_DEFAULT_TIMEOUT                        20  // (20 seconds)

#define SYSTEM_CHECK_MIN_INTERVAL        600
#define SSL_ACCEPT_NKBL_MAX_RETRY_CNT      5
#define SSL_ACCEPT_TIMEOUT                 5

/// I/O job container structure
typedef struct __s_job_ssl_container {
	time_t      _tStamp;   // time stamp
	CSSLIONode* _pJobNode; // job node pointer

} SJobSSLContainer, *pSJobSSLContainer;

// structure for additional environment properties
// SSockProperty.OptFlag : 
#define SWC_SOCK_ENABLE_IO_TIMEOUT (1 << 0)
#define SWC_SYSTEM_CHECK_ENABLE    (1 << 1)

/// I/O option structure
typedef struct __s_sock_property
{
	UINT _OptFlag;    ///< option flag (SWC_SOCK_ENABLE_IO_TIMEOUT / SWC_SYSTEM_CHECK_ENABLE)
	UINT _IOTimeout;  ///< I/O time (seconds). default 0. no timeout. from 1 ~ 3600 (seconds)
	UINT _SysChkItvl; ///< system check interval (seconds).
	
} SSockProperty, *pSSockProperty;

class CSSLWorkTh;

/// I/O handler class (SSL)
class CSSLHandler
{
friend class CSSLWorkTh;

public :
	SWC_BOOL    Init(IN const CHAR* sip, 
                     IN USHORT      nport,
                     IN INT         nlimit, 
                     IN INT         nrcvlen,
                     IN INT         nwhcnt,
                     IN const CHAR* scertfile,
                     IN const CHAR* scalist,
                     IN const CHAR* spasswd);
	VOID        SetProperties(IN pSSockProperty pprt) { memcpy(&m_OptProperty, pprt, sizeof(SSockProperty)); };
	SWC_BOOL    Run ();
	VOID        Exit();

	VOID        SetEnableClientAccept () { m_bAcceptStart = TRUE; };
	VOID        SetDisableClientAccept() { m_bAcceptStart = FALSE; };
	SWC_BOOL    CanAcceptClient       () { return m_bAcceptStart; };
	INT         GetWorkThCount        () { return m_nWorkThCount; };

	VOID        AcceptClient          ();
	VOID        AcceptSSL             (IN CSSLIONode* pobj);
	VOID        PushJobQueue          (IN CSSLIONode* pobj);
	CSSLIONode* PopJobQueue           ();
	VOID        RegisterNode          (IN CSSLIONode* pnode);
	VOID        DetachNode            (IN CSSLIONode* pnode);
	VOID        SetEnableOneShotNode  (IN CSSLIONode* pnode);
	VOID        SetDisableOneShotNode (IN CSSLIONode* pnode);
	VOID        DeleteNodeFromJobQueue(IN CSSLIONode* pnode);
	VOID        WaitJobCompleted      ();
	VOID        RenewalLstEpoll       ();
	VOID        AcceptAndTerminateClient();

	VOID           SetMapIPPtr        (IN WCMapStrToPtr* pptr) { m_pMapIP = pptr; };
	WCMapStrToPtr* GetMapIPPtr        () { return m_pMapIP; };

	SSL_CTX*    InitCTX(IN const CHAR* scertfile, IN const CHAR* scalist, IN const CHAR* spasswd);

	static PVOID HandlerThreadFunc(IN PVOID arg);

	CSSLIONode* FindNode      (IN SOCKET nsfd);
	CSSLIONode* AddNode       (IN SOCKET nsfd);
	VOID        DeleteNode    (IN SOCKET nsfd, SWC_BOOL bAddToDeleteMap = TRUE);
	VOID        DeleteNodeSafe(IN CSSLIONode* pNode);

	VOID        SetHold    () { m_bHold = TRUE; };
	VOID        SetContinue() { m_bHold = FALSE; };
	SWC_BOOL    IsHold     () { return m_bHold; };

	time_t      GetLastUpTime          () { return m_tLastUp; };
	VOID        RunCheckToAllWorkThread();
	VOID        AddToDeleleMap         (IN CSSLIONode* pdel);
	SWC_BOOL    SSLAcceptBlockMode     (IN SSL* ssl);
	SWC_BOOL    SSLAcceptNonBlockMode  (IN SSL* ssl);

	CSSLHandler();
	~CSSLHandler();

private :

	VOID        CheckSessionTimeout();
	VOID        RegAcceptedIP      (IN SOCKET nsfd);
	VOID        UpdateLastUpTime   ();
	CSSLIONode* GetIdleNode        ();

public :

//	CSSLWorkTh*         m_pWorkTh[SOCK_MAX_WORK_TH_CNT]; // work thread pointer array
	WCArray<CSSLWorkTh*> m_pWorkTh;           // work thread pointer array
	WCMapStrToPtr        m_mapPtrIONode;      // io-node object pointer map 
	WCMapStrToPtr        m_mapDelIONode;      // deleting target io-node object pointer map
	UINT                 m_ConnectCnt;        // connected client count
                                              // key : socket descriptor number string

	WCPtrQueue           m_qJob;              // job queue
	INT                  m_nWorkThCount;      // wotk thread count

	SWC_BOOL             m_bRun;              // run or not
	WCString             m_sIP;               // listen IP 
	USHORT               m_nPort;             // listen port
	INT                  m_nLimit;            // client control limit
	INT                  m_nInitRecvLen;      // init. receiving data length

	// session info. related (IP - last I/O time)
	WCMapStrToPtr*       m_pMapIP;

	// event polling object array pointer
	struct epoll_event   m_pArrEPollEvent[SOCK_MAX_EPOLL_STRUCT_SIZE];
	WCSock*              m_pLstSock;           // listen socket object
	struct epoll_event   m_LstEP;              // listen socket event object
	INT                  m_nEPfd;              // epoll file descriptor

	pthread_t            m_ThreadID;           // thread ID
	pthread_mutex_t      m_Lock;               // mutex object for sync. of work threads
	pthread_cond_t       m_Condition;          // condition for work threads

	pthread_t            m_DeleterThreadID;    // deleter thread ID

	CHAR                 m_sCertFile[256];
	CHAR                 m_sCAList[256];
	CHAR                 m_sPasswd[256];

	const SSL_METHOD*    m_pMeth;
	SSL_CTX*             m_pCTX;

    WCString             m_sModuleName;
    UINT                 m_nModuleID;
	SSockProperty        m_OptProperty;   // option property

private :
	UINT                 m_lCheckLoopCnt;
	SWC_BOOL             m_bAcceptStart;
	SWC_BOOL             m_bHold;
	time_t               m_tPrev;
	time_t               m_tLastUp;
	time_t               m_tPrevSysChk;
};

/// I/O work thread class (SSL)
class CSSLWorkTh
{
public :

	SWC_BOOL     Init(IN CSSLHandler* pparent);
	SWC_BOOL     Run ();
	SWC_BOOL     Exit();

	pthread_t    GetTid      ()                        { return m_ThreadID; };
	VOID         SetTid      (pthread_t tid)           { m_ThreadID = tid; };

	VOID         SetParentPtr(IN CSSLHandler* pparent) { m_pParent = pparent; };
	CSSLHandler* GetParentPtr()                        { return m_pParent; };

	VOID         SetSendMode (IN CSSLIONode* pobj, IN VOID* pbuf, IN INT nlen) 
	{
		if(NULL == pobj) return;
		pobj->SetSendMode(pbuf, nlen);
		m_pParent->PushJobQueue(pobj);
	}

	VOID         SetID(IN INT nid) { m_nID = nid; };
	INT          GetID()           { return m_nID; };

	VOID         SetBusy() { m_bIsBusy = TRUE; };
	VOID         SetIdle() { m_bIsBusy = FALSE; };
	SWC_BOOL     IsBusy()  { return m_bIsBusy; };

	virtual VOID RunJob  (IN CSSLIONode* pnode) {};
	virtual VOID RunCheck(IN UINT nInterval)    {};

	CSSLWorkTh();
	virtual ~CSSLWorkTh();

public :

	SWC_BOOL m_bRunCheck;

private :

	static PVOID WorkThreadFunc(IN PVOID arg);

private :

	CSSLHandler*    m_pParent;     // CSSLHandler object pointer
	SWC_BOOL        m_bRun;        // run or not
	pthread_t       m_ThreadID;    // thread ID
	INT             m_nID;         // thread array index
	SWC_BOOL        m_bIsBusy;
};

//-----------------------------------------------------------------------------
#endif // __SWC_SSL_HANDLER_H__
