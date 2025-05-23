/*****************************************************************************
 *                                                                           *
 * File Name : swc_mem_pool,cpp                                              *
 * Theme     : memory container class                                        *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 11. 23                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_mem_pool.cpp
/// @author Park, sung soo
/// @date 2009-11-23
/// @brief memory container class

#include <logger.h>

#include <swc_mem_pool.h>
#include <swc.h>

#ifndef __SWC_MEM_POOL_CPP__
#define __SWC_MEM_POOL_CPP__

//-----------------------------------------------------------------------------
// METHOD DEFINITIONS
//-----------------------------------------------------------------------------
/* ----------------------------------------------------------------------------
    description : creator & destroyer
    parameters  :
        - ()
    return      :
    history     :
        - 2012. 12. 28 created by Park. Sung soo
---------------------------------------------------------------------------- */
WCMemPool::WCMemPool()
{
	m_pMem = NULL;
	m_pWork = NULL;
	m_nMem = 0;
	m_nPos = 0;
}

WCMemPool::~WCMemPool()
{
	if(NULL != m_pMem) { free(m_pMem); m_pMem = NULL; m_pWork = NULL; }
	m_nMem = 0;
}

/* ----------------------------------------------------------------------------
    description : init. 
    parameters  :
        - ()
    return      : ()
    history     :
        - 2012. 12. 28 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCMemPool::Init()
{
	m_nPos = 0;
	m_pWork = m_pMem;
}

/* ----------------------------------------------------------------------------
    description : free
    parameters  :
        - ()
    return      : ()
    history     :
        - 2012. 12. 28 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCMemPool::Free()
{
	if(NULL != m_pMem)
	{
		free(m_pMem);
		m_pMem = NULL;
	}
	m_pWork = NULL;
	m_nPos = m_nMem = 0;
}

/* ----------------------------------------------------------------------------
    description : allocate memory
    parameters  :
        - (IN UINT) allocating memory size
    return      : ()
    history     :
        - 2012. 12. 28 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCMemPool::SetSize(IN UINT nLen)
{
	if(0 == nLen) return;
	Free();
	m_pWork = m_pMem = (CHAR*)malloc(nLen);
	if(NULL != m_pMem) memset(m_pMem, 0x00, nLen);
	m_nMem = nLen;
}

/* ----------------------------------------------------------------------------
    description : move to first position
    parameters  :
    return      : 
    history     :
        - 2018. 04. 01 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCMemPool::MoveFirst()
{
	m_nPos = 0;
	m_pWork = m_pMem;
}

/* ----------------------------------------------------------------------------
    description : copy memory 
    parameters  :
        - (VOID*) copy target memory pointer
		- (UINT)  copy target memory pointer length
    return      : ()
    history     :
        - 2012. 12. 28 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCMemPool::MemCpy(IN VOID* pptr, IN UINT nLen)
{
	if(0 == nLen) return;
	if(NULL == pptr) return;
	if(NULL == m_pMem)
	{
		m_pWork = m_pMem = (CHAR*)malloc(nLen);
		if(NULL == m_pMem)
		{
			_LOG_ERR("malloc() failed");
			m_nMem = m_nPos = 0;
			return;
		}
		memcpy(m_pMem, pptr, nLen);
		m_nMem = nLen;
		m_pWork += m_nMem;
	}
	else
	{
		if(m_nMem >= nLen)
		{
			if(NULL == m_pMem)
			{
				if(NULL != m_pMem) { free(m_pMem); m_pMem = NULL; }
				m_nMem = m_nPos = 0;
				return;
			}
			memcpy(m_pMem, pptr, nLen);
			m_pWork = m_pMem + nLen;
		}
		else
		{
			CHAR* ptemp = NULL;

			ptemp = (CHAR*)realloc(m_pMem, nLen);
			if(NULL == ptemp)
			{
				_LOG_ERR("realloc() failed");
				m_nPos = m_nMem = 0;
				if(NULL != m_pMem) 
				{
					free(m_pMem);
					m_pWork = m_pMem = NULL;
				}
				return;
			}
			m_pWork = m_pMem = ptemp;
			m_nMem = nLen;
			memcpy(m_pMem, pptr, nLen);
			m_pWork += nLen;
		}
	}
}

/* ----------------------------------------------------------------------------
    description : attach memory 
    parameters  :
        - (VOID*) attach target memory pointer
		- (UINT)  attach target memory pointer length
    return      : ()
    history     :
        - 2012. 12. 28 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCMemPool::Attach(IN VOID* pptr, IN UINT nLen)
{
	if(0 == nLen) return;
	//if(NULL == pptr) return;
	if(SWC_MAX_RW_BUF_LEN < nLen) return;
	if(NULL == m_pMem)
	{
		m_pWork = m_pMem = (CHAR*)malloc((size_t)nLen);
		if(NULL == m_pMem)
		{
			_LOG_ERR("malloc() failed");
			m_nMem = m_nPos = 0;
			return;
		}
		if(NULL != pptr) { memcpy(m_pMem, pptr, (size_t)nLen); }
		else             { memset(m_pMem, 0x00, (size_t)nLen); }
		m_nMem = nLen;
		m_pWork += m_nMem;
		m_nPos = m_nMem;
	}
	else
	{
		if((m_nPos + nLen) > m_nMem)
		{
			UINT ntemp = m_nPos + nLen;
			CHAR* ptemp = NULL;

			ptemp = (CHAR*)realloc(m_pMem, (size_t)ntemp);
			if(NULL == ptemp)
			{
				ptemp = (CHAR*)malloc((size_t)ntemp);
				if(NULL == ptemp)
				{
					_LOG_ERR("malloc() after realloc() failure failed. case 1");
					if(NULL != m_pMem)
					{
						free(m_pMem);
						m_pWork = m_pMem = NULL;
						m_nMem = m_nPos = 0;
						return;
					}
				}
				else
				{
					_LOG_ERR("malloc() after realloc() failure successed. case 1");
					memcpy(ptemp, m_pMem, (size_t)m_nMem);
					m_pMem = ptemp;
					m_pWork = m_pMem + m_nPos;
					if(NULL != pptr) { memcpy(m_pWork, pptr, (size_t)nLen); }
					else             { memset(m_pWork, 0x00, (size_t)nLen); }
					m_nPos += nLen;
					m_pWork += nLen;
				}
			}
			else
			{
				m_nMem = m_nPos + nLen;
				m_pMem = ptemp;
				m_pWork = m_pMem + m_nPos;
				if(NULL != pptr) { memcpy(m_pWork, pptr, (size_t)nLen); }
				else             { memset(m_pWork, 0x00, (size_t)nLen); }
				m_nPos += nLen;
				m_pWork += nLen;
			}
		}
		else // (m_nPos + nLen) <= m_nMem
		{
			if(NULL != pptr) { memcpy(m_pWork, pptr, (size_t)nLen); }
			else             { memset(m_pWork, 0x00, (size_t)nLen); }
			m_nPos += nLen;
			m_pWork += nLen;
		}
	}
}

//-----------------------------------------------------------------------------
#endif // __SWC_MEM_POOL_CPP__
