#include "stdafx.h"
#include "SystemDataManager.h"
#include <lm.h>
#pragma comment(lib, "Netapi32.lib")

#include <iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")

#pragma warning(disable : 4996)


#define PRIMX_REG_OS_CURRENTVERSION_PATH				_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion")
#define PRIMX_REG_OS_RELEASEID							_T("ReleaseId")
#define PRIMX_REG_OS_PRODUCT_NAME						_T("ProductName")
#define PRIMX_REG_OS_DISPLAY_VERSION					_T("DisplayVersion")
#define PRIMX_REG_OS_UBR								_T("UBR")
#define PRIMX_REG_OS_CURRENT_BUILD_NUMBER				_T("CurrentBuildNumber")


CSystemDataManager::CSystemDataManager()
{

}

CSystemDataManager::~CSystemDataManager()
{

}

BOOL CSystemDataManager::GetSystemDataCollection(PRIMX_SYSTEM_DATA& SystemData)
{
	BOOL bRe = FALSE;
	
	if (TRUE == GetNetBiosNameEx(SystemData)) {
		bRe = TRUE;
	}

	if (TRUE == GetWindowsVersion(SystemData)) {
		bRe = TRUE;
	}

	if (TRUE == GetNICData(SystemData)) {
		bRe = TRUE;
	}

	if (TRUE == GetDiskDriveData(SystemData)) {
		bRe = TRUE;
	}

	return bRe;
}

CString CSystemDataManager::GetComputername()
{
	CString strHostName;
	TCHAR buffer[256] = TEXT("");
	int cnf[6] = { 0,1,3,4,5,7 };
	DWORD dwSize = sizeof(buffer);

	for (int i = 0; i < 6; i++) {
		if (TRUE == GetComputerNameEx((COMPUTER_NAME_FORMAT)cnf[i], buffer, &dwSize)) {
			strHostName = buffer;
			if (0 < strHostName.GetLength()) {
				break;
			}
		}
		dwSize = sizeof(buffer);
		ZeroMemory(buffer, sizeof(buffer));
	}
	return strHostName;
}

CString CSystemDataManager::SetCreateProcesseForResult(CString strProcess, UINT nTimeOutSec)
{
	CString strResult = _T("");

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE h_pipe_write, h_pipe_read;
	SECURITY_ATTRIBUTES security_attributes;

	// 선언된 구조체 변수들을 초기화한다.
	ZeroMemory(&security_attributes, sizeof(SECURITY_ATTRIBUTES));

	// 현재 프로세스가 생성한 파이프 객체를 실행될 프로세스가 사용할수 있도록
	// 보안 설정을 한다.
	security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	security_attributes.bInheritHandle = TRUE;
	security_attributes.lpSecurityDescriptor = NULL;
	// 리다이렉션에 사용할 파이프를 생성한다.
	if (FALSE == CreatePipe(&h_pipe_read, &h_pipe_write, &security_attributes, (1024 * 100))) {
		return strResult;
	}

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdOutput = h_pipe_write;
	si.hStdError = h_pipe_write;

	if (!CreateProcess(NULL, (LPTSTR)LPCTSTR(strProcess), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
		CloseHandle(h_pipe_read);
		CloseHandle(h_pipe_write);
		return strResult;
	}

	BOOL bTimeOut = FALSE;
	__time64_t tStartTime = _time64(NULL);
	// 프로세스 실행이 완료될때까지 대기
	while (WaitForSingleObject(pi.hProcess, 0) != WAIT_OBJECT_0)
	{
		__time64_t tCheckTime = _time64(NULL);

		if (nTimeOutSec < (tCheckTime - tStartTime)) {
			bTimeOut = TRUE;
			break;
		}
		Sleep(500);
	}

	if (FALSE == bTimeOut) {
		// 파이프를 통해 전달받은 정보를 저장하기 위한 변수를 선언한다.
		char pipe_data[1024 * 100] = { 0 };
		DWORD total_bytes_available = 0;
		DWORD num_bytes_read = 0;
		DWORD bytes_left_this_message = 0;

		// 명시된 파이프에 출력된 정보를 얻어온다.
		//파이프에서 데이터를 삭제하지 않고 명명한 파이프에서 데이터를 복제할 때 사용한다. 이함수는 리버스 셸에 자주 사용한다.
		BOOL bRe = PeekNamedPipe(h_pipe_read, pipe_data, 1, &num_bytes_read, &total_bytes_available, &bytes_left_this_message);

		if (TRUE == bRe) {
			// 읽을 정보가 실제로 있는 경우
			if (num_bytes_read > 0) {
				// pipe_data 에 읽은 정보를 저장한다.
				bRe = ReadFile(h_pipe_read, pipe_data, (1024 * 100) - 1, &num_bytes_read, NULL);

				if (TRUE == bRe) {
					// 읽어들인 정보의 맨 뒤에 '\0' 를 추가하여 문자열의 끝임을 명시한다.
					pipe_data[num_bytes_read] = '\0';

					//동적할당
					TCHAR* sPipeData = new TCHAR[1024 * 100];
					::MultiByteToWideChar(CP_ACP, 0, pipe_data, -1, sPipeData, (1024 * 100));
					strResult = sPipeData;
					//메모리 해제
					delete[] sPipeData;
				}
			}
		}
	}

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	CloseHandle(h_pipe_read);
	CloseHandle(h_pipe_write);

	return strResult;
}

CString CSystemDataManager::GetDNS(CString strAdapterGUID, BOOL bDHCPEnabled)
{
	CString strNameServer;

	HKEY hKey = NULL;
	CString strRegPath;
	CString strKey = (FALSE == bDHCPEnabled) ? _T("NameServer") : _T("DhcpNameServer");

	strRegPath.Format(_T("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\%s"), strAdapterGUID);

	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegPath, 0, KEY_READ, &hKey)) {
		TCHAR szDNS[1024] = { 0 };
		DWORD dwSize = 1024;
		DWORD dwType = REG_SZ;
		if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, strKey, 0, &dwType, (LPBYTE)szDNS, &dwSize)) {
			CString strTemp = szDNS;
			strTemp.Trim();
			strTemp.Replace(_T(" "), _T(","));
			int nIndex = strTemp.Find(_T(","));
			if (0 < nIndex) {
				strNameServer = strTemp.Left(nIndex);
			}
			else {
				strNameServer = strTemp;
			}
		}
		::RegCloseKey(hKey);
	}
	return strNameServer;
}

CString CSystemDataManager::GetWindowsSystemUUID()
{
	CString strUUID;
	
	HKEY	hKey = NULL;
	DWORD dwReturn = 0;

	dwReturn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\SQMClient"), 0, KEY_READ, &hKey);

	if (ERROR_SUCCESS == dwReturn) {
		TCHAR	szBuffer[MAX_PATH] = { 0 };
		DWORD	dwType = REG_SZ;
		DWORD	dwBufferSize = MAX_PATH;

		memset(szBuffer, 0, sizeof(szBuffer));
		dwReturn = ::RegQueryValueEx(hKey, _T("MachineId"), NULL, &dwType, (BYTE *)&szBuffer, &dwBufferSize);
		if (ERROR_SUCCESS == dwReturn) {
			strUUID.Format(_T("%s"), szBuffer);
			strUUID.Trim();
			strUUID.Replace(_T("{"), _T(""));
			strUUID.Replace(_T("}"), _T(""));
		}
		::RegCloseKey(hKey);
	}

	if (0 == strUUID.GetLength()) {
		CString strResult = SetCreateProcesseForResult(_T("wmic csproduct get uuid /value"));
		strResult.Trim();
		strResult.Replace(_T("\r"), _T(""));
		strResult.Replace(_T("\n"), _T(""));
		if (0 < strResult.GetLength()) {

			int nIndex = strResult.Find(_T("="));

			if (0 < nIndex) {
				int nLen = strResult.GetLength();
				CString strKey = strResult.Left(nIndex);
				CString strValue = strResult.Right(nLen - nIndex - 1);
				strKey.Trim();
				strValue.Trim();

				if (0 == strKey.CompareNoCase(_T("UUID"))) {
					if (0 < strValue.GetLength()) {
						strValue.MakeUpper();
						strValue.Trim();
						strUUID = strValue;
					}
				}
			}
		}
	}
	return strUUID;
}

BOOL CSystemDataManager::GetWindowsVersion(PRIMX_SYSTEM_DATA& SystemData)
{
	HKEY	hKey = NULL;
	DWORD dwReturn = 0;

	dwReturn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, PRIMX_REG_OS_CURRENTVERSION_PATH, 0,  KEY_READ | KEY_WOW64_64KEY,&hKey);
		
	if (ERROR_SUCCESS == dwReturn) {
		TCHAR	szBuffer[MAX_PATH] = { 0 };
		DWORD	dwType = REG_SZ;
		DWORD	dwBufferSize = MAX_PATH;

		memset(szBuffer, 0, sizeof(szBuffer));
		dwReturn = ::RegQueryValueEx(hKey, PRIMX_REG_OS_DISPLAY_VERSION, NULL, &dwType, (BYTE *)&szBuffer, &dwBufferSize);
		if (ERROR_SUCCESS == dwReturn) {
			SystemData.WindowsVersion.strReleaseID.Format(_T("%s"), szBuffer);
		}

		UINT nBuildNumber = 0;
		dwType = REG_SZ;
		dwBufferSize = MAX_PATH;
		memset(szBuffer, 0, sizeof(szBuffer));
		dwReturn = ::RegQueryValueEx(hKey, PRIMX_REG_OS_CURRENT_BUILD_NUMBER, NULL, &dwType, (BYTE *)&szBuffer, &dwBufferSize);
		if (ERROR_SUCCESS == dwReturn) {
			nBuildNumber = (UINT)_ttof(szBuffer);

			DWORD dwUBR = 0;
			dwType = REG_DWORD;
			dwReturn = ::RegQueryValueEx(hKey, PRIMX_REG_OS_UBR, NULL, &dwType, (BYTE *)&dwUBR, &dwBufferSize);
			if (ERROR_SUCCESS == dwReturn) {
				SystemData.WindowsVersion.strBuildNumber.Format(_T("%s.%lu"), szBuffer, dwUBR);
			}				
		}
		
		dwType = REG_SZ;
		dwBufferSize = MAX_PATH;
		memset(szBuffer, 0, sizeof(szBuffer));
		dwReturn = ::RegQueryValueEx(hKey, PRIMX_REG_OS_PRODUCT_NAME, NULL, &dwType, (BYTE *)&szBuffer, &dwBufferSize);
		if (ERROR_SUCCESS == dwReturn) {
			SystemData.WindowsVersion.strVersionFullName.Format(_T("%s"), szBuffer);
			if (21996 <= nBuildNumber) { // 22000
				SystemData.WindowsVersion.strVersionFullName.Replace(_T("Windows 10"), _T("Windows 11"));
			}
		}		
		::RegCloseKey(hKey);
	}

	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	BOOL bOSVersionInfoEx;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOSVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi);
	if (!bOSVersionInfoEx) {
		return FALSE;
	}

	pGNSI = (PGNSI)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");

	if (pGNSI) {
		pGNSI(&si);
	}
	else {
		GetSystemInfo(&si);
	}

	SystemData.WindowsVersion.strUUID = GetWindowsSystemUUID();
	SystemData.WindowsVersion.nMajorVersion = osvi.dwMajorVersion;
	SystemData.WindowsVersion.nMinorVersion = osvi.dwMinorVersion;
	SystemData.WindowsVersion.nServicePackMajor = osvi.wServicePackMajor;
	SystemData.WindowsVersion.nServicePackMinor = osvi.wServicePackMinor;
	SystemData.WindowsVersion.nProcessorArchitecture = si.wProcessorArchitecture;
	
	if (0 == SystemData.WindowsVersion.strBuildNumber.GetLength()) {
		SystemData.WindowsVersion.strBuildNumber.Format(_T("%lu"), osvi.dwBuildNumber);
	}

	return TRUE;
}

BOOL CSystemDataManager::GetNetBiosNameEx(PRIMX_SYSTEM_DATA& SystemData)
{
	DWORD dwLevel = 102;
	LPWKSTA_INFO_102 pBuf = NULL;
	NET_API_STATUS nStatus;
	LPTSTR pszServerName = NULL;
	nStatus = NetWkstaGetInfo(pszServerName, dwLevel, (LPBYTE *)&pBuf);
	if (NERR_Success == nStatus) {
		SystemData.NetBiosData.strWorkGroupName = pBuf->wki102_langroup;
		SystemData.NetBiosData.strComputerName = pBuf->wki102_computername;
	}
	else {
		SystemData.NetBiosData.strComputerName = GetComputername();
	}
	if (NULL != pBuf) {
		NetApiBufferFree(pBuf);
	}
	return TRUE;
}

BOOL CSystemDataManager::GetDiskDriveData(PRIMX_SYSTEM_DATA& SystemData)
{
	DWORD dwDrive = GetLogicalDrives();

	if (0 == dwDrive){
		return FALSE;
	}

	for (short n = 0; n<32; n++){

		if ((1 << n) & dwDrive){
			PRIMX_DISK_DRIVE_DATA* pDiskData = new PRIMX_DISK_DRIVE_DATA;
			
			pDiskData->strName.Format(_T("%c:\\"), _T('A' + n));
			pDiskData->nType = ::GetDriveType(pDiskData->strName);
			
			if (DRIVE_UNKNOWN == pDiskData->nType || DRIVE_NO_ROOT_DIR == pDiskData->nType || DRIVE_REMOTE == pDiskData->nType)	{
				_SafeDelete(pDiskData);
				continue;
			}

			DWORD dwVolID = 0;
			DWORD dwMaxLen = 0;
			DWORD dwFSAtt = 0;

			TCHAR szVolName[50] = { 0 };
			TCHAR szFSName[300] = { 0 };
			
			BOOL bVolResult = GetVolumeInformation(pDiskData->strName, szVolName, 50, &dwVolID, &dwMaxLen, &dwFSAtt, szFSName, 300);
			if (0 != bVolResult){
				pDiskData->strVolumeName = szVolName;
				pDiskData->strFileSystemName = szFSName;

				ULARGE_INTEGER uAvailUser;
				ULARGE_INTEGER uTotal;
				ULARGE_INTEGER uFree;

				if (0 != GetDiskFreeSpaceEx(pDiskData->strName, &uAvailUser, &uTotal, &uFree)){
					pDiskData->nTotalSize = unsigned long(uTotal.QuadPart / 1024 / 1024);
					pDiskData->nFreeSize = unsigned long(uFree.QuadPart / 1024 / 1024);					
				}
			}
			else{
				DWORD dwErrCode = GetLastError();
				TRACE(_T("GetLastError : %d \n"), dwErrCode);
			}

			if (NULL == SystemData.DiskDriveList.AddTail(pDiskData)) {
				_SafeDelete(pDiskData);
			}
		}
	}
	return TRUE;
}

BOOL CSystemDataManager::GetNICData(PRIMX_SYSTEM_DATA& SystemData)
{
	BOOL bRe = FALSE;

	IP_ADAPTER_INFO* AdapterInfo = NULL;
	DWORD dwBufLen = 0;

	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	if (ERROR_BUFFER_OVERFLOW == dwStatus) {
		int nAdapterInfoLen = sizeof(IP_ADAPTER_INFO);
		int nNICount = (dwBufLen / nAdapterInfoLen) + (0<(dwBufLen % nAdapterInfoLen) ? 1 : 0);
		if (0 < nNICount) {
			AdapterInfo = new IP_ADAPTER_INFO[nNICount];
			dwBufLen = nNICount * sizeof(IP_ADAPTER_INFO);
		}

		dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
		if (ERROR_SUCCESS == dwStatus) {
			
			bRe = TRUE;

			PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
			while (NULL != pAdapterInfo) {

				PRIMX_NIC_DATA* pData = new PRIMX_NIC_DATA;
				
				pData->strAdapterGUID = AtoWConverter(pAdapterInfo->AdapterName);
				pData->strDescription = AtoWConverter(pAdapterInfo->Description);

				// DNS 수집
				pData->strDNS = GetDNS(pData->strAdapterGUID, (1==pAdapterInfo->DhcpEnabled? TRUE : FALSE ));

				// MAC 수집
				for (UINT i = 0; i<pAdapterInfo->AddressLength; i++) {
					CString strTemp;
					if (0 == i) {
						strTemp.Format(_T("%02X"), pAdapterInfo->Address[i]);
					}
					else {
						strTemp.Format(_T("-%02X"), pAdapterInfo->Address[i]);
					}
					pData->strMAC = pData->strMAC + strTemp;
				}

				// 게이트웨이 IP 수집
				PIP_ADDR_STRING pGWAddressList = &(pAdapterInfo->GatewayList);
				while (NULL != pGWAddressList) {
					CString strTemp = AtoWConverter(pGWAddressList->IpAddress.String);
					if (0 == strTemp.GetLength()) {
						strTemp = _T("0.0.0.0");
					}
					if (0 != strTemp.CompareNoCase(_T("0.0.0.0"))) {
						pData->strGW = strTemp;
						break;
					}
					pGWAddressList = pGWAddressList->Next;
				}
				pData->strGW.Trim();

				// DHCP 서버 IP 수집
				PIP_ADDR_STRING pDHCPServerAddressList = &(pAdapterInfo->DhcpServer);
				while (NULL != pDHCPServerAddressList) {
					CString strTemp = AtoWConverter(pDHCPServerAddressList->IpAddress.String);
					if (0 == strTemp.GetLength()) {
						strTemp = _T("0.0.0.0");
					}
					if (0 != strTemp.CompareNoCase(_T("0.0.0.0"))) {
						pData->strDHCPServer = strTemp;
						break;
					}
					pDHCPServerAddressList = pDHCPServerAddressList->Next;
				}
				pData->strDHCPServer.Trim();
				pData->nEnableDHCP = (1==pAdapterInfo->DhcpEnabled? 1: 0);

				// IP수집
				pData->strIP = _T("0.0.0.0");
				PIP_ADDR_STRING pIpAddressList = &(pAdapterInfo->IpAddressList);
				while (NULL != pIpAddressList) {
					CString strTemp = AtoWConverter(pIpAddressList->IpAddress.String);
					if (0 < strTemp.GetLength()) {
						pData->strIP = strTemp;
						break;
					}		
					pIpAddressList = pIpAddressList->Next;
				}			

				if (NULL == SystemData.NICList.AddTail(pData)) {
					_SafeDelete(pData);
				}
				pAdapterInfo = pAdapterInfo->Next;
			}
		}
	}

	if (NULL != AdapterInfo) {
		delete[] AdapterInfo;
		AdapterInfo = NULL;
	}

	return bRe;
}

CString CSystemDataManager::GetRegQueryValueString(HKEY hKey, CString strValueName)
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

DWORD CSystemDataManager::GetRegQueryValueDword(HKEY hKey, CString strValueName)
{
	DWORD dwData = 0;
	DWORD dwBufferSize = sizeof(dwData);
	LSTATUS nResult = RegQueryValueEx(hKey, strValueName, NULL, NULL, (LPBYTE)&dwData, &dwBufferSize);
	if (ERROR_SUCCESS != nResult) {
		dwData = 0;
	}
	return dwData;
}

BOOL CSystemDataManager::GetPRIMXProductInfo(PRIMX_PRODUCT_DATA& Zonecentrol, PRIMX_PRODUCT_DATA& Orizone, PRIMX_PRODUCT_DATA& Cryhod)
{
	HKEY hKey;
	DWORD	dwCnt = 0;
	CString strPath = _T("software\\microsoft\\windows\\currentversion\\uninstall");

	LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, strPath, 0, KEY_READ, &hKey);

	if (ERROR_SUCCESS != lResult) {
		return FALSE;
	}
	Zonecentrol.bInstall = FALSE;
	Orizone.bInstall = FALSE;
	Cryhod.bInstall = FALSE;

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
					bProduct = TRUE;
				}
				else if (0 == strDisplayName.CompareNoCase(_T("ZONECENTRAL"))) {
					bProduct = TRUE;
				}
				else if (0 == strDisplayName.CompareNoCase(_T("CRYHOD"))) {
					bProduct = TRUE;
				}

				if (TRUE == bProduct) {
					PRIMX_PRODUCT_DATA ProductData;	
					ProductData.bInstall = TRUE;
					ProductData.strUninstallString = GetRegQueryValueString(hSubKey, _T("UninstallString"));
					ProductData.strDisplayName = GetRegQueryValueString(hSubKey, _T("DisplayName"));
					ProductData.strDisplayVersion = GetRegQueryValueString(hSubKey, _T("DisplayVersion"));
					ProductData.strInstallLocation = GetRegQueryValueString(hSubKey, _T("InstallLocation"));
					ProductData.strInstallDate = GetRegQueryValueString(hSubKey, _T("InstallDate"));
					ProductData.dwVersionMajor = GetRegQueryValueDword(hSubKey, _T("VersionMajor"));
					ProductData.dwVersionMinor = GetRegQueryValueDword(hSubKey, _T("VersionMinor"));
					ProductData.dwVersion = GetRegQueryValueDword(hSubKey, _T("Version"));

					CString strHexVersion;
					CString strPatchVersion[2];
					UINT nPatchVersion[2] = { 0,0 };

					strHexVersion.Format(_T("%x"), ProductData.dwVersion);
					strPatchVersion[0] = strHexVersion.Right(1);
					strPatchVersion[1] = strHexVersion.Mid(strHexVersion.GetLength() - 2, 1);

					nPatchVersion[0] = _tcstol(strPatchVersion[0], NULL, 16);
					nPatchVersion[1] = _tcstol(strPatchVersion[1], NULL, 16);
					ProductData.strDisplayVersionEx.Format(_T("%lu.%lu.%02d%02d"), ProductData.dwVersionMajor, ProductData.dwVersionMinor, nPatchVersion[1], nPatchVersion[0]);
					
					if (0 == strDisplayName.CompareNoCase(_T("ORIZON"))) {
						Orizone= ProductData;
					}
					else if (0 == strDisplayName.CompareNoCase(_T("ZONECENTRAL"))) {
						Zonecentrol = ProductData;
					}
					else if (0 == strDisplayName.CompareNoCase(_T("CRYHOD"))) {
						Cryhod = ProductData;
					}
				}
			}
		}
		++dwCnt;
	}
	RegCloseKey(hKey);

	return TRUE;
}

