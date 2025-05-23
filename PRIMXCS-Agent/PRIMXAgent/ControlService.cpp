//----------------------------------------------------------------
// 윈도우 서비스 프로그램의 서비스 관리 클래스.
//
//----------------------------------------------------------------


#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ControlService.h"
#include <conio.h>
#include <stdio.h>
#include "resource.h"
#include "FileVersion.h"
#include "PRIMXAgentManager.h"
#include "ProcessRun.h"
#include "ZombieProcessCheck.h"

CControlService::CControlService(const TCHAR* szServiceName,const TCHAR* szServiceDisName,const TCHAR* szServiceDesc)
:CNTService(szServiceName,szServiceDisName,szServiceDesc)
{
	try
	{
		m_pPRIMXAgentManager = NULL;
		m_tAgentCSRunCheckTime = 0;

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
	catch(...)
	{
	}

}
CControlService::~CControlService(void)
{
	try
	{
		SetClose();

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
	catch(...)
	{
	}
}

void CControlService::ServiceMainEx(DWORD dwArgc, LPTSTR *lpszArgv)
{
	try
	{
		for( int i=1;i<int(dwArgc);i++ )
		{
			if( _tcsicmp(lpszArgv[i], _T("-lw")) == 0 )
			{
				theLogManager.SetDebugLogWrite();
			}
		} 
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
	catch(...)
	{
	}
}

void CControlService::SetWindowsFirewall()
{


}

BOOL CControlService::OnInit()
{	

	try
	{
		CFileVersion FileVersion;
		FileVersion.Init();
		theLogManager.SetPath(_T("Agent"), GetProgramDataPath());
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("=========================================================================================="));
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Service Start. v%s"), FileVersion.GetFileVersion());
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("=========================================================================================="));
		
		GetConfig();

#ifdef _DEBUG
		CString strXMLPath;
		strXMLPath.Format(_T("%s\\XML"), GetProgramDataPath());
		SetCreateDirectoryEx(strXMLPath);	
#endif

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
	catch(...)
	{
	}	
	
	return TRUE;
}

void CControlService::Run()
{
	try
	{
		if( SetCreateObject() == FALSE )
		{
			SetDeleteObject();
			LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_FAILEDINIT);
			return;
		}

		__time64_t tCheckTime = 0;
		__time64_t tEncMonitor = 0;
		int nLogRemoveHour = -1;

		UINT nCount = 0;

		while(m_bIsRunning)
		{
			nCount++;

			if (10000 < nCount) {
				COleDateTime tCurrentTime = COleDateTime::GetCurrentTime();
				if (nLogRemoveHour != tCurrentTime.GetHour()) {
					theLogManager.SetLogFileDelete(30);
					nLogRemoveHour = tCurrentTime.GetHour();
				}
				nCount = 0;
			}

			__time64_t tCurrentTime = _time64(NULL);

			if (tCheckTime < tCurrentTime) {
				SetPRIMXAgentCSRun();
				tCheckTime = _time64(NULL) + 2;
			}

			if (tEncMonitor < tCurrentTime) {
				SetPRIMXEncMonitorRun();
				tEncMonitor = _time64(NULL) + 10;
			}

			if(0 < _kbhit())
			{
				int Key = _getch();
				if(27 == Key)
				{
					theLogManager.LogPrint(LOG_INF,GetCurrentThreadId(),_T("The service stop [ESC key down] .....!!"));
					break;
				}
				else if(13 == Key)
				{
					_ftprintf(stderr,_T("\n"));
				}
				else if(45 == Key)
				{
					theLogManager.LogPrintLine();
				}
				else if(8 == Key)
				{
					system("cls");
				}
				else
				{
					_ftprintf(stderr,_T("%c"),Key);
				}
			}
			Sleep(1000);		
		}
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
	catch(...)
	{
	}
}

void CControlService::OnShutdown()
{
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Windows 시스템 종료 이벤트 발생"));
	if (NULL != m_pPRIMXAgentManager) {
		m_pPRIMXAgentManager->SetShutdown();
	}
}

void CControlService::OnStop()
{
	try
	{


		SetClose();
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
	catch(...)
	{
	}
}

void CControlService::SetClose()
{
	try
	{		
		SetDeleteObject();
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
	catch(...)
	{
	}
}


BOOL CControlService::OnUserControl(DWORD dwOpcode)
{
	BOOL bRe = FALSE;
	try
	{
		switch (dwOpcode) 
		{
		case SERVICE_CONTROL_USER + 0:
			bRe = TRUE;
			break;
		default:
			break;
		}
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
	catch(...)
	{
	}

	return bRe; // say not handled
}

BOOL CControlService::ParseStandardArgs(int argc, TCHAR *argv[])
{
	CString StringTable;
	PutCopyright();
	CString strServiceDispayName;
	strServiceDispayName.LoadString(IDS_SERVICE_SERVICE_DISPLAY_NAME);

	if ((argc <= 1) || (_stricmp(WtoAConverter(argv[1]), "-?") == 0) || (_stricmp(WtoAConverter(argv[1]), "/?") == 0)){
		_ftprintf(stderr,_T("Service Name : %s (%s) \n"),m_szServiceName,(LPCTSTR)strServiceDispayName);
		_ftprintf(stderr,_T("Arguments : \n"));
		StringTable.LoadString(IDS_STRING25);
		_ftprintf(stderr,_T("%s"),LPCTSTR(StringTable));
		StringTable.LoadString(IDS_STRING26);
		_ftprintf(stderr,_T("%s"),LPCTSTR(StringTable));
		StringTable.LoadString(IDS_STRING27);
		_ftprintf(stderr,_T("%s"),LPCTSTR(StringTable));
		StringTable.LoadString(IDS_STRING28);
		_ftprintf(stderr,_T("%s"),LPCTSTR(StringTable));

		return FALSE;
	}

	if((_stricmp(WtoAConverter(argv[1]), "-v") == 0) || (_stricmp(WtoAConverter(argv[1]), "/v") == 0)){
		CFileVersion Version;
		Version.Init();
		CString XeVersion = Version.GetProductVersion();
		XeVersion.Replace(_T(" "),_T(""));
		XeVersion.Replace(_T(","),_T("."));
		_ftprintf(stderr,_T("Service Name : %s (%s) \n"),m_szServiceName, (LPCTSTR)strServiceDispayName);
		_ftprintf(stderr,_T("Version : %s Build %s \n"),LPCTSTR(XeVersion),LPCTSTR(Version.GetPrivateBuild()) );
		_ftprintf(stderr,_T("Install : %s \n"),IsInstalled() ? _T("YES") : _T("NO"));
		_ftprintf(stderr,_T("Description  : %s \n"),m_szServiceDesc);
		return TRUE; 

	} else if((_stricmp(WtoAConverter(argv[1]), "-i") == 0) || (_stricmp(WtoAConverter(argv[1]), "/i") == 0)) {

		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"서비스 설치 모드 시작");

		if (IsInstalled()) {
			StringTable.LoadString(IDS_STRING16);
			_ftprintf(stderr,LPCTSTR(StringTable), strServiceDispayName);
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"서비스 설치 모드 > %s", StringTable);

		} else {
			if (Install()) {
				StringTable.LoadString(IDS_STRING17);
				_ftprintf(stderr,LPCTSTR(StringTable), strServiceDispayName);
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"서비스 설치 모드 > [%s]프로그램이 정상적으로 설치 되었습니다", strServiceDispayName);

				if (3 == argc) {
					CString strVersion = argv[2];
					strVersion.Trim();

					if (0 < strVersion.GetLength()) {
						CConfigDataManager ConfigDataManager;
						ConfigDataManager.SetAgentVersion(strVersion);
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"서비스 설치 모드 > 버전 : %s", strVersion);
					}
				}
			} else {
				_ftprintf(stderr,_T("[%s]프로그램 설치중 에러가 발생 하였습니다. %s \n"), (LPCTSTR)strServiceDispayName, (LPCTSTR)m_strErrorString);
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"서비스 설치 모드 > [%s]프로그램 설치중 에러가 발생 하였습니다. %s ", strServiceDispayName, m_strErrorString);
			}
		}
		return TRUE; // say we processed the argument

	} else if((_stricmp(WtoAConverter(argv[1]), "-u") == 0) || (_stricmp(WtoAConverter(argv[1]), "/u") == 0)) {

		CString strName = GetLocalFileName();
		CProcessRun ProcessRun;
		ProcessRun.SetKillProcess(L"PRIMXEncMonitor.exe");
		ProcessRun.SetKillProcess(L"PRIMXAgentCS.exe");
		if (0 != strName.CompareNoCase(L"PRIMXAgent.exe")) {
			ProcessRun.SetKillProcess(L"PRIMXAgent.exe");
		}
		
		m_strErrorString = _T("");
		if (!IsInstalled()) {
			if(0 < m_strErrorString.GetLength()){
				_ftprintf(stderr,_T("[%s]프로그램 삭제 중 에러가 발생 하였습니다. %s \n"), (LPCTSTR)strServiceDispayName, (LPCTSTR)m_strErrorString);
			}
			else{
				StringTable.LoadString(IDS_STRING19);
				_ftprintf(stderr,LPCTSTR(StringTable), strServiceDispayName);
			}
		} else {

			if(FALSE== IsServiceStart()){
				if (Uninstall()) {
					TCHAR szFilePath[_MAX_PATH];
					::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
					StringTable.LoadString(IDS_STRING20);
					_ftprintf(stderr,LPCTSTR(StringTable), strServiceDispayName);
					_ftprintf(stderr,_T("Path : %s \n"),szFilePath);
				} else {
					_ftprintf(stderr,_T("[%s]프로그램 삭제중 에러가 발생 하였습니다. %s \n"), (LPCTSTR)strServiceDispayName, (LPCTSTR)m_strErrorString);
				}
			}
			else{
				_ftprintf(stderr,_T("[%s] 서비스가 구동 중 입니다. 서비스 중지 후 삭제가 가능합니다.\n"), (LPCTSTR)strServiceDispayName);
			}

		}
		return TRUE; // say we processed the argument

	}
	return FALSE;
}


BOOL CControlService::SetCreateObject()
{
	BOOL bRe = FALSE;
	try
	{
		m_pPRIMXAgentManager = new CPRIMXAgentManager;
		m_pPRIMXAgentManager->SetCreate();

		bRe = TRUE;
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

	return bRe;
}

void CControlService::SetDeleteObject()
{
	try
	{
		if (NULL != m_pPRIMXAgentManager) {
			m_pPRIMXAgentManager->SetClose();
			_SafeDelete(m_pPRIMXAgentManager);
		}
	


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
}

void CControlService::SetServiceAllStop()
{
	try
	{	


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
	catch(...)
	{
	}

}

void CControlService::GetConfig()
{


}
CString CControlService::GetLocalPath()
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

void CControlService::SetPRIMXEncMonitorRun()
{
	CProcessRun ProcessRun;
	if (TRUE == ProcessRun.isProcess(PRIMX_ENC_MONITOR_NAME)) {
		return;
	}

	DWORD conSessId = ProcessRun.WTSGetActiveConsoleSessionIdEx();
	if (0xFFFFFFFF == conSessId) {
		conSessId = 0;
	}

	BOOL bProcess = ProcessRun.isProcessSessionWithKill(PRIMX_ENC_MONITOR_NAME, conSessId);
	if (TRUE == bProcess) {
		// 좀비가 존재하면 중복 실행 가능 하도록 예외 처리함.
		CZombieProcessCheck ZombieProcessCheck;
		if (TRUE == ZombieProcessCheck.SetZombieProcessCheck(PRIMX_ENC_MONITOR_NAME)) {
			bProcess = FALSE;
		}
	}
	if (FALSE == bProcess) {
		CString strEncMonitorPath;
		strEncMonitorPath.Format(_T("%s\\%s"), ProcessRun.GetLocalPath(), PRIMX_ENC_MONITOR_NAME);
		if (TRUE == ProcessRun.CreateProcessOnAdminAccountEx(strEncMonitorPath, PRIMX_SW_HIDE)) {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s execution complete.", PRIMX_ENC_MONITOR_NAME);
		}
		else {
			if (0 != GetLastError()) {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s execution fail : %s", PRIMX_ENC_MONITOR_NAME, ProcessRun.GetLastErrorString());
			}
		}
	}

}

void CControlService::SetPRIMXAgentCSRun(BOOL bNow)
{
	__time64_t tCurrentTime = _time64(NULL);

	if ((PRIMX_AGNET_CS_RUN_CYCLE < (tCurrentTime - m_tAgentCSRunCheckTime)) || (TRUE == bNow)) {
		CProcessRun ProcessRun;
		if (TRUE == ProcessRun.isProcess(PRIMX_AGNET_CS_NAME)) {
			m_tAgentCSRunCheckTime = _time64(NULL);
			return;
		}

		DWORD conSessId = ProcessRun.WTSGetActiveConsoleSessionIdEx();
		if (0xFFFFFFFF == conSessId) {
			conSessId = 0;
		}

		BOOL bProcess = ProcessRun.isProcessSessionWithKill(PRIMX_AGNET_CS_NAME, conSessId);
		if (TRUE == bProcess) {
			// 좀비가 존재하면 중복 실행 가능 하도록 예외 처리함.
			CZombieProcessCheck ZombieProcessCheck;
			if (TRUE == ZombieProcessCheck.SetZombieProcessCheck(PRIMX_AGNET_CS_NAME)) {
				bProcess = FALSE;
			}
		}
		if (FALSE == bProcess) {
			CString strAgentCSPath;
			strAgentCSPath.Format(_T("%s\\%s"), ProcessRun.GetLocalPath(), PRIMX_AGNET_CS_NAME);
			if (TRUE == ProcessRun.CreateProcessOnUserAccountEx(strAgentCSPath)) {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s execution complete.", PRIMX_AGNET_CS_NAME);
			}
			else {
				if (0 != GetLastError()) {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s execution fail : %s", PRIMX_AGNET_CS_NAME, ProcessRun.GetLastErrorString());
				}
			}
		}
		m_tAgentCSRunCheckTime = _time64(NULL);
	}
}
