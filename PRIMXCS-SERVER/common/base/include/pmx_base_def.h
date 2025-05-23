/// @file xma_base_def.h
/// @author Park, sung soo
/// @date 2023-06-16
/// @brief migration engine base definitions

#include <error.h>
#include <assert.h>

#include <logger.h>
#include <swc.h>

#include <pmx_error.h>

#ifndef __PMX_BASE_DEF_H__
#define __PMX_BASE_DEF_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
#define PMX_DEFAULT_CTRL_PORT     50101
#define PMX_DEFAULT_UPDATE_PORT   50102
#define PMX_DEFAULT_AUTH_PORT     50103
#define PMX_DEFAULT_DB_PORT       53306

// user and group
#define PMX_USER_GROUP       "primxsrvgp"
#define PMX_USER_SVR         "primxsrv"
#define PMX_USER_WEB         "primxweb"

#define PMX_ROOT_GROUP       "root"
#define PMX_USER_ROOT        "root"

// directories
#define PMX_DIR_ACCESSFILES  "ACCESSFILES"
#define PMX_DIR_BIN          "BIN"
#define PMX_DIR_CACHE        "CACHE"
#define PMX_DIR_CERTS        "CERTS"
#define PMX_DIR_CONF         "CONF"
#define PMX_DIR_CRYHOD       "CRYHOD"
#define PMX_DIR_KEYMAP       "KEYMAP"
#define PMX_DIR_LIB          "LIB"
#define PMX_DIR_LOGS         "LOGS"
#define PMX_DIR_ORIZON       "ORIZON"
#define PMX_DIR_PACKAGE      "PACKAGE"
#define PMX_DIR_POLICIES     "POLICIES"
#define PMX_DIR_POLICY       "POLICY"
#define PMX_DIR_POLICYSCRIPT "POLICYSCRIPT"
#define PMX_DIR_PRIMXSERVER  "PRIMXSERVER"
#define PMX_DIR_PRODUCT      "PRODUCT"
#define PMX_DIR_PROTECTION   "PROTECTION"
#define PMX_DIR_WEBSRV       "WEBSRV"
#define PMX_DIR_ZONECENTRAL  "ZONECENTRAL"
#define PMX_DIR_ZONEMAP      "ZONEMAP"

// paths
#define PMX_PATH_PRIMXSERVER                                         "/PRIMX/PRIMXSYSTEM"
#define PMX_PATH_PRIMXSERVER_BIN                                     "/PRIMX/PRIMXSYSTEM/BIN"
#define PMX_PATH_PRIMXSERVER_CONF                                    "/PRIMX/PRIMXSYSTEM/CONF"
#define PMX_PATH_PRIMXSERVER_CONF_CERTS                              "/PRIMX/PRIMXSYSTEM/CONF/CERTS"
#define PMX_PATH_PRIMXSERVER_CONF_CERTS_MARIADB_CLIENT               "/PRIMX/PRIMXSYSTEM/CONF/CERTS/MARIADB_CLIENT"
#define PMX_PATH_PRIMXSERVER_CONF_CERTS_SERVER                       "/PRIMX/PRIMXSYSTEM/CONF/CERTS/SERVER"
#define PMX_PATH_PRIMXSERVER_LIB                                     "/PRIMX/PRIMXSYSTEM/LIB"
#define PMX_PATH_PRIMXSERVER_LOGS                                    "/PRIMX/PRIMXSYSTEM/LOGS"
#define PMX_PATH_PRIMXSERVER_WAREHOUSE                               "/PRIMX/PRIMXSYSTEM/WAREHOUSE"
#define PMX_PATH_PRIMXSERVER_WAREHOUSE_DBBACKUP                      "/PRIMX/PRIMXSYSTEM/WAREHOUSE/DBBACKUP"
#define PMX_PATH_PRIMXSERVER_WAREHOUSE_TEMP                          "/PRIMX/PRIMXSYSTEM/WAREHOUSE/TEMP"
#define PMX_PATH_PRIMXSERVER_WAREHOUSE_ERRLOGS                       "/PRIMX/PRIMXSYSTEM/WAREHOUSE/ERRLOGS"
#define PMX_PATH_PRIMXSERVER_WEBSRV                                  "/PRIMX/PRIMXSYSTEM/WEBSRV"
#define PMX_PATH_PRIMXSERVER_WEBSRV                                  "/PRIMX/PRIMXSYSTEM/WEBSRV"
#define PMX_PATH_PRIMXSERVER_WEBSRV                                  "/PRIMX/PRIMXSYSTEM/WEBSRV"
#define PMX_PATH_PRIMXSERVER_PRODUCT                                 "/PRIMX/PRIMXSYSTEM/PRODUCT"
#define PMX_PATH_PRIMXSERVER_PRODUCT_ACCESSFILES                     "/PRIMX/PRIMXSYSTEM/PRODUCT/ACCESSFILES"
#define PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_POLICYSCRIPT           "/PRIMX/PRIMXSYSTEM/PRODUCT/POLICIES/POLICYSCRIPT"
#define PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_ZONECENTRAL            "/PRIMX/PRIMXSYSTEM/PRODUCT/POLICIES/ZONECENTRAL"
#define PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_ZONECENTRAL_PROTECTION "/PRIMX/PRIMXSYSTEM/PRODUCT/POLICIES/ZONECENTRAL/PROTECTION"
#define PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_ZONECENTRAL_POLICY     "/PRIMX/PRIMXSYSTEM/PRODUCT/POLICIES/ZONECENTRAL/POLICY"
#define PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_ORIZON                 "/PRIMX/PRIMXSYSTEM/PRODUCT/POLICIES/ORIZON"
#define PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_ORIZON_PROTECTION      "/PRIMX/PRIMXSYSTEM/PRODUCT/POLICIES/ORIZON/PROTECTION"
#define PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_ORIZON_POLICY          "/PRIMX/PRIMXSYSTEM/PRODUCT/POLICIES/ORIZON/POLICY"
#define PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_CRYHOD                 "/PRIMX/PRIMXSYSTEM/PRODUCT/POLICIES/CRYHOD"
#define PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_CRYHOD_PROTECTION      "/PRIMX/PRIMXSYSTEM/PRODUCT/POLICIES/CRYHOD/PROTECTION"
#define PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_CRYHOD_POLICY          "/PRIMX/PRIMXSYSTEM/PRODUCT/POLICIES/CRYHOD/POLICY"
#define PMX_PATH_PRIMXSERVER_PRODUCT_CACHE                           "/PRIMX/PRIMXSYSTEM/PRODUCT/CACHE"
#define PMX_PATH_PRIMXSERVER_PRODUCT_KEYMAP                          "/PRIMX/PRIMXSYSTEM/PRODUCT/KEYMAP"
#define PMX_PATH_PRIMXSERVER_PRODUCT_LOGS                            "/PRIMX/PRIMXSYSTEM/PRODUCT/LOGS"
#define PMX_PATH_PRIMXSERVER_PRODUCT_ZONEMAP                         "/PRIMX/PRIMXSYSTEM/PRODUCT/ZONEMAP"
#define PMX_PATH_PRIMXSERVER_PRODUCT_PACKAGE                         "/PRIMX/PRIMXSYSTEM/PRODUCT/PACKAGE"
#define PMX_PATH_PRIMXSERVER_PRODUCT_PACKAGE_ZONECENTRAL             "/PRIMX/PRIMXSYSTEM/PRODUCT/PACKAGE/ZONECENTRAL"
#define PMX_PATH_PRIMXSERVER_PRODUCT_PACKAGE_ORIZON                  "/PRIMX/PRIMXSYSTEM/PRODUCT/PACKAGE/ORIZON"
#define PMX_PATH_PRIMXSERVER_PRODUCT_PACKAGE_CRYHOD                  "/PRIMX/PRIMXSYSTEM/PRODUCT/PACKAGE/CRYHOD"
#define PMX_PATH_PRIMXSERVER_PRODUCT_PACKAGE_AGENT                   "/PRIMX/PRIMXSYSTEM/PRODUCT/PACKAGE/AGENT"

#define PMX_PATH_PRIMXAUTH                                           "/PRIMX/PRIMXAUTH"
#define PMX_PATH_PRIMXAUTH_BIN                                       "/PRIMX/PRIMXAUTH/BIN"
#define PMX_PATH_PRIMXAUTH_CONF                                      "/PRIMX/PRIMXAUTH/CONF"
#define PMX_PATH_PRIMXAUTH_CONF_CERTS                                "/PRIMX/PRIMXAUTH/CONF/CERTS"
#define PMX_PATH_PRIMXAUTH_CONF_CERTS_MARIADB_CLIENT                 "/PRIMX/PRIMXAUTH/CONF/CERTS/MARIADB_CLIENT"
#define PMX_PATH_PRIMXAUTH_CONF_CERTS_SERVER                         "/PRIMX/PRIMXAUTH/CONF/CERTS/SERVER"
#define PMX_PATH_PRIMXAUTH_LIB                                       "/PRIMX/PRIMXAUTH/LIB"
#define PMX_PATH_PRIMXAUTH_LOGS                                      "/PRIMX/PRIMXAUTH/LOGS"

// configuration file name & path
#define PMX_CU_CONF_FILE_DIR                      "/PRIMX/PRIMXSYSTEM/CONF/"    // configure directory path for ctrl & update server
#define PMX_CU_CONF_FILE_NAME                     "pmx_cu_env.conf"       // control & update server configuration file name
#define PMX_A_CONF_FILE_DIR                       "/PRIMX/PRIMXAUTH/CONF/"      // configure directory path for auth. server
#define PMX_A_CONF_FILE_NAME                      "pmx_a_env.conf"        // authentication server configuration file name

// system password
#define PMX_SYSTEM_PASSOWRD                       "primxadmin1234~!"

// mariadb ssl cert & key files path 
#define PMX_CU_MARIADB_CA_PEM_FILE_PATH           "/PRIMX/PRIMXSYSTEM/CONF/CERTS/MARIADB_CLIENT/ca.pem"
#define PMX_CU_MARIADB_CLIENT_CERT_PEM_FILE_PATH  "/PRIMX/PRIMXSYSTEM/CONF/CERTS/MARIADB_CLIENT/client-cert.pem"
#define PMX_CU_MARIADB_CLIENT_KEY_PEM_FILE_PATH   "/PRIMX/PRIMXSYSTEM/CONF/CERTS/MARIADB_CLIENT/client-key.pem"
#define PMX_A_MARIADB_CA_PEM_FILE_PATH            "/PRIMX/PRIMXAUTH/CONF/CERTS/MARIADB_CLIENT/ca.pem"
#define PMX_A_MARIADB_CLIENT_CERT_PEM_FILE_PATH   "/PRIMX/PRIMXAUTH/CONF/CERTS/MARIADB_CLIENT/client-cert.pem"
#define PMX_A_MARIADB_CLIENT_KEY_PEM_FILE_PATH    "/PRIMX/PRIMXAUTH/CONF/CERTS/MARIADB_CLIENT/client-key.pem"

// server ssl password & files path
#define PMX_SERVER_SSL_PASSWORD                   "PxAdmin9280!"
#define PMX_CU_SERVER_CERT_INFO_PATH              "/PRIMX/PRIMXSYSTEM/CONF/CERTS/SERVER/server.crt"
#define PMX_CU_SERVER_CA_LIST_PATH                "/PRIMX/PRIMXSYSTEM/CONF/CERTS/SERVER/server.key"
#define PMX_A_SERVER_CERT_INFO_PATH               "/PRIMX/PRIMXAUTH/CONF/CERTS/SERVER/server.crt"
#define PMX_A_SERVER_CA_LIST_PATH                 "/PRIMX/PRIMXAUTH/CONF/CERTS/SERVER/server.key"

// PID file directory path
#define PMX_CU_PID_FILE_DIR_PATH                  "/PRIMX/PRIMXSYSTEM/CONF/"
#define PMX_A_PID_FILE_DIR_PATH                   "/PRIMX/PRIMXAUTH/CONF/"

// module names
#define PMX_MODULE_NAME_CTRLD_AGENT_IO_HANDLER    "pmx_ctrld_agent_io_handler"
#define PMX_MODULE_NAME_UPDATED_AGENT_IO_HANDLER  "pmx_updated_agent_io_handler"
#define PMX_MODULE_NAME_AUTHD_AGENT_IO_HANDLER    "pmx_authd_agent_io_handler"

// environmental values
#define PMX_DEFAULT_IO_TIMEOUT     20

/// base methods and tools defined class
/*
class CBase
{
public:
	static VOID MakeDirsForCtrlUpdateServer();
	static VOID MakeDirsForAuthServer();
	
	static VOID ChkMakeDir(IN const CHAR* sDir, IN const CHAR* sUser, IN const CHAR* sGroup, IN INT nOwner, IN INT nGroup, IN INT nOther);

	CBase() {};
	~CBase() {};
};
*/

//-----------------------------------------------------------------------------
#endif // __PMX_BASE_DEF_H__
