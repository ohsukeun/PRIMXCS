
// PRIMXAgentCS.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPRIMXAgentCSApp:
// �� Ŭ������ ������ ���ؼ��� PRIMXAgentCS.cpp�� �����Ͻʽÿ�.
//

class CPRIMXAgentCSApp : public CWinApp
{
public:
	CPRIMXAgentCSApp();
	CString GetLocalPath();
	CString GetLogPath();
	

	HANDLE m_hMutex;

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CPRIMXAgentCSApp theApp;