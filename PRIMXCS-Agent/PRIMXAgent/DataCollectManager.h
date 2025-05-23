#pragma once
#include "ThreadEx.h"
#include "FileDBManager.h"
#include "CommonModule.h"


#define PRIMX_LOG_COLLECT_CYCLE				20
#define PRIMX_PRODUCT_COLLECT_CYCLE			60
#define PRIMX_LOG_SEND_CYCLE				60

#define EVENT_LOG_BUFFER_SIZE				40960


class CDataCollectManager : public CThreadEx , public CCommonModule
{
public:
	CDataCollectManager();
	~CDataCollectManager();


	BOOL SetStart();
	void SetStop();

	virtual void Run();


protected:
	
	void GetProductData();
	void GetEventData();
	void GetEventData(CString strSourceName);



	void GetLogFileData();
	void GetLogFileData(CString strCollectDirectory,UINT nProductType);
	void SetLogDataTransfer();
	CString GetPRIMXProductLogPath(UINT nProductType);

	void GetLogDataForDB(CList<PRIMX_LOG_DATA*>& LogDataList);
	void SetLogDataForDB(CList<PRIMX_LOG_DATA*>& LogDataList);


	BOOL SetLogDateParser(CString strFileName, CString& strLogDate);
	BOOL LoadLogFile(CString strFilePath, CString& strLogData);
	BOOL SetLogDataDBSave(CFileDBManager& FileDBManager, UINT nProductType,CString strFilePath, CString& strLogDate, CString& strLogFileFullData);
	BOOL SetLevelDataCheck(CString strData, PRIMX_LOG_DATA& DBSaveData);
	BOOL SetLogDataCheck(PRIMX_LOG_DATA& DBSaveData);
	BOOL SetLogDataCheck(PRIMX_EVENT_DATA_TABLE EventTable[],UINT nEventTableSize, PRIMX_LOG_DATA& DBSaveData);


	LPTSTR GetEventMessage(HMODULE hDll,DWORD dwEventIndex,DWORD dwLanguageID,LPTSTR* lpInserts); 
	BOOL ReadEventSourceInfo(CString strSourceName, LPCTSTR lpszESName, LPTSTR lpszEvent);
	LPTSTR GetEventString(CString strSourceName,EVENTLOGRECORD *pRecord, LPTSTR source);
	CString GetEventTime(time_t EventDate);
	BOOL EventLogDataCheck(CFileDBManager& FileDBManager, UINT nEventType, UINT nEventID, time_t tEventDate, CString strServiceName, CString strEventDesc);
	UINT SetEventSourceNameCheck(CString strSourceName);
	CString GetEventCodeConvert(UINT nProductType, UINT nEventID);


	BOOL			m_bRun;
	__time64_t		m_nCollectTime[3];

	CString			m_strZoneCentrolLogPath;
	CString			m_strOrizonLogPath;
	CString			m_strCryhodLogPath;
};

