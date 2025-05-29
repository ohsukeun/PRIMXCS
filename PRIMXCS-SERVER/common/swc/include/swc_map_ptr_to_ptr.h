/*****************************************************************************
 *                                                                           *
 * File Name : swc_map_ptr_to_ptr.h                                          *
 * Theme     : std:map wrapper class (map pointer to pointer)                *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 10. 27                     *
 *                                                                           *
 *****************************************************************************/

#include <swc_map.h>

#ifndef __SWC_MAP_PTR_TO_PTR_H__
#define __SWC_MAP_PTR_TO_PTR_H__

//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------
/**
 * @file swc_map_ptr_to_ptr.h
 * @brief pointer to pointer map inherited from WCMap
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

    // key - PVOID(WCString*) / value - PVOID(CValue*) type map define
    WCMapPtrToPtr mapPP;
    WCString *pFindKey = NULL, *pDeleteKey = NULL;
    UINT nID = 0;

    // set data
    for(nID = 0; nID < 5; nID++)
    {
        WCString* pKey = new WCString();
        CValue*   pVal = new CValue();

        pKey->Format("key-%u", nID);
        pVal->nValue = nID;
        pVal->sValue.Format("value-%u", nID);

        mapPP.SetAt((PVOID)pKey, (PVOID)pVal);

        if(4 == nID) { pFindKey = pDeleteKey = pKey; }
    }
    std::cout << "map count: " << mapPP.GetCount() << std::endl;
    // access each node
    if(0 < mapPP.GetCount())
    {
        PTR_TO_PTR_MAP_POSITION pos = mapPP.GetStartPosition();

        while(FALSE == mapPP.IsEnd(pos))
        {
            PVOID pKey = NULL;
            PVOID pOutValue = NULL;

            mapPP.GetNextAssoc(pos, pKey, pOutValue);
            if(NULL != pOutValue)
            {
                WCString* pKeyStr = (WCString*)pKey;
                CValue*   pValue  = (CValue*)pOutValue;

                std::cout << "key : " << pKeyStr->GetBuffer() << " / value : " << pValue->nValue << ", " << LPCSTR(pValue->sValue) << std::endl;
            }
        }
    }

    // find node
    if(0 < mapPP.GetCount())
    {
        PVOID pFindValue = NULL;

        if(TRUE == mapPP.Lookup(pFindKey, pFindValue))
        {
            if((NULL != pFindKey) && (NULL != pFindValue))
            {
                WCString* pFoundKeyStr = (WCString*)pFindKey;
                CValue*   pFoundValue  = (CValue*)pFindValue;

                std::cout << "found key : " << pFoundKeyStr->GetBuffer() << " / found value : " << pFoundValue->nValue << ", " << LPCSTR(pFoundValue->sValue) << std::endl;
            }
        }
    }

    // delete node
    if(0 < mapPP.GetCount())
    {
        PVOID pDeleteValue = NULL;

        if(TRUE == mapPP.Lookup(pDeleteKey, pDeleteValue))
        {
            delete (CValue*)pDeleteValue;
            mapPP.RemoveKey(pDeleteKey);
            delete (WCString*)pDeleteKey;
        }
    }
    std::cout << "map count: " << mapPP.GetCount() << std::endl;


    // cleanup heaped memory
    if(0 < mapPP.GetCount())
    {
        PTR_TO_PTR_MAP_POSITION pos = mapPP.GetStartPosition();

        while(FALSE == mapPP.IsEnd(pos))
        {
            PVOID pKey = NULL;
            PVOID pOutValue = NULL;

            mapPP.GetNextAssoc(pos, pKey, pOutValue);
            if(NULL != pKey)      delete (WCString*)pKey;
            if(NULL != pOutValue) delete (CValue*)pOutValue;
        }
        mapPP.RemoveAll();
    }
    std::cout << "map count: " << mapPP.GetCount() << std::endl;

    return 0;
}

[results]
map count: 5
key : key-0 / value : 0, value-0
key : key-1 / value : 1, value-1
key : key-2 / value : 2, value-2
key : key-3 / value : 3, value-3
key : key-4 / value : 4, value-4
found key : key-4 / found value : 4, value-4
map count: 4
map count: 0

@endcode
*/

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
#define PTR_TO_PTR_MAP_POSITION std::map<VOID*, VOID*>::const_iterator

/// pointer to pointer map class 
class WCMapPtrToPtr : public WCMap<VOID*, VOID*> 
{
public :
	PTR_TO_PTR_MAP_POSITION GetStartPosition() const
	{
		return m_Map.begin();
	};
	VOID* GetNextAssoc(OUT PTR_TO_PTR_MAP_POSITION& rPos, OUT VOID*& rKey, OUT VOID*& rValue) 
	{
		PTR_TO_PTR_MAP_POSITION curit = rPos;

		if(m_Map.end() == rPos)
		{
			rKey   = m_Map.end()->first;
			rValue = m_Map.end()->second;
			return m_Map.end()->second;
		}
		rPos++;
		rKey   = curit->first;
		rValue = curit->second;
		return curit->second;
	};
	SWC_BOOL IsEnd(IN PTR_TO_PTR_MAP_POSITION position)
	{
		if(m_Map.end() == position) return true;
		return false;
	};
	WCMapPtrToPtr()
	{
	};
	~WCMapPtrToPtr()
	{
		RemoveAll();
	};
};

//-----------------------------------------------------------------------------
#endif // __SWC_MAP_PTR_TO_PTR_H__
 
