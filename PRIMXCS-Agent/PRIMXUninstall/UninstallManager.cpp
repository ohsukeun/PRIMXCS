#include "stdafx.h"
#include "UninstallManager.h"
#include <winsvc.h>
#include "resource.h"

CUninstallManager::CUninstallManager()
{
}


CUninstallManager::~CUninstallManager()
{
}


/*

PRIMX 제품 삭제

서비스 중지
프러세스 중지
서비스 삭제
파일 삭제
레지스트리 삭제

*/
void CUninstallManager::SetUninstall()
{
	CString strCommnad;
	
	CString strZonecentrol;
	CString strOrizone;
	CString strCryhod;
	GetPRIMXProductInfo(strZonecentrol, strOrizone, strCryhod);
	strZonecentrol.Trim();
	strOrizone.Trim();
	strCryhod.Trim();

	if (0 < strZonecentrol.GetLength()) {
		SetCreateProcesseEx(strZonecentrol, (5 * 60));
	}
	if (0 < strOrizone.GetLength()) {
		SetCreateProcesseEx(strOrizone, (5 * 60));
	}
	if (0 < strCryhod.GetLength()) {
		SetCreateProcesseEx(strCryhod, (5 * 60));
	}

	CString strServiceBinaryPath = GetServiceBinaryPath(_T("PRIMXAgent"));
	strServiceBinaryPath.Trim();

	if (0 == strServiceBinaryPath.GetLength()) {
		strServiceBinaryPath = GetInstallPRIMXAgentPath();
	}

	strServiceBinaryPath.MakeLower();
	CString strModFileName = strServiceBinaryPath;
	strModFileName.Replace(_T("primxagent.exe"),_T("primxagentRemove.exe"));

	SetFileRename(strServiceBinaryPath, strModFileName);
	
	strCommnad.Format(_T("taskkill /F /IM %s"), _T("primxagent.exe"));
	SetCreateProcesseEx(strCommnad,5);

	strCommnad.Format(_T("%s -u"), strModFileName);
	SetCreateProcesseEx(strCommnad,5);

	strCommnad.Format(_T("taskkill /F /IM %s"), _T("primxagent.exe"));
	SetCreateProcesseEx(strCommnad, 5);

	strCommnad.Format(_T("taskkill /F /IM %s"), _T("primxagentcs.exe"));
	SetCreateProcesseEx(strCommnad, 5);

	strCommnad.Format(_T("taskkill /F /IM %s"), _T("PRIMXEncMonitor.exe"));
	SetCreateProcesseEx(strCommnad, 5);

	SetFileRemoveRecurseSubdirectories(SetSplitPath(strServiceBinaryPath),_T("PRIMXUninstall.exe"));
	
	SetFileRemoveRecurseSubdirectories(GetProgramDataPath(),_T(""));

	CString strAgentID = GetAgentID();

	SHDeleteKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Rimahr\\PRIMXAgent"));
	SHDeleteKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Rimahr"));

	SetAgentID(strAgentID);

}

CString CUninstallManager::GetAgentID()
{
	CString strAgentID;

	ReadRegistryStringValue(_T("SOFTWARE\\Rimahr\\PRIMXAgent"), _T("AgentID"), strAgentID);
	
	return strAgentID;
}

void CUninstallManager::SetAgentID(CString strAgentID)
{
	WriteRegistryStringValue(_T("SOFTWARE\\Rimahr\\PRIMXAgent"), _T("AgentID"), strAgentID);
}

BOOL CUninstallManager::ReadRegistryStringValue(CString KeyPath, CString Name, CString& Value)
{
	HKEY hKey = NULL;

	if (ERROR_SUCCESS != ::RegOpenKey(HKEY_LOCAL_MACHINE, LPCTSTR(KeyPath), &hKey)) {
		return FALSE;
	}

	DWORD dwType = REG_SZ;
	TCHAR szValue[1024] = { 0 };
	DWORD dwSize = 1024;
	if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, LPCTSTR(Name), NULL, &dwType, (BYTE*)szValue, &dwSize)) {
		Value = LPTSTR(szValue);
	}
	::RegCloseKey(hKey);

	return TRUE;
}
BOOL CUninstallManager::WriteRegistryStringValue(CString KeyPath, CString Name, CString Value)
{
	HKEY hKey = NULL;

	DWORD dwReturn = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, LPCTSTR(KeyPath), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if (ERROR_SUCCESS != dwReturn) {
		return FALSE;
	}

	::RegSetValueEx(hKey,
		LPCTSTR(Name),
		0,
		REG_SZ,
		(CONST BYTE*)LPCTSTR(Value),
		Value.GetLength() * sizeof(TCHAR));

	::RegCloseKey(hKey);

	return TRUE;
}
CString CUninstallManager::SetSplitPath(CString strPath)
{
	CString strSplitPath;

	TCHAR pFilename[MAX_PATH] = { 0 };
	TCHAR pExt[MAX_PATH] = { 0 };

	TCHAR pDrive[MAX_PATH] = { 0 };
	TCHAR pDir[MAX_PATH] = { 0 };

	_tsplitpath_s(strPath, pDrive, pDir, pFilename, pExt);

	strSplitPath.Format(_T("%s%s"), pDrive, pDir);

	return strSplitPath;
}
BOOL CUninstallManager::SetCreateProcesseEx(CString strProcess, DWORD dwTimeout)
{
	BOOL bRe = FALSE;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
	si.wShowWindow = SW_HIDE;

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

	if (0 == dwTimeout) {
		dwTimeout = (60 * 5);
	}

	if (WAIT_OBJECT_0 == WaitForSingleObject(pi.hProcess, dwTimeout * 1000)) {
		bRe = TRUE;
	}

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return bRe;
}

CString CUninstallManager::GetInstallPRIMXAgentPath()
{
	CString strPRIMXAgentPath = _T("c:\\Program Files\\Rimahr\\PRIMXAgent\\PRIMXAgent.exe");

	TCHAR szPath[MAX_PATH] = { 0 };
	if (TRUE == SHGetSpecialFolderPath(NULL, szPath, CSIDL_PROGRAM_FILES, 0)) {
		strPRIMXAgentPath.Format(_T("%s\\Rimahr\\PRIMXAgent\\PRIMXAgent.exe"), szPath);
	}
	else {
		if (0 < ::ExpandEnvironmentStrings(_T("%ProgramFiles%"), szPath, MAX_PATH)) {
			strPRIMXAgentPath.Format(_T("%s\\Rimahr\\PRIMXAgent\\PRIMXAgent.exe"), szPath);
		}
		else {
			if (0 != GetWindowsDirectory(szPath, MAX_PATH)) {
				TCHAR pDrive[MAX_PATH] = { 0 };
				TCHAR pDir[MAX_PATH] = { 0 };
				TCHAR pFileName[MAX_PATH] = { 0 };
				TCHAR pFileExt[MAX_PATH] = { 0 };
				_tsplitpath_s(szPath, pDrive, MAX_PATH, pDir, MAX_PATH, pFileName, MAX_PATH, pFileExt, MAX_PATH);
				strPRIMXAgentPath.Format(_T("%s:\\Program Files\\Rimahr\\PRIMXAgent\\PRIMXAgent.exe"), szPath);
			}
		}
	}

	return strPRIMXAgentPath;
}

CString CUninstallManager::GetServiceBinaryPath(CString ServiceName)
{
	CString strServiceBinaryPath;

	LPQUERY_SERVICE_CONFIG lpqscBuf;
	DWORD dwBytesNeeded = 0;
	DWORD dwState = 0xFFFFFFFF;
	DWORD dwErr = 0xFFFFFFFF;

	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (NULL == hSCM) {
		return strServiceBinaryPath;
	}

	SC_HANDLE hService = OpenService(hSCM, LPCTSTR(ServiceName), SERVICE_ALL_ACCESS);
	if (NULL == hService) {
		CloseServiceHandle(hSCM);
		return strServiceBinaryPath;
	}
		
	SERVICE_STATUS ss;
	memset(&ss, 0, sizeof(ss));
	BOOL b = QueryServiceStatus(hService, &ss);
	if (!b) {
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCM);
		return strServiceBinaryPath;
	}
	else {
		dwErr = ss.dwWin32ExitCode;
		dwState = ss.dwCurrentState;

		if (dwState == SERVICE_RUNNING) {
			b = ::ControlService(hService, SERVICE_CONTROL_INTERROGATE, &ss);
			if (b) {
				dwErr = ss.dwWin32ExitCode;
				dwState = ss.dwCurrentState;
			}
		}

		lpqscBuf = (LPQUERY_SERVICE_CONFIG) new BYTE[4096];
		if (0 != QueryServiceConfig(hService, lpqscBuf, 4096, &dwBytesNeeded)) {
			strServiceBinaryPath = lpqscBuf->lpBinaryPathName;
		}		
		delete[](BYTE*)lpqscBuf;
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCM);

	return strServiceBinaryPath;
}

void CUninstallManager::SetDeleteFile(CString strFilePath)
{
	CFileFind	Find;
	if (TRUE == Find.FindFile(LPCTSTR(strFilePath))) {
		if (FALSE == ::DeleteFile(strFilePath)) {
			CString strRenameRemove;
			strRenameRemove.Format(_T("%s.%lu"), LPCTSTR(strFilePath), UINT(_time32(NULL)));
			::DeleteFile(strRenameRemove);
			rename(WtoAConverter(strFilePath), WtoAConverter(strRenameRemove));
			MoveFileEx(strRenameRemove, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
		}
	}
	Find.Close();
}

BOOL CUninstallManager::SetFileRename(CString strSrcFileName,CString strModFileName)
{
	BOOL bRe = FALSE;
	CFileFind	Find;
	if (TRUE == Find.FindFile(LPCTSTR(strSrcFileName))) {
		DWORD dwAttrs = ::GetFileAttributes(strSrcFileName);
		if (dwAttrs & FILE_ATTRIBUTE_READONLY) {
			::SetFileAttributes(strSrcFileName, dwAttrs & ~FILE_ATTRIBUTE_READONLY);
		}
		SetDeleteFile(strModFileName);
		if (0 == _trename(strSrcFileName, strModFileName)) {
			bRe = TRUE;
		}		
	}
	Find.Close();

	return bRe;
}

BOOL CUninstallManager::IsSystemDirectory(CString strCheckDirectory)
{
	strCheckDirectory.Trim();
	strCheckDirectory.Replace(_T("\\"), _T(""));
	strCheckDirectory.MakeUpper();
	
	TCHAR tchBuffer[MAX_PATH] = { 0 };
	ExpandEnvironmentStrings(_T("%SystemRoot%"), tchBuffer, MAX_PATH);
	CString strSystemDir = tchBuffer;
	strSystemDir.Trim();
	strSystemDir.Replace(_T("\\"), _T(""));
	strSystemDir.MakeUpper();

	if (0 == strSystemDir.CompareNoCase(strCheckDirectory)) {
		return TRUE;
	}
	strSystemDir = strSystemDir + _T("System32");
	if (0 == strSystemDir.CompareNoCase(strCheckDirectory)) {
		return TRUE;
	}

	ZeroMemory(tchBuffer, sizeof(tchBuffer));
	ExpandEnvironmentStrings(_T("%ProgramFiles%"), tchBuffer, MAX_PATH);
	strSystemDir = tchBuffer;
	strSystemDir.Trim();
	strSystemDir.Replace(_T("\\"), _T(""));
	strSystemDir.MakeUpper();

	if (0 == strSystemDir.CompareNoCase(strCheckDirectory)) {
		return TRUE;
	}

	return FALSE;
}

void CUninstallManager::SetFileRemoveRecurseSubdirectories(CString strSrcDir, CString strExclusionFileName)
{
	if (3 >= strSrcDir.GetLength()) {
		return;
	}

	if (TRUE == IsSystemDirectory(strSrcDir)) {
		return;
	}

	CFileFind finderdel;
	CString TempDel;
	TempDel = strSrcDir + _T("\\*.*");
	BOOL bWorking = finderdel.FindFile(TempDel);
	while (bWorking) {
		bWorking = finderdel.FindNextFile();
		if (finderdel.IsDots()) {
			continue;
		}

		if (TRUE == finderdel.IsArchived()) {
			if (0 != strExclusionFileName.CompareNoCase(finderdel.GetFileName())) {
				CString DstFile;
				DstFile.Format(_T("%s\\%s"), strSrcDir, finderdel.GetFileName());
				SetDeleteFile(DstFile);
			}
		}
		if (TRUE == finderdel.IsDirectory()) {
			SetFileRemoveRecurseSubdirectories(finderdel.GetFilePath(), strExclusionFileName);
			::RemoveDirectory(finderdel.GetFilePath());
		}
		Sleep(100);
	}
	finderdel.Close();
	::RemoveDirectory(strSrcDir);
}
CString CUninstallManager::GetRegQueryValueString(HKEY hKey, CString strValueName)
{
	CString strData;
	DWORD dwBufferSize = 0;
	LSTATUS nResult = RegQueryValueEx(hKey, strValueName, NULL, NULL, NULL, &dwBufferSize);
	if (ERROR_SUCCESS == nResult) {
		TCHAR *pBuffer = new TCHAR[dwBufferSize + 1];
		memset(pBuffer, 0, (dwBufferSize + 1) * sizeof(TCHAR));
		nResult = RegQueryValueEx(hKey, strValueName, NULL, NULL, (LPBYTE)pBuffer, &dwBufferSize);
		if (ERROR_SUCCESS == nResult) {
			strData = pBuffer;
			strData.Trim();
		}
		delete[] pBuffer;
		pBuffer = NULL;
	}
	return strData;
}

BOOL CUninstallManager::GetPRIMXProductInfo(CString& strZonecentrol, CString& strOrizone, CString& strCryhod)
{
	HKEY hKey;
	DWORD	dwCnt = 0;
	CString strPath = _T("software\\microsoft\\windows\\currentversion\\uninstall");

	LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, strPath, 0, KEY_READ, &hKey);

	if (ERROR_SUCCESS != lResult) {
		return FALSE;
	}

	while (TRUE) {
		// 키 나열
		TCHAR SubKey[MAX_PATH + 1] = { 0 };

		FILETIME fileTime;
		DWORD dwSize = MAX_PATH + 1;

		lResult = ::RegEnumKeyEx(hKey, dwCnt, SubKey, &dwSize, NULL, NULL, NULL, &fileTime);

		if (ERROR_SUCCESS != lResult) {
			break;
		}
		else {
			CString strKeyPath;
			CString strDisplayName;
			strKeyPath.Format(_T("%s\\%s"), strPath, SubKey);

			HKEY hSubKey;
			LONG nRe = RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKeyPath, 0, KEY_READ, &hSubKey);

			if (ERROR_SUCCESS == nRe) {

				CString strDisplayName = GetRegQueryValueString(hSubKey, _T("DisplayName"));
				BOOL bProduct = FALSE;
				if (0 == strDisplayName.CompareNoCase(_T("ORIZON"))) {
					strOrizone = GetRegQueryValueString(hSubKey, _T("UninstallString"));
					if (0 < strOrizone.Find(SubKey)) {
						strOrizone.Format(_T("msiexec.exe /uninstall %s /quiet /norestart"), SubKey);
					}
				}
				else if (0 == strDisplayName.CompareNoCase(_T("ZONECENTRAL"))) {
					strZonecentrol = GetRegQueryValueString(hSubKey, _T("UninstallString"));
					if (0 < strZonecentrol.Find(SubKey)) {
						strZonecentrol.Format(_T("msiexec.exe /uninstall %s /quiet /norestart"), SubKey);
					}
				}
				else if (0 == strDisplayName.CompareNoCase(_T("CRYHOD"))) {
					strCryhod = GetRegQueryValueString(hSubKey, _T("UninstallString"));
					if (0 < strCryhod.Find(SubKey)) {
						strCryhod.Format(_T("msiexec.exe /uninstall %s /quiet /norestart"), SubKey);
					}
				}
			}
		}
		++dwCnt;
	}
	RegCloseKey(hKey);

	return TRUE;
}

CString CUninstallManager::GetProgramDataPath()
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