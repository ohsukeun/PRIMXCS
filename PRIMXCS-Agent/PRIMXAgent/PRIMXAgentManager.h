#pragma once
#include "CommonModule.h"
#include "ThreadEx.h"
#include "ConfigDataManager.h"
#include "CommandPolling.h"


class CIPCChannel;
class CDataCollectManager;
class CSystemDataModifyCheck;
class CPRIMXAgentManager : public CCommonModule, public CThreadEx
{
public:
	CPRIMXAgentManager();
	~CPRIMXAgentManager();

	BOOL SetCreate();
	void SetClose();
	void Run();

	BOOL SetShutdown();
	BOOL SetSystemUserReg(CString strUserID, UINT nSystemType, CString& strErrorString);

private:
	BOOL GetConfigData();
	UINT32_T GetPollingInterval();
	void SetPollingInterval(UINT32_T nPollingInterval);

	void SetLogFileDeleteCheck();
	BOOL IPCServerToAgentCSStart();
	CString GetFailStringConvert(CString strFailCode, CString strFailString);

	BOOL SetPolling(BOOL bNow);
	BOOL SetSystemRegCheck();
	BOOL SetSystemModCheck();
	BOOL SetUpdateCheck(CCommandPolling& CommandPolling);

	//  서비스 시작 지연 발생으로 서비스 시작 타임아웃을 90초로 조정하는 함수.
	void SetServicesPipeTimeout(DWORD dwTimeoutSec);
	DWORD GetServicesPipeTimeout();
	void AgentCSRecvData(const char* pData, UINT nDataLen);

	int SetAgentUpdate();
	BOOL SetProductUpdate(CString strProductType, DWORD& dwExitCode);

	BOOL SetDecompression(CString strCompressionFilePath, CString strDecompressionPath, CStringList& UnZipFileList);
	BOOL SetBackupAgentFile(CString strBackupPath,CStringList& FileList);
	BOOL SetRestoreAgentFile(CStringList& FileList);
	BOOL SetUpdateAgentFile(CString strUpdatePath, CStringList& FileList);
	BOOL SetAgentExeRestart(CStringList& FileList);
	BOOL SetUpdateProductFile(CString strProductType,CString strUpdatePath, CStringList& FileList, DWORD& dwExitCode);
	BOOL SetUpdateProductFile(CString strProductType, CString strUpdateFilePath, DWORD& dwExitCode);
	BOOL IsZipFile(CString strUpdateFile);

	void SetEnvironmentVariableForPRIMXAgent();

	void SetPRIMXAgentUninstall();

	static VOID WINAPI IPCNotification(const char* pData, UINT nDataLen, LPVOID lpObject);
	void SendAlertMsg(DWORD dwExitCodeResult);

	BOOL				m_bRun;
	UINT32_T			m_nPollingInterval;
	UINT32_T			m_nDebugPollingInterval;
	__time64_t			m_nPollingTime;
	__time64_t			m_nSystemModCheckTime;
	CString				m_strAgentID;
	CString				m_strUserID;
	CString				m_strWindowsUUID;
	UINT				m_nSystemType;
	CRITICAL_SECTION	m_csLock;
	CString				m_strAgentVersion;
	int					m_nLogClearHour;
	
	CStringList			m_ServerIPList;
	unsigned short		m_nServerPort;
	unsigned short		m_nUpdatePort;
	unsigned short		m_nAuthPort;

	BOOL				m_bInstallFailureCode[3];

	BOOL					m_bIPSServerStart;
	CIPCChannel*			m_pIPCServer;
	CConfigDataManager		m_ConfigDataManager;
	CDataCollectManager*	m_pDataCollectManager;
	CSystemDataModifyCheck*	m_pSystemDataModifyCheck;

};

