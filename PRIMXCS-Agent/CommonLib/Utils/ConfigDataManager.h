#pragma once


#define PRIMX_REG_ROOT_PATH						_T("SOFTWARE\\Rimahr\\PRIMXAgent")	
#define PRIMX_REG_AGENTID						_T("AgentID")
#define PRIMX_REG_USER_ID						_T("UserID")
#define PRIMX_REG_SYSTEM_TYPE					_T("SystemType")
#define PRIMX_REG_LAST_DIR						_T("ExportFileLastDirectory")


#define PRIMX_REG_POLLING_INTERVAL				_T("PollingInterval")
#define PRIMX_REG_POLLING_INTERVAL_DEBUG		_T("DebugPollingInterval")
#define PRIMX_REG_AGENT_VERSION					_T("AgentVersion")
#define PRIMX_REG_SERVER_IP						_T("ServerIP")
#define PRIMX_REG_SERVER_PORT					_T("ServerPort")
#define PRIMX_REG_UPDATE_PORT					_T("UpdatePort")
#define PRIMX_REG_AUTH_PORT						_T("AuthPort")
#define PRIMX_REG_AUTH_SERVER_ADDR				_T("AuthServerIP")
#define PRIMX_REG_LAST_POLLING_TIME				_T("LastPollingTime")
#define PRIMX_REG_LOG_INTERVAL					_T("LogInterval")
#define PRIMX_REG_COMPANY_ID					_T("CompanyID")

#define PRIMX_REG_ROOT_POLICY_PATH				_T("SOFTWARE\\Rimahr\\PRIMXAgent\\PolicyData")	
#define PRIMX_REG_AUTH_WEBMANAGER				_T("AuthWebManager")			// ��û �� ���� ���� 
#define PRIMX_REG_AUTH_WEBMANAGER_URL			_T("AuthWebManagerURL")			// WebManagerConsole URL �ּ�
#define PRIMX_REG_AUTH_EXPORT_FILE				_T("AuthExportFile")			// ���� ���� ���� ���� 
#define PRIMX_REG_AUTH_EXPORT_FILE_SHELFLIFE	_T("AuthExportFileShelfLife")	// �̵������� ��ȣȭ ���� �Ⱓ (����:��)
#define PRIMX_REG_AUTH_EXPORT_FILE_SVR_TIME		_T("AuthExportFileServerTime")	// ���� �ð� GMT 0 
#define PRIMX_REG_PRODUCT_MENU					_T("ProductMenu")				// PRIMX ��ǰ�� ���� �޴� ǥ�� ���� 
#define PRIMX_REG_WEB_MANAGER_CONSOLE_MENU		_T("WebManagerConsoleMenu")		// ����� �ܼ� �޴� ǥ�� ���� 
#define PRIMX_REG_EXPOR_TFILE_MENU				_T("ExportFileMenu")			// �̵����� ���� �޴� ǥ�� ���� 
#define PRIMX_REG_AUTH_NOT_ENCRYPTION			_T("AuthNotEncryption")			// ���ȣȭ ���� ����
#define PRIMX_REG_AUTH_NOT_ENCRYPTION_STAMP		_T("AuthNotEncryptionStamp")	// ���ȣȭ ���� ����

#define PRIMX_REG_ROOT_AUTH_NOT_ENCRYPTION_PATH _T("SOFTWARE\\Rimahr\\PRIMXAgent\\PolicyData\\AuthNotEncryptionPath")	

#define PRIMX_REG_ROOT_USER_DATA_PATH			_T("SOFTWARE\\Rimahr\\PRIMXAgent\\UserData")	
#define PRIMX_REG_USER_DATA						_T("UserData")

#define PRIMX_REG_ROOT_EVENT_LOG_PATH			_T("SOFTWARE\\Rimahr\\PRIMXAgent\\EventLog")	


class CConfigDataManager
{
public:
	CConfigDataManager();
	~CConfigDataManager();

	CString GetAgentID();
	BOOL SetAgentID(CString strNewAgentID);

	UINT32_T GetPollingInterval();
	BOOL SetPollingInterval(UINT32_T nPollingInterval);

	UINT32_T GetDebugPollingInterval();

	CString GetAgentVersion();
	BOOL SetAgentVersion(CString strAgentVersion);

	int GetServerIPEx(CStringList& AddrList);
	CString GetServerAddr();
	BOOL SetServerIP(CString strServerIP);

	unsigned short GetServerPort();
	BOOL SetServerPort(unsigned short nServerPort);
	
	unsigned short GetUpdatePort();
	BOOL SetUpdatePort(unsigned short nUpdatePort);

	unsigned short GetAuthPort();
	BOOL SetAuthPort(unsigned short nAuthPort);

	BOOL SetPolicyData(PRIMX_POLICY_DATA& PolicyData);
	BOOL GetPolicyData(PRIMX_POLICY_DATA& PolicyData);

	CString GetUserID();
	BOOL SetUserID(CString strUserID);
	BOOL SetUserIDRemove();

	CString GetUserData();
	BOOL SetUserData(CString strUserData);
	BOOL SetUserDataRemove();

	UINT GetSystemType();
	BOOL SetSystemType(UINT nType);

	CString GetLastPollingTime();
	BOOL SetLastPollingTime(CString strPollingTime);
	
	BOOL SetLogCycle(UINT nLogCycle);
	UINT GetLogCycle();

	BOOL SetCompanyID(CString strCompanyID);
	CString GetCompanyID();

	BOOL SetAuthServerAddr(CString strAuthServerAddr);
	CString GetAuthServerAddr();
	
	BOOL SetEventRecordNumber(CString strEventSourceName,DWORD dwRecordNumber);
	DWORD GetEventRecordNumber(CString strEventSourceName);
	void GetAuthNotEncryptionPathList(CStringList& AuthNotEncryptionPathList);
	BOOL SetAuthNotEncryptionPathListCompare(CStringList& AuthNotEncryptionPathList, CStringList& OldAuthNotEncryptionPathList);
	CString GetTimeStamp();


	BOOL SetExportFileLastDirectory(CString strDirectory);
	CString GetExportFileLastDirectory();


private:
	UINT32_T AtoUI(LPCTSTR Data);
	int AtoI(LPCTSTR Data);
	DWORD AtoDword(LPCTSTR Data);
	unsigned short AtoUS(LPCTSTR Data);
	BOOL AtoB(CString strBoolString, BOOL bDefault = TRUE);

	CString GetRegValueStringExForEnum(HKEY hKey, CString strValueName);
	CString GetRegValueStringEx(HKEY hRootKey, CString strPath, CString strValueName);
	BOOL GetRegValueBoolEx(HKEY hRootKey, CString strPath, CString strValueName);
	DWORD GetRegValueDwordEx(HKEY hRootKey, CString strPath, CString strValueName);

	BOOL SetRegValueStringEx(HKEY hRootKey, CString strPath, CString strValueName, CString strValue);
	BOOL SetRegValueBoolEx(HKEY hRootKey, CString strPath, CString strValueName, BOOL bValue);
	BOOL SetRegValueDwordEx(HKEY hRootKey, CString strPath, CString strValueName, DWORD dwValue);
	

	CString GetRegValue(HKEY hRootKey, CString strPath, CString strValueName);
	BOOL SetRegValue(HKEY hRootKey, CString strPath, CString strValueName, CString strValue);

	BOOL SetRegValueRemove(CString strPath, CString strValueName);
};

