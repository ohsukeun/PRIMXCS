
// PRIMXDecryptFile.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPRIMXDecryptFileApp:
// �� Ŭ������ ������ ���ؼ��� PRIMXDecryptFile.cpp�� �����Ͻʽÿ�.
//

class CPRIMXDecryptFileApp : public CWinApp
{
public:
	CPRIMXDecryptFileApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	CString GetLogPath();
	void SetSelfDelete();

	HANDLE m_hMutex;


// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CPRIMXDecryptFileApp theApp;