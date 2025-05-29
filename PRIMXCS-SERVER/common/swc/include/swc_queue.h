/*****************************************************************************
 *                                                                           *
 * File Name : swc_queue.h                                                   *
 * Theme     : std:map wrapper class (queue)                                 *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 10. 26                     *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include <deque>

#ifdef __SWC_OS_LINUX__
	#include <pthread.h>
#endif 

#include <swc_base.h>

#ifndef __SWC_QUEUE_H__
#define __SWC_QUEUE_H__

//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------
/**
 * @file swc_queue.h
 * @brief list template wrapped std:deque
 * @code

// sample code
INT main()
{
    // INT type queue define
    WCQueue<UINT> qINT;
    UINT nID = 0;

    // queue size should be defined ~!!
    qINT.SetSize(100);
    std::cout << "queue size: " << qINT.GetSize() << std::endl;

    // push data
    for(nID = 0; nID < 5; nID++)
    {
        qINT.Push(nID);
    }
    std::cout << "queue count : " << qINT.GetCount() << std::endl;

    // pop from queue
    UINT nValue = 0;

    while(TRUE == qINT.Pop(nValue))
    {
        std::cout << "poped value : " << nValue << std::endl;
    }
    std::cout << "queue count : " << qINT.GetCount() << std::endl;

    return 0;
}

[results]
queue size: 100
queue count : 5
poped value : 0
poped value : 1
poped value : 2
poped value : 3
poped value : 4
queue count : 0

@endcode
*/

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
/// queue template class (std::deque wrapper)
template < typename TYPE, 
           typename ALLOCATOR = std::allocator < TYPE > >
class WCQueue
{
public :
	VOID SetSize(IN UINT nSize)
	{
		m_nSize = nSize;
	};
	UINT GetSize()
	{
		return m_nSize;
	};
	UINT GetCount()
	{
		UINT nret = 0;

		Lock();
		nret = (UINT)m_Queue.size();
		Unlock();

		return nret;
	}
	SWC_BOOL IsEmpty()
	{
		return m_Queue.empty();
	}
	SWC_BOOL Push(IN TYPE rData)
	{
		SWC_BOOL bret = TRUE;
		UINT ncur = 0;

		Lock();
		ncur = (UINT)m_Queue.size();
		if(ncur < m_nSize) m_Queue.push_back(rData);
		else if(ncur >= m_nSize)
		{
			m_Queue.pop_front();
			m_Queue.push_back(rData); 
			bret = FALSE;
		}
		Unlock();

		return bret;
	}
	SWC_BOOL Pop(OUT TYPE& rData)
	{
		SWC_BOOL bret = TRUE;

		Lock();
		if(0 < m_Queue.size())
		{
			rData = m_Queue.front();
			m_Queue.pop_front();
		}
		else
		{
			bret = FALSE;
		}
		Unlock();

		return bret;
	}
	TYPE& GetAt(IN UINT nIndex)
	{
		return m_Queue.at(nIndex);
	}
	TYPE& operator [] (IN UINT nIndex)
	{
		return m_Queue.at(nIndex);
	}
	VOID RemoveAll()
	{
		Lock();
		m_Queue.clear();
		Unlock();
	}
	WCQueue<TYPE, ALLOCATOR> ()
	{
		m_nSize = 0;

#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
        pthread_mutex_init(&m_Lock, NULL);
#endif
	};
	~WCQueue<TYPE, ALLOCATOR> ()
	{
		RemoveAll();
		
#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
        pthread_mutex_destroy(&m_Lock);
#endif
	};
	
public :
	UINT             m_nSize;
	std::deque<TYPE> m_Queue;

private :
#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
    pthread_mutex_t      m_Lock;
#endif

public :
	VOID Lock()
	{
#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
        pthread_mutex_lock(&m_Lock);
#endif
	};
	VOID Unlock()
	{
#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
        pthread_mutex_unlock(&m_Lock);
#endif
	}	
};

//-----------------------------------------------------------------------------
#endif // __SWC_QUEUE_H__
 
