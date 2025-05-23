#pragma once
#pragma once
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <io.h>
#include "zcapi.h"
#include <string>
#include <vector>


class CPrimxzc
{
    //Function - General
    typedef HRESULT(*ZCAPI_ZC_Init)(void* reserved, ZCAPI_ZONECENTRAL_HANDLE* pzchdl);
    typedef HRESULT(*ZCAPI_ZC_SetUIParams)(ZCAPI_ZONECENTRAL_HANDLE zchdl, HWND hWndOwner);
    typedef HRESULT(*ZCAPI_ZC_GetFolderInfo)(ZCAPI_ZONECENTRAL_HANDLE zchdl, const WCHAR* pcwzPath, ZCAPI_FOLDER_TYPE* peType, BOOL* pbZoneHead, WCHAR* pwzZonePath, unsigned int* pcch);
    typedef HRESULT(*ZCAPI_ZC_GetPolicy)(ZCAPI_ZONECENTRAL_HANDLE zchdl, LONG lPolicy, VARIANT* pvarValue);
    typedef HRESULT(*ZCAPI_ZC_CloseKeys)(ZCAPI_ZONECENTRAL_HANDLE zchdl);
    typedef HRESULT(*ZCAPI_ZC_LoadZone)(ZCAPI_ZONECENTRAL_HANDLE zchdl, const WCHAR* pcwzPath, ZCAPI_ZONE_HANDLE* pzhdl);
    typedef HRESULT(*ZCAPI_ZC_LoadAccessFile)(ZCAPI_ZONECENTRAL_HANDLE zchdl, const WCHAR* pcwzPath, ZCAPI_ACCESSFILE_HANDLE* pzafhdl);
    typedef HRESULT(*ZCAPI_ZC_CreateAccessFile)(ZCAPI_ZONECENTRAL_HANDLE zchdl, const WCHAR* pcwzPath, ZCAPI_ACCESSFILE_HANDLE* pzafhdl);
    typedef HRESULT(*ZCAPI_ZC_LoadPersonalAccessFile)(ZCAPI_ZONECENTRAL_HANDLE zchdl, ZCAPI_PERSONALACCESSFILE_HANDLE* ppacfhdl);
    typedef HRESULT(*ZCAPI_ZC_LoadPersonalKey)(ZCAPI_ZONECENTRAL_HANDLE zchdl, ZCAPI_PERSONALKEY_HANDLE* ppkhdl);
    typedef HRESULT(*ZCAPI_ZC_GetRunTime)(ZCAPI_ZONECENTRAL_HANDLE zchdl, unsigned int sessionId, ZCAPI_RUNTIME_HANDLE* prthdl);
    typedef HRESULT(*ZCAPI_ZC_LoadZoneMap)(ZCAPI_ZONECENTRAL_HANDLE zchdl, DWORD dwFlags, ZCAPI_ZONEMAP_HANDLE* pzmdl);
    typedef HRESULT(*ZCAPI_ZC_CreateZoneScan)(ZCAPI_ZONECENTRAL_HANDLE zchdl, ZCAPI_ZONESCAN_HANDLE* pzshdl);
    typedef HRESULT(*ZCAPI_ZC_UngroupZone)(ZCAPI_ZONECENTRAL_HANDLE zchdl, const WCHAR* pcwzPath);
    typedef HRESULT(*ZCAPI_ZC_GroupZone)(ZCAPI_ZONECENTRAL_HANDLE zchdl, const WCHAR* pcwzPath, BOOL bForce);
    typedef HRESULT(*ZCAPI_ZC_CheckCertificate)(ZCAPI_ZONECENTRAL_HANDLE zchdl, const unsigned char* pcucCertificate, unsigned int cbCertificate, ZCAPI_CERTCHECK_PURPOSE ePurpose, ZCAPI_CERTCHECK_RESULT* peResult, WCHAR* pwzReason, unsigned int* pcch);
    typedef HRESULT(*ZCAPI_ZC_CloseHandle)(ZCAPI_ZONECENTRAL_HANDLE zchdl);
    //Function - Zones
    typedef HRESULT(*ZCAPI_ZN_OpenZone)(ZCAPI_ZONE_HANDLE zhdl, DWORD dwFlags, const WCHAR* pcwzCredName, const WCHAR* pcwzCredSecret);
    typedef HRESULT(*ZCAPI_ZN_GetZoneInfo)(ZCAPI_ZONE_HANDLE zhdl, ZCAPI_ZONE_TYPE* peType, WCHAR* pwzZonePath, unsigned int* pcchPath, WCHAR* pwzLabel, unsigned int* pcchLabel);
    typedef HRESULT(*ZCAPI_ZN_GetAccesses)(ZCAPI_ZONE_HANDLE zhdl, ZCAPI_ACCESSES_HANDLE* pahdl);
    typedef HRESULT(*ZCAPI_ZN_CloseZone)(ZCAPI_ZONE_HANDLE zhdl);
    typedef HRESULT(*ZCAPI_ZN_SaveZone)(ZCAPI_ZONE_HANDLE zhdl);
    typedef HRESULT(*ZCAPI_ZN_CloseHandle)(ZCAPI_ZONE_HANDLE zhdl);
    //Function - Access Files
    typedef HRESULT(*ZCAPI_AF_OpenAccessFile)(ZCAPI_ACCESSFILE_HANDLE zafhdl, DWORD dwFlags, const WCHAR* pcwzCredName, const WCHAR* pcwzCredSecret);
    typedef HRESULT(*ZCAPI_AF_GetFileInfo)(ZCAPI_ACCESSFILE_HANDLE zafhdl, WCHAR* pwzPath, unsigned int* pcch, ZCAPI_LOCATION_TYPE* peLocType, DWORD* pdwAttributes);
    typedef HRESULT(*ZCAPI_AF_ChangeAttributes)(ZCAPI_ACCESSFILE_HANDLE zafhdl, DWORD dwAttributes);
    typedef HRESULT(*ZCAPI_AF_GetAccesses)(ZCAPI_ACCESSFILE_HANDLE zafhdl, ZCAPI_ACCESSES_HANDLE* pahdl);
    typedef HRESULT(*ZCAPI_AF_GetSOSPassword)(ZCAPI_ACCESSFILE_HANDLE zafhdl, WCHAR* pwzSOSPwd, unsigned int* pcch, int* pnCheckValue);
    typedef HRESULT(*ZCAPI_AF_RenewSOSPassword)(ZCAPI_ACCESSFILE_HANDLE zafhdl);
    typedef HRESULT(*ZCAPI_AF_GetOTA)(ZCAPI_ACCESSFILE_HANDLE zafhdl, const WCHAR* pcwzTicket, WCHAR* pwzOTA, unsigned int* pcch, int* pnCheckValue);
    typedef HRESULT(*ZCAPI_AF_CloseAccessFile)(ZCAPI_ACCESSFILE_HANDLE zafhdl);
    typedef HRESULT(*ZCAPI_ZN_SaveAccessFile)(ZCAPI_ACCESSFILE_HANDLE zafhdl);
    typedef HRESULT(*ZCAPI_AF_CloseHandle)(ZCAPI_ACCESSFILE_HANDLE zafhdl);
    //Function - Accessess
    typedef HRESULT(*ZCAPI_AC_GetAccesses)(ZCAPI_ACCESSES_HANDLE zahdl, GUID* ptabGuidAccesses, unsigned int* pceAccesses);
    typedef HRESULT(*ZCAPI_AC_GetAccessInfo)(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess, struct ZCAPI_ACCESS_INFO* pstAccessInfo);
    typedef HRESULT(*ZCAPI_AC_AddAccessPassword)(ZCAPI_ACCESSES_HANDLE zahdl, const WCHAR* pcwzUserId, const WCHAR* pcwzPassword, DWORD dwRole, DWORD dwAttributes, GUID* pguidAccess);
    typedef HRESULT(*ZCAPI_AC_AddAccessCertificate)(ZCAPI_ACCESSES_HANDLE zahdl, const unsigned char* pcucCertificate, unsigned int cbCertificate, DWORD dwRole, DWORD dwAttributes, GUID* pguidAccess);
    typedef HRESULT(*ZCAPI_AC_AddIndirectAccess)(ZCAPI_ACCESSES_HANDLE zahdl, const WCHAR* pcwzPath, DWORD dwAttributes, GUID* pguidAccess);
    typedef HRESULT(*ZCAPI_AC_RemoveAccess)(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess);
    typedef HRESULT(*ZCAPI_AC_ChangeAccessLabel)(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess, const WCHAR* pcwzLabel);
    typedef HRESULT(*ZCAPI_AC_ChangeAccessRole)(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess, DWORD dwRole);
    typedef HRESULT(*ZCAPI_AC_ChangeAccessAttributes)(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess, DWORD dwAttributes);
    typedef HRESULT(*ZCAPI_AC_ChangeAccessPassword)(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess, const WCHAR* pcwzOldPassword, const WCHAR* pcwzNewPassword);
    typedef HRESULT(*ZCAPI_AC_LoadFromIndirectAccess)(ZCAPI_ACCESSES_HANDLE zahdl, const GUID* pcguidAccess, ZCAPI_ACCESSFILE_HANDLE pzafhdl);
    typedef HRESULT(*ZCAPI_AC_CloseHandle)(ZCAPI_ZONESCAN_HANDLE zahdl);
    //Function - Personal Access Key
    typedef HRESULT(*ZCAPI_PAF_Exists)(ZCAPI_PERSONALACCESSFILE_HANDLE pacfhdl, BOOL* pbExists);
    typedef HRESULT(*ZCAPI_PAF_GetAccess)(ZCAPI_PERSONALACCESSFILE_HANDLE pacfhdl, ZCAPI_ACCESSFILE_HANDLE pafhdl, GUID* pguidAccess);
    typedef HRESULT(*ZCAPI_PAF_CloseHandle)(ZCAPI_PERSONALACCESSFILE_HANDLE pacfhdl);
    //Function - Personal Key
    typedef HRESULT(*ZCAPI_PK_Exists)(ZCAPI_PERSONALKEY_HANDLE pkhdl, BOOL* pbExists);
    typedef HRESULT(*ZCAPI_PK_GetKeyInfo)(ZCAPI_PERSONALKEY_HANDLE pkhdl, struct ZCAPI_PERSONALKEY_INFO* pstKeyInfo);
    typedef HRESULT(*ZCAPI_PK_CloseHandle)(ZCAPI_PERSONALKEY_HANDLE pkhdl);
    //Function - Run-Time
    typedef HRESULT(*ZCAPI_RT_CloseAll)(ZCAPI_RUNTIME_HANDLE zrthdl);
    typedef HRESULT(*ZCAPI_RT_CloseAllZones)(ZCAPI_RUNTIME_HANDLE zrthdl);
    typedef HRESULT(*ZCAPI_RT_CloseAllKeys)(ZCAPI_RUNTIME_HANDLE zrthdl);
    typedef HRESULT(*ZCAPI_RT_AddPasswordKey)(ZCAPI_RUNTIME_HANDLE zrthdl, const WCHAR* pcwzUserId, const WCHAR* pcwzPassword);
    typedef HRESULT(*ZCAPI_RT_AddKeyFileKeys)(ZCAPI_RUNTIME_HANDLE zrthdl, const WCHAR* pcwzKeyFilePath, const WCHAR* pcwzAccessCode);
    typedef HRESULT(*ZCAPI_RT_GetVersion)(ZCAPI_RUNTIME_HANDLE zrthdl, unsigned int* puMajor, unsigned int* puMinor, unsigned int* puBuild, WCHAR* pwzVersion, unsigned int* pcchVersion);
    typedef HRESULT(*ZCAPI_RT_CloseHandle)(ZCAPI_RUNTIME_HANDLE zrthdl);
    typedef HRESULT(*ZCAPI_RT_ClosePasswordKey)(ZCAPI_RUNTIME_HANDLE zrthdl, const WCHAR* pcwzUserId, unsigned* pceTotalClosed);
    typedef HRESULT(*ZCAPI_RT_CloseKeyFileKey)(ZCAPI_RUNTIME_HANDLE zrthdl, const WCHAR* pcwzKeyFilePath, unsigned* pceTotalClosed);
    typedef HRESULT(*ZCAPI_RT_CloseZone)(ZCAPI_RUNTIME_HANDLE zrthdl, const WCHAR* pcwzZonePath);
    //Function - ZoneMap
    typedef HRESULT(*ZCAPI_ZM_GetNbZones)(ZCAPI_ZONEMAP_HANDLE zmhdl, unsigned int* pceZones);
    typedef HRESULT(*ZCAPI_ZM_GetZonePath)(ZCAPI_ZONEMAP_HANDLE zmhdl, unsigned int index, WCHAR* pwzPath, unsigned int* pcch);
    typedef HRESULT(*ZCAPI_ZM_CloseHandle)(ZCAPI_ZONEMAP_HANDLE zmhdl);
    //Function - ZoneScan
    //typedef HRESULT(*ZCAPI_ZS_StartScan)(ZCAPI_ZONESCAN_HANDLE zshdl, const WCHAR* pcwzStartPath, DWORD dwFlags, ZCAPI_SCAN_PROGRESS pfnScanProgress, void* pvData);
    typedef HRESULT(*ZCAPI_ZS_GetNbZones)(ZCAPI_ZONESCAN_HANDLE zshdl, unsigned int* pceZones);
    typedef HRESULT(*ZCAPI_ZS_GetZonePath)(ZCAPI_ZONESCAN_HANDLE zshdl, unsigned int index, WCHAR* pwzPath, unsigned int* pcch);
    typedef HRESULT(*ZCAPI_ZS_CloseHandle)(ZCAPI_ZONESCAN_HANDLE zshdl);
    typedef HRESULT(*ZCAPI_ScanProgress)(void* pvData, const WCHAR* pcwzPath, BOOL bZoneHead);
    //Function - Misc
    typedef HRESULT(*ZCAPI_FormatMessage)(HRESULT hrCode, const WCHAR* pwzMessage, unsigned int* pcch);

public:
    HRESULT Initalize();
    BOOL CheckZCService();
    BOOL CheckZCInstallPath(std::wstring &wInstallPath);
    BOOL m_bLoadModule = FALSE;


	ZCAPI_FOLDER_TYPE  GetFolderInfo(CString strDstPath);

	ZCAPI_FOLDER_TYPE SetCloseZone(CString strDstPath);

    ~CPrimxzc();

    //Function - General
    ZCAPI_ZC_Init                         pZCAPI_ZC_Init = NULL;
    ZCAPI_ZC_SetUIParams                  pZCAPI_ZC_SetUIParams = NULL;
    ZCAPI_ZC_GetFolderInfo                pZCAPI_ZC_GetFolderInfo = NULL;
    ZCAPI_ZC_GetPolicy                    pZCAPI_ZC_GetPolicy = NULL;
    ZCAPI_ZC_CloseKeys                    pZCAPI_ZC_CloseKeys = NULL;
    ZCAPI_ZC_LoadZone                     pZCAPI_ZC_LoadZone = NULL;
    ZCAPI_ZC_LoadAccessFile               pZCAPI_ZC_LoadAccessFile = NULL;
    ZCAPI_ZC_CreateAccessFile             pZCAPI_ZC_CreateAccessFile = NULL;
    ZCAPI_ZC_LoadPersonalAccessFile       pZCAPI_ZC_LoadPersonalAccessFile = NULL;
    ZCAPI_ZC_LoadPersonalKey              pZCAPI_ZC_LoadPersonalKey = NULL;
    ZCAPI_ZC_GetRunTime                   pZCAPI_ZC_GetRunTime = NULL;
    ZCAPI_ZC_LoadZoneMap                  pZCAPI_ZC_LoadZoneMap = NULL;
    ZCAPI_ZC_CreateZoneScan               pZCAPI_ZC_CreateZoneScan = NULL;
    ZCAPI_ZC_UngroupZone                  pZCAPI_ZC_UngroupZone = NULL;
    ZCAPI_ZC_GroupZone                    pZCAPI_ZC_GroupZone = NULL;
    ZCAPI_ZC_CheckCertificate             pZCAPI_ZC_CheckCertificate = NULL;
    ZCAPI_ZC_CloseHandle                  pZCAPI_ZC_CloseHandle = NULL;
    //Function - Zones
    ZCAPI_ZN_OpenZone                     pZCAPI_ZN_OpenZone = NULL;
    ZCAPI_ZN_GetZoneInfo                  pZCAPI_ZN_GetZoneInfo = NULL;
    ZCAPI_ZN_GetAccesses                  pZCAPI_ZN_GetAccesses = NULL;
    ZCAPI_ZN_CloseZone                    pZCAPI_ZN_CloseZone = NULL;
    ZCAPI_ZN_SaveZone                     pZCAPI_ZN_SaveZone = NULL;
    ZCAPI_ZN_CloseHandle                  pZCAPI_ZN_CloseHandle = NULL;
    //Function - Access Files
    ZCAPI_AF_OpenAccessFile               pZCAPI_AF_OpenAccessFile = NULL;
    ZCAPI_AF_GetFileInfo                  pZCAPI_AF_GetFileInfo = NULL;
    ZCAPI_AF_ChangeAttributes             pZCAPI_AF_ChangeAttributes = NULL;
    ZCAPI_AF_GetAccesses                  pZCAPI_AF_GetAccesses = NULL;
    ZCAPI_AF_GetSOSPassword               pZCAPI_AF_GetSOSPassword = NULL;
    ZCAPI_AF_RenewSOSPassword             pZCAPI_AF_RenewSOSPassword = NULL;
    ZCAPI_AF_GetOTA                       pZCAPI_AF_GetOTA = NULL;
    ZCAPI_AF_CloseAccessFile              pZCAPI_AF_CloseAccessFile = NULL;
    ZCAPI_ZN_SaveAccessFile               pZCAPI_ZN_SaveAccessFile = NULL;
    ZCAPI_AF_CloseHandle                  pZCAPI_AF_CloseHandle = NULL;
    //Function - Accessess
    ZCAPI_AC_GetAccesses                  pZCAPI_AC_GetAccesses = NULL;
    ZCAPI_AC_GetAccessInfo                pZCAPI_AC_GetAccessInfo = NULL;
    ZCAPI_AC_AddAccessPassword            pZCAPI_AC_AddAccessPassword = NULL;
    ZCAPI_AC_AddAccessCertificate         pZCAPI_AC_AddAccessCertificate = NULL;
    ZCAPI_AC_AddIndirectAccess            pZCAPI_AC_AddIndirectAccess = NULL;
    ZCAPI_AC_RemoveAccess                 pZCAPI_AC_RemoveAccess = NULL;
    ZCAPI_AC_ChangeAccessLabel            pZCAPI_AC_ChangeAccessLabel = NULL;
    ZCAPI_AC_ChangeAccessRole             pZCAPI_AC_ChangeAccessRole = NULL;
    ZCAPI_AC_ChangeAccessAttributes       pZCAPI_AC_ChangeAccessAttributes = NULL;
    ZCAPI_AC_ChangeAccessPassword         pZCAPI_AC_ChangeAccessPassword = NULL;
    ZCAPI_AC_LoadFromIndirectAccess       pZCAPI_AC_LoadFromIndirectAccess = NULL;
    ZCAPI_AC_CloseHandle                  pZCAPI_AC_CloseHandle = NULL;
    //Function - Personal Access Key
    ZCAPI_PAF_Exists                      pZCAPI_PAF_Exists = NULL;
    ZCAPI_PAF_GetAccess                   pZCAPI_PAF_GetAccess = NULL;
    ZCAPI_PAF_CloseHandle                 pZCAPI_PAF_CloseHandle = NULL;
    //Function - Personal Key
    ZCAPI_PK_Exists                       pZCAPI_PK_Exists = NULL;
    ZCAPI_PK_GetKeyInfo                   pZCAPI_PK_GetKeyInfo = NULL;
    ZCAPI_PK_CloseHandle                  pZCAPI_PK_CloseHandle = NULL;
    //Function - Run-Time
    ZCAPI_RT_CloseAll                     pZCAPI_RT_CloseAll = NULL;
    ZCAPI_RT_CloseAllZones                pZCAPI_RT_CloseAllZones = NULL;
    ZCAPI_RT_CloseAllKeys                 pZCAPI_RT_CloseAllKeys = NULL;
    ZCAPI_RT_AddPasswordKey               pZCAPI_RT_AddPasswordKey = NULL;
    ZCAPI_RT_AddKeyFileKeys               pZCAPI_RT_AddKeyFileKeys = NULL;
    ZCAPI_RT_GetVersion                   pZCAPI_RT_GetVersion = NULL;
    ZCAPI_RT_CloseHandle                  pZCAPI_RT_CloseHandle = NULL;
    ZCAPI_RT_ClosePasswordKey             pZCAPI_RT_ClosePasswordKey = NULL;
    ZCAPI_RT_CloseKeyFileKey              pZCAPI_RT_CloseKeyFileKey = NULL;
    ZCAPI_RT_CloseZone                    pZCAPI_RT_CloseZone = NULL;
    //Function - ZoneMap
    ZCAPI_ZM_GetNbZones                   pZCAPI_ZM_GetNbZones = NULL;
    ZCAPI_ZM_GetZonePath                  pZCAPI_ZM_GetZonePath = NULL;
    ZCAPI_ZM_CloseHandle                  pZCAPI_ZM_CloseHandle = NULL;
    //Function - ZoneScan
    //ZCAPI_ZS_StartScan                    pZCAPI_ZS_StartScan = NULL;
    ZCAPI_ZS_GetNbZones                   pZCAPI_ZS_GetNbZones = NULL;
    ZCAPI_ZS_GetZonePath                  pZCAPI_ZS_GetZonePath = NULL;
    ZCAPI_ZS_CloseHandle                  pZCAPI_ZS_CloseHandle = NULL;
    ZCAPI_ScanProgress                    pZCAPI_ScanProgress = NULL;
    //Function - Misc
    ZCAPI_FormatMessage                   pZCAPI_FormatMessage = NULL;

    HRESULT ZoneCentral_AccessFileUser(const WCHAR* pwzCerFiles);
    HRESULT ZoneCentral_AccessFileList();
    HRESULT ZoneCentral_Version();

    void ZoneCentral_ErrorMessage(HRESULT erhResult, std::wstring& pwzMsg);

    struct ZCAPI_ACCESS_INFO m_zcAIUser = { 0 };

    WCHAR* m_pwzPersonalAccessList = NULL;

public:
    HMODULE hDll = NULL;
    ZCAPI_ZONECENTRAL_HANDLE m_zcHdl = NULL;
    //HRESULT hResult = NULL;

};

