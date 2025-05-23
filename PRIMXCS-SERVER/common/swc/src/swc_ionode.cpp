/*****************************************************************************
 *                                                                           *
 * File Name : swc_ionode.cpp                                                *
 * Theme     : sock io-node                                                  *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '12. 11. 22                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_ionode.cpp
/// @author Park, sung soo
/// @date 2012-11-22
/// @brief sock io-node

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <math.h>

#include <logger.h>

#include <swc_safty.h>
#include <swc_tool.h>
#include <swc_debug.h>
#include <swc_ionode.h>

#ifndef __SWC_IONODE_CPP__
#define __SWC_IONODE_CPP__

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
CIONode::CIONode()
{
	Init();
}

/* ----------------------------------------------------------------------------
    description : creator
    parameters  :
        (SOCKET) socket descriptor
    return      :
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
CIONode::CIONode(IN SOCKET nsfd)
{
	Init();
	m_nSfd = nsfd;
}

/* ----------------------------------------------------------------------------
    description : creator
    parameters  :
        (IN SOCKET) socket descriptor
		(IN UINT)   init. receiving buffer length
    return      :
    history     :
        - 2019. 02. 12 created by Park. Sung soo
---------------------------------------------------------------------------- */
CIONode::CIONode(SOCKET nsfd, IN UINT nbuflen)
{
	Init();
	m_nSfd        = nsfd;
	m_pTempBuf    = (CHAR*)malloc(nbuflen);
	m_nTempBufLen = nbuflen;
}

/* ----------------------------------------------------------------------------
    description : destroyer
    parameters  :
		- ()
    return      :
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
CIONode::~CIONode()
{
	m_lID             = 0;
	m_llID            = 0;
	m_lIP             = 0;
	memset(m_cMAC, 0x00, 6);
	memset(m_sMAC, 0x00, 13);
	memset(m_sKey, 0x00, SOCK_IO_NODE_KEY_LEN);
	if(NULL != m_pPtr) m_pPtr = NULL;
	if(INVALID_SOCKET != m_nSfd) Close();
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
	pthread_mutex_destroy(&m_Lock);
}

/* ----------------------------------------------------------------------------
    description : Init.
    parameters  :
    return      :
    history     :
        - 2019. 02. 12 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CIONode::Init()
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
    description : close socket
    parameters  :
		(SWC_BOOL) close socket or not
    return      :
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CIONode::Close()
{
	Lock();
	if((0 < m_nSfd) && (65535 > m_nSfd))
	{
		struct linger tmp = {1, 0};
		setsockopt(m_nSfd, SOL_SOCKET, SO_LINGER, (CHAR*)&tmp, sizeof(struct linger));
		shutdown(m_nSfd, SHUT_RDWR);
		close(m_nSfd);
		m_nSfd = INVALID_SOCKET;
		Reset();
	}
	if(TRUE == m_bIsFileSendMode) ClrFileSendMode();
	if(0 != GetUserValue()) SetUserValue(0);
	SetTimeZeroLastIO();
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : reset object
    parameters  :
    return      : 
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CIONode::Reset()
{
	ResetRecv();
	ResetSend();
	SetConnectTime(0);
}

SOCKET CIONode::GetFd()
{
	SOCKET nret = INVALID_SOCKET;
	Lock();
	nret = m_nSfd;
	Unlock();
	return nret;
}

VOID CIONode::SetFd(IN SOCKET nsfd)
{
	Lock();
	m_nSfd = nsfd;
	Unlock();
	UpdateTimeLastIO();
}

/* ----------------------------------------------------------------------------
    description : reset receiving envrionment
    parameters  :
    return      :
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CIONode::ResetRecv()
{
	m_nRecvLen       = m_nRecvPos = 0;
	m_bRecvMode      = FALSE;
	m_bHeadRcvd      = FALSE;
	m_nRecvTimeSpend = 0;
	m_tRecvPrev      = 0;
}

/* ----------------------------------------------------------------------------
    description : reset sending envronment
    parameters  :
    return      :
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CIONode::ResetSend()
{
	m_nSendLen  = m_nSendPos = 0;
	m_bSendMode = FALSE; 
	m_bRecvMode = TRUE;
}

/* ----------------------------------------------------------------------------
    description : allocate temorary receiving buffer
    parameters  :
		(INT) temporary buffer length
    return      : (SWC_BOOL) return result
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CIONode::SetTempBufAlloc(IN INT nlen)
{
	if(0 >= nlen) return FALSE;

/*
	if(nlen > m_nTempBufLen) 
	{
		CHAR* ptemp = NULL;

		if(0 == m_nTempBufLen)
		{
			m_nTempBufLen = nlen;
			if(NULL != m_pTempBuf) { free(m_pTempBuf); m_pTempBuf = NULL; }
			ptemp = (CHAR*)malloc(m_nTempBufLen);

			if(NULL == ptemp)
			{
				_LOG_ERR("malloc() failed - case 1");
				m_nTempBufLen = 0;
				m_pTempBuf = NULL;
				return FALSE;
			}
		}
		else
		{
			if(NULL == m_pTempBuf)
			{
				m_nTempBufLen = nlen;
				ptemp = (CHAR*)malloc(m_nTempBufLen);

				if(NULL == ptemp)
				{
					_LOG_ERR("malloc() failed - case 2");
					m_nTempBufLen = 0;
					m_pTempBuf = NULL;
					return FALSE;
				}
			}
			else
			{
				m_nTempBufLen = nlen;
				ptemp = (CHAR*)realloc(m_pTempBuf, m_nTempBufLen);

				if(NULL == ptemp)
				{
					_LOG_ERR("realloc() failed");
					m_nTempBufLen = 0;
					if(NULL != m_pTempBuf) { free(m_pTempBuf); m_pTempBuf = NULL; }
					return FALSE;
				}
			}
		}
		if(NULL != ptemp) 
		{
			m_pTempBuf = ptemp;
			memset(m_pTempBuf, 0x00, m_nTempBufLen);
		}
	}
	else if(nlen == m_nTempBufLen)
	{
		if(NULL != m_pTempBuf) 
		{
			memset(m_pTempBuf, 0x00, m_nTempBufLen);
		}
		else
		{
			CHAR* ptemp = NULL;

			ptemp = (CHAR*)malloc(m_nTempBufLen);
			if(NULL == ptemp)
			{
				_LOG_ERR("malloc() failed - case 3");
				m_nTempBufLen = 0;
				return FALSE;
			}
			else 
			{
				m_pTempBuf = ptemp;
				memset(m_pTempBuf, 0x00, m_nTempBufLen);
			}
		}
	}
	else // nlen < m_nTempBufLen
	{
		CHAR* ptemp = NULL;

		if(NULL != m_pTempBuf) { free(m_pTempBuf); m_pTempBuf = NULL; }
		m_nTempBufLen = nlen;
		ptemp = (CHAR*)malloc(m_nTempBufLen);

		if(NULL == ptemp)
		{
			_LOG_ERR("malloc() failed - case 4");
			m_nTempBufLen = 0;
			return FALSE;
		}
		m_pTempBuf = ptemp;
		memset(m_pTempBuf, 0x00, m_nTempBufLen);
	}
*/
	m_nTempBufLen = nlen;
	if(NULL == m_pTempBuf)
	{
		m_pTempBuf = (CHAR*)malloc(nlen);
	}
	memset((CHAR*)m_pTempBuf, 0x00, nlen);
	return TRUE;
}

/* ----------------------------------------------------------------------------
    description : set send mode
    parameters  :
		(VOID*) sending buffer
		(INT)   sending buffer length
    return      : 
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CIONode::SetSendMode(IN VOID* pbuf, IN INT nlen)
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
			if(NULL != m_pSendBack) { free(m_pSendBack); m_pSendBack = NULL; }
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
				if(NULL != m_pSendBack) { free(m_pSendBack); m_pSendBack = NULL; }
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
					if(NULL != m_pSendBack) { free(m_pSendBack); m_pSendBack = NULL; }
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

		if(NULL != m_pSendBack) { free(m_pSendBack); m_pSendBack = NULL; }
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
		(INT) receiving buffer length
    return      : (SWC_BOOL) return result
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CIONode::SetRecvMode(IN INT nlen)
{
	if(0 >= nlen) return FALSE;
	SWC_BOOL bsuccess = TRUE;

	Lock();
	if((size_t)nlen > m_nRecvAllocLen)
	{
		if(0 == m_nRecvAllocLen)
		{
			CHAR* ptemp = NULL;

			m_nRecvAllocLen = (size_t)nlen;
			if(NULL != m_pRecvBack) { free(m_pRecvBack); m_pRecvBack = NULL; }
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
				if(NULL != m_pRecvBack) { free(m_pRecvBack); m_pRecvBack = NULL; }
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

		if(NULL != m_pRecvBack) { free(m_pRecvBack); m_pRecvBack = NULL; }
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
		(INT) receiving buffer length
    return      : (SWC_BOOL) return result
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CIONode::InitAsRecv(IN INT nlen)
{
	SWC_BOOL bret = TRUE;
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
	return bret;
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
INT CIONode::Write(IN VOID* buf, IN INT len)
{
	INT nret = 0;
	if(NULL == buf) return 0;
	if(0 > GetFd()) return 0;
	if(0 >= len) return 0;
	nret = send(m_nSfd, buf, len, 0);
	if(0 < nret) UpdateTimeLastIO();
	return nret;
}

/* ----------------------------------------------------------------------------
    description : write buffer with timeout
    parameters  :
		(VOID*)    writting buffer 
		(INT)      buffer length
		(INT)      timtout
		(SWC_BOOL) fixed timeout or not
    return      :
    history     :
        - 2010. 05. 10 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CIONode::Write(IN VOID* buf, IN INT len, IN INT timeout, IN SWC_BOOL bFlexTimeOut)
{
	time_t prev_time = 0, cur_time = 0;
	INT nsend = 0, remain = 0, tsum = 0;
	INT tTO = ((TRUE == bFlexTimeOut) ? SWC_IO_TIMEOUT_CALC(timeout, len) : timeout);
	CHAR* cp;

	if(NULL == buf) return 0;
	if(0 > GetFd()) return 0;
	if(0 >= len) return 0;

	remain = len;
	cp = (CHAR*)buf;
	len = 0;

	time(&cur_time);
	prev_time = cur_time;

	while(remain > 0)
	{
		SWC::swc_sleep(0, 1);

		if(0 < remain)
		{
			Lock();
			nsend = send(m_nSfd, cp, remain, 0);
			Unlock();

			if(nsend < 0)
			{
				if((errno == EWOULDBLOCK) || (errno == EINTR) || (errno == EAGAIN))
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
			else
			{
				UpdateTimeLastIO();
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
		(VOID*) reading buffer
		(INT)   reading length
    return      : 
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CIONode::Read(OUT VOID* buf, IN INT len)
{
	INT nret = 0;
	if(NULL == buf) return 0;
	if(0 > GetFd()) return 0;
	if(0 >= len) return 0;
	nret = read(m_nSfd, buf, len);
	if(0 < nret) UpdateTimeLastIO();
	return nret;
}

/* ----------------------------------------------------------------------------
    description : read with timeout
    parameters  :
		(VOID*)    reading buffer
		(INT)      reading length
		(INT)      timout
		(SWC_BOOL) fixed timeout or not
    return      : 
    history     :
        - 2010. 03. 21 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT CIONode::Read(OUT VOID* buf, IN INT len, IN INT timeout, IN SWC_BOOL bFlexTimeOut)
{
	time_t prev_time = 0, cur_time = 0;
	INT nrecv = 0, remain = 0, tsum = 0;
	INT tTO = ((TRUE == bFlexTimeOut) ? SWC_IO_TIMEOUT_CALC(timeout, len) : timeout);
	CHAR* cp;

	if(NULL == buf) return 0;
	if(0 > GetFd()) return 0;
	if(0 >= len) return 0;

	remain = len;
	cp = (CHAR*)buf;
	len = 0;

	time(&cur_time);
	prev_time = cur_time;

	while(remain > 0)
	{
		SWC::swc_sleep(0, 1);

		if(0 < remain)
		{
			Lock();
			nrecv = read(m_nSfd, cp, remain);
			Unlock();

			if(nrecv < 0)
			{
				if((errno == EWOULDBLOCK) || (errno == EINTR) || (errno == EAGAIN))
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
			else
			{
				UpdateTimeLastIO();
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
CHAR* CIONode::CopyTempBuf()
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
CHAR* CIONode::CopyRecvBuf()
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
    description : set file send mode 
    parameters  :
		(const CHAR*) target file path
		(UINT)        block size
    return      : (SWC_BOOL) return result
    history     :
        - 2010. 05. 06 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CIONode::SetFileSendMode(IN const CHAR* spath, IN UINT nbsize)
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
	return SetSendMode(m_pFBuf, m_nFSendingBytes);
}

/* ----------------------------------------------------------------------------
    description : clear (reset) file send mode
    parameters  :
    return      :
    history     :
        - 2010. 05. 06 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CIONode::ClrFileSendMode()
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
    return      : (SWC_BOOL) return result
    history     :
        - 2010. 05. 06 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CIONode::SetFileSendNext()
{
	if(NULL == m_pFile) return FALSE;
	SWC_BOOL bret = TRUE;

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
		return FALSE;
	}
	else
	{
		m_nFSendingBytes = m_nFRestSize;
	}
	fread(m_pFBuf, 1, m_nFSendingBytes, m_pFile);
	Unlock();
	bret = SetSendMode(m_pFBuf, m_nFSendingBytes);
	return bret;
}

/* ----------------------------------------------------------------------------
    description : set receiving pointer
    parameters  : (CHAR*) pointer
    return      :
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CIONode::SetRecvPtr(IN CHAR* pbuf)
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
CHAR* CIONode::GetRecvPtr()
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
VOID CIONode::SetSendPtr(IN CHAR* pbuf)
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
CHAR* CIONode::GetSendPtr()
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
VOID CIONode::SetRecvPos(IN INT npos)
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
INT CIONode::GetRecvPos()
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
VOID CIONode::SetSendPos(IN INT npos)
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
INT CIONode::GetSendPos()
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
VOID CIONode::SetRecvPosAdd(IN INT npos)
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
VOID CIONode::SetSendPosAdd(IN INT npos)
{
	Lock();
	m_nSendPos += npos;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : check if receving mode
    parameters  : 
    return      : return result
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CIONode::IsRecvMode()
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
    return      : return result
    history     :
        - 2010. 06. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL CIONode::IsSendMode()
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
VOID CIONode::SetHeadRcvd()
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
VOID CIONode::SetHeadEmpty()
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
SWC_BOOL CIONode::IsHeadRcvd()
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
SWC_BOOL CIONode::WaitSendingComplete(IN INT nsfd)
{
	if(0 >= nsfd) return FALSE;
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
     status related functions
---------------------------------------------------------------------------- */
VOID CIONode::SetBusy()
{
	Lock();
	m_bWorking = TRUE;
	Unlock();
}

VOID CIONode::SetIdle()
{
	Lock();
	m_bWorking = FALSE;
	ResetBuffer();
	Unlock();
	InitForIdleMode();
}

SWC_BOOL CIONode::IsBusy()
{
	SWC_BOOL bret = FALSE;

	Lock();
	bret = m_bWorking;
	Unlock();
	return bret;
}

SWC_BOOL CIONode::IsIdle()
{
	SWC_BOOL bret = FALSE;

	Lock();
	bret = !m_bWorking;
	Unlock();
	return bret;
}

VOID CIONode::StartJob()
{
	Lock();
	m_bHasJob = TRUE;
	Unlock();
}

VOID CIONode::CompleteJob()
{
	Lock();
	m_bHasJob = FALSE;
	Unlock();
}

SWC_BOOL CIONode::HasJob()
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
VOID CIONode::SetXmlBuf(WCString sxml)
{
	Lock();
	m_pXmlBuf = sxml;
	Unlock();
}

/* ----------------------------------------------------------------------------
    description : copy xml buffer string 
    parameters  : 
		(std::string) input xml
    return      :
    history     :
        - 2013. 11. 22 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID CIONode::SetXmlBuf(std::string sxml)
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
VOID CIONode::SetXmlBuf(const CHAR* sxml)
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
WCString CIONode::GetXmlBuf()
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
VOID CIONode::ResetBuffer()
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
VOID CIONode::InitForIdleMode()
{
	Close();
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
SWC_BOOL CIONode::CheckRecvTimeOut()
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
FILE* CIONode::GetFDesc()
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
UINT CIONode::GetFSentBytes()
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
UINT CIONode::GetSendingBytes()
{
	UINT nret = 0;
	Lock();
	nret = m_nFSendingBytes;
	Unlock();
	return nret;
}

//-----------------------------------------------------------------------------
#endif // __SWC_IONODE_CPP__
