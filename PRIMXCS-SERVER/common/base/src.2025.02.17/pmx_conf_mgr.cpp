/// @file prm_conf_mgr.cpp
/// @author Park, sung soo
/// @date 202i3-06-12
/// @brief primx-cs configuration file i/o class

#include <pmx_conf_mgr.h>

#ifndef __PMX_CONF_MGR_CPP__
#define __PMX_CONF_MGR_CPP__

//-----------------------------------------------------------------------------
// LOCAL DEFINITION
//-----------------------------------------------------------------------------
#define __CATCH__ \
catch(std::exception& e) \
{ \
	_LOG_ERR("%s", e.what()); \
}

#define __CATCH_RETURN__(X) \
catch(std::exception& e) \
{ \
	_LOG_ERR("%s", e.what()); \
	return X; \
}

//-----------------------------------------------------------------------------
// METHOS DEFINITION
//-----------------------------------------------------------------------------
CPmxConfMgr::CPmxConfMgr()
{
	m_sFilePath = "";
}

CPmxConfMgr::~CPmxConfMgr()
{

}

SWC_BOOL CPmxConfMgr::Load(IN WCString sDir, IN WCString sFileName)
{
	try
	{
		if (FALSE == SWC::swc_is_file_exists(sDir, sFileName))
		{
			return FALSE;
		}

		// read whole file contents
		WCString sFilePath = "", sEnc = "", sDec = "";
		sFilePath = sDir; 
		sFilePath += "/"; 
		sFilePath += sFileName;

		std::fstream hFile(LPCSTR(sFilePath), std::ifstream::in);
		hFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
		try
		{
			std::stringstream strStream;

			strStream << hFile.rdbuf();
			sEnc = strStream.str();
		}
		catch (std::ifstream::failure &e)
		{
			_LOG_ERR("xml file reading failed (%s)", e.what());
			return FALSE;
		}

		// decrypt encrypted file contents
		CARIAEncryption* pAria = new CARIAEncryption();
		sDec = pAria->DecryptString(sEnc);
		if (NULL != pAria) delete pAria;

		// set string to xml parser
		if (FALSE == SetXml(sDec))
		{
			_LOG_ERR("xml file parsing failed (%s)", GetLastErrStr());
			return FALSE;
		}

		m_sFilePath = sFilePath;
	}
	__CATCH_RETURN__(FALSE)
	return TRUE;
}

SWC_BOOL CPmxConfMgr::Save()
{
	try
	{
		// extract xml string
		WCString sXml = ToStr(), sEnc = "";

		// encrypt xml string
		CARIAEncryption* pAria = new CARIAEncryption();
		sEnc = pAria->EncryptString(sXml);
		if (NULL != pAria) delete pAria;

		// write to file
		std::ofstream hFile;
		hFile.open(LPCSTR(m_sFilePath), std::ios::trunc);

		if (true == hFile.is_open())
		{
			hFile << sEnc.m_Str;
			hFile.close();
		}
	}
	__CATCH_RETURN__(FALSE)
	return TRUE;
}

WCString CPmxConfMgr::GetStrValueByPath(IN const CHAR* sPath)
{
	WCString sRet = "";
	sRet = GetElmValueStrByPath(sPath);
	if (PMX_ERR_XML_RW_SUCCESS != GetLastErrNo())
	{
		_LOG_ERR("element %s data extraction failed. %s", sPath, GetLastErrStr());
		sRet = "";
		return sRet;
	}
	return sRet;
}

WCString CPmxConfMgr::GetDatabaseHost()
{
	return GetStrValueByPath(PMX_XML_PATH_DATABASE_HOST);
}

UINT CPmxConfMgr::GetDatabasePort()
{
	WCString sPort = "";
	
	sPort = GetStrValueByPath(PMX_XML_PATH_DATABASE_PORT);
	if (0 == sPort.GetLength()) return 0;
	return (UINT)atoi(LPCSTR(sPort));
}

WCString CPmxConfMgr::GetDatabasePortStr()
{
	return GetStrValueByPath(PMX_XML_PATH_DATABASE_PORT);
}

WCString CPmxConfMgr::GetDatabaseUser()
{
	return GetStrValueByPath(PMX_XML_PATH_DATABASE_USERNAME);
}

WCString CPmxConfMgr::GetDatabasePwd()
{
	return GetStrValueByPath(PMX_XML_PATH_DATABASE_PASSWORD);
}

WCString CPmxConfMgr::GetDatabaseSchema()
{
	return GetStrValueByPath(PMX_XML_PATH_DATABASE_SCHEMA);
}

SWC_BOOL CPmxConfMgr::SetStrValueByPath(IN const CHAR* sPath, const WCString sValue)
{
	SetElmValueStrByPath(sPath, sValue);
	if (PMX_ERR_XML_RW_SUCCESS != GetLastErrNo())
	{
		_LOG_ERR("element %s data setting failed. %s", sPath, GetLastErrStr());
		return FALSE;
	}
	return TRUE;
}

SWC_BOOL CPmxConfMgr::SetDatabaseHost(IN const WCString sHost)
{
	return SetStrValueByPath(PMX_XML_PATH_DATABASE_HOST, sHost);
}

SWC_BOOL CPmxConfMgr::SetDatabasePort(IN const WCString sPort)
{
	return SetStrValueByPath(PMX_XML_PATH_DATABASE_PORT, sPort);
}

SWC_BOOL CPmxConfMgr::SetDatabaseUser(IN const WCString sUser)
{
	return SetStrValueByPath(PMX_XML_PATH_DATABASE_USERNAME, sUser);
}

SWC_BOOL CPmxConfMgr::SetDatabasePwd(IN const WCString sPwd)
{
	return SetStrValueByPath(PMX_XML_PATH_DATABASE_PASSWORD, sPwd);
}

SWC_BOOL CPmxConfMgr::SetDatabaseSchema(IN const WCString sSchema)
{
	return SetStrValueByPath(PMX_XML_PATH_DATABASE_SCHEMA, sSchema);
}

CPmxCUConfMgr::CPmxCUConfMgr()
{

}

CPmxCUConfMgr::~CPmxCUConfMgr()
{

}

SWC_BOOL CPmxCUConfMgr::Load()
{
	if (FALSE == CPmxConfMgr::Load(PMX_CU_CONF_FILE_DIR, PMX_CU_CONF_FILE_NAME))
	{
		SetDefaultXml();
		Save();
	}
	return TRUE;
}

VOID CPmxCUConfMgr::SetDefaultXml()
{
	WCString sDefaultXml = "";
	sDefaultXml = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n\
<configuration>\n\
	<database type=\"mariadb\">\n\
		<host>127.0.0.1</host>\n\
		<port>53306</port>\n\
		<username>primx_ctrl</username>\n\
		<password>primxuser1234~!</password>\n\
		<schema>PRIMXDB</schema>\n\
	</database>\n\
	<network>\n\
		<ctrl_server>\n\
			<ip>127.0.0.1</ip>\n\
			<port>50101</port>\n\
		</ctrl_server>\n\
		<update_server>\n\
			<ip>127.0.0.1</ip>\n\
			<port>50102</port>\n\
		</update_server>\n\
	</network>\n\
</configuration>";

	// set string to xml parser
	if (FALSE == SetXml(sDefaultXml))
	{
		_LOG_ERR("xml file parsing failed (%s)", GetLastErrStr());
		return;
	}

	m_sFilePath = PMX_CU_CONF_FILE_DIR;
	m_sFilePath += "/";
	m_sFilePath += PMX_CU_CONF_FILE_NAME;
}

WCString CPmxCUConfMgr::GetNetworkCtrlSvrIP()
{
	return GetStrValueByPath(PMX_XML_PATH_NETWORK_CTRL_SERVER_IP);
}

UINT CPmxCUConfMgr::GetNetworkCtrlSvrPort()
{
	WCString sPort = "";
	
	sPort = GetStrValueByPath(PMX_XML_PATH_NETWORK_CTRL_SERVER_PORT);
	if (0 == sPort.GetLength()) return 0;
	return (UINT)atoi(LPCSTR(sPort));
}

WCString CPmxCUConfMgr::GetNetworkCtrlSvrPortStr()
{
	return GetStrValueByPath(PMX_XML_PATH_NETWORK_CTRL_SERVER_PORT);
}

WCString CPmxCUConfMgr::GetNetworkUpdateSvrIP()
{
	return GetStrValueByPath(PMX_XML_PATH_NETWORK_UPDATE_SERVER_IP);
}

UINT CPmxCUConfMgr::GetNetworkUpdateSvrPort()
{
	WCString sPort = "";
	
	sPort = GetStrValueByPath(PMX_XML_PATH_NETWORK_UPDATE_SERVER_PORT);
	if (0 == sPort.GetLength()) return 0;
	return (UINT)atoi(LPCSTR(sPort));
}

WCString CPmxCUConfMgr::GetNetworkUpdateSvrPortStr()
{
	return GetStrValueByPath(PMX_XML_PATH_NETWORK_UPDATE_SERVER_PORT);
}

SWC_BOOL CPmxCUConfMgr::SetNetworkCtrlSvrIP(IN const WCString sIP)
{
	return SetStrValueByPath(PMX_XML_PATH_NETWORK_CTRL_SERVER_IP, sIP);
}

SWC_BOOL CPmxCUConfMgr::SetNetworkCtrlSvrPort(IN const WCString sPort)
{
	return SetStrValueByPath(PMX_XML_PATH_NETWORK_CTRL_SERVER_PORT, sPort);
}

SWC_BOOL CPmxCUConfMgr::SetNetworkUpdateSvrIP(IN const WCString sIP)
{
	return SetStrValueByPath(PMX_XML_PATH_NETWORK_UPDATE_SERVER_IP, sIP);
}

SWC_BOOL CPmxCUConfMgr::SetNetworkUpdateSvrPort(IN const WCString sPort)
{
	return SetStrValueByPath(PMX_XML_PATH_NETWORK_UPDATE_SERVER_PORT, sPort);
}

CPmxAConfMgr::CPmxAConfMgr()
{

}

CPmxAConfMgr::~CPmxAConfMgr()
{

}

SWC_BOOL CPmxAConfMgr::Load()
{
	if (FALSE == CPmxConfMgr::Load(PMX_A_CONF_FILE_DIR, PMX_A_CONF_FILE_NAME))
	{
		SetDefaultXml();
		Save();
	}
	return TRUE;
}

VOID CPmxAConfMgr::SetDefaultXml()
{
	WCString sDefaultXml = "";
	sDefaultXml = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n\
<configuration>\n\
	<database type=\"mariadb\">\n\
		<host>127.0.0.1</host>\n\
		<port>53306</port>\n\
		<username>primx_auth</username>\n\
		<password>primxuser1234~!</password>\n\
		<schema>PRIMXDB</schema>\n\
	</database>\n\
	<network>\n\
		<auth_server>\n\
			<ip>127.0.0.1</ip>\n\
			<port>50103</port>\n\
		</auth_server>\n\
	</network>\n\
</configuration>";

	// set string to xml parser
	if (FALSE == SetXml(sDefaultXml))
	{
		_LOG_ERR("xml file parsing failed (%s)", GetLastErrStr());
		return;
	}

	m_sFilePath = PMX_A_CONF_FILE_DIR;
	m_sFilePath += "/";
	m_sFilePath += PMX_A_CONF_FILE_NAME;
}

WCString CPmxAConfMgr::GetNetworkAuthSvrIP()
{
	return GetStrValueByPath(PMX_XML_PATH_NETWORK_AUTH_SERVER_IP);
}

UINT CPmxAConfMgr::GetNetworkAuthSvrPort()
{
	WCString sPort = "";
	
	sPort = GetStrValueByPath(PMX_XML_PATH_NETWORK_AUTH_SERVER_PORT);
	if (0 == sPort.GetLength()) return 0;
	return (UINT)atoi(LPCSTR(sPort));
}

WCString CPmxAConfMgr::GetNetworkAuthSvrPortStr()
{
	return GetStrValueByPath(PMX_XML_PATH_NETWORK_AUTH_SERVER_PORT);
}

SWC_BOOL CPmxAConfMgr::SetNetworkAuthSvrIP(IN const WCString sIP)
{
	return SetStrValueByPath(PMX_XML_PATH_NETWORK_AUTH_SERVER_IP, sIP);
}

SWC_BOOL CPmxAConfMgr::SetNetworkAuthSvrPort(IN const WCString sPort)
{
	return SetStrValueByPath(PMX_XML_PATH_NETWORK_AUTH_SERVER_PORT, sPort);
}

//-----------------------------------------------------------------------------
#endif // __PMX_CONF_MGR_CPP__
