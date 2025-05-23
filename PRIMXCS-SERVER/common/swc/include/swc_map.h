/*****************************************************************************
 *                                                                           *
 * File Name : swc_map.h                                                     *
 * Theme     : std:map wrapper class (map)                                   *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 10. 16                     *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include <map>

#ifdef __SWC_OS_LINUX__
	#include <pthread.h>
#endif 


#include <swc_base.h>

#ifndef __SWC_MAP_H__
#define __SWC_MAP_H__

//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------
/**
 * @file swc_map.h
 * @brief array template wrapped std:map
 * @code

// sample code
INT main()
{
    // key - UINT / value - INT type map define
    WCMap<UINT, INT> mapINT;
    UINT nID = 0;

    // set data 
    for(nID = 0; nID < 4; nID++)
    {
        INT nVal = (INT)nID;

        mapINT.SetAt(nID, nVal);
    }
    std::cout << "map count: " << mapINT.GetCount() << std::endl;

    // access each node
    if(0 < mapINT.GetCount())
    {
        mapINT.GoBegin();
        do
        {
            UINT nKey = 0;
            INT nOutVal = 0;

            mapINT.GetKeyValue(nKey, nOutVal);
            std::cout << "key : " << nKey << " / value : " << nOutVal << std::endl;
        }
        while(TRUE == mapINT.GoNext());
    }

    // find node
    UINT nFindKey = 3;
    INT nOutVal = 0;

    if(TRUE == mapINT.Lookup(nFindKey, nOutVal))
    {
        std::cout << "find key : " << nFindKey << " / find value : " << nOutVal << std::endl;
    }

    // remove node
    UINT nDeleteKey = 2;

    mapINT.RemoveKey(nDeleteKey);
    std::cout << "map count: " << mapINT.GetCount() << std::endl;

    return 0;
}

[results]
map count: 4
key : 0 / value : 0
key : 1 / value : 1
key : 2 / value : 2
key : 3 / value : 3
find key : 3 / find value : 3
map count: 3

@endcode
*/

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
#define MAP_ITERATOR  typename std::map<KEY, VALUE>::const_iterator 
#define MAP_RITERATOR typename std::map<KEY, VALUE>::reverse_iterator 

/// map template class (std::map wrapper)
template < typename KEY, 
           typename VALUE, 
           typename LESS = std::less< KEY >,
           typename ALLOCATOR = std::allocator< std::pair< const KEY, VALUE > > > 
class WCMap 
{
public :
	UINT GetCount() const 
	{ 
		UINT nret = (UINT)m_Map.size();
		if(SWC_EXC_MAX_COUNT < nret) nret = 0;
		return nret;
	};
	UINT GetHashTableSize() const 
	{ 
		//return (UINT)m_Map.max_size(); 
		return m_nAllocSize; 
	}; 
	SWC_BOOL GoBegin()
	{
		m_Iterator = m_Map.begin();
		if(m_Map.end() != m_Iterator) return TRUE;
		return FALSE;
	}
	SWC_BOOL GoEnd()
	{
		m_rIterator = m_Map.rbegin();
		if(m_Map.rend() != m_rIterator) return TRUE;
		return FALSE;
	}
	SWC_BOOL GoNext()
	{
		if(0 == m_Map.size()) return FALSE;
		m_Iterator++;
		if(m_Map.end() != m_Iterator) return TRUE;
		return FALSE;
	}
	SWC_BOOL GoPrev()
	{
		if(0 == m_Map.size()) return FALSE;
		m_rIterator++;
		if(m_Map.rend() != m_rIterator) return TRUE;
		return FALSE;
	}
	SWC_BOOL IsEnd()
	{
		if(m_Map.end() == m_Iterator) return TRUE;
		return FALSE;
	}
	SWC_BOOL IsBegin()
	{
		if(m_Map.rend() == m_rIterator) return TRUE;
		return FALSE;
	}
	VOID GetKeyValue(OUT KEY& rKey, OUT VALUE& rValue)
	{
		if(m_Map.end() == m_Iterator) return;
		rKey = m_Iterator->first;
		rValue = m_Iterator->second;
	}
	VOID GetReverseKeyValue(OUT KEY& rKey, OUT VALUE& rValue)
	{
		if(m_Map.rend() == m_rIterator) return;
		rKey = m_rIterator->first;
		rValue = m_rIterator->second;
	}
#ifndef __SNP_USE_STLPORT__
	VOID InitHastTable(IN UINT hashSize)
	{ 
		m_Map.get_allocator().allocate(hashSize); 
		m_nAllocSize = hashSize;
		m_bAlloc = TRUE;
	};
#endif
	SWC_BOOL IsEmpty() const 
	{ 
		return (SWC_BOOL)m_Map.empty(); 
	};
	SWC_BOOL Lookup(KEY key, VALUE& rValue) const
	{
		MAP_ITERATOR curit = m_Map.find(key);

		if(m_Map.end() != curit)
		{
			rValue = curit->second;
			return TRUE;
		}
		return FALSE;		
	};
	VOID RemoveAll() 
	{ 
		if(0 < m_Map.size()) m_Map.clear(); 
	};
	SWC_BOOL RemoveKey(IN KEY key)
	{
		if(m_Map.end() != m_Map.find(key))
		{
			m_Map.erase(key);
			return TRUE;
		}
		return FALSE;
	};
	VOID SetAt(IN KEY key, IN VALUE newValue)
	{
		if(m_Map.end() == m_Map.find(key))
			m_Map.insert(std::make_pair(key, newValue));
		else
			m_Map[key] = newValue;		
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
	};
	SWC_BOOL IsLocked()
	{
		if(1 <= m_Lock.__data.__lock) return TRUE;
		return FALSE;
	}
	WCMap<KEY, VALUE, LESS, ALLOCATOR> () 
	{
		m_bAlloc     = FALSE;
		m_nAllocSize = 0;
#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
	    pthread_mutex_init(&m_Lock, NULL);
#endif
	};
	~WCMap<KEY, VALUE, LESS, ALLOCATOR> ()
	{
	    RemoveAll();
#ifndef __SNP_USE_STLPORT__
		if(TRUE == m_bAlloc) 
		{
			typename std::pair<const KEY, VALUE>* __p = NULL;
			m_Map.get_allocator().deallocate(__p, m_nAllocSize);
		}
#endif

#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
// temporary blocked by PSS (2010. 10. 13)
//    	pthread_mutex_destroy(&m_Lock);
#endif
	};
public :
	std::map<KEY, VALUE, LESS, ALLOCATOR> m_Map; // open for using iterator
	MAP_ITERATOR                          m_Iterator;
	MAP_RITERATOR                         m_rIterator;

private :
	SWC_BOOL                              m_bAlloc;
	UINT                                  m_nAllocSize;

#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
	pthread_mutex_t      m_Lock;
#endif
};

//-----------------------------------------------------------------------------
#endif // __SWC_MAP_H__
 
