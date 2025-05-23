/**
@file ThreadEx.h
@brief �����带 �����ϱ� ���� �⺻ Ŭ������ ��� ����.
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
/// @brief �����带 ����ϱ� ���� �⺻ �Լ��� ����.
class CThreadEx  
{
public:
	/// @brief ������
	/// @return ����.
	CThreadEx(
		UINT nStackSize = 0,					/**<  Stack Size*/
		UINT nPriority = THREAD_PRIORITY_NORMAL	/**<  ���� Priority */
		);

	/// @brief �Ҹ���
	/// @return ����.
	virtual ~CThreadEx();

public:
	/// @brief �����带 �����ϴ� �Լ�.
	/// @return ���� ����. TRUE : ���� , FALSE : ����.
	BOOL Start();

	/// @brief �����带 �����ϴ� �Լ�.
	/// @return ���� ����. TRUE : ���� , FALSE : ����.
	BOOL WaitForTermination(DWORD dwMilliSeconds = INFINITE /**<  ���� ��� �ð�(MilliSeconds) */);

	// Thread Priority
	/// @brief �������� ���� Priority ����. 
	/// @return void.
	void SetPriority(int nPriority /**<  ���� Priority */) ;

	/// @brief ���� Priority ��ȯ�ϴ� �Լ�
	/// @return ���� Priority
	int GetPriority() const;

public:

	/// @brief �����忡�� �����ϴ� work�Լ�.\n���������Լ��� CThreadEx�� ��ӹ޴� Ŭ������ �ݵ�� �����ؾ���.
	/// @return void.
	virtual void Run() = 0;

private:
	/// @brief ���������� ȣ��Ǵ� ������ �Լ��̸� �� �Լ����� Run()�Լ��� ȣ���Ѵ�.
	/// @return ���� ����. TRUE : ���� , FALSE : ����.
	static unsigned __stdcall __THREAD_ENTRY_POINT(void* /**<  void�� ������ */);

protected:
	DWORD m_dwThreadID;	/**<  ������ ID */
	HANDLE m_hThread;	/**<  ������ �ڵ� */
	int m_nStackSize;	/**<  Stack Size */
	int m_nPriority;	/**<  ������ Priority */

};

#endif // !defined(AFX_THREADEX_H__92CF910C_F832_428C_9E29_C1D7D04FB6E7__INCLUDED_)
