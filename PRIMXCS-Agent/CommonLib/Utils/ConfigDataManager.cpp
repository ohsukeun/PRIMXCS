#include "stdafx.h"
#include "ConfigDataManager.h"
#include "ARIAEncryption.h"
#include "Base64.h"

CConfigDataManager::CConfigDataManager()
{

}

CConfigDataManager::~CConfigDataManager()
{

}

int CConfigDataManager::AtoI(LPCTSTR Data)
{
	if (NULL == Data)
	{
		return 0;
	}
	return (int)_ttof(LPTSTR(Data));
}

UINT32_T CConfigDataManager::AtoUI(LPCTSTR Data)
{
	if (NULL == Data)
	{
		return 0;
	}
	return (UINT32_T)_ttof(LPTSTR(Data));
}

DWORD CConfigDataManager::AtoDword(LPCTSTR Data)
{
	if (NULL == Data) return 0;
	return DWORD(_tstof(Data));
}

unsigned short CConfigDataManager::AtoUS(LPCTSTR Data)
{
	if (NULL == Data) return 0;
	return unsigned short(_tstof(Data));
}
BOOL CConfigDataManager::AtoB(CString strBoolString, BOOL bDefault)
{
	BOOL bRe = bDefault;
	strBoolString.Trim();

	if (0 == strBoolString.CompareNoCase(_T("Yes"))) {
		bRe = TRUE;
	}
	if (0 == strBoolString.CompareNoCase(_T("TRUE"))) {
		bRe = TRUE;
	}
	if (0 == strBoolString.CompareNoCase(_T("1"))) {
		bRe = TRUE;
	}
	if (0 == strBoolString.CompareNoCase(_T("No"))) {
		bRe = FALSE;
	}
	if (0 == strBoolString.CompareNoCase(_T("FALSE"))) {
		bRe = FALSE;
	}
	if (0 == strBoolString.CompareNoCase(_T("0"))) {
		bRe = FALSE;
	}
	return bRe;
}

BOOL CConfigDataManager::GetRegValueBoolEx(HKEY hRootKey, CString strPath, CString strValueName)
{
	CString strTemp = GetRegValueStringEx(hRootKey, strPath, strValueName);
	strTemp.Trim();
	return AtoB(strTemp,FALSE);
}

DWORD CConfigDataManager::GetRegValueDwordEx(HKEY hRootKey, CString strPath, CString strValueName)
{
	CString strTemp = GetRegValueStringEx(hRootKey, strPath, strValueName);
	strTemp.Trim();
	return AtoDword(strTemp);
}

CString CConfigDataManager::GetRegValueStringExForEnum(HKEY hKey, CString strValueName)
{
	CString strValue;
	
	strValueName.Trim();
	if (0 == strValueName.GetLength()) return strValue;

	BYTE* pTempData = NULL;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 0;
	DWORD dwReturn = ::RegQueryValueEx(hKey, strValueName, NULL, &dwType, pTempData, &dwSize);
	if (0 < dwSize) {
		pTempData = new BYTE[dwSize+2];
		memset(pTempData, 0x00, dwSize+2);

		dwReturn = ::RegQueryValueEx(hKey, strValueName, NULL, &dwType, pTempData, &dwSize);
		if (ERROR_SUCCESS == dwReturn) {
			dwReturn = ERROR_INVALID_DATA;
			CARIAEncryption ARIAEncryption;
			CString strEncryptFullData = (TCHAR*)pTempData;

			int nIndex = strEncryptFullData.Find(_T("@"));
			if (0 < nIndex) {
				CString strEncryptData = strEncryptFullData.Right(strEncryptFullData.GetLength() - (nIndex + 1));
				CString strSha512 = strEncryptFullData.Left(nIndex);

				CString strEncryptDataHash = ARIAEncryption.GetSaltSHA512(strEncryptData);
				if (0 == strEncryptDataHash.CompareNoCase(strSha512)) {

					CString strDecryptData = ARIAEncryption.DecryptString(strEncryptData);
					CString strValueNameTemp = strDecryptData.Left(strValueName.GetLength());
					if (0 == strValueNameTemp.CompareNoCase(strValueName)) {
						strValue = strDecryptData.Right(strDecryptData.GetLength() - strValueName.GetLength());
						dwReturn = ERROR_SUCCESS;
					}
				}
			}
			if (ERROR_INVALID_DATA == dwReturn) {
				::RegDeleteValue(hKey, strValueName);
			}
		}
		delete[] pTempData;
		pTempData = NULL;
	}
	return strValue;
}

CString CConfigDataManager::GetRegValueStringEx(HKEY hRootKey, CString strPath, CString strValueName)
{
	CString strValue;

	strValueName.Trim();
	strPath.Trim();

	if (0 == strValueName.GetLength()) return strValue;
	if (0 == strPath.GetLength()) return strValue;

	HKEY hKey = NULL;
	DWORD dwReturn = ::RegOpenKeyEx(hRootKey, strPath, 0, KEY_READ, &hKey);
	if (ERROR_SUCCESS == dwReturn) {

		BYTE* pTempData = NULL;
		DWORD dwType = REG_SZ;
		DWORD dwSize = 0;
		DWORD dwReturn = ::RegQueryValueEx(hKey, strValueName, NULL, &dwType, pTempData, &dwSize);
		if (0 < dwSize) {
			pTempData = new BYTE[dwSize + 2];
			memset(pTempData, 0x00, dwSize + 2);

			dwReturn = ::RegQueryValueEx(hKey, strValueName, NULL, &dwType, pTempData, &dwSize);
			if (ERROR_SUCCESS == dwReturn) {
				dwReturn = ERROR_INVALID_DATA;
				CARIAEncryption ARIAEncryption;
				CString strEncryptFullData = (TCHAR*)pTempData;

				int nIndex = strEncryptFullData.Find(_T("@"));
				if (0 < nIndex) {
					CString strEncryptData = strEncryptFullData.Right(strEncryptFullData.GetLength() - (nIndex + 1));
					CString strSha512 = strEncryptFullData.Left(nIndex);

					CString strEncryptDataHash = ARIAEncryption.GetSaltSHA512(strEncryptData);
					if (0 == strEncryptDataHash.CompareNoCase(strSha512)) {

						CString strDecryptData = ARIAEncryption.DecryptString(strEncryptData);
						CString strValueNameTemp = strDecryptData.Left(strValueName.GetLength());
						if (0 == strValueNameTemp.CompareNoCase(strValueName)) {
							strValue = strDecryptData.Right(strDecryptData.GetLength() - strValueName.GetLength());
							dwReturn = ERROR_SUCCESS;
						}
					}
				}
				if (ERROR_INVALID_DATA == dwReturn) {
					::RegDeleteValue(hKey, strValueName);
				}
			}
			delete[] pTempData;
			pTempData = NULL;
		}
		::RegCloseKey(hKey);
	}

	return strValue;
}

BOOL CConfigDataManager::SetRegValueBoolEx(HKEY hRootKey, CString strPath, CString strValueName, BOOL bValue)
{
	CString strTemp;
	strTemp.Format(_T("%s"), (TRUE==bValue? _T("TRUE") :_T("FALSE")));
	return SetRegValueStringEx(hRootKey, strPath, strValueName, strTemp);
}

BOOL CConfigDataManager::SetRegValueDwordEx(HKEY hRootKey, CString strPath, CString strValueName, DWORD dwValue)
{
	CString strTemp;
	strTemp.Format(_T("%lu"), dwValue);
	return SetRegValueStringEx(hRootKey, strPath, strValueName, strTemp);
}

BOOL CConfigDataManager::SetRegValueStringEx(HKEY hRootKey, CString strPath, CString strValueName, CString strValue)
{
	BOOL bRe = FALSE;
	
	strPath.Trim();
	strValueName.Trim();

	if (0 == strPath.GetLength()) return bRe;
	if (0 == strValueName.GetLength()) return bRe;
	
	CARIAEncryption ARIAEncryption;

	// 암호화 할 문자열 앞에 레지스트리 값이름을 Prefix로 추가한다. 
	CString strData;
	strData.Format(_T("%s%s"), strValueName, strValue);

	// 암호화 수행
	CString strEncryptData = ARIAEncryption.EncryptString(strData);

	// 암호화된 문자열의 SHA512 해시 생성
	CString strEncryptDataHash = ARIAEncryption.GetSaltSHA512(strEncryptData);

	// 해시 값과 암호화된 문자열을 구분자로 구분하여 연결함.
	CString strEncryptDataSave;
	strEncryptDataSave.Format(_T("%s@%s"), strEncryptDataHash, strEncryptData);

	HKEY	hKey = NULL;
	DWORD dwReturn = ::RegCreateKeyEx(hRootKey, strPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if (ERROR_SUCCESS == dwReturn){
		dwReturn = ::RegSetValueEx(hKey, strValueName, NULL, REG_SZ, (BYTE*)((LPWSTR)(LPCTSTR)strEncryptDataSave.GetBuffer()), strEncryptDataSave.GetLength() * sizeof(TCHAR));
		if (ERROR_SUCCESS == dwReturn) {
			bRe = TRUE;
		}
		::RegCloseKey(hKey);
	}
	return bRe;
}

CString CConfigDataManager::GetRegValue(HKEY hRootKey, CString strPath, CString strValueName)
{
	CString strValue;

	strValueName.Trim();
	strPath.Trim();

	if (0 == strValueName.GetLength()) return strValue;
	if (0 == strPath.GetLength()) return strValue;

	HKEY hKey = NULL;
	DWORD dwReturn = ::RegOpenKeyEx(hRootKey, strPath, 0, KEY_READ, &hKey);
	if (ERROR_SUCCESS == dwReturn) {
		BYTE* pTempData = NULL;
		DWORD dwType = REG_SZ;
		DWORD dwSize = 0;
		DWORD dwReturn = ::RegQueryValueEx(hKey, strValueName, NULL, &dwType, pTempData, &dwSize);
		if (0 < dwSize) {
			pTempData = new BYTE[dwSize + 2];
			memset(pTempData, 0x00, dwSize + 2);
			dwReturn = ::RegQueryValueEx(hKey, strValueName, NULL, &dwType, pTempData, &dwSize);
			if (ERROR_SUCCESS == dwReturn) {
				dwReturn = ERROR_INVALID_DATA;
				strValue = (TCHAR*)pTempData;	
				strValue.Trim();
			}
			delete[] pTempData;
			pTempData = NULL;
		}
		::RegCloseKey(hKey);
	}
	return strValue;
}

BOOL CConfigDataManager::SetRegValue(HKEY hRootKey, CString strPath, CString strValueName, CString strValue)
{
	BOOL bRe = FALSE;

	strPath.Trim();
	strValueName.Trim();

	if (0 == strPath.GetLength()) return bRe;
	if (0 == strValueName.GetLength()) return bRe;
	
	HKEY	hKey = NULL;
	DWORD dwReturn = ::RegCreateKeyEx(hRootKey, strPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if (ERROR_SUCCESS == dwReturn) {

		if (0 < strValue.GetLength()) {
			dwReturn = ::RegSetValueEx(hKey, strValueName, NULL, REG_SZ, (BYTE*)((LPWSTR)(LPCTSTR)strValue.GetBuffer()), strValue.GetLength() * sizeof(TCHAR));
			if (ERROR_SUCCESS == dwReturn) {
				bRe = TRUE;
			}
		}
		else {
			::RegDeleteValue(hKey, strValueName);
		}
		::RegCloseKey(hKey);
	}
	return bRe;
}

BOOL CConfigDataManager::SetRegValueRemove(CString strPath, CString strValueName)
{
	HKEY hKey = NULL;
	DWORD dwReturn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strPath, 0, KEY_ALL_ACCESS, &hKey);
	if (ERROR_SUCCESS == dwReturn){
		::RegDeleteValue(hKey, strValueName);
		::RegCloseKey(hKey);
	}
	return TRUE;
}

CString CConfigDataManager::GetAgentID()
{ 
	CString strAgentID = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_AGENTID);
	strAgentID.Trim();
	return strAgentID;
}

BOOL CConfigDataManager::SetAgentID(CString strNewAgentID)
{
	BOOL bRe = FALSE;

	strNewAgentID.Trim();
	if (0 < strNewAgentID.GetLength()) {
		if (TRUE == SetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_AGENTID, strNewAgentID)) {
			bRe = TRUE;
		}
	}
	else {
		bRe = SetRegValueRemove(PRIMX_REG_ROOT_PATH, PRIMX_REG_AGENTID);
	}

	return bRe;
}

CString CConfigDataManager::GetUserID()
{
	CString strUserID = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_USER_ID);
	strUserID.Trim();
	return strUserID;
}

BOOL CConfigDataManager::SetUserID(CString strUserID)
{
	BOOL bRe = FALSE;

	strUserID.Trim();
	if (0 < strUserID.GetLength()) {
		if (TRUE == SetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_USER_ID, strUserID)) {
			bRe = TRUE;
		}
	}
	else {
		bRe = SetRegValueRemove(PRIMX_REG_ROOT_PATH, PRIMX_REG_USER_ID);
	}

	return bRe;
}

BOOL CConfigDataManager::SetUserIDRemove()
{
	return SetRegValueRemove(PRIMX_REG_ROOT_PATH, PRIMX_REG_USER_ID);
}

UINT CConfigDataManager::GetSystemType()
{
	return GetRegValueDwordEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_SYSTEM_TYPE);
}

BOOL CConfigDataManager::SetSystemType(UINT nType)
{
	BOOL bRe = FALSE;
	if (TRUE == SetRegValueDwordEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_SYSTEM_TYPE, nType)) {
		bRe = TRUE;
	}
	return bRe;
}


UINT32_T CConfigDataManager::GetPollingInterval()
{
	UINT32_T nPollingInterval = 0;
	CString strRegData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_POLLING_INTERVAL);
	if (0 == strRegData.GetLength()) {
		nPollingInterval = 60;
	}
	else {
		nPollingInterval = AtoUI(strRegData);
	}
	return nPollingInterval;
}

UINT32_T CConfigDataManager::GetDebugPollingInterval()
{
	CString strRegData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_POLLING_INTERVAL_DEBUG);

	return AtoUI(strRegData);
}

CString CConfigDataManager::GetAgentVersion()
{
	CString strRegData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_AGENT_VERSION);

	return strRegData;
}

CString CConfigDataManager::GetServerAddr()
{
	CString strRegData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_SERVER_IP);
	strRegData.Trim();
	return strRegData;
}

int CConfigDataManager::GetServerIPEx(CStringList& AddrList)
{
	AddrList.RemoveAll();

	CString strRegData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_SERVER_IP);
	strRegData.Trim();
		
	int nIndex = 0;
	BOOL bExtract = TRUE;
	while (TRUE == bExtract) {
		CString strServerIP;
		bExtract = AfxExtractSubString(strServerIP, (LPCTSTR)strRegData, nIndex, '-');
		if (TRUE == bExtract) {
			strServerIP.Trim();
			if (0 < strServerIP.GetLength()) {
				AddrList.AddTail(strServerIP);
			}
		}
		nIndex++;
	}

	return AddrList.GetCount();
}

unsigned short CConfigDataManager::GetServerPort()
{
	CString strRegData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_SERVER_PORT);

	return AtoUS(strRegData);
}

BOOL CConfigDataManager::SetPollingInterval(UINT32_T nPollingInterval)
{
	return SetRegValueDwordEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_POLLING_INTERVAL, nPollingInterval);
}

BOOL CConfigDataManager::SetAgentVersion(CString strAgentVersion)
{
	BOOL bRe = FALSE;
	strAgentVersion.Trim();
	if (0 < strAgentVersion.GetLength()) {
		if (TRUE == SetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_AGENT_VERSION, strAgentVersion)) {
			bRe = TRUE;
		}
	}
	else {
		bRe = SetRegValueRemove(PRIMX_REG_ROOT_PATH, PRIMX_REG_AGENT_VERSION);
	}
	return bRe;
}

BOOL CConfigDataManager::SetServerIP(CString strServerIP)
{
	BOOL bRe = FALSE;
	strServerIP.Trim();
	if (0 < strServerIP.GetLength()) {
		if (TRUE == SetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_SERVER_IP, strServerIP)) {
			bRe = TRUE;
		}
	}
	return bRe;
}

BOOL CConfigDataManager::SetServerPort(unsigned short nServerPort)
{
	return SetRegValueDwordEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_SERVER_PORT, nServerPort);
}

BOOL CConfigDataManager::SetUpdatePort(unsigned short nUpdatePort)
{
	return SetRegValueDwordEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_UPDATE_PORT, nUpdatePort);
}

unsigned short CConfigDataManager::GetUpdatePort()
{
	CString strRegData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_UPDATE_PORT);

	return AtoUS(strRegData);
}

BOOL CConfigDataManager::SetAuthPort(unsigned short nAuthPort)
{
	return SetRegValueDwordEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_AUTH_PORT, nAuthPort);
}

unsigned short CConfigDataManager::GetAuthPort()
{
	CString strRegData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_AUTH_PORT);

	return AtoUS(strRegData);
}

CString CConfigDataManager::GetUserData()
{
	CString strUserData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_USER_DATA_PATH, PRIMX_REG_USER_DATA);
	strUserData.Trim();
	return strUserData;
}

BOOL CConfigDataManager::SetUserData(CString strUserData)
{
	BOOL bRe = FALSE;

	strUserData.Trim();
	if (0 < strUserData.GetLength()) {
		if (TRUE == SetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_USER_DATA_PATH, PRIMX_REG_USER_DATA, strUserData)) {
			bRe = TRUE;
		}
	}
	else {
		bRe = SetRegValueRemove(PRIMX_REG_ROOT_USER_DATA_PATH, PRIMX_REG_USER_DATA);
	}

	return bRe;
}

BOOL CConfigDataManager::SetUserDataRemove()
{
	return SetRegValueRemove(PRIMX_REG_ROOT_USER_DATA_PATH, PRIMX_REG_USER_DATA);
}

CString CConfigDataManager::GetLastPollingTime()
{
	CString strRegData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_LAST_POLLING_TIME);

	return strRegData;
}

BOOL CConfigDataManager::SetLastPollingTime(CString strPollingTime)
{
	BOOL bRe = FALSE;

	strPollingTime.Trim();
	if (0 < strPollingTime.GetLength()) {
		if (TRUE == SetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_LAST_POLLING_TIME, strPollingTime)) {
			bRe = TRUE;
		}
	}
	else {
		bRe = SetRegValueRemove(PRIMX_REG_ROOT_PATH, PRIMX_REG_LAST_POLLING_TIME);
	}
	return bRe;
}

BOOL CConfigDataManager::SetLogCycle(UINT nLogCycle)
{
	if (PRIMX_LOG_CYCLE_MIN> nLogCycle) {
		nLogCycle = PRIMX_LOG_CYCLE_MIN;
	}
	return SetRegValueDwordEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_LOG_INTERVAL, nLogCycle);
}

UINT CConfigDataManager::GetLogCycle()
{
	CString strRegData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_LOG_INTERVAL);
	
	UINT nLogCycle = AtoUI(strRegData);

	if (PRIMX_LOG_CYCLE_MIN> nLogCycle) {
		nLogCycle = PRIMX_LOG_CYCLE_MIN;
	}

	return nLogCycle;
}

BOOL CConfigDataManager::SetCompanyID(CString strCompanyID)
{
	BOOL bRe = FALSE;
	strCompanyID.Trim();
	if (0 < strCompanyID.GetLength()) {
		if (TRUE == SetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_COMPANY_ID, strCompanyID)) {
			bRe = TRUE;
		}
	}
	else {
		bRe = SetRegValueRemove(PRIMX_REG_ROOT_PATH, PRIMX_REG_COMPANY_ID);
	}

	return bRe;
}

CString CConfigDataManager::GetCompanyID()
{
	CString strRegData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_COMPANY_ID);

	return strRegData;
}

BOOL CConfigDataManager::SetAuthServerAddr(CString strAuthServerAddr)
{
	BOOL bRe = FALSE;

	strAuthServerAddr.Trim();
	if (0 < strAuthServerAddr.GetLength()) {
		if (TRUE == SetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_AUTH_SERVER_ADDR, strAuthServerAddr)) {
			bRe = TRUE;
		}
	}
	else {
		bRe = SetRegValueRemove(PRIMX_REG_ROOT_PATH, PRIMX_REG_AUTH_SERVER_ADDR);
	}
	return bRe;
}

CString CConfigDataManager::GetAuthServerAddr()
{
	CString strRegData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_AUTH_SERVER_ADDR);

	return strRegData;
}

BOOL CConfigDataManager::SetPolicyData(PRIMX_POLICY_DATA& PolicyData)
{
	PolicyData.strAuthWebManagerURL.Trim();
	PolicyData.strServerDateTime.Trim();
	
	if (TRUE == SetRegValueBoolEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_WEBMANAGER, PolicyData.bAuthWebManager)) {
		if (TRUE == PolicyData.bAuthWebManager) {
			SetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_WEBMANAGER_URL, PolicyData.strAuthWebManagerURL);
		}
		else {
			SetRegValueRemove( PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_WEBMANAGER_URL);
		}
	}

	if (TRUE == SetRegValueBoolEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_EXPORT_FILE, PolicyData.bAuthExportFile)) {
		if (TRUE == PolicyData.bAuthExportFile) {
			SetRegValueDwordEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_EXPORT_FILE_SHELFLIFE, PolicyData.nShelfLife);
			SetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_EXPORT_FILE_SVR_TIME, PolicyData.strServerDateTime);
		}
		else {
			SetRegValueRemove(PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_EXPORT_FILE_SHELFLIFE);
			SetRegValueRemove(PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_EXPORT_FILE_SVR_TIME);
		}
	}
	
	SetRegValueBoolEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_PRODUCT_MENU, PolicyData.bProductMenu);
	SetRegValueBoolEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_WEB_MANAGER_CONSOLE_MENU, PolicyData.bWebManagerConsoleMenu);
	SetRegValueBoolEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_EXPOR_TFILE_MENU, PolicyData.bExportFileMenu);
	

	BOOL bAuthNotEncryptionSave = FALSE;

	BOOL bOldAuthNotEncryption = GetRegValueBoolEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_NOT_ENCRYPTION);

	if (bOldAuthNotEncryption != PolicyData.bAuthNotEncryption) {
		bAuthNotEncryptionSave = TRUE;
	}
	else {
		CStringList OldAuthNotEncryptionPathList;
		GetAuthNotEncryptionPathList(OldAuthNotEncryptionPathList);
		if (FALSE == SetAuthNotEncryptionPathListCompare(PolicyData.AuthNotEncryptionPathList, OldAuthNotEncryptionPathList)) {
			bAuthNotEncryptionSave = TRUE;
		}
	}

	if (TRUE == bAuthNotEncryptionSave) {
		if (TRUE == SetRegValueBoolEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_NOT_ENCRYPTION, PolicyData.bAuthNotEncryption)) {
			if (TRUE == PolicyData.bAuthNotEncryption) {

				::RegDeleteKey(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_AUTH_NOT_ENCRYPTION_PATH);

				UINT nIndex = 1;
				POSITION Pos = PolicyData.AuthNotEncryptionPathList.GetHeadPosition();
				while (NULL != Pos) {
					CString strPath = PolicyData.AuthNotEncryptionPathList.GetNext(Pos);
					strPath.Trim();
					strPath.MakeLower();
					if (0 < strPath.GetLength()) {
						CString strValueName;
						strValueName.Format(_T("%02d"), nIndex);
						if (TRUE == SetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_AUTH_NOT_ENCRYPTION_PATH, strValueName, strPath)) {
							nIndex++;
						}
					}
				}
			}
			else {
				::RegDeleteKey(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_AUTH_NOT_ENCRYPTION_PATH);
			}
			SetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_NOT_ENCRYPTION_STAMP, GetTimeStamp());
		}
	}
	
	return TRUE;
}

BOOL CConfigDataManager::GetPolicyData(PRIMX_POLICY_DATA& PolicyData)
{
	PolicyData.Clear();
	
	PolicyData.bAuthWebManager = GetRegValueBoolEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_WEBMANAGER);
	if (TRUE == PolicyData.bAuthWebManager) {
		PolicyData.strAuthWebManagerURL = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_WEBMANAGER_URL);
	}

	PolicyData.bAuthExportFile = GetRegValueBoolEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_EXPORT_FILE);
	if (TRUE == PolicyData.bAuthExportFile) {
		PolicyData.nShelfLife = GetRegValueDwordEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_EXPORT_FILE_SHELFLIFE);
		PolicyData.strServerDateTime = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_EXPORT_FILE_SVR_TIME);
	}
	PolicyData.bProductMenu = GetRegValueBoolEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_PRODUCT_MENU);
	PolicyData.bWebManagerConsoleMenu = GetRegValueBoolEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_WEB_MANAGER_CONSOLE_MENU);
	PolicyData.bExportFileMenu = GetRegValueBoolEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_EXPOR_TFILE_MENU);
	PolicyData.bAuthNotEncryption = GetRegValueBoolEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_NOT_ENCRYPTION);
	PolicyData.strAuthNotEncryptionStamp = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_POLICY_PATH, PRIMX_REG_AUTH_NOT_ENCRYPTION_STAMP);

	if (TRUE == PolicyData.bAuthNotEncryption) {
		GetAuthNotEncryptionPathList(PolicyData.AuthNotEncryptionPathList);
	}
	
	UINT nType = GetSystemType();
	// 공용 PC
	if (PRIMX_SYS_TYPE_COMM_SETUP == nType || PRIMX_SYS_TYPE_COMM == nType) {
		PolicyData.bExportFileMenu = FALSE;				// 이동파일 Menu 표시
		PolicyData.bWebManagerConsoleMenu = FALSE;		// Web Manager Console Menu 표시
		PolicyData.bAuthWebManager = FALSE;				// Web Manager 호출 권한 
		PolicyData.bAuthExportFile = FALSE;				// 이동파일 생성 권한
		PolicyData.strAuthWebManagerURL = _T("");
	}

	return TRUE;
}

BOOL CConfigDataManager::SetAuthNotEncryptionPathListCompare(CStringList& AuthNotEncryptionPathList, CStringList& OldAuthNotEncryptionPathList)
{
	if (AuthNotEncryptionPathList.GetCount() != OldAuthNotEncryptionPathList.GetCount()) {
		return FALSE;
	}

	POSITION Pos = AuthNotEncryptionPathList.GetHeadPosition();
	while (NULL != Pos) {
		CString strData = AuthNotEncryptionPathList.GetNext(Pos);
		if (NULL == OldAuthNotEncryptionPathList.Find(strData)) {
			return FALSE;
		}
	}

	return TRUE;
}

void CConfigDataManager::GetAuthNotEncryptionPathList(CStringList& AuthNotEncryptionPathList)
{
	HKEY hKey = NULL;
	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_AUTH_NOT_ENCRYPTION_PATH, 0, KEY_READ, &hKey)) {
		DWORD dwIndex = 0;
		TCHAR SubKey[MAX_PATH + 1] = { 0 };
		while (1) {
			DWORD dwSize = MAX_PATH;
			LONG lResult = RegEnumValue(hKey, dwIndex, SubKey, &dwSize, NULL, NULL, NULL, NULL);
			if (ERROR_SUCCESS == lResult) {
				CString strValue = SubKey;
				strValue.Trim();
				CString strValueData = GetRegValueStringExForEnum(hKey, strValue);
				strValueData.Trim();
				if (0 < strValueData.GetLength()) {
					AuthNotEncryptionPathList.AddTail(strValueData);
				}
			}
			else {
				break;
			}
			dwIndex++;
		}
		::RegCloseKey(hKey);
	}
}

BOOL CConfigDataManager::SetEventRecordNumber(CString strEventSourceName, DWORD dwRecordNumber)
{
	BOOL bRe = FALSE;

	CString strValueName;
	strEventSourceName.Trim();
	strValueName.Format(_T("EventNumber-%s"), strEventSourceName);
		
	if (TRUE == SetRegValueDwordEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_EVENT_LOG_PATH, strValueName, dwRecordNumber)) {
		bRe = TRUE;
	}

	return bRe;
}

DWORD CConfigDataManager::GetEventRecordNumber(CString strEventSourceName)
{
	DWORD dwRecordNumber = 0;
	CString strValueName;
	strEventSourceName.Trim();
	strValueName.Format(_T("EventNumber-%s"), strEventSourceName);

	dwRecordNumber = GetRegValueDwordEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_EVENT_LOG_PATH, strValueName);

	return dwRecordNumber;
}

CString CConfigDataManager::GetTimeStamp()
{
	CString strTimeStamp;
	__time64_t tTimeStamp = _time64(NULL);
	strTimeStamp.Format(_T("%I64u"), tTimeStamp);
	return strTimeStamp;
}

BOOL CConfigDataManager::SetExportFileLastDirectory(CString strDirectory)
{
	BOOL bRe = FALSE;

	strDirectory.Trim();
	if (0 < strDirectory.GetLength()) {
		if (TRUE == SetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_LAST_DIR, strDirectory)) {
			bRe = TRUE;
		}
	}
	else {
		bRe = SetRegValueRemove(PRIMX_REG_ROOT_PATH, PRIMX_REG_LAST_DIR);
	}
	return bRe;
}

CString CConfigDataManager::GetExportFileLastDirectory()
{
	CString strRegData = GetRegValueStringEx(HKEY_LOCAL_MACHINE, PRIMX_REG_ROOT_PATH, PRIMX_REG_LAST_DIR);

	return strRegData;
}