#pragma once

#if defined(_WIN32) || defined(_WIN64)
typedef unsigned __int16    UINT16_T;
typedef unsigned __int32    UINT32_T;
typedef unsigned __int64    UINT64_T;
typedef __int16    INT16_T;
typedef __int32    INT32_T;
typedef __int64    INT64_T;
#else
typedef uint16_t    UINT16_T;
typedef uint32_t    UINT32_T;
typedef uint64_t   UINT64_T;
typedef int16_t    INT16_T;
typedef int32_t    INT32_T;
typedef int64_t    INT64_T;
typedef u_int    UINT;
typedef char     TCHAR;
#endif

#if defined(_WIN32) || defined(_WIN64)
#ifndef _CountOfArray
#define _CountOfArray(array)	(sizeof(array)/sizeof(array[0]))
#endif

#ifndef _CountOfArrayEx
#define _CountOfArrayEx(array) (_msize(array)/sizeof(array[0]))
#endif

#ifndef _SafeDelete
#define _SafeDelete(p)			if(NULL != p){ delete (p); (p)=NULL; }
#endif

#ifndef _SafeDeleteArray
#define _SafeDeleteArray(p)	if(NULL != p){ delete [] (p); (p)=NULL; }
#endif

#ifndef _Random
#define _Random(min, max) ((rand()%(int)(((max) + 1)-(min)))+ (min))
#endif
#endif



#define PRIMX_CONFIG_FILE_PATH							_T("PRIMXAgentConfig.ini")
#define PRIMX_CONFIG_SERVICE_SESSION					_T("SERVICE-CONFIG")
#define PRIMX_CONFIG_SERVER_ADDR						_T("SERVER-ADDR")	
#define PRIMX_CONFIG_SERVER_PORT						_T("SERVER-PORT")	
#define PRIMX_CONFIG_UPDATE_PORT						_T("UPDATE-PORT")	
#define PRIMX_CONFIG_AUTH_PORT							_T("AUTH-PORT")	


#define STRINGA(s)										#s 
#define STRING(s)										_T(#s) 
#define BLANK_STR										_T("")
#define BLANK_STR_A										""
#define R_RETURN_LINE_STR								_T("\r")
#define N_RETURN_LINE_STR								_T("\n")
#define RETURN_LINE_STR									_T("\r\n")

// 오류 코드
#define PRIMX_ERROR_SUCCESS								1					// 성공
#define PRIMX_ERROR_XML_10000							10000		        // XML 형식 오류
#define PRIMX_ERROR_XML_10001							10001               // 지정된 요소(Element)가 존재 하지 않은 경우
#define PRIMX_ERROR_XML_10002							10002               // 지정된 명령 코드가 아닌 경우


// 시스템 구분
#define PRIMX_SYS_TYPE_USER								1		// 사용지 PC
#define PRIMX_SYS_TYPE_COMM								2		// 공용 PC
#define PRIMX_SYS_TYPE_USER_SETUP						3		// 사용지 PC (설치 시 설정)
#define PRIMX_SYS_TYPE_COMM_SETUP						4		// 공용 PC (설치 시 설정)


#define PRIMX_ENC_MONITOR_NAME							_T("PRIMXEncMonitor.exe")
#define PRIMX_AGNET_CS_NAME								_T("PRIMXAgentCS.exe")
#define PRIMX_AGNET_MSG_NAME							_T("PRIMXAgentCS.exe")
#define PRIMX_AGNET_CS_RUN_CYCLE						10

#define PRIMX_IPC_AGNET_CS_KEY							_T("PRIMX_IPC_AGNET_CS_KEY")
#define PRIMX_IPC_ALERT_MSG_KEY							_T("PRIMX_IPC_ALERT_MSG_KEY")


#define PRIMX_PRODUCT_TYPE_ZONECENTRAL					1
#define PRIMX_PRODUCT_TYPE_ORIZON						2
#define PRIMX_PRODUCT_TYPE_CRYHOD						3
#define PRIMX_PRODUCT_TYPE_AGENT						4


#define PRIMX_SYS_MOD_WIN_VERSION						(1 << 0)
#define PRIMX_SYS_MOD_NIC								(1 << 1)
#define PRIMX_SYS_MOD_DISK								(1 << 2)
#define PRIMX_SYS_NETBIOS								(1 << 3)
#define PRIMX_SYS_SHARE_PC								(1 << 4)
		
#define PRIMX_LOG_CYCLE_MIN								(60 * 5)


#define PRIMX_EXPAND_COMMAND_EX001						_T("EX001")	// 등록된 Agent ID가 없는 경우 시스템 정보를 등록하도록 명령
#define PRIMX_EXPAND_COMMAND_EX002						_T("EX002")	// 등록된 사용자ID가 존재하지 않는 경우 사용자ID 재등록 명령
#define PRIMX_EXPAND_COMMAND_EX003						_T("EX003")	// PRIMX Agent 삭제 명령
#define PRIMX_EXPAND_COMMAND_EX004						_T("EX004")	// 사용자PC로 변경
#define PRIMX_EXPAND_COMMAND_EX005						_T("EX005")	// 공용PC로 변경



typedef struct _PRIMX_EVENT_DATA_TABLE_
{
	CString			strData;
	CString			strEventCode;

}PRIMX_EVENT_DATA_TABLE;

typedef struct _PRIMX_EVENT_DATA_TABLE2_
{
	UINT			nCode;
	CString			strEventCode;

}PRIMX_EVENT_DATA_TABLE2;

// 사용자 정보
typedef struct _PRIMX_USER_DATA_
{
	CString			strUserID;
	CString			strUserName;
	CString			strTel;
	CString			strCellPhone;
	CString			strExtNo;
	CString			strMail;
	CString			strDepartment;

	void Clear() {
		strUserID = _T("");
		strUserName = _T("");
		strTel = _T("");
		strCellPhone = _T("");
		strExtNo = _T("");
		strMail = _T("");
		strDepartment = _T("");
	};

	_PRIMX_USER_DATA_()
	{
		Clear();
	};

	void operator=(const _PRIMX_USER_DATA_& UserData)
	{
		strUserID = UserData.strUserID;
		strUserName = UserData.strUserName;
		strTel = UserData.strTel;
		strCellPhone = UserData.strCellPhone;
		strExtNo = UserData.strExtNo;
		strMail = UserData.strMail;
		strDepartment = UserData.strDepartment;
	}


}PRIMX_USER_DATA;

// Windows 버전 정보
typedef struct _PRIMX_WINDOWS_VERSION_DATA_
{
	CString strVersionFullName;
	CString strUUID;
	UINT nMajorVersion;
	UINT nMinorVersion;
	CString strBuildNumber;
	UINT nServicePackMajor;
	UINT nServicePackMinor;
	UINT nProcessorArchitecture;
	CString strReleaseID;

	void Clear() {
		strVersionFullName = _T("");
		strUUID = _T("");
		nMajorVersion = 0;
		nMinorVersion = 0;
		strBuildNumber = _T("");
		nServicePackMajor = 0;
		nServicePackMinor = 0;
		nProcessorArchitecture = 0;
		strReleaseID = _T("");
	};

	void Trim() {
		strVersionFullName.Trim();
		strUUID.Trim();
		strBuildNumber.Trim();
		strReleaseID.Trim();
	};

	_PRIMX_WINDOWS_VERSION_DATA_()
	{
		Clear();
	};

	void operator=(const _PRIMX_WINDOWS_VERSION_DATA_& Data)
	{
		strVersionFullName = Data.strVersionFullName;
		strUUID = Data.strUUID;
		nMajorVersion = Data.nMajorVersion;
		nMinorVersion = Data.nMinorVersion;
		strBuildNumber = Data.strBuildNumber;
		nServicePackMajor = Data.nServicePackMajor;
		nServicePackMinor = Data.nServicePackMinor;
		nProcessorArchitecture = Data.nProcessorArchitecture;
		strReleaseID = Data.strReleaseID;
	};

	BOOL Compare(const _PRIMX_WINDOWS_VERSION_DATA_& Data)
	{
		if (0 != strVersionFullName.CompareNoCase(Data.strVersionFullName)) {
			return FALSE;
		}
		if (0 != strUUID.CompareNoCase(Data.strUUID)) {
			return FALSE;
		}
		if (0 != strBuildNumber.CompareNoCase(Data.strBuildNumber)) {
			return FALSE;
		}
		if (0 != strReleaseID.CompareNoCase(Data.strReleaseID)) {
			return FALSE;
		}
		if (nMajorVersion != Data.nMajorVersion) {
			return FALSE;
		}
		if (nMinorVersion != Data.nMinorVersion) {
			return FALSE;
		}
		if (nServicePackMajor != Data.nServicePackMajor) {
			return FALSE;
		}
		if (nServicePackMinor != Data.nServicePackMinor) {
			return FALSE;
		}
		if (nProcessorArchitecture != Data.nProcessorArchitecture) {
			return FALSE;
		}
		return TRUE;
	};

}PRIMX_WINDOWS_VERSION_DATA;


// 하드디스크 정보
typedef struct _PRIMX_DISK_DRIVE_DATA_
{
	CString strName;
	UINT nType;
	CString strVolumeName;
	CString strFileSystemName;
	UINT nTotalSize;
	UINT nFreeSize;
	
	void Clear() {
		strName = _T("");
		nType = 0;
		strVolumeName = _T("");
		strFileSystemName = _T("");
		nTotalSize = 0;
		nFreeSize = 0;
	};

	void Trim() {
		strName.Trim();
		strVolumeName.Trim();
		strFileSystemName.Trim();
	};

	_PRIMX_DISK_DRIVE_DATA_()
	{
		Clear();
	};

	void operator=(const _PRIMX_DISK_DRIVE_DATA_& Data)
	{
		strName = Data.strName;
		nType = Data.nType;
		strVolumeName = Data.strVolumeName;
		strFileSystemName = Data.strFileSystemName;
		nTotalSize = Data.nTotalSize;
		nFreeSize = Data.nFreeSize;
	};

	BOOL Compare(const _PRIMX_DISK_DRIVE_DATA_* pData)
	{
		if(NULL == pData) return FALSE;

		if (0 != strName.CompareNoCase(pData->strName)) {
			return FALSE;
		}
		if (0 != strVolumeName.CompareNoCase(pData->strVolumeName)) {
			return FALSE;
		}
		if (0 != strFileSystemName.CompareNoCase(pData->strFileSystemName)) {
			return FALSE;
		}
		if (nTotalSize != pData->nTotalSize) {
			return FALSE;
		}
		if (nType != pData->nType) {
			return FALSE;
		}

		if (nFreeSize != pData->nFreeSize) {

			UINT nTemp = UINT(float(nTotalSize) * float(10.0 / 100.0));
			if (nFreeSize < pData->nFreeSize) {
				if (nTemp < (pData->nFreeSize - nFreeSize)) {
					return FALSE;
				}
			}
			else {
				if (nTemp < (nFreeSize - pData->nFreeSize)) {
					return FALSE;
				}
			}
		}

		return TRUE;
	};
	
}PRIMX_DISK_DRIVE_DATA;

// NIC 정보
typedef struct _PRIMX_NIC_DATA_
{
	CString strAdapterGUID;
	CString strDescription;
	CString strMAC;
	CString strGW;
	CString strDNS;
	CString strDHCPServer;
	CString strIP;
	UINT    nEnableDHCP;

	void Clear() {
		strAdapterGUID = _T("");
		strDescription = _T("");
		strMAC = _T("");
		strGW = _T("");
		strDNS = _T("");
		strDHCPServer = _T("");
		strIP = _T("");
		nEnableDHCP = FALSE;
	};

	void Trim() {
		strAdapterGUID.Trim();
		strDescription.Trim();
		strMAC.Trim();
		strGW.Trim();
		strDNS.Trim();
		strDHCPServer.Trim();
		strIP.Trim();
	};

	_PRIMX_NIC_DATA_()
	{
		Clear();
	};

	void operator=(const _PRIMX_NIC_DATA_& Data)
	{
		strAdapterGUID = Data.strAdapterGUID;
		strDescription = Data.strDescription;
		strMAC = Data.strMAC;
		strGW = Data.strGW;
		strDNS = Data.strDNS;
		strDHCPServer = Data.strDHCPServer;
		strIP = Data.strIP;
		nEnableDHCP = Data.nEnableDHCP;
	};

	BOOL Compare(const _PRIMX_NIC_DATA_* pData)
	{
		if(NULL == pData) return FALSE;

		if (0 != strAdapterGUID.CompareNoCase(pData->strAdapterGUID)) {
			return FALSE;
		}
		if (0 != strDescription.CompareNoCase(pData->strDescription)) {
			return FALSE;
		}
		if (0 != strMAC.CompareNoCase(pData->strMAC)) {
			return FALSE;
		}
		if (0 != strGW.CompareNoCase(pData->strGW)) {
			return FALSE;
		}
		if (0 != strDNS.CompareNoCase(pData->strDNS)) {
			return FALSE;
		}
		if (0 != strDHCPServer.CompareNoCase(pData->strDHCPServer)) {
			return FALSE;
		}
		if (0 != strIP.CompareNoCase(pData->strIP)) {
			return FALSE;
		}
		if (nEnableDHCP != pData->nEnableDHCP) {
			return FALSE;
		}
		return TRUE;
	};

}PRIMX_NIC_DATA;

// NetBIOS 정보
typedef struct _PRIMX_NETBIOS_DATA_
{
	CString strComputerName;
	CString strWorkGroupName;

	void Clear() {
		strComputerName = _T("");
		strWorkGroupName = _T("");
	};
	void Trim() {
		strComputerName.Trim();
		strWorkGroupName.Trim();
	};
	_PRIMX_NETBIOS_DATA_()
	{
		Clear();
	};

	void operator=(const _PRIMX_NETBIOS_DATA_& Data)
	{
		strComputerName = Data.strComputerName;
		strWorkGroupName = Data.strWorkGroupName;
	};

	BOOL Compare(const _PRIMX_NETBIOS_DATA_& Data)
	{
		if (0 != strComputerName.CompareNoCase(Data.strComputerName)) {
			return FALSE;
		}
		if (0 != strWorkGroupName.CompareNoCase(Data.strWorkGroupName)) {
			return FALSE;
		}
		return TRUE;
	};

}PRIMX_NETBIOS_DATA;


// 시스템 정보
typedef struct _PRIMX_SYSTEM_DATA_
{
	PRIMX_WINDOWS_VERSION_DATA		WindowsVersion;
	CList<PRIMX_DISK_DRIVE_DATA*>	DiskDriveList;
	CList<PRIMX_NIC_DATA*>			NICList;
	PRIMX_NETBIOS_DATA				NetBiosData;
	UINT							nSharePC;
	
	void Clear() {

		nSharePC = 0;		
		POSITION Pos = DiskDriveList.GetHeadPosition();
		while (NULL != Pos) {
			PRIMX_DISK_DRIVE_DATA* pData = DiskDriveList.GetNext(Pos);
			if (NULL != pData) {
				delete pData;
				pData = NULL;
			}
		}
		DiskDriveList.RemoveAll();

		Pos = NICList.GetHeadPosition();
		while (NULL != Pos) {
			PRIMX_NIC_DATA* pData = NICList.GetNext(Pos);
			if (NULL != pData) {
				delete pData;
				pData = NULL;
			}
		}
		NICList.RemoveAll();

		NetBiosData.Clear();
	};

	_PRIMX_SYSTEM_DATA_()
	{
		Clear();
	};
	~_PRIMX_SYSTEM_DATA_()
	{
		Clear();
	};
	void operator=(const _PRIMX_SYSTEM_DATA_& Data)
	{
		Clear();

		WindowsVersion = Data.WindowsVersion;
		NetBiosData = Data.NetBiosData;
		nSharePC = Data.nSharePC;
		
		POSITION Pos = Data.DiskDriveList.GetHeadPosition();
		while (NULL != Pos) {
			PRIMX_DISK_DRIVE_DATA* pData = Data.DiskDriveList.GetNext(Pos);
			if (NULL != pData) {
				PRIMX_DISK_DRIVE_DATA* pSaveData = new PRIMX_DISK_DRIVE_DATA;
				*pSaveData = *pData;
				if (NULL == DiskDriveList.AddTail(pSaveData)) {
					delete pSaveData;
					pSaveData = NULL;
				}
			}
		}
		Pos = Data.NICList.GetHeadPosition();
		while (NULL != Pos) {
			PRIMX_NIC_DATA* pData = Data.NICList.GetNext(Pos);
			if (NULL != pData) {
				PRIMX_NIC_DATA* pSaveData = new PRIMX_NIC_DATA;
				*pSaveData = *pData;
				if (NULL == NICList.AddTail(pSaveData)) {
					delete pSaveData;
					pSaveData = NULL;
				}
			}
		}
	};


}PRIMX_SYSTEM_DATA;


// 사용자 정보
typedef struct _PRIMX_PRODUCT_DATA_
{
	BOOL		bInstall;
	CString		strDisplayVersionEx;
	CString		strDisplayName;
	CString		strUninstallString;
	CString		strDisplayVersion;
	CString		strInstallLocation;
	CString		strInstallDate;
	CString		strLastPolicyDate;
	CString		strLastPolicyID;
	CString		strPolictSet;
	CString		strPackageMaster;

	DWORD		dwVersionMajor;
	DWORD		dwVersionMinor;
	DWORD		dwVersion;

	void Clear() {
		bInstall = FALSE;
		strDisplayVersionEx = _T("");
		strDisplayName = _T("");
		strUninstallString = _T("");
		strDisplayVersion = _T("");
		strInstallLocation = _T("");
		strInstallDate = _T("");
		strLastPolicyDate = _T("");
		strLastPolicyID = _T("");
		strPolictSet = _T("");
		strPackageMaster = _T("");
		dwVersionMajor = 0;
		dwVersionMinor = 0;
		dwVersion = 0;
	};

	_PRIMX_PRODUCT_DATA_()
	{
		Clear();
	};

	void operator=(const _PRIMX_PRODUCT_DATA_& Data)
	{
		bInstall = Data.bInstall;
		strDisplayVersionEx = Data.strDisplayVersionEx;
		strDisplayName = Data.strDisplayName;
		strUninstallString = Data.strUninstallString;
		strDisplayVersion = Data.strDisplayVersion;
		strInstallLocation = Data.strInstallLocation;
		strInstallDate = Data.strInstallDate;
		strLastPolicyDate = Data.strLastPolicyDate;
		strLastPolicyID = Data.strLastPolicyID;
		strPolictSet = Data.strPolictSet;
		strPackageMaster = Data.strPackageMaster;
		dwVersionMajor = Data.dwVersionMajor;
		dwVersionMinor = Data.dwVersionMinor;
		dwVersion = Data.dwVersion;
	}


}PRIMX_PRODUCT_DATA;


// 정책 정보
typedef struct _PRIMX_POLICY_DATA_
{
	BOOL bAuthWebManager;
	CString strAuthWebManagerURL;
	BOOL bAuthExportFile;
	UINT nShelfLife;
	CString strServerDateTime;
	BOOL bProductMenu;
	BOOL bWebManagerConsoleMenu;
	BOOL bExportFileMenu;
	BOOL bAuthNotEncryption;
	CString strAuthNotEncryptionStamp;
	CStringList AuthNotEncryptionPathList;
	
	void Clear() {
		strAuthWebManagerURL = _T("");
		nShelfLife = 0;
		strServerDateTime = _T("");
		bAuthWebManager = FALSE;
		bAuthExportFile = FALSE;
		bProductMenu = FALSE;
		bWebManagerConsoleMenu = FALSE;
		bExportFileMenu = FALSE;
		bAuthNotEncryption = FALSE;
		strAuthNotEncryptionStamp = _T("");
		AuthNotEncryptionPathList.RemoveAll();
	};

	_PRIMX_POLICY_DATA_()
	{
		Clear();
	};

	void operator=(const _PRIMX_POLICY_DATA_& Data)
	{
		Clear();

		bAuthWebManager = Data.bAuthWebManager;
		strAuthWebManagerURL = Data.strAuthWebManagerURL;
		bAuthExportFile = Data.bAuthExportFile;
		nShelfLife = Data.nShelfLife;
		strServerDateTime = Data.strServerDateTime;
		bProductMenu = Data.bProductMenu;
		bWebManagerConsoleMenu = Data.bWebManagerConsoleMenu;
		bExportFileMenu = Data.bExportFileMenu;
		bAuthNotEncryption = Data.bAuthNotEncryption;
		strAuthNotEncryptionStamp = Data.strAuthNotEncryptionStamp;
		POSITION Pos = Data.AuthNotEncryptionPathList.GetHeadPosition();
		while (NULL != Pos) {
			CString strPath = Data.AuthNotEncryptionPathList.GetNext(Pos);
			strPath.Trim();
			strPath.MakeLower();
			AuthNotEncryptionPathList.AddTail(strPath);
		}
	};
	

}PRIMX_POLICY_DATA;


#define PRIMX_REQUEST_TYPE_USER_REG			100
#define PRIMX_REQUEST_TYPE_ALEERT_MSG		200
#define PRIMX_REQUEST_TYPE_USER_DATA_SAVE	300


#define PRIMX_USER_ID_LEN					400
#define PRIMX_MESSAGE_LEN					512

typedef struct _PRIMXCS_REQUEST_DATA_ {

	UINT	nRequestType;
	UINT    nResult;
	union {
		TCHAR	strUserID[PRIMX_USER_ID_LEN];
		TCHAR	strFailMessage[PRIMX_MESSAGE_LEN];
	};
	
	void Clear() {
		nRequestType = 0;
		nResult = 0;
		memset(strUserID, 0x00, PRIMX_USER_ID_LEN * sizeof(TCHAR));
		memset(strUserID, 0x00, PRIMX_MESSAGE_LEN * sizeof(TCHAR));
	};

	_PRIMXCS_REQUEST_DATA_() {
		Clear();
	};

}PRIMXCS_REQUEST_DATA;
#define PRIMXCS_REQUEST_DATA_LEN	sizeof(PRIMXCS_REQUEST_DATA)


typedef struct _PRIMXCS_REQUEST_ALERT_DATA_ {

	UINT	nRequestType;
	UINT    nResult;
	
	void Clear() {
		nRequestType = 0;
		nResult = 0;
	};

	_PRIMXCS_REQUEST_ALERT_DATA_() {
		Clear();
	};

}PRIMXCS_REQUEST_ALERT_DATA;
#define PRIMXCS_REQUEST_ALERT_DATA_LEN	sizeof(PRIMXCS_REQUEST_ALERT_DATA)

// 로그 정보
typedef struct _PRIMX_LOG_DATA_
{
	UINT nProductType;			// 0: 알 수 없음, 1: PRIMX Zonecentral, 2: PRIMX Orizon, 3: PRIMX Cryhod 4: PRIMX Agent
	CString strLogDate;
	CString strLogTime;
	CString strLogType;
	CString strLogCode;
	CString strLogLevel;
	CString strLogData;
	CString strLogFileName;
	CString strLogFilePath;

	void Clear() {
		strLogTime = _T("");
		strLogType = _T("");
		strLogCode = _T("");
		strLogLevel = _T("");
		strLogData = _T("");
		strLogFilePath = _T("");
	};

	_PRIMX_LOG_DATA_()
	{

	};

	void operator=(const _PRIMX_LOG_DATA_& Data)
	{
		nProductType = Data.nProductType;
		strLogDate = Data.strLogDate;
		strLogTime = Data.strLogTime;
		strLogType = Data.strLogType;
		strLogCode = Data.strLogCode;
		strLogLevel = Data.strLogLevel;
		strLogData = Data.strLogData;
		strLogFileName = Data.strLogFileName;
		strLogFilePath = Data.strLogFilePath;
	}

}PRIMX_LOG_DATA;



typedef struct _PRIMX_ORG_FILE_INFO_ {
	CString		strFilePath;
	LONGLONG	nFileSize;

	_PRIMX_ORG_FILE_INFO_() {
		nFileSize = 0;
	};
} PRIMX_ORG_FILE_INFO;


typedef struct _PRIMX_EXPORT_FILE_INFO_ {

	CString strExportFileKey;
	CString strExportFileName;
	LONGLONG nExportFileSize;
	CString strCreateDate;
	LONGLONG nOrgFileTotalSize;

	CList<PRIMX_ORG_FILE_INFO*>		OrgFileList;

	_PRIMX_EXPORT_FILE_INFO_() {
		nExportFileSize = 0;
		nOrgFileTotalSize = 0;
	};

	~_PRIMX_EXPORT_FILE_INFO_() {
		POSITION Pos = OrgFileList.GetHeadPosition();
		while (NULL != Pos) {
			PRIMX_ORG_FILE_INFO* pData = OrgFileList.GetNext(Pos);
			_SafeDelete(pData);
		}
		OrgFileList.RemoveAll();
	};

} PRIMX_EXPORT_FILE_INFO;

