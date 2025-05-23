
// PRIMXUninstall.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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


// CPRIMXUninstallApp 생성

CPRIMXUninstallApp::CPRIMXUninstallApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CPRIMXUninstallApp 개체입니다.

CPRIMXUninstallApp theApp;


// CPRIMXUninstallApp 초기화

BOOL CPRIMXUninstallApp::InitInstance()
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


	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		
	SetRegistryKey(_T("PRIMXUninstall@Rimahr"));

	// 인자값 처리
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
	//	// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
	//	//  코드를 배치합니다.
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
	//	//  코드를 배치합니다.
	//}
	//else if (nResponse == -1)
	//{
	//	TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 응용 프로그램이 예기치 않게 종료됩니다.\n");
	//	TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	//}

	// 위에서 만든 셸 관리자를 삭제합니다.
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

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
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

	// batch파일을 생성합니다.
	errno_t err = _tfopen_s(&fp, LPCTSTR(strBatFileName), _T("wt"));
	if (0 == err) {
		char szBatFile[512] = { 0 };
		fprintf(fp, ":Repeat \r\ndel /f /s /q \"%s\" \r\nif exist \"%s\" goto Repeat \r\ndel /s /q %s \r\n", WtoAConverter(strFileName), WtoAConverter(strFileName), WtoAConverter(strBatFileName));
		fclose(fp);
		ShellExecute(NULL, _T("open"), strBatFileName, NULL, NULL, 0);
	}

}

