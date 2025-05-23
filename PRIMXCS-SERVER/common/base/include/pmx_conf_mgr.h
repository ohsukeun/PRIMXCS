/// @file pmx_conf_mgr.h
/// @author Park, sung soo
/// @date 202i3-06-12
/// @brief primx-cs configuration file i/o class

/*
<?xml version="1.0" encoding="UTF-8" ?>
<configuration>
	<database type="mariadb">
		<host>127.0.0.1</host>
		<port>53306</port>
		<username>root</username>
		<password>primxadmin1234~!</password>
		<schema>PRIMXDB</schema>
	</database>
	<network>
		<ctrl_server>
			<ip>127.0.0.1</ip>
			<port>50101</port>
		</ctrl_server>
		<update_server>
			<ip>127.0.0.1</ip>
			<port>50103</port>
		</update_server>
	</network>
</configuration>

<?xml version="1.0" encoding="UTF-8" ?>
<configuration>
	<database type="mariadb">
		<host>127.0.0.1</host>
		<port>53306</port>
		<username>root</username>
		<password>primxadmin1234~!</password>
		<schema>PRIMXDB</schema>
	</database>
	<network>
		<auth_server>
			<ip>127.0.0.1</ip>
			<port>50102</port>
		</auth_server>
	</network>
</configuration>
*/

#include <fstream>
#include <sstream>

#include <ARIAEncryption.h>
#include <Markup.h>
#include <pmx_xml.h>

#ifndef __PMX_CONF_MGR_H__
#define __PMX_CONF_MGR_H__
//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

// XML elements
#define PMX_XML_ELEM_AUTH_SERVER                "auth_server"
#define PMX_XML_ELEM_CONFIGURATION              "configuration"
#define PMX_XML_ELEM_CTRL_SERVER                "ctrl_server"
#define PMX_XML_ELEM_DATABASE                   "database"
#define PMX_XML_ELEM_HOST                       "host"
#define PMS_XML_ELEM_IP                         "ip"
#define PMX_XML_ELEM_NETWORK                    "network"
#define PMX_XML_ELEM_PASSWORD                   "password"
#define PMX_XML_ELEM_PORT                       "port"
#define PMX_XML_ELEM_SCHEMA                     "schema"
#define PMX_XML_ELEM_UPDATE_SERVER              "update_server"
#define PMX_XML_ELEM_USERNAME                   "username"

// XML attributes
#define PMX_XML_ATTR_TYPE                       "type"

// XML paths
#define PMX_XML_PATH_DATABASE_HOST              "/configuration/database/host"
#define PMX_XML_PATH_DATABASE_PORT              "/configuration/database/port"
#define PMX_XML_PATH_DATABASE_USERNAME          "/configuration/database/username"
#define PMX_XML_PATH_DATABASE_PASSWORD          "/configuration/database/password"
#define PMX_XML_PATH_DATABASE_SCHEMA            "/configuration/database/schema"
#define PMX_XML_PATH_NETWORK_CTRL_SERVER_IP     "/configuration/network/ctrl_server/ip"
#define PMX_XML_PATH_NETWORK_CTRL_SERVER_PORT   "/configuration/network/ctrl_server/port"
#define PMX_XML_PATH_NETWORK_AUTH_SERVER_IP     "/configuration/network/auth_server/ip"
#define PMX_XML_PATH_NETWORK_AUTH_SERVER_PORT   "/configuration/network/auth_server/port"
#define PMX_XML_PATH_NETWORK_UPDATE_SERVER_IP   "/configuration/network/update_server/ip"
#define PMX_XML_PATH_NETWORK_UPDATE_SERVER_PORT "/configuration/network/update_server/port"

/// PRIMX-CS configuration file I/O class
class CPmxConfMgr : public CPmxXml
{
public:
	SWC_BOOL Load(IN WCString sDir, WCString sFileName);
	SWC_BOOL Save();

	WCString GetStrValueByPath (IN const CHAR* sPath);
	WCString GetDatabaseHost   ();
	UINT     GetDatabasePort   ();
	WCString GetDatabasePortStr();
	WCString GetDatabaseUser   ();
	WCString GetDatabasePwd    ();
	WCString GetDatabaseSchema ();

	SWC_BOOL SetStrValueByPath(IN const CHAR* sPath, const WCString sValue);
	SWC_BOOL SetDatabaseHost  (IN const WCString sHost);
	SWC_BOOL SetDatabasePort  (IN const WCString sPort);
	SWC_BOOL SetDatabaseUser  (IN const WCString sUser);
	SWC_BOOL SetDatabasePwd   (IN const WCString sPwd);
	SWC_BOOL SetDatabaseSchema(IN const WCString sSchema);

	CPmxConfMgr();
	virtual ~CPmxConfMgr();

private:
	WCString GetFileContents();
	SWC_BOOL SaveFileContents();

public:
	WCString m_sFilePath;
};

/// Control and update server configuration file I/O class
class CPmxCUConfMgr : public CPmxConfMgr
{
public:
	SWC_BOOL Load();

	WCString GetNetworkCtrlSvrIP       ();
	UINT     GetNetworkCtrlSvrPort     ();
	WCString GetNetworkCtrlSvrPortStr  ();
	WCString GetNetworkUpdateSvrIP     ();
	UINT     GetNetworkUpdateSvrPort   ();
	WCString GetNetworkUpdateSvrPortStr();

	SWC_BOOL SetNetworkCtrlSvrIP    (IN const WCString sIP);
	SWC_BOOL SetNetworkCtrlSvrPort  (IN const WCString sPort);
	SWC_BOOL SetNetworkUpdateSvrIP  (IN const WCString sIP);
	SWC_BOOL SetNetworkUpdateSvrPort(IN const WCString sPort);

	CPmxCUConfMgr();
	~CPmxCUConfMgr();

private:
	VOID SetDefaultXml();
};

/// Authentication server configuration file I/O class
class CPmxAConfMgr : public CPmxConfMgr
{
public:
	SWC_BOOL Load();

	WCString GetNetworkAuthSvrIP     ();
	UINT     GetNetworkAuthSvrPort   ();
	WCString GetNetworkAuthSvrPortStr();

	SWC_BOOL SetNetworkAuthSvrIP  (IN const WCString sIP);
	SWC_BOOL SetNetworkAuthSvrPort(IN const WCString sPort);

	CPmxAConfMgr();
	~CPmxAConfMgr();

private:
	VOID SetDefaultXml();
};

//-----------------------------------------------------------------------------
#endif // __PMX_CONF_MGR_H__
