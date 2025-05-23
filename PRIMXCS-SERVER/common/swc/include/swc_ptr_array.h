/*****************************************************************************
 *                                                                           *
 * File Name : swc_ptr_array.h                                               *
 * Theme     : std:array wrapper class (pointer array)                       *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 10. 27                     *
 *                                                                           *
 *****************************************************************************/

#include <swc_array.h>

#ifndef __SWC_PTR_ARRAY_H__
#define __SWC_PTR_ARRAY_H__

//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------
/**
 * @file swc_ptr_array.h
 * @brief pointer array inherited from WCArray
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

    // add node to array
    WCPtrArray arrPtr;
    UINT nID = 0;

    for(nID = 0; nID < 5; nID++)
    {
        CValue* pValue = new CValue();

        pValue->nValue = nID;
        pValue->sValue.Format("value-%u", nID);
        arrPtr.Add((PVOID)pValue);
    }

    // check array node count
    std::cout << "array count: " << arrPtr.GetCount() << std::endl;

    // access for each node (equivalnt methods: ElementAt(UINT nIndex) & GetAt(UINT nIndex) )
    for(nID = 0; nID < arrPtr.GetCount(); nID++)
    {
        CValue* pValue = (CValue*)arrPtr[nID];

        if(NULL != pValue)
        {
            std::cout << "arrIN[" << nID << "] = " << "nValue : " << pValue->nValue << " / sValue : " << LPCSTR(pValue->sValue) << std::endl;
        }
    }

    // remove node
    UINT nRemoveIndex = 1; // remove node given array index 1
    if(arrPtr.GetCount() > nRemoveIndex)
    {
        CValue* pDelete = (CValue*)arrPtr[nRemoveIndex];
        if(NULL != pDelete) delete pDelete;
        arrPtr.RemoveAt(nRemoveIndex);
    }
    std::cout << "array count: " << arrPtr.GetCount() << std::endl;

    // remove all nodes
    if(0 < arrPtr.GetCount())
    {
        for(nID = 0; nID < arrPtr.GetCount(); nID++)
        {
            CValue* pValue = (CValue*)arrPtr[nID];
            if(NULL != pValue)
            {
                delete pValue;
            }
        }
        arrPtr.RemoveAll();
    }
    std::cout << "array count: " << arrPtr.GetCount() << std::endl;

    return 0;
}

[results]
array count: 5
arrIN[0] = nValue : 0 / sValue : value-0
arrIN[1] = nValue : 1 / sValue : value-1
arrIN[2] = nValue : 2 / sValue : value-2
arrIN[3] = nValue : 3 / sValue : value-3
arrIN[4] = nValue : 4 / sValue : value-4
array count: 4
array count: 0

@endcode
*/

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
/// pointer array class
class WCPtrArray : public WCArray<VOID*> 
{
public :
	WCPtrArray()
	{
	};
	~WCPtrArray()
	{
		RemoveAll();
	};
};

//-----------------------------------------------------------------------------
#endif // __SWC_PTR_ARRAY_H__
 
