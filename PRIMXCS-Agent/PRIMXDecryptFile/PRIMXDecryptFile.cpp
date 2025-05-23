
// PRIMXDecryptFile.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "PRIMXDecryptFile.h"
#include "PRIMXDecryptFileDlg.h"
#include "LogManager.h"
#include "SNIDSocket.h"
#include "FileVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPRIMXDecryptFileApp

BEGIN_MESSAGE_MAP(CPRIMXDecryptFileApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPRIMXDecryptFileApp ����

CPRIMXDecryptFileApp::CPRIMXDecryptFileApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CPRIMXDecryptFileApp ��ü�Դϴ�.

CPRIMXDecryptFileApp theApp;
CLogManager theLogManager;

BOOL CSNIDSocket::m_bSocketInit = FALSE;
BOOL CSNIDSocket::m_bSSLInit = FALSE;


// CPRIMXDecryptFileApp �ʱ�ȭ

BOOL CPRIMXDecryptFileApp::InitInstance()
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


	// ��ȭ ���ڿ� �� Ʈ�� �� �Ǵ�
	// �� ��� �� ��Ʈ���� ���ԵǾ� �ִ� ��� �� �����ڸ� ����ϴ�.
	CShellManager *pShellManager = new CShellManager;

	// MFC ��Ʈ���� �׸��� ����ϱ� ���� "Windows ����" ���־� ������ Ȱ��ȭ
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("PRIMXDecryptFile@Rimahr"));

	CFileVersion FileVersion;
	FileVersion.Init();
	theLogManager.SetPath(_T("ExFile"), GetLogPath());
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("=========================================================================================="));
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("PRIMXDecryptFile Start. v%s"), FileVersion.GetFileVersion());
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("=========================================================================================="));
	
	static const TCHAR szProgName[] = _T("PRIMXDecryptFile@Rimahr");
	m_hMutex = CreateMutex(NULL, TRUE, szProgName);
	if (ERROR_ALREADY_EXISTS == GetLastError()) {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̹� ����� ������, ��� ����");
		CString strMsg;
		strMsg.LoadString(IDS_ERROR_STRING5);
		AfxMessageBox(strMsg);
		return FALSE;
	}
	
	CPRIMXDecryptFileDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ���⿡ [Ȯ��]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ���⿡ [���]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "���: ��ȭ ���ڸ� ������ �������Ƿ� ���� ���α׷��� ����ġ �ʰ� ����˴ϴ�.\n");
		TRACE(traceAppMsg, 0, "���: ��ȭ ���ڿ��� MFC ��Ʈ���� ����ϴ� ��� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS�� ������ �� �����ϴ�.\n");
	}



	// ������ ���� �� �����ڸ� �����մϴ�.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	if (TRUE == dlg.GetSelfDeleteFlag()) {
		SetSelfDelete();
	}

	return FALSE;
}

CString CPRIMXDecryptFileApp::GetLogPath()
{
	CString strLocalDBSavePath;

	TCHAR szPath[MAX_PATH] = { 0 };
	if (TRUE == SHGetSpecialFolderPath(NULL, szPath, CSIDL_COMMON_APPDATA, 0)) {
		strLocalDBSavePath.Format(PRIMX_AGNET_INSTALL_PATH_FORMAT, szPath);
	}
	else {
		if (0 < ::ExpandEnvironmentStrings(_T("%ProgramData%"), szPath, MAX_PATH)) {
			strLocalDBSavePath.Format(PRIMX_AGNET_INSTALL_PATH_FORMAT, szPath);
		}
		else {
			if (0 != GetWindowsDirectory(szPath, MAX_PATH)) {
				TCHAR pDrive[MAX_PATH] = { 0 };
				TCHAR pDir[MAX_PATH] = { 0 };
				TCHAR pFileName[MAX_PATH] = { 0 };
				TCHAR pFileExt[MAX_PATH] = { 0 };
				_tsplitpath_s(szPath, pDrive, MAX_PATH, pDir, MAX_PATH, pFileName, MAX_PATH, pFileExt, MAX_PATH);
				strLocalDBSavePath.Format(PRIMX_AGNET_INSTALL_PROGRAMDATA_PATH_FORMAT, pDrive);
			}
			else {
				strLocalDBSavePath.Format(PRIMX_AGNET_INSTALL_PROGRAMDATA_PATH_FORMAT, _T("c:"));
			}
		}
	}

	return strLocalDBSavePath;
}

void CPRIMXDecryptFileApp::SetSelfDelete()
{
	FILE *fp = NULL;

	TCHAR szLocalPath[MAX_PATH + 1] = { 0 };
	TCHAR pFilename[MAX_PATH] = { 0 };
	TCHAR pExt[MAX_PATH] = { 0 };
	CString strFileName;
	CString strBatFileName = _T("PRIMXDecryptRemove.bat");

	GetModuleFileName(NULL, szLocalPath, sizeof(szLocalPath));
	_tsplitpath(szLocalPath, NULL, NULL, pFilename, pExt);
	strFileName.Format(_T("%s%s"), pFilename, pExt);

	// batch������ �����մϴ�.
	errno_t err = _tfopen_s(&fp, LPCTSTR(strBatFileName), _T("wt"));
	if (0 == err) {
		char szBatFile[512] = { 0 };
		fprintf(fp, ":Repeat \r\ndel /f /s /q \"%s\" \r\nif exist \"%s\" goto Repeat \r\ndel /s /q %s \r\n", WtoAConverter(strFileName), WtoAConverter(strFileName), WtoAConverter(strBatFileName));
		fclose(fp);
		ShellExecute(NULL, _T("open"), strBatFileName, NULL, NULL, 0);
	}

}