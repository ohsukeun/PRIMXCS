/**
@file ThreadEx.h
@brief 쓰레드를 구동하기 위한 기본 클래스의 헤더 파일.
@date 2010/04/08
@version 0.1
@author hylee
*/

#if !defined(AFX_THREADEX_H__92CF910C_F832_428C_9E29_C1D7D04FB6E7__INCLUDED_)
#define AFX_THREADEX_H__92CF910C_F832_428C_9E29_C1D7D04FB6E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <process.h>
#include <time.h>


/// @class CThreadEx
/// @brief 쓰레드를 사용하기 위한 기본 함수를 제공.
class CThreadEx  
{
public:
	/// @brief 생성자
	/// @return 없음.
	CThreadEx(
		UINT nStackSize = 0,					/**<  Stack Size*/
		UINT nPriority = THREAD_PRIORITY_NORMAL	/**<  실행 Priority */
		);

	/// @brief 소멸자
	/// @return 없음.
	virtual ~CThreadEx();

public:
	/// @brief 쓰레드를 시작하는 함수.
	/// @return 성공 여부. TRUE : 성공 , FALSE : 실패.
	BOOL Start();

	/// @brief 쓰레드를 종료하는 함수.
	/// @return 성공 여부. TRUE : 성공 , FALSE : 실패.
	BOOL WaitForTermination(DWORD dwMilliSeconds = INFINITE /**<  정료 대기 시간(MilliSeconds) */);

	// Thread Priority
	/// @brief 쓰레드의 실행 Priority 설정. 
	/// @return void.
	void SetPriority(int nPriority /**<  실행 Priority */) ;

	/// @brief 실행 Priority 반환하는 함수
	/// @return 실행 Priority
	int GetPriority() const;

public:

	/// @brief 쓰레드에서 실행하는 work함수.\n순수가상함수로 CThreadEx를 상속받는 클래스는 반드시 구현해야함.
	/// @return void.
	virtual void Run() = 0;

private:
	/// @brief 내부적으로 호출되는 쓰레드 함수이며 이 함수에서 Run()함수를 호출한다.
	/// @return 성공 여부. TRUE : 성공 , FALSE : 실패.
	static unsigned __stdcall __THREAD_ENTRY_POINT(void* /**<  void형 포인터 */);

protected:
	DWORD m_dwThreadID;	/**<  쓰레드 ID */
	HANDLE m_hThread;	/**<  쓰레드 핸들 */
	int m_nStackSize;	/**<  Stack Size */
	int m_nPriority;	/**<  쓰레드 Priority */

};

#endif // !defined(AFX_THREADEX_H__92CF910C_F832_428C_9E29_C1D7D04FB6E7__INCLUDED_)
