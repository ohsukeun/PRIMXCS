/*****************************************************************************
 *                                                                           *
 * File Name : swc_map_str_to_ptr.h                                          *
 * Theme     : std:map wrapper class (map for string to pointer)             *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 11. 18                     *
 *                                                                           *
 *****************************************************************************/

#include <swc_map.h>
#include <swc_string.h>

#ifndef __SWC_MAP_STR_TO_PTR_H__
#define __SWC_MAP_STR_TO_PTR_H__

//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------
/**
 * @file swc_map_str_to_ptr.h
 * @brief WCString to pointer map inherited from WCMap
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

    // key - WCString / value - PVOID(CValue*) type map define
    WCMapStrToPtr mapSP;
    UINT nID = 0;

    // set data
    for(nID = 0; nID < 5; nID++)
    {
        WCString sKey;
        CValue*   pVal = new CValue();

        sKey.Format("key-%u", nID);
        pVal->nValue = nID;
        pVal->sValue.Format("value-%u", nID);

        mapSP.SetAt(sKey, (PVOID)pVal);
    }
    std::cout << "map count: " << mapSP.GetCount() << std::endl;

    // access each node
    if(0 < mapSP.GetCount())
    {
        STR_TO_PTR_MAP_POSITION pos = mapSP.GetStartPosition();

        while(FALSE == mapSP.IsEnd(pos))
        {
            WCString sKey;
            PVOID pOutValue = NULL;

            mapSP.GetNextAssoc(pos, sKey, pOutValue);
            if(NULL != pOutValue)
            {
                CValue* pValue = (CValue*)pOutValue;

                std::cout << "key : " << LPCSTR(sKey) << " / value : " << pValue->nValue << ", " << LPCSTR(pValue->sValue) << std::endl;
            }
        }
    }

    // find node
    if(0 < mapSP.GetCount())
    {
        WCString sFindKey("key-3");
        PVOID pFindValue = NULL;

        if(TRUE == mapSP.Lookup(sFindKey, pFindValue))
        {
            if(NULL != pFindValue)
            {
                CValue* pFoundValue  = (CValue*)pFindValue;

                std::cout << "found key : " << LPCSTR(sFindKey) << " / found value : " << pFoundValue->nValue << ", " << LPCSTR(pFoundValue->sValue) << std::endl;
            }
        }
    }

    // delete node
    if(0 < mapSP.GetCount())
    {
        WCString sDeleteKey("key-4");
        PVOID pDeleteValue = NULL;

        if(TRUE == mapSP.Lookup(sDeleteKey, pDeleteValue))
        {
            delete (CValue*)pDeleteValue;
            mapSP.RemoveKey(sDeleteKey);
        }
    }
    std::cout << "map count: " << mapSP.GetCount() << std::endl;

    // cleanup heaped memory
    if(0 < mapSP.GetCount())
    {
        STR_TO_PTR_MAP_POSITION pos = mapSP.GetStartPosition();

        while(FALSE == mapSP.IsEnd(pos))
        {
            WCString sKey;
            PVOID pOutValue = NULL;

            mapSP.GetNextAssoc(pos, sKey, pOutValue);
            if(NULL != pOutValue) delete (CValue*)pOutValue;
        }
        mapSP.RemoveAll();
    }
    std::cout << "map count: " << mapSP.GetCount() << std::endl;

    return 0;
}

[results]
map count: 5
key : key-0 / value : 0, value-0
key : key-1 / value : 1, value-1
key : key-2 / value : 2, value-2
key : key-3 / value : 3, value-3
key : key-4 / value : 4, value-4
found key : key-3 / found value : 3, value-3
map count: 4
map count: 0

@endcode
*/

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
#define STR_TO_PTR_MAP_POSITION std::map<std::string, VOID*>::const_iterator

/// std::string to pointer map class 
class WCMapStrToPtr : public WCMap<std::string, VOID*> 
{
public :
	STR_TO_PTR_MAP_POSITION GetStartPosition() const
	{
		return m_Map.begin();
	};
	VOID* GetNextAssoc(OUT STR_TO_PTR_MAP_POSITION& rPos, OUT WCString& rKey, OUT VOID*& rValue) 
	{
		STR_TO_PTR_MAP_POSITION curit = rPos;

		if(m_Map.end() == rPos)
		{
			rKey   = m_Map.end()->first;
			rValue = m_Map.end()->second;
			return (VOID*)m_Map.end()->second;
		}
		rPos++;
		rKey   = curit->first;
		rValue = curit->second;
		return (VOID*)curit->second;
	};
	SWC_BOOL IsEnd(IN STR_TO_PTR_MAP_POSITION position)
	{
		if(m_Map.end() == position) return TRUE;
		return FALSE;
	};
	VOID GetKeyValue(OUT WCString& rKey, OUT VOID*& rValue)
	{
		if(m_Map.end() == m_Iterator) return;
		rKey   = m_Iterator->first;
		rValue = m_Iterator->second;
	};
	VOID GetReverseKeyValue(OUT WCString& rKey, OUT VOID*& rValue)
	{
		if(m_Map.rend() == m_rIterator) return;
		rKey   = m_rIterator->first;
		rValue = m_rIterator->second;
	};
	SWC_BOOL Lookup(IN WCString key, OUT VOID*& rValue)
	{
		STR_TO_PTR_MAP_POSITION curit = m_Map.find(key.m_Str);

		if(m_Map.end() != curit)
		{
			rValue = curit->second;
			return TRUE;
		}
		return FALSE;
	};
	SWC_BOOL Lookup(IN const CHAR* key, OUT VOID* rValue)
	{
		if(NULL == key) return FALSE;
		std::string skey = key;
		STR_TO_PTR_MAP_POSITION curit = m_Map.find(skey);
		if(m_Map.end() != curit)
		{
			rValue = curit->second;
			return TRUE;
		}
		return FALSE;
	}
	SWC_BOOL RemoveKey(IN WCString key)
	{
		STR_TO_PTR_MAP_POSITION curit = m_Map.find(key.m_Str);

		if(m_Map.end() != curit)
		{
			m_Map.erase(key.m_Str);
			return TRUE;
		}
		return FALSE;
	};
	VOID SetAt(IN WCString key, IN VOID* newValue)
	{
		if(m_Map.end() == m_Map.find(key.m_Str))
			m_Map.insert(std::make_pair(key.m_Str, newValue));
		else
			m_Map[key.m_Str] = newValue;
	};
	WCMapStrToPtr()
	{
	};
	~WCMapStrToPtr()
	{
		RemoveAll();
	};
};

//-----------------------------------------------------------------------------
#endif // __SWC_MAP_STR_TO_PTR_H__
 
