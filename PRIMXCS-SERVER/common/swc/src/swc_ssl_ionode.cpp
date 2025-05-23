/*****************************************************************************
 *                                                                           *
 * File Name : swc_ssl_ionode.cpp                                            *
 * Theme     : ssl sock io-node                                              *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '10. 04. 13                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_ssl_ionode.cpp
/// @author Park, sung soo
/// @date 2010-04-13
/// @brief ssl sock io-node 

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/poll.h>

#include <logger.h>
#include <swc_tool.h>
#include <swc_debug.h>
#include <swc_ssl_ionode.h>

#ifndef __SWC_SSL_IONODE_CPP__
#define __SWC_SSL_IONODE_CPP__

//-----------------------------------------------------------------------------
// LOCAL DEFINITIONS
//-----------------------------------------------------------------------------
// X : base timeout. Y : buffer length
#define SWC_IO_TIMEOUT_CALC(X, Y)  ( X + (INT)round( Y / (1024 * 1024)) ) 

//-----------------------------------------------------------------------------
// METHOD DEFINITIONS
//-----------------------------------------------------------------------------
/* ----------------------------------------------------------------------------
    description : creator
    parameters  :
    return      :
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
CSSLIONode::CSSLIONode()
{
	m_lID             = 0;
	m_llID            = 0;
	m_lIP             = 0;
	memset(m_cMAC, 0x00, 6);
	memset(m_sMAC, 0x00, 13);
	memset(m_sKey, 0x00, SOCK_IO_NODE_KEY_LEN);
	m_pPtr            = NULL;
	m_lUserVal        = 0;
	m_nSfd            = INVALID_SOCKET;
	m_nRecvLen        = 0;
	m_nSendLen        = 0;
	m_bRecvMode       = FALSE;
	m_bSendMode       = FALSE;
	m_pRecv           = NULL;
	m_pSend           = NULL;
	m_pTempBuf        = NULL;
	m_pRecvBack       = NULL;
	m_pSendBack       = NULL;
	m_nRecvPos        = 0;
	m_nSendPos        = 0;
	m_bWorking        = FALSE;
	m_bHasJob         = FALSE;
	m_nTempBufLen     = 0;
	m_bHeadRcvd       = FALSE;
	m_nRecvAllocLen   = 0;
	m_nSendAllocLen   = 0;
	time(&m_tConnect);
	m_tLastIO         = 0;
	m_pSSL            = NULL;
	m_bIsFileSendMode = FALSE;
	m_pFile           = NULL;
	m_nFTotalSize     = 0;
	m_nFTotalBlock    = 0;
	m_nFBlockSize     = 0;
	m_nFRestSize      = 0;
	m_nFSentBytes     = 0;
	m_nFSentBytes     = 0;
	m_nFSendingBytes  = 0;
	m_pFBuf           = NULL;
	m_nRecvTimeOut    = SOCK_RECV_TIMEOUT;
	m_nRecvTimeSpend  = 0;
	m_tRecvPrev       = 0;
	pthread_mutex_init(&m_Lock, NULL);
}

/* ----------------------------------------------------------------------------
	description : creator
	parameters  :
		- (UINT) init. receiving buffer length
	return      :
	history     :
		- 2013. 01. 14 created by Park. Sung soo
---------------------------------------------------------------------------- */
CSSLIONode::CSSLIONode(UINT nbuflen)
{
	m_lID             = 0;
	m_llID            = 0;
	m_lIP             = 0;
	memset(m_cMAC, 0x00, 6);
	memset(m_sMAC, 0x00, 13);
	memset(m_sKey, 0x00, SOCK_IO_NODE_KEY_LEN);
	m_pPtr            = NULL;
	m_lUserVal        = 0;
	m_nSfd            = INVALID_SOCKET;
	m_nRecvLen        = 0;
	m_nSendLen        = 0;
	m_bRecvMode       = FALSE;
	m_bSendMode       = FALSE;
	m_pRecv           = NULL;
	m_pSend           = NULL;
	m_pTempBuf        = (CHAR*)malloc(nbuflen);
	m_pRecvBack       = NULL;
	m_pSendBack       = NULL;
	m_nRecvPos        = 0;
	m_nSendPos        = 0;
	m_bWorking        = FALSE;
	m_bHasJob         = FALSE;
	m_nTempBufLen     = nbuflen;
	m_bHeadRcvd       = FALSE;
	m_nRecvAllocLen   = 0;
	m_nSendAllocLen   = 0;
	time(&m_tConnect);
	m_tLastIO         = 0;
	m_pSSL            = NULL;
	m_bIsFileSendMode = FALSE;
	m_pFile           = NULL;
	m_nFTotalSize     = 0;
	m_nFTotalBlock    = 0;
	m_nFBlockSize     = 0;
	m_nFRestSize      = 0;
	m_nFSentBytes     = 0;
	m_nFSentBytes     = 0;
	m_nFSendingBytes  = 0;
	m_pFBuf           = NULL;
	m_nRecvTimeOut    = SOCK_RECV_TIMEOUT;
	m_nRecvTimeSpend  = 0;
	m_tRecvPrev       = 0;
	pthread_mutex_init(&m_Lock, NULL);
}

/* ----------------------------------------------------------------------------
    description : destroyer
    parameters  :
    return      :
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
CSSLIONode::~CSSLIONode()
{
	m_lID             = 0;
	m_llID            = 0;
	m_lIP             = 0;
	memset(m_cMAC, 0x00, 6);
	memset(m_sMAC, 0x00, 13);
	memset(m_sKey, 0x00, SOCK_IO_NODE_KEY_LEN);
	if(NULL != m_pPtr) m_pPtr = NULL;
	Close(TRUE);
	m_nRecvLen        = 0;
	m_nSendLen        = 0;
	m_bRecvMode       = FALSE;
	m_bSendMode       = FALSE;
	if(NULL != m_pRecvBack)
	{
		free(m_pRecvBack); m_pRecvBack = NULL;

		CHAR* ptemp = (CHAR*)m_pTempBuf;
		if(NULL != ptemp) free(ptemp);
	}
	if(NULL != m_pSendBack) { free(m_pSendBack); m_pSendBack = NULL; }
	m_nRecvPos        = 0;
	m_nSendPos        = 0;
	m_bWorking        = FALSE;
	m_nTempBufLen     = 0;
	m_bIsFileSendMode = FALSE;
	if(NULL != m_pFile) { fclose(m_pFile); m_pFile = NULL; } 
	m_nFTotalSize     = 0;
	m_nFTotalBlock    = 0;
	m_nFBlockSize     = 0;
	m_nFRestSize      = 0;
	m_nFSentBytes     = 0;
	m_nFSendingBytes  = 0;
	m_pFBuf           = NULL;
	m_pXmlBuf.Empty();
	if(NULL != m_pSSL)
	{
		WF_SSL_shutdown(m_pSSL);
		WF_SSL_free(m_pSSL);
		m_pSSL = NULL;
	}
	pthread_mutex_destroy(&m_Lock);
}

/* ----------------------------------------------------------------------------
    description : close socket
    parameters  :
		(SWC_BOOL) close socket or not
    return      :
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::Close(SWC_BOOL bClose)
{
	Lock();
	if((INVALID_SOCKET == m_nSfd) || (0 == m_nSfd)) { Unlock(); return;}
	if(FALSE == bClose) 
	{
		if(NULL != m_pSSL) { WF_SSL_free(m_pSSL); m_pSSL = NULL; }
		m_nSfd = INVALID_SOCKET;
		m_bWorking = FALSE; 
		Unlock();
		return;
	}

	SOCKET nclifd = INVALID_SOCKET;
	if((0 == m_nSfd) || (65535 <= m_nSfd))
	{
		if(NULL != m_pSSL) { WF_SSL_free(m_pSSL); m_pSSL = NULL; }
		m_nSfd = INVALID_SOCKET;
		m_bWorking = FALSE; 
		Unlock();
		return;
	}

	if(NULL != m_pSSL)
	{
		INT nret = 0;

		if(SSL_RECEIVED_SHUTDOWN == WF_SSL_get_shutdown(m_pSSL))
		{
			nret = WF_SSL_shutdown(m_pSSL);
		}
		else if(TLS_ST_OK == WF_SSL_get_state(m_pSSL))
		{
			nret = WF_SSL_shutdown(m_pSSL);
		}
		if(0 == nret)
		{
			struct pollfd Events;
			INT nSslErr = 0, nR = 0, nPollRet = 0, nLoopCnt = 0;

			while(1)
			{
				SWC_BOOL bPoll = TRUE;

				nSslErr = SSL_get_error(m_pSSL, nR);
				nclifd = WF_SSL_get_fd(m_pSSL);
				if(0 >= nclifd) break;
				Events.fd = nclifd;

				switch(nSslErr)
				{
					case SSL_ERROR_WANT_CONNECT :
					case SSL_ERROR_WANT_ACCEPT  :
					case SSL_ERROR_WANT_READ    :
						Events.events = POLLIN;
						break;
					case SSL_ERROR_WANT_WRITE :
						Events.events = POLLOUT;
						break;
					default :
						bPoll = FALSE;
						break;
				}	
				if(TRUE == bPoll)
				{
					nPollRet = poll(&Events, 1, 1000);
					if(0 >= nPollRet) break; 
				}			
				nret = WF_SSL_shutdown(m_pSSL);
				if(1 == nret) break; 
				else if(-1 == nret) break; 
				
				nLoopCnt++;
				if(10 < nLoopCnt) break;
				SWC::swc_sleep(0, 10000); 
			}
		}
		if(0 < nclifd)
		{
			struct linger tmp = {1, 0};
			CHAR _tbuf[1024];

			setsockopt(nclifd, SOL_SOCKET, SO_LINGER, (CHAR*)&tmp, sizeof(struct linger));
			shutdown(nclifd, SHUT_RDWR);
			read(nclifd, _tbuf, 1024);
			close(nclifd);
		}
		else
		{
			struct linger tmp = {1, 0};
			CHAR _tbuf[1024];

			setsockopt(m_nSfd, SOL_SOCKET, SO_LINGER, (CHAR*)&tmp, sizeof(struct linger));
			shutdown(m_nSfd, SHUT_RDWR);
			read(m_nSfd, _tbuf, 1024);
			close(m_nSfd);
		}
		WF_SSL_free(m_pSSL);
		m_pSSL = NULL;
	}
	else
	{
		struct linger tmp = {1, 0};
		CHAR _tbuf[1024];

		setsockopt(m_nSfd, SOL_SOCKET, SO_LINGER, (CHAR*)&tmp, sizeof(struct linger));
		shutdown(m_nSfd, SHUT_RDWR);
		read(m_nSfd, _tbuf, 1024);
		close(m_nSfd);
		m_pSSL = NULL;
	}
	if((0 < m_nSfd) && (nclifd != m_nSfd))
	{
		Reset();
	}
	m_nSfd = INVALID_SOCKET;
	ClrFileSendMode();
	if(0 != GetUserValue()) SetUserValue(0);
	SetTimeZeroLastIO();
	m_bWorking = FALSE; 
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : reset object
    parameters  :
    return      : 
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::Reset()
{
	ResetRecv();
	ResetSend();
	SetConnectTime(0);
}

SOCKET CSSLIONode::GetFd()
{
	SOCKET nret = INVALID_SOCKET;
	Lock();
	nret = m_nSfd;
	Unlock();
	return nret;
}

VOID CSSLIONode::SetFd(SOCKET nsfd)
{
	Lock();
	m_nSfd = nsfd;
	Unlock();
	UpdateTimeLastIO();
}

SSL* CSSLIONode::GetSSL()
{
	SSL* pret = NULL;
	Lock();
	pret = m_pSSL;
	Unlock();
	return pret;
}

VOID CSSLIONode::SetSSL(SSL* pssl)
{
	Lock();
	m_pSSL = pssl;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : reset receiving envrionment
    parameters  :
    return      : 
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::ResetRecv()
{
	m_nRecvLen = m_nRecvPos = 0;
	m_bRecvMode = FALSE;
	m_bHeadRcvd = FALSE;
	m_nRecvTimeSpend = 0;
	m_tRecvPrev = 0;
}

/* ----------------------------------------------------------------------------
    description : reset sending envronment
    parameters  :
    return      :
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::ResetSend()
{
	m_nSendLen = m_nSendPos = 0;
	m_bSendMode = FALSE; 
	m_bRecvMode = TRUE;
}

/* ----------------------------------------------------------------------------
    description : allocate temorary receiving buffer
    parameters  :
		(INT) temporary buffer length
    return      :
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetTempBufAlloc(INT nlen)
{
	if(0 >= nlen) return;
/*
	if(nlen > m_nTempBufLen) 
	{
		if(0 == m_nTempBufLen)
		{
			m_nTempBufLen = nlen;
			m_pTempBuf = (CHAR*)malloc(m_nTempBufLen);
		}
		else
		{
			m_nTempBufLen = nlen;
			m_pTempBuf = (CHAR*)realloc(m_pTempBuf, m_nTempBufLen);
			memset(m_pTempBuf, 0x00, m_nTempBufLen);
		}
	}
	else
	{
		if(NULL == m_pTempBuf)
		{
			m_pTempBuf = (CHAR*)malloc(nlen);
		}
		memset(m_pTempBuf, 0x00, nlen);
	}
*/
	m_nTempBufLen = nlen;
	if(NULL != m_pTempBuf)
	{
		memset((CHAR*)m_pTempBuf, 0x00, nlen);
	}
}

/* ----------------------------------------------------------------------------
    description : set send mode
    parameters  :
		(VOID*) sending buffer
		(INT)   sending buffer length
    return      : (SWC_BOOL)
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CSSLIONode::SetSendMode(VOID* pbuf, INT nlen)
{
	if((NULL == pbuf) || (0 >= nlen)) return FALSE;
	SWC_BOOL bsuccess = TRUE;

	Lock();
	if((size_t)nlen > m_nSendAllocLen)
	{
		if(0 == m_nSendAllocLen)
		{
			CHAR* ptemp = NULL;

			m_nSendAllocLen = (size_t)nlen;
			if(NULL != m_pSendBack) free(m_pSendBack);
			ptemp = (CHAR*)malloc(m_nSendAllocLen);
			if(NULL != ptemp)
			{
				m_pSend = m_pSendBack = ptemp;
			}
			else
			{
				_LOG_ERR("malloc() failed - case 1");
				m_pSend = m_pSendBack = NULL;
				m_nSendAllocLen = 0;
				bsuccess = FALSE;
			}
		}
		else
		{
			CHAR* ptemp = NULL;

			if(NULL == m_pSendBack)
			{
				m_nSendAllocLen = (size_t)nlen;
				if(NULL != m_pSendBack) free(m_pSendBack);
				ptemp = (CHAR*)malloc(m_nSendAllocLen);

				if(NULL != ptemp)
				{
					m_pSend = m_pSendBack = ptemp;
				}
				else
				{
					_LOG_ERR("malloc() failed - case 2");
					m_pSend = m_pSendBack = NULL;
					m_nSendAllocLen = 0;
					bsuccess = FALSE;
				}
			}
			else
			{
				m_nSendAllocLen = (size_t)nlen;
				ptemp = (CHAR*)realloc(m_pSendBack, m_nSendAllocLen);

				if(NULL != ptemp)
				{
					m_pSend = m_pSendBack = ptemp;
				}
				else
				{
					_LOG_ERR("realloc() failed");
					if(NULL != m_pSendBack) free(m_pSendBack);
					m_pSend = m_pSendBack = NULL;
					m_nSendAllocLen = 0;
					bsuccess = FALSE;
				}
			}
		}
	}
	else if((size_t)nlen == m_nSendAllocLen)
	{
		if(NULL == m_pSendBack)
		{
			CHAR* ptemp = NULL;

			ptemp = (CHAR*)malloc(m_nSendAllocLen);
			if(NULL != ptemp)
			{
				m_pSend = m_pSendBack = ptemp;
			}
			else
			{
				_LOG_ERR("malloc() failed - case 3");
				m_pSend = m_pSendBack = NULL;
				m_nSendAllocLen = 0;
				bsuccess = FALSE;
			}
		}
	}
	else // (size_t)nlen < m_nSendAllocLen)
	{
		m_nSendAllocLen = (size_t)nlen;

		if(NULL != m_pSendBack) free(m_pSendBack);
		CHAR* ptemp = (CHAR*)malloc(m_nSendAllocLen);

		if(NULL != ptemp)
		{
			m_pSend = m_pSendBack = ptemp;
		}
		else
		{
			_LOG_ERR("malloc() failed - case 4");
			m_pSend = m_pSendBack = NULL;
			m_nSendAllocLen = 0;
			bsuccess = FALSE;
		}
	}
	if(TRUE == bsuccess)
	{
		memcpy(m_pSend, pbuf, nlen);
		m_nSendLen       = nlen;
		m_nSendPos       = 0;
		m_bSendMode      = TRUE;
		m_bRecvMode      = FALSE;
		m_nRecvTimeSpend = 0;
		m_tRecvPrev      = 0;
	}
	Unlock();
	return bsuccess;
}

/* ----------------------------------------------------------------------------
    description : set receive mode
    parameters  :
		(INT)   receiving buffer length
    return      : (SWC_BOOL)
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CSSLIONode::SetRecvMode(INT nlen)
{
	if(0 >= nlen) return FALSE;
	if(SWC_MAX_RECV_BUF_LEN < nlen) return FALSE;

	SWC_BOOL bsuccess = TRUE;

	Lock();
	if((size_t)nlen > m_nRecvAllocLen)
	{
		if(0 == m_nRecvAllocLen)
		{
			CHAR* ptemp = NULL;

			m_nRecvAllocLen = (size_t)nlen;
			if(NULL != m_pRecvBack) free(m_pRecvBack);
			ptemp = (CHAR*)malloc(m_nRecvAllocLen);
			if(NULL != ptemp)
			{
				m_pRecv = m_pRecvBack = ptemp;
			}
			else
			{
				_LOG_ERR("malloc() failed - case 1");
				m_pRecv = m_pRecvBack = NULL;
				m_nRecvAllocLen = 0;
				bsuccess = FALSE;
			}
		}
		else
		{
			CHAR* ptemp = NULL;

			if(NULL == m_pRecvBack)
			{
				m_nRecvAllocLen = (size_t)nlen;
				if(NULL != m_pRecvBack) free(m_pRecvBack);
				ptemp = (CHAR*)malloc(m_nRecvAllocLen);

				if(NULL != ptemp)
				{
					m_pRecv = m_pRecvBack = ptemp;
				}
				else
				{
					_LOG_ERR("malloc() failed - case 2");
					m_pRecv = m_pRecvBack = NULL;
					m_nRecvAllocLen = 0;
					bsuccess = FALSE;
				}
			}
			else
			{
				m_nRecvAllocLen = (size_t)nlen;
				ptemp = (CHAR*)realloc(m_pRecvBack, m_nRecvAllocLen);

				if(NULL != ptemp)
				{
					m_pRecv = m_pRecvBack = ptemp;
				}
				else
				{
					_LOG_ERR("realloc() failed");
					m_pRecv = m_pRecvBack = NULL;
					m_nRecvAllocLen = 0;
					bsuccess = FALSE;
				}
			}
		}
	}
	else if((size_t)nlen == m_nRecvAllocLen)
	{
		if(NULL == m_pRecvBack)
		{
			CHAR* ptemp = NULL;

			ptemp = (CHAR*)malloc(m_nRecvAllocLen);
			if(NULL != ptemp)
			{
				m_pRecv = m_pRecvBack = ptemp;
			}
			else
			{
				_LOG_ERR("malloc() failed - case 3");
				m_pRecv = m_pRecvBack = NULL;
				m_nRecvAllocLen = 0;
				bsuccess = FALSE;
			}
		}
	}
	else // (size_t)nlen < m_nRecvAllocLen
	{
		m_nRecvAllocLen = (size_t)nlen;

		if(NULL != m_pRecvBack) free(m_pRecvBack);
		CHAR* ptemp = (CHAR*)malloc(m_nRecvAllocLen);

		if(NULL != ptemp)
		{
			m_pRecv = m_pRecvBack = ptemp;
		}
		else
		{
			_LOG_ERR("malloc() failed - case 4");
			m_pRecv = m_pRecvBack = NULL;
			m_nRecvAllocLen = 0;
			bsuccess = FALSE;
		}
	}
	if(TRUE == bsuccess)
	{
		memset(m_pRecvBack, 0x00, nlen);
		m_pRecv          = m_pRecvBack;
		m_nRecvLen       = nlen;
		m_nRecvPos       = 0;
		m_bRecvMode      = TRUE;
		m_bSendMode      = FALSE;
		m_nRecvTimeSpend = 0;
		m_tRecvPrev      = 0;
	}
	Unlock();
	return bsuccess;
}

/* ----------------------------------------------------------------------------
    description : init. as receive mode (header receive)
    parameters  :
		(INT)   receiving buffer length
    return      :
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::InitAsRecv(INT nlen)
{
	Lock();
	SetTempBufAlloc(nlen);
	m_pRecv          = (CHAR*)m_pTempBuf;
	m_nRecvLen       = nlen;
	m_nRecvPos       = 0;
	m_bRecvMode      = TRUE;
	m_bHeadRcvd      = FALSE;
	m_pXmlBuf.Empty();
	m_nRecvTimeSpend = 0;
	m_tRecvPrev      = 0;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : write buffer
    parameters  :
		(VOID*) writting buffer 
		(INT)   buffer length
    return      : 
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CSSLIONode::Write(VOID* buf, INT len)
{
	INT nret = 0;
	if(NULL == buf) return 0;
	if(0 > GetFd()) return D_SOCK_ECLOSE;
	if(NULL == GetSSL()) return D_SOCK_ECLOSE;
	if(0 >= len) return 0;
	Lock();
	if(NULL != m_pSSL)
	{
		nret = WF_SSL_write(m_pSSL, buf, len);
		if(0 < nret) UpdateTimeLastIO();
	}
	Unlock();
	return nret;
}

/* ----------------------------------------------------------------------------
    description : write buffer with timeout
    parameters  :
		- (VOID*)    writting buffer 
		- (INT)      buffer length
		- (INT)      timtout
		- (SWC_BOOL) increase timeout considering buffer length (increase one second per 1 Mb)
    return      :
    history     :
        - 2010. 05. 10 created by Park. Sung soo
		- 2023. 06. 06 WF_SSL_set_mode() added
---------------------------------------------------------------------------- */
INT CSSLIONode::Write(VOID* buf, INT len, INT timeout, SWC_BOOL bFlexTimeOut)
{
	time_t prev_time = 0, cur_time = 0;
	INT nsend = 0, remain = 0, tsum = 0, r = 0;
	INT tTO = ((TRUE == bFlexTimeOut) ? SWC_IO_TIMEOUT_CALC(timeout, len) : timeout);
	CHAR* cp;

	if(NULL == buf) return 0;
	if(0 > GetFd()) return D_SOCK_ECLOSE;
	if(NULL == GetSSL()) return D_SOCK_ECLOSE;
	if(0 >= len) return 0;

	remain = len;
	cp = (CHAR*)buf;
	len = 0;

	time(&cur_time);
	prev_time = cur_time;

	WF_SSL_set_mode(m_pSSL, SSL_MODE_ENABLE_PARTIAL_WRITE);

	while(remain > 0)
	{
		SWC::swc_sleep(0, 1);

		if(0 < remain)
		{
			Lock();
			if(NULL != m_pSSL) 
			{
				nsend = WF_SSL_write(m_pSSL, cp, remain);
				if(0 < nsend) UpdateTimeLastIO();
			}
			else
			{
				Unlock();
				return D_SOCK_ETIMEOUT;
			}
			Unlock();

			if(nsend < 0)
			{
				INT nerr = 0;

				Lock();
				if(NULL != m_pSSL) nerr = WF_SSL_get_error(m_pSSL, r);
				else
				{
					Unlock();
					return D_SOCK_ETIMEOUT;
				}
				Unlock();

				if( (SSL_ERROR_WANT_WRITE == nerr) ||
					(errno == EWOULDBLOCK) || (errno == EINTR) || (errno == EAGAIN) ||
					((SSL_ERROR_SYSCALL == nerr) && (0 == r)))
				{
					time(&cur_time);
					if(prev_time != cur_time)
					{
						prev_time = cur_time;
						tsum++;
					}

					if(tTO < tsum)
					{
						return D_SOCK_ETIMEOUT;
					}
					else continue;
				}
				else
				{
					return D_SOCK_ECLOSE;
				}
			}
			else if(nsend == 0) 
			{
				break;
			}

			remain -= nsend;
			cp += nsend;
			len += nsend;
		}
	}
	return len;
}

/* ----------------------------------------------------------------------------
    description : read 
    parameters  :
		- (VOID*) reading buffer
		- (INT)   reading length
    return      :
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CSSLIONode::Read(VOID* buf, INT len)
{
	INT nret = 0;
	if(NULL == buf) return 0;
	if(0 > GetFd()) return D_SOCK_ECLOSE;
	if(NULL == GetSSL()) return D_SOCK_ECLOSE;
	if(0 >= len) return 0;
	Lock();
	if(NULL != m_pSSL)
	{
		nret = WF_SSL_read(m_pSSL, buf, len);
		if(0 < nret) UpdateTimeLastIO();
	}
	Unlock();
	return nret;
}

/* ----------------------------------------------------------------------------
    description : read with timeout
    parameters  :
		- (VOID*) reading buffer
		- (INT)   reading length
		- (INT)   timout
		- (SWC_BOOL) increase timeout considering buffer length (increase one second per 1 Mb)
    return      :
    history     :
        - 2010. 03. 21 created by Park. Sung soo
		- 2023. 06. 06 WF_SSL_set_mode() added
---------------------------------------------------------------------------- */
INT CSSLIONode::Read(VOID* buf, INT len, INT timeout, SWC_BOOL bFlexTimeOut)
{
	time_t prev_time = 0, cur_time = 0;
	INT nrecv = 0, remain = 0, tsum = 0, r = 0;
	INT tTO = ((TRUE == bFlexTimeOut) ? SWC_IO_TIMEOUT_CALC(timeout, len) : timeout);
	CHAR* cp;

	if(NULL == buf) return 0;
	if(0 > GetFd()) return D_SOCK_ECLOSE;
	if(NULL == GetSSL()) return D_SOCK_ECLOSE;
	if(0 >= len) return 0;

	remain = len;
	cp = (CHAR*)buf;
	len = 0;

	time(&cur_time);
	prev_time = cur_time;

	WF_SSL_set_mode(m_pSSL, SSL_MODE_AUTO_RETRY);

	while(remain > 0)
	{
		SWC::swc_sleep(0, 1);

		if(0 < remain)
		{
			Lock();
			if(NULL != m_pSSL) 
			{
				nrecv = WF_SSL_read(m_pSSL, cp, remain);
				if(0 < nrecv) UpdateTimeLastIO();
			}
			else
			{
				Unlock();
				return D_SOCK_ECLOSE;
			}
			Unlock();

			if(nrecv < 0)
			{
				INT nerr = 0;

				Lock();
				if(NULL != m_pSSL) nerr = WF_SSL_get_error(m_pSSL, r);
				else 
				{
					Unlock();
					return D_SOCK_ECLOSE;
				}
				Unlock();

				if( (SSL_ERROR_WANT_READ == nerr) ||
					(errno == EWOULDBLOCK) || (errno == EINTR) || (errno == EAGAIN) ||
					((SSL_ERROR_SYSCALL == nerr) && (0 == r)))
				{
					time(&cur_time);
					if(prev_time != cur_time)
					{
						prev_time = cur_time;
						tsum++;
					}

					if(tTO < tsum)
					{
						return D_SOCK_ETIMEOUT;
					}
					else continue;
				}
				else
				{
					return D_SOCK_ECLOSE;
				}
			}
			else if(nrecv == 0) 
			{
				break;
			}

			remain -= nrecv;
			cp += nrecv;
			len += nrecv;
		}
	}
	return len;
}

/* ----------------------------------------------------------------------------
    description : memory allocale, copy & return m_pTempBuf
    parameters  :
    return      : (CHAR*) return buffer pointer
    history     :
        - 2010. 04. 28 created by Park. Sung soo
---------------------------------------------------------------------------- */
CHAR* CSSLIONode::CopyTempBuf()
{
	if(NULL == m_pTempBuf) return NULL;
	if(0 == m_nTempBufLen) return NULL;
	Lock();
	CHAR* pret = NULL;
	pret = (CHAR*)malloc(m_nTempBufLen);
	if(NULL != pret) memcpy(pret, m_pTempBuf, m_nTempBufLen);
	Unlock();
	return pret;
}

/* ----------------------------------------------------------------------------
    description : memory allocale, copy & return m_pRecv
    parameters  :
    return      : (CHAR*) return buffer pointer
    history     :
        - 2010. 04. 28 created by Park. Sung soo
---------------------------------------------------------------------------- */
CHAR* CSSLIONode::CopyRecvBuf()
{
	if(NULL == m_pRecv) return NULL;
	if(0 == m_nRecvLen) return NULL;
	Lock();
	CHAR* pret = NULL;
	pret = (CHAR*)malloc(m_nRecvLen);
	if(NULL != pret) memcpy(pret, m_pRecv, m_nRecvLen);
	Unlock();
	return pret;
}

/* ----------------------------------------------------------------------------
    description : get ssl error
    parameters  :
		(SWC_BOOL) prINT message or not
    return      : 
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CSSLIONode::GetSSLError(SWC_BOOL bout)
{
	INT r = 0;
	if(NULL == GetSSL())
	{
		if(TRUE == bout) _LOG_ERR("SSL is null");
		return 0;
	}
	INT nret = WF_SSL_get_error(GetSSL(), r);
	if(TRUE == bout)
	{
		switch(nret)
		{
			case SSL_ERROR_NONE :
				_LOG_DBG("SSL_ERROR_NONE");
				break;
			case SSL_ERROR_SSL :
				_LOG_ERR("SSL_ERROR_SSL. ssl library interner error");
				break;
			case SSL_ERROR_WANT_READ :
				_LOG_DBG("SSL_ERROR_WANT_READ");
				break;
			case SSL_ERROR_WANT_WRITE :
				_LOG_DBG("SSL_ERROR_WANT_WRITE");
				break;
			case SSL_ERROR_SYSCALL :
				_LOG_ERR("SSL_ERROR_SYSCALL");
				break;
			case SSL_ERROR_ZERO_RETURN :
				_LOG_ERR("SSL_ERROR_ZERO_RETURN. remote ssl machine shutdown");
				break;
			case SSL_ERROR_WANT_CONNECT :
				_LOG_DBG("SSL_ERROR_WANT_CONNECT. ssl session is connecting...");
				break;
			default :
				_LOG_ERR("unknown error");
				break;
		}
	}
	return nret;
}

/* ----------------------------------------------------------------------------
    description : set file send mode 
    parameters  :
		(const CHAR*) target file path
		(UINT)       block size
    return      : (SWC_BOOL) return result
    history     :
        - 2010. 05. 06 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CSSLIONode::SetFileSendMode(const CHAR* spath, UINT nbsize)
{
	// init. file sending environments
	ClrFileSendMode();
	if(NULL == spath) return FALSE;
	if(NULL == (m_pFile = fopen(spath, "rb")))
	{
		_LOG_ERR("file open failed (%s)", spath);
		return FALSE;
	}
	m_nFTotalSize = (INT)SWC::swc_get_file_size(spath);
	m_nFBlockSize  = nbsize;
	if(m_nFTotalSize >= m_nFBlockSize) m_nFTotalBlock = (INT)(m_nFTotalSize / m_nFBlockSize);
	else                               m_nFTotalBlock = 0;
	m_nFRestSize = (m_nFTotalSize - (m_nFTotalBlock * m_nFBlockSize));
	if(0 < m_nFTotalBlock) m_nFSendingBytes = m_nFBlockSize;
	else                   m_nFSendingBytes = m_nFRestSize;
	m_nFSentBytes = 0;
	m_bIsFileSendMode = TRUE;

	// start file sending...
	if(NULL != m_pFBuf) { free(m_pFBuf); m_pFBuf = NULL; }
	m_pFBuf = (CHAR*)malloc(nbsize);
	if(NULL == m_pFBuf) return FALSE;
	fread(m_pFBuf, 1, m_nFSendingBytes, m_pFile);
	SetSendMode(m_pFBuf, m_nFSendingBytes);
	return TRUE;
}

/* ----------------------------------------------------------------------------
    description : clear (reset) file send mode
    parameters  :
    return      : 
    history     :
        - 2010. 05. 06 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::ClrFileSendMode()
{
	m_bIsFileSendMode = FALSE;
	if(NULL != m_pFile) { fclose(m_pFile); m_pFile = NULL; }
	m_nFTotalSize    = 0;
	m_nFTotalBlock   = 0;
	m_nFBlockSize    = 0;
	m_nFRestSize     = 0;
	m_nFSentBytes    = 0;
	m_nFSendingBytes = 0;
	if(NULL != m_pFBuf) { free(m_pFBuf); m_pFBuf = NULL; }
}

/* ----------------------------------------------------------------------------
    description : set send next block in the file contents
    parameters  :
    return      : 
    history     :
        - 2010. 05. 06 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetFileSendNext()
{
	if(NULL == m_pFile) return;
	Lock();
	m_nFSentBytes += m_nFSendingBytes;
	if((m_nFTotalSize - m_nFSentBytes) >= m_nFBlockSize)
	{
		m_nFSendingBytes = m_nFBlockSize;
	}
	else if(0 == (m_nFTotalSize - m_nFSentBytes))
	{
		m_nFSendingBytes = 0;
		m_nFSentBytes = m_nFTotalSize;
		Unlock();
		return;
	}
	else
	{
		m_nFSendingBytes = m_nFRestSize;
	}
	fread(m_pFBuf, 1, m_nFSendingBytes, m_pFile);
	Unlock();
	SetSendMode(m_pFBuf, m_nFSendingBytes);
}

/* ----------------------------------------------------------------------------
    description : set receiving pointer
    parameters  : 
		(CHAR*) pointer
    return      : 
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetRecvPtr(CHAR* pbuf)
{
	Lock();
	m_pRecv = pbuf;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : get receving pointer
    parameters  : 
    return      : (CHAR*) return pointer
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
CHAR* CSSLIONode::GetRecvPtr()
{
	CHAR* pret = NULL;
	Lock();
	pret = m_pRecv;
	Unlock();
	return pret;
}

/* ----------------------------------------------------------------------------
    description : set sending pointer
    parameters  : (CHAR*) pointer
    return      : 
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetSendPtr(CHAR* pbuf)
{
	Lock();
	m_pSend = pbuf;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : get sending pointer
    parameters  : 
    return      : (CHAR*) return pointer
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
CHAR* CSSLIONode::GetSendPtr()
{
	CHAR* pret = NULL;
	Lock();
	pret = m_pSend;
	Unlock();
	return pret;
}

/* ----------------------------------------------------------------------------
    description : set recevived position
    parameters  : (INT) position
    return      : 
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetRecvPos(INT npos)
{
	Lock();
	m_nRecvPos = npos;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : get recevied position
    parameters  : 
    return      : (INT) received position
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CSSLIONode::GetRecvPos()
{
	INT nret = 0;
	Lock();
	nret = m_nRecvPos;
	Unlock();
	return nret;
}

/* ----------------------------------------------------------------------------
    description : set sent position
    parameters  : (INT) position
    return      : 
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetSendPos(INT npos)
{
	Lock();
	m_nSendPos = npos;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : get sent position
    parameters  : 
    return      : (INT) return position
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CSSLIONode::GetSendPos()
{
	INT nret = 0;
	Lock();
	nret = m_nSendPos;
	Unlock();
	return nret;
}

/* ----------------------------------------------------------------------------
    description : add recevied position
    parameters  : (INT) position
    return      : 
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetRecvPosAdd(INT npos)
{
	Lock();
	m_nRecvPos += npos;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : add sent position
    parameters  : (INT) position
    return      : 
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetSendPosAdd(INT npos)
{
	Lock();
	m_nSendPos += npos;
	Unlock();
	//SWC::swc_sleep(0, 1000);
}

/* ----------------------------------------------------------------------------
    description : check if receving mode
    parameters  :
    return      : return result
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CSSLIONode::IsRecvMode()
{
	SWC_BOOL bret = FALSE;
	Lock();
	bret = m_bRecvMode;
	Unlock();
	return bret;
}

/* ----------------------------------------------------------------------------
    description : check if sending mode
    parameters  : 
    return      : (SWC_BOOL) return result
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CSSLIONode::IsSendMode()
{
	SWC_BOOL bret = FALSE;
	Lock();
	bret = m_bSendMode;
	Unlock();
	return bret;
}

/* ----------------------------------------------------------------------------
    description : set as command header received status
    parameters  : 
    return      : 
    history     :
        - 2010. 07. 29 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetHeadRcvd()
{
	Lock();
	m_bHeadRcvd = TRUE;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : set as command header not received
    parameters  : 
    return      : 
    history     :
        - 2010. 07. 29 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetHeadEmpty()
{
	Lock();
	m_bHeadRcvd = FALSE;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : check if command header received
    parameters  : 
    return      : (SWC_BOOL) return result
    history     :
        - 2010. 07. 29 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CSSLIONode::IsHeadRcvd()
{
	SWC_BOOL bret = FALSE;
	Lock();
	bret = m_bHeadRcvd;
	Unlock();
	return bret;
}

/* ----------------------------------------------------------------------------
    description : wait until socket sending buffer is empty for 2 seconds
    parameters  : 
    return      : return result
    history     :
        - 2010. 08. 09 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CSSLIONode::WaitSendingComplete(INT nsfd)
{
	if(0 > nsfd) return FALSE;
	INT unsent = 0;
	time_t tcur = 0, tprev = 0;

	ioctl(nsfd, TIOCOUTQ, &unsent);
	if(0 == unsent) return TRUE;
	time(&tcur);
	tprev = tcur;
	while(0 < unsent)
	{
		SWC::swc_sleep(0, 1);
		ioctl(nsfd, TIOCOUTQ, &unsent);
		if(0 == unsent) return TRUE;
		time(&tcur);
		if(2 <= abs(tcur - tprev)) break;
	}
	return FALSE;
}

/* ----------------------------------------------------------------------------
    description : extract SSL error
    parameters  : 
		(SOCKET) socket descriptor
		(INT)    previous error value
    return      : (INT) return result
		0  -> ssl closed
		-1 -> ssl error
		1  -> try again
    history     :
        - 2010. 08. 09 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CSSLIONode::GetSSLError(SOCKET nsfd, INT nresult)
{
	if(0 > nsfd) return -1;
	INT nerr = WF_SSL_get_error(m_pSSL, nresult);
	INT nret = 0;

	switch(nerr)
	{
		case SSL_ERROR_NONE :
			nret = 1;
			break;
		case SSL_ERROR_WANT_WRITE :
		case SSL_ERROR_WANT_READ :
			nret = 1;
			break;
		case SSL_ERROR_ZERO_RETURN :
		case SSL_ERROR_SYSCALL :
		case SSL_ERROR_SSL :
			nret = 0;
			break;
		default :
			nret = -1;
	}
	return nret;
}

/* ----------------------------------------------------------------------------
	status related functions
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetBusy()
{
	Lock();
	m_bWorking = TRUE; 
	Unlock();
}

VOID CSSLIONode::SetIdle()
{ 
	Lock();
	m_bWorking = FALSE; 
	ResetBuffer();
	Unlock();
	InitForIdleMode();
}

SWC_BOOL CSSLIONode::IsBusy()
{ 
	SWC_BOOL bret = FALSE;

	Lock();
	bret = m_bWorking;
	Unlock();
	return bret;
}

SWC_BOOL CSSLIONode::IsIdle()
{ 
	SWC_BOOL bret = FALSE;

	Lock();
	bret = !m_bWorking;
	Unlock();
	return bret;
}

VOID CSSLIONode::StartJob()
{
	Lock();
	m_bHasJob = TRUE;
	Unlock();
}

VOID CSSLIONode::CompleteJob()
{
	Lock();
	m_bHasJob = FALSE;
	Unlock();
}

SWC_BOOL CSSLIONode::HasJob()
{
	SWC_BOOL bRet = FALSE;
	Lock();
	bRet = m_bHasJob;
	Unlock();
	return bRet;
}

/* ----------------------------------------------------------------------------
    description : copy xml buffer string 
    parameters  : 
		(WCString) input xml 
    return      : 
    history     :
        - 2013. 11. 22 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetXmlBuf(WCString sxml)
{
	Lock();
	m_pXmlBuf = sxml;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : copy xml buffer string 
    parameters  : 
		(std::string) input xml
    return      : ()
    history     :
        - 2013. 11. 22 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetXmlBuf(std::string sxml)
{
	Lock();
	m_pXmlBuf = sxml;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : copy xml buffer string 
    parameters  : 
		(const CHAR*) input xml
    return      :
    history     :
        - 2013. 11. 22 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::SetXmlBuf(const CHAR* sxml)
{
	Lock();
	m_pXmlBuf = sxml;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : get xml buffer string 
    parameters  : 
    return      : (WCString) return string
    history     :
        - 2013. 11. 22 created by Park. Sung soo
---------------------------------------------------------------------------- */
WCString CSSLIONode::GetXmlBuf()
{
	WCString sRet;

	Lock();
	sRet = m_pXmlBuf;
	Unlock();

	return sRet;
}

/* ----------------------------------------------------------------------------
    description : reset receiving and sending buffer
    parameters  : 
    return      : 
    history     :
        - 2014. 04. 22 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::ResetBuffer()
{
    if(NULL != m_pRecvBack) { free(m_pRecvBack); m_pRecvBack = NULL; }
    if(NULL != m_pSendBack) { free(m_pSendBack); m_pSendBack = NULL; }
	if(NULL != m_pFile) { fclose(m_pFile); m_pFile = NULL; } 
}

/* ----------------------------------------------------------------------------
    description : init all variables except temp buffer
    parameters  : 
    return      :
    history     :
        - 2014. 04. 22 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CSSLIONode::InitForIdleMode()
{
	Close(TRUE);
	m_lID             = 0;
	m_llID            = 0;
	m_lIP             = 0;
	memset(m_cMAC, 0x00, 6);
	memset(m_sMAC, 0x00, 13);
	memset(m_sKey, 0x00, SOCK_IO_NODE_KEY_LEN);
	m_pPtr            = NULL;
	m_nSfd            = INVALID_SOCKET;
	m_nRecvLen        = 0;
	m_nSendLen        = 0;
	m_bRecvMode       = FALSE;
	m_bSendMode       = FALSE;
	m_pRecv           = NULL;
	m_pSend           = NULL;
	m_pRecvBack       = NULL;
	m_pSendBack       = NULL;
	m_nRecvPos        = 0;
	m_nSendPos        = 0;
	m_bWorking        = FALSE;
	m_bHasJob         = FALSE;
	m_bHeadRcvd       = FALSE;
	m_nRecvAllocLen   = 0;
	m_nSendAllocLen   = 0;
	m_tConnect        = 0;
	m_tLastIO         = 0;
	m_pSSL            = NULL;
	m_bIsFileSendMode = FALSE;
	m_pFile           = NULL;
	m_nFTotalSize     = 0;
	m_nFTotalBlock    = 0;
	m_nFBlockSize     = 0;
	m_nFRestSize      = 0;
	m_nFSentBytes     = 0;
	m_nFSentBytes     = 0;
	m_nFSendingBytes  = 0;
	m_nRecvTimeSpend  = 0;
	m_tRecvPrev       = 0;
	m_pFBuf           = NULL;
}

/* ----------------------------------------------------------------------------
    description : check receive timeout
    parameters  : 
    return      : (SWC_BOOL) return result
    history     :
        - 2014. 04. 22 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CSSLIONode::CheckRecvTimeOut()
{
	SWC_BOOL bRet = FALSE;
	time_t tCur = 0;
	time(&tCur);

	if(0 < abs(tCur - m_tRecvPrev))
	{
		m_nRecvTimeSpend++;
		if(m_nRecvTimeOut < m_nRecvTimeSpend) bRet = TRUE;
		m_tRecvPrev = tCur;
	}
	return bRet;
}

/* ----------------------------------------------------------------------------
    description : get file pointer
    parameters  : 
    return      : (FILE*) file pointer
    history     :
        - 2019. 02. 11 created by Park. Sung soo
---------------------------------------------------------------------------- */
FILE* CSSLIONode::GetFDesc()
{
	FILE* pret = NULL;
	Lock();
	pret = m_pFile;
	Unlock();
	return pret;
}

/* ----------------------------------------------------------------------------
	description : get sent bytes on file contents
	parameters  : 
	return      : (UINT) sent bytes
	history     :
		- 2019. 02. 11 created by Park. Sung soo
---------------------------------------------------------------------------- */
UINT CSSLIONode::GetFSentBytes()
{
	UINT nret = 0;
	Lock();
	nret = m_nFSentBytes;
	Unlock();
	return nret;
}

/* ----------------------------------------------------------------------------
	description : get sending bytes on file contents
	parameters  : 
	return      : (UINT) sent bytes
	history     :
		- 2019. 02. 11 created by Park. Sung soo
---------------------------------------------------------------------------- */
UINT CSSLIONode::GetSendingBytes()
{
	UINT nret = 0;
	Lock();
	nret = m_nFSendingBytes;
	Unlock();
	return nret;
}

//-----------------------------------------------------------------------------
#endif // __SWC_SSL_IONODE_CPP__
