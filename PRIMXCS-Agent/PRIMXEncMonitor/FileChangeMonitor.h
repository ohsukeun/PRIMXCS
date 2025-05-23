#pragma once
#include "CommonModule.h"
#include "CPrimxzc.h"
#include "ThreadEx.h"


typedef struct  _PRIMX_CONTEXT_
{
	OVERLAPPED			OverLapped;
	HANDLE				hDirectory;	
	CString				strDirectory;

	LPVOID              lpBuffer;
	DWORD               nBufferLength;
	BOOL                bWatchSubtree;
	DWORD               dwNotifyFilter;
	DWORD				dwBytesReturned;


	_PRIMX_CONTEXT_() {
		hDirectory = NULL;
		nBufferLength = (16 * 1024);
		lpBuffer = new BYTE[nBufferLength];
		ZeroMemory(lpBuffer, nBufferLength);
		dwBytesReturned = 0;
		bWatchSubtree = TRUE;
	//	dwNotifyFilter = FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME;
		dwNotifyFilter = FILE_NOTIFY_CHANGE_LAST_WRITE;
		::ZeroMemory(&OverLapped, sizeof(OVERLAPPED));
	};

	~_PRIMX_CONTEXT_() {
		delete[] lpBuffer;
		lpBuffer = NULL;
	};

	void SetBufferClear() {
		dwBytesReturned = 0;
		ZeroMemory(lpBuffer, nBufferLength);
	};

} PRIMX_CONTEXT;


typedef struct  _PRIMX_MONITOR_PATH_INFO_
{
	CString				strDirectory;
	CString				strCreateDateTime;
	BOOL				bMonitorStart;
	_PRIMX_MONITOR_PATH_INFO_() {
		bMonitorStart = FALSE;
	};
} PRIMX_MONITOR_PATH_INFO;


class CFileChangeMonitor : public CThreadEx , public CCommonModule
{
public:
	CFileChangeMonitor();
	~CFileChangeMonitor();

	BOOL SetMonitorStart(UINT nThreadCount);
	void SetMonitorStop();
	

protected:
	CString GetErrToString();
	static unsigned int CALLBACK MonitorRun(LPVOID pParams);
	void MonitorRun();

	PRIMX_CONTEXT* CreateContext();
	PRIMX_CONTEXT* UpdateCompletionPort(CString strDirectory);
	BOOL ReadChangeData(PRIMX_CONTEXT* pContextData);
	UINT GetClipboardDataSearch(CString strSearchFileName, CStringList& SearchDataList);
	void SetAuthNotEncryptionListCheck();
	void SetEncryptionPathCheck();
	void SetCloseAll();
	void SetRemoveContext(PRIMX_CONTEXT* pContextData);
	void SetPathMonitorStop(CString strMonitorPath);
	void SetRemoveAllContext();
	void SetRemoveAllEncryptionPathList();

	void SetRemoveFileData(CString strFilePath);
	CString GetRemoveFileData();
	CString GetDirectoryCreateDateTime(CString strDirPath);
	void SetEncryptionPathListInsert(CString strPath, BOOL bMonitorStart);
	void SetEncryptionPathListRemove(CString strPath);

	void Run();

	HANDLE				m_hCompletion;			

	UINT				m_nThreadCount;
	BOOL				m_bThreadRun;
	CList<HANDLE>		m_ThradList;

	CPrimxzc			m_Primxzc;
	CStringList			m_RemoveFileList;
	CStringList			m_AuthNotEncryptionList[2];
	UINT				m_AuthNotEncryptionIndex;
	CString				m_strAuthNotEncryptionTimeStamp;
	CRITICAL_SECTION	m_csLock;
	CRITICAL_SECTION	m_csRemoveFileLock;
	CRITICAL_SECTION	m_csEncryptionPathLock;
	BOOL				m_bRun;
	
	CList<PRIMX_CONTEXT*>				m_ContextList;

	CList<PRIMX_MONITOR_PATH_INFO*>		m_EncryptionPathList;


};

