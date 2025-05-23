#pragma once
#include "../SQLite/CppSQLite3U.h"
#include <memory>
#include "CommonModule.h"


#define	PRIMX_DB_FILE_NAME				_T("PRIMXAgent.db")
#define	PRIMX_DB_LOG_FILE_NAME			_T("PRIMXLogData.db")
#define	PRIMX_DB_EXPORT_FILE_NAME		_T("PRIMXExportData.db")


class CFileDBManager : public CCommonModule
{
public:
	CFileDBManager();
	~CFileDBManager(void);

public:
	// DB를 생성하고 테이블을 생성하는 함수.
	BOOL SetInitDataBase();
	BOOL SetInitDataBaseForLogData();
	BOOL SetInitDataBaseForExportFile();

	BOOL SetSystemDataDBSave(CString strAgentID, CString strUserID, UINT nSystemType, PRIMX_SYSTEM_DATA& SystemData, BOOL bBeforeDataRemove);
	BOOL GetSystemData(CString strAgentID,PRIMX_SYSTEM_DATA& SystemData);

	BOOL SetPRIMXProductDataDBSave(CString strAgentID, PRIMX_PRODUCT_DATA& Zonecentrol, PRIMX_PRODUCT_DATA& Orizone, PRIMX_PRODUCT_DATA& Cryhod);
	BOOL GetPRIMXProductData(CString strAgentID, PRIMX_PRODUCT_DATA& Zonecentrol, PRIMX_PRODUCT_DATA& Orizone, PRIMX_PRODUCT_DATA& Cryhod);

	BOOL SetLogDataSave(PRIMX_LOG_DATA& DBSaveData, UINT nType);
	BOOL GetLogDataTransfer(CList<PRIMX_LOG_DATA*>& LogDataList);
	BOOL SetLogDataTransfer(CList<PRIMX_LOG_DATA*>& LogDataList);


	BOOL SetExportFileDataSave(CString strExFileKey, CString strExFileName, LONGLONG nExFileSize, CList<PRIMX_ORG_FILE_INFO*>& OrgFileList);
	BOOL GetExportFileData(CList<PRIMX_EXPORT_FILE_INFO*>& ExFileList);
	BOOL GetExportFileData(CString strKey,PRIMX_EXPORT_FILE_INFO*& pExFileData);



	// 에러 메시지를 리턴하는 함수.
	CString GetErrorString();
	

	// File DB를 Open하는 함수.
	BOOL OpenLogDB(void);
	// File DB를 Open하는 함수.
	BOOL OpenDB(void);

	// Export File DB를 Open하는 함수.
	BOOL OpenExportDB(void);


	// File DB를 닫는 함수.
	void CloseDB(void);
	
protected:
	CString GetDateTimeGMT0(COleDateTime& DateTime);
	void SetConvertData(CString& strData);
	CString SetConvertDataEx(CString strData);
	
	BOOL SetPRIMXProductDataDBSave(CString strAgentID, UINT nProductType, PRIMX_PRODUCT_DATA& ProductData);
	BOOL GetPRIMXProductData(CString strAgentID, UINT nProductType, PRIMX_PRODUCT_DATA& ProductData);



	// 테이블 존재 여부를 확인하는 함수.
	BOOL IsExistTable(CString strTableName);

	// Result가 없는 쿼리를 실행하는 함수.
	BOOL SetExcuteNoResultset(CString strQuery);

	// Result가 있는 쿼리를 실행하는 함수.
	CppSQLite3Query* SetExcute(CString strQuery);
	

	CString				m_strKey;
	CString				m_strFileDBName;
	CString				m_strErrorString;		//  에러 스트링 
	CString				m_strDBHandlingError;	//  DB 에러 스트링
	std::unique_ptr<CppSQLite3DB> m_pSQLiteDB;	//  File DB 객체 			
	CppSQLite3Query		m_sqlite3Query;			//  쿼리 Result 객체 

};

