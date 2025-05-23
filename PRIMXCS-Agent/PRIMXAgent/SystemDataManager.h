#pragma once


typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL(WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

class CSystemDataManager
{
public:
	CSystemDataManager();
	~CSystemDataManager();

	BOOL GetSystemDataCollection(PRIMX_SYSTEM_DATA& SystemData);
	BOOL GetPRIMXProductInfo(PRIMX_PRODUCT_DATA& Zonecentrol, PRIMX_PRODUCT_DATA& Orizone, PRIMX_PRODUCT_DATA& Cryhod);
	
protected:
	BOOL GetWindowsVersion(PRIMX_SYSTEM_DATA& SystemData);
	BOOL GetNetBiosNameEx(PRIMX_SYSTEM_DATA& SystemData);
	BOOL GetDiskDriveData(PRIMX_SYSTEM_DATA& SystemData);
	BOOL GetNICData(PRIMX_SYSTEM_DATA& SystemData);

	CString GetWindowsSystemUUID();
	CString GetDNS(CString strAdapterGUID, BOOL bDHCPEnabled);
	CString GetComputername();
	CString SetCreateProcesseForResult(CString strProcess, UINT nTimeOutSec = 2);	
	
	CString GetRegQueryValueString(HKEY hKey, CString strValueName);
	DWORD GetRegQueryValueDword(HKEY hKey, CString strValueName);



};

