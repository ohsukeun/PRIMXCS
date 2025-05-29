/*****************************************************************************
 *                                                                           *
 * File Name : swc_ptr_queue.h                                               *
 * Theme     : std:map wrapper class (queue)                                 *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '10. 06. 06                     *
 *                                                                           *
 *****************************************************************************/

#include <swc_queue.h>
#include <swc_safty.h>

#ifndef __SWC_PTR_QUEUE_H__
#define __SWC_PTR_QUEUE_H__

//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------
/**
 * @file swc_array.h
 * @brief pointer queue inherited from WCQueue
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

    // queue define
    WCPtrQueue qPtr;
    UINT nID = 0;

    // queue size should be defined ~!!
    qPtr.SetSize(100);
    std::cout << "queue size: " << qPtr.GetSize() << std::endl;

    // push data
    for(nID = 0; nID < 5; nID++)
    {
        CValue* pValue = new CValue();

        pValue->nValue = nID;
        pValue->sValue.Format("value-%u", nID);

        qPtr.Push((PVOID)pValue);
    }
    std::cout << "queue count : " << qPtr.GetCount() << std::endl;

    // pop from queue
    if(0 < qPtr.GetCount())
    {
        PVOID pOut = NULL;

        while(TRUE == qPtr.Pop(pOut))
        {
            CValue* pValue = (CValue*)pOut;
            if(NULL != pValue)
            {
                std::cout << "poped nValue : " << pValue->nValue << " / sValue : " << LPCSTR(pValue->sValue) << std::endl;
                delete pValue;
            }
        }
    }
    std::cout << "queue count : " << qPtr.GetCount() << std::endl;

    return 0;
}

[results]
queue size: 100
queue count : 5
poped nValue : 0 / sValue : value-0
poped nValue : 1 / sValue : value-1
poped nValue : 2 / sValue : value-2
poped nValue : 3 / sValue : value-3
poped nValue : 4 / sValue : value-4
queue count : 0

@endcode
*/

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
/// pointer queue class
class WCPtrQueue : public WCQueue<VOID*>
{
public :
	SWC_BOOL Push(IN VOID* rData)
	{
		SWC_BOOL         bret = true;
		unsigned int ncur = 0;

		Lock();
		ncur = (unsigned int)m_Queue.size();
		if(ncur < m_nSize) m_Queue.push_back(rData);
		else if(ncur >= m_nSize)
		{
			free(m_Queue.front());
			m_Queue.pop_front();
			m_Queue.push_back(rData); 
			bret = false;
		}
		Unlock();

		return bret;
	}
	SWC_BOOL Pop(OUT VOID*& rData)
	{
		SWC_BOOL bret = true;

		Lock();
		if(0 < m_Queue.size())
		{
			rData = m_Queue.front();
			m_Queue.pop_front();
		}
		else
		{
			bret = false;
		}
		Unlock();

		return bret;
	}
	void RemoveAll()
	{
		while(0 < m_Queue.size())
		{
			VOID* pout = NULL;

			Pop(pout);
			if(NULL != pout) free(pout);
		}
		m_Queue.clear();
	}
};

//-----------------------------------------------------------------------------
#endif // __SWC_PTR_QUEUE_H__
 
