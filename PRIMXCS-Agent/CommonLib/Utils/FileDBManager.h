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
	// DB�� �����ϰ� ���̺��� �����ϴ� �Լ�.
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



	// ���� �޽����� �����ϴ� �Լ�.
	CString GetErrorString();
	

	// File DB�� Open�ϴ� �Լ�.
	BOOL OpenLogDB(void);
	// File DB�� Open�ϴ� �Լ�.
	BOOL OpenDB(void);

	// Export File DB�� Open�ϴ� �Լ�.
	BOOL OpenExportDB(void);


	// File DB�� �ݴ� �Լ�.
	void CloseDB(void);
	
protected:
	CString GetDateTimeGMT0(COleDateTime& DateTime);
	void SetConvertData(CString& strData);
	CString SetConvertDataEx(CString strData);
	
	BOOL SetPRIMXProductDataDBSave(CString strAgentID, UINT nProductType, PRIMX_PRODUCT_DATA& ProductData);
	BOOL GetPRIMXProductData(CString strAgentID, UINT nProductType, PRIMX_PRODUCT_DATA& ProductData);



	// ���̺� ���� ���θ� Ȯ���ϴ� �Լ�.
	BOOL IsExistTable(CString strTableName);

	// Result�� ���� ������ �����ϴ� �Լ�.
	BOOL SetExcuteNoResultset(CString strQuery);

	// Result�� �ִ� ������ �����ϴ� �Լ�.
	CppSQLite3Query* SetExcute(CString strQuery);
	

	CString				m_strKey;
	CString				m_strFileDBName;
	CString				m_strErrorString;		//  ���� ��Ʈ�� 
	CString				m_strDBHandlingError;	//  DB ���� ��Ʈ��
	std::unique_ptr<CppSQLite3DB> m_pSQLiteDB;	//  File DB ��ü 			
	CppSQLite3Query		m_sqlite3Query;			//  ���� Result ��ü 

};

