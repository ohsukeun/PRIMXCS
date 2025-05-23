#include "stdafx.h"
#include "ZombieProcessCheck.h"
#include <tlhelp32.h>

CZombieProcessCheck::CZombieProcessCheck()
{
}


CZombieProcessCheck::~CZombieProcessCheck()
{

	m_ProcessIDList.RemoveAll();
	m_ZombieIDList.RemoveAll();
}

int CZombieProcessCheck::GetProcessID(CString strProcessName)
{
	int nRe = 0;

	strProcessName.Trim();
	if (0 == strProcessName.GetLength()) {
		return nRe;
	}

	PROCESSENTRY32 procEntry;

	m_ProcessIDList.RemoveAll();

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		return nRe;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry)){
		CloseHandle(hSnap);
		return nRe;
	}
	
	DWORD dwCurrentProcessID = GetCurrentProcessId();

	DWORD dwSessionId = WTSGetActiveConsoleSessionId();
	do
	{
		if (0 == strProcessName.CompareNoCase(procEntry.szExeFile))	{
			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId) && winlogonSessId == dwSessionId)
			{
				DWORD dwProcessID = procEntry.th32ProcessID;
				if (dwCurrentProcessID != dwProcessID) {
					m_ProcessIDList.AddTail(dwProcessID);
					nRe++;
				}
			}
		}

	} while (Process32Next(hSnap, &procEntry));

	CloseHandle(hSnap);

	return nRe;
}

BOOL CZombieProcessCheck::GetProcessHandleCountEx(DWORD dwProcessID, UINT& nHandleCount)
{
	BOOL bRe = FALSE;

	HANDLE hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, dwProcessID);

	if (NULL != hProcess) {
		DWORD dwCount = 0;
		GetProcessHandleCount(hProcess, &dwCount);
		CloseHandle(hProcess);
		nHandleCount = dwCount;
		bRe = TRUE;
	}

	return bRe;
}

BOOL CZombieProcessCheck::GetProcessThreadCount(DWORD dwProcessID,UINT& nThreadCount)
{
	nThreadCount = 0;

	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (INVALID_HANDLE_VALUE == hThreadSnap) {
		return FALSE;
	}

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (FALSE == Thread32First(hThreadSnap, &te32)) {
		CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
		return FALSE;
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	do
	{
		if (te32.th32OwnerProcessID == dwProcessID)
		{
			TRACE(TEXT("\n     THREAD ID      = 0x%08X"), te32.th32ThreadID);
			nThreadCount++;
		}
	} while (Thread32Next(hThreadSnap, &te32));

	CloseHandle(hThreadSnap);
	return TRUE;
}

CString CZombieProcessCheck::SetSplitPathFileName(CString strPath)
{
	CString strFileName;

	TCHAR pFilename[MAX_PATH] = { 0 };
	TCHAR pExt[MAX_PATH] = { 0 };

	_wsplitpath_s(strPath, NULL,0, NULL,0, pFilename, MAX_PATH, pExt, MAX_PATH);

	strFileName.Format(_T("%s%s"), pFilename, pExt);

	return strFileName;
}

BOOL CZombieProcessCheck::SetZombieProcessCheck()
{
	TCHAR szPath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szPath, sizeof(szPath));
	CString strProcessName = SetSplitPathFileName(szPath);
	strProcessName.Trim();
	return SetZombieProcessCheck(strProcessName);
}

BOOL CZombieProcessCheck::SetZombieProcessCheck(CString strProcessName)
{
	BOOL bRe = FALSE;

	int nCount = GetProcessID(strProcessName);

	int nZombieCount = 0;
	int nValidCount = 0;

	if (0 < nCount) {
		POSITION Pos = m_ProcessIDList.GetHeadPosition();
		while (NULL != Pos) {
			DWORD dwPID = m_ProcessIDList.GetNext(Pos);
			if (0 < dwPID) {
				BOOL bZombie = FALSE;
				UINT nThreadCount = 0;
				UINT nHandleCount = 0;
				if (TRUE == GetProcessThreadCount(dwPID, nThreadCount)) {
					if (0 == nThreadCount) {
						bZombie = TRUE;
					}
				}
				if (TRUE == GetProcessHandleCountEx(dwPID, nHandleCount)) {
					if (0 == nHandleCount) {
						bZombie = TRUE;
					}
				}
				if (TRUE == bZombie) {
					nZombieCount++;
					m_ZombieIDList.AddTail(dwPID);
				}
				else {
					nValidCount++;
				}
			}
		}
		m_ProcessIDList.RemoveAll();
	}

	if (1 <= nZombieCount && 0 == nValidCount) {
		bRe = TRUE;
	}
	return bRe;
}

BOOL CZombieProcessCheck::SetCreateProcesseEx(CString strProcess, BOOL bWait, WORD wShowWindow)
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
		if (WAIT_OBJECT_0 == WaitForSingleObject(pi.hProcess, 2 * 1000)) {
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

void CZombieProcessCheck::SetZombieProcessKill()
{
	POSITION Pos = m_ZombieIDList.GetHeadPosition();
	while (NULL != Pos) {
		DWORD dwID = m_ZombieIDList.GetNext(Pos);
		CString strCommand;
		strCommand.Format(_T("WMIC process where processid=%lu delete"), dwID);
		SetCreateProcesseEx(strCommand,TRUE);

		strCommand.Format(_T("taskkill /F /PID %lu"), dwID);
		SetCreateProcesseEx(strCommand, TRUE);

	}
}

void CZombieProcessCheck::SetZombieProcessKill(CList<DWORD, DWORD>& ZombieIDList)
{
	POSITION Pos = ZombieIDList.GetHeadPosition();
	while (NULL != Pos) {
		DWORD dwID = ZombieIDList.GetNext(Pos);
		CString strCommand;
		strCommand.Format(_T("WMIC process where processid=%lu delete"), dwID);
		SetCreateProcesseEx(strCommand, TRUE);

		strCommand.Format(_T("taskkill /F /PID %lu"), dwID);
		SetCreateProcesseEx(strCommand, TRUE);

	}
}
