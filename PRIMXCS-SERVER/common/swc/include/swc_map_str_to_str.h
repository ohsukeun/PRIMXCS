/*****************************************************************************
 *                                                                           *
 * File Name : swc_map_str_to_str.h                                          *
 * Theme     : std:map wrapper class (map for string to string)              *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 11. 18                     *
 *                                                                           *
 *****************************************************************************/
 
#include <swc_map.h>
#include <swc_string.h>

#ifndef __SWC_MAP_STR_TO_STR_H__
#define __SWC_MAP_STR_TO_STR_H__

//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------
/**
 * @file swc_map_str_to_str.h
 * @brief WCString to WCString map inherited from WCMao
 * @code

// sample code
INT main()
{
    // key - WCString / value - WCString type map define
    WCMapStrToStr mapSS;
    UINT nID = 0;

    // set data
    for(nID = 0; nID < 5; nID++)
    {
        WCString sKey, sValue;

        sKey.Format("key-%u", nID);
        sValue.Format("value-%u", nID);

        mapSS.SetAt(sKey, sValue);
    }
    std::cout << "map count: " << mapSS.GetCount() << std::endl;

    // access each node
    if(0 < mapSS.GetCount())
    {
        STR_TO_STR_MAP_POSITION pos = mapSS.GetStartPosition();

        while(FALSE == mapSS.IsEnd(pos))
        {
            WCString sKey, sValue;

            mapSS.GetNextAssoc(pos, sKey, sValue);
            std::cout << "key : " << LPCSTR(sKey) << " / value : " << LPCSTR(sValue) << std::endl;
        }
    }

    // find node
    if(0 < mapSS.GetCount())
    {
        WCString sFindKey("key-3"), sFoundValue;

        if(TRUE == mapSS.Lookup(sFindKey, sFoundValue))
        {
            std::cout << "found key : " << LPCSTR(sFindKey) << " / found value : " << LPCSTR(sFoundValue) << std::endl;
        }
    }

    // delete node
    if(0 < mapSS.GetCount())
    {
        WCString sDeleteKey("key-4"), sDeleteValue;

        if(TRUE == mapSS.Lookup(sDeleteKey, sDeleteValue))
        {
            mapSS.RemoveKey(sDeleteKey);
        }
    }
    std::cout << "map count: " << mapSS.GetCount() << std::endl;

    // cleanup heaped memory
    if(0 < mapSS.GetCount())
    {
        mapSS.RemoveAll();
    }
    std::cout << "map count: " << mapSS.GetCount() << std::endl;

    return 0;
}

[results]
map count: 5
key : key-0 / value : value-0
key : key-1 / value : value-1
key : key-2 / value : value-2
key : key-3 / value : value-3
key : key-4 / value : value-4
found key : key-3 / found value : value-3
map count: 4
map count: 0

@endcode
*/

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
#define STR_TO_STR_MAP_POSITION std::map<std::string, std::string>::const_iterator

/// std::string to std::string map class
class WCMapStrToStr : public WCMap<std::string, std::string> 
{
public :
	STR_TO_STR_MAP_POSITION GetStartPosition() const
	{
		return m_Map.begin();
	};
	WCString GetNextAssoc(OUT STR_TO_STR_MAP_POSITION& rPos, OUT WCString& rKey, OUT WCString& rValue) 
	{
		STR_TO_STR_MAP_POSITION curit = rPos;
		WCString sret;

		if(m_Map.end() == rPos)
		{
			rKey   = m_Map.end()->first;
			rValue = m_Map.end()->second;
			sret   = m_Map.end()->second;
			return sret;
		}
		rPos++;
		rKey    = curit->first;
		rValue  = curit->second;
		sret    = curit->second;
		return sret;
	};
	SWC_BOOL IsEnd(IN STR_TO_STR_MAP_POSITION position)
	{
		if(m_Map.end() == position) return TRUE;
		return FALSE;
	};
	VOID GetKeyValue(OUT WCString& rKey, OUT WCString& rValue)
	{
		if(m_Map.end() == m_Iterator) return;
		rKey   = m_Iterator->first;
		rValue = m_Iterator->second;
	};
	VOID GetReverseKeyValue(OUT WCString& rKey, OUT WCString& rValue)
	{
		if(m_Map.rend() == m_rIterator) return;
		rKey   = m_rIterator->first;
		rValue = m_rIterator->second;
	};
	SWC_BOOL Lookup(IN WCString key, OUT WCString& rValue)
	{
		STR_TO_STR_MAP_POSITION curit = m_Map.find(key.m_Str);

		if(m_Map.end() != curit)
		{
			rValue = curit->second;
			return TRUE;
		}
		return FALSE;
	};
	SWC_BOOL Lookup(IN const CHAR* key, OUT WCString& rValue)
	{
		if(NULL == key) return FALSE;;
		std::string skey = key;
		STR_TO_STR_MAP_POSITION curit = m_Map.find(skey);

		if(m_Map.end() != curit)
		{
			rValue = curit->second;
			return TRUE;
		}
		return FALSE;
	};
	SWC_BOOL RemoveKey(IN WCString key)
	{
		STR_TO_STR_MAP_POSITION curit = m_Map.find(key.m_Str);

		if(m_Map.end() != curit)
		{
			m_Map.erase(key.m_Str);
			return TRUE;
		}
		return FALSE;
	};
	VOID SetAt(IN WCString key, IN WCString newValue)
	{
		if(m_Map.end() == m_Map.find(key.m_Str))
			m_Map.insert(std::make_pair(key.m_Str, newValue.m_Str));
		else
			m_Map[key.m_Str] = newValue.m_Str;
	};
	VOID SetAt(IN const CHAR* key, IN const CHAR* newValue)
	{
		if((NULL == key) || (NULL == newValue)) return;
		std::string skey = key;
		std::string snewValue = newValue;
		if(m_Map.end() == m_Map.find(skey))
			m_Map.insert(std::make_pair(skey, snewValue));
		else
			m_Map[skey] = snewValue;
	}
	VOID SetAt(IN const CHAR* key, IN WCString newValue)
	{
		if(NULL == key) return;
		std::string skey = key;
		if(m_Map.end() == m_Map.find(skey))
			m_Map.insert(std::make_pair(skey, newValue.m_Str));
		else
			m_Map[skey] = newValue.m_Str;
	}
	VOID SetAt(IN WCString key, IN const CHAR* newValue)
	{
		if(NULL == newValue) return;
		std::string snewValue = newValue;
		if(m_Map.end() == m_Map.find(key.m_Str))
			m_Map.insert(std::make_pair(key.m_Str, snewValue));
		else
			m_Map[key.m_Str] = snewValue;
	}
	WCMapStrToStr()
	{
	};
	~WCMapStrToStr()
	{
	};
};

//-----------------------------------------------------------------------------
#endif // __SWC_MAP_STR_TO_STR_H__
 
