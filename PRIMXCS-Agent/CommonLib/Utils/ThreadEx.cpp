/**
@file ThreadEx.cpp
@brief 쓰레드를 구동하기 위한 기본 클래스의 CPP 파일.
@date 2010/04/01
@version 1.0
@author hylee
*/


#include "stdafx.h"
#include "ThreadEx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadEx::CThreadEx( UINT nStackSize, UINT nPriority)
{
	m_hThread		= NULL;
	m_nStackSize	= nStackSize;
	m_nPriority		= nPriority;
}

CThreadEx::~CThreadEx()
{
	::CloseHandle(m_hThread);
	m_hThread = NULL;
}

unsigned __stdcall CThreadEx::__THREAD_ENTRY_POINT(void* pVoid)
{
	CThreadEx* pThis = (CThreadEx*)pVoid;
	pThis->Run();
	_endthreadex(0);
	return 0;
}

BOOL CThreadEx::Start()
{
	m_hThread = (HANDLE) _beginthreadex(	0, 
											m_nStackSize,
											CThreadEx::__THREAD_ENTRY_POINT,
											this, 
											0, 
											(unsigned *)&m_dwThreadID);
	if(NULL == m_hThread) return FALSE;

	return TRUE;
}

BOOL CThreadEx::WaitForTermination(DWORD dwMilliSeconds /*= INFINITE*/)
{
	if(NULL == m_hThread) return TRUE;	
	if(WAIT_OBJECT_0 != ::WaitForSingleObject(m_hThread, dwMilliSeconds)){
		::TerminateThread(m_hThread, 0);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	return TRUE;
}

void CThreadEx::SetPriority(int nPriority)
{
	if(NULL == m_hThread) return;
	m_nPriority = nPriority;
	::SetThreadPriority( m_hThread, nPriority);
}


int CThreadEx::GetPriority() const
{
	return ::GetThreadPriority(m_hThread);
}

