#pragma once

#include <Softpub.h>
#include <wincrypt.h>
#include <wintrust.h>
#include "CommonDefine.h"

// Link with the Wintrust.lib file.
#pragma comment (lib, "wintrust")
#pragma comment(lib, "Crypt32.lib") 


typedef UINT(CALLBACK* LPFNDLLFUNC1) (REFKNOWNFOLDERID, DWORD, HANDLE , __out PWSTR*);
typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
typedef BOOL(WINAPI *LPWOW64FSREDIRECTION)(PVOID *);

class CCommonModule
{
public:
	CCommonModule();
	~CCommonModule();
	
	BOOL GetBool(CString strBoolString,BOOL bDefault = TRUE);

	UINT AtoUI(LPCTSTR Data);
	LONGLONG AtoLONG2(LPCTSTR Data);
	int AtoI(LPCTSTR Data);
	DWORD AtoDword(LPCTSTR Data);
	BOOL IsNumberTok(CString& strData, BOOL bOrgDataMod = TRUE);

	BOOL ReadRegistryValue(CString strKey, CString strSubkey, CString strDataName, BOOL bRegType32, CString& strValue);
	BOOL GetRegKeyExists(CString strKey, CString strSubkey, BOOL bRegType32);

	CString GetLoadStringTable(UINT nID);
	CString SizeToA(LONGLONG nData);
	CString IntToCurrencyString(LONGLONG nData);
	CString GetLocalFileName();
	CString GetLocalPath();
	CString GetLocalFullPath();
	CString GetProgramDataPath();

	CString SetSplitPathFileNameEx(CString strPath);
	CString SetSplitPathFileName(CString strPath);
	CString SetSplitPath(CString strPath);
	UINT GetFileSizeWithCreateFileTime(CString strFilePath, COleDateTime& tCreateFileTime);
	LONGLONG GetFileSizeEx(CString strFilePath);
	LONGLONG GetFileSizeEx(HANDLE hFile);
	CString GetFileExtension(CString strFilePath);

	BOOL IsFile(CString strFilePath);

	UINT GetFileSizeForPE(CString strFilePath =_T(""));
	
	CString GetCreateKey();
	unsigned __int64 GetGMTime();

	BOOL IsNum(CString strString);
		
	BOOL SetFileCopy(CString strSrcPath, CString strDstPath, CString strFileName);
	BOOL SetFileCopy(CString strSrcFileName, CString strDstFileName);
	BOOL SetDeleteFile(CString strFilePath);
	BOOL SetDeleteFileEx(CString strFilePath);
	BOOL SetCreateDirectoryEx(CString strDirectory);

	CString GetTimeString();
	CString GetTimeStringForFileName();
	CString GetDateTimeForGMT();

	CString GetLastErrorString();
	BOOL IsSystemDirectory(CString strCheckDirectory);
	void SetFileRemoveRecurseSubdirectories(CString strSrcDir);

	CString SetBase64EncodeForFile(CString strFilePath);

	BOOL FileExist(CString strPath);
	void SetDeleteFileWithSubDir(CString strSrcDir, CString strRemoveFormat=_T(""));


	BOOL CreateRegistryKey(CString KeyPath);
	BOOL WriteRegistryStringValue(CString KeyPath, CString Name, CString Value);
	BOOL WriteRegistryBinaryValue(CString KeyPath, CString Name, char* pValue, UINT PValueLen);
	BOOL WriteRegistryDwordValue(CString KeyPath, CString Name, DWORD dwValue);
	BOOL ReadRegistryDwordValue(CString KeyPath, CString Name, DWORD& Value);
	BOOL ReadRegistryStringValue(CString KeyPath, CString Name, CString& Value);

	BOOL DeleteRegistryValue(CString KeyPath, CString Name);

	UINT GetRandNum(UINT MinValue, UINT MaxValue);
	CString IPConvertString(unsigned long IP);
	unsigned long IPConvertString(CString strAddr);

	UINT GetProfileIntEx(LPCTSTR lpSession, LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFilePath);
	BOOL WriteProfileIntEx(LPCTSTR lpSession, LPCTSTR lpszEntry, int nValue, LPCTSTR lpFilePath);
	BOOL WriteProfileUINTEx(LPCTSTR lpSession, LPCTSTR lpszEntry, UINT nValue, LPCTSTR lpFilePath);
	BOOL WriteProfileStringEx(LPCTSTR lpSession, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFilePath);
	CString GetProfileStringEx(LPCTSTR lpSession, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPCTSTR lpFilePath);
		
	CString GetFileDir(CString strFilePath);
	CString GetCurrentDirectoryEx();
	void SetCurrentDirectoryEx(CString strDir);
	
	BOOL IsIPAddr(CString strAddr);

	CString SetCreateProcesseForResult(CString strProcess, UINT nTimeOutSec = 30);
	BOOL GetWindowsSystemUUID(CString& strUUID);
	BOOL SetCreateProcesseEx(CString strProcess, DWORD& dwExitCode, DWORD dwTimeout = 600);

	void SetProcessCommandLineCheckForKill(CString strProcessName, CString strCommandLineString);
	BOOL GetProcessData3(CString strData, CString strCommandLineString);
	BOOL SetMakeFile(WORD iResource, CString strPath, CString strModule);

	BOOL SetUserData(CString strUserXMLData);

protected:
	DWORD WTSGetActiveConsoleSessionIdEx();
	BOOL StartImpersonation();


};

