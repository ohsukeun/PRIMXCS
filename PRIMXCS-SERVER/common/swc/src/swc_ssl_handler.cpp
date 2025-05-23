/*****************************************************************************
 *                                                                           *
 * File Name : swc_ssl_handler.cpp                                           *
 * Theme     : socket handler object using 'epoll' & 'ssl'                   *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by PSS '10. 04. 13                                 *
 *             2. ssl accept routine modified by PSS '14. 05. 17             *
 *                                                                           *
 *****************************************************************************/

/// @file swc_ssl_handler.cpp
/// @author Park, sung soo
/// @date 2010-04-13
/// @brief socket handler object using 'epoll' & 'ssl'

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/poll.h>

#include <logger.h>
#include <swc_tool.h>
#include <swc_debug.h>
#include <swc_ssl_handler.h>
#include <swc_sock.h>
#include <swc_ip_obj.h>

#ifndef __SWC_SSL_HANDLER_CPP__
#define __SWC_SSL_HANDLER_CPP__

//-----------------------------------------------------------------------------
// LOCAL DEFINITIONS
//-----------------------------------------------------------------------------
#ifndef CIPHER_FORMAT_STRING
	// ssl v23
	//#define CIPHER_FORMAT_STRING "AES+SHA:3DES+SHA:@STRENGTH"
	// TLS 1.2
	#define CIPHER_FORMAT_STRING "AES256+SHA256:3DES+SHA256:@STRENGTH"
#endif 

//-----------------------------------------------------------------------------
// METHOD DEFINITIONS
//-----------------------------------------------------------------------------
/* ----------------------------------------------------------------------------
	description : CSSLWorkTh object creator
	paramentrs  : 
	return      : 
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
CSSLWorkTh::CSSLWorkTh()
{
	m_pParent   = NULL;
	m_bRun      = TRUE;
	m_ThreadID  = 0;
	m_nID       = 0;
	m_bIsBusy   = FALSE;
	m_bRunCheck = FALSE;
}

/* ----------------------------------------------------------------------------
	description : CSSLWorkTh object destroyer
	paramentrs  : 
	return      : 
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
CSSLWorkTh::~CSSLWorkTh()
{
	Exit();
}

/* ----------------------------------------------------------------------------
	description : CSSLWorkTh object init.
	paramentrs  : 
		(CSSLHandler*) CSSLHandler object pointer
	return      : (SWC_BOOL)
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
SWC_BOOL CSSLWorkTh::Init(IN CSSLHandler* pparent)
{
	SWC_BOOL bret = TRUE;

	SetParentPtr(pparent);
	return bret;
}

/* ----------------------------------------------------------------------------
	description : CSSLWorkTh object stop 
	paramentrs  : 
	return      : (SWC_BOOL)
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
SWC_BOOL CSSLWorkTh::Exit()
{
	SWC_BOOL bret = TRUE;
	m_bRun = FALSE;
	return bret;
}

/* ----------------------------------------------------------------------------
	description : CSSLWorkTh object run 
	paramentrs  : 
	return      : (SWC_BOOL)
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
SWC_BOOL CSSLWorkTh::Run()
{
	SWC_BOOL bret = TRUE;

	if(0 > pthread_create(&m_ThreadID, NULL, WorkThreadFunc, this))
	{
		_LOG_ERR("WorkThreadFunc crate failed");
		return FALSE;
	}
	return bret;
}

/* ----------------------------------------------------------------------------
	description : CSSLWorkTh object thread function
	paramentrs  : 
		(PVOID) thread function argument
	return      : (PVOID)
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
PVOID CSSLWorkTh::WorkThreadFunc(IN PVOID arg)
{
	CSSLWorkTh* pobj = NULL;
	unsigned long lMask = SWC_USE_CPU_CORE_NUM; // 1

	if(NULL == arg)
	{
		_LOG_ERR("thread argument is NULL");
		return NULL;
	}
	pobj = (CSSLWorkTh*)arg;
	pobj->m_ThreadID = pthread_self();

	if(0 > pthread_setaffinity_np(pthread_self(), sizeof(lMask), (cpu_set_t*)&lMask))
	{
		_LOG_ERR("pthread_setaffinity_np() failed");
		return NULL;
	}
	pthread_detach(pobj->m_ThreadID);

	while(TRUE == pobj->m_bRun)
	{
		// run system check
		if(TRUE == pobj->m_bRunCheck)
		{
			pobj->RunCheck(pobj->GetParentPtr()->m_OptProperty._SysChkItvl);
			pobj->m_bRunCheck = FALSE;
		}

		// pop from job queue and run ....		
		if(1 < pobj->GetParentPtr()->m_qJob.GetCount()) 
			_LOG_DBG("job queue count - %u (work thread id - %u)", pobj->GetParentPtr()->m_qJob.GetCount(), pobj->GetID());

		CSSLIONode* pjob = pobj->GetParentPtr()->PopJobQueue();

		if(NULL == pjob)
		{
			pobj->SetIdle();

			pthread_mutex_lock(&pobj->GetParentPtr()->m_Lock);
			pthread_cond_wait(&pobj->GetParentPtr()->m_Condition, &pobj->GetParentPtr()->m_Lock);
			pthread_mutex_unlock(&pobj->GetParentPtr()->m_Lock);
		}
		else
		{
			if(FALSE == pjob->IsIdle())
			{
				pobj->SetBusy();

				if(INVALID_SOCKET != pjob->GetFd())
				{
					if(NULL == pjob->GetSSL())
					{
						// SSL nego.
						pjob->StartJob();
						pobj->GetParentPtr()->AcceptSSL(pjob);
						pjob->CompleteJob();
					}
					else
					{
						// run a job
						pjob->StartJob();
						pobj->RunJob(pjob);
						pjob->CompleteJob();
					}
				}
				else
				{
					SWC::swc_sleep(0, 1);
				}
			}
		}
	}
	pthread_exit((PVOID)NULL);
	return NULL;
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object creator
	paramentrs  : 
	return      : 
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
CSSLHandler::CSSLHandler()
{
	time_t tcur = 0;

	time(&tcur);
	m_qJob.SetSize(SOCK_MAX_JOB_Q_CNT); 
	m_ConnectCnt      = 0;
	m_bRun            = TRUE;
	m_nPort           = 0;                    // no default
	m_nLimit          = SOCK_MAX_CLIENT_CNT;  // default 200
	m_nInitRecvLen    = 0;                    // no default
	m_pLstSock        = new WCSock();
	memset(&m_LstEP, 0x00, sizeof(struct epoll_event));
	m_nEPfd           = 0;
	m_ThreadID        = 0;
	m_DeleterThreadID = 0;
	m_bAcceptStart    = FALSE;
	memset(&m_OptProperty, 0x00, sizeof(SSockProperty));
	m_lCheckLoopCnt   = 0;
	m_tPrev           = 0;
	m_bHold           = FALSE;
	m_pMapIP          = NULL;
	//m_pMeth           = SSLv23_server_method();
	m_pMeth           = TLS_method(); // support TLS 1.2
	m_pCTX            = NULL;
	m_tLastUp         = tcur;
	m_tPrevSysChk     = tcur;
	m_nWorkThCount    = SOCK_MAX_WORK_TH_CNT;

	pthread_mutex_init(&m_Lock, NULL);
	pthread_cond_init(&m_Condition, NULL);
	WF_SSL_init();
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object destroyer
	paramentrs  : 
	return      : 
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
CSSLHandler::~CSSLHandler()
{
	Exit();
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object initionalize
	paramentrs  : 
		(const CHAR*) listen ip string
		(USHORT)     listen port
		(INT)         client count limit
		(INT)         initionally receive length
		(INT)         work thread count
		(const CHAR*) sertificate file path
		(const CHAR*) key file path
		(const CHAR*) password
	return      : (SWC_BOOL)
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
SWC_BOOL CSSLHandler::Init(IN const CHAR* sip, 
                           IN USHORT     nport, 
                           IN INT         nlimit, 
                           IN INT         nrcvlen,
                           IN INT         nwhcnt, 
                           IN const CHAR* scertfile,
                           IN const CHAR* scalist,
                           IN const CHAR* spasswd)
{
	SWC_BOOL bret = TRUE;
	SOCKET nlstfd = INVALID_SOCKET;

	if(NULL != sip)
	{
		if(0 < strlen(sip)) m_sIP = sip;
	}
	if(0 == nport) 
	{
		_LOG_ERR("listen port is zero");
		return FALSE;
	}
	m_nPort = nport;
	if(1 > nlimit) 
	{
		_LOG_ERR("client limit is less than 1");
		return FALSE;
	}
	m_nLimit = nlimit;
	m_nInitRecvLen = nrcvlen;

	snprintf(m_sCertFile, 256, "%s", scertfile);
	snprintf(m_sCAList,   256, "%s", scalist);
	snprintf(m_sPasswd,   256, "%s", spasswd);
	if(NULL == InitCTX(m_sCertFile, m_sCAList, m_sPasswd))
	{
		_LOG_ERR("InitCTX() failed");
		return FALSE;
	}

	if(0 == m_sIP.GetLength()) nlstfd = m_pLstSock->Listen(m_nPort, 0, NULL);
	else                       nlstfd = m_pLstSock->Listen(m_nPort, 1, LPCSTR(m_sIP));
	m_pLstSock->SetNonBlockMode(m_pLstSock->GetListenSockDes());

	if(0 > nlstfd) 
	{
		_LOG_ERR("listen failed");
		SockErrMsg(m_pLstSock->GetLastError());
		return FALSE;
	}
	
	_LOG_MSG("Listening. sfd = %d. client limit = %lu", nlstfd, (u_long)m_nLimit);
	m_nEPfd = epoll_create(SOCK_MAX_EPOLL_FD_CNT);
	m_pLstSock->SetNonBlockMode(m_nEPfd);

	if(0 >= m_nEPfd) 
	{
		_LOG_ERR("epoll_create() failed (m_nLimit = %d)", m_nLimit);
		return FALSE;
	}
	m_LstEP.events  = EPOLLIN | EPOLLET | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
	m_LstEP.data.fd = m_pLstSock->GetListenSockDes();
	if(0 > epoll_ctl(m_nEPfd, EPOLL_CTL_ADD, m_pLstSock->GetListenSockDes(), &m_LstEP))
	{
		_LOG_ERR("epoll_ctl() failed");
		return FALSE;
	}
	if(SOCK_MAX_WORK_TH_CNT > nwhcnt) m_nWorkThCount = SOCK_MAX_WORK_TH_CNT;
	else                              m_nWorkThCount = nwhcnt;
	for(int i = 0; i < m_nWorkThCount; i++)
	{
		m_pWorkTh.SetAtGrow(i, (CSSLWorkTh*)NULL);
	}
	return bret;
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object Run
	paramentrs  : 
	return      : (SWC_BOOL)
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
SWC_BOOL CSSLHandler::Run()
{
	SWC_BOOL bret = TRUE;

	if(0 > pthread_create(&m_ThreadID, NULL, HandlerThreadFunc, (PVOID)this))
	{
		_LOG_ERR("HandlerThreadFunc pthread_create() failed");
		return FALSE;
	}
	for(int i = 0; i < m_nWorkThCount; i++)
	{
		if(NULL != m_pWorkTh[i]) 
		{
			m_pWorkTh[i]->Run();
			m_pWorkTh[i]->SetID(i);
		}
		else
		{
			_LOG_ERR("work thread pointing null");
			return FALSE;
		}
	}
	return bret;
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object Stop
	paramentrs  : 
	return      : 
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::Exit()
{
	UINT i = 0;

	m_bRun = FALSE;
	SWC::swc_sleep(0, 1000);
	if(NULL != m_pLstSock)
	{
		// cleanup work threads
		for(i = 0; i < m_pWorkTh.GetCount(); i++) 
		{
			if(NULL != m_pWorkTh[i]) m_pWorkTh[i]->Exit();
		}
		pthread_mutex_lock(&m_Lock);
		pthread_cond_broadcast(&m_Condition);
		pthread_mutex_unlock(&m_Lock);
		for(i = 0; i < m_pWorkTh.GetCount(); i++) 
		{
			if(NULL != m_pWorkTh[i]) 
			{
				delete m_pWorkTh[i];
				m_pWorkTh[i] = NULL;
			}
		}

		// cleanup io-node object pointer map
		STR_TO_PTR_MAP_POSITION pos = m_mapPtrIONode.GetStartPosition();

		while(FALSE == m_mapPtrIONode.IsEnd(pos))
		{
			WCString skey;
			PVOID pptr = NULL;

			m_mapPtrIONode.GetNextAssoc(pos, skey, pptr);
			if(NULL != pptr)
			{
				delete (CSSLIONode*)pptr;
			}
		}
		m_mapPtrIONode.RemoveAll();

		// cleanup listen socket
		if(NULL != m_pLstSock) 
		{
			delete m_pLstSock;
			m_pLstSock = NULL;
		}

		// destroy sync. environments
		pthread_cond_destroy(&m_Condition);
		pthread_mutex_destroy(&m_Lock);

		// cleanup handler thread
		if(0 != m_ThreadID)
		{
		//	int nstatus = 0;
		//	pthread_join(m_ThreadID, (PVOID*)nstatus);
			m_ThreadID = 0;
		}

		// close epoll file descriptor
		if(-1 != m_nEPfd) 
		{
			close(m_nEPfd);
			m_nEPfd = -1;
		}
	}
	if(NULL != m_pCTX)
	{
		WF_SSL_CTX_free(m_pCTX);
		m_pCTX = NULL;
	}
	WF_SSL_exit();
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler main thread function
	paramentrs  : 
		(PVOID) thread argument
	return      : (PVOID)
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
PVOID CSSLHandler::HandlerThreadFunc(IN PVOID arg)
{
	CSSLHandler* pobj = NULL;
	int nret = 0, nRstCnt = 0;
	unsigned long lMask = SWC_USE_CPU_CORE_NUM; // 1
	sigset_t newset;

	if(NULL == arg)
	{
		_LOG_ERR("thread argument is NULL");
		return NULL;
	}
	pobj = (CSSLHandler*)arg;
	pobj->m_ThreadID = pthread_self();
	if(0 > pthread_setaffinity_np(pthread_self(), sizeof(lMask), (cpu_set_t*)&lMask))
	{
		_LOG_ERR("pthread_setaffinity_np() failed");
		return NULL;
	}
	pthread_detach(pobj->m_ThreadID);

	sigemptyset(&newset);
	sigaddset(&newset, SIGPIPE);

	while(TRUE == pobj->m_bRun)
	{
		pobj->UpdateLastUpTime();

		if(FALSE == pobj->CanAcceptClient())
		{
			SWC::swc_sleep(0, 10000);
			continue;
		}

		sigset_t orgset;	
		
		pthread_sigmask(SIG_SETMASK, &newset, &orgset);
		nret = epoll_wait(pobj->m_nEPfd, pobj->m_pArrEPollEvent, pobj->m_nLimit, 1000);
		pthread_sigmask(SIG_SETMASK, &orgset, NULL);

		if(-1 == nret)
		{
			int nerr = errno;

			if(EBADF == nerr) 
				_LOG_ERR("epoll_wait() error : m_nEPfd is not a valid descroptor (EBADF)");
			else if(EFAULT == nerr) 
				_LOG_ERR("epoll_wait() error : The memory area pointed to by events is not accessible with write permissions (EFAULT)");
			else if(EINTR == nerr)
			{
				_LOG_ERR("epoll_wait() error : The call was interrupted by a signal handler before any of the requested events occurred or the timeout expired (EINTR)");

				nRstCnt++;
				if(5 == nRstCnt)
				{
					pobj->RenewalLstEpoll();
					nRstCnt = 0;
				}
				SWC::swc_sleep(0, 1);
				continue;
			}
			else if(EINVAL == nerr)
				_LOG_ERR("epoll_wait() error : m_nEPfd is not an epoll file descriptor, or maxevents is less than or equal to zero");
			else
				_LOG_ERR("epoll_wait() error : %d");
		}
		// session time check 
		pobj->CheckSessionTimeout();

		// system check run
		pobj->RunCheckToAllWorkThread();

		if(NULL == pobj->m_pLstSock) break;
		if(FALSE == pobj->m_bRun) break;

		if(0 == nret) 
		{
//			_LOG_DBG("no event occured. check backlog and try accept(). exception handling");
			pobj->AcceptClient();
		}
		else 
		{
			_LOG_DBG("%d event occured", nret);
		}
		for(int i = 0; i < nret; i++)
		{
			if(FALSE == pobj->m_bRun) break;

			// accept client
			if(EPOLLIN & pobj->m_pArrEPollEvent[i].events)
			{
				pthread_mutex_lock(&pobj->m_Lock);
				SOCKET nsfd = pobj->m_pArrEPollEvent[i].data.fd;
				pthread_mutex_unlock(&pobj->m_Lock);
				
				if(pobj->m_pLstSock->GetListenSockDes() == nsfd)
				{
					if((UINT)pobj->m_nLimit <= pobj->m_mapPtrIONode.GetCount())
					{
						_LOG_ERR("connection limit over. new session blocked (current count = %d)", pobj->m_mapPtrIONode.GetCount());
						pobj->AcceptAndTerminateClient();
						SWC::swc_sleep(0, 1);
						continue;
					}
					pobj->AcceptClient();
				}
				else if((EPOLLERR   & pobj->m_pArrEPollEvent[i].events) ||
						(EPOLLHUP   & pobj->m_pArrEPollEvent[i].events) ||
						(EPOLLRDHUP & pobj->m_pArrEPollEvent[i].events))
				{
					if(FALSE == pobj->m_bRun) break;
					CSSLIONode* pnode = pobj->FindNode(nsfd);

					if(NULL == pnode) 
					{
						_LOG_ERR("pnode is null. case 1-1");
						continue;
					}
					if(65535 >= (u_long)pnode) 
					{
						_LOG_ERR("pnode pointer is smaller than 65535. case 1-2");
						continue;
					}
					if(nsfd != pnode->GetFd()) continue;
					if(pobj->m_pLstSock->GetListenSockDes() == pnode->GetFd())
					{
						_LOG_ERR("system fatal error. listening port error occured. exit");
						break;
					}
					_LOG_ERR("error occured. case 1. fd = %u", pnode->GetFd());

					pobj->DetachNode(pnode);
				}
				else
				{
					if(FALSE == pobj->m_bRun) break;
					CSSLIONode* pnode = pobj->FindNode(nsfd);

					if(NULL == pnode) 
					{
						_LOG_ERR("pnode is null. case 1-3");
						continue;
					}
					if(65535 >= (u_long)pnode) 
					{
						_LOG_ERR("pnode pointer is smaller than 65535. case 1-4");
						continue;
					}
					if(nsfd != pnode->GetFd()) continue;
					if(pobj->m_pLstSock->GetListenSockDes() == pnode->GetFd()) 
					{
						_LOG_ERR("pnode sfd is same with listen sfd. case 1-5");
						continue;
					}
					pobj->PushJobQueue(pnode);
					_LOG_DBG("PushJobQueue() called 1st (errno = %d. events = %d)", errno, pobj->m_pArrEPollEvent[i].events);
					SWC::swc_sleep(0, 1);
				}
			}
			else if(EPOLLPRI & pobj->m_pArrEPollEvent[i].events)
			{
				pthread_mutex_lock(&pobj->m_Lock);
				SOCKET nsfd = pobj->m_pArrEPollEvent[i].data.fd;
				pthread_mutex_unlock(&pobj->m_Lock);

				if( (EPOLLERR & pobj->m_pArrEPollEvent[i].events) ||
					(EPOLLHUP & pobj->m_pArrEPollEvent[i].events) ||
					(EPOLLRDHUP & pobj->m_pArrEPollEvent[i].events))
				{
					if(FALSE == pobj->m_bRun) break;
					CSSLIONode* pnode = pobj->FindNode(nsfd);

					_LOG_ERR("error occured. case 2");
					if(NULL == pnode) 
					{
						_LOG_ERR("pnode is null. case 2-1");
						continue;
					}
					if(65535 >= (u_long)pnode) 
					{
						_LOG_ERR("node pointer is smaller than 65535. case 2-2");
						continue;
					}
					if(nsfd != pnode->GetFd()) continue;
					if(pobj->m_pLstSock->GetListenSockDes() == pnode->GetFd()) 
					{
						_LOG_ERR("pnode sfd is same with listen sfd. case 2-2");
						continue;
					}
					_LOG_ERR("error occured. case 2. sfd = %d", pnode->GetFd());

					pobj->DetachNode(pnode);
				}
				else
				{
					if(FALSE == pobj->m_bRun) break;
					CSSLIONode* pnode = pobj->FindNode(nsfd);

					if(NULL == pnode) 
					{
						_LOG_ERR("pnode is null. case 4-1");
						continue;
					}
					if(65535 >= (u_long)pnode) 
					{
						_LOG_ERR("pnode is smaller than 65535. case 4-2");
						continue;
					}
					if(nsfd != pnode->GetFd()) continue;
					if(pobj->m_pLstSock->GetListenSockDes() == pnode->GetFd())
					{
						_LOG_ERR("pnode sfd is same with listen sfd.");
						continue;
					}
					pobj->PushJobQueue(pnode);
					_LOG_DBG("PushJobQueue() 2nd");
					SWC::swc_sleep(0, 1);
				}
			}
			else 
			{
				pthread_mutex_lock(&pobj->m_Lock);
				SOCKET nsfd = pobj->m_pArrEPollEvent[i].data.fd;
				pthread_mutex_unlock(&pobj->m_Lock);

				if( (EPOLLERR & pobj->m_pArrEPollEvent[i].events) ||
					(EPOLLHUP & pobj->m_pArrEPollEvent[i].events) ||
					(EPOLLRDHUP & pobj->m_pArrEPollEvent[i].events))
				{
					if(FALSE == pobj->m_bRun) break;
					CSSLIONode* pnode = pobj->FindNode(nsfd);

					_LOG_ERR("error occured. case 3");
					if(NULL == pnode) 
					{
						_LOG_ERR("pnode is null. case 3-1");
						continue;
					}
					if(65535 >= (u_long)pnode) 
					{
						_LOG_ERR("pnode is smaller than 65535. case 3-2");
						continue;
					}
					if(nsfd != pnode->GetFd()) continue;
					if(pobj->m_pLstSock->GetListenSockDes() == pnode->GetFd()) 
					{
						_LOG_ERR("pnode sfd is same with listen sfd. case 3-3");
						continue;
					}
					_LOG_ERR("error occured. case 3. sfd = %d", pnode->GetFd());

					pobj->DetachNode(pnode);
				}
			}
		}
	}
	pthread_exit((PVOID)NULL);
	return NULL;
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler - push a job to the job queue
	paramentrs  : 
		(CSSLIONode*) client node
	return      :
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::PushJobQueue(IN CSSLIONode* pobj)
{
	time_t tcur = 0;

	if(NULL == pobj) return;
	pSJobSSLContainer pjob = (pSJobSSLContainer)malloc(sizeof(SJobSSLContainer));
	if(NULL == pjob) return;

	time(&tcur);
	pjob->_tStamp = tcur;
	pjob->_pJobNode = pobj;
	m_qJob.Push(pjob);

	pthread_mutex_lock(&m_Lock);
	pthread_cond_signal(&m_Condition);
	pthread_mutex_unlock(&m_Lock);
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler - pop a job from the job queue
	paramentrs  : 
	return      : (CSSLIONode*) return job
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
CSSLIONode* CSSLHandler::PopJobQueue()
{
	pSJobSSLContainer pjob = NULL;
	void*             pout = NULL;
	CSSLIONode*       pret = NULL;

	if(FALSE == m_qJob.Pop(pout)) return NULL;
	if(NULL == pout) return NULL;
	pjob = (pSJobSSLContainer)pout;
	if(NULL == pjob->_pJobNode) 
	{ 
		free(pjob); 
		return NULL; 
	}
	pret = pjob->_pJobNode; free(pjob);
	return pret;
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler - accept client
	paramentrs  : 
	return      :
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::AcceptClient()
{
	while(1)
	{
		if((UINT)m_nLimit <= m_mapPtrIONode.GetCount())
		{
			AcceptAndTerminateClient();
			break;
		}

		SOCKET             nclifd = INVALID_SOCKET;
		int                nsin   = 0;
		struct sockaddr_in cliaddr;

		if(NULL == m_pLstSock) return;

		if(0 > (nclifd = accept(m_pLstSock->GetListenSockDes(), 
								(struct sockaddr*)&cliaddr,
								(socklen_t*)&nsin)))
		{
			return;	
		}

		_LOG_DBG("[%s] something accepted (listen fd = %d / fd = %d)", 
			LPCSTR(m_sModuleName), m_pLstSock->GetListenSockDes(), nclifd);

		int nsize = 30000;
		setsockopt(nclifd, SOL_SOCKET, SO_SNDBUF, (char*)&nsize, sizeof(nsize));
		setsockopt(nclifd, SOL_SOCKET, SO_RCVBUF, (char*)&nsize, sizeof(nsize));

		struct timeval tv_timeo = {SOCK_DEFAULT_TIMEOUT, 0}; 
		setsockopt(nclifd, SOL_SOCKET, SO_SNDTIMEO, &tv_timeo, sizeof(tv_timeo));
		setsockopt(nclifd, SOL_SOCKET, SO_RCVTIMEO, &tv_timeo, sizeof(tv_timeo));

		m_pLstSock->SetNonBlockMode(nclifd);

		RegAcceptedIP(nclifd);

		CSSLIONode* pnode = AddNode(nclifd);
		if(NULL == pnode) return;

		pnode->SetSSL(NULL);
		PushJobQueue(pnode);

		_LOG_DBG("PushJobQueue() called");
	}
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler - registering node to the epoll
	paramentrs  : 
		(CSSLIONode*) io node object pointer
	return      :
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::RegisterNode(IN CSSLIONode* pnode)
{
	if(NULL == pnode) return;
	if(NULL == m_pLstSock) return;
	
	struct epoll_event cli_event;

	memset(&cli_event, 0x00, sizeof(struct epoll_event));
	cli_event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLERR | EPOLLHUP;
	cli_event.data.fd = pnode->GetFd();
	if(0 > epoll_ctl(m_nEPfd, EPOLL_CTL_ADD, pnode->GetFd(), &cli_event))
	{
		_LOG_ERR("epoll_ctl() failed (SFD = %d)", pnode->GetFd());
		if(EBADF != errno) DeleteNode(pnode->GetFd());
		return;
	}
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler - detach node from epoll
	paramentrs  : 
		(CSSLIONode*) io node object pointer
	return      :
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::DetachNode(IN CSSLIONode* pnode)
{
	if(NULL == pnode) return;
	if(NULL == m_pLstSock) return;

	SOCKET nsfd = pnode->GetFd();
	if(0 >= nsfd) { DeleteNodeSafe(pnode); return; }

	_LOG_DBG("[%s] node detached (sfd = %d)", LPCSTR(m_sModuleName), nsfd);
	if((nsfd == m_nEPfd) || (nsfd == m_pLstSock->GetListenSockDes()))
	{
		_LOG_ERR("detaching node is listen socket. skip (%d)", nsfd);
		DeleteNode(nsfd, FALSE);
		return;
	}

	struct epoll_event cli_event;

	if((0 < nsfd) && (nsfd < 65535))
	{
		memset(&cli_event, 0x00, sizeof(struct epoll_event));
		cli_event.data.fd = nsfd;

		if((m_nEPfd != nsfd) && (m_pLstSock->GetListenSockDes() != nsfd))
		{
			epoll_ctl(m_nEPfd, EPOLL_CTL_DEL, nsfd, &cli_event);
		}
	}
	DeleteNodeFromJobQueue(pnode);
	if(0 < m_ConnectCnt) m_ConnectCnt--;

	DeleteNode(nsfd);
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler - enable EPOLLONESHOT
	paramentrs  : 
		(CSSLIONode*) io node object pointer
	return      :
	history     :
		- 2010. 05. 02 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::SetEnableOneShotNode(IN CSSLIONode* pnode)
{
	if(NULL == pnode) return;

	SOCKET nsfd = pnode->GetFd();
	struct epoll_event cli_event;

	memset(&cli_event, 0x00, sizeof(struct epoll_event));
	cli_event.events = EPOLLONESHOT;
	cli_event.data.fd = nsfd;
	epoll_ctl(m_nEPfd, EPOLL_CTL_MOD, nsfd, &cli_event);
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler - disable EPOLLONESHOT
	paramentrs  : 
		(CSSLIONode*) io node object pointer
	return      :
	history     :
		- 2010. 05. 02 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::SetDisableOneShotNode(IN CSSLIONode* pnode)
{
	if(NULL == pnode) return;

	SOCKET nsfd = pnode->GetFd();
	struct epoll_event cli_event;

	memset(&cli_event, 0x00, sizeof(struct epoll_event));
	cli_event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLERR | EPOLLHUP;
	cli_event.data.fd = nsfd;
	epoll_ctl(m_nEPfd, EPOLL_CTL_MOD, nsfd, &cli_event);
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler - find CSSLIONode from m_mapPtrIONode
	paramentrs  : 
		(SOCKET) client socket descriptor
	return      : (CSSLIONode*) return find CSSLIONode
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
CSSLIONode* CSSLHandler::FindNode(IN SOCKET nsfd)
{
	CSSLIONode* pret = NULL;
	WCString skey;
	PVOID    pout = NULL;

	skey.Format("%d", nsfd);
	m_mapPtrIONode.Lock();
	if(FALSE == m_mapPtrIONode.Lookup(skey, pout)) 
	{ 
		m_mapPtrIONode.Unlock(); 
		return NULL;
	}
	if(NULL == pout)
	{
		m_mapPtrIONode.RemoveKey(skey);
		m_mapPtrIONode.Unlock();
		return NULL;
	}
	pret = (CSSLIONode*)pout;
	m_mapPtrIONode.Unlock();
	return pret;
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler - add new CIONOde to m_mapPtrIONode
	paramentrs  : 
		(SOCKET) client socket descriptor
	return      : (CSSLIONode*) return find CSSLIONode
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
CSSLIONode* CSSLHandler::AddNode(IN SOCKET nsfd)
{
	CSSLIONode* pret = NULL;
	WCString skey;
	PVOID    pout = NULL;

	skey.Format("%d", nsfd);
	m_mapPtrIONode.Lock();
	if(FALSE == m_mapPtrIONode.Lookup(skey, pout))
	{
		pret = GetIdleNode();
		pret->SetFd(nsfd);
		pret->InitAsRecv(m_nInitRecvLen);
		m_mapPtrIONode.SetAt(skey, (PVOID)pret);
	}
	else
	{
		pret = (CSSLIONode*)pout;
		pret->Reset();
		pret->SetFd(nsfd);
		pret->InitAsRecv(m_nInitRecvLen);
	}
	m_mapPtrIONode.Unlock();

	_LOG_MSG("node count = %u", m_mapPtrIONode.GetCount());
	return pret;
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler - delete CIONOde from m_mapPtrIONode
	paramentrs  : 
		(SOCKET)   socket descriptor
		(SWC_BOOL) delete from delete map
	return      : 
	history     :
		- 2012. 07. 26 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::DeleteNode(IN SOCKET nsfd, IN SWC_BOOL bAddToDeleteMap)
{
	WCString skey;
	CSSLIONode* pFind = NULL;
	PVOID pout = NULL;

	skey.Format("%d", nsfd);

	m_mapPtrIONode.Lock();
	m_mapPtrIONode.Lookup(skey, pout);
	if(NULL != pout)
	{
		pFind = (CSSLIONode*)pout;
		m_mapPtrIONode.RemoveKey(skey);
		if(TRUE == bAddToDeleteMap) AddToDeleleMap(pFind);
	}
	m_mapPtrIONode.Unlock();
	_LOG_MSG("node count = %u", m_mapPtrIONode.GetCount());
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler - delete CIONOde from m_mapPtrIONode for exception handling
	paramentrs  : 
		(CSSLIONode*) I/O node
	return      : 
	history     :
		- 2014. 11. 05 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::DeleteNodeSafe(IN CSSLIONode* pNode)
{
	SWC_BOOL bFind = FALSE;

	if(0 < m_mapPtrIONode.GetCount())
	{
		m_mapPtrIONode.Lock();
		STR_TO_PTR_MAP_POSITION pos = m_mapPtrIONode.GetStartPosition();

		while(FALSE == m_mapPtrIONode.IsEnd(pos))
		{
			WCString sKey;
			PVOID pOut = NULL;

			m_mapPtrIONode.GetNextAssoc(pos, sKey, pOut);
			if(NULL != pOut)
			{
				if(pOut == (PVOID)pNode)
				{
					m_mapPtrIONode.RemoveKey(sKey);
					bFind = TRUE;
					break;
				}
			}
		}
		m_mapPtrIONode.Unlock();
	}
	if(TRUE == bFind) 
	{
		AddToDeleleMap(pNode);
	}
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler - Init. CTX object
	paramentrs  : 
		(const CHAR*) certification file path
		(const CHAR*) CA list file path
		(const CHAR*) password
	return      : 
	history     :
		- 2010. 04. 13 created by Park, sung soo
		- 2023. 06. 06 WF_SSLeay_add_ssl_algorithms(), WF_SSL_load_error_strings() added
-----------------------------------------------------------------------------*/
SSL_CTX* CSSLHandler::InitCTX(IN const CHAR* scertfile, IN const CHAR* scalist, IN const CHAR* spasswd)
{
	SSL_CTX* ctx = NULL;

	ctx = WF_SSL_CTX_new(m_pMeth);

   if(NULL == ctx) return NULL;
	if(0 == WF_SSL_CTX_set_cipher_list(ctx, CIPHER_FORMAT_STRING))
	{
		_LOG_ERR("SSL_CTX_set_cipher_list() failed");
		if(TRUE == IsDMsgOn()) ERR_print_errors_fp(stderr);
		if(NULL != ctx) WF_SSL_CTX_free(ctx);
		return NULL;
	}
	if(0 > WF_SSL_CTX_use_certificate_file(ctx, scertfile, SSL_FILETYPE_PEM))
	{
		_LOG_ERR("can not read certificate file (%s)", scertfile);
		if(TRUE == IsDMsgOn()) ERR_print_errors_fp(stderr);
		if(NULL != ctx) WF_SSL_CTX_free(ctx);
		return NULL;
	}
	WF_SSL_CTX_set_default_passwd_cb_userdata(ctx, (CHAR*)spasswd);
	if(0 > WF_SSL_CTX_use_PrivateKey_file(ctx, scalist, SSL_FILETYPE_PEM))
	{
		_LOG_ERR("can not read key file (%s)", scalist);
		if(TRUE == IsDMsgOn()) ERR_print_errors_fp(stderr);
		if(NULL != ctx) WF_SSL_CTX_free(ctx);
		return NULL;
	}
	if (!WF_SSL_CTX_check_private_key(ctx))
	{
		_LOG_ERR("Private key does not match the certificate public key");
		if(TRUE == IsDMsgOn()) ERR_print_errors_fp(stderr);
		if(NULL != ctx) WF_SSL_CTX_free(ctx);
		return NULL;
	}
	m_pCTX = ctx;
	return ctx;
}

/* ----------------------------------------------------------------------------
	description : CSSLHandler object handler - accept SSL & registering
	paramentrs  : 
		(CSSLIONode*) ssl io node object pointer
	return      :
	history     :
		- 2010. 04. 13 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::AcceptSSL(IN CSSLIONode* pnode)
{
	SSL* ssl = NULL;

	if(NULL == pnode) return;
	if(NULL == m_pLstSock) return;
	if(NULL == m_pCTX) return;

	SOCKET nSfd = pnode->GetFd();
	if((0 > nSfd) || (65535 < nSfd)) return;

	ssl = WF_SSL_new(m_pCTX);
	if(NULL == ssl)
	{
		_LOG_ERR("WF_SSL_new() failed");	
		if(TRUE == IsDMsgOn()) ERR_print_errors_fp(stderr);
		m_pLstSock->SetNonBlockMode(nSfd);
		DeleteNode(nSfd);
		return;
	}
	if(0 == WF_SSL_set_fd(ssl, nSfd))
	{
		_LOG_ERR("WF_SSL_set_fd() failed");
		if(TRUE == IsDMsgOn()) ERR_print_errors_fp(stderr);
		m_pLstSock->SetNonBlockMode(nSfd);
		DeleteNode(nSfd);
		WF_SSL_free(ssl);
		return;
	}
	SWC::swc_sleep(0, 100);
//	if(FALSE == SSLAcceptBlockMode(ssl))
	if(FALSE == SSLAcceptNonBlockMode(ssl))
	{
		m_pLstSock->SetNonBlockMode(nSfd);
		DeleteNode(nSfd);
		WF_SSL_shutdown(ssl);
		WF_SSL_free(ssl);
		return;
	}

	pnode->SetSSL(ssl);
	RegisterNode(pnode);
//	m_pLstSock->SetNonBlockMode(nSfd);
	m_ConnectCnt++;
}

/* ----------------------------------------------------------------------------
	description : delete job node from job queue given I/O node
	paramentrs  : 
		(CSSLIONode*) ssl io node object pointer
	return      :
	history     :
		- 2010. 05. 05 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::DeleteNodeFromJobQueue(IN CSSLIONode* pnode)
{
	u_int nQSize = m_qJob.m_Queue.size(), i = 0;
	_LOG_DBG("enter - m_qJob.m_Queue.size() = %u", nQSize);

	if(0 == nQSize) return;
	m_qJob.Lock();
	while(1)
	{
		u_int qsize = m_qJob.m_Queue.size();
		pSJobSSLContainer pdel = NULL;

		if(0 == qsize) break;
		for(; i < qsize; i++)
		{
			pSJobSSLContainer pcur = (pSJobSSLContainer)m_qJob.m_Queue[i];
			if(NULL != pcur)
			{
				if(pcur->_pJobNode == pnode)
				{
					pdel = pcur;
					break;
				}
			}
		}
		if(NULL != pdel)
		{
			m_qJob.m_Queue.erase(m_qJob.m_Queue.begin() + i);
			delete pdel;
		}		
		else break;
	}
	m_qJob.Unlock();
	_LOG_DBG("exit");
}

/* ----------------------------------------------------------------------------
	description : check session timeout
	paramentrs  : 
		(CSSLIONode*) ssl io node object pointer
	return      :
	history     :
		- 2010. 05. 05 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::CheckSessionTimeout()
{
	time_t tcur = 0;
	WCArray<CSSLIONode*> arrTimeOut;

	time(&tcur);
	if(0 == m_tPrev)    { m_tPrev = tcur; return; }
	if(tcur != m_tPrev) m_lCheckLoopCnt++;
	else                return;
	if(10 > m_lCheckLoopCnt) { m_tPrev = tcur; return; }
	else m_lCheckLoopCnt = 0;

	if((SWC_SOCK_ENABLE_IO_TIMEOUT & m_OptProperty._OptFlag) && ((0 < m_OptProperty._IOTimeout)))
	{
		if(0 < m_mapPtrIONode.GetCount())
		{
			STR_TO_PTR_MAP_POSITION pos = m_mapPtrIONode.GetStartPosition();

			while(FALSE == m_mapPtrIONode.IsEnd(pos))
			{
				WCString skey;
				PVOID pptr = NULL;

				m_mapPtrIONode.GetNextAssoc(pos, skey, pptr);
				if(NULL != pptr)
				{
					CSSLIONode* pNode = (CSSLIONode*)pptr;

					if(0 == pNode->GetTimeLastIO())
					{
						pNode->UpdateTimeLastIO();
					}
					else if((UINT)m_OptProperty._IOTimeout <= (UINT)abs(tcur - pNode->GetTimeLastIO()))
					{
						arrTimeOut.Add(pNode);
					}
				}
			}

			if(0 < arrTimeOut.GetCount())
			{
				for(UINT nID = 0; nID < arrTimeOut.GetCount(); nID++)
				{
					CSSLIONode* pDel = arrTimeOut[nID];
					if(NULL != pDel) 
					{
						_LOG_ERR("[%s] session timeout. closed (sfd = %d)", LPCSTR(m_sModuleName), pDel->GetFd());
						
						DetachNode(pDel);
					}
				}
				arrTimeOut.RemoveAll();
			}
		}
	}
}

/* ----------------------------------------------------------------------------
	description : wait until job completing on all work threads 
	paramentrs  : 
	return      : 
	history     :
		- 2010. 09. 23 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::WaitJobCompleted()
{
	while(1)
	{
		SWC_BOOL bBusy = FALSE;

		for(int i = 0; i < m_nWorkThCount; i++)
		{
			if(NULL != m_pWorkTh[i])
			{
				if(TRUE == m_pWorkTh[i]->IsBusy()) bBusy = TRUE;
			}
		}
		if(FALSE == bBusy) break;
	}
}

/* ----------------------------------------------------------------------------
	description : renewal epoll event setting for exceptional handing
	paramentrs  : 
	return      : 
	history     :
		- 2010. 09. 23 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::RenewalLstEpoll()
{
	if(NULL == m_pLstSock) return;
	if(0 > epoll_ctl(m_nEPfd, EPOLL_CTL_DEL, m_pLstSock->GetListenSockDes(), &m_LstEP))
	{
		_LOG_ERR("epoll_ctl() delete failed");
		return;
	}
	m_LstEP.events = 0;
	m_LstEP.events  = EPOLLIN | EPOLLET | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
	m_LstEP.data.fd = m_pLstSock->GetListenSockDes();
	if(0 > epoll_ctl(m_nEPfd, EPOLL_CTL_ADD, m_pLstSock->GetListenSockDes(), &m_LstEP))
	{
		_LOG_ERR("epoll_ctl() add failed");
		return;
	}
	_LOG_DBG("RenewalLstEpoll() operated");
}

/* ----------------------------------------------------------------------------
	description : check and add or update last I/O time if IP map is valid pointer
	paramentrs  : 
		(SOCKET) socket descriptor
	return      :
	history     :
		- 2010. 09. 23 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::RegAcceptedIP(IN SOCKET nsfd)
{
	if(NULL == m_pMapIP) return;
	if(0 >= nsfd) return;
	UINT lip = SWC::swc_get_peer_ip(nsfd);
	if(0 < lip) 
	{
		m_pMapIP->Lock();

		WCString skey;
		void* pout = NULL;

		skey.Format("%lu", (u_long)lip);
		m_pMapIP->Lookup(skey, pout);
		if(NULL != pout)
		{
			WCIP* pfind = (WCIP*)pout;

			pfind->SetUpdateTime();

			_LOG_MSG("[%s] CSSLHandler : IP - %s / SOCKET - %d updated,", LPCSTR(m_sModuleName), pfind->GetSIP(), nsfd);
		}
		else
		{
			WCIP* pnew = new WCIP();

			pnew->SetIP(lip);
			pnew->SetUpdateTime();
			m_pMapIP->SetAt(skey, (void*)pnew);

			_LOG_MSG("[%s] CSSLHandler : IP - %s / SOCKET - %d add\n", LPCSTR(m_sModuleName), pnew->GetSIP(), nsfd);
		}

		m_pMapIP->Unlock();
	}
}

/* ----------------------------------------------------------------------------
	description : update loop check time.
	parameters  : 
	return      : 
	history     :
		- 2012. 01. 12 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLHandler::UpdateLastUpTime()
{
	time_t tcur = 0;

	time(&tcur);
	m_tLastUp = tcur;
}

/* ----------------------------------------------------------------------------
	description : accept and terminate session
	paramentrs  : 
	return      : 
	history     :
		- 2012. 01. 11 created by Park, sung soo
		- 2014. 05. 15 modified by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::AcceptAndTerminateClient()
{
	while(1)
	{
		SOCKET             nclifd = INVALID_SOCKET;
		int                nsin   = 0;
		struct sockaddr_in cliaddr;

		if(NULL == m_pLstSock) return;
		if(0 > (nclifd = accept(m_pLstSock->GetListenSockDes(),
   	                         (struct sockaddr*)&cliaddr,
   	                         (socklen_t*)&nsin)))
		{
			return;
		}
		if((0 < nclifd) && (nclifd != m_pLstSock->GetListenSockDes()))
		{
			CSSLIONode* pnode = FindNode(nclifd);
	
			if(NULL != pnode)
			{
				DetachNode(pnode);
			}
			else
			{
				m_pLstSock->SetNonBlockMode(nclifd); // (2014. 05. 15) added by Park, sung soo

				struct linger tmp = {1, 0};
				setsockopt(nclifd, SOL_SOCKET, SO_LINGER, (char*)&tmp, sizeof(struct linger));
				shutdown(nclifd, SHUT_RDWR);
				close(nclifd);
			}
		}
	}
}

/* ----------------------------------------------------------------------------
	description : run system check command to all work thread
	paramentrs  : 
	return      : 
	history     :
		- 2012. 01. 11 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::RunCheckToAllWorkThread()
{
	time_t tcur = 0;

	time(&tcur);
	if( (0 < m_tPrevSysChk) && 
		(SWC_SYSTEM_CHECK_ENABLE & m_OptProperty._OptFlag) &&
		(SYSTEM_CHECK_MIN_INTERVAL < m_OptProperty._SysChkItvl))
	{
		time_t tGap = abs(tcur - m_tPrevSysChk);
		if(m_OptProperty._SysChkItvl <= tGap)
		{
			for(UINT i = 0; i < m_pWorkTh.GetCount(); i++)
			{
				CSSLWorkTh* pWork = m_pWorkTh[i];
				pWork->m_bRunCheck = TRUE;
			}
			pthread_mutex_lock(&m_Lock);
			pthread_cond_broadcast(&m_Condition);
			pthread_mutex_unlock(&m_Lock);

			m_tPrevSysChk = tcur;
		}
	}
}

/* ----------------------------------------------------------------------------
	description : add io node to the deleter map
	paramentrs  : 
		(CSSLIONode*) io-node pointer
	return      :
	history     :
		- 2013. 05. 03 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CSSLHandler::AddToDeleleMap(IN CSSLIONode* pDel)
{
	if(NULL == pDel) return;
	WCString sKey;
	PVOID    pout = NULL;
	
	sKey.Format("%lu", (UINT64)pDel);
	if(false == m_mapDelIONode.Lookup(sKey, pout))
	{
		if(FALSE == pDel->HasJob())
		{
			pDel->SetIdle();
		}
		m_mapDelIONode.SetAt(sKey, (PVOID)pDel);
	}
}

/* ----------------------------------------------------------------------------
	description : get idle node for reuse
	paramentrs  : 
	return      : (CSSIONode*) return object pointer
	history     :
		- 2014. 04. 22 created by Park, sung soo
-----------------------------------------------------------------------------*/
CSSLIONode* CSSLHandler::GetIdleNode()
{
	CSSLIONode* pRet = NULL;
	SWC_BOOL bFind = FALSE;

	if(0 < m_mapDelIONode.GetCount())
	{
		STR_TO_PTR_MAP_POSITION pos = m_mapDelIONode.GetStartPosition();

		while(FALSE == m_mapDelIONode.IsEnd(pos))
		{
			WCString sKey;
			PVOID pout = NULL;

			m_mapDelIONode.GetNextAssoc(pos, sKey, pout);

			if(NULL != pout)
			{
				pRet = (CSSLIONode*)pout;
				if(TRUE == pRet->HasJob()) continue;
				else
				{
					bFind = TRUE;
					if(FALSE == pRet->IsIdle()) 
					{
						pRet->SetIdle();
					}
					m_mapDelIONode.RemoveKey(sKey);
					_LOG_DBG("=========> CSSLIONode reuse");
					break;
				}
			}
		}
		if(FALSE == bFind)
		{
			pRet = new CSSLIONode(m_nInitRecvLen);
			_LOG_DBG("=========> CSSLIONode created (case 1)");
		}
	}
	else 
	{
		pRet = new CSSLIONode(m_nInitRecvLen);
		_LOG_DBG("=========> CSSLIONode created (case 2)");
	}

	pRet->SetBusy();
	return pRet;
}

/* ----------------------------------------------------------------------------
	description : ssl accept for blocking socket descriptor
	paramentrs  : 
		- (SSL*) ssl object pointer
	return      : (SWC_BOOL) return result
	history     :
		- 2014. 05. 17 created by Park, sung soo
-----------------------------------------------------------------------------*/
SWC_BOOL CSSLHandler::SSLAcceptBlockMode(IN SSL* ssl)
{
	if(0 >= WF_SSL_accept(ssl)) return FALSE;
	return TRUE;
}

/* ----------------------------------------------------------------------------
	description : ssl accept for non-blocking socket descriptor
	paramentrs  : 
		(SSL*) ssl object pointer
	return      : (SWC_BOOL) return result
	history     :
		- 2014. 05. 17 created by Park, sung soo
-----------------------------------------------------------------------------*/
SWC_BOOL CSSLHandler::SSLAcceptNonBlockMode(IN SSL* ssl)
{
	SWC_BOOL       bRet = FALSE;
	INT            nLoopCnt = SSL_ACCEPT_NKBL_MAX_RETRY_CNT;
	struct pollfd  Events;
	SOCKET         nSfd = INVALID_SOCKET;

	while(0 < nLoopCnt)
	{
		INT nSslRet = 0, nSslErr = 0, nR = 0, nPollRet = 0;

		if(0 >= (nSslRet = WF_SSL_accept(ssl)))
		{
			nSslErr    = WF_SSL_get_error(ssl, nR);
			nSfd       = WF_SSL_get_fd(ssl);

			if(0 >= nSfd) return FALSE;

			Events.fd     = nSfd;
			Events.events = POLLIN;
			nPollRet      = poll(&Events, 1, 1000 * SSL_ACCEPT_TIMEOUT);
			
			if     (0 == nPollRet) 
			{
				_LOG_ERR("timeout occured");
				return FALSE;
			}
			else if(0 >  nPollRet) 
			{ 
				_LOG_ERR("poll() error occured. giveup"); 
				return FALSE;
			}

			switch(nSslErr)
			{
				case SSL_ERROR_NONE :
					_LOG_DBG("CSSLHandler::SSLAcceptNonBlockMode() - SSL_ERROR_NONE");
					if(TRUE == IsDMsgOn()) ERR_print_errors_fp(stderr);
					return TRUE;
				case SSL_ERROR_SSL :
					_LOG_ERR("SSL_ERROR_SSL. check OpenSSL_add_all_algorithms() is called after SSL_library_init()");
					if(TRUE == IsDMsgOn()) ERR_print_errors_fp(stderr);
					return FALSE;
				case SSL_ERROR_WANT_READ :
					_LOG_DBG("SSL_ERROR_WANT_READ. continue");
					break;
				case SSL_ERROR_WANT_WRITE :
					_LOG_DBG("SSL_ERROR_WANT_READ. continue");
					break;
				case SSL_ERROR_SYSCALL :
					{
						_LOG_ERR("SSL_ERROR_SYSCALL (errno = %d)", errno);
						if(errno == EINTR) 
						{
							_LOG_DBG("EINTR occrued");
							return FALSE;
						}
						else if(errno == EBADF)
						{
							_LOG_ERR("EBADF occured");
							return FALSE;
						}
						else if(errno == EPIPE)
						{
							_LOG_ERR("EPIPE occured");
							return FALSE;
						}
						else if(errno == EAGAIN)
						{
							_LOG_DBG("EAGAIN occured");
						}
						else if(errno == EINPROGRESS)
						{
							_LOG_DBG("EINPROGRESS occured");
						}
						else if(0 == errno)
						{
							_LOG_ERR("errno is zero. exceptional condition. (nLoopCnt=%d)", nLoopCnt);
							return FALSE;
						}
					}
					break;
				case SSL_ERROR_ZERO_RETURN :
					_LOG_ERR("SSL_ERROR_ZERO_RETURN. remote ssl machine shutdown");
					return FALSE;
				case SSL_ERROR_WANT_CONNECT :
					_LOG_DBG("SSL_ERROR_WANT_CONNECT. ssl session is connecting...");
					break;
				default :
					_LOG_ERR("unknown error");
					return FALSE;
			}
			nLoopCnt--;
		}
		else
		{
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}

//-----------------------------------------------------------------------------
#endif // __SWC_SSL_HANDLER_CPP__
