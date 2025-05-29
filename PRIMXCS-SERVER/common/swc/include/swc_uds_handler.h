/*****************************************************************************
 *                                                                           *
 * File Name : swc_uds_handler.h                                             *
 * Theme     : unix domain socket handler object using 'epoll'               *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by PSS '10. 04. 13                                 *
 *                                                                           *
 *****************************************************************************/

/// @file swc_uds_handler.h 
/// @author Park, sung soo
/// @date 2010-04-13
/// @brief unix domain socket handler object using 'epoll' 

#include <stdlib.h>
#include <pthread.h>
#include <sys/epoll.h>

#include <swc_array.h>
#include <swc_ptr_queue.h>
#include <swc_ionode.h>
#include <swc_map_str_to_ptr.h>
#include <swc_sock.h>
#include <swc_udsock.h>
#include <swc_ip_obj.h>

#ifndef __SWC_UDS_HANDLER_H__
#define __SWC_UDS_HANDLER_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

#ifndef EPOLLRDHUP
	#define EPOLLRDHUP    0x2000
#endif

#define SOCK_MIN_WORK_TH_CNT                      5
#define SOCK_MAX_JOB_Q_CNT                    10000
#define SOCK_MAX_CLIENT_CNT                     200
#define SOCK_MAX_EPOLL_STRUCT_SIZE             1024
#define SOCK_MAX_EPOLL_FD_CNT                 65000
#define SOCK_MAX_FILE_SEND_BLOCK_SIZE (1024 * 1024)
#define SOCK_IO_TIMEOUT                         330  // (5 minutes 30 seconds)

#define SYSTEM_CHECK_MIN_INTERVAL        600

/// I/O job container structure (UNIX domain socket)
typedef struct __s_job_container {
	time_t   _tStamp;   // time stamp
	CIONode* _pJobNode; // job node pointer

} SJobContainer, *pSJobContainer;

// structure for additional environment properties
// SSockProperty.OptFlag : 
#define SWC_SOCK_ENABLE_IO_TIMEOUT (1 << 0)
#define SWC_SYSTEM_CHECK_ENABLE    (1 << 1)

/// I/O socket property structure (UNIX domain socket)
typedef struct __s_sock_property
{
	UINT _OptFlag;    // option flag (SWC_SOCK_ENABLE_IO_TIMEOUT / SWC_SYSTEM_CHECK_ENABLE)
	UINT _IOTimeout;  // I/O time (seconds). default 0. no timeout. from 1 ~ 3600 (srconds)
	UINT _SysChkItvl; // system check interval (seconds). min 600

} SSockProperty, *pSSockProperty;

class CWorkTh;

/// UNIX domain socket I/O handler class 
class CUDSHandler
{
friend class CWorkTh;

public :
	SWC_BOOL Init(IN const CHAR* sio, 
                  IN INT         nlimit, 
                  IN INT         nrcvlen,
                  IN INT         nwhcnt);
	VOID     SetProperties(IN pSSockProperty pprt) { memcpy(&m_OptProperty, pprt, sizeof(SSockProperty)); };
	SWC_BOOL Run ();
	VOID     Exit();

	VOID     SetEnableClientAccept () { m_bAcceptStart = TRUE; };
	VOID     SetDisableClientAccept() { m_bAcceptStart = FALSE; };
	SWC_BOOL CanAcceptClient       () { return m_bAcceptStart; };
	INT      GetWorkThCount        () { return m_nWorkThCount; };

	VOID     AcceptClient          ();
	VOID     PushJobQueue          (IN CIONode* pobj);
	CIONode* PopJobQueue           ();
	VOID     RegisterNode          (IN CIONode* pnode);
	VOID     DetachNode            (IN CIONode* pnode);
	VOID     SetEnableOneShotNode  (IN CIONode* pnode);
	VOID     SetDisableOneShotNode (IN CIONode* pnode);
	VOID     DeleteNodeFromJobQueue(IN CIONode* pnode);
	VOID     WaitJobCompleted      ();
	VOID     RenewalLstEpoll       ();
	VOID     AcceptAndTerminateClient();
	VOID     AddToDeleleMap        (IN CIONode* pdel);
	VOID     CheckAndDeleteIONode  ();

	VOID           SetMapIPPtr(IN WCMapStrToPtr* pptr) { m_pMapIP = pptr; };
	WCMapStrToPtr* GetMapIPPtr() { return m_pMapIP; };

	CIONode* FindNode  (IN SOCKET nsfd);
	CIONode* AddNode   (IN SOCKET nsfd);
	VOID     DeleteNode(IN SOCKET nsfd, IN SWC_BOOL bAddToDeleteMap = TRUE);

	VOID     SetHold    () { m_bHold = TRUE; };
	VOID     SetContinue() { m_bHold = FALSE; };
	SWC_BOOL IsHold     () { return m_bHold; };

	SWC_BOOL GetLastUpTime          () { return m_tLastUp; };
	VOID     RunCheckToAllWorkThread();

	static PVOID HandlerThreadFunc(IN PVOID arg);
	static PVOID DeleterThreadFunc(IN PVOID arg);

	CUDSHandler();
	~CUDSHandler();

private :
	VOID     CheckSessionTimeout();
	VOID     RegAcceptedIP      (IN SOCKET nsfd);
	VOID     UpdateLastUpTime   ();
	CIONode* GetIdleNode        ();

public : 
	WCArray<CWorkTh*>   m_pWorkTh;           // work thread pointer array
	WCMapStrToPtr       m_mapPtrIONode;      // io-node object pointer map 
	WCMapStrToPtr       m_mapDelIONode;      // deleting target io-node object pointer map
	UINT                m_ConnectCnt;        // connected client count
                                             // key : socket descriptor number string
	WCPtrQueue          m_qJob;              // job queue
	INT                 m_nWorkThCount;      // wotk thread count

	SWC_BOOL            m_bRun;              // run or not
	WCString            m_sIO;               // unix domain socket I/O path
	INT                 m_nLimit;            // client control limit
	INT                 m_nInitRecvLen;      // init. receiving data length

	// session info. related (IP - last I/O time)
	WCMapStrToPtr*      m_pMapIP;

	// event polling object array pointer
	struct epoll_event  m_pArrEPollEvent[SOCK_MAX_EPOLL_STRUCT_SIZE];
	WCUDSock*           m_pLstSock;           // listen socket object
	struct epoll_event  m_LstEP;              // listen socket event object
	INT                 m_nEPfd;              // epoll file descriptor

	pthread_t           m_ThreadID;           // thread ID
	pthread_t           m_DeleterThreadID;    // object delete thread ID
	pthread_mutex_t     m_Lock;               // mutex object for sync. of work threads
	pthread_cond_t      m_Condition;          // condition for work threads

	WCString            m_sModuleName;
	UINT                m_nModuleID;
	SSockProperty       m_OptProperty;        // option property

private :
	UINT                m_lCheckLoopCnt;
	SWC_BOOL            m_bAcceptStart;
	SWC_BOOL            m_bHold;
	time_t              m_tPrev;
	time_t              m_tLastUp;
	time_t              m_tPrevSysChk;
};

/// UNIX domain socket I/O work thread class
class CWorkTh
{
public :
	SWC_BOOL     Init(IN CUDSHandler* pparent);
	SWC_BOOL     Run ();
	SWC_BOOL     Exit();

	pthread_t    GetTid      ()                        { return m_ThreadID; };
	VOID         SetTid      (IN pthread_t tid)        { m_ThreadID = tid; };

	VOID         SetParentPtr(IN CUDSHandler* pparent) { m_pParent = pparent; };
	CUDSHandler* GetParentPtr()                        { return m_pParent; };

	SWC_BOOL     SetSendMode (IN CIONode* pobj, IN VOID* pbuf, IN INT nlen) 
	{
		if(NULL == pobj) return FALSE;
		if(FALSE == pobj->SetSendMode(pbuf, nlen)) return FALSE;
		m_pParent->PushJobQueue(pobj);
		return TRUE;
	}

	VOID         SetID(IN INT nid) { m_nID = nid; };
	INT          GetID()           { return m_nID; };

	VOID         SetBusy() { m_bIsBusy = TRUE; };
	VOID         SetIdle() { m_bIsBusy = FALSE; };
	SWC_BOOL     IsBusy()  { return m_bIsBusy; };

	virtual VOID RunJob  (IN CIONode* pnode) {};
	virtual VOID RunCheck(IN UINT nInterval) {};

	CWorkTh();
	virtual ~CWorkTh();

public :
	SWC_BOOL m_bRunCheck;

private :
	static PVOID WorkThreadFunc(IN PVOID arg);

private :
	CUDSHandler*    m_pParent;     // CUDSHandler object pointer
	SWC_BOOL        m_bRun;        // run or not
	pthread_t       m_ThreadID;    // thread ID
	INT             m_nID;         // thread array index
	SWC_BOOL        m_bIsBusy;
};

//-----------------------------------------------------------------------------
#endif // __SWC_UDS_HANDLER_H__
