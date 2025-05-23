/// @file xma_xml.cpp
/// @author Park, sung soo
/// @date 2023-06-12
/// @brief xml I/O hamdling class

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <pmx_base_def.h>

#ifndef __PMX_BASE_DEF_CPP__
#define __PMX_BASE_DEF_CPP__

//-----------------------------------------------------------------------------
// LOCAL DEFINTION
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// METHODS DEFINTION
//-----------------------------------------------------------------------------
/*
VOID CBase::MakeDirsForCtrlUpdateServer()
{
	ChkMakeDir(PMX_PATH_PRIMXSERVER,                                         PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 7, 7);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_BIN,                                     PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_CONF,                                    PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_CONF_CERTS,                              PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_CONF_CERTS_MARIADB_CLIENT,               PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_CONF_CERTS_SERVER,                       PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_LIB,                                     PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_LOGS,                                    PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_WAREHOUSE,                               PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_WAREHOUSE_DBBACKUP,                      PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_WAREHOUSE_TEMP,                          PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_WAREHOUSE_ERRLOGS,                       PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_WEBSRV,                                  PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT,                                 PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_ACCESSFILES,                     PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_POLICYSCRIPT,           PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_ZONECENTRAL,            PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_ZONECENTRAL_PROTECTION, PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_ZONECENTRAL_POLICY,     PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_ORIZON,                 PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_ORIZON_PROTECTION,      PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_ORIZON_POLICY,          PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_CRYHOD,                 PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_CRYHOD_PROTECTION,      PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_CRYHOD_POLICY,          PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_CACHE,                           PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_KEYMAP,                          PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_LOGS,                            PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_ZONEMAP,                         PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_PACKAGE,                         PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_PACKAGE_ZONECENTRAL,             PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_PACKAGE_ORIZON,                  PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_PACKAGE_CRYHOD,                  PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
	ChkMakeDir(PMX_PATH_PRIMXSERVER_PRODUCT_PACKAGE_AGENT,                   PMX_USER_WEB, PMX_USER_GROUP, 7, 5, 0);
}

VOID CBase::MakeDirsForAuthServer()
{
	ChkMakeDir(PMX_PATH_PRIMXAUTH,                           PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXAUTH_BIN,                       PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXAUTH_CONF,                      PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXAUTH_CONF_CERTS,                PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXAUTH_CONF_CERTS_MARIADB_CLIENT, PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXAUTH_CONF_CERTS_SERVER,         PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXAUTH_LIB,                       PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
	ChkMakeDir(PMX_PATH_PRIMXAUTH_LOGS,                      PMX_USER_ROOT, PMX_ROOT_GROUP, 7, 0, 0);
}

VOID CBase::ChkMakeDir(IN const CHAR* sDir, 
					   IN const CHAR* sUser, 
					   IN const CHAR* sGroup, 
					   IN INT nOwner, 
					   IN INT nGroup, 
					   IN INT nOther)
{
	if (NULL == sDir || NULL == sUser || NULL == sGroup) return;
	if (0 == strlen(sDir) || 0 == strlen(sUser) || 0 == strlen(sGroup)) return;
	if ((7 < nOwner || 0 > nOwner) || (7 < nGroup || 0 > nGroup) || (7 < nOther || 0 > nOther)) return;

	INT nFlag = (nOwner << 6) | (nGroup << 3) | nOther;
	
	if (FALSE == SWC::swc_is_dir_exists(sDir)) 
	{
		mkdir(sDir, nFlag);
		SWC::swc_chown_chgrp(sDir, sUser, sGroup);
	}
}
*/

//-----------------------------------------------------------------------------
#endif // __PMX_BASE_DEF_CPP__
