#include "stdafx.h"
#include "CPrimxzc.h"
#include <iostream>
#include <winsvc.h>

#ifndef ZCAPICALL
#define ZCAPICALL __declspec(dllexport) __stdcall
#endif

#define PRIMXZC_DLLFILENAME L"zcax.dll"
#define PRIMXZC_DEFAULTSERVICE L"ZCP"

#define PRIMXZC_REGKEY_DEFAULT L"SOFTWARE\\Prim'X"
#define PRIMXZC_REGKEY_OFFICIAL L"ZoneCentral"
#define PRIMXZC_REGKEY_TRIAL L"ZoneCentral Trial"

std::vector<std::wstring> wzOfficialVersion = { L"22.1", L"22.2", L"22.3", L"22.4",
                                             L"23.1", L"23.2", L"23.3", L"23.4",
                                             L"24.1", L"24.2", L"24.3", L"24.4",
                                             L"25.1", L"25.2", L"25.3", L"25.4",
                                             L"26.1", L"26.2", L"26.3", L"26.4",
                                             L"27.1", L"27.2", L"27.3", L"27.4",
                                             L"28.1", L"28.2", L"28.3", L"28.4",
                                             L"29.1", L"29.2", L"29.3", L"29.4",
                                             L"30.1", L"30.2", L"30.3", L"30.4" };
std::vector<std::wstring> wzTrialVersion = { L"22.1", L"22.2", L"22.3", L"22.4",
                                             L"23.1", L"23.2", L"23.3", L"23.4",
                                             L"24.1", L"24.2", L"24.3", L"24.4",
                                             L"25.1", L"25.2", L"25.3", L"25.4",
                                             L"26.1", L"26.2", L"26.3", L"26.4",
                                             L"27.1", L"27.2", L"27.3", L"27.4",
                                             L"28.1", L"28.2", L"28.3", L"28.4",
                                             L"29.1", L"29.2", L"29.3", L"29.4",
                                             L"30.1", L"30.2", L"30.3", L"30.4"};

BOOL CPrimxzc::CheckZCService()
{
    BOOL bResult = FALSE;

    SC_HANDLE schService = NULL;
    SC_HANDLE schSCManager = NULL;
    wchar_t* zcServiceName = PRIMXZC_DEFAULTSERVICE;

    schSCManager = OpenSCManagerW(NULL, NULL, GENERIC_READ);
    if (schSCManager != NULL)
    {
        schService = OpenServiceW(schSCManager, zcServiceName, GENERIC_READ);
        if (schService != NULL)
        {
            CloseServiceHandle(schService);
            bResult = TRUE;
        }
        CloseServiceHandle(schSCManager);
    }
    return bResult;
}

BOOL CPrimxzc::CheckZCInstallPath(std::wstring& wInstallPath)
{
    // ZoneCentral Official Program install and Versiob Check
    // ZoneCentral Trial Program install and Versiob Check

    BOOL bResult = FALSE;

    CString szDefKey;

    //Check Official Version
    szDefKey.Format(_T("%s\\%s"), PRIMXZC_REGKEY_DEFAULT, PRIMXZC_REGKEY_OFFICIAL);

    for (int i = 0; i < wzOfficialVersion.size(); i++)
    {
        HKEY hKey = NULL;
        LONG lResult = 0;
        CString szKey;
        szKey.Format(_T("%s\\%s\\Install"), szDefKey.GetString(), wzOfficialVersion[i].c_str());

        lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey.GetString(), 0, KEY_READ, &hKey);
        if (lResult != ERROR_SUCCESS)
        {
            bResult = FALSE;
        }
        else {
            CString szFormat;
            DWORD dwType;
            DWORD dwBytes = 100;
            char cBuffer[100];

            memset(cBuffer, 0x00, dwBytes);
            lResult = RegQueryValueEx(hKey, L"InstallPath", 0, &dwType, (LPBYTE)cBuffer, &dwBytes);
            if (lResult != ERROR_SUCCESS)
            {
                bResult = FALSE;
            }
            else {
                szFormat.Format(_T("%s"), cBuffer);
                wInstallPath = szFormat.operator LPCWSTR();
                bResult = TRUE;
                RegCloseKey(hKey);
                break;
            }
        }

        RegCloseKey(hKey);
    }
    
    //Check Trial Version
    if (!bResult)
    {
        szDefKey.Format(_T("%s\\%s"), PRIMXZC_REGKEY_DEFAULT, PRIMXZC_REGKEY_TRIAL);
        for (int i = 0; i < wzTrialVersion.size(); i++)
        {
            LONG lResult;
            HKEY hKey = NULL;
            CString szKey;
            szKey.Format(_T("%s\\%s\\Install"), szDefKey.GetString(), wzTrialVersion[i].c_str());

            lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey.GetString(), 0, KEY_READ, &hKey);
            if (lResult != ERROR_SUCCESS)
            {
                bResult = FALSE;
            }
            else {
                CString szFormat;
                DWORD dwType;
                DWORD dwBytes = 100;
                char cBuffer[100];

                memset(cBuffer, 0x00, dwBytes);
                lResult = RegQueryValueEx(hKey, L"InstallPath", 0, &dwType, (LPBYTE)cBuffer, &dwBytes);
                if (lResult != ERROR_SUCCESS)
                {
                    bResult = FALSE;
                }
                else {
                    szFormat.Format(_T("%s"), cBuffer);
                    wInstallPath = szFormat.operator LPCWSTR();
                    bResult = TRUE;
                    RegCloseKey(hKey);
                    break;
                }
            }

            RegCloseKey(hKey);
        }
    }
    
    return bResult;
}

HRESULT CPrimxzc::Initalize() 
{
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZC API > Initalize"));

    std::wstring wFullName;

    if (!CheckZCService())
    {
        return -1;
    }
    else {
        if (!CheckZCInstallPath(wFullName))
        {
            return -1;
        }
    }
    
    wFullName += PRIMXZC_DLLFILENAME;

    char* psFullName = NULL;
    int strSize = WideCharToMultiByte(CP_ACP, 0, wFullName.c_str(), -1, NULL, 0, NULL, NULL);
    psFullName = new char[strSize];
    WideCharToMultiByte(CP_ACP, 0, wFullName.c_str(), -1, psFullName, strSize, 0, 0);

    if (access(psFullName, 0) == 0)
    {
        hDll = ::LoadLibraryA(psFullName);
    };
    delete psFullName;


    if (hDll != NULL)
    {
		HRESULT hres;

		hres = CoInitializeEx(0, COINIT_MULTITHREADED);
		if (FAILED(hres))
		{
			return -1;
		}

		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZC API > Initalize > func load"));
				

        //Function - General
        pZCAPI_ZC_Init = (ZCAPI_ZC_Init)::GetProcAddress(hDll, "ZCAPI_ZC_Init");
        pZCAPI_ZC_SetUIParams = (ZCAPI_ZC_SetUIParams)::GetProcAddress(hDll, "ZCAPI_ZC_SetUIParams");
        pZCAPI_ZC_GetFolderInfo = (ZCAPI_ZC_GetFolderInfo)::GetProcAddress(hDll, "ZCAPI_ZC_GetFolderInfo");
        pZCAPI_ZC_GetPolicy = (ZCAPI_ZC_GetPolicy)::GetProcAddress(hDll, "ZCAPI_ZC_GetPolicy");
        pZCAPI_ZC_CloseKeys = (ZCAPI_ZC_CloseKeys)::GetProcAddress(hDll, "ZCAPI_ZC_CloseKeys");
        pZCAPI_ZC_LoadZone = (ZCAPI_ZC_LoadZone)::GetProcAddress(hDll, "ZCAPI_ZC_LoadZone");
        pZCAPI_ZC_LoadAccessFile = (ZCAPI_ZC_LoadAccessFile)::GetProcAddress(hDll, "ZCAPI_ZC_LoadAccessFile");
        pZCAPI_ZC_CreateAccessFile = (ZCAPI_ZC_CreateAccessFile)::GetProcAddress(hDll, "ZCAPI_ZC_CreateAccessFile");
        pZCAPI_ZC_LoadPersonalAccessFile = (ZCAPI_ZC_LoadPersonalAccessFile)::GetProcAddress(hDll, "ZCAPI_ZC_LoadPersonalAccessFile");
        pZCAPI_ZC_LoadPersonalKey = (ZCAPI_ZC_LoadPersonalKey)::GetProcAddress(hDll, "ZCAPI_ZC_LoadPersonalKey");
        pZCAPI_ZC_GetRunTime = (ZCAPI_ZC_GetRunTime)::GetProcAddress(hDll, "ZCAPI_ZC_GetRunTime");
        pZCAPI_ZC_LoadZoneMap = (ZCAPI_ZC_LoadZoneMap)::GetProcAddress(hDll, "ZCAPI_ZC_LoadZoneMap");
        pZCAPI_ZC_CreateZoneScan = (ZCAPI_ZC_CreateZoneScan)::GetProcAddress(hDll, "ZCAPI_ZC_CreateZoneScan");
        pZCAPI_ZC_UngroupZone = (ZCAPI_ZC_UngroupZone)::GetProcAddress(hDll, "ZCAPI_ZC_UngroupZone");
        pZCAPI_ZC_GroupZone = (ZCAPI_ZC_GroupZone)::GetProcAddress(hDll, "ZCAPI_ZC_GroupZone");
        pZCAPI_ZC_CheckCertificate = (ZCAPI_ZC_CheckCertificate)::GetProcAddress(hDll, "ZCAPI_ZC_CheckCertificate");
        pZCAPI_ZC_CloseHandle = (ZCAPI_ZC_CloseHandle)::GetProcAddress(hDll, "ZCAPI_ZC_CloseHandle");
        //Function - Zones
        pZCAPI_ZN_OpenZone = (ZCAPI_ZN_OpenZone)::GetProcAddress(hDll, "ZCAPI_ZN_OpenZone");
        pZCAPI_ZN_GetZoneInfo = (ZCAPI_ZN_GetZoneInfo)::GetProcAddress(hDll, "ZCAPI_ZN_GetZoneInfo");
        pZCAPI_ZN_GetAccesses = (ZCAPI_ZN_GetAccesses)::GetProcAddress(hDll, "ZCAPI_ZN_GetAccesses");
        pZCAPI_ZN_CloseZone = (ZCAPI_ZN_CloseZone)::GetProcAddress(hDll, "ZCAPI_ZN_CloseZone");
        pZCAPI_ZN_SaveZone = (ZCAPI_ZN_SaveZone)::GetProcAddress(hDll, "ZCAPI_ZN_SaveZone");
        pZCAPI_ZN_CloseHandle = (ZCAPI_ZN_CloseHandle)::GetProcAddress(hDll, "ZCAPI_ZN_CloseHandle");
        //Function - Access Files
        pZCAPI_AF_OpenAccessFile = (ZCAPI_AF_OpenAccessFile)::GetProcAddress(hDll, "ZCAPI_AF_OpenAccessFile");
        pZCAPI_AF_GetFileInfo = (ZCAPI_AF_GetFileInfo)::GetProcAddress(hDll, "ZCAPI_AF_GetFileInfo");
        pZCAPI_AF_ChangeAttributes = (ZCAPI_AF_ChangeAttributes)::GetProcAddress(hDll, "ZCAPI_AF_ChangeAttributes");
        pZCAPI_AF_GetAccesses = (ZCAPI_AF_GetAccesses)::GetProcAddress(hDll, "ZCAPI_AF_GetAccesses");
        pZCAPI_AF_GetSOSPassword = (ZCAPI_AF_GetSOSPassword)::GetProcAddress(hDll, "ZCAPI_AF_GetSOSPassword");
        pZCAPI_AF_RenewSOSPassword = (ZCAPI_AF_RenewSOSPassword)::GetProcAddress(hDll, "ZCAPI_AF_RenewSOSPassword");
        pZCAPI_AF_GetOTA = (ZCAPI_AF_GetOTA)::GetProcAddress(hDll, "ZCAPI_AF_GetOTA");
        pZCAPI_AF_CloseAccessFile = (ZCAPI_AF_CloseAccessFile)::GetProcAddress(hDll, "ZCAPI_AF_CloseAccessFile");
        pZCAPI_ZN_SaveAccessFile = (ZCAPI_ZN_SaveAccessFile)::GetProcAddress(hDll, "ZCAPI_ZN_SaveAccessFile");
        pZCAPI_AF_CloseHandle = (ZCAPI_AF_CloseHandle)::GetProcAddress(hDll, "ZCAPI_AF_CloseHandle");
        //Function - Accessess
        pZCAPI_AC_GetAccesses = (ZCAPI_AC_GetAccesses)::GetProcAddress(hDll, "ZCAPI_AC_GetAccesses");
        pZCAPI_AC_GetAccessInfo = (ZCAPI_AC_GetAccessInfo)::GetProcAddress(hDll, "ZCAPI_AC_GetAccessInfo");
        pZCAPI_AC_AddAccessPassword = (ZCAPI_AC_AddAccessPassword)::GetProcAddress(hDll, "ZCAPI_AC_AddAccessPassword");
        pZCAPI_AC_AddAccessCertificate = (ZCAPI_AC_AddAccessCertificate)::GetProcAddress(hDll, "ZCAPI_AC_AddAccessCertificate");
        pZCAPI_AC_AddIndirectAccess = (ZCAPI_AC_AddIndirectAccess)::GetProcAddress(hDll, "ZCAPI_AC_AddIndirectAccess");
        pZCAPI_AC_RemoveAccess = (ZCAPI_AC_RemoveAccess)::GetProcAddress(hDll, "ZCAPI_AC_RemoveAccess");
        pZCAPI_AC_ChangeAccessLabel = (ZCAPI_AC_ChangeAccessLabel)::GetProcAddress(hDll, "ZCAPI_AC_ChangeAccessLabel");
        pZCAPI_AC_ChangeAccessRole = (ZCAPI_AC_ChangeAccessRole)::GetProcAddress(hDll, "ZCAPI_AC_ChangeAccessRole");
        pZCAPI_AC_ChangeAccessAttributes = (ZCAPI_AC_ChangeAccessAttributes)::GetProcAddress(hDll, "ZCAPI_AC_ChangeAccessAttributes");
        pZCAPI_AC_ChangeAccessPassword = (ZCAPI_AC_ChangeAccessPassword)::GetProcAddress(hDll, "ZCAPI_AC_ChangeAccessPassword");
        pZCAPI_AC_LoadFromIndirectAccess = (ZCAPI_AC_LoadFromIndirectAccess)::GetProcAddress(hDll, "ZCAPI_AC_LoadFromIndirectAccess");
        pZCAPI_AC_CloseHandle = (ZCAPI_AC_CloseHandle)::GetProcAddress(hDll, "ZCAPI_AC_CloseHandle");
        //Function - Personal Access Key
        pZCAPI_PAF_Exists = (ZCAPI_PAF_Exists)::GetProcAddress(hDll, "ZCAPI_PAF_Exists");
        pZCAPI_PAF_GetAccess = (ZCAPI_PAF_GetAccess)::GetProcAddress(hDll, "ZCAPI_PAF_GetAccess");
        pZCAPI_PAF_CloseHandle = (ZCAPI_PAF_CloseHandle)::GetProcAddress(hDll, "ZCAPI_PAF_CloseHandle");
        //Function - Personal Key
        pZCAPI_PK_Exists = (ZCAPI_PK_Exists)::GetProcAddress(hDll, "ZCAPI_PK_Exists");
        pZCAPI_PK_GetKeyInfo = (ZCAPI_PK_GetKeyInfo)::GetProcAddress(hDll, "ZCAPI_PK_GetKeyInfo");
        pZCAPI_PK_CloseHandle = (ZCAPI_PK_CloseHandle)::GetProcAddress(hDll, "ZCAPI_PK_CloseHandle");
        //Function - Run-Time
        pZCAPI_RT_CloseAll = (ZCAPI_RT_CloseAll)::GetProcAddress(hDll, "ZCAPI_RT_CloseAll");
        pZCAPI_RT_CloseAllZones = (ZCAPI_RT_CloseAllZones)::GetProcAddress(hDll, "ZCAPI_RT_CloseAllZones");
        pZCAPI_RT_CloseAllKeys = (ZCAPI_RT_CloseAllKeys)::GetProcAddress(hDll, "ZCAPI_RT_CloseAllKeys");
        pZCAPI_RT_AddPasswordKey = (ZCAPI_RT_AddPasswordKey)::GetProcAddress(hDll, "ZCAPI_RT_AddPasswordKey");
        pZCAPI_RT_AddKeyFileKeys = (ZCAPI_RT_AddKeyFileKeys)::GetProcAddress(hDll, "ZCAPI_RT_AddKeyFileKeys");
        pZCAPI_RT_GetVersion = (ZCAPI_RT_GetVersion)::GetProcAddress(hDll, "ZCAPI_RT_GetVersion");
        pZCAPI_RT_CloseHandle = (ZCAPI_RT_CloseHandle)::GetProcAddress(hDll, "ZCAPI_RT_CloseHandle");
        pZCAPI_RT_ClosePasswordKey = (ZCAPI_RT_ClosePasswordKey)::GetProcAddress(hDll, "ZCAPI_RT_ClosePasswordKey");
        pZCAPI_RT_CloseKeyFileKey = (ZCAPI_RT_CloseKeyFileKey)::GetProcAddress(hDll, "ZCAPI_RT_CloseKeyFileKey");
        pZCAPI_RT_CloseZone = (ZCAPI_RT_CloseZone)::GetProcAddress(hDll, "ZCAPI_RT_CloseZone");
        //Function - ZoneMap
        pZCAPI_ZM_GetNbZones = (ZCAPI_ZM_GetNbZones)::GetProcAddress(hDll, "ZCAPI_ZM_GetNbZones");
        pZCAPI_ZM_GetZonePath = (ZCAPI_ZM_GetZonePath)::GetProcAddress(hDll, "ZCAPI_ZM_GetZonePath");
        pZCAPI_ZM_CloseHandle = (ZCAPI_ZM_CloseHandle)::GetProcAddress(hDll, "ZCAPI_ZM_CloseHandle");
        //Function - ZoneScan
        //pZCAPI_ZS_StartScan = (ZCAPI_ZS_StartScan)::GetProcAddress(hDll, "ZCAPI_ZS_StartScan");
        pZCAPI_ZS_GetNbZones = (ZCAPI_ZS_GetNbZones)::GetProcAddress(hDll, "ZCAPI_ZS_GetNbZones");
        pZCAPI_ZS_GetZonePath = (ZCAPI_ZS_GetZonePath)::GetProcAddress(hDll, "ZCAPI_ZS_GetZonePath");
        pZCAPI_ZS_CloseHandle = (ZCAPI_ZS_CloseHandle)::GetProcAddress(hDll, "ZCAPI_ZS_CloseHandle");
        pZCAPI_ScanProgress = (ZCAPI_ScanProgress)::GetProcAddress(hDll, "ZCAPI_ScanProgress");
        //Function - Misc
        pZCAPI_FormatMessage = (ZCAPI_FormatMessage)::GetProcAddress(hDll, "ZCAPI_FormatMessage");

        //ZoneCentral init
		m_zcHdl = (ZCAPI_ZONECENTRAL_HANDLE)hDll;
        HRESULT hResult = pZCAPI_ZC_Init(0, &m_zcHdl);
		if(FAILED(hResult)){
			std::wstring wMsg;
			ZoneCentral_ErrorMessage(hResult, wMsg);
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZC API > pZCAPI_ZC_Init fail > %s (%x)"), wMsg.c_str(), hResult);
		}
		else {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZC API > pZCAPI_ZC_Init OK > 0x%x"), hResult);
		}  

        return hResult;
    }

    return -1;
}

CPrimxzc::~CPrimxzc() {
    if (m_zcHdl != NULL) {
        HRESULT hResult = pZCAPI_ZC_CloseHandle(m_zcHdl);
    };

    if (hDll != NULL) {
        ::FreeLibrary(hDll);
    }

    if (m_pwzPersonalAccessList != NULL)
    {
        delete m_pwzPersonalAccessList;
    }

    free(m_zcAIUser.pwzLabel);
    free(m_zcAIUser.pucCertificate);

}

void CPrimxzc::ZoneCentral_ErrorMessage(HRESULT erhResult, std::wstring& pwzMsg)
{
    switch (erhResult)
    {
        case E_INVALIDARG:
        case E_HANDLE:
        case E_DENIED:
        case E_TOO_SMALL:
        case E_FILE_NOT_FOUND:
        case E_FILE_EXISTS:
        case E_BAD_PATHNAME:
        case E_DIR_NOT_EMPTY:
        case E_NO_MORE_FILES:
        case E_NOT_FOUND:
        case E_OUTOFMEMORY:
        {
            WCHAR* pwzGetMessage = NULL;
            unsigned int cch = MAX_PATH * sizeof(WCHAR);

            pwzGetMessage = new WCHAR[cch];
            memset(pwzGetMessage, 0x00, cch);

            //wcscpy(pwzGetMessage, L"OK Very!!!!");
            pZCAPI_FormatMessage(erhResult, pwzGetMessage, &cch);

            pwzMsg = std::wstring(pwzGetMessage);
            delete pwzGetMessage;

        } break;
        default:
        {
           // pwzMsg = L"Unknown error";

			WCHAR* pwzGetMessage = NULL;
			unsigned int cch = MAX_PATH * sizeof(WCHAR);

			pwzGetMessage = new WCHAR[cch];
			memset(pwzGetMessage, 0x00, cch);

			//wcscpy(pwzGetMessage, L"OK Very!!!!");
			pZCAPI_FormatMessage(erhResult, pwzGetMessage, &cch);

			pwzMsg = std::wstring(pwzGetMessage);
			delete pwzGetMessage;
        } break;
    }
}

HRESULT CPrimxzc::ZoneCentral_AccessFileUser(const WCHAR* pwzCerFiles)
{
    HRESULT hResult = -1;
    unsigned int ce = 0;
    
    ZCAPI_ACCESSFILE_HANDLE zAccessFilehdl=NULL;
    ZCAPI_ACCESSES_HANDLE zAccesshdl=NULL;
    GUID *pGuidAccesses = NULL;

    m_zcAIUser.cbSize = sizeof(m_zcAIUser);

    if (!m_zcHdl) return hResult;  // case ZONECENTRAL not load

    hResult = pZCAPI_ZC_LoadAccessFile(m_zcHdl, pwzCerFiles, &zAccessFilehdl);
    if (FAILED(hResult)) { return hResult; };

    hResult = pZCAPI_AF_GetAccesses(zAccessFilehdl, &zAccesshdl);
    if (FAILED(hResult)) { return hResult; };

    pZCAPI_AC_GetAccesses(zAccesshdl, NULL, &ce);
    pGuidAccesses = (GUID*)malloc(ce * sizeof(*pGuidAccesses));
    pZCAPI_AC_GetAccesses(zAccesshdl, pGuidAccesses, &ce);
    if (FAILED(hResult)) { return hResult; };

    hResult = pZCAPI_AC_GetAccessInfo(zAccesshdl, pGuidAccesses, &m_zcAIUser);
    if (FAILED(hResult)) { return hResult; };

    m_zcAIUser.pwzLabel = (WCHAR*)malloc((m_zcAIUser.cchLabel + 1) * sizeof(m_zcAIUser.pwzLabel[0]));
    if (m_zcAIUser.cbCertificate != 0)
    {
        m_zcAIUser.pucCertificate = (unsigned char*)malloc(m_zcAIUser.cbCertificate * sizeof(m_zcAIUser.pucCertificate[0]));
    }

    hResult = pZCAPI_AC_GetAccessInfo(zAccesshdl, pGuidAccesses, &m_zcAIUser);
    if (FAILED(hResult)) { return hResult; }


    pZCAPI_AF_CloseHandle(zAccessFilehdl);

    free(pGuidAccesses);

    return hResult;
}

HRESULT CPrimxzc::ZoneCentral_AccessFileList()
{
    HRESULT hResult = -1;
    LONG lPolicy;
    VARIANT varValue;
    CString szFormat;
    WCHAR* pwzPath;
    WCHAR* pwzName;

    if (!m_zcHdl) return hResult;  // case ZONECENTRAL not load

    lPolicy = 121;  //Policy number P121 "P121 Primary Location"
    hResult = pZCAPI_ZC_GetPolicy( m_zcHdl, lPolicy, &varValue);
    if (FAILED(hResult)) { return hResult; }
    pwzPath = varValue.bstrVal;

	WCHAR* token = wcstok(pwzPath, L";");
	if (token != NULL) {
		wcscpy(pwzPath, token);
	}

    lPolicy = 126;  //Policy number "P126 Personal access list name format"
    hResult = pZCAPI_ZC_GetPolicy(m_zcHdl, lPolicy, &varValue);
    if (FAILED(hResult)) { return hResult; }
    pwzName = varValue.bstrVal;

    szFormat.Format(_T("%s\\%s.zaf"), pwzPath, pwzName);
    if (m_pwzPersonalAccessList == NULL)
    {
        m_pwzPersonalAccessList = (WCHAR*)malloc(MAX_PATH * sizeof(WCHAR));
        memset(m_pwzPersonalAccessList, 0x00, MAX_PATH * sizeof(WCHAR));
    }

    wcscpy(m_pwzPersonalAccessList, (WCHAR*)szFormat.GetBuffer());

    return hResult;
}

HRESULT CPrimxzc::ZoneCentral_Version()
{
    HRESULT hResult = -1;
    ZCAPI_RUNTIME_HANDLE zrthdl;
    unsigned int uMajor;
    unsigned int uMinor;
    unsigned int uBuild;
    WCHAR* pwzVersion = NULL;
    unsigned int cchVersion;

    //Get Run-Time Handle
    hResult = pZCAPI_ZC_GetRunTime(m_zcHdl, ZCAPI_SESSIONID_CURRENT_SESSION, &zrthdl);
    if (FAILED(hResult)) { 
		std::wstring wMsg;
		ZoneCentral_ErrorMessage(hResult, wMsg);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZC API > pZCAPI_ZC_GetRunTime fail > %s"), wMsg.c_str());
		
		return hResult; 
	};

    hResult = pZCAPI_RT_GetVersion(zrthdl, NULL, NULL, NULL, NULL, &cchVersion);
    pwzVersion = (WCHAR*)malloc(cchVersion * sizeof(*pwzVersion));

    hResult = pZCAPI_RT_GetVersion(zrthdl, &uMajor, &uMinor, &uBuild, pwzVersion, &cchVersion);
    if (SUCCEEDED(hResult))
    {
        //gbRMConfig.Get_RMCfg_GeneralInfo(&gbRMConfig.m_stGeneralinfo);

        //wcscpy(gbRMConfig.m_stGeneralinfo.pwzVal_ZCVersion, pwzVersion);
        
        //gbRMConfig.Set_RMCfg_GeneralInfo(&gbRMConfig.m_stGeneralinfo);
    }

    delete pwzVersion;

    return hResult;

}

ZCAPI_FOLDER_TYPE CPrimxzc::SetCloseZone(CString strDstPath)
{
	ZCAPI_FOLDER_TYPE enType = ZCAPI_FOLDER_UNKNOWN;

	if (NULL != pZCAPI_ZC_Init && NULL != pZCAPI_ZC_GetFolderInfo && NULL != pZCAPI_ZC_CloseHandle) {

		ZCAPI_ZONECENTRAL_HANDLE zcHdl = NULL;
		HRESULT hResult = pZCAPI_ZC_Init(0, &zcHdl);

		if (SUCCEEDED(hResult)) {
			unsigned int cch = 0;
			hResult = pZCAPI_ZC_GetFolderInfo(zcHdl, strDstPath, &enType, NULL, NULL, &cch);
			if (SUCCEEDED(hResult)) {
				if (ZCAPI_FOLDER_ENCRYPTED_ZONE == enType) {
					ZCAPI_RUNTIME_HANDLE zcRunHdl = NULL;
					if (S_OK == pZCAPI_ZC_GetRunTime(zcHdl, ZCAPI_SESSIONID_CURRENT_SESSION, &zcRunHdl)) {

						hResult = pZCAPI_RT_CloseAll(zcRunHdl);

						//if (SUCCEEDED(hResult)) {
						//	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZC API > SetCloseZone > OK"));
						//}
						pZCAPI_RT_CloseHandle(zcRunHdl);
					}
				}	
			}
		}
	}
	
	return enType;
}

ZCAPI_FOLDER_TYPE  CPrimxzc::GetFolderInfo(CString strDstPath)
{
	ZCAPI_FOLDER_TYPE enType = ZCAPI_FOLDER_UNKNOWN;

	if (NULL != pZCAPI_ZC_Init && NULL != pZCAPI_ZC_GetFolderInfo && NULL != pZCAPI_ZC_CloseHandle) {

		ZCAPI_ZONECENTRAL_HANDLE zcHdl = NULL;
		HRESULT hResult = pZCAPI_ZC_Init(0, &zcHdl);
		if (SUCCEEDED(hResult)) {

			unsigned int cch = 0;
			hResult = pZCAPI_ZC_GetFolderInfo(zcHdl, strDstPath, NULL, NULL, NULL, &cch);
			
			if (SUCCEEDED(hResult)) {
				WCHAR* pwzZonePath = new WCHAR[cch];
				hResult = pZCAPI_ZC_GetFolderInfo(zcHdl, strDstPath, &enType, NULL, pwzZonePath, &cch);

				if (FAILED(hResult)) {
					std::wstring wMsg;
					ZoneCentral_ErrorMessage(hResult, wMsg);
					CString strMsg;
					strMsg.Format(L"%s [0x%x]", wMsg.c_str(), hResult);
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZC API > GetFolderInfo > pZCAPI_ZC_GetFolderInfo > %s"), strMsg);
				}
				if (SUCCEEDED(hResult)) {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZC API > GetFolderInfo > pZCAPI_ZC_GetFolderInfo [%d] > pwzZonePath:%s"), enType, pwzZonePath);
				}

				delete[] pwzZonePath;
			}
			else {
				std::wstring wMsg;
				ZoneCentral_ErrorMessage(hResult, wMsg);
				CString strMsg;
				strMsg.Format(L"%s [0x%x]", wMsg.c_str(), hResult);
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZC API > GetFolderInfo > pZCAPI_ZC_GetFolderInfo > %s"), strMsg);
			}
		}
		else {
			std::wstring wMsg;
			ZoneCentral_ErrorMessage(hResult, wMsg);
			CString strMsg;
			strMsg.Format(L"%s [0x%x]", wMsg.c_str(), hResult);
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZC API > GetFolderInfo > pZCAPI_ZC_Init > %s"), strMsg);
		}
	}
	else {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZC API > SetCloseZone > NULL Func"));

	}	

	return enType;
}