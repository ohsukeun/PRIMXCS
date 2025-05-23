#include "StdAfx.h"
#include "FileDBManager.h"
#include "ConfigDataManager.h"

typedef struct _PRIMX_TABLE_INFO_ {
	CString	strTableName;
	CString	strSQL;
} PRIMX_TABLE_INFO;

#define PRIMX_TABLE_SYSTEM_MASTER				_T("T_SYSTEM_MASTER")   
#define PRIMX_TABLE_SYSTEM_MASTER_CREATE		_T("CREATE TABLE T_SYSTEM_MASTER (\
													C_AGENT_ID	VARCHAR(50) NOT NULL,\
													C_WIN_VERSION_STRING	VARCHAR(50),\
													C_UUID	VARCHAR(50),\
													C_MAJOR_VERSION	INT,\
													C_MINOR_VERSION	INT,\
													C_BUILD_NUMBER	VARCHAR(50),\
													C_SERVICEPACK_MAJOR	INT,\
													C_SERVICEPACK_MINOR	INT,\
													C_PROCESSOR_ARCHITECTURE	INT,\
													C_RELEASE_ID	VARCHAR(10),\
													C_COMPUTER_NAME	VARCHAR(260),\
													C_WORKGROUP_NAME	VARCHAR(260),\
													PRIMARY KEY(C_AGENT_ID));")

#define PRIMX_TABLE_NIC_INFO					_T("T_NIC_INFO")   
#define PRIMX_TABLE_NIC_INFO_CREATE				_T("CREATE TABLE T_NIC_INFO (\
													C_AGENT_ID	VARCHAR(50) NOT NULL,\
													C_ADAPTER_ID	VARCHAR(50) NOT NULL,\
													C_MAC	VARCHAR(30),\
													C_IP	VARCHAR(15),\
													C_GATEWAY	VARCHAR(15),\
													C_DNS	VARCHAR(15),\
													C_DHCP_ENABLE	TINYINT,\
													C_DHCP_SERVER	VARCHAR(15),\
													C_ADAPTER_DESC	VARCHAR(50),\
													PRIMARY KEY(C_AGENT_ID, C_ADAPTER_ID));")

#define PRIMX_TABLE_DISK_INFO					_T("T_DISK_INFO")   
#define PRIMX_TABLE_DISK_INFO_CREATE			_T("CREATE TABLE T_DISK_INFO (\
													C_AGENT_ID	VARCHAR(50) NOT NULL,\
													C_DISK_NAME	VARCHAR(50) NOT NULL,\
													C_DISK_TYPE	TINYINT,\
													C_VOLUME_NAME	VARCHAR(50),\
													C_FILE_SYSTEM_NAME	VARCHAR(50),\
													C_TOTAL_SIZE	BIGINT,\
													C_FREE_SIZE	BIGINT,\
													PRIMARY KEY(C_DISK_NAME, C_AGENT_ID)); ")

#define PRIMX_TABLE_PRIMX_SETUP_INFO			_T("T_PRIMX_SETUP_INFO")   
#define PRIMX_TABLE_PRIMX_SETUP_INFO_CREATE		_T("CREATE TABLE T_PRIMX_SETUP_INFO (\
													C_AGENT_ID	VARCHAR(50) NOT NULL,\
													C_PRODUCT_TYPE	TINYINT NOT NULL,\
													C_PRODUCT_NAME	VARCHAR(50),\
													C_PRODUCT_VERSION	VARCHAR(20),\
													C_INSTALL_LOCATION	VARCHAR(300),\
													C_INSTALL_DATE	VARCHAR(30),\
													C_UNINSTALL_STRING	VARCHAR(600),\
													C_PACKAGE_MASTER	VARCHAR(50),\
													C_POLICY_SET		VARCHAR(50),\
													C_LAST_POLICY_ID	VARCHAR(20),\
													C_LAST_POLICY_DATE	DATETIME,\
													PRIMARY KEY(C_PRODUCT_TYPE, C_AGENT_ID));")


#define PRIMX_TABLE_LOG_INFO					_T("T_LOG_INFO")   
#define PRIMX_TABLE_LOG_INFO_CREATE				_T("CREATE TABLE T_LOG_INFO (\
													C_SEQ_NO	INTEGER,\
													C_PRODUCT_TYPE	INT,\
													C_LOG_GUBUN	INT,\
													C_LOG_DATE	VARCHAR(30),\
													C_LOG_TYPE	VARCHAR(20),\
													C_LOG_CODE	VARCHAR(20),\
													C_LOG_LEVEL	VARCHAR(10),\
													C_LOG_DATA	VARCHAR(1024),\
													C_STATE	INTEGER,\
													PRIMARY KEY(C_SEQ_NO AUTOINCREMENT)); ")

#define PRIMX_TABLE_EXPORT_FILE_INFO			_T("T_EXPORT_FILE_INFO")   
#define PRIMX_TABLE_EXPORT_FILE_INFO_CREATE		_T("CREATE TABLE T_EXPORT_FILE_INFO (\
													C_KEY	VARCHAR(30),\
													C_FILE_NAME	VARCHAR(256),\
													C_FILE_SIZE	BIGINT,\
													C_ORG_FILE_TOTAL_SIZE	BIGINT,\
													C_CREATE_DATE	VARCHAR(30),\
													PRIMARY KEY(C_KEY)); ")

#define PRIMX_TABLE_ORG_FILE_INFO				_T("T_ORG_FILE_INFO")   
#define PRIMX_TABLE_ORG_FILE_INFO_CREATE		_T("CREATE TABLE T_ORG_FILE_INFO (\
													C_SEQ_NO	INTEGER,\
													C_KEY	VARCHAR(30),\
													C_ORG_FILE_NAME	VARCHAR(256),\
													C_ORG_FILE_SIZE	BIGINT,\
													PRIMARY KEY(C_SEQ_NO AUTOINCREMENT)); ")



static PRIMX_TABLE_INFO FileDBTable[] = {
	{ PRIMX_TABLE_SYSTEM_MASTER,		PRIMX_TABLE_SYSTEM_MASTER_CREATE },
	{ PRIMX_TABLE_PRIMX_SETUP_INFO,		PRIMX_TABLE_PRIMX_SETUP_INFO_CREATE },
	{ PRIMX_TABLE_NIC_INFO,				PRIMX_TABLE_NIC_INFO_CREATE },
	{ PRIMX_TABLE_DISK_INFO,			PRIMX_TABLE_DISK_INFO_CREATE }
};

CFileDBManager::CFileDBManager()
{
	m_pSQLiteDB = std::unique_ptr<CppSQLite3DB>(new CppSQLite3DB);
	m_strDBHandlingError = _T("File DB error[%d] : %s");

	CConfigDataManager ConfigDataManager;
	CString strKey = _T("PRIMXAgent"); // ConfigDataManager.GetAgentID();
	strKey.Trim();
	m_strKey.Format(_T("pragma key='%s'"), strKey);
}

CFileDBManager::~CFileDBManager(void)
{
	m_sqlite3Query.finalize();

	if(NULL != m_pSQLiteDB){
		m_pSQLiteDB->close();
	}

}

BOOL CFileDBManager::SetInitDataBase()
{
	BOOL bRe = FALSE;
	if(NULL == m_pSQLiteDB) return bRe;

	try
	{
		CString strFileDBName;
		strFileDBName.Format(_T("%s\\%s"), GetProgramDataPath(), PRIMX_DB_FILE_NAME);

		CString strFileDBPath;
		strFileDBPath.Format(_T("%s"), GetProgramDataPath());
		SetCreateDirectoryEx(strFileDBPath);

		m_strFileDBName = strFileDBName;

		m_pSQLiteDB->open(m_strFileDBName);
		m_pSQLiteDB->execDML(m_strKey);
		m_pSQLiteDB->setBusyTimeout(5000); // 최대 5초 대기

		for(int i=0;i<_countof(FileDBTable);i++){
			if(FALSE == IsExistTable(FileDBTable[i].strTableName)){
				m_pSQLiteDB->execQuery(FileDBTable[i].strSQL);
			}	
		}
		
		m_pSQLiteDB->close();
		bRe = TRUE;
	}
	catch(CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
	}

	return bRe;

}

BOOL CFileDBManager::SetInitDataBaseForLogData()
{
	BOOL bRe = FALSE;
	if (NULL == m_pSQLiteDB) return bRe;

	try
	{
		CString strFileDBName;
		strFileDBName.Format(_T("%s\\%s"), GetProgramDataPath(), PRIMX_DB_LOG_FILE_NAME);

		CString strFileDBPath;
		strFileDBPath.Format(_T("%s"), GetProgramDataPath());
		SetCreateDirectoryEx(strFileDBPath);

		m_strFileDBName = strFileDBName;

		m_pSQLiteDB->open(m_strFileDBName);
		m_pSQLiteDB->execDML(m_strKey);
		m_pSQLiteDB->setBusyTimeout(5000); // 최대 5초 대기
		
		if (FALSE == IsExistTable(PRIMX_TABLE_LOG_INFO)) {
			m_pSQLiteDB->execQuery(PRIMX_TABLE_LOG_INFO_CREATE);
		}

		m_pSQLiteDB->close();
		bRe = TRUE;
	}
	catch (CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
	}

	return bRe;

}
BOOL CFileDBManager::SetInitDataBaseForExportFile()
{
	BOOL bRe = FALSE;
	if (NULL == m_pSQLiteDB) return bRe;

	try
	{
		CString strFileDBName;
		strFileDBName.Format(_T("%s\\%s"), GetProgramDataPath(), PRIMX_DB_EXPORT_FILE_NAME);

		CString strFileDBPath;
		strFileDBPath.Format(_T("%s"), GetProgramDataPath());
		SetCreateDirectoryEx(strFileDBPath);

		if (0 == GetFileSizeEx(strFileDBName)) {
			SetDeleteFile(strFileDBName);
		}

		m_strFileDBName = strFileDBName;

		m_pSQLiteDB->open(m_strFileDBName);
		m_pSQLiteDB->execDML(m_strKey);
		m_pSQLiteDB->setBusyTimeout(5000); // 최대 5초 대기

		if (FALSE == IsExistTable(PRIMX_TABLE_EXPORT_FILE_INFO)) {
			m_pSQLiteDB->execQuery(PRIMX_TABLE_EXPORT_FILE_INFO_CREATE);
		}
		if (FALSE == IsExistTable(PRIMX_TABLE_ORG_FILE_INFO)) {
			m_pSQLiteDB->execQuery(PRIMX_TABLE_ORG_FILE_INFO_CREATE);
		}

		m_pSQLiteDB->close();
		bRe = TRUE;
	}
	catch (CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
	}

	return bRe;

}
CString CFileDBManager::GetErrorString()
{
	return m_strErrorString;
}

BOOL CFileDBManager::OpenDB(void)
{
	BOOL bRe = TRUE;
	
	if(NULL == m_pSQLiteDB) return bRe;

	try
	{	
		CString strFileDBName;
		strFileDBName.Format(_T("%s\\%s"), GetProgramDataPath(), PRIMX_DB_FILE_NAME);

		CString strFileDBPath;
		strFileDBPath.Format(_T("%s"), GetProgramDataPath());
		SetCreateDirectoryEx(strFileDBPath);
		
		m_strFileDBName = strFileDBName;

		m_pSQLiteDB->open(m_strFileDBName);
		m_pSQLiteDB->execDML(m_strKey);

		//m_pSQLiteDB->execDML(L"PRAGMA journal_mode=WAL");
		//m_pSQLiteDB->execDML(L"PRAGMA temp_store=MEMORY");
		//m_pSQLiteDB->execDML(L"PRAGMA synchronous=OFF"); // NORMAL
		//m_pSQLiteDB->execDML(L"PRAGMA cache_size=10000");
				
		m_pSQLiteDB->setBusyTimeout(5000); // 최대 5초 대기
	}
	catch(CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
		bRe = FALSE;
	}

	return bRe;
}

BOOL CFileDBManager::OpenLogDB(void)
{
	BOOL bRe = TRUE;

	if (NULL == m_pSQLiteDB) return bRe;

	try
	{
		CString strFileDBName;
		strFileDBName.Format(_T("%s\\%s"), GetProgramDataPath(), PRIMX_DB_LOG_FILE_NAME);

		CString strFileDBPath;
		strFileDBPath.Format(_T("%s"), GetProgramDataPath());
		SetCreateDirectoryEx(strFileDBPath);

		m_strFileDBName = strFileDBName;

		m_pSQLiteDB->open(m_strFileDBName);
		m_pSQLiteDB->execDML(m_strKey);
		//m_pSQLiteDB->execDML(L"PRAGMA journal_mode=WAL");
		//m_pSQLiteDB->execDML(L"PRAGMA temp_store=MEMORY");
		//m_pSQLiteDB->execDML(L"PRAGMA synchronous=OFF"); // NORMAL
		//m_pSQLiteDB->execDML(L"PRAGMA cache_size=10000");
		m_pSQLiteDB->setBusyTimeout(5000); // 최대 5초 대기

	}
	catch (CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
		bRe = FALSE;
	}

	return bRe;
}
BOOL CFileDBManager::OpenExportDB(void)
{
	BOOL bRe = TRUE;

	if (NULL == m_pSQLiteDB) return bRe;

	try
	{
		CString strFileDBName;
		strFileDBName.Format(_T("%s\\%s"), GetProgramDataPath(), PRIMX_DB_EXPORT_FILE_NAME);

		CString strFileDBPath;
		strFileDBPath.Format(_T("%s"), GetProgramDataPath());
		SetCreateDirectoryEx(strFileDBPath);

		m_strFileDBName = strFileDBName;

		m_pSQLiteDB->open(m_strFileDBName);
		m_pSQLiteDB->execDML(m_strKey);
		//m_pSQLiteDB->execDML(L"PRAGMA journal_mode=WAL");
		//m_pSQLiteDB->execDML(L"PRAGMA temp_store=MEMORY");
		//m_pSQLiteDB->execDML(L"PRAGMA synchronous=OFF"); // NORMAL
		//m_pSQLiteDB->execDML(L"PRAGMA cache_size=10000");
		m_pSQLiteDB->setBusyTimeout(5000); // 최대 5초 대기

	}
	catch (CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
		bRe = FALSE;
	}

	return bRe;
}
void CFileDBManager::CloseDB(void)
{
	try
	{
		if(NULL == m_pSQLiteDB) return;

		m_pSQLiteDB->close();
	}
	catch(CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
	}
}

BOOL CFileDBManager::IsExistTable(CString strTableName)
{
	BOOL bExist = TRUE;
	
	if(NULL == m_pSQLiteDB) return FALSE;
	
	try{
		bExist = m_pSQLiteDB->tableExists(strTableName);
	}
	catch(CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
		return bExist;
	}

	return bExist;
}

BOOL CFileDBManager::SetExcuteNoResultset(CString strQuery)
{
	BOOL bResult = TRUE;

	if(NULL == m_pSQLiteDB) return FALSE;

	try{
		m_pSQLiteDB->execDML(strQuery);
	}
	catch(CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
		bResult = FALSE;
		TRACE(_T("SQLiteDB > SetExcuteNoResultset Error > %s \n"), m_strErrorString);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"SQLiteDB > SetExcuteNoResultset Error > %s", m_strErrorString);
	}

	return bResult;
}

CppSQLite3Query* CFileDBManager::SetExcute(CString strQuery)
{
	if(NULL == m_pSQLiteDB) return NULL;

	try{
		m_sqlite3Query = m_pSQLiteDB->execQuery(strQuery);
	}
	catch(CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
		TRACE(_T("SQLiteDB > SetExcute Error > %s \n"), m_strErrorString);

	}

	return &m_sqlite3Query;
}

void CFileDBManager::SetConvertData(CString& strData)
{
	strData.Trim();
	strData.Replace(_T("'"), _T("''"));
	strData.Replace(_T("\""), _T("\"\""));
}

CString CFileDBManager::SetConvertDataEx(CString strData)
{
	strData.Trim();
	strData.Replace(_T("'"), _T("''"));
//	strData.Replace(_T("\""), _T("\"\""));
	return strData;
}

BOOL CFileDBManager::SetSystemDataDBSave(CString strAgentID, CString strUserID, UINT nSystemType, PRIMX_SYSTEM_DATA& SystemData, BOOL bBeforeDataRemove)
{
	BOOL bRe = TRUE;

	if (TRUE == OpenDB()) {
		CString strSQL;

		if (TRUE == bBeforeDataRemove) {
			strSQL.Format(_T("DELETE FROM T_SYSTEM_MASTER;"));
			SetExcuteNoResultset(strSQL);
			strSQL.Format(_T("DELETE FROM T_DISK_INFO;"));
			SetExcuteNoResultset(strSQL);
			strSQL.Format(_T("DELETE FROM T_NIC_INFO;"));
			SetExcuteNoResultset(strSQL);
		}

		strSQL.Format(_T("REPLACE INTO T_SYSTEM_MASTER( C_AGENT_ID,\
														C_WIN_VERSION_STRING,\
														C_UUID,\
														C_MAJOR_VERSION,\
														C_MINOR_VERSION,\
														C_BUILD_NUMBER,\
														C_SERVICEPACK_MAJOR,\
														C_SERVICEPACK_MINOR,\
														C_PROCESSOR_ARCHITECTURE,\
														C_RELEASE_ID,\
														C_COMPUTER_NAME,\
														C_WORKGROUP_NAME) VALUES('%s','%s','%s',%lu,%lu,'%s',%lu,%lu,%lu,'%s','%s','%s');"),
														SetConvertDataEx(strAgentID),
														SetConvertDataEx(SystemData.WindowsVersion.strVersionFullName),
														SetConvertDataEx(SystemData.WindowsVersion.strUUID),
														SystemData.WindowsVersion.nMajorVersion,
														SystemData.WindowsVersion.nMinorVersion,
														SetConvertDataEx(SystemData.WindowsVersion.strBuildNumber),
														SystemData.WindowsVersion.nServicePackMajor,
														SystemData.WindowsVersion.nServicePackMinor,
														SystemData.WindowsVersion.nProcessorArchitecture,
														SetConvertDataEx(SystemData.WindowsVersion.strReleaseID),
														SetConvertDataEx(SystemData.NetBiosData.strComputerName),
														SetConvertDataEx(SystemData.NetBiosData.strWorkGroupName));
		if (TRUE == SetExcuteNoResultset(strSQL)) {
			POSITION Pos = SystemData.DiskDriveList.GetHeadPosition();
			while (NULL != Pos) {
				PRIMX_DISK_DRIVE_DATA* pData = SystemData.DiskDriveList.GetNext(Pos);
				if (NULL != pData) {
					CString strSQL;
					strSQL.Format(_T("REPLACE INTO T_DISK_INFO( C_AGENT_ID, \
																C_DISK_NAME, \
																C_DISK_TYPE, \
																C_VOLUME_NAME, \
																C_FILE_SYSTEM_NAME, \
																C_TOTAL_SIZE, \
																C_FREE_SIZE) VALUES('%s','%s',%lu,'%s','%s',%lu,%lu);"),
																SetConvertDataEx(strAgentID),
																SetConvertDataEx(pData->strName),
																pData->nType,
																SetConvertDataEx(pData->strVolumeName),
																SetConvertDataEx(pData->strFileSystemName),
																pData->nTotalSize,
																pData->nFreeSize);
					SetExcuteNoResultset(strSQL);
				}
			}

			Pos = SystemData.NICList.GetHeadPosition();
			while (NULL != Pos) {
				PRIMX_NIC_DATA* pData = SystemData.NICList.GetNext(Pos);
				if (NULL != pData) {
					CString strSQL;
					strSQL.Format(_T("REPLACE INTO T_NIC_INFO(  C_AGENT_ID, \
																C_ADAPTER_ID,\
																C_MAC, \
																C_IP, \
																C_GATEWAY, \
																C_DNS, \
																C_DHCP_ENABLE,\
																C_DHCP_SERVER,\
																C_ADAPTER_DESC) VALUES('%s','%s','%s','%s','%s','%s',%lu,'%s','%s');"),
																SetConvertDataEx(strAgentID),
																SetConvertDataEx(pData->strAdapterGUID),
																SetConvertDataEx(pData->strMAC),
																SetConvertDataEx(pData->strIP),
																SetConvertDataEx(pData->strGW),
																SetConvertDataEx(pData->strDNS),
																pData->nEnableDHCP,
																SetConvertDataEx(pData->strDHCPServer),
																SetConvertDataEx(pData->strDescription));
					SetExcuteNoResultset(strSQL);
				}
			}			
		}		
		CloseDB();
	}
	return bRe;
}

BOOL CFileDBManager::GetSystemData(CString strAgentID, PRIMX_SYSTEM_DATA& SystemData)
{
	BOOL bRe = FALSE;

	try {
		if (TRUE == OpenDB()) {
			CString strSQL;
			strSQL.Format(_T("SELECT * FROM T_SYSTEM_MASTER WHERE C_AGENT_ID='%s';"), SetConvertDataEx(strAgentID));

			CppSQLite3Query* pResult = SetExcute(strSQL);
			if (NULL != pResult) {
				if (!pResult->eof()) {

					SystemData.WindowsVersion.strVersionFullName = pResult->getStringField(_T("C_WIN_VERSION_STRING"));
					SystemData.WindowsVersion.strUUID = pResult->getStringField(_T("C_UUID"));
					SystemData.WindowsVersion.nMajorVersion = pResult->getIntField(_T("C_MAJOR_VERSION"));
					SystemData.WindowsVersion.nMinorVersion = pResult->getIntField(_T("C_MINOR_VERSION"));
					SystemData.WindowsVersion.strBuildNumber = pResult->getStringField(_T("C_BUILD_NUMBER"));
					SystemData.WindowsVersion.nServicePackMajor = pResult->getIntField(_T("C_SERVICEPACK_MAJOR"));
					SystemData.WindowsVersion.nServicePackMinor = pResult->getIntField(_T("C_SERVICEPACK_MINOR"));
					SystemData.WindowsVersion.nProcessorArchitecture = pResult->getIntField(_T("C_PROCESSOR_ARCHITECTURE"));
					SystemData.WindowsVersion.strReleaseID = pResult->getStringField(_T("C_RELEASE_ID"));		
					SystemData.NetBiosData.strComputerName = pResult->getStringField(_T("C_COMPUTER_NAME"));
					SystemData.NetBiosData.strWorkGroupName = pResult->getStringField(_T("C_WORKGROUP_NAME"));
					SystemData.WindowsVersion.Trim();
					SystemData.NetBiosData.Trim();
					bRe = TRUE;
				}
				pResult->finalize();
			}

			strSQL.Format(_T("SELECT * FROM T_NIC_INFO WHERE C_AGENT_ID='%s';"), SetConvertDataEx(strAgentID));
			pResult = SetExcute(strSQL);
			if (NULL != pResult) {
				while (!pResult->eof()) {
					PRIMX_NIC_DATA* pNICData = new PRIMX_NIC_DATA;
					pNICData->strAdapterGUID = pResult->getStringField(_T("C_ADAPTER_ID"));
					pNICData->strDescription = pResult->getStringField(_T("C_ADAPTER_DESC"));
					pNICData->strMAC = pResult->getStringField(_T("C_MAC"));
					pNICData->strGW = pResult->getStringField(_T("C_GATEWAY"));
					pNICData->strDNS = pResult->getStringField(_T("C_DNS"));
					pNICData->strDHCPServer = pResult->getStringField(_T("C_DHCP_SERVER"));
					pNICData->strIP = pResult->getStringField(_T("C_IP"));
					pNICData->nEnableDHCP = pResult->getIntField(_T("C_DHCP_ENABLE"));
					pNICData->Trim();
					if (NULL == SystemData.NICList.AddTail(pNICData)) {
						_SafeDelete(pNICData);
					}
					pResult->nextRow();
				}
				pResult->finalize();
			}

			strSQL.Format(_T("SELECT * FROM T_DISK_INFO WHERE C_AGENT_ID='%s';"), SetConvertDataEx(strAgentID));
			pResult = SetExcute(strSQL);
			if (NULL != pResult) {
				while (!pResult->eof()) {
					PRIMX_DISK_DRIVE_DATA* pDiskData = new PRIMX_DISK_DRIVE_DATA;
					pDiskData->strName = pResult->getStringField(_T("C_DISK_NAME"));
					pDiskData->nType = pResult->getIntField(_T("C_DISK_TYPE"));
					pDiskData->strVolumeName = pResult->getStringField(_T("C_VOLUME_NAME"));
					pDiskData->strFileSystemName = pResult->getStringField(_T("C_FILE_SYSTEM_NAME"));
					pDiskData->nTotalSize = pResult->getIntField(_T("C_TOTAL_SIZE"));
					pDiskData->nFreeSize = pResult->getIntField(_T("C_FREE_SIZE"));
					pDiskData->Trim();
					if (NULL == SystemData.DiskDriveList.AddTail(pDiskData)) {
						_SafeDelete(pDiskData);
					}
					pResult->nextRow();
				}
				pResult->finalize();
			}
			CloseDB();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
		TRACE(_T("DB Error > %s \n"), m_strErrorString);
	}	
	return bRe;
}

BOOL CFileDBManager::SetPRIMXProductDataDBSave(CString strAgentID,PRIMX_PRODUCT_DATA& Zonecentrol, PRIMX_PRODUCT_DATA& Orizone, PRIMX_PRODUCT_DATA& Cryhod)
{
	BOOL bRe = FALSE;

	if (TRUE == OpenDB()) {
		SetPRIMXProductDataDBSave(strAgentID, PRIMX_PRODUCT_TYPE_ZONECENTRAL, Zonecentrol);
		SetPRIMXProductDataDBSave(strAgentID, PRIMX_PRODUCT_TYPE_ORIZON, Orizone);
		SetPRIMXProductDataDBSave(strAgentID, PRIMX_PRODUCT_TYPE_CRYHOD, Cryhod);		
		CloseDB();
		bRe = TRUE;
	}
	return bRe;
}

BOOL CFileDBManager::SetPRIMXProductDataDBSave(CString strAgentID,UINT nProductType, PRIMX_PRODUCT_DATA& ProductData)
{
	BOOL bRe = FALSE;

	CString strSQL;
	if (TRUE == ProductData.bInstall) {
		strSQL.Format(_T("REPLACE INTO T_PRIMX_SETUP_INFO(  C_AGENT_ID, \
														C_PRODUCT_TYPE,\
														C_PRODUCT_NAME, \
														C_PRODUCT_VERSION, \
														C_UNINSTALL_STRING, \
														C_PACKAGE_MASTER, \
														C_POLICY_SET,\
														C_LAST_POLICY_ID,\
														C_INSTALL_LOCATION,\
														C_INSTALL_DATE,\
														C_LAST_POLICY_DATE) VALUES('%s',%lu,'%s','%s','%s','%s','%s','%s','%s','%s','%s');"),
			SetConvertDataEx(strAgentID),
			nProductType,
			SetConvertDataEx(ProductData.strDisplayName),
			SetConvertDataEx(ProductData.strDisplayVersionEx),
			SetConvertDataEx(ProductData.strUninstallString),
			SetConvertDataEx(ProductData.strPackageMaster),
			SetConvertDataEx(ProductData.strPolictSet),
			SetConvertDataEx(ProductData.strLastPolicyID),
			SetConvertDataEx(ProductData.strInstallLocation),
			SetConvertDataEx(ProductData.strInstallDate),
			SetConvertDataEx(ProductData.strLastPolicyDate));

	}
	else {
		strSQL.Format(_T("DELETE FROM T_PRIMX_SETUP_INFO WHERE C_AGENT_ID='%s' AND C_PRODUCT_TYPE=%lu;"), SetConvertDataEx(strAgentID), nProductType);
	}

	bRe = SetExcuteNoResultset(strSQL);

	return bRe;
}

BOOL CFileDBManager::GetPRIMXProductData(CString strAgentID, PRIMX_PRODUCT_DATA& Zonecentrol, PRIMX_PRODUCT_DATA& Orizone, PRIMX_PRODUCT_DATA& Cryhod)
{
	BOOL bRe = FALSE;

	if (TRUE == OpenDB()){
		GetPRIMXProductData(strAgentID, PRIMX_PRODUCT_TYPE_ZONECENTRAL, Zonecentrol);
		GetPRIMXProductData(strAgentID, PRIMX_PRODUCT_TYPE_ORIZON, Orizone);
		GetPRIMXProductData(strAgentID, PRIMX_PRODUCT_TYPE_CRYHOD, Cryhod);
		CloseDB();
		bRe = TRUE;
	}

	return bRe;
}

BOOL CFileDBManager::GetPRIMXProductData(CString strAgentID, UINT nProductType, PRIMX_PRODUCT_DATA& ProductData)
{
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM T_PRIMX_SETUP_INFO WHERE C_AGENT_ID='%s' AND C_PRODUCT_TYPE=%lu;"), SetConvertDataEx(strAgentID), nProductType);
	try {

		CppSQLite3Query* pResult = SetExcute(strSQL);
		if (NULL != pResult) {
			if (!pResult->eof()) {

				ProductData.bInstall = TRUE;
				ProductData.strDisplayVersionEx = pResult->getStringField(_T("C_PRODUCT_VERSION"));
				ProductData.strDisplayName = pResult->getStringField(_T("C_PRODUCT_NAME"));
				ProductData.strUninstallString = pResult->getStringField(_T("C_UNINSTALL_STRING"));
				ProductData.strInstallLocation = pResult->getStringField(_T("C_INSTALL_LOCATION"));
				ProductData.strInstallDate = pResult->getStringField(_T("C_INSTALL_DATE"));
				ProductData.strLastPolicyDate = pResult->getStringField(_T("C_LAST_POLICY_DATE"));
				ProductData.strLastPolicyID = pResult->getStringField(_T("C_LAST_POLICY_ID"));
				ProductData.strPolictSet = pResult->getStringField(_T("C_POLICY_SET"));
				ProductData.strPackageMaster = pResult->getStringField(_T("C_PACKAGE_MASTER"));
			}
			pResult->finalize();
		}
		else {
			ProductData.bInstall = FALSE;
		}

	}
	catch (CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
		TRACE(_T("DB Error > %s \n"), m_strErrorString);
	}
	return TRUE;
}

CString CFileDBManager::GetDateTimeGMT0(COleDateTime& DateTime)
{
	CString strDateTime;

	SYSTEMTIME UTCTime;
	SYSTEMTIME SystemTime;
	DateTime.GetAsSystemTime(SystemTime);

	TzSpecificLocalTimeToSystemTime(NULL,&SystemTime,&UTCTime);
	
	strDateTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
		UTCTime.wYear,
		UTCTime.wMonth,
		UTCTime.wDay,
		UTCTime.wHour,
		UTCTime.wMinute,
		UTCTime.wSecond);

	return strDateTime;
}

BOOL CFileDBManager::SetLogDataSave(PRIMX_LOG_DATA& DBSaveData,UINT nType)
{
	BOOL bRe = FALSE;

	CString strDateTime;
	strDateTime.Format(_T("%s %s"), DBSaveData.strLogDate, DBSaveData.strLogTime);
	COleDateTime LogDateTime;
	if (false == LogDateTime.ParseDateTime(strDateTime)) {
		return bRe;
	}
	strDateTime = GetDateTimeGMT0(LogDateTime);
	
	CString strSQL;
	strSQL.Format(_T("INSERT INTO T_LOG_INFO(C_PRODUCT_TYPE,\
											C_LOG_GUBUN,\
											C_LOG_DATE, \
											C_LOG_TYPE, \
											C_LOG_CODE, \
											C_LOG_LEVEL, \
											C_LOG_DATA,C_STATE) VALUES(%lu,%lu,'%s','%s','%s','%s','%s',1);"),
		DBSaveData.nProductType,
		nType,
		strDateTime,
		SetConvertDataEx(DBSaveData.strLogType),
		SetConvertDataEx(DBSaveData.strLogCode),
		SetConvertDataEx(DBSaveData.strLogLevel),
		SetConvertDataEx(DBSaveData.strLogData));

	bRe = SetExcuteNoResultset(strSQL);

	return bRe;
}

BOOL CFileDBManager::GetLogDataTransfer(CList<PRIMX_LOG_DATA*>& LogDataList)
{
	BOOL bRe = FALSE;

	if (TRUE == OpenLogDB()) {
		UINT nTransferLogCount = 0;
		CString strSQL;
		strSQL = _T("SELECT * FROM T_LOG_INFO WHERE C_STATE=1;");
		try {

			CppSQLite3Query* pResult = SetExcute(strSQL);
			if (NULL != pResult) {
				while (!pResult->eof()) {
					PRIMX_LOG_DATA* pLogData = new PRIMX_LOG_DATA;

					pLogData->nProductType = pResult->getIntField(_T("C_PRODUCT_TYPE"));
					pLogData->strLogDate = pResult->getStringField(_T("C_LOG_DATE"));
					pLogData->strLogType = pResult->getStringField(_T("C_LOG_TYPE"));
					pLogData->strLogCode = pResult->getStringField(_T("C_LOG_CODE"));
					pLogData->strLogLevel = pResult->getStringField(_T("C_LOG_LEVEL"));
					pLogData->strLogData = pResult->getStringField(_T("C_LOG_DATA"));

					if (NULL == LogDataList.AddTail(pLogData)) {
						_SafeDelete(pLogData);
					}

					nTransferLogCount++;
					pResult->nextRow();
				}
				pResult->finalize();
			}

		}
		catch (CppSQLite3Exception& e)
		{
			m_strErrorString = e.errorMessage();
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"SQLite Fail > %s", m_strErrorString);
		}

		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"로그 전송 개수 > %lu", nTransferLogCount);

		CloseDB();
		bRe = TRUE;
	}
	return bRe;
}

BOOL CFileDBManager::SetLogDataTransfer(CList<PRIMX_LOG_DATA*>& LogDataList)
{
	BOOL bRe = FALSE;

	if (TRUE == OpenLogDB()) {
		CString strSQL;
//		strSQL.Format(_T("DELETE FROM T_LOG_INFO;"));
		strSQL.Format(_T("UPDATE T_LOG_INFO SET C_STATE=%lu;"), 2);
		bRe = SetExcuteNoResultset(strSQL);
		CloseDB();
	}
	return bRe;
}

BOOL CFileDBManager::SetExportFileDataSave(CString strExFileKey, CString strExFileName, LONGLONG nExFileSize,CList<PRIMX_ORG_FILE_INFO*>& OrgFileList)
{
	BOOL bRe = FALSE;

	CString strSQL;

	LONGLONG nOrgFileSize = 0;

	if (0 == OrgFileList.GetCount()) {
		return bRe;
	}
	if (TRUE == OpenExportDB()) {
		try {
			POSITION Pos = OrgFileList.GetHeadPosition();
			while (NULL != Pos) {
				PRIMX_ORG_FILE_INFO* pFileData = OrgFileList.GetNext(Pos);
				if (NULL != pFileData) {
					nOrgFileSize = nOrgFileSize + pFileData->nFileSize;

					strSQL.Format(_T("INSERT INTO T_ORG_FILE_INFO(C_KEY,C_ORG_FILE_NAME,C_ORG_FILE_SIZE) VALUES('%s','%s',%I64u)"),
						SetConvertDataEx(strExFileKey),
						SetConvertDataEx(SetSplitPathFileName(pFileData->strFilePath)),
						pFileData->nFileSize);

					if (FALSE == SetExcuteNoResultset(strSQL)) {
						strSQL.Format(_T("DELETE FROM T_ORG_FILE_INFO WHERE C_KEY='%s'"), SetConvertDataEx(strExFileKey));
						SetExcuteNoResultset(strSQL);
						CloseDB();
						return FALSE;
					}
				}
			}
			strSQL.Format(_T("INSERT INTO T_EXPORT_FILE_INFO(C_KEY,C_FILE_NAME,C_FILE_SIZE,C_ORG_FILE_TOTAL_SIZE,C_CREATE_DATE) VALUES('%s','%s',%I64u,%I64u,'%s');"),
				SetConvertDataEx(strExFileKey),
				SetConvertDataEx(strExFileName),
				nExFileSize,
				nOrgFileSize,
				GetTimeString());

			bRe = SetExcuteNoResultset(strSQL);

			if (FALSE == bRe) {
				strSQL.Format(_T("DELETE FROM T_ORG_FILE_INFO WHERE C_KEY='%s';"), SetConvertDataEx(strExFileKey));
				SetExcuteNoResultset(strSQL);
				CloseDB();
				return FALSE;
			}

		}
		catch (CppSQLite3Exception& e)
		{
			m_strErrorString = e.errorMessage();
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"SQLite Fail > %s", m_strErrorString);
		}

		CloseDB();
	}

	return bRe;
}

BOOL CFileDBManager::GetExportFileData(CList<PRIMX_EXPORT_FILE_INFO*>& ExFileList)
{
	CString strSQL;
	strSQL.Format(_T("SELECT A.C_KEY,A.C_FILE_NAME,A.C_FILE_SIZE,A.C_ORG_FILE_TOTAL_SIZE,A.C_CREATE_DATE,B.C_ORG_FILE_NAME,B.C_ORG_FILE_SIZE FROM T_EXPORT_FILE_INFO as A, T_ORG_FILE_INFO as B where A.C_KEY = B.C_KEY ORDER BY A.C_KEY;"));
	try {
		if (TRUE == OpenExportDB()) {
			CppSQLite3Query* pResult = SetExcute(strSQL);
			if (NULL != pResult) {
				PRIMX_EXPORT_FILE_INFO* pTempData = NULL;
				while (!pResult->eof()) {
					CString strKey = pResult->getStringField(_T("C_KEY"));
					if (NULL != pTempData) {
						if (0 != strKey.CompareNoCase(pTempData->strExportFileKey)) {
							pTempData = NULL;
						}
					}
					if (NULL == pTempData) {
						pTempData = new PRIMX_EXPORT_FILE_INFO;
						ExFileList.AddTail(pTempData);
						pTempData->strExportFileKey = strKey;
						pTempData->strExportFileName = pResult->getStringField(_T("C_FILE_NAME"));
						pTempData->nExportFileSize = AtoLONG2(pResult->getStringField(_T("C_FILE_SIZE")));
						pTempData->strCreateDate = pResult->getStringField(_T("C_CREATE_DATE"));
						pTempData->nOrgFileTotalSize = AtoLONG2(pResult->getStringField(_T("C_ORG_FILE_TOTAL_SIZE")));
					}
					PRIMX_ORG_FILE_INFO* pOrgData = new PRIMX_ORG_FILE_INFO;
					pTempData->OrgFileList.AddTail(pOrgData);
					pOrgData->strFilePath = pResult->getStringField(_T("C_ORG_FILE_NAME"));
					pOrgData->nFileSize = AtoLONG2(pResult->getStringField(_T("C_ORG_FILE_SIZE")));

					pResult->nextRow();
				}
				pResult->finalize();
			}
			CloseDB();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
		TRACE(_T("DB Error > %s \n"), m_strErrorString);
	}
	return TRUE;
}

BOOL CFileDBManager::GetExportFileData(CString strKey, PRIMX_EXPORT_FILE_INFO*& pExFileData)
{
	BOOL bRe = FALSE;
	CString strSQL;
	strSQL.Format(_T("SELECT A.C_KEY,A.C_FILE_NAME,A.C_FILE_SIZE,A.C_ORG_FILE_TOTAL_SIZE,A.C_CREATE_DATE,B.C_ORG_FILE_NAME,B.C_ORG_FILE_SIZE FROM T_EXPORT_FILE_INFO as A, T_ORG_FILE_INFO as B where A.C_KEY = B.C_KEY AND A.C_KEY = '%s';"), strKey);
	try {
		if (TRUE == OpenExportDB()) {
			CppSQLite3Query* pResult = SetExcute(strSQL);
			if (NULL != pResult) {
				while (!pResult->eof()) {
					pExFileData->strExportFileKey = pResult->getStringField(_T("C_KEY"));;
					pExFileData->strExportFileName = pResult->getStringField(_T("C_FILE_NAME"));
					pExFileData->nExportFileSize = AtoLONG2(pResult->getStringField(_T("C_FILE_SIZE")));
					pExFileData->strCreateDate = pResult->getStringField(_T("C_CREATE_DATE"));
					pExFileData->nOrgFileTotalSize = AtoLONG2(pResult->getStringField(_T("C_ORG_FILE_TOTAL_SIZE")));
					
					PRIMX_ORG_FILE_INFO* pOrgData = new PRIMX_ORG_FILE_INFO;
					pExFileData->OrgFileList.AddTail(pOrgData);
					pOrgData->strFilePath = pResult->getStringField(_T("C_ORG_FILE_NAME"));
					pOrgData->nFileSize = AtoLONG2(pResult->getStringField(_T("C_ORG_FILE_SIZE")));
					pResult->nextRow();
					bRe = TRUE;
				}
				pResult->finalize();
			}
			CloseDB();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		m_strErrorString = e.errorMessage();
		TRACE(_T("DB Error > %s \n"), m_strErrorString);
	}
	return bRe;
}