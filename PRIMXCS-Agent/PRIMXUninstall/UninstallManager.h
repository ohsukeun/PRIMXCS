#pragma once
class CUninstallManager
{
public:
	CUninstallManager();
	~CUninstallManager();

	void SetUninstall();

protected:

	BOOL SetFileRename(CString strSrcFileName, CString strModFileName);
	CString GetServiceBinaryPath(CString ServiceName);
	CString GetInstallPRIMXAgentPath();
	void SetDeleteFile(CString strFilePath);
	BOOL SetCreateProcesseEx(CString strProcess, DWORD dwTimeout);
	BOOL IsSystemDirectory(CString strCheckDirectory);
	void SetFileRemoveRecurseSubdirectories(CString strSrcDir, CString strExclusionFileName);
	CString SetSplitPath(CString strPath);
	void SetAgentID(CString strAgentID);
	CString GetAgentID();
	BOOL ReadRegistryStringValue(CString KeyPath, CString Name, CString& Value);
	BOOL WriteRegistryStringValue(CString KeyPath, CString Name, CString Value);
	BOOL GetPRIMXProductInfo(CString& strZonecentrol, CString& strOrizone, CString& strCryhod);
	CString GetRegQueryValueString(HKEY hKey, CString strValueName);
	CString GetProgramDataPath();
};

