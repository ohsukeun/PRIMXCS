/// @file xma_xml.cpp
/// @author Park, sung soo
/// @date 2023-06-12
/// @brief xml I/O hamdling class

#include <iostream>
#include <sstream>

#include <pmx_db_io.h>
#include <pmx_error.h>

#ifndef __PMX_DB_IO_CPP__
#define __PMX_DB_IO_CPP__

//-----------------------------------------------------------------------------
// LOCAL DEFINTION
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// METHODS DEFINTION
//-----------------------------------------------------------------------------
SWC_BOOL CDBIO::RunSessionMngQuery(IN CMySQLDBObj* pDBObj, IN const CHAR* sModuleName)
{
	if (NULL == pDBObj) return FALSE;

	SWC_BOOL bRet = TRUE;
	WCString sQuery = (CHAR*)"";

	sQuery.Format("INSERT INTO T_SERVER_MODULE_STATUS (C_MODULE_NAME, C_CHECK_TIME) VALUES ('%s', NOW()) ON DUPLICATE KEY UPDATE C_CHECK_TIME = NOW()", sModuleName);
	pDBObj->ExecuteQuery(FALSE, sQuery);
	if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
	{
		pDBObj->Rollback();
		bRet = FALSE;
	}
	else
	{
		pDBObj->Commit();
	}
	return bRet;
}

SWC_BOOL CDBIO::RunTransaction(IN CMySQLDBObj* pDBObj, IN WCString& sQuery, OUT WCString& sError)
{
	if (NULL == pDBObj) return FALSE;

	SWC_BOOL bRet = TRUE;

	pDBObj->ExecuteQuery(FALSE, sQuery);
	if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
	{
		sError = pDBObj->GetLastErrorString();
		_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
		_LOG_ERR("%s", LPCSTR(sQuery));
		bRet = FALSE;
	}
	return bRet;
}

CMyRSet* CDBIO::ExtractUserData(IN CMySQLDBObj* pDBObj, 
								IN const CHAR* sCompanyID, 
								IN const CHAR* sUserID, 
								IN const CHAR* sAgentID, 
								OUT WCString& sFailCode, 
								OUT WCString& sFailMsg)
{
	CMyRSet* pRet = NULL;
	WCString sQuery = "";

	sQuery =  "SELECT ";
	sQuery += "  TUM.C_COMPANY_ID AS COMPANY_ID, ";
 	sQuery += "  TUM.C_USER_ID AS USER_ID, ";
 	sQuery += "  TUM.C_USER_PWD AS USER_PWD, ";
 	sQuery += "  TUM.C_USER_NAME AS USER_NAME, ";
 	sQuery += "  TDM.C_DEPT_NAME AS DEPT_NAME, ";
 	sQuery += "  TUM.C_CELL_PHONE_NO  AS CELL_PHONE_NO, ";
 	sQuery += "  TUM.C_TEL_NO AS TEL_NO, ";
 	sQuery += "  TUM.C_EMAIL_ADDR AS EMAIL_ADDR, ";
 	sQuery += "  TUM.C_EXT_NO AS EXT_NO, ";
 	sQuery += "  TUM.C_INSERTED_BY_HR_YN AS INSERTER_BY_HR_YN, ";
 	sQuery += "  TUM.C_USER_TYPE AS USER_TYPE ";
 	sQuery += "FROM ";
 	sQuery += "  T_USER_MASTER AS TUM ";
 	sQuery += "  LEFT OUTER JOIN T_DEPT_MASTER AS TDM ON TUM.C_DEPT_ID  = TDM.C_DEPT_ID ";
 	sQuery += "WHERE ";
 	sQuery.AppendFormat("  TUM.C_USER_ID = '%s' ", sUserID);

	pRet = pDBObj->ExecuteQuery(TRUE, sQuery);
	if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
	{
		_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
		_LOG_ERR("%s", LPCSTR(sQuery));
		sFailCode = PMX_ERROR_E10003;
		sFailMsg = pDBObj->GetLastErrorString();
		if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10003;
		pRet = NULL;
	}
	else
	{
		SWC_BOOL bFoundUser = TRUE;

		if      (NULL == pRet)             bFoundUser = FALSE;
		else if (0 == pRet->GetRowCount()) bFoundUser = FALSE;

		if (FALSE == bFoundUser)
		{
			sFailCode = PMX_ERROR_E20001;
			sFailMsg = PMX_ERROR_MSG_E20001;

			if (NULL != pRet)
			{
				pRet->Close();
				delete pRet;
				pRet = NULL;
			}
		}
	}
	return pRet;
}

SWC_BOOL CDBIO::CheckInsertIntoUserSystemMatch(IN CMySQLDBObj* pDBObj, 
											   IN const CHAR* sCompanyID,
											   IN const CHAR* sAgentID, 
											   IN const CHAR* sUserID, 
											   IN const CHAR* sSharePC, 
											   OUT WCString& sFailCode, 
											   OUT WCString& sFailMsg)
{
	WCString sQuery = "";
	INT      nRetVal = 0;
	SWC_BOOL bRet = TRUE;

	sQuery.Format("CALL PROC_CHECK_INSERT_USER_SYSTEM_MATCH('%s', '%s', '%s', '%s', @OUT_VALUE) ",
		sAgentID, sUserID, sCompanyID, sSharePC);

	pDBObj->ExecuteQuery(FALSE, sQuery);
	if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
	{
		_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
		_LOG_ERR("%s", LPCSTR(sQuery));
		sFailCode = PMX_ERROR_E10002;
		sFailMsg = pDBObj->GetLastErrorString();
		if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10002;
		bRet = FALSE;
	}
	else
	{
		CMyRSet* pRSet = pDBObj->ExecuteQuery(TRUE, "SELECT @OUT_VALUE AS RET_VALUE");
		if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
		{
			_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
			_LOG_ERR("%s", LPCSTR(sQuery));
			sFailCode = PMX_ERROR_E10003;
			sFailMsg = pDBObj->GetLastErrorString();
			if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10003;
			bRet = FALSE;
		}
		else
		{
			if (NULL != pRSet)
			{
				if (TRUE == pRSet->FetchRow())
				{
					nRetVal = pRSet->GetIntValue("RET_VALUE");
				}
			}
			else
			{
				_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
				_LOG_ERR("%s", LPCSTR(sQuery));
				sFailCode = PMX_ERROR_E10003;
				sFailMsg = pDBObj->GetLastErrorString();
				if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10003;
				bRet = FALSE;
			}
		}
		if(NULL != pRSet)
		{
			pRSet->Close();
			delete pRSet;
		}
	}

	if (1 != nRetVal)
	{
		bRet = FALSE;
		switch(nRetVal)
		{
			case -1: 
			{
				sFailCode = PMX_ERROR_E20002;
				sFailMsg  = PMX_ERROR_MSG_E20002;
				break;
			}
			case -2:
			{
				sFailCode = PMX_ERROR_E20001;
				sFailMsg  = PMX_ERROR_MSG_E20001;
				break;
			}
			case -3:
			{
				sFailCode = PMX_ERROR_E20001;
				sFailMsg  = PMX_ERROR_MSG_E20001;
				break;
			}
			case -4:
			{
				sFailCode = PMX_ERROR_E10004;
				sFailMsg  = PMX_ERROR_MSG_E10004;
				break;
			}
			default:
			{
				sFailCode = PMX_ERROR_E10003;
				sFailMsg  = PMX_ERROR_MSG_E10003;
				break;
			}
		}
	}

	return bRet;
}

SWC_BOOL CDBIO::ExtractKeepAliveData(IN CMySQLDBObj* pDBObj,
									 IN const CHAR* sAgentID,                      // 1. Agent ID                      
									 IN const CHAR* sUserID,                       // 2. User ID
									 IN const CHAR* sCompanyID,                    // 3. Company ID
									 IN const CHAR* sAgentTimeStamp,               // 4. agent applied last policy timestamp
									 IN const CHAR* sSharePC,                      // 5. share pc or not (YES/NO)
									 OUT WCString& sExpandCommmand,                // 6. extended command
									 OUT INT& nKeepAliveCycle,                     // 7. keep alive interval (seconds)
									 OUT INT& nLogCycle,                           // 8. log send interval (seconds)
									 OUT WCString& sAgentUpdate,                   // 9. PRIMX agent update or not (YES/NO)
									 OUT WCString& sZonecentralUpdate,             // 10. PRIMX ZoneCentral update or not (YES/NO)
									 OUT WCString& sOrizonUpdate,                  // 11. PRIMX Orizon update or not (YES/NO)
									 OUT WCString& sCryhodUpdate,                  // 12. PRIMX Cryhod update or not (YES/NO)
									 OUT WCString& sServerTime,                    // 13. current server time
									 OUT INT& nExportShelfLife,                    // 14. export file day limit (days)
									 OUT WCString& sOutCompanyID,                  // 15. out company ID
									 OUT WCString& sAuthServerAddr,                // 16. authentication server address
									 OUT INT& nAuthServerPort,                     // 17. authentication server port
									 OUT WCString& sProductMenuEnable,             // 18. product ment enable (YES/NO)
									 OUT WCString& sWebManagerConsoleMenuEnable,   // 19. web manager console menu enable (YES/NO)
									 OUT WCString& sWebmanagerConsoleURL,          // 20. web manager console url
									 OUT WCString& sExportMenuEnable,              // 21. export menu enable (YES/NO)
									 OUT WCString& sAuthWebManager,                // 22. file export request, confirm auth. (YES/NO)
									 OUT WCString& sAuthExportFile,                // 23. create export file auth. (YES/NO)
									 OUT WCString& sAuthNotEncryptionPath,         // 24. use not encryption path auth. (YES/NO)
									 OUT WCArray<WCString>& arrNotEncryptPathList, // 25. not encryption path list ('|' divied)
									 OUT WCString& sFailCode, 
									 OUT WCString& sFailMsg)
{
	WCString sQuery = "";
	SWC_BOOL bRet = TRUE;

	sQuery.Format("CALL PROC_GET_KEEPALIVE_RESPONSE('%s', '%s', '%s', '%s', '%s', ", sAgentID, sUserID, sCompanyID, sAgentTimeStamp, sSharePC);
	sQuery += "@OUT_EXPAND_COMMAND, ";
	sQuery += "@OUT_KEEPALIVE_CYCLE, ";
	sQuery += "@OUT_LOG_CYCLE, ";
	sQuery += "@OUT_AGENT_UPDATE, ";
	sQuery += "@OUT_ZONECENTRAL_UPDATE, ";
	sQuery += "@OUT_ORIZON_UPDATE, ";
	sQuery += "@OUT_CRYHOD_UPDATE, ";
	sQuery += "@OUT_SERVER_TIME, ";
	sQuery += "@OUT_EXPORT_SHELF_LIFE, ";
	sQuery += "@OUT_COMPANY_ID, ";
	sQuery += "@OUT_AUTH_SERVER_ADDR, ";
	sQuery += "@OUT_AUTH_SERVER_PORT, ";
	sQuery += "@OUT_PRODUCT_MENU_ENABLE, ";
	sQuery += "@OUT_WEBMANAGER_CONSOLE_MENU_ENABLE, ";
	sQuery += "@OUT_WEBMANAGER_CONSOLE_URL, ";
	sQuery += "@OUT_EXPORT_FILE_MENU_ENABLE,";
	sQuery += "@OUT_AUTH_WEB_MANAGER, ";
	sQuery += "@OUT_AUTH_EXPORT_FILE, ";
	sQuery += "@OUT_AUTH_NOT_ENCRYPTION_PATH, ";
	sQuery += "@OUT_AUTH_NOT_ENCRYPT_PATH_LIST) ";

	pDBObj->ExecuteQuery(FALSE, sQuery);
	if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
	{
		_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
		_LOG_ERR("%s", LPCSTR(sQuery));
		sFailCode = PMX_ERROR_E10002;
		sFailMsg = pDBObj->GetLastErrorString();
		if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10002;
		bRet = FALSE;
	}
	else
	{
		WCString sSelectQuery = "SELECT ";
		sSelectQuery += "@OUT_EXPAND_COMMAND                 AS EXPAND_COMMAND, ";
		sSelectQuery += "@OUT_KEEPALIVE_CYCLE                AS KEEPALIVE_CYCLE, ";
		sSelectQuery += "@OUT_LOG_CYCLE                      AS LOG_CYCLE, ";
		sSelectQuery += "@OUT_AGENT_UPDATE                   AS AGENT_UPDATE, ";
		sSelectQuery += "@OUT_ZONECENTRAL_UPDATE             AS ZONECENTRAL_UPDATE, ";
		sSelectQuery += "@OUT_ORIZON_UPDATE                  AS ORIZON_UPDATE, ";
		sSelectQuery += "@OUT_CRYHOD_UPDATE                  AS CRYHOD_UPDATE, ";
		sSelectQuery += "@OUT_SERVER_TIME                    AS SERVER_TIME, ";
		sSelectQuery += "@OUT_EXPORT_SHELF_LIFE              AS EXPORT_SHELF_LIFE, ";
		sSelectQuery += "@OUT_COMPANY_ID                     AS COMPANY_ID, ";
		sSelectQuery += "@OUT_AUTH_SERVER_ADDR               AS AUTH_SERVER_ADDR, ";
		sSelectQuery += "@OUT_AUTH_SERVER_PORT               AS AUTH_SERVER_PORT, ";
		sSelectQuery += "@OUT_PRODUCT_MENU_ENABLE            AS PRODUCT_MENU_ENABLE, ";
		sSelectQuery += "@OUT_WEBMANAGER_CONSOLE_MENU_ENABLE AS WEBMANAGER_CONSOLE_MENU_ENABLE, ";
		sSelectQuery += "@OUT_WEBMANAGER_CONSOLE_URL         AS WEBMANAGER_CONSOLE_URL, ";
		sSelectQuery += "@OUT_EXPORT_FILE_MENU_ENABLE        AS EXPORT_FILE_MENU_ENABLE,";
		sSelectQuery += "@OUT_AUTH_WEB_MANAGER               AS AUTH_WEB_MANAGER, ";
		sSelectQuery += "@OUT_AUTH_EXPORT_FILE               AS AUTH_EXPORT_FILE, ";
		sSelectQuery += "@OUT_AUTH_NOT_ENCRYPTION_PATH       AS AUTH_NOT_ENCRYPTION_PATH, ";
		sSelectQuery += "@OUT_AUTH_NOT_ENCRYPT_PATH_LIST     AS AUTH_NOT_ENCRYPT_PATH_LIST ";

		CMyRSet* pRSet = pDBObj->ExecuteQuery(TRUE, sSelectQuery);
		if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
		{
			_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
			_LOG_ERR("%s", LPCSTR(sQuery));
			sFailCode = PMX_ERROR_E10003;
			sFailMsg = pDBObj->GetLastErrorString();
			if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10003;
			bRet = FALSE;
		}
		else
		{
			if (NULL != pRSet)
			{
				if (TRUE == pRSet->FetchRow())
				{
					sExpandCommmand              = pRSet->GetStrValue("EXPAND_COMMAND");                 // 6
					nKeepAliveCycle              = pRSet->GetIntValue("KEEPALIVE_CYCLE");                // 7
					nLogCycle                    = pRSet->GetIntValue("LOG_CYCLE");                      // 8
					sAgentUpdate                 = pRSet->GetStrValue("AGENT_UPDATE");                   // 9
					sZonecentralUpdate           = pRSet->GetStrValue("ZONECENTRAL_UPDATE");             // 10
					sOrizonUpdate                = pRSet->GetStrValue("ORIZON_UPDATE");                  // 11
					sCryhodUpdate                = pRSet->GetStrValue("CRYHOD_UPDATE");                  // 12
					sServerTime                  = pRSet->GetStrValue("SERVER_TIME");                    // 13
					nExportShelfLife             = pRSet->GetIntValue("EXPORT_SHELF_LIFE");              // 14
					sOutCompanyID                = pRSet->GetStrValue("COMPANY_ID");                     // 15
					sAuthServerAddr              = pRSet->GetStrValue("AUTH_SERVER_ADDR");               // 16
					nAuthServerPort              = pRSet->GetIntValue("AUTH_SERVER_PORT");               // 17
					sProductMenuEnable           = pRSet->GetStrValue("PRODUCT_MENU_ENABLE");            // 18
					sWebManagerConsoleMenuEnable = pRSet->GetStrValue("WEBMANAGER_CONSOLE_MENU_ENABLE"); // 19
					sWebmanagerConsoleURL        = pRSet->GetStrValue("WEBMANAGER_CONSOLE_URL");         // 20
					sExportMenuEnable            = pRSet->GetStrValue("EXPORT_FILE_MENU_ENABLE");        // 21
					sAuthWebManager              = pRSet->GetStrValue("AUTH_WEB_MANAGER");               // 22
					sAuthExportFile              = pRSet->GetStrValue("AUTH_EXPORT_FILE");               // 23
					sAuthNotEncryptionPath       = pRSet->GetStrValue("AUTH_NOT_ENCRYPTION_PATH");       // 24
					WCString sNotEncPaths        = pRSet->GetStrValue("AUTH_NOT_ENCRYPT_PATH_LIST");     // 25

					if (0 < sNotEncPaths.GetLength())
					{
						std::stringstream sstream(sNotEncPaths.m_Str);
						std::string sCurPath = "";

						while (std::getline(sstream, sCurPath, '|'))
						{
							WCString sCur = "";
							sCur = sCurPath;
							arrNotEncryptPathList.Add(sCur);
						}
					}
				}
			}
			else
			{
				_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
				_LOG_ERR("%s", LPCSTR(sQuery));
				sFailCode = PMX_ERROR_E10003;
				sFailMsg = pDBObj->GetLastErrorString();
				if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10003;
				bRet = FALSE;
			}
		}
		if(NULL != pRSet)
		{
			pRSet->Close();
			delete pRSet;
		}
	}

	return bRet;
}

SWC_BOOL CDBIO::ExtractProductUpdateInfo(IN CMySQLDBObj* pDBObj, 
										 IN const CHAR* sAgentID, 
										 IN INT nProductType, 
										 IN const CHAR* sInstalledVersion, 
										 OUT WCString& sNeedPatchYN, 
										 OUT WCString& sCurrentProductVersion, 
										 OUT WCArray<WCString>& arrExServerURL, 
										 OUT WCString& sFilePath, 
										 OUT WCString& sFileHash, 
										 OUT WCString& sFailCode, 
										 OUT WCString& sFailMsg)
{
	WCString sQuery = "", sExServerURL = "";
	SWC_BOOL bRet = TRUE;

	sQuery.Format("CALL PROC_GET_PRODUCT_UPDATE_INFO('%s', %d, '%s', ", sAgentID, nProductType, sInstalledVersion);
	sQuery += "@OUT_NEED_PATCH_YN, ";
	sQuery += "@OUT_PRODUCT_VERSION, ";
	sQuery += "@OUT_EX_SERVER_URL, ";
	sQuery += "@OUT_FILE_PATH, ";
	sQuery += "@OUT_FILE_HASH) ";

	pDBObj->ExecuteQuery(FALSE, sQuery);
	if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
	{
		_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
		_LOG_ERR("%s", LPCSTR(sQuery));
		sFailCode = PMX_ERROR_E10002;
		sFailMsg = pDBObj->GetLastErrorString();
		if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10002;
		bRet = FALSE;
	}
	else
	{
		WCString sSelectQuery = "SELECT ";
		sSelectQuery += "@OUT_NEED_PATCH_YN    AS NEED_PATCH_YN, ";
		sSelectQuery += "@OUT_PRODUCT_VERSION  AS PRODUCT_VERSION, ";
		sSelectQuery += "@OUT_EX_SERVER_URL    AS EX_SERVER_URL, ";
		sSelectQuery += "@OUT_FILE_PATH        AS FILE_PATH, ";
		sSelectQuery += "@OUT_FILE_HASH        AS FILE_HASH ";

		CMyRSet* pRSet = pDBObj->ExecuteQuery(TRUE, sSelectQuery);
		if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
		{
			_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
			_LOG_ERR("%s", LPCSTR(sQuery));
			sFailCode = PMX_ERROR_E10003;
			sFailMsg = pDBObj->GetLastErrorString();
			if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10003;
			bRet = FALSE;
		}
		else
		{
			if (NULL != pRSet)
			{
				if (TRUE == pRSet->FetchRow())
				{
					sNeedPatchYN           = pRSet->GetStrValue("NEED_PATCH_YN");
					sCurrentProductVersion = pRSet->GetStrValue("PRODUCT_VERSION");
					sExServerURL           = pRSet->GetStrValue("EX_SERVER_URL");
					sFilePath              = pRSet->GetStrValue("FILE_PATH");
					sFileHash              = pRSet->GetStrValue("FILE_HASH");

					if (0 < sExServerURL.GetLength())
					{
						std::stringstream sstream(sExServerURL.m_Str);
						std::string sCurURL = "";

						while (std::getline(sstream, sCurURL, '|'))
						{
							WCString sCur = "";
							sCur = sCurURL;
							arrExServerURL.Add(sCur);
						}
					}
				}
			}
			else
			{
				_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
				_LOG_ERR("%s", LPCSTR(sQuery));
				sFailCode = PMX_ERROR_E10003;
				sFailMsg = pDBObj->GetLastErrorString();
				if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10003;
				bRet = FALSE;
			}
		}
		if(NULL != pRSet)
		{
			pRSet->Close();
			delete pRSet;
		}
	}

	return bRet;
}

SWC_BOOL CDBIO::ExtractAuthenticateExDecrypt(IN CMySQLDBObj* pDBObj, 
											 IN const CHAR* sUserID, 
											 IN const CHAR* sAgentID, 
											 IN const CHAR* sFileKey, 
											 IN const CHAR* sCompanyID, 
											 IN const CHAR* sFileHash, 
											 OUT WCString& sFailCode,
											 OUT WCString& sFailMsg)
{
	WCString sQuery = "", sResultCode = "";
	SWC_BOOL bRet = TRUE;

	sQuery.Format("CALL PROC_GET_EX_FILE_AUTH_INFO('%s', '%s', '%s', '%s', '%s', @OUT_RESULT_CODE) ",
		sUserID, sAgentID, sFileKey, sCompanyID, sFileHash);
	
	pDBObj->ExecuteQuery(FALSE, sQuery);
	if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
	{
		_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
		_LOG_ERR("%s", LPCSTR(sQuery));
		sFailCode = PMX_ERROR_E10004;
		sFailMsg = pDBObj->GetLastErrorString();
		if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10004;
		bRet = FALSE;
	}
	else
	{
		WCString sSelectQuery = "SELECT @OUT_RESULT_CODE AS RESULT_CODE ";

		CMyRSet* pRSet = pDBObj->ExecuteQuery(TRUE, sSelectQuery);
		if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
		{
			_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
			_LOG_ERR("%s", LPCSTR(sQuery));
			sFailCode = PMX_ERROR_E10003;
			sFailMsg = pDBObj->GetLastErrorString();
			if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10003;
			bRet = FALSE;
		}
		else
		{
			if (NULL != pRSet)
			{
				if (TRUE == pRSet->FetchRow())
				{
					sResultCode = pRSet->GetStrValue("RESULT_CODE");
					/*
					SUCCESS  
					E20001 / invalid value: user not found
					E20002 / invalid value: empty agent id
					E20003 / invalid value: empty user id
					E20005 / invalid value: empty file key
					E20006 / invalid value: empty company id
					E20007 / invalid value: empty file hash
					E40002 / check result: file key not found
					E40003 / check result: file hash mismatched
					E40004 / check result: expired file
					E40005 / check result: decryption limit exceeded
					E40006 / check result: user id mismatched
					E40007 / check result: agent id mismatched
					*/
					if     (sResultCode == PMX_ERROR_E20001) { sFailCode = PMX_ERROR_E20001; sFailMsg = PMX_ERROR_MSG_E20001; }
					else if(sResultCode == PMX_ERROR_E20002) { sFailCode = PMX_ERROR_E20002; sFailMsg = PMX_ERROR_MSG_E20002; }
					else if(sResultCode == PMX_ERROR_E20003) { sFailCode = PMX_ERROR_E20003; sFailMsg = PMX_ERROR_MSG_E20003; }
					else if(sResultCode == PMX_ERROR_E20005) { sFailCode = PMX_ERROR_E20005; sFailMsg = PMX_ERROR_MSG_E20005; }
					else if(sResultCode == PMX_ERROR_E20006) { sFailCode = PMX_ERROR_E20006; sFailMsg = PMX_ERROR_MSG_E20006; }
					else if(sResultCode == PMX_ERROR_E20007) { sFailCode = PMX_ERROR_E20007; sFailMsg = PMX_ERROR_MSG_E20007; }
					else if(sResultCode == PMX_ERROR_E40002) { sFailCode = PMX_ERROR_E40002; sFailMsg = PMX_ERROR_MSG_E40002; }
					else if(sResultCode == PMX_ERROR_E40003) { sFailCode = PMX_ERROR_E40003; sFailMsg = PMX_ERROR_MSG_E40003; }
					else if(sResultCode == PMX_ERROR_E40004) { sFailCode = PMX_ERROR_E40004; sFailMsg = PMX_ERROR_MSG_E40004; }
					else if(sResultCode == PMX_ERROR_E40005) { sFailCode = PMX_ERROR_E40005; sFailMsg = PMX_ERROR_MSG_E40005; }
					else if(sResultCode == PMX_ERROR_E40006) { sFailCode = PMX_ERROR_E40006; sFailMsg = PMX_ERROR_MSG_E40006; }
					else if(sResultCode == PMX_ERROR_E40007) { sFailCode = PMX_ERROR_E40007; sFailMsg = PMX_ERROR_MSG_E40007; }

					if (sResultCode != PMX_ERROR_SUCCESS)
					{
						bRet = FALSE;
					}
				}
			}
			else
			{
				_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
				_LOG_ERR("%s", LPCSTR(sQuery));
				sFailCode = PMX_ERROR_E10003;
				sFailMsg = pDBObj->GetLastErrorString();
				if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10003;
				bRet = FALSE;
			}
		}
		if(NULL != pRSet)
		{
			pRSet->Close();
			delete pRSet;
		}
	}

	return bRet;
}

SWC_BOOL CDBIO::ApplyAuthenticatedExDecrypt(IN CMySQLDBObj* pDBObj,
											IN const CHAR* sUserID,
											IN const CHAR* sAgentID,
											IN const CHAR* sFileKey,
											IN const CHAR* sCompanyID,
											IN const CHAR* sRequestIP,
											IN const CHAR* sRequestMAC, 
											IN const CHAR* sResultCode,
											IN const CHAR* sComment)
{
	WCString sQuery = "";
	SWC_BOOL bRet = TRUE;

	sQuery.Format("CALL PROC_APPLY_EX_FILE_AUTH_INFO('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s') ",
		sUserID, sAgentID, sCompanyID, sFileKey, sRequestIP, sRequestMAC, sResultCode, sComment);

	pDBObj->ExecuteQuery(FALSE, sQuery);
	if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
	{
		_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
		_LOG_ERR("%s", LPCSTR(sQuery));
		bRet = FALSE;
	}

	return bRet;
}

SWC_BOOL CDBIO::InsertExFileInfo(IN CMySQLDBObj* pDBObj, 
								 IN const CHAR* sUserID, 
								 IN const CHAR* sAgentID, 
								 IN const CHAR* sFileKey, 
								 IN const CHAR* sCompanyID, 
								 IN const CHAR* sFileHash, 
								 IN const CHAR* sFileName,
								 IN UINT64 lFileSize,
								 OUT WCString& sFailCode,
								 OUT WCString& sFailMsg)
{
	WCString sQuery = "", sResultCode = "";
	SWC_BOOL bRet = TRUE;

	sQuery.Format("CALL PROC_INSERT_T_EX_FILE_AUTH_INFO('%s', '%s', '%s', '%s', '%s', '%s', %lu, @OUT_RESULT_CODE) ",
		sUserID, sAgentID, sFileKey, sCompanyID, sFileHash, sFileName, lFileSize);
	
	pDBObj->ExecuteQuery(FALSE, sQuery);
	if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
	{
		_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
		_LOG_ERR("%s", LPCSTR(sQuery));
		sFailCode = PMX_ERROR_E10002;
		sFailMsg = pDBObj->GetLastErrorString();
		if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10002;
		bRet = FALSE;
	}
	else
	{
		WCString sSelectQuery = "SELECT @OUT_RESULT_CODE AS RESULT_CODE ";

		CMyRSet* pRSet = pDBObj->ExecuteQuery(TRUE, sSelectQuery);
		if (DB_ERR_SUCCESS != pDBObj->GetLastErrorNumber())
		{
			_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
			_LOG_ERR("%s", LPCSTR(sQuery));
			sFailCode = PMX_ERROR_E10003;
			sFailMsg = pDBObj->GetLastErrorString();
			if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10003;
			bRet = FALSE;
		}
		else
		{
			if (NULL != pRSet)
			{
				if (TRUE == pRSet->FetchRow())
				{
					sResultCode = pRSet->GetStrValue("RESULT_CODE");
					/*
					SUCCESS  
					E10002 / database error: transaction query failed
					E40008 / database error: transaction query failed
					*/
					if     (sResultCode == PMX_ERROR_E10002) { sFailCode = PMX_ERROR_E20001; sFailMsg = PMX_ERROR_MSG_E10002; }
					else if(sResultCode == PMX_ERROR_E40008) { sFailCode = PMX_ERROR_E20002; sFailMsg = PMX_ERROR_MSG_E40008; }

					if (sResultCode != PMX_ERROR_SUCCESS)
					{
						bRet = FALSE;
					}
				}
			}
			else
			{
				_LOG_ERR("query failed: (%ld)%s", pDBObj->GetDatabaseErrorNo(), pDBObj->GetLastErrorString());
				_LOG_ERR("%s", LPCSTR(sQuery));
				sFailCode = PMX_ERROR_E10003;
				sFailMsg = pDBObj->GetLastErrorString();
				if (0 == sFailMsg.GetLength()) sFailMsg = PMX_ERROR_MSG_E10003;
				bRet = FALSE;
			}
		}
		if(NULL != pRSet)
		{
			pRSet->Close();
			delete pRSet;
		}
	}

	return bRet;
}

//-----------------------------------------------------------------------------
#endif // __PMX_DB_IO_CPP__
