/// @file pmx_agent_ctrl_svr.cpp
/// @author Park, sung soo
/// @date 2023-06-21
/// service main

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <pmx_conf_mgr.h>
#include <pmx_agent_ctrl_svr.h>

#ifndef __PMX_AGENT_CTRL_SVR_CPP__
#define __PMX_AGENT_CTRL_SVR_CPP__

//-----------------------------------------------------------------------------
// LOCAL DEFINITIONS
//-----------------------------------------------------------------------------
typedef struct _PMX_FILE_INFO
{
	CHAR   _name[NAME_MAX+1];
	time_t _ctime;
} PMX_FILE_INFO, *pPMX_FILE_INFO;
#define PMX_FILE_INFO_LEN   sizeof(PMX_FILE_INFO)

bool CompareObj(pPMX_FILE_INFO pPrev, pPMX_FILE_INFO pNext)
{
	return (bool)(pPrev->_ctime > pNext->_ctime);
}

//-----------------------------------------------------------------------------
// METHOD DEFINITIONS
//-----------------------------------------------------------------------------
CAgentCtrlSvr::CAgentCtrlSvr()
{
	m_pAgentCtrlHd       = NULL;
	m_pAgentCtrlLog      = NULL;
	m_nConcurrentSession = SOCK_MAX_CLIENT_CNT;
	m_nWorkThreadCount   = 5;
	m_nAgentCtrlPort     = PMX_DEFAULT_CTRL_PORT;
}

CAgentCtrlSvr::~CAgentCtrlSvr()
{
	Exit();
}

SWC_BOOL CAgentCtrlSvr::LoadConfig(OUT WCString& sAgentCtrlIP,
								   OUT USHORT&   nAgentCtrlPort,
								   OUT WCString& sDBAddr,
								   OUT USHORT&   nDBPort, 
								   OUT WCString& sDBUser,
								   OUT WCString& sDBPwd,
								   OUT WCString& sDBName)
{
	// load config. from file
	CPmxCUConfMgr* pConfMgr = new CPmxCUConfMgr();
	if (FALSE == pConfMgr->Load())
	{
		_LOG_ERR("config file reading failed");
		delete pConfMgr;
		return FALSE;
	}

	sAgentCtrlIP   = pConfMgr->GetNetworkCtrlSvrIP();
	nAgentCtrlPort = (USHORT)pConfMgr->GetNetworkCtrlSvrPort();
	sDBAddr        = pConfMgr->GetDatabaseHost();
	nDBPort        = (USHORT)pConfMgr->GetDatabasePort();
	sDBUser        = pConfMgr->GetDatabaseUser();
	sDBPwd         = pConfMgr->GetDatabasePwd();
	sDBName        = pConfMgr->GetDatabaseSchema();

	delete pConfMgr;

	return TRUE;
}

SWC_BOOL CAgentCtrlSvr::Init()
{
	SSockProperty attr;

	// remove core files. if exists
	WCString sCoreFileName = "";

	sCoreFileName.Format("%s.core", BIN_STR);
	DelCoreFiles(PMX_PATH_PRIMXSERVER_BIN, LPCSTR(sCoreFileName), 3);

	// load config.
	WCString sAgentCtrlIP = "", sDBAddr = "", sDBUser = "", sDBPwd = "", sDBName = "";
	USHORT   nAgentCtrlPort = 0, nDBPort = 0;

	if (FALSE == LoadConfig(sAgentCtrlIP, nAgentCtrlPort, sDBAddr, nDBPort, sDBUser, sDBPwd, sDBName))
	{
		return FALSE;
	}

	// init. agent control channel handler ------------------------------------
	m_pAgentCtrlHd = new CSSLHandler();

	sAgentCtrlIP.MakeUpper(); sAgentCtrlIP.Trim();
	m_nAgentCtrlPort = nAgentCtrlPort;

	// ssl init.
	SSL_library_init();
	SSLeay_add_ssl_algorithms();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	WF_SSL_init();

	// calc. work thread count
	CalcWorkThreadCount();

	// agent control session handler init.
	if (FALSE == m_pAgentCtrlHd->Init((sAgentCtrlIP == "ANY") ? NULL : LPCSTR(sAgentCtrlIP), 
									 m_nAgentCtrlPort, 
									 m_nConcurrentSession,
									 S_PMX_AGENT_CMD_HD_LEN,
									 m_nWorkThreadCount,
									 PMX_CU_SERVER_CERT_INFO_PATH, 
									 PMX_CU_SERVER_CA_LIST_PATH, 
									 PMX_SERVER_SSL_PASSWORD))
	{
		_LOG_ERR("agent control session handler init. failed");
		return FALSE;
	}

	m_pAgentCtrlHd->m_sModuleName = "primx-cs agent control session handler";
	
	// set attribute
	memset(&attr, 0x00, sizeof(SSockProperty));
	attr._OptFlag = SWC_SYSTEM_CHECK_ENABLE | SWC_SOCK_ENABLE_IO_TIMEOUT;
	attr._IOTimeout = (60 * 2); // each two minutes
	attr._SysChkItvl = (60 * 60); // each one hour
	m_pAgentCtrlHd->SetProperties(&attr);

	// init. work threads
	INT nID = 0;
	for (nID = 0; nID < m_pAgentCtrlHd->GetWorkThCount(); nID++)
	{
		CAgentCtrlIO* pNode = new CAgentCtrlIO();

		if (FALSE == pNode->ConnectToDatabase(sDBAddr, nDBPort, sDBUser, sDBPwd, sDBName))
		{
			_LOG_ERR("work thread[%d]: can not connect to database", nID);
			return FALSE;
		}
		else
		{
			m_pAgentCtrlHd->m_pWorkTh[nID] = pNode;
			m_pAgentCtrlHd->m_pWorkTh[nID]->Init(m_pAgentCtrlHd);
		}
	}
	//-------------------------------------------------------------------------
	
	// init. primx product log writter ----------------------------------------
	m_pAgentCtrlLog = new CAgentCtrlLog();

	if (FALSE == m_pAgentCtrlLog->Init(sDBAddr, nDBPort, sDBUser, sDBPwd, sDBName))
	{
		_LOG_ERR("primx log writter: can not connect to database");
		return FALSE;
	}
	//-------------------------------------------------------------------------

	return TRUE;
}

SWC_BOOL CAgentCtrlSvr::Run()
{
	_LOG_DBG("enter ...");

	if (FALSE == m_pAgentCtrlHd->Run())
	{
		_LOG_ERR("%s running failed", LPCSTR(m_pAgentCtrlHd->m_sModuleName));
		return FALSE;
	}
	m_pAgentCtrlHd->SetEnableClientAccept();

	if (FALSE == m_pAgentCtrlLog->Run())
	{
		_LOG_ERR("primx log writter running failed");
		return FALSE;
	}

	_LOG_MSG("%s started", LPCSTR(m_pAgentCtrlHd->m_sModuleName));

	return TRUE;
}

VOID CAgentCtrlSvr::Exit()
{
	if (NULL != m_pAgentCtrlHd)
	{
		delete m_pAgentCtrlHd;
		m_pAgentCtrlHd = NULL;
	}
	WF_SSL_exit();

	if (NULL != m_pAgentCtrlLog)
	{
		delete m_pAgentCtrlLog;
		m_pAgentCtrlLog = NULL;
	}
}

VOID CAgentCtrlSvr::DelCoreFiles(IN const CHAR* sDir, IN const CHAR* sFind, IN UINT nRemain)
{
	DIR*           pDir = NULL;
	struct dirent* pEntry = NULL;
	struct stat    statBuf;
	CHAR           sOrgPath[SWC_MAX_PATH_LEN] = {0};
	UINT           nID = 0, nTotal = 0;
	WCArray<pPMX_FILE_INFO> arrFiles;

	// extract core file lists
	getcwd(sOrgPath, SWC_MAX_PATH_LEN);
	if (NULL == (pDir = opendir(sDir)))
	{
		_LOG_ERR("can not open directory: %s", sDir);
		return;
	}
	chdir(sDir);
	while(NULL != (pEntry = (readdir(pDir))))
	{
		stat(pEntry->d_name, &statBuf);
		if (0 == S_ISDIR(statBuf.st_mode))
		{
			WCString sFile(pEntry->d_name);

			if(0 < sFile.Find(sFind))
			{
				pPMX_FILE_INFO pInfo = (pPMX_FILE_INFO)malloc(PMX_FILE_INFO_LEN);

				snprintf(pInfo->_name, NAME_MAX+1, "%s", pEntry->d_name);
				arrFiles.Add(pInfo);
			}
		}
	}
	closedir(pDir);
	chdir(sOrgPath);

	// check and remove
	nTotal = arrFiles.GetCount();
	if (nTotal > nRemain)
	{
		sort(arrFiles.m_Array.begin(), arrFiles.m_Array.end(), CompareObj);

		for (nID = 0; nID < (nTotal - nRemain); nID++)
		{
			WCString sPath, sTemp;
			pPMX_FILE_INFO pCur = arrFiles[nID];
			sPath.Format("%s/%s", sDir, pCur->_name);
			unlink(LPCSTR(sPath));
		}
	}

	for (nID = 0; nID < nTotal; nID++)
	{
		pPMX_FILE_INFO pDel = arrFiles[nID];
		if (NULL != pDel) free(pDel);
	}
	arrFiles.RemoveAll();
}

VOID CAgentCtrlSvr::CalcWorkThreadCount()
{
	UINT nCpu = SWC::swc_get_cpu_count();

	if (1 >= nCpu) m_nWorkThreadCount = SOCK_MAX_WORK_TH_CNT;
	else           m_nWorkThreadCount = nCpu * 2 + 2;

	_LOG_MSG("work thread count: %u", m_nWorkThreadCount);
}

//-----------------------------------------------------------------------------
#endif // __PMX_AGENT_CTRL_SVR_CPP__
