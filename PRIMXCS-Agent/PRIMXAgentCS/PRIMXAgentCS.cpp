
// PRIMXAgentCS.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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


// CPRIMXAgentCSApp 생성

CPRIMXAgentCSApp::CPRIMXAgentCSApp()
{
	m_hMutex = NULL;
}


// 유일한 CPRIMXAgentCSApp 개체입니다.

UINT g_uCustomClipbrdFormat = RegisterClipboardFormat(_T("SNAC_3860BD1D-EFD2-4FFD-AAA6-0B04B17BB1D9"));
BOOL CSNIDSocket::m_bSocketInit = FALSE;
BOOL CSNIDSocket::m_bSSLInit = FALSE;

CPRIMXAgentCSApp theApp;
CLogManager theLogManager;

// CPRIMXAgentCSApp 초기화

BOOL CPRIMXAgentCSApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// OLE 라이브러리를 초기화합니다.
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

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("PRIMXAgentCS@Rimahr"));

	::PostMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)TEXT("Environment"));

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMXAgentCS Dlg 생성");

	static const TCHAR szProgName[] = _T("PRIMXAgentCS@Rimahr");
	m_hMutex = CreateMutex(NULL, TRUE, szProgName);
	if (ERROR_ALREADY_EXISTS == GetLastError()){
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMXAgentCS.exe가 이미 실행된 상태임, 즉시 종료");
		return FALSE;
	}

	CPRIMXAgentCSDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
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
