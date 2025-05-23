// LogManager.h: interface for the CLogManager class.
//
//////////////////////////////////////////////////////////////////////
#pragma once


#define	LOG_EMERG		0	// system is unusable 
#define	LOG_ALERT		1	// action must be taken immediately 
#define	LOG_CRIT		2	// critical conditions 
#define	LOG_ERR			3	// error conditions 
#define	LOG_WAR			4	// warning conditions 
#define	LOG_NOTICE		5	// normal but significant condition 
#define	LOG_INF			6	// informational 
#define	LOG_DEBUG		7	// debug-level messages 


#define COMMAND_WINDOWS_SIZE		80
#define COMMAND_INFO_SIZE			15
#define SERVICE_NAME_LEN			256

#define TMS_WORK_DIR_LOG			_T("Log")

/// @class CLogManager
/// @brief 서버의 운영 로그를 관리하는 클래스
class CLogManager  
{
public:
	CLogManager();
	virtual ~CLogManager();

public:

	CString GetPath();
	void SetPrintLog();
	void SetDebugLogWrite();
	void LogPrintTitle(const TCHAR* pszFormat, ...);
	void LogPrintLine();
	void SetServiceName(TCHAR* Name);

	void SetPath(CString strPrefixSting ,CString Path);
	void Close();

	void LogPrint(UINT Prioryty,const TCHAR* pszFormat, ...);
	void LogPrint(UINT Prioryty,DWORD ThreadID,const TCHAR* pszFormat, ...);
	void LogWrite(TCHAR *pszFormat, ...);
	void LogEventEx(UINT Prioryty,DWORD ThreadID,const TCHAR* pszFormat, ...);
	void LogEvent(WORD wType, DWORD dwID,const TCHAR* pszS1 = NULL,	const TCHAR* pszS2 = NULL,	const TCHAR* pszS3 = NULL);
	void LogPrintRawData(const char *pData, int nLen);

	void SetLogFileDelete(int nKeepDay);

private:
	CString GetTime(BOOL bTime = TRUE);

	CRITICAL_SECTION	m_Lock;
	BOOL				m_DebugLogWrite;
	BOOL				m_PrintLog;

	DWORD				m_ThreadID;
	CString				m_Path;
	TCHAR				m_szServiceName[SERVICE_NAME_LEN];
	HANDLE				m_hEventSource;
	CString				m_strPrefixSting;

};
