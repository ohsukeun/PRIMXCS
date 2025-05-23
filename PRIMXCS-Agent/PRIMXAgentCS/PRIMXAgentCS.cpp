
// PRIMXAgentCS.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "PRIMXAgentCS.h"
#include "PRIMXAgentCSDlg.h"
#include "SNIDSocket.h"
#include "UserRegDlg.h"
#include "LogManager.h"
#include "CommonModule.h"
#include "FileVersion.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPRIMXAgentCSApp

BEGIN_MESSAGE_MAP(CPRIMXAgentCSApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPRIMXAgentCSApp ����

CPRIMXAgentCSApp::CPRIMXAgentCSApp()
{
	m_hMutex = NULL;
}


// ������ CPRIMXAgentCSApp ��ü�Դϴ�.

UINT g_uCustomClipbrdFormat = RegisterClipboardFormat(_T("SNAC_3860BD1D-EFD2-4FFD-AAA6-0B04B17BB1D9"));
BOOL CSNIDSocket::m_bSocketInit = FALSE;
BOOL CSNIDSocket::m_bSSLInit = FALSE;

CPRIMXAgentCSApp theApp;
CLogManager theLogManager;

// CPRIMXAgentCSApp �ʱ�ȭ

BOOL CPRIMXAgentCSApp::InitInstance()
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

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		return FALSE;
	}

	if (3 == __argc) {
		CString strMode = __targv[1];
		DWORD dwExitCode = (DWORD)_ttof(__targv[2]);

		strMode.Trim();
		if (0 == strMode.CompareNoCase(_T("-AlertCode"))) {

			CCommonModule CommonModule;
			CommonModule.SetProcessCommandLineCheckForKill(_T("PRIMXAgentCS.exe"),_T("-AlertCode"));

			if (0x01 == dwExitCode) {
				ALERT_MSGBOX(IDS_ALERT_MSG_01);
			}
			if (0x02 == dwExitCode) {
				ALERT_MSGBOX(IDS_ALERT_MSG_02);
			}
			if (0x03 == dwExitCode) {
				ALERT_MSGBOX(IDS_ALERT_MSG_02);
			}
			return FALSE;
		}
	}	

	CFileVersion FileVersion;
	FileVersion.Init();
	theLogManager.SetPath(_T("AgentCS"), GetLogPath());
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("=========================================================================================="));
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("PRIMXAgentCS Start. v%s"), FileVersion.GetFileVersion());
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("=========================================================================================="));
	
	AfxEnableControlContainer();

	// ��ȭ ���ڿ� �� Ʈ�� �� �Ǵ�
	// �� ��� �� ��Ʈ���� ���ԵǾ� �ִ� ��� �� �����ڸ� ����ϴ�.
	CShellManager *pShellManager = new CShellManager;

	// MFC ��Ʈ���� �׸��� ����ϱ� ���� "Windows ����" ���־� ������ Ȱ��ȭ
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("PRIMXAgentCS@Rimahr"));

	::PostMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)TEXT("Environment"));

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMXAgentCS Dlg ����");

	static const TCHAR szProgName[] = _T("PRIMXAgentCS@Rimahr");
	m_hMutex = CreateMutex(NULL, TRUE, szProgName);
	if (ERROR_ALREADY_EXISTS == GetLastError()){
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMXAgentCS.exe�� �̹� ����� ������, ��� ����");
		return FALSE;
	}

	CPRIMXAgentCSDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	// ������ ���� �� �����ڸ� �����մϴ�.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�  ���� ���α׷��� ���� �� �ֵ��� FALSE��
	// ��ȯ�մϴ�.
	return FALSE;
}

CString CPRIMXAgentCSApp::GetLocalPath()
{
	CString Path;
	CString FileName;
	int pos = 0;
	int pos_b = 0;

	try
	{
		TCHAR szWindowsPath[MAX_PATH + 1] = { 0 };

		GetModuleFileName(NULL, szWindowsPath, sizeof(szWindowsPath));
		Path = FileName = szWindowsPath;

		while (1)
		{
			pos = FileName.Find(_T("\\"), pos);

			if (pos == -1)
			{
				FileName = FileName.Right(FileName.GetLength() - (pos_b));
				break;
			}
			else
			{
				pos = pos + 1;
				pos_b = pos;
			}
		}

		Path = Path.Left(Path.GetLength() - (FileName.GetLength() + 1));
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
	catch (...)
	{
	}
	return Path;
}

CString CPRIMXAgentCSApp::GetLogPath()
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

int CPRIMXAgentCSApp::ExitInstance()
{
	if (m_hMutex)
		CloseHandle(m_hMutex);
	m_hMutex = NULL;

	return CWinApp::ExitInstance();
}
