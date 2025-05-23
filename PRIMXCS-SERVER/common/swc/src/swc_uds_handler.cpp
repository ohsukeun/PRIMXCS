/*****************************************************************************
 *                                                                           *
 * File Name : swc_uds_handler.cpp                                           *
 * Theme     : unix domain socket handler object using 'epoll'               *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by PSS '12. 11. 22                                 *
 *                                                                           *
 *****************************************************************************/

/// @file swc_uds_handler.cpp 
/// @author Park, sung soo
/// @date 2012-11-22
/// @brief unix domain socket handler object using 'epoll'

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>
#include <sched.h>

#include <logger.h>
#include <swc_tool.h>
#include <swc_debug.h>
#include <swc_sock.h>
#include <swc_uds_handler.h>
#include <swc_ip_obj.h>
#include <swc_pseudo_set.h>

#ifndef __SWC_UDS_HANDLER_CPP__
#define __SWC_UDS_HANDLER_CPP__
//-----------------------------------------------------------------------------
// METHOD DEFINITIONS
//-----------------------------------------------------------------------------
/* ----------------------------------------------------------------------------
	description : CWorkTh object creator
	paramentrs  : 
	return      : 
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
CWorkTh::CWorkTh()
{
	m_pParent   = NULL;
	m_bRun      = TRUE;
	m_ThreadID  = 0;
	m_nID       = 0;
	m_bIsBusy   = FALSE;
	m_bRunCheck = FALSE;
}

/* ----------------------------------------------------------------------------
	description : CWorkTh object destroyer
	paramentrs  : 
	return      : 
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
CWorkTh::~CWorkTh()
{
	Exit();
}

/* ----------------------------------------------------------------------------
	description : CWorkTh object init.
	paramentrs  : 
		(CUDSHandler*) CUDSHandler object pointer
	return      : (SWC_BOOL)
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
SWC_BOOL CWorkTh::Init(IN CUDSHandler* pparent)
{
	SWC_BOOL bret = TRUE;

	SetParentPtr(pparent);
	return bret;
}

/* ----------------------------------------------------------------------------
	description : CWorkTh object stop 
	paramentrs  : 
	return      : (SWC_BOOL)
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
SWC_BOOL CWorkTh::Exit()
{
	SWC_BOOL bret = TRUE;
	m_bRun = FALSE;
	return bret;
}

/* ----------------------------------------------------------------------------
	description : CWorkTh object run 
	paramentrs  : 
	return      : (SWC_BOOL)
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
SWC_BOOL CWorkTh::Run()
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
	description : CWorkTh object thread function
	paramentrs  : 
		(PVOID thread function argument
	return      : (PVOID)
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
PVOID CWorkTh::WorkThreadFunc(IN PVOID arg)
{
	CWorkTh* pobj = NULL;
	unsigned long lMask = SWC_USE_CPU_CORE_NUM; // 1

	if(NULL == arg)
	{
		_LOG_ERR("thread argument is NULL");
		return NULL;
	}
	pobj = (CWorkTh*)arg;
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
		CIONode* pjob = pobj->GetParentPtr()->PopJobQueue();

		if(NULL == pjob)
		{
			_LOG_ERR("Poped Job is NULL");

			pobj->SetIdle();

			pthread_mutex_lock(&pobj->GetParentPtr()->m_Lock);
			pthread_cond_wait(&pobj->GetParentPtr()->m_Condition, &pobj->GetParentPtr()->m_Lock);
			pthread_mutex_unlock(&pobj->GetParentPtr()->m_Lock);
		}
		else
		{
			_LOG_ERR("Job Poped");

			if(TRUE == pjob->IsIdle())
			{
				pobj->SetBusy();

				if(INVALID_SOCKET != pjob->GetFd())
				{
					// run a job
					pjob->StartJob();
					pobj->RunJob(pjob);
					pjob->CompleteJob();
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
	description : CUDSHandler object creator
	paramentrs  : 
	return      : 
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
CUDSHandler::CUDSHandler()
{
	time_t tcur = 0;

	time(&tcur);
	m_qJob.SetSize(SOCK_MAX_JOB_Q_CNT); 
	m_ConnectCnt      = 0;
	m_bRun            = TRUE; 
	m_sIO.Empty();                            // no default
	m_nLimit          = SOCK_MAX_CLIENT_CNT;  // default 200
	m_nInitRecvLen    = 0;                    // no default
	m_pLstSock        = new WCUDSock();
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
	m_tLastUp         = tcur;
	m_tPrevSysChk     = tcur;

	pthread_mutex_init(&m_Lock, NULL);
	pthread_cond_init(&m_Condition, NULL);
}

/* ----------------------------------------------------------------------------
	description : CUDSHandler object destroyer
	paramentrs  : 
	return      : 
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
CUDSHandler::~CUDSHandler()
{
	Exit();
}

/* ----------------------------------------------------------------------------
	description : CUDSHandler object initionalize
	paramentrs  : 
		(const CHAR*) unix domain socket I/O file path
		(INT)         client count limit
		(INT)         initionally receive length
		(INT)         work thread count
	return      : (SWC_BOOL)
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
SWC_BOOL CUDSHandler::Init(IN const CHAR* sio, 
						   IN INT         nlimit, 
						   IN INT         nrcvlen,
						   IN INT         nwhcnt)
{
	SWC_BOOL bret = TRUE;
	SOCKET nlstfd = INVALID_SOCKET;

	if(NULL == sio)
	{
		_LOG_ERR("interface file path is null");
		return FALSE;
	}
	m_sIO = sio;
	m_sIO.Trim();
	if(0 == m_sIO.GetLength())
	{
		_LOG_ERR("empty interface file path");
		return FALSE;
	}
	m_pLstSock->Destroy(LPCSTR(m_sIO));
	SWC::swc_sleep(0, 500);
	m_pLstSock->SetIOFile(LPCSTR(m_sIO));
	if(1 > nlimit) 
	{
		_LOG_ERR("client limit is less than 1");
		return FALSE;
	}

	if     (SOCK_MAX_EPOLL_FD_CNT < nlimit) m_nLimit = SOCK_MAX_EPOLL_FD_CNT;
	else if(0 >= nlimit)                    m_nLimit = SOCK_MAX_EPOLL_FD_CNT;
	else                                    m_nLimit = nlimit;

	m_nInitRecvLen = nrcvlen;

	nlstfd = m_pLstSock->Listen();
	if(0 > nlstfd) 
	{
		_LOG_ERR("listen failed");
		return FALSE;
	}
	m_pLstSock->SetNonBlockMode(m_pLstSock->GetListenSfd());

	_LOG_DBG("Listening. sfd = %d. client limit = %u", nlstfd, m_nLimit);
	m_nEPfd = epoll_create(m_nLimit);
	m_pLstSock->SetNonBlockMode(m_nEPfd);

	if(0 >= m_nEPfd) 
	{
		_LOG_ERR("epoll_create() failed (m_nLimit = %d)", m_nLimit);
		return FALSE;
	}
	m_LstEP.events  = EPOLLIN | EPOLLET | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
	m_LstEP.data.fd = m_pLstSock->GetListenSfd();
	if(0 > epoll_ctl(m_nEPfd, EPOLL_CTL_ADD, m_pLstSock->GetListenSfd(), &m_LstEP))
	{
		_LOG_ERR("epoll_ctl() failed (%d, m_nEPfd = %d)", errno, m_nEPfd);
		return FALSE;
	}
	if(SOCK_MIN_WORK_TH_CNT > nwhcnt) m_nWorkThCount = SOCK_MIN_WORK_TH_CNT;
	else                              m_nWorkThCount = nwhcnt;
	for(INT i = 0; i < m_nWorkThCount; i++)
	{
		m_pWorkTh.SetAtGrow(i, (CWorkTh*)NULL);
	}
	return bret;
}

/* ----------------------------------------------------------------------------
	description : CUDSHandler object Run
	paramentrs  : 
	return      : (SWC_BOOL)
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
SWC_BOOL CUDSHandler::Run()
{
	SWC_BOOL bret = TRUE;

	if(0 > pthread_create(&m_ThreadID, NULL, HandlerThreadFunc, (PVOID)this))
	{
		_LOG_ERR("pthread_create() failed");
		return FALSE;
	}
	for(INT i = 0; i < m_nWorkThCount; i++)
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
	if(0 > pthread_create(&m_DeleterThreadID, NULL, DeleterThreadFunc, (PVOID)this))
	{
		_LOG_ERR("DeleterThreadFunc pthread_create() failed");
		return FALSE;
	}
	return bret;
}

/* ----------------------------------------------------------------------------
	description : CUDSHandler object Stop
	paramentrs  : 
	return      : 
	history     :
		- 2010. 03. 14 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CUDSHandler::Exit()
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
				delete (CIONode*)pptr;
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
		//  INT nstatus = 0;
		//  pthread_join(m_ThreadID, (PVOID*)nstatus);
			m_ThreadID = 0;
		}

		// close epoll file descriptor
		if(-1 != m_nEPfd) 
		{
			close(m_nEPfd);
			m_nEPfd = -1;
		}
	}
}

/* ----------------------------------------------------------------------------
	description : CUDSHandler object handler main thread function
	paramentrs  : 
		(PVOID) thread argument
	return      : (PVOID)
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
PVOID CUDSHandler::HandlerThreadFunc(IN PVOID arg)
{
	CUDSHandler* pobj = NULL;
	INT nret = 0;
	unsigned long lMask = SWC_USE_CPU_CORE_NUM; // 1
	sigset_t newset;

	if(NULL == arg)
	{
		_LOG_ERR("thread argument is NULL");
		return NULL;
	}
	pobj = (CUDSHandler*)arg;
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
			INT nerr = errno;

			if(EBADF == nerr) 
				_LOG_ERR("epoll_wait() error : m_nEPfd is not a valid descroptor (EBADF)");
			else if(EFAULT == nerr) 
				_LOG_ERR("epoll_wait() error : The memory area pointed to by events is not accessible with write permissions (EFAULT)");
			else if(EINTR == nerr)
			{
				//_LOG_ERR("epoll_wait() error : The call was interrupted by a signal handler before any of the requested events occurred or the timeout expired (EINTR)");
				SWC::swc_sleep(0, 1);
				continue;
			}
			else if(EINVAL == nerr)
				_LOG_ERR("epoll_wait() error : m_nEPfd is not an epoll file descriptor, or maxevents is less than or equal to zero");
			else
				_LOG_ERR("epoll_wait() error : %d", nerr);
		}
		// session time check 
		pobj->CheckSessionTimeout();

		// system check run
		pobj->RunCheckToAllWorkThread();

		if(NULL == pobj->m_pLstSock) break;
		if(FALSE == pobj->m_bRun) break;

		for(INT i = 0; i < nret; i++)
		{
			if(FALSE == pobj->m_bRun) break;

			// accept client
			if(EPOLLIN & pobj->m_pArrEPollEvent[i].events)
			{
				pthread_mutex_lock(&pobj->m_Lock);
				SOCKET nsfd = pobj->m_pArrEPollEvent[i].data.fd;
				pthread_mutex_unlock(&pobj->m_Lock);

				if(pobj->m_pLstSock->GetListenSfd() == nsfd)
				{
					if((UINT)pobj->m_nLimit < pobj->m_mapPtrIONode.GetCount())
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
					CIONode* pnode = pobj->FindNode(nsfd);

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
					if(pobj->m_pLstSock->GetListenSfd() == pnode->GetFd())
					{
						_LOG_ERR("system fatal error. listening port error occured. exit");
						break;
					}
					_LOG_ERR("error occured. case 1. sfd = %d", pnode->GetFd());

					pobj->DetachNode(pnode);
				}
				else
				{
					if(FALSE == pobj->m_bRun) break;
					CIONode* pnode = pobj->FindNode(nsfd);

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
					if(pobj->m_pLstSock->GetListenSfd() == pnode->GetFd())
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
					CIONode* pnode = pobj->FindNode(nsfd);

					_LOG_ERR("error occured. case 2");
					if(NULL == pnode) 
					{ 
						_LOG_DBG("pnode is null. case 2-1");
						continue; 
					}
					if(65535 >= (u_long)pnode)
					{
						_LOG_DBG("pnode pointer is smaller than 65535. case 2-2");
						continue;
					}
					if(nsfd != pnode->GetFd()) continue;
					if(pobj->m_pLstSock->GetListenSfd() == pnode->GetFd())
					{
						_LOG_DBG("pnode sfd is same with listen sfd. case 2-2");
						continue;
					}
					_LOG_ERR("error occured. case 2. sfd = %d", pnode->GetFd());

					pobj->DetachNode(pnode);
				}
				else
				{
					if(FALSE == pobj->m_bRun) break;
					CIONode* pnode = pobj->FindNode(nsfd);

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
					if(pobj->m_pLstSock->GetListenSfd() == pnode->GetFd())
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
					CIONode* pnode = pobj->FindNode(nsfd);

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
					if(pobj->m_pLstSock->GetListenSfd() == pnode->GetFd())
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
	description : CUDSHandler object handler - push a job to the job queue
	paramentrs  : 
		(CIONode*) client node
	return      :
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CUDSHandler::PushJobQueue(IN CIONode* pobj)
{
	time_t tcur = 0;

	if(NULL == pobj) return;
	pSJobContainer pjob = (pSJobContainer)malloc(sizeof(SJobContainer));
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
	description : CUDSHandler object handler - pop a job from the job queue
	paramentrs  : 
	return      : (CIONode*) return job
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
CIONode* CUDSHandler::PopJobQueue()
{
	pSJobContainer pjob = NULL;
	VOID*          pout = NULL;
	CIONode*       pret = NULL;

	if(FALSE == m_qJob.Pop(pout)) return NULL;
	if(NULL == pout) return NULL;
	pjob = (pSJobContainer)pout;
	if(NULL == pjob->_pJobNode) 
	{ 
		free(pjob); 
		return NULL; 
	}
	pret = pjob->_pJobNode; free(pjob);
	return pret;
}

/* ----------------------------------------------------------------------------
	description : CUDSHandler object handler - accept client
	paramentrs  : 
	return      : 
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CUDSHandler::AcceptClient()
{
	while(1)
	{
		if((UINT)m_nLimit <= m_mapPtrIONode.GetCount())
		{
			AcceptAndTerminateClient();
			break;
		}

		SOCKET             nclifd = INVALID_SOCKET;
		INT                nsin   = 0;
		struct sockaddr_in cliaddr;
		UCHAR              cbuf[4] = {0};

		if(NULL == m_pLstSock) return;

		if(0 > (nclifd = accept(m_pLstSock->GetListenSfd(), 
								(struct sockaddr*)&cliaddr,
								(socklen_t*)&nsin)))
		{
			return;
		}
		memcpy(cbuf, &cliaddr.sin_addr.s_addr, 4);
		_LOG_MSG("listen fd = %d / fd = %d", m_pLstSock->GetListenSfd(), nclifd);
		INT nsize = 30000;
		setsockopt(nclifd, SOL_SOCKET, SO_SNDBUF, (CHAR*)&nsize, sizeof(nsize));
		setsockopt(nclifd, SOL_SOCKET, SO_RCVBUF, (CHAR*)&nsize, sizeof(nsize));

		m_pLstSock->SetNonBlockMode(nclifd);

		RegAcceptedIP(nclifd);

		CIONode* pnode = AddNode(nclifd);
		if(NULL == pnode) return;
		RegisterNode(pnode);
		m_ConnectCnt++;
		PushJobQueue(pnode);
		SWC::swc_sleep(0, 10);
	}
}

/* ----------------------------------------------------------------------------
	description : CUDSHandler object handler - registering node to the epoll
	paramentrs  : 
		(CIONode*) io node object pointer
	return      :
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CUDSHandler::RegisterNode(IN CIONode* pnode)
{
	if(NULL == pnode) return;
	if(NULL == m_pLstSock) return;

	struct epoll_event cli_event;

	m_pLstSock->SetNonBlockMode(pnode->GetFd());
	memset(&cli_event, 0x00, sizeof(struct epoll_event));
	cli_event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLERR | EPOLLHUP;
	cli_event.data.fd = pnode->GetFd();
	if(0 > epoll_ctl(m_nEPfd, EPOLL_CTL_ADD, pnode->GetFd(), &cli_event))
	{
		_LOG_ERR("epoll_ctl() failed (SFD = %d)", pnode->GetFd());
		if(EBADF != errno)
		{
			SOCKET nDelSfd = pnode->GetFd();
			pnode->Close();
			DeleteNode(nDelSfd);
		}
		return;
	}
}

/* ----------------------------------------------------------------------------
	description : CUDSHandler object handler - detach node from epoll
	paramentrs  : 
		(CIONode*) io node object pointer
	return      : 
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CUDSHandler::DetachNode(IN CIONode* pnode)
{
	if(NULL == pnode) return;
	if(0 >= pnode->GetFd()) return;

	SOCKET nsfd = pnode->GetFd();

	pnode->Close();

	_LOG_MSG("node detached (sfd = %d)", nsfd);
	if((nsfd == m_nEPfd) || (nsfd == m_pLstSock->GetListenSfd()))
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

		if((m_nEPfd != nsfd) && (m_pLstSock->GetListenSfd() != nsfd))
		{
			epoll_ctl(m_nEPfd, EPOLL_CTL_DEL, nsfd, &cli_event);
		}
	}
	DeleteNodeFromJobQueue(pnode);
	if(0 < m_ConnectCnt) m_ConnectCnt--;

	DeleteNode(nsfd);
}

/* ----------------------------------------------------------------------------
	description : CUDSHandler object handler - enable EPOLLONESHOT
	paramentrs  : 
		(CIONode*) io node object pointer
	return      :
	history     :
		- 2010. 05. 02 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CUDSHandler::SetEnableOneShotNode(IN CIONode* pnode)
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
	description : CUDSHandler object handler - disable EPOLLONESHOT
	paramentrs  : 
		(CIONode*) io node object pointer
	return      :
	history     :
		- 2010. 05. 02 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CUDSHandler::SetDisableOneShotNode(IN CIONode* pnode)
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
	description : CUDSHandler object handler - find CIONode from m_mapPtrIONode
	paramentrs  : 
		(SOCKET) client socket descriptor
	return      : (CIONode*) return find CIONode
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
CIONode* CUDSHandler::FindNode(IN SOCKET nsfd)
{
	CIONode* pret = NULL;
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
	pret = (CIONode*)pout;
	m_mapPtrIONode.Unlock();
	return pret;
}

/* ----------------------------------------------------------------------------
	description : CUDSHandler object handler - add new CIONOde to m_mapPtrIONode
	paramentrs  : 
		(SOCKET) client socket descriptor
	return      : (CIONode*) return find CIONode
	history     :
		- 2010. 03. 15 created by Park, sung soo
-----------------------------------------------------------------------------*/
CIONode* CUDSHandler::AddNode(IN SOCKET nsfd)
{
	CIONode* pret = NULL;
	WCString skey;
	PVOID    pout = NULL;

	skey.Format("%d", nsfd);
	m_mapPtrIONode.Lock();
	if(FALSE == m_mapPtrIONode.Lookup(skey, pout))
	{
		pret = new CIONode(m_nInitRecvLen);
		pret->SetFd(nsfd);
		pret->InitAsRecv(m_nInitRecvLen);
		m_mapPtrIONode.SetAt(skey, (PVOID)pret);
	}
	else
	{
		pret = (CIONode*)pout;
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
	return      : (void)
	history     :
		- 2012. 07. 26 created by Park, sung soo
-----------------------------------------------------------------------------*/
void CUDSHandler::DeleteNode(IN SOCKET nsfd, IN SWC_BOOL bAddToDeleteMap)
{
	WCString skey;
	CIONode* pFind = NULL;
	PVOID pout = NULL;

	skey.Format("%d", nsfd);

	m_mapPtrIONode.Lock();
	m_mapPtrIONode.Lookup(skey, pout);
	if(NULL != pout)
	{
		pFind = (CIONode*)pout;
		m_mapPtrIONode.RemoveKey(skey);
		if(TRUE == bAddToDeleteMap) AddToDeleleMap(pFind);
	}
	m_mapPtrIONode.Unlock();
	_LOG_MSG("node count = %u", m_mapPtrIONode.GetCount());
}

/* ----------------------------------------------------------------------------
	description : delete job node from job queue given I/O node
	paramentrs  : 
		(CIONode*) io node object pointer
	return      :
	history     :
		- 2010. 05. 05 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CUDSHandler::DeleteNodeFromJobQueue(IN CIONode* pnode)
{
	UINT nQSize = m_qJob.m_Queue.size();
	_LOG_DBG("enter - m_qJob.m_Queue.size() = %u", nQSize);

	if(0 == nQSize) return;
	m_qJob.Lock();
	while(1)
	{
		UINT i = 0, qsize = m_qJob.m_Queue.size();
		pSJobContainer pdel = NULL;

		if(0 == qsize) break;
		for(i = 0; i < qsize; i++)
		{
			pSJobContainer pcur = (pSJobContainer)m_qJob.m_Queue[i];
			if(NULL != pcur)
			{
				if(pcur->_pJobNode == pnode)
				{
					pdel = pcur;
					break;
				}
			}
		}
		if((NULL != pdel) && (i > 0))
		{
			m_qJob.m_Queue.erase(m_qJob.m_Queue.begin() + i);
			delete pdel;
		}
		else break;
	}
	m_qJob.Unlock();
}

/* ----------------------------------------------------------------------------
	description : check session timeout
	paramentrs  : 
		(CIONode*) io node object pointer
	return      :
	history     :
		- 2010. 05. 05 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CUDSHandler::CheckSessionTimeout()
{
	time_t tcur = 0;
	WCArray<CIONode*> arrTimeOut;

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
					CIONode* pnode = (CIONode*)pptr;

					if(0 == pnode->GetTimeLastIO())
					{
						pnode->UpdateTimeLastIO();
					}
					else if((UINT)m_OptProperty._IOTimeout <= (UINT)abs(tcur - pnode->GetTimeLastIO()))
					{
						arrTimeOut.Add(pnode);
					}
				}
				SWC::swc_sleep(0, 10);
			}

			if(0 < arrTimeOut.GetCount())
			{
				for(UINT nID = 0; nID < arrTimeOut.GetCount(); nID++)
				{
					CIONode* pDel = arrTimeOut[nID];
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
VOID CUDSHandler::WaitJobCompleted()
{
	while(1)
	{
		SWC_BOOL bBusy = FALSE;

		for(INT i = 0; i < m_nWorkThCount; i++)
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
VOID CUDSHandler::RenewalLstEpoll()
{
	if(0 > epoll_ctl(m_nEPfd, EPOLL_CTL_DEL, m_pLstSock->GetListenSfd(), &m_LstEP))
	{
		_LOG_ERR("epoll_ctl() delete failed");
		return;
	}
	m_LstEP.events = 0;
	m_LstEP.events  = EPOLLIN | EPOLLET | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
	m_LstEP.data.fd = m_pLstSock->GetListenSfd();
	if(0 > epoll_ctl(m_nEPfd, EPOLL_CTL_ADD, m_pLstSock->GetListenSfd(), &m_LstEP))
	{
		_LOG_ERR("epoll_ctl() add failed");
		return;
	}
	_LOG_DBG("operated");
}

/* ----------------------------------------------------------------------------
	description : check and add or update last I/O time if IP map is valid pointer
	paramentrs  : 
		(SOCKET) socket descriptor
	return      :
	history     :
		- 2010. 09. 23 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CUDSHandler::RegAcceptedIP(IN SOCKET nsfd)
{
	if(NULL == m_pMapIP) return;
	if(0 >= nsfd) return;
	UINT lip = SWC::swc_get_peer_ip(nsfd);
	if(0 < lip) 
	{
		m_pMapIP->Lock();

		WCString skey;
		VOID* pout = NULL;

		skey.Format("%u", lip);
		m_pMapIP->Lookup(skey, pout);
		if(NULL != pout)
		{
			WCIP* pfind = (WCIP*)pout;

			pfind->SetUpdateTime();

			_LOG_DBG("IP - %s / SOCKET - %d updated", pfind->GetSIP(), nsfd);
		}
		else
		{
			SWC_SAFE(WCIP* pnew = new WCIP);

			pnew->SetIP(lip);
			pnew->SetUpdateTime();
			m_pMapIP->SetAt(skey, (VOID*)pnew);

			_LOG_DBG("IP - %s / SOCKET - %d add", pnew->GetSIP(), nsfd);
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
VOID CUDSHandler::UpdateLastUpTime()
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
-----------------------------------------------------------------------------*/
VOID CUDSHandler::AcceptAndTerminateClient()
{
	while(1)
	{
		SOCKET             nclifd = INVALID_SOCKET;
		INT                nsin   = 0;
		struct sockaddr_in cliaddr;

		if(NULL == m_pLstSock) return;
		if(0 > (nclifd = accept(m_pLstSock->GetListenSfd(),
								(struct sockaddr*)&cliaddr,
								(socklen_t*)&nsin)))
		{
			return;
		}
		if((0 < nclifd) && (nclifd != m_pLstSock->GetListenSfd()))
		{
			CIONode* pnode = FindNode(nclifd);

			if(NULL != pnode)
			{
				DetachNode(pnode);
			}
			else
			{
				m_pLstSock->SetNonBlockMode(nclifd);

				struct linger tmp = {1, 0};
				setsockopt(nclifd, SOL_SOCKET, SO_LINGER, (CHAR*)&tmp, sizeof(struct linger));
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
		- 2014. 03. 18 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CUDSHandler::RunCheckToAllWorkThread()
{
	time_t tcur = 0;

	time(&tcur);
	if( (0 < m_tPrevSysChk) && (SWC_SYSTEM_CHECK_ENABLE & m_OptProperty._OptFlag) &&
		(SYSTEM_CHECK_MIN_INTERVAL < m_OptProperty._SysChkItvl))
	{
		time_t tGap = abs(tcur - m_tPrevSysChk);
		if(m_OptProperty._SysChkItvl <= tGap)
		{
			for(UINT i = 0; i < m_pWorkTh.GetCount(); i++)
			{
				CWorkTh* pWork = m_pWorkTh[i];
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
VOID CUDSHandler::AddToDeleleMap(IN CIONode* pdel)
{
	if(NULL == pdel) return;
	WCString sKey;
	PVOID    pout = NULL;

	sKey.Format("%lu", (UINT64)pdel);
	if(false == m_mapDelIONode.Lookup(sKey, pout))
	{
		if(FALSE == pdel->HasJob())
		{
			pdel->SetIdle();
		}
		m_mapDelIONode.SetAt(sKey, (PVOID)pdel);
	}
}

/* ----------------------------------------------------------------------------
	description : check status and delete io node on the deleter map
	paramentrs  : 
	return      : 
	history     :
		- 2013. 05. 03 created by Park, sung soo
-----------------------------------------------------------------------------*/
VOID CUDSHandler::CheckAndDeleteIONode()
{
	WCArray< CPairContainer<WCString, CIONode*> > arrDel;

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
				CIONode* pNode = (CIONode*)pout;
				if(TRUE == pNode->IsIdle())
				{
					CPairContainer<WCString, CIONode*> objDel;
					WCString sDelKey;

					sDelKey.Format("%s", LPCSTR(sKey));
					objDel.SetData(sDelKey, pNode);

					arrDel.Add(objDel);
				}
			}
			SWC::swc_sleep(0, 10);
		}

		if(0 < arrDel.GetCount())
		{
			for(UINT nID = 0; nID < arrDel.GetCount(); nID++)
			{
				CIONode* pDel = arrDel[nID].GetSecond();
				if(NULL != pDel)
				{
					pthread_mutex_lock(&m_Lock);
					delete pDel;
					pthread_mutex_unlock(&m_Lock);
				}
				m_mapDelIONode.RemoveKey(arrDel[nID].GetFirst());

				_LOG_MSG("IO-node pointer %s deleted (%u remained. %u connected)", 
					LPCSTR(arrDel[nID].GetFirst()), 
					m_mapDelIONode.GetCount(), 
					m_mapPtrIONode.GetCount());

			}
			arrDel.RemoveAll();
		}
	}
}

/* ----------------------------------------------------------------------------
	description : deleter thread function
	paramentrs  : 
		(PVOID) thread argument
	return      : 
	history     :
		- 2013. 05. 03 created by Park, sung soo
-----------------------------------------------------------------------------*/
PVOID CUDSHandler::DeleterThreadFunc(IN PVOID arg)
{
	CUDSHandler* pobj = NULL;
	unsigned long lMask = SWC_USE_CPU_CORE_NUM; // 1

	if(NULL == arg) return NULL;
	pobj = (CUDSHandler*)arg;
	pobj->m_DeleterThreadID = pthread_self();
	if(0 > pthread_setaffinity_np(pthread_self(), sizeof(lMask), (cpu_set_t*)&lMask))
	{
		_LOG_ERR("pthread_setaffinity_np() failed");
		return NULL;
	}
	pthread_detach(pobj->m_DeleterThreadID);

	while(TRUE == pobj->m_bRun)
	{
		SWC::swc_sleep(0, 100000);
		pobj->CheckAndDeleteIONode();
	}

	pthread_exit((PVOID)NULL);
	return NULL;
}

//-----------------------------------------------------------------------------
#endif // __SWC_UDS_HANDLER_CPP__
