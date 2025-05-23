#include "stdafx.h"
#include "PRIMXAgentManager.h"
#include "FileDBManager.h"
#include "ProcessRun.h"
#include "ZombieProcessCheck.h"
#include "IPCChannel.h"

#include "SystemDataManager.h"
#include "CommandSystemReg.h"
#include "CommandPolling.h"
#include "DataCollectManager.h"
#include "SystemDataModifyCheck.h"
#include "CommandUpdateFile.h"
#include "ZIPFile.h"
#include "sha512.h"

CPRIMXAgentManager::CPRIMXAgentManager()
{
	m_bRun = FALSE;
	m_nPollingInterval = 0;
	m_nDebugPollingInterval = 0;
	m_nPollingTime = 0;
	m_nLogClearHour = 0;
	m_nServerPort = 0;
	m_nUpdatePort = 0;
	m_nAuthPort = 0;

	m_bInstallFailureCode[0] = FALSE;
	m_bInstallFailureCode[1] = FALSE;
	m_bInstallFailureCode[2] = FALSE;

	m_nSystemType = PRIMX_SYS_TYPE_USER;

	m_pIPCServer = NULL;
	m_bIPSServerStart = FALSE;

	m_pDataCollectManager = NULL;

	struct tm ConvertTime;
	__time64_t tCurrentTime = _time64(NULL);

	if (0 == _localtime64_s(&ConvertTime, &tCurrentTime)) {
		m_nLogClearHour = ConvertTime.tm_hour;
		if (23 < m_nLogClearHour) m_nLogClearHour = 0;
	}
	else {
		m_nLogClearHour = 0;
	}

	m_nSystemModCheckTime = 0;
	m_pSystemDataModifyCheck = NULL;

	InitializeCriticalSectionAndSpinCount(&m_csLock, 4000);
}


CPRIMXAgentManager::~CPRIMXAgentManager()
{
	SetClose();
	DeleteCriticalSection(&m_csLock);

}

BOOL CPRIMXAgentManager::SetCreate()
{
	GetConfigData();

	CFileDBManager FileDBManager;
	FileDBManager.SetInitDataBase();
	FileDBManager.SetInitDataBaseForLogData();

	SetEnvironmentVariableForPRIMXAgent();
	
	m_bRun = TRUE;
	if (FALSE == Start()) {
		return FALSE;
	}

	DWORD dwServicesPipeTimeout = GetServicesPipeTimeout();
	if (90 > dwServicesPipeTimeout) {
		SetServicesPipeTimeout(90);
	}

	m_pDataCollectManager = new CDataCollectManager;
	m_pDataCollectManager->SetStart();

	m_pSystemDataModifyCheck = new CSystemDataModifyCheck;
	m_pSystemDataModifyCheck->GetSystemData();

	return TRUE;
}

void CPRIMXAgentManager::SetClose()
{
	m_bRun = FALSE;
	WaitForTermination(2000);

	DWORD dwServicesPipeTimeout = GetServicesPipeTimeout();
	if (90 > dwServicesPipeTimeout) {
		SetServicesPipeTimeout(90);
	}

	if (NULL != m_pDataCollectManager) {
		m_pDataCollectManager->SetStop();
		_SafeDelete(m_pDataCollectManager);
	}

	if (NULL != m_pIPCServer) {
		m_pIPCServer->SetServerStop();
		delete m_pIPCServer;
		m_pIPCServer = NULL;
	}

	m_ServerIPList.RemoveAll();
}

DWORD CPRIMXAgentManager::GetServicesPipeTimeout()
{
	DWORD dwTimeout = 0;
	HKEY phkResult;
	HKEY hKey = HKEY_LOCAL_MACHINE;
	LONG nError = ::RegOpenKeyEx(hKey, _T("SYSTEM\\CurrentControlSet\\Control"), 0, KEY_READ | KEY_WOW64_64KEY, &phkResult);
	if (ERROR_SUCCESS != nError) {
		return dwTimeout;
	}

	BOOL bRe = FALSE;
	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwValue = 0;

	LONG nRegRe = ::RegQueryValueEx(phkResult, _T("ServicesPipeTimeout"), NULL, &dwType, (BYTE*)&dwValue, &dwSize);

	if (ERROR_SUCCESS == nRegRe) {
		dwTimeout = dwValue / 1000;
	}
	else {
		dwTimeout = 0;
	}
	::RegCloseKey(phkResult);

	return dwTimeout;
}

void CPRIMXAgentManager::SetServicesPipeTimeout(DWORD dwTimeoutSec)
{
	HKEY	hKey = NULL;
	DWORD dwReturn = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if (dwReturn == ERROR_SUCCESS)
	{
		dwTimeoutSec = dwTimeoutSec * 1000;
		dwReturn = ::RegSetValueEx(hKey, _T("ServicesPipeTimeout"), NULL, REG_DWORD, (BYTE *)&dwTimeoutSec, sizeof(DWORD));
		::RegCloseKey(hKey);
	}

}

void CPRIMXAgentManager::SetEnvironmentVariableForPRIMXAgent()
{
	if (0 < m_strAgentID.GetLength()) {
		HKEY	hKey = NULL;
		DWORD dwReturn = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("System\\CurrentControlSet\\Control\\Session Manager\\Environment"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
		if (dwReturn == ERROR_SUCCESS)
		{
			dwReturn = ::RegSetValueEx(hKey, _T("PRIMXAGENT"), NULL, REG_SZ, (BYTE*)((LPWSTR)(LPCTSTR)m_strAgentID.GetBuffer()), m_strAgentID.GetLength() * sizeof(TCHAR));
			::RegCloseKey(hKey);
			::PostMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)TEXT("Environment"));
		}
	}
}

BOOL CPRIMXAgentManager::GetConfigData()
{
	m_strAgentID = m_ConfigDataManager.GetAgentID();
	m_strUserID = m_ConfigDataManager.GetUserID();
	m_nSystemType = m_ConfigDataManager.GetSystemType();
	SetPollingInterval(m_ConfigDataManager.GetPollingInterval());
	m_nDebugPollingInterval = m_ConfigDataManager.GetDebugPollingInterval();
	m_strAgentVersion = m_ConfigDataManager.GetAgentVersion();
	m_ConfigDataManager.GetServerIPEx(m_ServerIPList);
	m_nServerPort = m_ConfigDataManager.GetServerPort();
	m_nUpdatePort = m_ConfigDataManager.GetUpdatePort();
	m_nAuthPort = m_ConfigDataManager.GetAuthPort();

	if (FALSE == GetWindowsSystemUUID(m_strWindowsUUID)) {
		m_strWindowsUUID = _T("");
	}

	CString strConfigPath;
	strConfigPath.Format(_T("%s\\%s"), LPCTSTR(GetLocalPath()), PRIMX_CONFIG_FILE_PATH);

	DWORD dwAttrs = GetFileAttributes(strConfigPath);
	if (dwAttrs != INVALID_FILE_ATTRIBUTES) {
		if (dwAttrs & FILE_ATTRIBUTE_READONLY) {
			SetFileAttributes(strConfigPath, dwAttrs & ~FILE_ATTRIBUTE_READONLY);
		}
	}

	if (0 == m_ServerIPList.GetCount()) {
		CString strServerIP = GetProfileStringEx(PRIMX_CONFIG_SERVICE_SESSION, PRIMX_CONFIG_SERVER_ADDR, _T(""), strConfigPath);
		m_nServerPort = unsigned short(GetProfileIntEx(PRIMX_CONFIG_SERVICE_SESSION, PRIMX_CONFIG_SERVER_PORT, 50101, strConfigPath));
		m_nUpdatePort = unsigned short(GetProfileIntEx(PRIMX_CONFIG_SERVICE_SESSION, PRIMX_CONFIG_UPDATE_PORT, 50102, strConfigPath));
		m_nAuthPort = unsigned short(GetProfileIntEx(PRIMX_CONFIG_SERVICE_SESSION, PRIMX_CONFIG_AUTH_PORT, 50103, strConfigPath));

		strServerIP.Trim();

		if (0 < strServerIP.GetLength()) {
			m_ConfigDataManager.SetServerIP(strServerIP);
			m_ConfigDataManager.SetServerPort(m_nServerPort);
			m_ConfigDataManager.SetUpdatePort(m_nUpdatePort);
			m_ConfigDataManager.SetAuthPort(m_nAuthPort);

			m_ConfigDataManager.GetServerIPEx(m_ServerIPList);
		}
	}
	if (0 == m_nServerPort) {
		m_nServerPort = unsigned short(GetProfileIntEx(PRIMX_CONFIG_SERVICE_SESSION, PRIMX_CONFIG_SERVER_PORT, 50101, strConfigPath));
		m_ConfigDataManager.SetServerPort(m_nServerPort);
	}
	if (0 == m_nUpdatePort) {
		m_nUpdatePort = unsigned short(GetProfileIntEx(PRIMX_CONFIG_SERVICE_SESSION, PRIMX_CONFIG_UPDATE_PORT, 50102, strConfigPath));
		m_ConfigDataManager.SetServerPort(m_nUpdatePort);

	}
	if (0 == m_nAuthPort) {
		m_nAuthPort = unsigned short(GetProfileIntEx(PRIMX_CONFIG_SERVICE_SESSION, PRIMX_CONFIG_AUTH_PORT, 50103, strConfigPath));
		m_ConfigDataManager.SetServerPort(m_nAuthPort);
	}
	
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Config Data > AgentID [%s]"), m_strAgentID);
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Config Data > UserID [%s]"), m_strUserID);
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Config Data > SystemType [%d]"), m_nSystemType);
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Config Data > Windows UUID [%s]"), m_strWindowsUUID);
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Config Data > Polling Interval [%lu]"), m_nPollingInterval);
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Config Data > DebugPollingInterval [%lu]"), m_nDebugPollingInterval);
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Config Data > AgentVersion [%s]"), m_strAgentVersion);
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Config Data > ServerIP [%s], Port [%d], Update Port [%d], Auth Port [%d]"), m_ConfigDataManager.GetServerAddr(), m_nServerPort, m_nUpdatePort, m_nAuthPort);
	
	return TRUE;
}

void CPRIMXAgentManager::SetPollingInterval(UINT32_T nPollingInterval)
{
	if (0 == nPollingInterval) {
		nPollingInterval = (60 * 5);
	}
	m_nPollingInterval = nPollingInterval;
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Polling Interval 설정 [%lu Sec]"), m_nPollingInterval);
}

UINT32_T CPRIMXAgentManager::GetPollingInterval()
{
	UINT32_T nPollingInterval = m_nPollingInterval;
	if (0 < m_nDebugPollingInterval) {
		if (60 > m_nDebugPollingInterval) {
			m_nDebugPollingInterval = 60;
		}
		nPollingInterval = m_nDebugPollingInterval;
	}
	else {
		if (60 > nPollingInterval) {
			nPollingInterval = 60;
		}
	}
	return nPollingInterval;
}

void CPRIMXAgentManager::Run()
{
	BOOL bIPCServerStart = FALSE;
		
	while (m_bRun) {

		if (FALSE == bIPCServerStart) {
			bIPCServerStart = IPCServerToAgentCSStart();
		}
	
		BOOL bRe = SetSystemRegCheck();

		if (FALSE == SetPolling(bRe)) {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Agent 삭제 명령 수신 > 메인 쓰레드 종료"));
			break;
		}

		if (FALSE == bRe) {
			SetSystemModCheck();
		}
		else{
			m_pSystemDataModifyCheck->GetSystemData();
		}
		
		SetLogFileDeleteCheck();

		Sleep(1000);
	}
}

void CPRIMXAgentManager::IPCNotification(const char* pData, UINT nDataLen, LPVOID lpObject)
{
	CPRIMXAgentManager* pthis = (CPRIMXAgentManager*)lpObject;
	if(NULL != pthis) pthis->AgentCSRecvData(pData, nDataLen);
}

void CPRIMXAgentManager::AgentCSRecvData(const char* pData, UINT nDataLen)
{
	if (NULL != pData) {
		if (PRIMXCS_REQUEST_DATA_LEN <= nDataLen) {
			PRIMXCS_REQUEST_DATA* pRequestData = (PRIMXCS_REQUEST_DATA*)pData;

			if (PRIMX_REQUEST_TYPE_USER_REG == pRequestData->nRequestType) {
				PRIMXCS_REQUEST_DATA* pResponsedata = new PRIMXCS_REQUEST_DATA;
				pResponsedata->nRequestType = pRequestData->nRequestType;

				CARIAEncryption ARIAEncryption;
				CString strUSerID = ARIAEncryption.DecryptString(pRequestData->strUserID);
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("사용자 등록 요청 [%s]"), strUSerID);

				CString strFailString;
				if (TRUE == SetSystemUserReg(strUSerID, PRIMX_SYS_TYPE_USER, strFailString)) {
					pResponsedata->nResult = 1;
				}
				else {
					pResponsedata->nResult = 0;
					_tcsncpy_s(pResponsedata->strFailMessage, PRIMX_MESSAGE_LEN, (LPCTSTR)strFailString, _TRUNCATE);
				}
				if (NULL != m_pIPCServer) {
					m_pIPCServer->SetSendData((char*)pResponsedata, PRIMXCS_REQUEST_DATA_LEN,10);
				}

				_SafeDelete(pResponsedata);		
			}

			if (PRIMX_REQUEST_TYPE_USER_DATA_SAVE == pRequestData->nRequestType) {
				PRIMXCS_REQUEST_DATA* pResponsedata = new PRIMXCS_REQUEST_DATA;
				pResponsedata->nRequestType = pRequestData->nRequestType;

				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("사용자 정보 저장 요청 수신"));

				CString strTemp = (TCHAR*)(pData + PRIMXCS_REQUEST_DATA_LEN);

				CARIAEncryption ARIAEncryption;
				CString strUserData = ARIAEncryption.DecryptString(strTemp);
				
				CConfigDataManager ConfigDataManager;
				if (TRUE == ConfigDataManager.SetUserData(strUserData)) {
					pResponsedata->nResult = 1;
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("사용자 정보 저장 요청 수신 > 저장 성공"));

				}
				else {
					pResponsedata->nResult = 0;
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("사용자 정보 저장 요청 수신 > 저장 실패"));
				}
				if (NULL != m_pIPCServer) {
					m_pIPCServer->SetSendData((char*)pResponsedata, PRIMXCS_REQUEST_DATA_LEN, 10);
				}
				_SafeDelete(pResponsedata);
			}
		}
	}
}

BOOL CPRIMXAgentManager::IPCServerToAgentCSStart()
{
	if (NULL == m_pIPCServer) {
		m_pIPCServer = new CIPCChannel;
	}

	BOOL bRe = m_pIPCServer->SetServerStart(PRIMX_IPC_AGNET_CS_KEY, 1024 * 10);
	if (TRUE == bRe)
	{
		m_pIPCServer->SetReceiveNotification(&IPCNotification, this);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"프로세스간 통신 서버 시작 (성공)");
	}
	else
	{
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"프로세스간 통신 서버 시작 (실패) > %s" ,m_pIPCServer->GetLastErrorString());
	}
	return bRe;
}

void CPRIMXAgentManager::SetLogFileDeleteCheck()
{
	__time64_t tCurrentTime = _time64(NULL);
	struct tm ConvertTime;
	if (0 == _localtime64_s(&ConvertTime, &tCurrentTime)) {
		// 30일 이전 로그는 삭제 처리함
		if (m_nLogClearHour == ConvertTime.tm_hour) {
			m_nLogClearHour = ConvertTime.tm_hour + 1;
			if (23 < m_nLogClearHour) m_nLogClearHour = 0;
			theLogManager.SetLogFileDelete(30);
		}
	}

}

BOOL CPRIMXAgentManager::SetSystemUserReg(CString strUserID,UINT nSystemType, CString& strErrorString)
{
	BOOL bRe = FALSE;
	CSystemDataManager SystemDataManager;
	PRIMX_SYSTEM_DATA SystemData;

	if (0 == m_strAgentID.GetLength()) {
		m_strAgentID = GetCreateKey();
		m_ConfigDataManager.SetAgentID(m_strAgentID);
	}	

	if (TRUE == SystemDataManager.GetSystemDataCollection(SystemData)) {
		POSITION Pos = m_ServerIPList.GetHeadPosition();
		while (NULL != Pos) {
			CString strServerIP = m_ServerIPList.GetNext(Pos);

			CCommandSystemReg CommandSystemReg;

			if (PRIMX_SYS_TYPE_USER == nSystemType) {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"사용자 정보 등록 > 연결 시작 > %s/%d", strServerIP, m_nServerPort);
			}
			else {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"공용 PC 정보 등록 > 연결 시작 > %s/%d", strServerIP, m_nServerPort);
			}

			if (TRUE == CommandSystemReg.SetConnect(strServerIP, m_nServerPort)) {
				if (TRUE == CommandSystemReg.SetSystemDataReg(strUserID, nSystemType,SystemData)) {
					m_ConfigDataManager.SetAgentID(m_strAgentID);
					if (PRIMX_SYS_TYPE_USER == nSystemType) {
						m_ConfigDataManager.SetUserID(strUserID);
						m_ConfigDataManager.SetSystemType(PRIMX_SYS_TYPE_USER);
					}
					else {
						m_ConfigDataManager.SetUserIDRemove();
						m_ConfigDataManager.SetSystemType(PRIMX_SYS_TYPE_COMM);
					}
					CFileDBManager CFileDBManager;
					CFileDBManager.SetSystemDataDBSave(m_strAgentID, strUserID, nSystemType, SystemData,TRUE);
					bRe = TRUE;
					break;
				}
				else {
					CString strFailCode = CommandSystemReg.GetFailCode();
					CString strFailString = CommandSystemReg.GetFailString();
					strErrorString = GetFailStringConvert(strFailCode, strFailString);

					if (PRIMX_SYS_TYPE_USER == nSystemType) {
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"사용자 정보 등록 > 등록 실패 > [%s] %s", strFailCode, strFailString);
					}
					else {
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"공용 PC 정보 등록 > 등록 실패 > [%s] %s", strFailCode, strFailString);
					}
				}
			}
			else {
				strErrorString.LoadString(IDS_ERROR_STRING1);

				if (PRIMX_SYS_TYPE_USER == nSystemType) {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"사용자 정보 등록 > 연결 실패 > %s", CommandSystemReg.GetLastErrorString());
				}
				else {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"공용 PC 정보 등록 > 연결 실패 > %s", CommandSystemReg.GetLastErrorString());
				}
			}	
			Sleep(500);
		}		
	}
	else {
		strErrorString.LoadString(IDS_ERROR_STRING2);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"시스템 정보 수집 실패");
	}
	
	return bRe;
}

BOOL CPRIMXAgentManager::SetPolling(BOOL bNow)
{
	BOOL bRe = TRUE;

	if (TRUE == bNow) {
		m_nPollingTime = 0;
	}

	if (0 < m_strAgentID.GetLength() || 0 < m_strUserID.GetLength()) {
		__time64_t tCurrentTime = _time64(NULL);
		if (m_nPollingTime < tCurrentTime) {
			
			POSITION Pos = m_ServerIPList.GetHeadPosition();
			while (NULL != Pos) {
				CString strServerIP = m_ServerIPList.GetNext(Pos);

				CCommandPolling CommandPolling;
				if (TRUE == CommandPolling.SetConnect(strServerIP, m_nServerPort)) {
					if (TRUE == CommandPolling.SetPolling()) {

						COleDateTime dtCurrentDateTime = COleDateTime::GetCurrentTime();
						SetPollingInterval(CommandPolling.GetKeepAliveCycle());
						m_ConfigDataManager.SetLastPollingTime(dtCurrentDateTime.Format(L"%Y-%m-%d %H:%M:%S"));
						m_ConfigDataManager.SetPollingInterval(GetPollingInterval());
						m_ConfigDataManager.SetLogCycle(CommandPolling.GetLogCycle());
						m_ConfigDataManager.SetCompanyID(CommandPolling.GetCompanyID());
						m_ConfigDataManager.SetAuthServerAddr(CommandPolling.GetAuthServerAddr());
						m_ConfigDataManager.SetAuthPort(CommandPolling.GetAuthServerPort());

						PRIMX_POLICY_DATA PolicyData;
						if (TRUE == CommandPolling.GetPolicyData(PolicyData)) {
							m_ConfigDataManager.SetPolicyData(PolicyData);
						}

						SetUpdateCheck(CommandPolling);

						CString strExpandCommand = CommandPolling.GetExpandCommand();
						if (0 < strExpandCommand.GetLength()) {
							theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"Polling > 확장 명령 수신 > %s", strExpandCommand);

							if (0 == strExpandCommand.CompareNoCase(PRIMX_EXPAND_COMMAND_EX001)) { // 등록된 Agent ID가 없는 경우 시스템 정보를 등록하도록 명령
								theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"확장 명령[%s] > 등록된 AgentID 미존재로 재등록", strExpandCommand);

								CString strErrorString;
								UINT nType = m_ConfigDataManager.GetSystemType();
								CString strUserID = m_ConfigDataManager.GetUserID();
								if (TRUE == SetSystemUserReg(strUserID, nType, strErrorString)) {
									theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"재등록 > 사용자PC 등록 성공 > %s", m_ConfigDataManager.GetAgentID());
								}
								else {
									theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"재등록 > 사용자PC 등록 실패 > %s", strErrorString);
								}
							}
							if (0 == strExpandCommand.CompareNoCase(PRIMX_EXPAND_COMMAND_EX002)) { // 등록된 사용자ID가 존재하지 않는 경우 사용자ID 재등록 명령
								theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"확장 명령[%s] > 등록된 사용자ID 미존재로 재등록", strExpandCommand);
								m_ConfigDataManager.SetUserIDRemove();
							}
							if (0 == strExpandCommand.CompareNoCase(PRIMX_EXPAND_COMMAND_EX003)) { // PRIMX Agent 삭제 명령
								theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"확장 명령[%s] > PRIMX Agent 삭제", strExpandCommand);
								SetPRIMXAgentUninstall();
								bRe = FALSE;
							}
							if (0 == strExpandCommand.CompareNoCase(PRIMX_EXPAND_COMMAND_EX004)) { // 사용자PC로 변경
								theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"확장 명령[%s] > 사용자 시스템으로 변경", strExpandCommand);
								m_ConfigDataManager.SetUserIDRemove();
								m_ConfigDataManager.SetSystemType(PRIMX_SYS_TYPE_USER);
							}
							if (0 == strExpandCommand.CompareNoCase(PRIMX_EXPAND_COMMAND_EX005)) { // 공용PC로 변경
								theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"확장 명령[%s] > 공용 시스템으로 변경", strExpandCommand);
								m_ConfigDataManager.SetUserIDRemove();
								m_ConfigDataManager.SetSystemType(PRIMX_SYS_TYPE_COMM);

								CString strErrorString;
								UINT nType = m_ConfigDataManager.GetSystemType();
								if (TRUE == SetSystemUserReg(_T(""), nType, strErrorString)) {
									theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"재등록 > 사용자PC 등록 성공 > %s", m_ConfigDataManager.GetAgentID());
								}
								else {
									theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"재등록 > 사용자PC 등록 실패 > %s", strErrorString);
								}
							}
						}
					}
					break;
				}
				Sleep(500);
			}
			m_nPollingTime = _time64(NULL) + GetPollingInterval();
		}
	}
	return bRe;
}

CString CPRIMXAgentManager::GetFailStringConvert(CString strFailCode, CString strFailString)
{
	CString strFailMsg;

	strFailMsg = strFailString;

	return strFailMsg;
}

BOOL CPRIMXAgentManager::SetSystemRegCheck()
{
	BOOL bRe = FALSE;

	UINT nType = m_ConfigDataManager.GetSystemType();
	CString strUserID = m_ConfigDataManager.GetUserID();
	
 	if (PRIMX_SYS_TYPE_USER == nType) {
		if (0 == strUserID.GetLength()) { 
			// 사용자PC 설정 > 사용자ID 없음 => 사용자 등록 유도 (PRIMXAgentCS에서 처리)
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"사용자PC 설정 > 사용자ID 없음 > 사용자 등록 유도 (PRIMXAgentCS에서 처리)");

		}
		else {
			m_strAgentID = m_ConfigDataManager.GetAgentID();
			if (0 == m_strAgentID.GetLength()) {
				CString strErrorString;
				if (TRUE == SetSystemUserReg(strUserID, PRIMX_SYS_TYPE_USER, strErrorString)) {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"재등록 > 사용자PC 등록 성공 > %s", m_ConfigDataManager.GetAgentID());
					bRe = TRUE;
				}
				else {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"재등록 > 사용자PC 등록 실패 > %s", strErrorString);
				}
			}
			else {
				// 사용자PC, 사용자 ID 존재 => 정상
			}
		}
	}
	else if (PRIMX_SYS_TYPE_COMM == nType) {
		if (0 < strUserID.GetLength()) {
			// 공용PC 설정, 사용자 ID 존재 => 사용자 ID 삭제
			m_ConfigDataManager.SetUserIDRemove(); 
		}
		else {
			m_strAgentID = m_ConfigDataManager.GetAgentID();
			if (0 == m_strAgentID.GetLength()) {
				CString strErrorString;
				if (TRUE == SetSystemUserReg(_T(""), PRIMX_SYS_TYPE_COMM, strErrorString)) {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"재등록 > 공용PC 등록 성공 > %s", m_ConfigDataManager.GetAgentID());
					bRe = TRUE;
				}
				else {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"재등록 > 공용PC 등록 실패 > %s", strErrorString);
				}
			}
			else {
				// 공용PC => 정상
			}
		}
	}
	else if (PRIMX_SYS_TYPE_USER_SETUP == nType) { // 사용자PC로 설치됨
		if (0 == strUserID.GetLength()) { 
			// 설치 시 사용자PC 설정, 사용자ID 없음 => 사용자 등록 유도 (PRIMXAgentCS에서 처리)
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"설치 시 사용자PC 설정 > 사용자ID 없음 > 사용자 등록 유도 (PRIMXAgentCS에서 처리)");

		}
		else {
			// 설치 시 사용자PC 설정, 사용자ID 존재 => 시스템 등록 처리
			CString strErrorString;
			if (TRUE == SetSystemUserReg(strUserID, PRIMX_SYS_TYPE_USER, strErrorString)) {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"설치 > 사용자PC 등록 성공 > %s", m_ConfigDataManager.GetAgentID());
				bRe = TRUE;
			}
			else {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"설치 > 사용자PC 등록 실패 > %s", strErrorString);
			}

		}
	}
	else if (PRIMX_SYS_TYPE_COMM_SETUP == nType) { 
		if (0 < strUserID.GetLength()) {
			// 설치 시 공용PC 설정, 사용자ID 존재 => 사용자 ID 삭제
			m_ConfigDataManager.SetUserIDRemove();
		}

		// 설치 시 공용PC 설정 => 시스템 등록 처리
		CString strErrorString;
		if (TRUE == SetSystemUserReg(_T(""), PRIMX_SYS_TYPE_COMM, strErrorString)) {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"설치 > 공용PC 등록 성공 > %s", m_ConfigDataManager.GetAgentID());
			bRe = TRUE;
		}
		else {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"설치 > 공용PC 등록 실패 > %s", strErrorString);
		}	
	}
	else { 
		if (0 < strUserID.GetLength()) { 
			// 시스템 구분값 비정상, 사용자 ID 존재 => 시스템 구분값 설정 => k/A 시 재등록 또는 정상 동작
			m_ConfigDataManager.SetSystemType(PRIMX_SYS_TYPE_USER);
		}
		else {
			// 시스템 구분값 비정상, 사용자 ID 없음 => 시스템 구분값 설정 => 사용자 등록 유도 (PRIMXAgentCS에서 처리)
			m_ConfigDataManager.SetSystemType(PRIMX_SYS_TYPE_USER);
		}
	}

	return bRe;
}

BOOL CPRIMXAgentManager::SetSystemModCheck()
{
	BOOL bRe = FALSE;

	if (0 < m_strAgentID.GetLength() || 0 < m_strUserID.GetLength()) {
		__time64_t tCurrentTime = _time64(NULL);
		if (m_nSystemModCheckTime < tCurrentTime) {
			if (NULL != m_pSystemDataModifyCheck) {
				m_pSystemDataModifyCheck->SetSystemDataModifyCheck();
			}
			m_nSystemModCheckTime = _time64(NULL) + 60;
		}

	}

	return bRe;
}

BOOL CPRIMXAgentManager::SetUpdateCheck(CCommandPolling& CommandPolling)
{
	BOOL bRe = FALSE;

	if (TRUE == CommandPolling.GetAgentUpdate()) {
		int nRe = SetAgentUpdate();
		if (1 == nRe) {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Agent 업데이트 > PRIMXAgent 재시작");
			exit(0);
		}
	}
	else 
	{
		DWORD dwExitCodeResult = 0;

		if (TRUE == CommandPolling.GetZonecentralUpdate()) {
			if (FALSE == m_bInstallFailureCode[0]) {
				DWORD dwExitCode = 0;
				bRe = SetProductUpdate(_T("ZONECENTRAL"), dwExitCode);
				if (ERROR_INSTALL_FAILURE == dwExitCode) {
					dwExitCodeResult = dwExitCodeResult | 0x01;
					m_bInstallFailureCode[0] = TRUE;
				}
				else if (ERROR_SUCCESS_REBOOT_REQUIRED == dwExitCode) {
					dwExitCodeResult = dwExitCodeResult | 0x02;
				}
			}
			else {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX ZONECENTRAL 업데이트 > 설지 전 재시작 필요하여 업데이트 진행 안함.");
			}
		}
		if (TRUE == CommandPolling.GetOrizonUpdate()) {
			if (FALSE == m_bInstallFailureCode[1]) {
				DWORD dwExitCode = 0;
				bRe = SetProductUpdate(_T("ORIZON"), dwExitCode);
				if (ERROR_INSTALL_FAILURE == dwExitCode) {
					dwExitCodeResult = dwExitCodeResult | 0x01;
					m_bInstallFailureCode[1] = TRUE;
				}
				else if (ERROR_SUCCESS_REBOOT_REQUIRED == dwExitCode) {
					dwExitCodeResult = dwExitCodeResult | 0x02;
				}
			}
			else {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX ORIZON 업데이트 > 설지 전 재시작 필요하여 업데이트 진행 안함.");
			}
		}
		if (TRUE == CommandPolling.GetCryhodUpdate()) {
			if (FALSE == m_bInstallFailureCode[2]) {
				DWORD dwExitCode = 0;
				bRe = SetProductUpdate(_T("CRYHOD"), dwExitCode);
				if (ERROR_INSTALL_FAILURE == dwExitCode) {
					dwExitCodeResult = dwExitCodeResult | 0x01;
					m_bInstallFailureCode[2] = TRUE;
				}
				else if (ERROR_SUCCESS_REBOOT_REQUIRED == dwExitCode) {
					dwExitCodeResult = dwExitCodeResult | 0x02;
				}
			}
			else {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX CRYHOD 업데이트 > 설지 전 재시작 필요하여 업데이트 진행 안함.");
			}
		}
		if (0 < dwExitCodeResult) {		
			SendAlertMsg(dwExitCodeResult);
		}
	}

	return bRe;
}

void CPRIMXAgentManager::SendAlertMsg(DWORD dwExitCodeResult)
{
	CProcessRun ProcessRun;
	CString strAgentCSPath;
	strAgentCSPath.Format(_T("%s\\%s -AlertCode %lu"), ProcessRun.GetLocalPath(), PRIMX_AGNET_MSG_NAME, dwExitCodeResult);

	if (TRUE == ProcessRun.CreateProcessOnUserAccountEx(strAgentCSPath)) {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s execution complete.(AlertMsg)", PRIMX_AGNET_MSG_NAME);
	}
	else {
		if (0 != GetLastError()) {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s execution fail(AlertMsg) : %s", PRIMX_AGNET_MSG_NAME, ProcessRun.GetLastErrorString());
		}
	}
}

BOOL CPRIMXAgentManager::SetDecompression(CString strCompressionFilePath, CString strDecompressionPath, CStringList& UnZipFileList)
{
	BOOL bRe = FALSE;

	CZIPFile ZipControl;
	bRe = ZipControl.DoUnzipFile(strCompressionFilePath, strDecompressionPath, UnZipFileList);

	return bRe;
}

BOOL CPRIMXAgentManager::SetBackupAgentFile(CString strBackupPath,CStringList& FileList)
{
	BOOL bRe = TRUE;

	CString strLocalPath = GetLocalPath();
	SetDeleteFileWithSubDir(strBackupPath);

	POSITION Pos = FileList.GetHeadPosition();
	while (NULL != Pos) {
		CString strFileName = FileList.GetNext(Pos);
		strFileName.Trim();
		if (0 < strFileName.GetLength()) {
			if (FALSE == SetFileCopy(strLocalPath, strBackupPath, strFileName)) {
				SetDeleteFileWithSubDir(strBackupPath);
				bRe = FALSE;
				break;
			}
		}
	}

	return bRe;
}

BOOL CPRIMXAgentManager::SetRestoreAgentFile(CStringList& FileList)
{
	BOOL bRe = TRUE;

	CString strLocalPath = GetLocalPath();
	CString strBackupPath;
	strBackupPath.Format(_T("%s\\BackupFiles"), strLocalPath);
	
	POSITION Pos = FileList.GetHeadPosition();
	while (NULL != Pos) {
		CString strFileName = FileList.GetNext(Pos);
		strFileName.Trim();
		if (0 < strFileName.GetLength()) {
			CString strFilePath;
			strFilePath.Format(_T("%s\\%s"), strLocalPath, strFileName);
			if (TRUE == FileExist(strFilePath)) {
				CString strRename;
				strRename.Format(_T("%s.%lu"), LPCTSTR(strFilePath), UINT(_time32(NULL)));
				_trename(strFilePath, strRename);
				MoveFileEx(strRename, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
			}
			if (FALSE == SetFileCopy(strBackupPath, strLocalPath, strFileName)) {
				bRe = FALSE;
				break;
			}
		}
	}

	return bRe;
}

BOOL CPRIMXAgentManager::SetUpdateProductFile(CString strProductType, CString strUpdateFilePath, DWORD& dwExitCode)
{
	BOOL bRe = FALSE;

	CString strInstallLogPath;
	strInstallLogPath.Format(_T("%s\\Log\\PRIMXSetuplog_%s_%s.txt"), GetProgramDataPath(), strProductType,GetTimeStringForFileName());

	CString strInstallCommand;
	strInstallCommand.Format(_T("msiexec.exe /i \"%s\" /quiet /norestart /l*vx \"%s\""), strUpdateFilePath, strInstallLogPath);

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"설치 실행 > %s", strInstallCommand);

	dwExitCode = 0;
	bRe = SetCreateProcesseEx(strInstallCommand, dwExitCode);

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"설치 실행(SetCreateProcesseEx) > %s > exit code [%lu]", strInstallCommand, dwExitCode);

	//if (1603 == dwExitCode) {
	//	Sleep(5000);
	//	CString strInstallLogPathEx;
	//	strInstallLogPathEx.Format(_T("%s\\Log\\PRIMXSetuplog_%s_%s.txt"), GetProgramDataPath(), strProductType, GetTimeStringForFileName());

	//	CString strInstallCommandEx;
	//	strInstallCommandEx.Format(_T("msiexec.exe /i \"%s\" /quiet /norestart /l*vx \"%s\""), strUpdateFilePath, strInstallLogPathEx);

	//	CProcessRun ProcessRun;
	//	bRe = ProcessRun.SetCreateProcesseOnAdmin(strInstallCommandEx, dwExitCode);
	//	if (TRUE == bRe) {
	//  dwExitCode = 0;
	//  theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"설치 실행(CreateProcessOnAdminAccountEx) OK");
	//	}else{
	//  theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"설치 실행(CreateProcessOnAdminAccountEx) > %s > exit code [%lu]", strInstallCommandEx, dwExitCode);
	//	}
	//}

	return bRe;
}

BOOL CPRIMXAgentManager::SetUpdateProductFile(CString strProductType, CString strUpdatePath, CStringList& FileList, DWORD& dwExitCode)
{
	BOOL bRe = FALSE;
	
	POSITION Pos = FileList.GetHeadPosition();
	while (NULL != Pos) {
		CString strFileName = FileList.GetNext(Pos);
		strFileName.Trim();
		if (0 < strFileName.GetLength()) {
			if (0 == GetFileExtension(strFileName).CompareNoCase(_T("msi"))){
				CString strInstallMSIPath;
				CString strInstallLogPath;
				strInstallMSIPath.Format(_T("%s\\%s"), strUpdatePath, strFileName);
				strInstallLogPath.Format(_T("%s\\Log\\PRIMXSetuplog_%s_%s.txt"), GetProgramDataPath(), strProductType, GetTimeStringForFileName());

				CString strInstallCommand;
				strInstallCommand.Format(_T("msiexec.exe /i \"%s\" /quiet /norestart /l*vx \"%s\""), strInstallMSIPath, strInstallLogPath);

				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"설치 실행 > %s", strInstallCommand);

				dwExitCode = 0;
				bRe = SetCreateProcesseEx(strInstallCommand, dwExitCode);

				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"설치 실행 > %s > exit code [%lu]", strInstallCommand, dwExitCode);

				break;
			}
		}
	}	

	return bRe;
}

BOOL CPRIMXAgentManager::SetUpdateAgentFile(CString strUpdatePath, CStringList& FileList)
{
	BOOL bRe = TRUE;

	CString strLocalPath = GetLocalPath();

	POSITION Pos = FileList.GetHeadPosition();
	while (NULL != Pos) {
		CString strFileName = FileList.GetNext(Pos);
		strFileName.Trim();
		if (0 < strFileName.GetLength()) {
			CString strDestFilePath;
			CString strSourceFilePath;
			strSourceFilePath.Format(_T("%s\\%s"), strUpdatePath, strFileName);
			strDestFilePath.Format(_T("%s\\%s"), strLocalPath, strFileName);

			if (TRUE == FileExist(strSourceFilePath)) {
				if (FALSE == SetFileCopy(strSourceFilePath, strDestFilePath)) {
					bRe = FALSE;
					break;
				}
			}
		}
	}

	return bRe;
}

BOOL CPRIMXAgentManager::SetAgentExeRestart(CStringList& FileList)
{
	CProcessRun ProcessRun;

	POSITION Pos = FileList.GetHeadPosition();
	while (NULL != Pos) {
		CString strFileName = FileList.GetNext(Pos);
		strFileName.Trim();
		if (0 < strFileName.GetLength()) {
			if (0 != strFileName.CompareNoCase(_T("PRIMXAgentCS.exe")) && 0 != strFileName.CompareNoCase(_T("PRIMXAgent.exe"))) {
				CString strExtension = GetFileExtension(strFileName);
				if (0 == strExtension.CompareNoCase(_T("exe"))) {
					ProcessRun.SetKillProcess(strFileName);
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Agent 업데이트 > 프로세스 종료 > %s", strFileName);
				}
			}
		}
	}
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Agent 업데이트 > 프로세스 종료 > PRIMXAgentCS.exe");

	ProcessRun.SetKillProcess(_T("PRIMXAgentCS.exe"));
//	ProcessRun.SetKillProcess(_T("PRIMXAgent.exe"));

	return TRUE;
}

BOOL CPRIMXAgentManager::SetProductUpdate(CString strProductType, DWORD& dwExitCode)
{
	BOOL bRe = FALSE;

	CString strProductVersion;
	CSystemDataManager SystemDataManager;
	PRIMX_PRODUCT_DATA ZonecentrolData;
	PRIMX_PRODUCT_DATA OrizoneData;
	PRIMX_PRODUCT_DATA CryhodData;
	SystemDataManager.GetPRIMXProductInfo(ZonecentrolData, OrizoneData, CryhodData);
	
	dwExitCode = 0;

	if (0 == strProductType.CompareNoCase(_T("ZONECENTRAL"))) {
		strProductVersion = ZonecentrolData.strDisplayVersionEx;
	}
	else if(0 == strProductType.CompareNoCase(_T("ORIZON"))) {
		strProductVersion = OrizoneData.strDisplayVersionEx;
	}
	else if (0 == strProductType.CompareNoCase(_T("CRYHOD"))) {
		strProductVersion = CryhodData.strDisplayVersionEx;
	}
	else {
		return bRe;
	}
	
	if (0 == strProductVersion.GetLength()) {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 신규 설치 시작",strProductType);
	}
	else {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 업데이트 시작 > 현재버전[v%s]", strProductType, strProductVersion);
	}

	POSITION Pos = m_ServerIPList.GetHeadPosition();
	while (NULL != Pos) {
		CString strServerIP = m_ServerIPList.GetNext(Pos);

		CCommandUpdateFile CommandUpdateFile;
		if (TRUE == CommandUpdateFile.SetConnect(strServerIP, m_nUpdatePort)) {
			CString strUpdatePath;
			CString strUpdateFile;
			CString strUpdateTempName = GetCreateKey();

			strUpdatePath.Format(_T("%s\\UpdateFiles"), GetLocalPath());
			SetCreateDirectoryEx(strUpdatePath);

			strUpdateFile.Format(_T("%s\\%s.msi"), strUpdatePath, strUpdateTempName);

			if (TRUE == CommandUpdateFile.GetUpdateFile(strProductType, strUpdateFile)) {
				if (TRUE == FileExist(strUpdateFile)) {

					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 업데이트 > 업데이트 파일 다운로드 성공 > %s", strProductType, strUpdateFile);
					CString strHasData;
					CSHA512 SHAChecksum512;
					if (TRUE == SHAChecksum512.GetShaDataCheck(strUpdateFile, CommandUpdateFile.GetUpdateFileHaseData(), strHasData)) {

						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 업데이트 > 업데이트 파일 무결성 일치 > %s", strProductType, strUpdateFile);

						if (TRUE == IsZipFile(strUpdateFile)) {
							theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 업데이트 > 업데이트 파일이 ZIP 압축됨 > %s", strProductType, strUpdateFile);

							CString strDecompressionPath;
							strDecompressionPath.Format(_T("%s\\%s"), strUpdatePath, strUpdateTempName);
							SetCreateDirectoryEx(strDecompressionPath);

							CStringList UnZipFileList;
							if (TRUE == SetDecompression(strUpdateFile, strDecompressionPath, UnZipFileList)) {
								if (TRUE == SetUpdateProductFile(strProductType, strDecompressionPath, UnZipFileList, dwExitCode)) {
									SystemDataManager.GetPRIMXProductInfo(ZonecentrolData, OrizoneData, CryhodData);

									if (0 == strProductType.CompareNoCase(_T("ZONECENTRAL"))) {
										strProductVersion = ZonecentrolData.strDisplayVersionEx;
									}
									else if (0 == strProductType.CompareNoCase(_T("ORIZON"))) {
										strProductVersion = OrizoneData.strDisplayVersionEx;
									}
									else if (0 == strProductType.CompareNoCase(_T("CRYHOD"))) {
										strProductVersion = CryhodData.strDisplayVersionEx;
									}

									if (0 < strProductVersion.GetLength()) {
										theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 업데이트 > 업데이트 성공 > v%s", strProductType, strProductVersion);
										bRe = TRUE;
									}
									else {
										theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 업데이트 > 업데이트 실패 > 버전 정보 없음", strProductType);
									}
								}
							}
							else {
								theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 업데이트 > 업데이트 파일 > 압축해제 실패 > %s", strProductType, strUpdateFile);
							}
							SetFileRemoveRecurseSubdirectories(strDecompressionPath);
						}
						else {
							if (TRUE == SetUpdateProductFile(strProductType, strUpdateFile, dwExitCode)) {
								SystemDataManager.GetPRIMXProductInfo(ZonecentrolData, OrizoneData, CryhodData);

								if (0 == strProductType.CompareNoCase(_T("ZONECENTRAL"))) {
									strProductVersion = ZonecentrolData.strDisplayVersionEx;
								}
								else if (0 == strProductType.CompareNoCase(_T("ORIZON"))) {
									strProductVersion = OrizoneData.strDisplayVersionEx;
								}
								else if (0 == strProductType.CompareNoCase(_T("CRYHOD"))) {
									strProductVersion = CryhodData.strDisplayVersionEx;
								}
								if (0 < strProductVersion.GetLength()) {
									theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 업데이트 > 업데이트 성공 > v%s", strProductType, strProductVersion);
									bRe = TRUE;
								}
								else {
									theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 업데이트 > 업데이트 실패 > 버전 정보 없음", strProductType);
								}
							}
						}
					}
					else {
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 업데이트 > 업데이트 파일 무결성 불일치 > 파일[%s] > 설정[%s]", strProductType, strHasData, CommandUpdateFile.GetUpdateFileHaseData());
					}
				}
				else {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 업데이트 > 다운로드한 파일 존재하지 않음 > %s", strProductType, strUpdateFile);
				}
			}
			else {
				SetDeleteFile(strUpdateFile);
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 업데이트 > 업데이트 파일 다운로드 실패", strProductType);
			}
			SetFileRemoveRecurseSubdirectories(strUpdatePath);
			break;
		}
		else {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Product [%s] 업데이트 시작 > 업데이트 서버 연결 실패 > %s:%lu", strProductType, strServerIP, m_nUpdatePort);
		}

		Sleep(500);
	}

	return bRe;
}

int CPRIMXAgentManager::SetAgentUpdate()
{
	int nRe = -1;

	CConfigDataManager ConfigDataManager;
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Agent 업데이트 시작 > 현재버전[v%s]", ConfigDataManager.GetAgentVersion());



	POSITION Pos = m_ServerIPList.GetHeadPosition();
	while (NULL != Pos) {
		CString strServerIP = m_ServerIPList.GetNext(Pos);
		CCommandUpdateFile CommandUpdateFile;
		if (TRUE == CommandUpdateFile.SetConnect(strServerIP, m_nUpdatePort)) {
			CString strUpdatePath;
			CString strUpdateFile;
			CString strDecompressionPath;
			CString strBackupPath;
			CString strUpdateTempName = GetCreateKey();

			strBackupPath.Format(_T("%s\\BackupFiles"), GetLocalPath());
			strUpdatePath.Format(_T("%s\\UpdateFiles"), GetLocalPath());
			strDecompressionPath.Format(_T("%s\\%s"), strUpdatePath, strUpdateTempName);
			SetCreateDirectoryEx(strBackupPath);
			SetCreateDirectoryEx(strUpdatePath);
			SetCreateDirectoryEx(strDecompressionPath);

			strUpdateFile.Format(_T("%s\\%s.zip"), strUpdatePath, strUpdateTempName);

			if (TRUE == CommandUpdateFile.GetUpdateFile(_T("Agent"), strUpdateFile)) {
				if (TRUE == FileExist(strUpdateFile)) {

					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Agent 업데이트 > 업데이트 파일 다운로드 성공 > %s", strUpdateFile);

					CString strHasData;
					CSHA512 SHAChecksum512;
					if (TRUE == SHAChecksum512.GetShaDataCheck(strUpdateFile, CommandUpdateFile.GetUpdateFileHaseData(), strHasData)) {

						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Agent 업데이트 > 업데이트 파일 무결성 일치 > %s", strUpdateFile);

						CStringList UnZipFileList;
						if (TRUE == SetDecompression(strUpdateFile, strDecompressionPath, UnZipFileList)) {

							SetBackupAgentFile(strBackupPath, UnZipFileList);

							if (TRUE == SetUpdateAgentFile(strDecompressionPath, UnZipFileList)) {
								CConfigDataManager ConfigDataManager;
								ConfigDataManager.SetAgentVersion(CommandUpdateFile.GetUpdateVersion());

								theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Agent 업데이트 > 업데이트 성공 > v%s", ConfigDataManager.GetAgentVersion());

								SetAgentExeRestart(UnZipFileList);
								nRe = 1;
							}
							else {
								theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Agent 업데이트 > 업데이트 실패 > 파일 복구");
								SetRestoreAgentFile(UnZipFileList);
							}
						}
						else {
							theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Agent 업데이트 > 업데이트 파일 > 압축해제 실패 > %s", strUpdateFile);
						}
					}
					else {
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Agent 업데이트 > 업데이트 파일 무결성 불일치 > 파일[%s] > 설정[%s]", strHasData, CommandUpdateFile.GetUpdateFileHaseData());
					}
				}
				else {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Agent 업데이트 > 다운로드한 파일 존재하지 않음 > %s", strUpdateFile);
				}
			}
			else {
				SetDeleteFile(strUpdateFile);
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Agent 업데이트 > 업데이트 파일 다운로드 실패");
			}

			SetFileRemoveRecurseSubdirectories(strDecompressionPath);
			SetFileRemoveRecurseSubdirectories(strUpdatePath);
			SetFileRemoveRecurseSubdirectories(strBackupPath);
			break;
		}
		else {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"PRIMX Agent 업데이트 시작 > 업데이트 서버 연결 실패 > %s:%lu", strServerIP, m_nUpdatePort);
		}
		Sleep(500);
	}

	return nRe;
}

BOOL CPRIMXAgentManager::IsZipFile(CString strUpdateFile)
{
	BOOL bRe = FALSE;
	
	HANDLE hFile = CreateFile((LPCTSTR)strUpdateFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile){
		return bRe;
	}

	HANDLE hMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
	if (NULL == hMap) return bRe;

	void *pBase = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	if (NULL == pBase) return bRe;	

	DWORD* pZipSignature = (DWORD*)pBase;
	if (0x04034b50 == *pZipSignature) {
		bRe = TRUE;
	}
	UnmapViewOfFile(pBase);
	CloseHandle(hMap);
	CloseHandle(hFile);
	return bRe;
}

void CPRIMXAgentManager::SetPRIMXAgentUninstall()
{
	CString strCommand;
	DWORD dwExitCode = 0;
	CString strLocalPath = GetLocalPath();
	
	POSITION Pos = m_ServerIPList.GetHeadPosition();
	while (NULL != Pos) {
		CString strServerIP = m_ServerIPList.GetNext(Pos);
		CCommandPolling CommandPolling;
		if (TRUE == CommandPolling.SetConnect(strServerIP, m_nServerPort)) {
			CommandPolling.SetUninstall();
			break;
		}
		Sleep(500);
	}

	strCommand.Format(_T("\"%s\\PRIMXUninstall.exe\" Rimahr9280!"), GetLocalPath());
	SetCreateProcesseEx(strCommand, dwExitCode,1);

}

BOOL CPRIMXAgentManager::SetShutdown()
{

	POSITION Pos = m_ServerIPList.GetHeadPosition();
	while (NULL != Pos) {
		CString strServerIP = m_ServerIPList.GetNext(Pos);
		CCommandPolling CommandPolling;
		if (TRUE == CommandPolling.SetConnect(strServerIP, m_nServerPort)) {
			CommandPolling.SetShutdown();
			break;
		}
		Sleep(500);
	}
	return TRUE;
}
