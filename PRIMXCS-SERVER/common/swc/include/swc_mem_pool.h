/*****************************************************************************
 *                                                                           *
 * File Name : swc_mem_pool.h                                                *
 * Theme     : memory container class                                        *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '10. 05. 22                     *
 *                                                                           *
 *****************************************************************************/

#include <iostream>

#include <swc_base.h>

#ifndef __SWC_MEM_POOL_H__
#define __SWC_MEM_POOL_H__

//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------
/**
 * @file swc_map_pool.h
 * @brief memory container class (caution : not memory pool)
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

    // allocate datum and attach to memory container
    WCMemPool Mp;
    UINT nID = 0;

    for(nID = 0; nID < 5; nID++)
    {
        CValue* pValue = new CValue();

        pValue->nValue = nID;
        pValue->sValue.Format("value-%u", nID);
        Mp.Attach((PVOID)pValue, sizeof(class CValue));
    }

    // check attached buffer length
    std::cout << "attached buffer length : " << Mp.GetPos() << std::endl;

    // access attached memory
    CHAR* pPtr = (CHAR*)Mp.GetPtr();

    for(nID = 0; nID < 5; nID++)
    {
        CValue* pValue = (CValue*)pPtr;
        if(NULL != pValue)
        {
            std::cout << "nValue : " << pValue->nValue << " / sValue : " << LPCSTR(pValue->sValue) << std::endl;
        }
        pPtr += sizeof(class CValue);
    }

    // free memory
    Mp.Free();
    std::cout << "buffer length after freed : " << Mp.GetPos() << std::endl;

    return 0;
}

[results]
attached buffer length : 160
nValue : 0 / sValue : value-0
nValue : 1 / sValue : value-1
nValue : 2 / sValue : value-2
nValue : 3 / sValue : value-3
nValue : 4 / sValue : value-4
buffer length after freed : 0

@endcode
*/

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
/// memory container class
class WCMemPool
{
public :
	VOID  Init     ();
	VOID  Free     ();
	VOID  SetSize  (IN UINT nlen);
	VOID  MoveFirst();
    VOID  MemCpy   (IN VOID* pptr, IN UINT nlen);
	VOID  Attach   (IN VOID* pptr, IN UINT nlen);

	VOID  SetUserPtr(IN VOID* pptr) { m_pUserPtr = pptr; };
    CHAR* GetPtr    ()              { return m_pMem; };
	CHAR* GetWorkPtr()              { return m_pWork; }
	UINT  GetSize   ()              { return m_nMem; };
	UINT  GetPos    ()              { return m_nPos; };
	VOID* GetUserPtr()              { return m_pUserPtr; };

    WCMemPool();
    ~WCMemPool();

public :
    CHAR*  m_pMem;  ///< allocated memory starting pointer
	CHAR*  m_pWork; ///< current working position pointer
    UINT   m_nMem;  ///< allocated memory size (length)
	UINT   m_nPos;  ///< current working position

	// user poiner
	PVOID  m_pUserPtr;
};

//-----------------------------------------------------------------------------
#endif // __SWC_MEM_POOL_H__
 
