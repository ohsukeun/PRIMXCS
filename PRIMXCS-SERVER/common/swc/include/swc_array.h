/*****************************************************************************
 *                                                                           *
 * File Name : swc_array.h                                                   *
 * Theme     : std:vector wrapper class (array)                              *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 10. 26                     *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include <vector>

#ifdef __SWC_OS_LINUX__
	#include <pthread.h>
#endif 

#include <swc_base.h>

#ifndef __SWC_ARRAY_H__
#define __SWC_ARRAY_H__

//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------
/**
 * @file swc_array.h
 * @brief array template wrapped std:vector
 * @code

// sample code
INT main()
{
    // INT type array define
    WCArray<INT> arrINT;

    // add node to array
    arrINT.Add(1);
    arrINT.Add(2);
    arrINT.Add(3);
    arrINT.Add(4);

    // check array node count
    std::cout << "array count: " << arrINT.GetCount() << std::endl;

    // access for each node (equivalnt methods: ElementAt(UINT nIndex) & GetAt(UINT nIndex) )
    UINT nID = 0;
    for(nID = 0; nID < arrINT.GetCount(); nID++)
    {
        std::cout << "arrIN[" << nID << "] = " << arrINT[nID] << std::endl;
    }

    // remove node
    arrINT.RemoveAt(1);
    std::cout << "array count: " << arrINT.GetCount() << std::endl;
    for(nID = 0; nID < arrINT.GetCount(); nID++)
    {
        std::cout << "arrIN[" << nID << "] = " << arrINT[nID] << std::endl;
    }

    // remove all node
    arrINT.RemoveAll();
    std::cout << "array count: " << arrINT.GetCount() << std::endl;

    return 0;
}

[results]
array count: 4
arrIN[0] = 1
arrIN[1] = 2
arrIN[2] = 3
arrIN[3] = 4
array count: 3
arrIN[0] = 1
arrIN[1] = 3
arrIN[2] = 4
array count: 0

@endcode
*/

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
#define ARRAY_ITERATOR  typename std::vector<TYPE>::iterator
#define ARRAY_RITERATOR typename std::vector<TYPE>::reverse_iterator

/// array template class (std::vector wrapper)
template < typename TYPE, 
           typename ALLOCATOR = std::allocator < TYPE > >
class WCArray
{
public :
	UINT Add(IN TYPE rData)
	{
		m_Array.push_back(rData);
		return (UINT)m_Array.size();
	}
	UINT Append(IN WCArray& src)
	{
		ARRAY_ITERATOR it_1_s, it_1_l, it_2_s, it_2_l;
		std::vector<TYPE> _tarray;

		_tarray=this->m_Array;	

		it_1_s = _tarray.begin(); 
		it_1_l = it_1_s + _tarray.size();
		it_2_s = src.m_Array.begin();
		it_2_l = it_2_s + src.m_Array.size();
		this->m_Array.resize(_tarray.size() + src.m_Array.size());

		merge(it_1_s, it_1_l, it_2_s, it_2_l, this->m_Array.begin()); 
		_tarray.clear();
		return (UINT)m_Array.size();
	}
	TYPE& ElementAt(IN UINT nIndex)
	{
		return m_Array.at(nIndex);
	}
	const TYPE& ElementAt(IN UINT nIndex) const
	{
		return m_Array.at(nIndex);
	}
	TYPE& GetAt(IN UINT nIndex)
	{
		return m_Array.at(nIndex);
	}
	const TYPE& GetAt(IN UINT nIndex) const
	{
		return m_Array.at(nIndex);
	}
	UINT GetCount() const
	{
		UINT lret = (UINT)m_Array.size();
		if(SWC_EXC_MAX_COUNT < lret) lret = 0;
		return lret;
	}
	UINT GetSize() const
	{
		return (UINT)m_Array.capacity();
	}
	INT GetUpperBound() const
	{
		if(0 == m_Array.size()) return -1;
		return (INT)(m_Array.size() -1);
	}
	SWC_BOOL InsertAt(IN UINT nIndex, IN TYPE newData)
	{
		SWC_BOOL       bret = FALSE;
		UINT           ncur = 0;
		ARRAY_ITERATOR curit;

		if(0 == m_Array.size()) 
		{
			m_Array.push_back(newData);
			return TRUE;
		}
		for(curit = m_Array.begin(); curit != m_Array.end(); curit++)
		{
			if(ncur == nIndex)
			{
				m_Array.insert(curit, newData);
				bret = TRUE;
				break;
			}
			ncur++;
		}
		return bret;
	}
	SWC_BOOL InsertAt(IN UINT nIndex, IN WCArray* pNewArray)
	{
		SWC_BOOL       bret = FALSE;
		UINT           ncur = 0;
		ARRAY_ITERATOR curit;
		TYPE           curDatum;

		if(NULL == pNewArray) return FALSE;
		curDatum = pNewArray->GetAt(nIndex);
		for(curit = m_Array.begin(); curit != m_Array.end(); curit++)
		{
			if(ncur == nIndex)
			{
				m_Array.insert(curit, curDatum);
				bret = TRUE;
				break;
			}
			ncur++;
		}
		return bret;
	}
	SWC_BOOL IsEmpty()
	{
		return m_Array.empty();
	}
	VOID RemoveAll()
	{
		//TYPE *p;

		m_Array.clear();
		//m_Array.get_allocator().deallocate(p, m_Array.size());
	}
	SWC_BOOL RemoveAt(IN UINT nIndex)
	{
		SWC_BOOL       bret = FALSE;
		UINT           ncur = 0;
		ARRAY_ITERATOR curit;
		
		for(curit = m_Array.begin(); curit != m_Array.end(); curit++)
		{
			if(ncur == nIndex)
			{
				m_Array.erase(curit);
				bret = TRUE;
				break;
			}
			ncur++;
		}
		return bret;
	}
	SWC_BOOL SetAt(IN UINT nIndex, IN TYPE newData)
	{
		if(nIndex >= m_Array.size()) return FALSE;
		m_Array.at(nIndex) = newData;
		return TRUE;
	}
	VOID SetAtGrow(IN UINT nIndex, IN TYPE newData)
	{
		if(nIndex >= m_Array.size())
			m_Array.push_back(newData);
		else
			m_Array.at(nIndex) = newData;
	}
	VOID SetSize(IN UINT nNewSize)
	{
		m_Array.reserve(nNewSize);
	}
	VOID SetResize(IN UINT nNewSize)
	{
		m_Array.resize(nNewSize);
	}
	TYPE& operator [] (IN UINT nIndex)
	{
		return m_Array.at(nIndex);
	}
	const TYPE& operator [] (IN UINT nIndex) const
	{
		return m_Array.at(nIndex);
	}
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
	WCArray<TYPE, ALLOCATOR> ()
	{
#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
        pthread_mutex_init(&m_Lock, NULL);
#endif
	};
	~WCArray<TYPE, ALLOCATOR> ()
	{
		RemoveAll();

#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
        pthread_mutex_destroy(&m_Lock);
#endif
	};

public :
	std::vector<TYPE> m_Array;

private :
	ARRAY_ITERATOR    m_Iterator;
	ARRAY_RITERATOR   m_rIterator;

#if defined(__SWC_OS_LINUX__) || defined(__SWC_OS_MAC__)
    pthread_mutex_t   m_Lock;
#endif
};

//-----------------------------------------------------------------------------
#endif // __SWC_ARRAY_H__
 
