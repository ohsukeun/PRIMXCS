#include "StdAfx.h"
#include "SQLiteManager.h"
#include "../Utils/ConfigDataManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSQLiteManager::CSQLiteManager(CString strDBName)
{
	m_pSQLiteDB = new CppSQLite3DB;

	if (strDBName.IsEmpty())
		strDBName = _T("PRIMXAgent.db");

	m_strFileDBName = strDBName;
	CConfigDataManager ConfigDataManager;
	CString strKey = ConfigDataManager.GetAgentID();
	strKey.Trim();
	m_strKey.Format(_T("pragma key='%s'"), strKey);

}


CSQLiteManager::~CSQLiteManager(void)
{
	m_sqlite3Query.finalize();

	if (m_pSQLiteDB) 
		delete m_pSQLiteDB;
}

CString CSQLiteManager::GetLocalDBSavePath()
{
	CString strMapFileCreatePath;

	TCHAR szPath[MAX_PATH] = { 0 };
	if (TRUE == SHGetSpecialFolderPath(NULL, szPath, CSIDL_COMMON_APPDATA, 0)) {
		strMapFileCreatePath.Format(PRIMX_INSTALL_PATH_FORMAT, szPath);
	}
	else {
		if (0 < ::ExpandEnvironmentStrings(_T("%ProgramData%"), szPath, MAX_PATH)) {
			strMapFileCreatePath.Format(PRIMX_INSTALL_PATH_FORMAT, szPath);
		}
		else {
			if (0 != GetWindowsDirectory(szPath, MAX_PATH)) {
				TCHAR pDrive[MAX_PATH] = { 0 };
				TCHAR pDir[MAX_PATH] = { 0 };
				TCHAR pFileName[MAX_PATH] = { 0 };
				TCHAR pFileExt[MAX_PATH] = { 0 };
				_tsplitpath_s(szPath, pDrive, MAX_PATH, pDir, MAX_PATH, pFileName, MAX_PATH, pFileExt, MAX_PATH);
				strMapFileCreatePath.Format(PRIMX_INSTALL_PROGRAMDATA_PATH_FORMAT, pDrive);
			}
			else {
				strMapFileCreatePath.Format(PRIMX_INSTALL_PROGRAMDATA_PATH_FORMAT, _T("c:"));
			}
		}
	}

	return strMapFileCreatePath;
}

BOOL CSQLiteManager::CreateTable(CString strTable, CString strQuery)
{
	BOOL bRe = FALSE;
	
	if(NULL == m_pSQLiteDB) return bRe;
	if (strTable.IsEmpty() || strQuery.IsEmpty()) return bRe;

	try{
		CString strDBFilePath;
		strDBFilePath.Format(_T("%s\\%s"), GetLocalDBSavePath(), m_strFileDBName);
		m_pSQLiteDB->open(strDBFilePath);
		m_pSQLiteDB->execDML(m_strKey);

		if(FALSE == IsExistTable(strTable))
			m_pSQLiteDB->execQuery(strQuery);
		
		bRe = TRUE;
	}
	catch(CppSQLite3Exception& e){
		m_strErrorString = e.errorMessage();
	}

	m_pSQLiteDB->close();
	return bRe;
}

CString CSQLiteManager::GetErrorString()
{
	return m_strErrorString;
}


BOOL CSQLiteManager::OpenDB(void)
{
	BOOL bRe = TRUE;
	
	if(NULL == m_pSQLiteDB) return bRe;
	
	try{
		if (NULL == m_pSQLiteDB->mpDB)
		{
			CString strDBFilePath;
			strDBFilePath.Format(_T("%s\\%s"), GetLocalPath(), m_strFileDBName);
			m_pSQLiteDB->open(strDBFilePath);
			m_pSQLiteDB->execDML(m_strKey);
			m_pSQLiteDB->setBusyTimeout(5000); // 최대 5초 대기
		}		
	}
	catch(CppSQLite3Exception& e){
		m_strErrorString = e.errorMessage();
		bRe = FALSE;
	}

	return bRe;
}

void CSQLiteManager::CloseDB(void)
{
	try{
		if(NULL == m_pSQLiteDB) return;

		m_pSQLiteDB->close();
	}
	catch(CppSQLite3Exception& e){
		m_strErrorString = e.errorMessage();
	}
}

BOOL CSQLiteManager::IsExistTable(CString strTableName)
{
	BOOL bExist = TRUE;
	
	if(NULL == m_pSQLiteDB) return FALSE;
	
	try{
		bExist = m_pSQLiteDB->tableExists(strTableName);
	}
	catch(CppSQLite3Exception& e){
		m_strErrorString = e.errorMessage();
		return bExist;
	}

	return bExist;
}

BOOL CSQLiteManager::SetExcuteNoResultset(CString strQuery, BOOL bOpenDB)
{
	BOOL bResult = TRUE;

	if(NULL == m_pSQLiteDB) return FALSE;
	
	if (TRUE == bOpenDB){
		if (FALSE == OpenDB())
			return FALSE;
	}

	try{
		m_pSQLiteDB->execDML(strQuery);
	}
	catch(CppSQLite3Exception& e){
		m_strErrorString = e.errorMessage();
		bResult = FALSE;
	}

	if (TRUE == bOpenDB) CloseDB();

	return bResult;
}

CppSQLite3Query* CSQLiteManager::SetExcute(CString strQuery, BOOL bOpenDB)
{
	if(NULL == m_pSQLiteDB) return NULL;

	if (TRUE == bOpenDB) {
		if (FALSE == OpenDB())
			return FALSE;
	}

	try{
		m_sqlite3Query = m_pSQLiteDB->execQuery(strQuery);
	}
	catch(CppSQLite3Exception& e){
		m_strErrorString = e.errorMessage();
	}

	if (TRUE == bOpenDB) CloseDB();

	return &m_sqlite3Query;
}

void CSQLiteManager::SetTableCheckForCreate()
{









}