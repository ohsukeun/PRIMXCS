
// PRIMXUninstall.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPRIMXUninstallApp:
// �� Ŭ������ ������ ���ؼ��� PRIMXUninstall.cpp�� �����Ͻʽÿ�.
//

class CPRIMXUninstallApp : public CWinApp
{
public:
	CPRIMXUninstallApp();


	void SetSelfDelete();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CPRIMXUninstallApp theApp;