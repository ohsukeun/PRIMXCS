#pragma once
#include <map>
#include "CppSQLite3U.h"


class CSQLiteManager 
{
public:
	CSQLiteManager(CString strDBName=_T(""));
	~CSQLiteManager(void);

public:

	void SetTableCheckForCreate();

	// 에러 메시지를 리턴하는 함수.
	CString GetErrorString();	
	
	// File DB를 Open하는 함수.
	BOOL OpenDB(void);

	// File DB를 닫는 함수.
	void CloseDB(void);

	// Result가 없는 쿼리를 실행하는 함수.
	BOOL SetExcuteNoResultset(CString strQuery, BOOL bOpenDB=TRUE);

	// Result가 있는 쿼리를 실행하는 함수.
	CppSQLite3Query* SetExcute(CString strQuery, BOOL bOpenDB = TRUE);
	
private:
	// DB를 생성하고 테이블을 생성하는 함수.
	BOOL CreateTable(CString strTable, CString strQuery);

	// 테이블 존재 여부를 확인하는 함수.
	BOOL IsExistTable(CString strTableName);

	CString GetLocalDBSavePath();
	

	CString				m_strFileDBName;
	CString				m_strErrorString;		//  에러 스트링 
	CppSQLite3DB*		m_pSQLiteDB;			//  File DB 객체 			
	CppSQLite3Query		m_sqlite3Query;			//  쿼리 Result 객체 
	CString				m_strKey;
};

