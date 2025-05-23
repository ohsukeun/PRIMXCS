/// @file pmx_agent_ctrl_log.cpp
/// @author Park, sung soo
/// @date 2023-07-17
/// agent log writter

#include <fstream>
#include <sstream>

#include <pmx_db_io.h>
#include <pmx_agent_ctrl_log.h>

#ifndef __PMX_AGENT_CTRL_LOG_CPP__
#define __PMX_AGENT_CTRL_LOG_CPP__

//-----------------------------------------------------------------------------
// METHOD DEFINITIONS
//-----------------------------------------------------------------------------
CAgentCtrlLog::CAgentCtrlLog()
{
	m_ThreadID            = 0;
	m_bRun                = FALSE;
	m_pDBObj              = NULL;
	time(&m_tPrevLogOperate);
	time(&m_tPrevRunChecked);
	m_nLogOperateInterval = 10;
	m_nRunCheckInterval   = (60 * 60);
	m_sModuleName         = "primx product log writter";
}

CAgentCtrlLog::~CAgentCtrlLog()
{
	Exit();
}

SWC_BOOL CAgentCtrlLog::Init(IN WCString sHost, IN USHORT nPort, IN WCString sUser, IN WCString sPwd, IN WCString sDBName)
{
	if (FALSE == ConnectToDatabase(sHost, nPort, sUser, sPwd, sDBName)) return FALSE;
	return TRUE;
}

SWC_BOOL CAgentCtrlLog::Run()
{
	SWC_BOOL bRet = TRUE;

	m_bRun = TRUE;
	if (0 > pthread_create(&m_ThreadID, NULL, LogWriteTreadFunc, this))
	{
		_LOG_ERR("primx log writter thread creating failed");
		m_bRun = FALSE;
		return FALSE;
	}
	return bRet;
}

VOID CAgentCtrlLog::Exit()
{
	m_bRun = FALSE;
	if (NULL != m_pDBObj)
	{
		delete m_pDBObj;
		m_pDBObj = NULL;
	}
	/*
	SWC::swc_sleep(0, 2000);
	if (0 != m_ThreadID)
	{
		INT nStatus = 0;
		pthread_join(m_ThreadID, (PVOID*)&nStatus);
		m_ThreadID = 0;
	}
	*/
}

SWC_BOOL CAgentCtrlLog::ConnectToDatabase(IN WCString sHost, IN USHORT nPort, IN WCString sUser, IN WCString sPwd, IN WCString sDBName)
{
	if (NULL == m_pDBObj) delete m_pDBObj;
	m_pDBObj = new CMySQLDBObj();

	m_pDBObj->Init(LPCSTR(sHost),
				   LPCSTR(sDBName),
				   "UTF8",
				   "UTF8",
				   LPCSTR(sUser),
				   LPCSTR(sPwd),
				   nPort);
	m_pDBObj->InitSSL(TRUE,
					  PMX_CU_MARIADB_CLIENT_KEY_PEM_FILE_PATH,
					  PMX_CU_MARIADB_CLIENT_CERT_PEM_FILE_PATH,
					  PMX_CU_MARIADB_CA_PEM_FILE_PATH,
					  NULL,
					  NULL);

	if (FALSE == m_pDBObj->Connect())
	{
		_LOG_ERR("database connection failed: (%d) %s", m_pDBObj->GetDatabaseErrorNo(), m_pDBObj->GetLastErrorString());
		delete m_pDBObj;
		m_pDBObj = NULL;
		return FALSE;
	}
	m_pDBObj->SetAutoCommit(FALSE); // auto-commit off

	return TRUE;
}

PVOID CAgentCtrlLog::LogWriteTreadFunc(PVOID pArg)
{
	CAgentCtrlLog* pObj = NULL;

	if (NULL == pArg)
	{
		_LOG_ERR("thread argument is null");
		return NULL;
	}
	pObj = (CAgentCtrlLog*)pArg;
	pObj->m_ThreadID = pthread_self();
	pthread_detach(pObj->m_ThreadID);

	while (TRUE == pObj->m_bRun)
	{
		pObj->OperateLogXml();
		pObj->RunCheck();
		SWC::swc_sleep(0, 1000);
	}

	pthread_exit((PVOID)NULL);
	return NULL;
}

VOID CAgentCtrlLog::RunCheck()
{
	if (NULL == m_pDBObj) return;

	time_t tCur = 0;
	time(&tCur);

	if (m_nRunCheckInterval <= abs(tCur - m_tPrevRunChecked))
	{
		CDBIO::RunSessionMngQuery(m_pDBObj, LPCSTR(m_sModuleName));
		m_tPrevRunChecked = tCur;
	}
}

VOID CAgentCtrlLog::OperateLogXml()
{
	WCArray<WCString> arrPath;
	time_t tCur = 0;
	time(&tCur);

	if (m_nLogOperateInterval <= abs(tCur - m_tPrevLogOperate))
	{
		if (0 < SWC::swc_get_matched_extension_files_in_dir(PMX_PATH_PRIMXSERVER_WAREHOUSE_TEMP, "xml", arrPath))
		{
			for(UINT nID = 0; nID < arrPath.GetCount(); nID++)
			{
				if(0 < arrPath[nID].GetLength())
				{
					OperateLogXmlFile(arrPath[nID]);
				}
			}
		}
		m_tPrevLogOperate = tCur;
	}
}

SWC_BOOL CAgentCtrlLog::OperateLogXmlFile(IN WCString sFilePath)
{
	SWC_BOOL bRet = TRUE;
	CHAR sFileName[SWC_MAX_PATH_LEN] = {0};
	WCString sFileContents = "";

	if (FALSE == SWC::swc_parse_file_name_from_path(LPCSTR(sFilePath), sFileName, SWC_MAX_PATH_LEN)) return FALSE;
	if (FALSE == SWC::swc_is_file_exists(PMX_PATH_PRIMXSERVER_WAREHOUSE_TEMP, sFileName)) return FALSE;

	// read whole file contents
	std::fstream hFile(LPCSTR(sFilePath), std::ifstream::in);
	hFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	try
	{
		std::stringstream strStream;

		strStream << hFile.rdbuf();
		sFileContents = strStream.str();
	}
	catch (std::ifstream::failure &e)
	{
		_LOG_ERR("xml file reading failed (%s)", e.what());
		return FALSE;
	}

	if (0 < sFileContents.GetLength())
	{
		CPmxXmlIO objXml(sFileContents);
		WCArray<WCString> arrQueries;
		WCString sError = "";

		objXml.MakeTransactionQueriesFromLogData(arrQueries);
		sFileContents.Empty();

		if (FALSE == ExecTransactionQueries(arrQueries, sError))
		{
			_LOG_ERR("agent id: %s, user id: %s, file: %s. log operating failed", 
				LPCSTR(objXml.GetAgentID()), LPCSTR(objXml.GetUserID()), LPCSTR(sFilePath));
			bRet = FALSE;
		}
	}

	// if successed, remove file
	if (TRUE == bRet)
	{
		_LOG_DBG("%s file operated", sFileName);
		unlink(LPCSTR(sFilePath));
	}
	// else, move to ERRLOGS directory
	else
	{
		_LOG_ERR("%s file operating failed", sFileName);

		WCString sMovePath = "";
		sMovePath.Format("%s/%s", PMX_PATH_PRIMXSERVER_WAREHOUSE_ERRLOGS, sFileName);
		rename(LPCSTR(sFilePath), LPCSTR(sMovePath));
	}

	return bRet;
}

SWC_BOOL CAgentCtrlLog::ExecTransactionQueries(IN WCArray<WCString>& arrQueries, OUT WCString& sError)
{
	SWC_BOOL bRet = TRUE;

	if (NULL == m_pDBObj) return FALSE;
	if (0 == arrQueries.GetCount()) return FALSE;

	for(UINT nID = 0; nID < arrQueries.GetCount(); nID++)
	{
		if (FALSE == CDBIO::RunTransaction(m_pDBObj, arrQueries[nID], sError))
		{
			m_pDBObj->Rollback();
			bRet = FALSE;
			break;
		}
	}
	if(TRUE == bRet) m_pDBObj->Commit();
	return bRet;
}

//-----------------------------------------------------------------------------
#endif // __PMX_AGENT_CTRL_LOG_CPP__
