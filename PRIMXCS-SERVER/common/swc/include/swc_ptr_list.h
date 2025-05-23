/*****************************************************************************
 *                                                                           *
 * File Name : swc_ptr_list.h                                                *
 * Theme     : std:list wrapper class (pointer list)                         *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 10. 27                     *
 *                                                                           *
 *****************************************************************************/

#include <swc_list.h>

#ifndef __SWC_PTR_LIST_H__
#define __SWC_PTR_LIST_H__

//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------
/**
 * @file swc_ptr_list.h
 * @brief pointer list inherited from WCList
 * @code

// sample code
INT main()
{
    // sample class
    class CValue
    {
    public :
        UINT nValue;
        WCString sValue;

        CValue() { nValue = 0; };
        ~CValue() {};
    };

    // add node to list
    WCPtrList listPtr;
    UINT nID = 0;

    for(nID = 0; nID < 5; nID++)
    {
        CValue* pValue = new CValue();

        pValue->nValue = nID;
        pValue->sValue.Format("value-%u", nID);

        listPtr.AddTail((PVOID)pValue);
    }

    std::cout << "list count : " << listPtr.GetCount() << std::endl;

    // access each node
    if(0 < listPtr.GetCount())
    {
        PTR_LIST_POSITION pos = listPtr.GetHeadPosition();

        while(FALSE == listPtr.IsTail(pos))
        {
            PVOID pOut = NULL;

            pOut = listPtr.GetNext(pos);
            if(NULL != pOut)
            {
                CValue* pValue = (CValue*)pOut;

                std::cout << "nValue : " << pValue->nValue << " / sValue : " << LPCSTR(pValue->sValue) << std::endl;
            }
        }
    }

    // cleanup heaped memory
    if(0 < listPtr.GetCount())
    {
        PTR_LIST_POSITION pos = listPtr.GetHeadPosition();

        while(FALSE == listPtr.IsTail(pos))
        {
            PVOID pOut = NULL;

            pOut = listPtr.GetNext(pos);
            if(NULL != pOut) delete (CValue*)pOut;
        }
        listPtr.RemoveAll();
    }

    std::cout << "list count : " << listPtr.GetCount() << std::endl;

    return 0;
}

[results]
st count : 5
nValue : 0 / sValue : value-0
nValue : 1 / sValue : value-1
nValue : 2 / sValue : value-2
nValue : 3 / sValue : value-3
nValue : 4 / sValue : value-4
list count : 0

@endcode
*/

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
#define PTR_LIST_POSITION std::list<VOID*>::const_iterator

/// pointer list class
class WCPtrList : public WCList<VOID*> 
{
public :
	PTR_LIST_POSITION GetHeadPosition() const
	{
		return m_List.begin();
	};
	VOID* GetNext(OUT PTR_LIST_POSITION& rPosition)
	{
		PTR_LIST_POSITION curit = rPosition;

		if(m_List.end() == rPosition) 
		{
			return *m_List.end();
		}
		rPosition++;
		return *curit;
	};
	const VOID* GetNext(OUT PTR_LIST_POSITION& rPosition) const
	{
		PTR_LIST_POSITION curit = rPosition;

		if(m_List.end() == rPosition) 
		{
			return *m_List.end();
		}
		rPosition++;
		return *curit;
	};
	SWC_BOOL IsTail(IN PTR_LIST_POSITION position) const
	{
		if(m_List.end() == position) return true;
		return false;
	}
	WCPtrList()
	{
	};
	~WCPtrList()
	{
		RemoveAll();
	};
};

//-----------------------------------------------------------------------------
#endif // __SWC_PTR_LIST_H__
 
