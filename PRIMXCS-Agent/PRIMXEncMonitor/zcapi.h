/*
ZC>API - Version 2022

Product(s)  ZONECENTRAL, ZEDMAIL, ZED! FULL EDITION
Platforms   Windows 7, 8.1, 10
File:        zcapi.h

Usage:
-----
    - #include "this header"
    - dynamically load the functions from the zcax.dll or link with
      the zcapi.lib library
    - the zcax.dll bridge must always be installed by your application
    - the API only works with an installed run-time (see product list
        above). If the run-time is a Limited Edition, features are also
        limited.
Copyright Rimahr LAB
------------------------------------------------------------------------------*/

#include <OAIdl.h>

#ifndef __ZCAPI_H__
#define __ZCAPI_H__

#ifdef __cplusplus
extern "C"{
#endif 

#ifndef ZCAPICALL 
#define ZCAPICALL __declspec(dllimport) __stdcall
#endif

//Return Code
#ifndef E_DENIED
#define E_DENIED __HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED)
#endif

#ifndef E_TOO_SMALL
#define E_TOO_SMALL __HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER)
#endif

#ifndef E_FILE_NOT_FOUND
#define E_FILE_NOT_FOUND __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)
#endif

#ifndef E_FILE_EXISTS
#define E_FILE_EXISTS __HRESULT_FROM_WIN32(ERROR_FILE_EXISTS)
#endif

#ifndef E_BAD_PATHNAME
#define E_BAD_PATHNAME __HRESULT_FROM_WIN32(ERROR_BAD_PATHNAME)
#endif

#ifndef E_DIR_NOT_EMPTY
#define E_DIR_NOT_EMPTY __HRESULT_FROM_WIN32(ERROR_DIR_NOT_EMPTY)
#endif

#ifndef E_NO_MORE_FILES
#define E_NO_MORE_FILES __HRESULT_FROM_WIN32(ERROR_NO_MORE_FILES)
#endif

#ifndef E_NOT_FOUND
#define E_NOT_FOUND __HRESULT_FROM_WIN32(ERROR_NOT_FOUND)
#endif


//Structures ans Data Types
#ifndef __ZCAPIIMPL__
typedef struct { int _dontuse; } *ZCAPI_ZONECENTRAL_HANDLE;
typedef struct { int _dontuse; } *ZCAPI_ZONE_HANDLE;
typedef struct { int _dontuse; } *ZCAPI_ACCESSFILE_HANDLE;
typedef struct { int _dontuse; } *ZCAPI_ACCESSES_HANDLE;
typedef struct { int _dontuse; } *ZCAPI_PERSONALACCESSFILE_HANDLE;
typedef struct { int _dontuse; } *ZCAPI_PERSONALKEY_HANDLE;
typedef struct { int _dontuse; } *ZCAPI_RUNTIME_HANDLE;
typedef struct { int _dontuse; } *ZCAPI_ZONEMAP_HANDLE;
typedef struct { int _dontuse; } *ZCAPI_ZONESCAN_HANDLE;
#endif

enum ZCAPI_FOLDER_TYPE
{
    ZCAPI_FOLDER_UNKNOWN = 0,
    ZCAPI_FOLDER_ENCRYPTED_ZONE = 1,
    ZCAPI_FOLDER_CLEAR_ZONE = 2,
    ZCAPI_FOLDER_MAINTENANCE_ZONE = 3,
    ZCAPI_FOLDER_RAW_ZONE = 4,
    ZCAPI_FOLDER_CLEAR = 5,
};
enum ZCAPI_SESSIONID_TYPE
{
    ZCAPI_SESSIONID_ALL_SESSIONS = -2,
    ZCAPI_SESSIONID_CURRENT_SESSION = -1
};
enum ZCAPI_CERTCHECK_PURPOSE
{
    ZCAPI_CCHECKPURP_DEFAULT = 0,
    ZCAPI_CCHECKPURP_CONTAINER = 1
};
enum ZCAPI_CERTCHECK_RESULT
{
    ZCAPI_CCHECKRES_UNKNOWN = 0,
    ZCAPI_CCHECKRES_OK = 1,
    ZCAPI_CCHECKRES_WARNING = 2,
    ZCAPI_CCHECKRES_ERROR = 3
};
enum ZCAPI_ZONE_TYPE
{
    ZCAPI_ZONE_UNKNOWN = 0,
    ZCAPI_ZONE_ENCRYPTED = 1,
    ZCAPI_ZONE_CLEAR = 2,
    ZCAPI_ZONE_MAINTENANCE = 3,
    ZCAPI_ZONE_RAW = 4
};
enum ZCAPI_ACCESS_TYPE
{
    ZCAPI_ACCESS_PASSWORD = 1,
    ZCAPI_ACCESS_CERTIFICATE = 2,
    ZCAPI_ACCESS_INDIRECT = 3
};
enum ZCAPI_LOCATION_TYPE
{
    ZCAPI_LOCATION_UNKNOWN = 0,
    ZCAPI_LOCATION_PERSONAL = 1,
    ZCAPI_LOCATION_PRIMARY = 2,
    ZCAPI_LOCATION_CACHE = 3,
    ZCAPI_LOCATION_CURRENTDIR = 4,
    ZCAPI_LOCATION_ABSOLUTEPATH = 5,
};
struct ZCAPI_ACCESS_INFO
{
    unsigned int cbSize;
    GUID guidAccess;
    ZCAPI_ACCESS_TYPE eType;
    DWORD dwRole;
    DWORD dwAttributes;
    WCHAR* pwzLabel;
    unsigned int cchLabel;
    WCHAR* pwzIndirectFileName;
    unsigned int cchIndirectFileName;
    unsigned char* pucCertificate;
    unsigned int cbCertificate;
};
struct ZCAPI_PERSONALKEY_INFO
{
    unsigned int cbSize;
    GUID guidAccess;
    ZCAPI_ACCESS_TYPE eType;
    WCHAR* pwzLabel;
    unsigned int cchLabel;
    unsigned char* pucCertificate;
    unsigned int cbCertificate;
};

//Function - General
extern HRESULT ZCAPICALL ZCAPI_ZC_Init(void* reserved, ZCAPI_ZONECENTRAL_HANDLE* pzchdl);
extern HRESULT ZCAPICALL ZCAPI_ZC_SetUIParams(ZCAPI_ZONECENTRAL_HANDLE zchdl, HWND hWndOwner);
extern HRESULT ZCAPICALL ZCAPI_ZC_GetFolderInfo(ZCAPI_ZONECENTRAL_HANDLE zchdl, const WCHAR* pcwzPath, ZCAPI_FOLDER_TYPE* peType, BOOL* pbZoneHead, WCHAR* pwzZonePath, unsigned int* pcch);
extern HRESULT ZCAPICALL ZCAPI_ZC_GetPolicy(ZCAPI_ZONECENTRAL_HANDLE zchdl, LONG lPolicy, VARIANT* pvarValue);
extern HRESULT ZCAPICALL ZCAPI_ZC_CloseKeys(ZCAPI_ZONECENTRAL_HANDLE zchdl);
extern HRESULT ZCAPICALL ZCAPI_ZC_LoadZone(ZCAPI_ZONECENTRAL_HANDLE zchdl, const WCHAR* pcwzPath, ZCAPI_ZONE_HANDLE* pzhdl);
extern HRESULT ZCAPICALL ZCAPI_ZC_LoadAccessFile(ZCAPI_ZONECENTRAL_HANDLE zchdl, const WCHAR* pcwzPath, ZCAPI_ACCESSFILE_HANDLE* pzafhdl);
extern HRESULT ZCAPICALL ZCAPI_ZC_CreateAccessFile(ZCAPI_ZONECENTRAL_HANDLE zchdl, const WCHAR* pcwzPath, ZCAPI_ACCESSFILE_HANDLE* pzafhdl);
extern HRESULT ZCAPICALL ZCAPI_ZC_LoadPersonalAccessFile(ZCAPI_ZONECENTRAL_HANDLE zchdl, ZCAPI_PERSONALACCESSFILE_HANDLE* ppacfhdl);
extern HRESULT ZCAPICALL ZCAPI_ZC_LoadPersonalKey(ZCAPI_ZONECENTRAL_HANDLE zchdl, ZCAPI_PERSONALKEY_HANDLE* ppkhdl);
extern HRESULT ZCAPICALL ZCAPI_ZC_GetRunTime(ZCAPI_ZONECENTRAL_HANDLE zchdl, unsigned int sessionId, ZCAPI_RUNTIME_HANDLE* prthdl);
extern HRESULT ZCAPICALL ZCAPI_ZC_LoadZoneMap(ZCAPI_ZONECENTRAL_HANDLE zchdl, DWORD dwFlags, ZCAPI_ZONEMAP_HANDLE* pzmdl);
extern HRESULT ZCAPICALL ZCAPI_ZC_CreateZoneScan(ZCAPI_ZONECENTRAL_HANDLE zchdl, ZCAPI_ZONESCAN_HANDLE* pzshdl);
extern HRESULT ZCAPICALL ZCAPI_ZC_UngroupZone(ZCAPI_ZONECENTRAL_HANDLE zchdl, const WCHAR* pcwzPath);
extern HRESULT ZCAPICALL ZCAPI_ZC_GroupZone(ZCAPI_ZONECENTRAL_HANDLE zchdl, const WCHAR* pcwzPath, BOOL bForce);
extern HRESULT ZCAPICALL ZCAPI_ZC_CheckCertificate(ZCAPI_ZONECENTRAL_HANDLE zchdl, const unsigned char* pcucCertificate, unsigned int cbCertificate, ZCAPI_CERTCHECK_PURPOSE ePurpose, ZCAPI_CERTCHECK_RESULT* peResult, WCHAR* pwzReason, unsigned int* pcch);
extern HRESULT ZCAPICALL ZCAPI_ZC_CloseHandle(ZCAPI_ZONECENTRAL_HANDLE zchdl);

//Function - Zones
extern HRESULT ZCAPICALL ZCAPI_ZN_OpenZone(ZCAPI_ZONE_HANDLE zhdl, DWORD dwFlags, const WCHAR* pcwzCredName, const WCHAR* pcwzCredSecret);
extern HRESULT ZCAPICALL ZCAPI_ZN_GetZoneInfo(ZCAPI_ZONE_HANDLE zhdl, ZCAPI_ZONE_TYPE* peType, WCHAR* pwzZonePath, unsigned int* pcchPath, WCHAR* pwzLabel, unsigned int* pcchLabel);
extern HRESULT ZCAPICALL ZCAPI_ZN_GetAccesses(ZCAPI_ZONE_HANDLE zhdl, ZCAPI_ACCESSES_HANDLE* pahdl);
extern HRESULT ZCAPICALL ZCAPI_ZN_CloseZone(ZCAPI_ZONE_HANDLE zhdl);
extern HRESULT ZCAPICALL ZCAPI_ZN_SaveZone(ZCAPI_ZONE_HANDLE zhdl);
extern HRESULT ZCAPICALL ZCAPI_ZN_CloseHandle(ZCAPI_ZONE_HANDLE zhdl);

//Function - Access Files
extern HRESULT ZCAPICALL ZCAPI_AF_OpenAccessFile(ZCAPI_ACCESSFILE_HANDLE zafhdl, DWORD dwFlags, const WCHAR* pcwzCredName, const WCHAR* pcwzCredSecret);
extern HRESULT ZCAPICALL ZCAPI_AF_GetFileInfo(ZCAPI_ACCESSFILE_HANDLE zafhdl, WCHAR* pwzPath, unsigned int* pcch, ZCAPI_LOCATION_TYPE* peLocType, DWORD* pdwAttributes);
extern HRESULT ZCAPICALL ZCAPI_AF_ChangeAttributes(ZCAPI_ACCESSFILE_HANDLE zafhdl, DWORD dwAttributes);
extern HRESULT ZCAPICALL ZCAPI_AF_GetAccesses(ZCAPI_ACCESSFILE_HANDLE zafhdl, ZCAPI_ACCESSES_HANDLE* pahdl);
extern HRESULT ZCAPICALL ZCAPI_AF_GetSOSPassword(ZCAPI_ACCESSFILE_HANDLE zafhdl, WCHAR* pwzSOSPwd, unsigned int* pcch, int* pnCheckValue);
extern HRESULT ZCAPICALL ZCAPI_AF_RenewSOSPassword(ZCAPI_ACCESSFILE_HANDLE zafhdl);
extern HRESULT ZCAPICALL ZCAPI_AF_GetOTA(ZCAPI_ACCESSFILE_HANDLE zafhdl, const WCHAR* pcwzTicket, WCHAR* pwzOTA, unsigned int* pcch, int* pnCheckValue);
extern HRESULT ZCAPICALL ZCAPI_AF_CloseAccessFile(ZCAPI_ACCESSFILE_HANDLE zafhdl);
extern HRESULT ZCAPICALL ZCAPI_ZN_SaveAccessFile(ZCAPI_ACCESSFILE_HANDLE zafhdl);
extern HRESULT ZCAPICALL ZCAPI_AF_CloseHandle(ZCAPI_ACCESSFILE_HANDLE zafhdl);

//Function - Accessess
extern HRESULT ZCAPICALL ZCAPI_AC_GetAccesses(ZCAPI_ACCESSES_HANDLE zahdl, GUID* ptabGuidAccesses, unsigned int* pceAccesses);
extern HRESULT ZCAPICALL ZCAPI_AC_GetAccessInfo(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess, struct ZCAPI_ACCESS_INFO* pstAccessInfo);
extern HRESULT ZCAPICALL ZCAPI_AC_AddAccessPassword(ZCAPI_ACCESSES_HANDLE zahdl, const WCHAR* pcwzUserId, const WCHAR* pcwzPassword, DWORD dwRole, DWORD dwAttributes, GUID* pguidAccess);
extern HRESULT ZCAPICALL ZCAPI_AC_AddAccessCertificate(ZCAPI_ACCESSES_HANDLE zahdl, const unsigned char* pcucCertificate, unsigned int cbCertificate, DWORD dwRole, DWORD dwAttributes, GUID* pguidAccess);
extern HRESULT ZCAPICALL ZCAPI_AC_AddIndirectAccess(ZCAPI_ACCESSES_HANDLE zahdl, const WCHAR* pcwzPath, DWORD dwAttributes, GUID* pguidAccess);
extern HRESULT ZCAPICALL ZCAPI_AC_RemoveAccess(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess);
extern HRESULT ZCAPICALL ZCAPI_AC_ChangeAccessLabel(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess, const WCHAR* pcwzLabel);
extern HRESULT ZCAPICALL ZCAPI_AC_ChangeAccessRole(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess, DWORD dwRole);
extern HRESULT ZCAPICALL ZCAPI_AC_ChangeAccessAttributes(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess, DWORD dwAttributes);
extern HRESULT ZCAPICALL ZCAPI_AC_ChangeAccessPassword(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess, const WCHAR* pcwzOldPassword, const WCHAR* pcwzNewPassword);
extern HRESULT ZCAPICALL ZCAPI_AC_LoadFromIndirectAccess(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess, ZCAPI_ACCESSFILE_HANDLE pzafhdl);
extern HRESULT ZCAPICALL ZCAPI_AC_CloseHandle(ZCAPI_ZONESCAN_HANDLE zahdl);

//Function - Personal Access Key
extern HRESULT ZCAPICALL ZCAPI_PAF_Exists(ZCAPI_PERSONALACCESSFILE_HANDLE pacfhdl, BOOL *pbExists);
extern HRESULT ZCAPICALL ZCAPI_PAF_GetAccess(ZCAPI_PERSONALACCESSFILE_HANDLE pacfhdl, ZCAPI_ACCESSFILE_HANDLE pafhdl, GUID* pguidAccess);
extern HRESULT ZCAPICALL ZCAPI_PAF_CloseHandle(ZCAPI_PERSONALACCESSFILE_HANDLE pacfhdl);

//Function - Personal Key
extern HRESULT ZCAPICALL ZCAPI_PK_Exists(ZCAPI_PERSONALKEY_HANDLE pkhdl, BOOL* pbExists);
extern HRESULT ZCAPICALL ZCAPI_PK_GetKeyInfo(ZCAPI_PERSONALKEY_HANDLE pkhdl, struct ZCAPI_PERSONALKEY_INFO* pstKeyInfo);
extern HRESULT ZCAPICALL ZCAPI_PK_CloseHandle(ZCAPI_PERSONALKEY_HANDLE pkhdl);

//Function - Run-Time
extern HRESULT ZCAPICALL ZCAPI_RT_CloseAll(ZCAPI_RUNTIME_HANDLE zrthdl);
extern HRESULT ZCAPICALL ZCAPI_RT_CloseAllZones(ZCAPI_RUNTIME_HANDLE zrthdl);
extern HRESULT ZCAPICALL ZCAPI_RT_CloseAllKeys(ZCAPI_RUNTIME_HANDLE zrthdl);
extern HRESULT ZCAPICALL ZCAPI_RT_AddPasswordKey(ZCAPI_RUNTIME_HANDLE zrthdl, const WCHAR* pcwzUserId, const WCHAR* pcwzPassword);
extern HRESULT ZCAPICALL ZCAPI_RT_AddKeyFileKeys(ZCAPI_RUNTIME_HANDLE zrthdl, const WCHAR* pcwzKeyFilePath, const WCHAR* pcwzAccessCode);
extern HRESULT ZCAPICALL ZCAPI_RT_GetVersion(ZCAPI_RUNTIME_HANDLE zrthdl, unsigned int* puMajor, unsigned int* puMinor, unsigned int* puBuild, WCHAR* pwzVersion, unsigned int* pcchVersion);
extern HRESULT ZCAPICALL ZCAPI_RT_CloseHandle(ZCAPI_RUNTIME_HANDLE zrthdl);
extern HRESULT ZCAPICALL ZCAPI_RT_ClosePasswordKey(ZCAPI_RUNTIME_HANDLE zrthdl, const WCHAR* pcwzUserId, unsigned* pceTotalClosed);
extern HRESULT ZCAPICALL ZCAPI_RT_CloseKeyFileKey(ZCAPI_RUNTIME_HANDLE zrthdl, const WCHAR* pcwzKeyFilePath, unsigned* pceTotalClosed);
extern HRESULT ZCAPICALL ZCAPI_RT_CloseZone(ZCAPI_RUNTIME_HANDLE zrthdl, const WCHAR* pcwzZonePath);

//Function - ZoneMap
extern HRESULT ZCAPICALL ZCAPI_ZM_GetNbZones(ZCAPI_ZONEMAP_HANDLE zmhdl, unsigned int* pceZones);
extern HRESULT ZCAPICALL ZCAPI_ZM_GetZonePath(ZCAPI_ZONEMAP_HANDLE zmhdl, unsigned int index, WCHAR* pwzPath, unsigned int* pcch);
extern HRESULT ZCAPICALL ZCAPI_ZM_CloseHandle(ZCAPI_ZONEMAP_HANDLE zmhdl);

//Function - ZoneScan
//extern HRESULT ZCAPICALL ZCAPI_ZS_StartScan(ZCAPI_ZONESCAN_HANDLE zshdl, const WCHAR* pcwzStartPath, DWORD dwFlags, ZCAPI_SCAN_PROGRESS pfnScanProgress, void* pvData);
extern HRESULT ZCAPICALL ZCAPI_ZS_GetNbZones(ZCAPI_ZONESCAN_HANDLE zshdl, unsigned int* pceZones);
extern HRESULT ZCAPICALL ZCAPI_ZS_GetZonePath(ZCAPI_ZONESCAN_HANDLE zshdl, unsigned int index, WCHAR* pwzPath, unsigned int* pcch);
extern HRESULT ZCAPICALL ZCAPI_ZS_CloseHandle(ZCAPI_ZONESCAN_HANDLE zshdl);
extern HRESULT ZCAPICALL ZCAPI_ScanProgress(void* pvData, const WCHAR* pcwzPath, BOOL bZoneHead);

//Function - Misc
extern HRESULT ZCAPICALL ZCAPI_FormatMessage(HRESULT hrCode, const WCHAR* pwzMessage, unsigned int* pcch);

#ifdef __cplusplus
}
#endif

#endif    /* __ZCAPI_H__ */
