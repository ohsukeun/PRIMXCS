/*****************************************************************************
 *                                                                           *
 * File Name : swc_pseudo_set.h                                              *
 * Theme     : std:map wrapper class. psuedo set                             *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '10. 05. 20                     *
 *                                                                           *
 *****************************************************************************/

#include <swc_map_str_to_ptr.h>
#include <swc_array.h>
#include <swc_safty.h>

#ifdef __SWC_OS_LINUX__
	#include <pthread.h>
#endif 

#include <swc_base.h>
#include <swc_safty.h>

#ifndef __SWC_PSEUDO_SET_H__
#define __SWC_PSEUDO_SET_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
/// pair data container template class
template <typename FIRST, typename SECOND>
class CPairContainer
{
public :
	VOID SetData(FIRST first, SECOND second)
	{
		m_First  = first;
		m_Second = second;
	}
	FIRST  GetFirst () { return m_First; };
	SECOND GetSecond() { return m_Second; };

	CPairContainer<FIRST, SECOND>() {};
	CPairContainer<FIRST, SECOND>(FIRST first, SECOND second) { SetData(first, second); }
	~CPairContainer<FIRST, SECOND>() {};

public :
	FIRST  m_First;
	SECOND m_Second;
};

/// triad data container template class
template <typename FIRST, typename SECOND, typename THIRD>
class CTriadContainer
{
public :
	VOID SetData(FIRST first, SECOND second, THIRD third)
	{
		m_First  = first;
		m_Second = second;
		m_Third  = third;
	}
	FIRST  GetFirst () { return m_First; };
	SECOND GetSecond() { return m_Second; };
	THIRD  GetThird () { return m_Third; };

	CTriadContainer<FIRST, SECOND, THIRD>() {};
	CTriadContainer<FIRST, SECOND, THIRD>(FIRST first, SECOND second, THIRD third) { SetData(first, second, third); }
	~CTriadContainer<FIRST, SECOND, THIRD>() {};

public :
	FIRST  m_First;
	SECOND m_Second;
	THIRD  m_Third;
};

/// quadrial data container template class
template <typename FIRST, typename SECOND, typename THIRD, typename FOURTH>
class CQuadContainer
{
public :
	VOID SetData(FIRST first, SECOND second, THIRD third, FOURTH fourth)
	{
		m_First  = first;
		m_Second = second;
		m_Third  = third;
		m_Fourth = fourth;
	}
	FIRST  GetFirst () { return m_First; };
	SECOND GetSecond() { return m_Second; };
	THIRD  GetThird () { return m_Third; };
	FOURTH GetFourth() { return m_Fourth; };

	CQuadContainer<FIRST, SECOND, THIRD, FOURTH>() {};
	CQuadContainer<FIRST, SECOND, THIRD, FOURTH>(FIRST first, SECOND second, THIRD third, FOURTH fourth) { SetData(first, second, third, fourth); }
	~CQuadContainer<FIRST, SECOND, THIRD, FOURTH>() {};

public :
	FIRST  m_First;
	SECOND m_Second;
	THIRD  m_Third;
	FOURTH m_Fourth;
};

/// pseudo group class
class WCPseudoGrp
{
public :
    UINT GetCount()
    {
        return m_Ary.GetCount();
    }
    VOID AddNode(VOID* pnode)
    {
        m_Ary.Add(pnode);
    }
    VOID FreeAll()
    {
        for(UINT i = 0; i < m_Ary.GetCount(); i++)
        {
            if(NULL != m_Ary[i])
			{
				free(m_Ary[i]);
				m_Ary[i] = NULL;
			}
        }
		if(NULL != m_pGrpOpt)
		{
			free(m_pGrpOpt);
			m_pGrpOpt = NULL;
		}
    }
    VOID* GetAt(UINT index)
    {
        if(index >= m_Ary.GetCount()) return NULL;
        return m_Ary[index];
    }
    VOID* operator [](UINT index)
    {
        if(index >= m_Ary.GetCount()) return NULL;
        return m_Ary[index];
    }
    VOID RemoveAll()
    {
        m_Ary.RemoveAll();
    }
	VOID SetGrpOptPtr(VOID* pptr) 
	{
		m_pGrpOpt  = pptr;
	}
	VOID* GetGrpOptPtr()
	{
		return m_pGrpOpt;
	}
    WCPseudoGrp() 
	{
		m_pGrpOpt = NULL;
	};
    ~WCPseudoGrp()
    {
        FreeAll();
        m_Ary.RemoveAll();
    }
public :
    WCArray<VOID*> m_Ary;
	VOID*          m_pGrpOpt;
};

/// pseudo set class
class WCPseudoSet
{
public :
    UINT GetCount()
    {
        return m_PseudoMap.GetCount();
    }
    WCPseudoGrp* FindGrp(IN UINT lid)
    {
        WCString skey;
        VOID* pout = NULL;

        skey.Format("%u", lid);
        if(TRUE == m_PseudoMap.Lookup(skey, pout)) return (WCPseudoGrp*)pout;
        return NULL;
    }
	VOID AddToSet(IN UINT lid)
	{
        WCString skey;
        VOID* pout = NULL;

        skey.Format("%u", lid);
        if(FALSE == m_PseudoMap.Lookup(skey, pout))
        {
            WCPseudoGrp* pnode = new WCPseudoGrp;
            m_PseudoMap.SetAt(skey, (VOID*)pnode);
        }
	}
    VOID AddToSet(IN UINT lid, IN VOID* popt)
    {
        WCString skey;
        VOID* pout = NULL;

        skey.Format("%u", lid);
        if(FALSE == m_PseudoMap.Lookup(skey, pout))
        {
            WCPseudoGrp* pnode = new WCPseudoGrp;
			pnode->SetGrpOptPtr(popt);
            m_PseudoMap.SetAt(skey, (VOID*)pnode);
        }
    }
    VOID AddToGrp(IN UINT lid, IN VOID* pnode)
    {
        WCString skey;
        VOID* pout = NULL;

        skey.Format("%u", lid);
        if(TRUE == m_PseudoMap.Lookup(skey, pout))
        {
            if(NULL != pout)
            {
                WCPseudoGrp* pfind = (WCPseudoGrp*)pout;
                pfind->AddNode(pnode);
            }
        }
    }
    VOID FreeAll()
    {
        STR_TO_PTR_MAP_POSITION pos = m_PseudoMap.GetStartPosition();

        while(FALSE == m_PseudoMap.IsEnd(pos))
        {
            WCString skey;
            VOID* pout;
            m_PseudoMap.GetNextAssoc(pos, skey, pout);
            if(NULL != pout)
            {
                WCPseudoGrp* pcur = (WCPseudoGrp*)pout;
                pcur->FreeAll();
                pcur->RemoveAll();
                delete pcur;
            }
        }
        m_PseudoMap.RemoveAll();
    }
    VOID FreeSetOnly()
    {
        STR_TO_PTR_MAP_POSITION pos = m_PseudoMap.GetStartPosition();

        while(FALSE == m_PseudoMap.IsEnd(pos))
        {
            WCString skey;
            VOID* pout;
            m_PseudoMap.GetNextAssoc(pos, skey, pout);
            if(NULL != pout)
            {
                WCPseudoGrp* pcur = (WCPseudoGrp*)pout;
                delete pcur;
            }
        }
        m_PseudoMap.RemoveAll();
    }
    WCPseudoSet() {};
    ~WCPseudoSet()
    {
        FreeAll();
        m_PseudoMap.RemoveAll();
    }
public :
    WCMapStrToPtr m_PseudoMap;
};


//-----------------------------------------------------------------------------
#endif // __SWC_PSEUDO_SET_H__
 
