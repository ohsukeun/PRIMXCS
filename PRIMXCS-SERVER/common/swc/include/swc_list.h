/*****************************************************************************
 *                                                                           *
 * File Name : swc_list.h                                                    *
 * Theme     : std:map wrapper class (list)                                  *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 10. 20                     *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include <list>

#ifdef __SWC_OS_LINUX__
	#include <pthread.h>
#endif 

#include <swc_base.h>

#ifndef __SWC_LIST_H__
#define __SWC_LIST_H__

//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------
/**
 * @file swc_list.h
 * @brief list template wrapped std:list
 * @code

// sample code
INT main()
{
    // INT type list define
    WCList<INT> listINT;

    // add node to list
    listINT.AddTail(1);
    listINT.AddTail(2);
    listINT.AddTail(3);
    listINT.AddTail(4);

    // check list node count
    std::cout << "list count: " << listINT.GetCount() << std::endl;

    // access for each node
    if(0 < listINT.GetCount())
    {
        listINT.GoHead();
        do
        {
            INT nVal = 0;

            listINT.GetData(nVal);
            std::cout << "list node: " << nVal << std::endl;
        }
        while(TRUE == listINT.GoNext());
    }

    // remove all nodes
    listINT.RemoveAll();
    std::cout << "list count: " << listINT.GetCount() << std::endl;

    return 0;
}

[results]
list count: 4
list node: 1
list node: 2
list node: 3
list node: 4
list count: 0

@endcode
*/

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
#define LIST_ITERATOR     typename std::list<TYPE>::iterator
#define LIST_RITERATOR    typename std::list<TYPE>::reverse_iterator

/// list template class (std::list wrapper)
template < typename TYPE, 
           typename ALLOCATOR = std::allocator < TYPE > >
class WCList
{
public :
	VOID AddHead(IN TYPE newData)
	{
		m_List.push_front(newData);
	};
	VOID AddTail(IN TYPE newData)
	{
		m_List.push_back(newData);
	};
	SWC_BOOL GetAtFromHead(IN UINT nIndex, IN TYPE& rData) 
	{
		UINT ncur = 0;
		SWC_BOOL bret = FALSE;

		for(m_Iterator = m_List.begin(); m_Iterator != m_List.end(); m_Iterator++)
		{
			if(ncur == nIndex)
			{
				rData = *m_Iterator;
				bret = TRUE;
				break;
			}
			ncur++;
		}
		return bret;
	};
	SWC_BOOL GetAtFromTail(IN UINT nIndex, IN TYPE& rData)
	{
		UINT ncur = 0;
		SWC_BOOL bret = FALSE;
		
		for(m_rIterator = m_List.rbegin(); m_rIterator != m_List.rend(); m_rIterator++)
		{
			if(ncur == nIndex)
			{
				rData = *m_rIterator;
				bret = TRUE;
				break;
			}
			ncur++;
		}
		return bret;
	}
	UINT GetCount() const
	{
		UINT nret = (UINT)m_List.size();
		if(SWC_EXC_MAX_COUNT < nret) nret = 0;
		return nret;
	};
	const TYPE& GetHead() const
	{
		return *m_List.begin();
	};
	const TYPE& GetTail() const
	{
		return *m_List.end();
	};
	SWC_BOOL GoHead()
	{
		m_Iterator = m_List.begin();
		if(m_Iterator != m_List.end()) return TRUE;
		return FALSE;
	};
	SWC_BOOL GoTail()
	{
		m_rIterator = m_List.rbegin();
		if(m_rIterator != m_List.rend()) return TRUE;
		return FALSE;
	};
	SWC_BOOL GoNext()
	{
		m_Iterator++;
		if(m_Iterator != m_List.end()) return TRUE;
		return FALSE;
	};
	SWC_BOOL GoPrev()
	{
		m_rIterator++;
		if(m_rIterator != m_List.rend()) return TRUE;
		return FALSE;
	};
	SWC_BOOL IsHead()
	{
		if(m_rIterator == m_List.rend()) return TRUE;
		return FALSE;
	}
	SWC_BOOL IsTail()
	{
		if(m_Iterator == m_List.end()) return TRUE;
		return FALSE;
	}
	SWC_BOOL GetData(OUT TYPE& rData)
	{
		if(m_Iterator != m_List.end()) 
		{
			rData = *m_Iterator;
			return TRUE;
		}
		return FALSE;
	};
	SWC_BOOL GetReverseData(OUT TYPE& rData)
	{
		if(m_rIterator != m_List.rend())
		{
			rData = *m_rIterator;
			return TRUE;
		}
		return FALSE;
	};
	SWC_BOOL IsEmpty()
	{
		return m_List.empty();
	};
	VOID RemoveAll()
	{
		m_List.clear();
	};
/* on the construction ..
	SWC_BOOL RemoveAt(UINT nIndex, TYPE& rData)
	{
		UINT  ncur = 0;
		SWC_BOOL          bret = FALSE;
		LIST_ITERATOR curit;
		
		for(curit = m_List.begin(); curit != m_List.end(); curit++)
		{
			if(ncur == nIndex)
			{
				rData = *curit;
				bret = TRUE;
				break;
			}
			ncur++;
		}
		if(TRUE == bret) m_List.erase(curit);
		return bret;
	};
*/
	SWC_BOOL RemoveHead(OUT TYPE& rData)
	{
		LIST_ITERATOR curit = m_List.begin();

		if(curit != m_List.end()) 
		{
			rData = *curit;
			m_List.pop_front();
			return TRUE;
		}
		return FALSE;
	};
	SWC_BOOL RemoveTail(OUT TYPE& rData)
	{
		LIST_RITERATOR curit = m_List.rbegin();

		if(curit != m_List.rend())
		{
			rData = *curit;
			m_List.pop_back();
			return TRUE;
		}
		return FALSE;
	};
	SWC_BOOL InsertAfter(IN UINT nIndex, IN TYPE newData)
	{
		UINT          ncur = 0;
		SWC_BOOL          bret = FALSE, btail = FALSE;;
		LIST_ITERATOR curit;

		for(curit = m_List.begin(); curit != m_List.end(); curit++)
		{
			if(ncur == nIndex)
			{
				curit++;
				if(curit == m_List.end()) 
				{
					bret = TRUE;
					btail = TRUE;
					break;
				}
				else
				{
					m_List.insert(curit, newData);
					bret = TRUE;
					break;
				}
			}
			ncur++;
		}
		if(TRUE == btail)  m_List.push_back(newData);
		return bret;
	};
	SWC_BOOL InsertBefore(IN UINT nIndex, IN TYPE newData)
	{
		UINT          ncur = 0;
		SWC_BOOL          bret = FALSE;
		LIST_ITERATOR curit;

		for(curit = m_List.begin(); curit != m_List.end(); curit++)
		{
			if(ncur == nIndex)
			{
				m_List.insert(curit, newData);
				bret = TRUE;
				break;
			}
			ncur++;
		}
		return bret;
	};
	SWC_BOOL SetAt(IN UINT nIndex, IN TYPE newData)
	{
		UINT          ncur = 0;
		SWC_BOOL          bret = FALSE, bfind = FALSE;
		LIST_ITERATOR curit;

		for(curit = m_List.begin(); curit != m_List.end(); curit++)
		{
			if(ncur == nIndex)
			{
				bfind = TRUE;
				break;
			}
			ncur++;
		}
		if(TRUE == bfind)
		{
			m_List.insert(curit, newData);
			m_List.erase(curit);
		}
		return bret;
	};
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
	SWC_BOOL IsLocked()
	{
		if(1 <= m_Lock.__data.__lock) return TRUE;
		return FALSE;
	}
	WCList<TYPE, ALLOCATOR> ()
	{
#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
        pthread_mutex_init(&m_Lock, NULL);
#endif
	};
	~WCList<TYPE, ALLOCATOR> ()
	{
		RemoveAll();

#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
        pthread_mutex_destroy(&m_Lock);
#endif
	};
	
public :
	std::list<TYPE> m_List;
	LIST_ITERATOR   m_Iterator;
	LIST_RITERATOR  m_rIterator;

private :
#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
    pthread_mutex_t      m_Lock;
#endif
};

//-----------------------------------------------------------------------------
#endif // __SWC_LIST_H__
 
