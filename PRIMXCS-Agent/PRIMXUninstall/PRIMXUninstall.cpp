
// PRIMXUninstall.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "PRIMXUninstall.h"
#include "PRIMXUninstallDlg.h"

#include "UninstallManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPRIMXUninstallApp

BEGIN_MESSAGE_MAP(CPRIMXUninstallApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPRIMXUninstallApp ����

CPRIMXUninstallApp::CPRIMXUninstallApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CPRIMXUninstallApp ��ü�Դϴ�.

CPRIMXUninstallApp theApp;


// CPRIMXUninstallApp �ʱ�ȭ

BOOL CPRIMXUninstallApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�.
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ��ȭ ���ڿ� �� Ʈ�� �� �Ǵ�
	// �� ��� �� ��Ʈ���� ���ԵǾ� �ִ� ��� �� �����ڸ� ����ϴ�.
	CShellManager *pShellManager = new CShellManager;

	// MFC ��Ʈ���� �׸��� ����ϱ� ���� "Windows ����" ���־� ������ Ȱ��ȭ
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		
	SetRegistryKey(_T("PRIMXUninstall@Rimahr"));

	// ���ڰ� ó��
	BOOL bUninstall = FALSE;
	CString strCmd;
	if(2 == __argc){
		strCmd = __wargv[1];
		if (0 == strCmd.Compare(_T("Rimahr9280!"))) {
			CUninstallManager UninstallManager;
			UninstallManager.SetUninstall();
			bUninstall = TRUE;
		}
	}
	
	if (FALSE == bUninstall) {
		AfxMessageBox(PRIMX_AGNET_UNINSTALL_MSG, MB_ICONEXCLAMATION);
	}

	CPRIMXUninstallDlg dlg;
	m_pMainWnd = &dlg;

	//INT_PTR nResponse = dlg.DoModal();
	//if (nResponse == IDOK)
	//{
	//	// TODO: ���⿡ [Ȯ��]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
	//	//  �ڵ带 ��ġ�մϴ�.
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO: ���⿡ [���]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
	//	//  �ڵ带 ��ġ�մϴ�.
	//}
	//else if (nResponse == -1)
	//{
	//	TRACE(traceAppMsg, 0, "���: ��ȭ ���ڸ� ������ �������Ƿ� ���� ���α׷��� ����ġ �ʰ� ����˴ϴ�.\n");
	//	TRACE(traceAppMsg, 0, "���: ��ȭ ���ڿ��� MFC ��Ʈ���� ����ϴ� ��� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS�� ������ �� �����ϴ�.\n");
	//}

	// ������ ���� �� �����ڸ� �����մϴ�.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	if (TRUE == bUninstall) {
		SetSelfDelete();
	}

	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�  ���� ���α׷��� ���� �� �ֵ��� FALSE��
	// ��ȯ�մϴ�.
	return FALSE;
}

void CPRIMXUninstallApp::SetSelfDelete()
{
	FILE *fp = NULL;

	TCHAR szLocalPath[MAX_PATH + 1] = { 0 };
	TCHAR pFilename[MAX_PATH] = { 0 };
	TCHAR pExt[MAX_PATH] = { 0 };
	TCHAR pDrive[MAX_PATH] = { 0 };
	TCHAR pDir[MAX_PATH] = { 0 };

	CString strCurrentPath;
	CString strFileName;
	CString strBatFileName = _T("PRIMXUninstallEx.bat");

	GetModuleFileName(NULL, szLocalPath, sizeof(szLocalPath));
	_tsplitpath(szLocalPath, pDrive, pDir, pFilename, pExt);
	strFileName.Format(_T("%s%s"), pFilename, pExt);
	strCurrentPath.Format(_T("%s%s"), pDrive, pDir);

	SetCurrentDirectory(strCurrentPath);

	// batch������ �����մϴ�.
	errno_t err = _tfopen_s(&fp, LPCTSTR(strBatFileName), _T("wt"));
	if (0 == err) {
		char szBatFile[512] = { 0 };
		fprintf(fp, ":Repeat \r\ndel /f /s /q \"%s\" \r\nif exist \"%s\" goto Repeat \r\ndel /s /q %s \r\n", WtoAConverter(strFileName), WtoAConverter(strFileName), WtoAConverter(strBatFileName));
		fclose(fp);
		ShellExecute(NULL, _T("open"), strBatFileName, NULL, NULL, 0);
	}

}

