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

	// ���� �޽����� �����ϴ� �Լ�.
	CString GetErrorString();	
	
	// File DB�� Open�ϴ� �Լ�.
	BOOL OpenDB(void);

	// File DB�� �ݴ� �Լ�.
	void CloseDB(void);

	// Result�� ���� ������ �����ϴ� �Լ�.
	BOOL SetExcuteNoResultset(CString strQuery, BOOL bOpenDB=TRUE);

	// Result�� �ִ� ������ �����ϴ� �Լ�.
	CppSQLite3Query* SetExcute(CString strQuery, BOOL bOpenDB = TRUE);
	
private:
	// DB�� �����ϰ� ���̺��� �����ϴ� �Լ�.
	BOOL CreateTable(CString strTable, CString strQuery);

	// ���̺� ���� ���θ� Ȯ���ϴ� �Լ�.
	BOOL IsExistTable(CString strTableName);

	CString GetLocalDBSavePath();
	

	CString				m_strFileDBName;
	CString				m_strErrorString;		//  ���� ��Ʈ�� 
	CppSQLite3DB*		m_pSQLiteDB;			//  File DB ��ü 			
	CppSQLite3Query		m_sqlite3Query;			//  ���� Result ��ü 
	CString				m_strKey;
};

