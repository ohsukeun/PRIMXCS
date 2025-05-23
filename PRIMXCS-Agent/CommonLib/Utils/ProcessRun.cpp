#include "StdAfx.h"
#include "ProcessRun.h"

#include <Tlhelp32.h>
#include <UserEnv.h>
#include <lm.h>
#include <tlhelp32.h>
#include <Wtsapi32.h>
#include <Psapi.h>
#include <regex> // ���Խ�

#pragma comment(lib,"UserEnv.lib")
#pragma comment(lib,"Wtsapi32.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment( lib, "netapi32.lib" )


CProcessRun::CProcessRun(void)
{
	m_wShowWindow = 0;
	m_dwExitCode = 0;
	m_dwMilliseconds = 5000;
	m_bWaitExitCodeOption = FALSE;
}

CProcessRun::~CProcessRun(void)
{

}
CString CProcessRun::GetLocalPath()
{
	CString Path;
	CString FileName;
	int pos		= 0;
	int pos_b	= 0;

	try
	{
		TCHAR szWindowsPath[MAX_PATH + 1] = {0};

		GetModuleFileName(NULL,szWindowsPath,sizeof(szWindowsPath));
		Path = FileName = szWindowsPath;

		while(1)
		{		
			pos = FileName.Find(_T("\\"),pos); 

			if( pos == -1 )
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

		Path = Path.Left(Path.GetLength() - (FileName.GetLength()+1)); 
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
	return Path;
}
BOOL CProcessRun::isProcess(CString strProcess)
{
	BOOL bResult = FALSE;	

	HANDLE hPToken = NULL;
	HANDLE hProcess = NULL;

	PROCESSENTRY32 procEntry;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		// ���ҽ�(�ڵ�) ������ ��� ��� ���� ó����. 
		// �Ϻ� OS���� CreateToolhelp32Snapshot ȣ�� �� CloseHandle�� �ص� Handle Leak �߻�. 
		DWORD dwError = GetLastError();
		if (1450 == dwError) { 
			exit(0);
		}
		return bResult;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry)) {
		CloseHandle(hSnap);
		return bResult;
	}
	
	do {
		if (CompareStringWithWildCard(procEntry.szExeFile, strProcess)) {
			bResult = TRUE;
			break;
		}
	}while (Process32Next(hSnap, &procEntry));

	CloseHandle(hSnap);

	return bResult; 
}

BOOL CProcessRun::isProcessSessionWithKill(CString strProcess, DWORD dwSessionID)
{
	BOOL bResult = FALSE;

	HANDLE hPToken = NULL;
	HANDLE hProcess = NULL;

	PROCESSENTRY32 procEntry;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		DWORD dwError = GetLastError();
		if (1450 == dwError) {
			exit(0);
		}
		return bResult;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry)) {
		CloseHandle(hSnap);
		return bResult;
	}
	do {
		if (0 == strProcess.CompareNoCase(procEntry.szExeFile)) {
			DWORD dwProcessSessionID = 0;
			if (TRUE == ProcessIdToSessionId(procEntry.th32ProcessID, &dwProcessSessionID)) {
				if (dwSessionID != dwProcessSessionID) {
					CString strKillProcessPath;
					CString strErrorString;
					SetKillProcess(strProcess, strKillProcessPath, strErrorString);
				}
				else {
					bResult = TRUE;
				}
			}
			else {
				bResult = TRUE;
			}
			break;
		}
	} while (Process32Next(hSnap, &procEntry));

	CloseHandle(hSnap);

	return bResult;
}

BOOL CProcessRun::isWindowsVistaLater()
{
	BOOL bRe = FALSE;
	OSVERSIONINFOEX OSVer;
	OSVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetOSVersion(&OSVer);
	if(OSVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if(OSVer.dwMajorVersion >= 6 && OSVer.dwMinorVersion >= 0) // Vista Later
		{
			bRe = TRUE;		
		}
	}
	return bRe;
}

BOOL CProcessRun::GetCurrentSessionAdmin()
{
	DWORD dwSessionId = WTSGetActiveConsoleSessionIdEx();
	
	DWORD dwBytesReturned = 0;

	LPTSTR pstrBuff = NULL;

	CString strUserName = _T("");

	WTSQuerySessionInformation(
		WTS_CURRENT_SERVER_HANDLE,
		dwSessionId,
		WTSUserName,
		&pstrBuff,
		&dwBytesReturned);

	strUserName = pstrBuff;

	WTSFreeMemory(pstrBuff);

	return IsAdminUser(strUserName);
}

BOOL CProcessRun::CreateProcessOnAdminOrUser(CString strProcess, WORD wShowWindow)
{
	BOOL bRe = FALSE;
		
	if (TRUE == isWindowsVistaLater()) {
		if (TRUE == GetUACEnable()) {
			bRe = CreateProcessOnAdminAccountEx(strProcess, wShowWindow);
			if (FALSE == bRe) {
				bRe = CreateProcessOnAdminEx(strProcess, wShowWindow);
				if (FALSE == bRe) {
					bRe = CreateProcessOnUserAccountEx(strProcess, wShowWindow);
				}
			}	
		}
		else {
			bRe = CreateProcessOnUserAccountEx(strProcess, wShowWindow);
			if (FALSE == bRe) {
				bRe = CreateProcessOnAdminAccountEx(strProcess, wShowWindow);
				if (FALSE == bRe) {
					bRe = CreateProcessOnAdminEx(strProcess, wShowWindow);
				}
			}
		}
	}
	else {
		bRe = CreateProcessOnUserAccountEx(strProcess, wShowWindow);
		if (FALSE == bRe) {
			bRe = CreateProcessOnAdminAccountEx(strProcess, wShowWindow);
			if (FALSE == bRe) {
				bRe = CreateProcessOnAdminEx(strProcess, wShowWindow);
			}
		}
	}
	return bRe;
}

BOOL CProcessRun::CreateProcessOnAdminAccount(CString strProcess, WORD wShowWindow)
{
	BOOL bRe = FALSE;
	if(TRUE == isWindowsVistaLater()){	
		bRe = CreateProcessOnAdminAccountEx(strProcess, wShowWindow);
		if (FALSE == bRe) {
			bRe = CreateProcessOnAdminEx(strProcess, wShowWindow);
			if (FALSE == bRe) {
				bRe = CreateProcessOnUserAccountEx(strProcess, wShowWindow);
			}	
		}
	}
	else{
		bRe = CreateProcessOnUserAccountEx(strProcess,wShowWindow);
		if(FALSE == bRe){
			bRe = CreateProcessOnAdminAccountEx(strProcess,wShowWindow);
			if (FALSE == bRe) {
				bRe = CreateProcessOnAdminEx(strProcess, wShowWindow);
			}
		}
	}
	return bRe;
}

BOOL CProcessRun::CreateProcessOnAdminAccountOfLockIgnore(CString strProcess, WORD wShowWindow)
{
	BOOL bRe = FALSE;
	UINT nStackSize = 0;			/**<  Stack Size*/
	DWORD dwThreadID = 0;			/**<  ������ ID */
	HANDLE hThread = NULL;			/**<  ������ �ڵ� */

	m_strProcess = strProcess;
	m_wShowWindow = wShowWindow;
	hThread = (HANDLE)_beginthreadex(0,	nStackSize, CProcessRun::__LOCK_IGNORE_RUN__,this,0,(unsigned *)&dwThreadID);
	
	if (NULL == hThread) return bRe;
	
	if (WAIT_OBJECT_0 != ::WaitForSingleObject(hThread, 3000 )) {
		::TerminateThread(hThread, 0);
		CloseHandle(hThread);
		hThread = NULL;
	}
	else {
		bRe = TRUE;
	}
	return bRe;
}

BOOL CProcessRun::CreateProcessOnAdminAccountOfLockIgnore()
{
	return LaunchAppIntoDifferentSession(m_strProcess);
}

unsigned __stdcall CProcessRun::__LOCK_IGNORE_RUN__(void* pVoid)
{
	CProcessRun* pThis = (CProcessRun*)pVoid;
	pThis->CreateProcessOnAdminAccountOfLockIgnore();
	_endthreadex(0);
	return 0;
}

CString CProcessRun::GetLastErrorString()
{
	CString strError;

	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	strError = (LPCTSTR)lpMsgBuf;
	strError.Replace('\n',' ');
	strError.Replace('\r',' ');
	LocalFree( lpMsgBuf );

	return strError;
}

CString CProcessRun::GetLastErrorStringEx()
{
	return m_strLastErrorString;

}

BOOL CProcessRun::CreateProcessOnAdminEx(CString strProcess, WORD wShowWindow)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL bResult = FALSE;
	DWORD dwSessionId = 0;
	DWORD winlogonPid = 0;
	HANDLE hUserToken = NULL;
	HANDLE hUserTokenDup = NULL;
	HANDLE hPToken = NULL;
	HANDLE hProcess = NULL;
	DWORD dwCreationFlags = 0;

	// Log the client on to the local computer.  

	dwSessionId = WTSGetActiveConsoleSessionIdEx();

	//////////////////////////////////////////  
	// Find the winlogon process  
	////////////////////////////////////////  

	PROCESSENTRY32 procEntry;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnAdminEx > CreateToolhelp32Snapshot fail > %s"), strProcess, GetLastErrorString());
		return FALSE;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry))
	{
		CloseHandle(hSnap);
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnAdminEx > Process32First fail > %s"), strProcess, GetLastErrorString());
		return FALSE;
	}

	do
	{
		if (_wcsicmp(procEntry.szExeFile, L"winlogon.exe") == 0)
		{
			// We found a winlogon process...make sure it's running in the console session  
			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId) && winlogonSessId == dwSessionId)
			{
				winlogonPid = procEntry.th32ProcessID;
				break;
			}
		}

	} while (Process32Next(hSnap, &procEntry));

	if (0 >= winlogonPid) {
		CloseHandle(hSnap);
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnAdminEx > winlogon.exe ���μ��� �������"), strProcess);
		return FALSE;
	}

	////////////////////////////////////////////////////////////////////////  

	WTSQueryUserToken(dwSessionId, &hUserToken);
	dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = L"winsta0\\default";
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = wShowWindow;
	ZeroMemory(&pi, sizeof(pi));
	TOKEN_PRIVILEGES tp;
	LUID luid;
	hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, winlogonPid);

	if (!::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY	| TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_SESSIONID | TOKEN_READ | TOKEN_WRITE, &hPToken))
	{
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnAdminEx > OpenProcessToken fail > %s"), strProcess, GetLastErrorString());
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	{
		TRACE(L"Lookup Privilege value Error: %u\n", GetLastError());
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	DuplicateTokenEx(hPToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hUserTokenDup);
	int dup = GetLastError();

	//Adjust Token privilege  
	SetTokenInformation(hUserTokenDup, TokenSessionId, (void*)dwSessionId, sizeof(DWORD));

	if (!AdjustTokenPrivileges(hUserTokenDup, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, NULL))
	{
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnAdminEx > AdjustTokenPrivileges fail > %s"), strProcess, GetLastErrorString());
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		TRACE(L"Token does not have the provilege\n");
	}

	LPVOID pEnv = NULL;

	if (CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE))
	{
		dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
	}
	else
		pEnv = NULL;

	// Launch the process in the client's logon session.  

	bResult = CreateProcessAsUser(
		hUserTokenDup,            // client's access token  
		NULL,              // file to execute  
		(LPTSTR)(LPCTSTR)strProcess,     // command line  
		NULL,              // pointer to process SECURITY_ATTRIBUTES  
		NULL,              // pointer to thread SECURITY_ATTRIBUTES  
		FALSE,             // handles are not inheritable  
		dwCreationFlags,  // creation flags  
		pEnv,              // pointer to new environment block   
		NULL,              // name of current directory   
		&si,               // pointer to STARTUPINFO structure  
		&pi                // receives information about new process  
	);
	// End impersonation of client.  

	if (TRUE == bResult)
	{
		SetForegroundWindow((HWND)pi.hProcess);

		//Wait until child process exits.
		if (TRUE == m_bWaitExitCodeOption)
		{
			if (WAIT_OBJECT_0 == WaitForSingleObject(pi.hProcess, m_dwMilliseconds)) // wait 5sec
			{
				DWORD dwExitCode = 0;
				if (TRUE == GetExitCodeProcess(pi.hProcess, &dwExitCode))
					m_dwExitCode = dwExitCode;
			}
		}
	}
	else{
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnAdminEx > CreateProcessAsUser fail > %s"), strProcess, GetLastErrorString());
	}
	
	//if (pi.hProcess != NULL) {
	//	/* ���α׷� ����ǰ� �ʱ�ȭ �� �Է¹��� �� �ִ� ���°� �Ǹ� �����ڵ�� ����*/
	//	DWORD dwWait = ::WaitForInputIdle(pi.hProcess, INFINITE);
	//	::CloseHandle(pi.hProcess);
	//}

	CloseHandle(hProcess);
	CloseHandle(hUserToken);
	CloseHandle(hUserTokenDup);
	CloseHandle(hPToken);

	return bResult;
}

BOOL CProcessRun::CreateProcessOnAdminAccountEx(CString strProcess, WORD wShowWindow)
{

	BOOL bResult = FALSE;	

	DWORD winlogonPid = 0;
	HANDLE hUserToken = NULL;
	HANDLE hUserTokenDup = NULL;
	HANDLE hPToken = NULL;
	HANDLE hProcess = NULL;

	PROCESSENTRY32 procEntry;

	DWORD dwSessionId = WTSGetActiveConsoleSessionIdEx();
	WTSQueryUserToken(dwSessionId, &hUserToken);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnAdminAccountEx > CreateToolhelp32Snapshot fail > %s"), strProcess, GetLastErrorString());
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s"), m_strLastErrorString);
		return bResult;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry)) {
		CloseHandle(hSnap);
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnAdminAccountEx > Process32First fail > %s"), strProcess, GetLastErrorString());
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s"), m_strLastErrorString);
		return bResult;
	}

	CString strExplorer = _T("explorer.exe");


	BOOL bFindIt = FALSE;
	do {
		if (0 == strExplorer.CompareNoCase(procEntry.szExeFile)) {
			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId))
			{				
				if (winlogonSessId == dwSessionId) {
					bFindIt = TRUE;
					winlogonPid = procEntry.th32ProcessID;
					break;
				}
				else{
					winlogonPid = procEntry.th32ProcessID;
				}
			}
		}
	}while (Process32Next(hSnap, &procEntry));

	if(FALSE == bFindIt && 0 == winlogonPid){
		CloseHandle(hSnap);
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnAdminAccountEx > explorer.exe ���μ��� �������"), strProcess);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s"), m_strLastErrorString);
		return bResult;
	}

	DWORD dwCreationFlags = NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, winlogonPid);

	::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY|TOKEN_DUPLICATE|TOKEN_ASSIGN_PRIMARY|TOKEN_ADJUST_SESSIONID|TOKEN_READ|TOKEN_WRITE, &hPToken);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	tp.PrivilegeCount =1;
	tp.Privileges[0].Luid =luid;
	tp.Privileges[0].Attributes =SE_PRIVILEGE_ENABLED;

	DuplicateTokenEx(hPToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hUserTokenDup);

	HANDLE hPrimaryToken = NULL;
	TOKEN_LINKED_TOKEN linkedToken = {0};
	DWORD dwSize = sizeof(linkedToken);
	if (GetTokenInformation(hPToken, TokenLinkedToken, &linkedToken, dwSize, &dwSize)) {
		if (DuplicateTokenEx(linkedToken.LinkedToken, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &hPrimaryToken)) { 			
			LPVOID pEnv =NULL;

			if(CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE)){
				dwCreationFlags|=CREATE_UNICODE_ENVIRONMENT;
			}
			else{
				pEnv=NULL;
			}

			PROCESS_INFORMATION ProcessInfo;
			STARTUPINFO StartupInfo;

			ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));
			ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
			StartupInfo.cb= sizeof(STARTUPINFO);
			StartupInfo.lpDesktop = _T("winsta0\\default");
			StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
			StartupInfo.wShowWindow = wShowWindow;
			bResult = CreateProcessAsUser(hPrimaryToken, NULL, (LPTSTR)(LPCTSTR)strProcess, NULL, NULL, FALSE, dwCreationFlags, pEnv, NULL, &StartupInfo, &ProcessInfo);

			if (TRUE == bResult) {

				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("���μ��� ����(%s) > CreateProcessOnAdminAccountEx > %lu"), strProcess, ProcessInfo.dwProcessId);

				SetForegroundWindow((HWND)ProcessInfo.hProcess);

				//Wait until child process exits.
				if (TRUE == m_bWaitExitCodeOption)
				{
					if (WAIT_OBJECT_0 == WaitForSingleObject(ProcessInfo.hProcess, m_dwMilliseconds)) // wait 5sec
					{
						DWORD dwExitCode = 0;
						if (TRUE == GetExitCodeProcess(ProcessInfo.hProcess, &dwExitCode))
							m_dwExitCode = dwExitCode;
					}
				}
			}
			
			CloseHandle(hPrimaryToken);
			CloseHandle(ProcessInfo.hProcess);
			CloseHandle(ProcessInfo.hThread);
			if (pEnv) DestroyEnvironmentBlock(pEnv);
		}
		else {
			m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnAdminAccountEx > DuplicateTokenEx fail > %s"), strProcess, GetLastErrorString());
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s"), m_strLastErrorString);
		}
	}
	else {
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnAdminAccountEx > GetTokenInformation fail > %s"), strProcess, GetLastErrorString());
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s"), m_strLastErrorString);
	}

	if (linkedToken.LinkedToken)
		CloseHandle(linkedToken.LinkedToken);
	CloseHandle(hUserToken);
	CloseHandle(hUserTokenDup);
	CloseHandle(hPToken);
	CloseHandle(hProcess);
	CloseHandle(hSnap);

	return bResult;

}

BOOL CProcessRun::CreateProcessOnUserAccount(CString strProcess, WORD wShowWindow)
{
	DWORD dwSessionId = 0;
	DWORD winlogonPid;
	DWORD dwCreationFlags;
	HANDLE hUserTokenDup;
	HANDLE hPToken;
	HANDLE hProcess;

	PROCESSENTRY32 procEntry;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnUserAccount > CreateToolhelp32Snapshot fail > %s"), strProcess,GetLastErrorString());
		return FALSE;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry)) {
		CloseHandle(hSnap);
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnUserAccount > Process32First fail > %s"), strProcess, GetLastErrorString());
		return FALSE;
	}

	CString strExplorer = _T("explorer.exe");

	BOOL bFindIt = FALSE;
	do {
		if (0 == strExplorer.CompareNoCase(procEntry.szExeFile)) {
			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId))// && winlogonSessId == dwSessionId)
			{
				winlogonPid = procEntry.th32ProcessID;
				bFindIt = TRUE;
				break;
			}
		}
	}while (Process32Next(hSnap, &procEntry));

	if(!bFindIt) {
		CloseHandle(hSnap);
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnUserAccount > explorer.exe ���μ��� ����"), strProcess);
		return FALSE;
	}

	dwCreationFlags = NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE;

	TOKEN_PRIVILEGES tp;
	LUID luid;
	hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, winlogonPid);

	::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY
		|TOKEN_DUPLICATE|TOKEN_ASSIGN_PRIMARY|TOKEN_ADJUST_SESSIONID
		|TOKEN_READ|TOKEN_WRITE, &hPToken);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	tp.PrivilegeCount =1;
	tp.Privileges[0].Luid =luid;
	tp.Privileges[0].Attributes =SE_PRIVILEGE_ENABLED;

	DuplicateTokenEx(hPToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hUserTokenDup);
	SetTokenInformation(hUserTokenDup, TokenSessionId, (LPVOID)dwSessionId, sizeof(DWORD));
	AdjustTokenPrivileges(hUserTokenDup, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, NULL);

	LPVOID pEnv =NULL;

	if(CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE))
	{
		dwCreationFlags|=CREATE_UNICODE_ENVIRONMENT;
	}
	else
	{
		pEnv=NULL;
	}

	BOOL bResult = FALSE;
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;

	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	StartupInfo.cb= sizeof(STARTUPINFO);
	StartupInfo.lpDesktop = _T("winsta0\\default");
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = wShowWindow;

	bResult = CreateProcessAsUser(hUserTokenDup, NULL, (LPTSTR)(LPCTSTR)strProcess, NULL, NULL, FALSE, dwCreationFlags, pEnv, NULL, &StartupInfo, &ProcessInfo);
	
	if (FALSE == bResult) {
		CString strErrorString = GetLastErrorString();
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnUserAccount > CreateProcessAsUser fail > %s"), strProcess, GetLastErrorString());
	}

	CloseHandle(ProcessInfo.hProcess);
	CloseHandle(ProcessInfo.hThread);
	if (pEnv) DestroyEnvironmentBlock(pEnv);
	CloseHandle(hProcess);
	CloseHandle(hUserTokenDup);
	CloseHandle(hPToken);
	CloseHandle(hSnap);

	return bResult;
}

DWORD CProcessRun::WTSGetActiveConsoleSessionIdEx()
{
	DWORD conSessId = WTSGetActiveConsoleSessionId();
	
	PWTS_SESSION_INFO ppSessionInfo = NULL;
	DWORD     pCount = 0;
	WTS_SESSION_INFO  wts;

	WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &ppSessionInfo, &pCount);
	for (DWORD i = 0; i < pCount; i++)
	{
		wts = ppSessionInfo[i];
		if (wts.State == WTSActive)
		{
			conSessId = wts.SessionId;
		}
	}
	WTSFreeMemory(ppSessionInfo);

	return conSessId;
}

BOOL CProcessRun::CreateProcessOnUserAccountEx(CString strProcess, WORD wShowWindow)
{
	BOOL bRet = FALSE;

	HANDLE processToken = NULL;
	TOKEN_PRIVILEGES oldTokenPrivileges = { 0 };

	HANDLE impersonationToken = NULL;
	HANDLE userToken = NULL;

	LPVOID pEnvironment = NULL;


	bRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &processToken);
	if (FALSE == bRet) {
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnUserAccountEx > OpenProcessToken fail > %s"), strProcess, GetLastErrorString());
		return bRet;
	}
	
	// This step might not be necessary because SeTcbPrivilege is enabled by default for Local System  
	LUID luid;
	bRet = LookupPrivilegeValue(NULL, SE_TCB_NAME, &luid);
	if (FALSE == bRet) {
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnUserAccountEx > LookupPrivilegeValue fail > %s"), strProcess, GetLastErrorString());

		if (processToken) {
			AdjustTokenPrivileges(processToken, FALSE, &oldTokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
			CloseHandle(processToken);
		}
		return FALSE;
	}


	TOKEN_PRIVILEGES adjTokenPrivileges = { 0 };
	adjTokenPrivileges.PrivilegeCount = 1;
	adjTokenPrivileges.Privileges[0].Luid = luid;
	adjTokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	DWORD dwOldTPLen;
	bRet = AdjustTokenPrivileges(processToken, FALSE, &adjTokenPrivileges, sizeof(TOKEN_PRIVILEGES), &oldTokenPrivileges, &dwOldTPLen);
	if (FALSE == bRet) {
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnUserAccountEx > AdjustTokenPrivileges fail > %s"), strProcess, GetLastErrorString());

		if (processToken) {
			AdjustTokenPrivileges(processToken, FALSE, &oldTokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
			CloseHandle(processToken);
		}
		return FALSE;
	}

	HMODULE hInstKernel32 = NULL;
	HMODULE hInstWtsapi32 = NULL;

	DWORD conSessId = WTSGetActiveConsoleSessionIdEx();
	if (0xFFFFFFFF == conSessId) {
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnUserAccountEx > WTSGetActiveConsoleSessionIdEx fail"), strProcess);

		if (processToken) {
			AdjustTokenPrivileges(processToken, FALSE, &oldTokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
			CloseHandle(processToken);
		}
		return FALSE;
	}

	bRet = WTSQueryUserToken(conSessId, &impersonationToken);
	if (FALSE == bRet) {
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnUserAccountEx > WTSQueryUserToken fail > %s"), strProcess, GetLastErrorString());

		if (processToken) {
			AdjustTokenPrivileges(processToken, FALSE, &oldTokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
			CloseHandle(processToken);
		}		
		return FALSE;
	}

	bRet = DuplicateTokenEx(impersonationToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &userToken);
	if (FALSE == bRet) {
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnUserAccountEx > DuplicateTokenEx fail > %s"), strProcess, GetLastErrorString());
		if (impersonationToken) {
			CloseHandle(impersonationToken);
		}
		if (processToken) {
			AdjustTokenPrivileges(processToken, FALSE, &oldTokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
			CloseHandle(processToken);
		}
		return FALSE;
	}

	bRet = CreateEnvironmentBlock(&pEnvironment, userToken, TRUE);
	if (FALSE == bRet) {
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnUserAccountEx > CreateEnvironmentBlock fail > %s"), strProcess, GetLastErrorString());

		if (userToken) {
			CloseHandle(userToken);
		}
		if (impersonationToken) {
			CloseHandle(impersonationToken);
		}
		if (processToken) {
			AdjustTokenPrivileges(processToken, FALSE, &oldTokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
			CloseHandle(processToken);
		}
		return FALSE;
	}

	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;

	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.lpDesktop = _T("winsta0\\default");
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = wShowWindow;

	bRet = CreateProcessAsUser(
		userToken,
		NULL,
		(LPTSTR)(LPCTSTR)strProcess,
		NULL,
		NULL,
		FALSE,
		CREATE_UNICODE_ENVIRONMENT,
		pEnvironment,
		NULL,
		&StartupInfo,
		&ProcessInfo
	);

	if (TRUE == bRet)
	{
		//Wait until child process exits.
		if (TRUE == m_bWaitExitCodeOption)
		{
			if (WAIT_OBJECT_0 == WaitForSingleObject(ProcessInfo.hProcess, m_dwMilliseconds)) // wait 5sec
			{
				DWORD dwExitCode = 0;
				if (TRUE == GetExitCodeProcess(ProcessInfo.hProcess, &dwExitCode))
					m_dwExitCode = dwExitCode;
			}
		}
	}
	else {
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > CreateProcessOnUserAccountEx > CreateProcessAsUser fail > %s"), strProcess, GetLastErrorString());
	}

	if (pEnvironment) {
		bRet = DestroyEnvironmentBlock(pEnvironment);
	}
	if (userToken) {
		CloseHandle(userToken);
	}
	if (impersonationToken) {
		CloseHandle(impersonationToken);
	}
	if (processToken) {
		bRet = AdjustTokenPrivileges(processToken, FALSE, &oldTokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
		CloseHandle(processToken);
	}

	return bRet;
}

BOOL CProcessRun::GetUACEnable()
{

	DWORD	dwRetValue = 0;

	HKEY	hKey				= NULL;
	DWORD	dwType				= REG_DWORD;
	DWORD	dwBufferSize		= sizeof(DWORD);

	DWORD dwReturn = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if(dwReturn == ERROR_SUCCESS)
	{
		dwReturn = ::RegQueryValueEx(hKey, _T("EnableLUA"), NULL, &dwType, (BYTE*)&dwRetValue, &dwBufferSize);

		if(dwReturn != ERROR_SUCCESS)
		{
			dwRetValue = 0;
		}

		::RegCloseKey(hKey);
	}

	return (1==dwRetValue?TRUE:FALSE);
}

BOOL CProcessRun::LaunchAppIntoDifferentSession(LPCTSTR lpAppName)
{
	TRACE(L"*** ���μ��� (%s) �����۾� ���� ***", (LPTSTR)lpAppName);

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL bResult = FALSE;
	DWORD dwSessionId, winlogonPid = 0;
	HANDLE hUserToken = NULL;
	HANDLE hUserTokenDup = NULL;
	HANDLE hPToken = NULL;
	HANDLE hProcess = NULL;
	DWORD dwCreationFlags;

	// Log the client on to the local computer.

	dwSessionId = WTSGetActiveConsoleSessionIdEx();

	//////////////////////////////////////////
	// Find the winlogon process
	////////////////////////////////////////

	PROCESSENTRY32 procEntry;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		TRACE(L"CreateToolhelp32Snapshot error.");
		if (hSnap)
			CloseHandle(hSnap);
		return FALSE;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry))
	{
		TRACE(L"Process32First error code = %d", GetLastError());
		CloseHandle(hSnap);
		return FALSE;
	}

	BOOL bGetPid = FALSE;
	do
	{
		if (_wcsicmp(procEntry.szExeFile, L"winlogon.exe") == 0)
		{
			// We found a winlogon process...make sure it's running in the console session
			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId) && winlogonSessId == dwSessionId)
			{
				winlogonPid = procEntry.th32ProcessID;
				bGetPid = TRUE;
				break;
			}
		}

	} while (Process32Next(hSnap, &procEntry));

	CloseHandle(hSnap);

	if (FALSE == bGetPid) {
		TRACE(L"PID Not Find");
		return FALSE;
	}


	////////////////////////////////////////////////////////////////////////

	WTSQueryUserToken(dwSessionId, &hUserToken);
	dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = L"winsta0\\default";
	ZeroMemory(&pi, sizeof(pi));
	TOKEN_PRIVILEGES tp;
	LUID luid;
	hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, winlogonPid);
	if (hProcess == NULL)
	{
		TRACE(L"OpenProcess Error: %u", GetLastError());
	}


	if (!::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY
		| TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_SESSIONID
		| TOKEN_READ | TOKEN_WRITE, &hPToken))
	{
		TRACE(L"Process token open Error: %u", GetLastError());
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	{
		TRACE(L"Lookup Privilege value Error: %u", GetLastError());
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	DuplicateTokenEx(hPToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hUserTokenDup);

	//Adjust Token privilege
	SetTokenInformation(hUserTokenDup, TokenSessionId, (void*)dwSessionId, sizeof(DWORD));

	if (!AdjustTokenPrivileges(hUserTokenDup, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, NULL))
	{
		TRACE(L"Adjust Privilege value Error: %u", GetLastError());
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		TRACE(L"Token does not have the provilege");
	}

	LPVOID pEnv = NULL;

	if (CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE))
	{
		dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
	}
	else
		pEnv = NULL;

	// Launch the process in the client's logon session.

	int iResultOfCreateProcessAsUser = -1;
	if (pEnv != NULL)
	{
		bResult = CreateProcessAsUser(
			hUserTokenDup,      // client's access token
			lpAppName,			// file to execute
			NULL,				// command line
			NULL,				// pointer to process SECURITY_ATTRIBUTES
			NULL,				// pointer to thread SECURITY_ATTRIBUTES
			FALSE,				// handles are not inheritable
			dwCreationFlags,	// creation flags
			pEnv,				// pointer to new environment block 
			NULL,				// name of current directory 
			&si,				// pointer to STARTUPINFO structure
			&pi					// receives information about new process
		);


		//GetLastError Shud be 0
		iResultOfCreateProcessAsUser = GetLastError();

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	if (pEnv)
		DestroyEnvironmentBlock(pEnv);

	//Perform All the Close Handles task
	if (hProcess != NULL)
		CloseHandle(hProcess);

	if (hUserToken != NULL)
		CloseHandle(hUserToken);

	if (hUserTokenDup != NULL)
		CloseHandle(hUserTokenDup);

	if (hPToken != NULL)
		CloseHandle(hPToken);


	if (bResult == FALSE)
	{
		TRACE(L"*** ���μ��� (%s) �����۾� ����(%d) ***", (LPTSTR)lpAppName, iResultOfCreateProcessAsUser);
	}
	else
		TRACE(L"*** ���μ��� (%s) �����۾� ���� ***", (LPTSTR)lpAppName);

	return bResult;
}

BOOL CProcessRun::SendCloseMessage(const DWORD & dwPid)
{
	HWND hCurWnd = NULL;
	do
	{
		hCurWnd = ::FindWindowEx(NULL, hCurWnd, NULL, NULL);

		DWORD dwProcessID = 0;
		GetWindowThreadProcessId(hCurWnd, &dwProcessID);
		if (dwProcessID == dwPid)
		{
			::PostMessage(hCurWnd, WM_CLOSE, 0, 0);
		}
	} while (hCurWnd != NULL);

	return TRUE;
}

CString CProcessRun::GetReplaceByRegExp(CString strData)
{
	// * ���� ��ġ �˻�
	int dwIndex = strData.Find('*');
	int dwRIndex = strData.ReverseFind('*');
	int nLength = strData.GetLength() - 1;

	// ���Խ��� ���� ��� ���� ��ȯ
	if (-1 == dwIndex)
		return strData;

	CString strOutData = strData;

	if (0 == dwIndex) // ó�� *
	{
		if (nLength != dwRIndex) // �� *
			strOutData = strData + L"$";	// "*Service.exe$";
	}
	else if (nLength == dwIndex) // �� *
	{
		strOutData = L"^" + strData;		// "^AuthService.*";
	}
	else // ��� *
	{
		strOutData = L"^" + strData;		// "^Auth*.exe";
		if (nLength != dwRIndex) // �� *
			strOutData = strOutData + L"$";	// "^Auth*.exe$";
	}

	// * to .* ġȯ
	strOutData.Replace(L".", L"\\.");
	strOutData.Replace(L"*", L".*");

	return strOutData;
}

BOOL CProcessRun::IsWildCard(CString strData)
{
	int nIndex = strData.Find('*');
	if (nIndex >= 0)
		return TRUE;
	else
		return FALSE;
}


BOOL CProcessRun::CompareStringWithWildCard(CString strNomal, CString strWildCard)
{
	if (!IsWildCard(strWildCard) && !IsWildCard(strNomal))
	{
		if (strWildCard.CompareNoCase(strNomal) == 0)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		CString strExp = GetReplaceByRegExp(strWildCard);
		strExp.MakeUpper();
		std::string strReg = WtoAConverter(strExp);
		std::regex reg(strReg);
		std::smatch m;

		CString strExeFile = GetReplaceByRegExp(strNomal);
		strExeFile.MakeUpper();
		std::string strProcess = WtoAConverter(strExeFile);
		if (std::regex_match(strProcess, m, reg))
		{
			return TRUE;
		}

		return FALSE;
	}
}

void CProcessRun::SetKillProcess(CString strProcessName)
{
	CString strCommnad;
	strCommnad.Format(_T("taskkill /F /IM %s"), strProcessName);
	SetCreateProcesseEx(strCommnad);
}

BOOL CProcessRun::SetKillProcess(CString strProcessName, CString& strKillProcessPath, CString& strErrorString)
{
	strKillProcessPath.Empty();
	strErrorString.Empty();
	
	if (FALSE == isProcess(strProcessName)) 
		return TRUE;
		
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) 
		return FALSE;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32W);

	if (!Process32First(hSnapshot, &pe32))
	{
		if (hSnapshot)
			CloseHandle(hSnapshot);
		return FALSE;
	}

	while (Process32Next(hSnapshot, &pe32))
	{
		if (CompareStringWithWildCard(pe32.szExeFile, strProcessName))
			SetKillProcess(pe32.th32ProcessID, strKillProcessPath, strErrorString);
	}

	if (hSnapshot)
		CloseHandle(hSnapshot);

	return !isProcess(strProcessName);
}

BOOL CProcessRun::SetCreateProcesseEx(CString strProcess, BOOL bWait, WORD wShowWindow)
{
	BOOL bRe = FALSE;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
	si.wShowWindow = wShowWindow;

	if (!CreateProcess(NULL,   // No module name (use command line)
		(LPTSTR)LPCTSTR(strProcess),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		return bRe;
	}

	if (TRUE == bWait) {
		if (WAIT_OBJECT_0 == WaitForSingleObject(pi.hProcess, 60 * 1000)) {
			bRe = TRUE;
		}
	}
	else {
		bRe = TRUE;
	}

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return bRe;
}

BOOL CProcessRun::SetKillProcess(DWORD PID, CString& strKillProcessPath, CString& strErrorString)
{
	BOOL bResult = FALSE;
	TCHAR szWindowsPath[MAX_PATH + 1];

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, PID);
	if (NULL == hProcess)
	{
		strErrorString = GetLastErrorString();
		return FALSE;
	}

	if (0 >= GetModuleFileNameEx(hProcess, NULL, szWindowsPath, sizeof(szWindowsPath)))
	{
		strErrorString = GetLastErrorString();
		if (hProcess)
			CloseHandle(hProcess);
		return FALSE;
	}

	strKillProcessPath = szWindowsPath;
	DWORD dwExitCode = 0;
	if (TRUE == GetExitCodeProcess(hProcess, &dwExitCode)) 
	{
		if (0 != TerminateProcess(hProcess, dwExitCode)) 
		{
			bResult = TRUE;
		}
		else 
		{
			strErrorString = GetLastErrorString();
			CString strCommand;
			strCommand.Format(_T("taskkill /F /PID %lu"), PID);
			SetCreateProcesseEx(strCommand);
			strCommand.Format(_T("WMIC process where processid=%lu delete"), PID);
			SetCreateProcesseEx(strCommand);
			SendCloseMessage(PID);
		}
	}

	if (hProcess)
		CloseHandle(hProcess);

	return bResult;
}

BOOL CProcessRun::IsAdminUser(CString strUserName)
{
	// ����� ���� ���� ����
	LPUSER_INFO_2	pUserInfo = NULL;
	LPUSER_INFO_2	psInfo = NULL;
	DWORD			dwLevel = 2;
	DWORD			dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD			dwEntriesRead = 0;
	DWORD			dwTotalEntries = 0;
	DWORD			dwResumeHandle = 0;
	NET_API_STATUS	nStatus;
	BOOL			bAdmin = FALSE;

	// �� ��ǻ�� �̸� ����
	TCHAR	strComputerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD	dwSize = MAX_COMPUTERNAME_LENGTH + 1;

	// �� ��ǻ�� �̸� ���
	if (GetComputerName(strComputerName, &dwSize) == FALSE)
	{
		// ��ǻ�� �̸� ��� ����
		return bAdmin;
	}

	do {
		nStatus = NetUserEnum(strComputerName,
			dwLevel,
			FILTER_NORMAL_ACCOUNT, // global users
			(LPBYTE*)&pUserInfo,
			dwPrefMaxLen,
			&dwEntriesRead,
			&dwTotalEntries,
			&dwResumeHandle);

		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) {
			if (NULL != pUserInfo) {
				psInfo = pUserInfo;
				for (DWORD i = 0; (i < dwEntriesRead); i++) {
					if (0 == strUserName.CompareNoCase(psInfo->usri2_name)) {
						if (USER_PRIV_ADMIN == psInfo->usri2_priv) {
							bAdmin = TRUE;
						}
					}
					psInfo++;
				}
			}
		}
		else {
			return bAdmin;
		}

		if (pUserInfo != NULL)
		{
			NetApiBufferFree(pUserInfo);
			pUserInfo = NULL;
		}
	} while (nStatus == ERROR_MORE_DATA); // end do

	if (pUserInfo != NULL) {
		NetApiBufferFree(pUserInfo);
		pUserInfo = NULL;
	}

	return bAdmin;
}

BOOL CProcessRun::GetOSVersion(OSVERSIONINFOEX* OSVer)
{
	typedef LONG(WINAPI* tRtlGetVersion)(OSVERSIONINFOEX*);

	HMODULE h_NtDll = GetModuleHandleW(L"ntdll.dll");
	tRtlGetVersion f_RtlGetVersion = (tRtlGetVersion)GetProcAddress(h_NtDll, "RtlGetVersion");

	if (!f_RtlGetVersion)
		return FALSE; // This will never happen (all processes load ntdll.dll)

	LONG Status = f_RtlGetVersion(OSVer);
	return Status == 0; // STATUS_SUCCESS;
}

BOOL CProcessRun::CreateProcessOnAdminAccountForThr(CString strProcess, WORD wShowWindow)
{
	BOOL bRe = FALSE;
	UINT nStackSize = 0;			/**<  Stack Size*/
	DWORD dwThreadID = 0;			/**<  ������ ID */
	HANDLE hThread = NULL;			/**<  ������ �ڵ� */

	m_strProcess = strProcess;
	m_wShowWindow = wShowWindow;
	hThread = (HANDLE)_beginthreadex(0, nStackSize, CProcessRun::__ADMIN_ACCOUNT_RUN__, this, 0, (unsigned *)&dwThreadID);

	if (NULL == hThread) return bRe;

	if (WAIT_OBJECT_0 != ::WaitForSingleObject(hThread, 3000)) {
		::TerminateThread(hThread, 0);
		CloseHandle(hThread);
		hThread = NULL;
	}
	else {
		bRe = TRUE;
	}
	return bRe;
}

BOOL CProcessRun::CreateProcessOnUserAccountForThr(CString strProcess, WORD wShowWindow)
{

	BOOL bRe = FALSE;
	UINT nStackSize = 0;			/**<  Stack Size*/
	DWORD dwThreadID = 0;			/**<  ������ ID */
	HANDLE hThread = NULL;			/**<  ������ �ڵ� */

	m_strProcess = strProcess;
	m_wShowWindow = wShowWindow;
	hThread = (HANDLE)_beginthreadex(0, nStackSize, CProcessRun::__USER_ACCOUNT_RUN__, this, 0, (unsigned *)&dwThreadID);

	if (NULL == hThread) return bRe;

	if (WAIT_OBJECT_0 != ::WaitForSingleObject(hThread, 3000)) {
		::TerminateThread(hThread, 0);
		CloseHandle(hThread);
		hThread = NULL;
	}
	else {
		bRe = TRUE;
	}
	return bRe;
}
DWORD CProcessRun::GetProcessPID(CString strProcessName)
{
	DWORD dwPIDRet = 0;

	if(strProcessName.IsEmpty())
		return 0;

	if (FALSE == isProcess(strProcessName))
		return 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
		return 0;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32W);

	if (!Process32First(hSnapshot, &pe32))
	{
		if (hSnapshot)
			CloseHandle(hSnapshot);
		return 0;
	}

	while (Process32Next(hSnapshot, &pe32))
	{
		if (CompareStringWithWildCard(pe32.szExeFile, strProcessName))
		{
			dwPIDRet = pe32.th32ProcessID;
			break;
		}
	}

	if (hSnapshot)
		CloseHandle(hSnapshot);
	
	return dwPIDRet;
}
BOOL CProcessRun::CreateProcessOnAdminAccountForThr()
{
	return CreateProcessOnAdminAccount(m_strProcess, m_wShowWindow);
}

BOOL CProcessRun::CreateProcessOnUserAccountForThr()
{
	return CreateProcessOnUserAccount(m_strProcess, m_wShowWindow);
}
unsigned __stdcall CProcessRun::__USER_ACCOUNT_RUN__(void* pVoid)
{
	CProcessRun* pThis = (CProcessRun*)pVoid;
	pThis->CreateProcessOnUserAccountForThr();
	_endthreadex(0);
	return 0;
}
unsigned __stdcall CProcessRun::__ADMIN_ACCOUNT_RUN__(void* pVoid)
{
	CProcessRun* pThis = (CProcessRun*)pVoid;
	pThis->CreateProcessOnAdminAccountForThr();
	_endthreadex(0);
	return 0;
}

CString CProcessRun::GetProcessUserName()
{
	CString strUserName;
	HANDLE hProcessToken = NULL;
	PTOKEN_USER pUserToken = NULL;
	
	// ���μ��� ��ū�� �����Ѵ�. 
	if (TRUE == OpenProcessToken(::GetCurrentProcess(), TOKEN_READ, &hProcessToken))
	{
		// ��ū ������ ȹ���ϱ� ���� �ʿ��� ũ�⸦ ���Ѵ�. 
		DWORD dwProcessTokenInfoAllocSize = 0;
		if (FALSE == GetTokenInformation(hProcessToken, TokenUser, NULL, 0, &dwProcessTokenInfoAllocSize))
		{
			if (0x0000007a == GetLastError() && 0 < dwProcessTokenInfoAllocSize) {
				// ��ū ������ ������� �ʿ��� ũ�⸸ŭ �޸𸮸� �Ҵ��Ѵ�. 
				BYTE* pTempData = new BYTE[dwProcessTokenInfoAllocSize];
				pUserToken = reinterpret_cast<PTOKEN_USER>(pTempData);
				if (NULL != pUserToken)
				{
					// ����� ������ ������� ���μ����� ��ū ������ ȹ���Ѵ�. 
					if (TRUE == GetTokenInformation(hProcessToken, TokenUser, pUserToken, dwProcessTokenInfoAllocSize, &dwProcessTokenInfoAllocSize))
					{
						SID_NAME_USE   snuSIDNameUse;
						TCHAR          szUser[MAX_PATH] = { 0 };
						DWORD          dwUserNameLength = MAX_PATH;
						TCHAR          szDomain[MAX_PATH] = { 0 };
						DWORD          dwDomainNameLength = MAX_PATH;

						// ������� SID �� �̿��Ͽ� ����� �̸��� ��´�. 
						if (TRUE == LookupAccountSid(NULL, pUserToken->User.Sid, szUser, &dwUserNameLength, szDomain, &dwDomainNameLength, &snuSIDNameUse))
						{
							// ����� �̸��� �����Ѵ�. 
							strUserName = szUser;
						}
					}
				}
				if (NULL != pTempData)
				{
					delete[] pTempData;
					pTempData = NULL;
				}
			}
		}
		if (NULL != hProcessToken)
		{
			CloseHandle(hProcessToken);
		}
	}
	return strUserName;
}

BOOL CProcessRun::GetProcessElevation(TOKEN_ELEVATION_TYPE *pElevationType, BOOL *pIsAdmin)
{
	HANDLE hToken = NULL;
	BOOL bResult = FALSE;
	DWORD dwSize = 0;

	// ���� ���μ����� ��ū�� ��´�.
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		return FALSE;

	// ���ѻ�� ���¿� ���� ������ ��´�.
	if (GetTokenInformation(hToken, TokenElevationType, pElevationType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize))
	{
		BYTE adminSID[SECURITY_MAX_SID_SIZE];
		dwSize = sizeof(adminSID);

		// ������ �׷��� SID ���� �����Ѵ�.
		CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);

		if (*pElevationType == TokenElevationTypeLimited)
		{
			HANDLE hUnfilteredToken = NULL;

			// ����� ��ū�� �ڵ��� ��´�.
			GetTokenInformation(hToken, TokenLinkedToken, (void *)&hUnfilteredToken, sizeof(HANDLE), &dwSize);

			// ������ ��ū�� �������� SID�� �����ϰ� �ִ��� ���θ� Ȯ���Ѵ�.
			if (CheckTokenMembership(hUnfilteredToken, &adminSID, pIsAdmin))
				bResult = TRUE;

			CloseHandle(hUnfilteredToken);
		}
		else
		{
			*pIsAdmin = IsUserAnAdmin();
			bResult = TRUE;
		}
	}

	CloseHandle(hToken);
	return bResult;
}

BOOL CProcessRun::IsAdministrator()
{
	TOKEN_ELEVATION_TYPE t;
	BOOL bAdmin = FALSE;
	TCHAR szUser[255] = { 0 };
	DWORD dwUser = _countof(szUser);
	GetUserName(szUser, &dwUser);

	if (GetProcessElevation(&t, &bAdmin))
	{
		if (bAdmin) {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"����� ���� > (%s) ������ ����", szUser);
		}
		else{
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"����� ���� > (%s) ����� ����", szUser);
		}		
		switch (t)
		{
		case TokenElevationTypeDefault:
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"���μ��� ���� ���� (%s) > �⺻ ������̰ų�, UAC ����� OFF �� ����", GetProcessUserName());
			break;
		case TokenElevationTypeFull:
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"���μ��� ���� ���� (%s) > ���ѻ��", GetProcessUserName());
			break;
		case TokenElevationTypeLimited:
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"���μ��� ���� ���� (%s) > ���ѵ� �������� ����", GetProcessUserName());
			break;
		default:
			break;
		}
	}
	return bAdmin;
}

BOOL CProcessRun::IsDesktopOpen()
{
	PROCESSENTRY32 procEntry;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry)) {
		CloseHandle(hSnap);
		return FALSE;
	}

	CString strExplorer = _T("explorer.exe");

	BOOL bFindIt = FALSE;
	do {
		if (0 == strExplorer.CompareNoCase(procEntry.szExeFile)) {
			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId))// && winlogonSessId == dwSessionId)
			{
				bFindIt = TRUE;
				break;
			}
		}
	} while (Process32Next(hSnap, &procEntry));

	if (!bFindIt) {
		CloseHandle(hSnap);
		return FALSE;
	}

	return TRUE;
}
BOOL CProcessRun::SetCreateProcesseOnAdmin(CString strProcess, DWORD& dwExitCode, DWORD dwTimeout)
{

	BOOL bResult = FALSE;

	DWORD winlogonPid = 0;
	HANDLE hUserToken = NULL;
	HANDLE hUserTokenDup = NULL;
	HANDLE hPToken = NULL;
	HANDLE hProcess = NULL;

	PROCESSENTRY32 procEntry;

	DWORD dwSessionId = WTSGetActiveConsoleSessionIdEx();
	WTSQueryUserToken(dwSessionId, &hUserToken);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > SetCreateProcesseOnAdmin > CreateToolhelp32Snapshot fail > %s"), strProcess, GetLastErrorString());
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s"), m_strLastErrorString);
		return bResult;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry)) {
		CloseHandle(hSnap);
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > SetCreateProcesseOnAdmin > Process32First fail > %s"), strProcess, GetLastErrorString());
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s"), m_strLastErrorString);
		return bResult;
	}

	CString strExplorer = _T("explorer.exe");


	BOOL bFindIt = FALSE;
	do {
		if (0 == strExplorer.CompareNoCase(procEntry.szExeFile)) {
			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId))
			{
				if (winlogonSessId == dwSessionId) {
					bFindIt = TRUE;
					winlogonPid = procEntry.th32ProcessID;
					break;
				}
				else {
					winlogonPid = procEntry.th32ProcessID;
				}
			}
		}
	} while (Process32Next(hSnap, &procEntry));

	if (FALSE == bFindIt && 0 == winlogonPid) {
		CloseHandle(hSnap);
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > SetCreateProcesseOnAdmin > explorer.exe ���μ��� �������"), strProcess);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s"), m_strLastErrorString);
		return bResult;
	}

	DWORD dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, winlogonPid);

	::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_SESSIONID | TOKEN_READ | TOKEN_WRITE, &hPToken);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	DuplicateTokenEx(hPToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hUserTokenDup);

	HANDLE hPrimaryToken = NULL;
	TOKEN_LINKED_TOKEN linkedToken = { 0 };
	DWORD dwSize = sizeof(linkedToken);
	if (GetTokenInformation(hPToken, TokenLinkedToken, &linkedToken, dwSize, &dwSize)) {
		if (DuplicateTokenEx(linkedToken.LinkedToken, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &hPrimaryToken)) {
			LPVOID pEnv = NULL;

			if (CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE)) {
				dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
			}
			else {
				pEnv = NULL;
			}

			PROCESS_INFORMATION ProcessInfo;
			STARTUPINFO StartupInfo;

			ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));
			ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
			StartupInfo.cb = sizeof(STARTUPINFO);
			StartupInfo.lpDesktop = _T("winsta0\\default");
			StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
			StartupInfo.wShowWindow = SW_HIDE;
			bResult = CreateProcessAsUser(hPrimaryToken, NULL, (LPTSTR)(LPCTSTR)strProcess, NULL, NULL, FALSE, dwCreationFlags, pEnv, NULL, &StartupInfo, &ProcessInfo);

			if (TRUE == bResult) {
				bResult = FALSE;
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("���μ��� ����(%s) > SetCreateProcesseOnAdmin > %lu"), strProcess, ProcessInfo.dwProcessId);

				SetForegroundWindow((HWND)ProcessInfo.hProcess);
				if (WAIT_OBJECT_0 == WaitForSingleObject(ProcessInfo.hProcess, dwTimeout * 1000))
				{
					if (FALSE == GetExitCodeProcess(ProcessInfo.hProcess, &dwExitCode)) {
						dwExitCode = ERROR_INSTALL_FAILURE;
					}
					else {
						bResult = TRUE;
					}
				}
			}

			CloseHandle(hPrimaryToken);
			CloseHandle(ProcessInfo.hProcess);
			CloseHandle(ProcessInfo.hThread);
			if (pEnv) DestroyEnvironmentBlock(pEnv);
		}
		else {
			m_strLastErrorString.Format(_T("���μ��� ����(%s) > SetCreateProcesseOnAdmin > DuplicateTokenEx fail > %s"), strProcess, GetLastErrorString());
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s"), m_strLastErrorString);
		}
	}
	else {
		m_strLastErrorString.Format(_T("���μ��� ����(%s) > SetCreateProcesseOnAdmin > GetTokenInformation fail > %s"), strProcess, GetLastErrorString());
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s"), m_strLastErrorString);
	}

	if (linkedToken.LinkedToken)
		CloseHandle(linkedToken.LinkedToken);
	CloseHandle(hUserToken);
	CloseHandle(hUserTokenDup);
	CloseHandle(hPToken);
	CloseHandle(hProcess);
	CloseHandle(hSnap);

	return bResult;

}