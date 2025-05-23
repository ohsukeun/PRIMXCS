#include "stdafx.h"
#include "CommonModule.h"
#include <io.h>

#include <wincrypt.h>
#include <wintrust.h>
#include <ws2tcpip.h>
#include <bcrypt.h>
#include <tlhelp32.h>
#include <WtsApi32.h>
#include <UserEnv.h>
#include <tlhelp32.h>
#include <Psapi.h>

#include "Base64.h"
#include "SHA512.H"

#include "IPCChannel.h"
#include "ARIAEncryption.h"

#pragma comment(lib, "Bcrypt.lib")
#pragma comment (lib, "wintrust")
#pragma comment(lib,"WtsApi32.lib")
#pragma comment(lib,"UserEnv.lib")
#pragma comment(lib, "Psapi.lib")

#pragma warning(disable : 4996)


#define PRIMX_MAX_PIPE_BUFF_SIZE			(1024 * 100)

CCommonModule::CCommonModule()
{


}


CCommonModule::~CCommonModule()
{

}

BOOL CCommonModule::FileExist(CString strPath)
{
	BOOL bRe = FALSE;
	CFileFind	Find;
	bRe = Find.FindFile(LPCTSTR(strPath));
	Find.Close();
	return bRe;
}

BOOL CCommonModule::SetFileCopy(CString strSrcFileName, CString strDstFileName)
{
	BOOL Re = TRUE;

	CFileFind FileFind;
	if (TRUE == FileFind.FindFile(strSrcFileName)) {
		if (FALSE == ::CopyFile(strSrcFileName, strDstFileName, FALSE)) {

			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"파일 복사 실패 > %s > %s > %s", GetLastErrorString(), strSrcFileName, strDstFileName);

			CString strRenameRemove;
			strRenameRemove.Format(_T("%s.%lu"), LPCTSTR(strDstFileName), UINT(_time32(NULL)));
			::DeleteFile(strRenameRemove);
			_trename(strDstFileName, strRenameRemove);
			MoveFileEx(strRenameRemove, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
			if (FALSE == ::CopyFile(strSrcFileName, strDstFileName, FALSE)) {
				Re = FALSE;
			}
		}
	}
	else {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"파일 복사 실패 > 파일 존재 안함 > %s > %s", strSrcFileName, strDstFileName);
	}
	FileFind.Close();
	return Re;
}

BOOL CCommonModule::SetFileCopy(CString strSrcPath, CString strDstPath, CString strFileName)
{
	BOOL Re = TRUE;
	CString strDstFile, strSrcFile;

	strDstFile.Format(_T("%s\\%s"), LPCTSTR(strDstPath), LPCTSTR(strFileName));
	strSrcFile.Format(_T("%s\\%s"), LPCTSTR(strSrcPath), LPCTSTR(strFileName));

	CFileFind FileFind;
	if (TRUE == FileFind.FindFile(strSrcFile)) {
		if (FALSE == ::CopyFile(strSrcFile, strDstFile, FALSE)) {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"파일 복사 실패 > %s > %s > %s > %s", GetLastErrorString(), strSrcPath, strDstPath, strFileName);
			CString strRenameRemove;
			strRenameRemove.Format(_T("%s.%lu"), LPCTSTR(strDstFile), UINT(_time32(NULL)));
			::DeleteFile(strRenameRemove);
			_trename(strDstFile, strRenameRemove);
			MoveFileEx(strRenameRemove, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
			if (FALSE == ::CopyFile(strSrcFile, strDstFile, FALSE)) {
				Re = FALSE;
			}
		}
	}
	else {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"파일 복사 실패 > 파일 존재 안함 > %s > %s > %s", strSrcPath, strDstPath, strFileName);
	}
	FileFind.Close();
	return Re;
}

BOOL CCommonModule::GetBool(CString strBoolString, BOOL bDefault)
{
	BOOL bRe = bDefault;
	strBoolString.Trim();

	if (0 == strBoolString.CompareNoCase(_T("Yes"))) {
		bRe = TRUE;
	}
	if (0 == strBoolString.CompareNoCase(_T("TRUE"))) {
		bRe = TRUE;
	}
	if (0 == strBoolString.CompareNoCase(_T("1"))) {
		bRe = TRUE;
	}
	if (0 == strBoolString.CompareNoCase(_T("No"))) {
		bRe = FALSE;
	}
	if (0 == strBoolString.CompareNoCase(_T("FALSE"))) {
		bRe = FALSE;
	}
	if (0 == strBoolString.CompareNoCase(_T("0"))) {
		bRe = FALSE;
	}
	return bRe;
}
int CCommonModule::AtoI(LPCTSTR Data)
{
	if (NULL == Data)
	{
		return 0;
	}
	return (int)_ttof(LPTSTR(Data));
}
UINT CCommonModule::AtoUI(LPCTSTR Data)
{
	if (NULL == Data)
	{
		return 0;
	}
	return (UINT)_ttof(LPTSTR(Data));
}

LONGLONG CCommonModule::AtoLONG2(LPCTSTR Data)
{
	if (NULL == Data)
	{
		return 0;
	}
	return (LONGLONG)_ttof(LPTSTR(Data));
}

DWORD CCommonModule::AtoDword(LPCTSTR Data)
{
	if (NULL == Data) return 0;
	return DWORD(_tstof(Data));
}

BOOL CCommonModule::IsNumberTok(CString& strData, BOOL bOrgDataMod)
{
	BOOL Re = TRUE;
	for (int i = 0; i<strData.GetLength(); i++) {
		if (0 == _istdigit(strData[i])) {
			if (TRUE == bOrgDataMod) {
				strData = strData.Left(i);
			}
			Re = FALSE;
			break;
		}
	}
	return Re;
}

CString CCommonModule::GetLoadStringTable(UINT nID)
{
	CString strData;
	strData.LoadString(nID);
	return strData;
}

CString CCommonModule::SizeToA(LONGLONG  nData)
{
	CString strData;

	TCHAR szFileLen[1024] = { 0 };
	StrFormatByteSize(nData, szFileLen, 1024);
	strData = szFileLen;

	return strData;
}

CString CCommonModule::IntToCurrencyString(LONGLONG nData)
{

	CString strData;
	strData.Format(_T("%I64u"), nData);
	for (int i = strData.GetLength() - 3; i > 0; i -= 3){
		strData.Insert(i, _T(','));
	}
	return strData;
}

CString CCommonModule::GetLocalFileName()
{
	CString strFileName;
	TCHAR szWindowsPath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szWindowsPath, sizeof(szWindowsPath));
	CString strPath = szWindowsPath;
	strFileName = SetSplitPathFileName(strPath);
	strFileName.Trim();
	return strFileName;
}

CString CCommonModule::GetProgramDataPath()
{
	CString strLogPath;

	TCHAR szPath[MAX_PATH] = { 0 };
	if (TRUE == SHGetSpecialFolderPath(NULL, szPath, CSIDL_COMMON_APPDATA, 0)) {
		strLogPath.Format(PRIMX_AGNET_INSTALL_PATH_FORMAT, szPath);
	}
	else {
		if (0 < ::ExpandEnvironmentStrings(_T("%ProgramData%"), szPath, MAX_PATH)) {
			strLogPath.Format(PRIMX_AGNET_INSTALL_PATH_FORMAT, szPath);
		}
		else {
			if (0 != GetWindowsDirectory(szPath, MAX_PATH)) {
				TCHAR pDrive[MAX_PATH] = { 0 };
				TCHAR pDir[MAX_PATH] = { 0 };
				TCHAR pFileName[MAX_PATH] = { 0 };
				TCHAR pFileExt[MAX_PATH] = { 0 };
				_tsplitpath_s(szPath, pDrive, MAX_PATH, pDir, MAX_PATH, pFileName, MAX_PATH, pFileExt, MAX_PATH);
				strLogPath.Format(PRIMX_AGNET_INSTALL_PROGRAMDATA_PATH_FORMAT, pDrive);
			}
			else {
				strLogPath.Format(PRIMX_AGNET_INSTALL_PROGRAMDATA_PATH_FORMAT, _T("c:"));
			}
		}
	}

	return strLogPath;
}

CString CCommonModule::GetLocalPath()
{
	CString Path;
	CString FileName;
	int pos = 0;
	int pos_b = 0;

	TCHAR szWindowsPath[MAX_PATH + 1] = { 0 };

	GetModuleFileName(NULL, szWindowsPath, sizeof(szWindowsPath));
	Path = FileName = szWindowsPath;

	while (1) {
		pos = FileName.Find(_T("\\"), pos);

		if (-1 == pos) {
			FileName = FileName.Right(FileName.GetLength() - (pos_b));
			break;
		}
		else {
			pos = pos + 1;
			pos_b = pos;
		}
	}

	Path = Path.Left(Path.GetLength() - (FileName.GetLength() + 1));
	return Path;
}

CString CCommonModule::GetLocalFullPath()
{
	CString Path;
	TCHAR szWindowsPath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szWindowsPath, sizeof(szWindowsPath));
	Path = szWindowsPath;
	return Path;
}
CString CCommonModule::SetSplitPathFileNameEx(CString strPath)
{
	CString strFileName;

	TCHAR pFilename[MAX_PATH] = { 0 };
	TCHAR pExt[MAX_PATH] = { 0 };

	TCHAR pDrive[MAX_PATH] = { 0 };
	TCHAR pDir[MAX_PATH] = { 0 };

	_tsplitpath_s(strPath, pDrive, pDir, pFilename, pExt);

	strFileName.Format(_T("%s"), pFilename);

	return strFileName;
}

CString CCommonModule::SetSplitPathFileName(CString strPath)
{
	CString strFileName;
	
	TCHAR pFilename[MAX_PATH] = { 0 };
	TCHAR pExt[MAX_PATH] = { 0 };

	TCHAR pDrive[MAX_PATH] = { 0 };
	TCHAR pDir[MAX_PATH] = { 0 };

	_tsplitpath_s(strPath, pDrive, pDir, pFilename, pExt);

	strFileName.Format(_T("%s%s"), pFilename, pExt);

	return strFileName;
}

CString CCommonModule::SetSplitPath(CString strPath)
{
	CString strSplitPath;

	TCHAR pFilename[MAX_PATH] = { 0 };
	TCHAR pExt[MAX_PATH] = { 0 };

	TCHAR pDrive[MAX_PATH] = { 0 };
	TCHAR pDir[MAX_PATH] = { 0 };

	_tsplitpath_s(strPath, pDrive, pDir, pFilename, pExt);

	strSplitPath.Format(_T("%s%s"), pDrive, pDir);

	return strSplitPath;
}
CString CCommonModule::GetFileExtension(CString strFilePath)
{
	CString strExtension;

	TCHAR pFilename[MAX_PATH] = { 0 };
	TCHAR pExt[MAX_PATH] = { 0 };

	TCHAR pDrive[MAX_PATH] = { 0 };
	TCHAR pDir[MAX_PATH] = { 0 };

	_tsplitpath_s(strFilePath, pDrive, pDir, pFilename, pExt);

	strExtension.Format(_T("%s"), pExt);
	strExtension.Replace(_T("."),_T(""));
	strExtension.Trim();

	return strExtension;
}

UINT CCommonModule::GetFileSizeWithCreateFileTime(CString strFilePath, COleDateTime& tCreateFileTime)
{
	HANDLE hFile = CreateFile((LPCTSTR)strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return 0;
	}

	FILETIME create_time, access_time, write_time;
	GetFileTime(hFile, &create_time, &access_time, &write_time);

	SYSTEMTIME create_system_time, create_local_time;
	FileTimeToSystemTime(&create_time, &create_system_time);
	SystemTimeToTzSpecificLocalTime(NULL, &create_system_time, &create_local_time);

	COleDateTime tTemp(create_system_time);
	tCreateFileTime = tTemp;

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (INVALID_FILE_SIZE == dwFileSize)
	{
		CloseHandle(hFile);
		return 0;
	}

	CloseHandle(hFile);

	return (UINT)dwFileSize;
}

LONGLONG CCommonModule::GetFileSizeEx(CString strFilePath)
{
	HANDLE hFile = CreateFile((LPCTSTR)strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return -1;
	}
	LARGE_INTEGER fileSize;
	memset(&fileSize, 0x0, sizeof(LARGE_INTEGER));
	if (FALSE == ::GetFileSizeEx(hFile, &fileSize)) {
		CloseHandle(hFile);
		return -1;
	}
	CloseHandle(hFile);

	return fileSize.QuadPart;
}

LONGLONG CCommonModule::GetFileSizeEx(HANDLE hFile)
{
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return -1;
	}
	LARGE_INTEGER fileSize;
	memset(&fileSize, 0x0, sizeof(LARGE_INTEGER));
	if (FALSE == ::GetFileSizeEx(hFile, &fileSize)) {
		return -1;
	}

	return fileSize.QuadPart;
}

BOOL CCommonModule::IsFile(CString strFilePath)
{
	BOOL bRe = FALSE;
	_tfinddata64_t  c_file;
	intptr_t  hFile = 0;
	
	hFile = _tfindfirsti64(strFilePath, &c_file);

	if (-1 != hFile) {
		if (!(c_file.attrib & _A_SUBDIR)) {
			bRe = TRUE;
		}
	}
	_findclose(hFile);

	return bRe;
}

UINT CCommonModule::GetFileSizeForPE(CString strFilePath)
{
	UINT nFileSize = 0;
	IMAGE_DOS_HEADER *pDos = NULL;
	IMAGE_NT_HEADERS *pNt = NULL;
	IMAGE_FILE_HEADER *pFile = NULL;
	IMAGE_OPTIONAL_HEADER *pOption = NULL;
	IMAGE_DATA_DIRECTORY *pDataDir = NULL;
	IMAGE_SECTION_HEADER *pSection = NULL;
	DWORD NumberofSections = 0;
	DWORD NumberofData = 0;
	DWORD PointertoRawdata = 0;
	DWORD SizeofRawdata = 0;	

	strFilePath.Trim();

	if (0 == strFilePath.GetLength()) {
		strFilePath = GetLocalFullPath();
	}

	HANDLE hFile = CreateFile((LPCTSTR)strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return nFileSize;
	}

	HANDLE hMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
	if (NULL == hMap) return nFileSize;

	void *pBase = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	if(NULL == pBase) return nFileSize;

	/* (BYTE *)는 그 주소와 상수를 더하기 위해 필요함 */
	pDos = (IMAGE_DOS_HEADER *)pBase;
	pNt = (IMAGE_NT_HEADERS *)(pDos->e_lfanew + (BYTE *)pDos);
	pFile = (IMAGE_FILE_HEADER *)((BYTE *)pNt + 4);
	pOption = (IMAGE_OPTIONAL_HEADER *)((BYTE *)pNt + 0x18);

	if (pDos->e_magic != 0x5a4d || pNt->Signature != 0x4550)
	{
		CloseHandle(hFile);
		return nFileSize;
	}
	
	NumberofSections = pFile->NumberOfSections;
	NumberofData = pOption->NumberOfRvaAndSizes;

	pDataDir = (IMAGE_DATA_DIRECTORY *)((BYTE *)pOption + 0x60);
	pSection = (IMAGE_SECTION_HEADER *)((BYTE *)pDataDir + (NumberofData * 8));

	for (int i = 0; i < int(NumberofSections - 1); i++)
	{
		pSection++; // 마지막 섹션으로 이동
	}

	PointertoRawdata = ntohl(pSection->PointerToRawData);
	SizeofRawdata = ntohl(pSection->SizeOfRawData);

	UnmapViewOfFile(pBase);
	CloseHandle(hMap);

	nFileSize = PointertoRawdata + SizeofRawdata;
	//nFileSize = GetFileSize(hFile, NULL);

	CloseHandle(hFile);

	return nFileSize;
}

unsigned __int64 CCommonModule::GetGMTime()
{
	__time64_t t;
	t = _time64(NULL);
	return unsigned __int64(t);
}

CString CCommonModule::GetCreateKey()
{
	GUID GuidData;
	CString strKeyString;

	GuidData = GUID_NULL;
	::CoCreateGuid(&GuidData);

	if (GuidData == GUID_NULL) {
		Sleep(900);
		BYTE  RandomData[12] = { 0 };
		NTSTATUS hr = BCryptGenRandom(
			NULL,
			(BYTE*)RandomData,
			12,
			BCRYPT_USE_SYSTEM_PREFERRED_RNG
		);

		if (FAILED(hr)) {
			for (int i = 0; i < 12; i++) {
				RandomData[i] = _Random(1, 254);
			}
		}
		strKeyString.Format(_T("%08lX-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X"), GetGMTime(), RandomData[0], RandomData[1], RandomData[2], RandomData[3], RandomData[4], RandomData[5],
			RandomData[6], RandomData[7], RandomData[8], RandomData[9], RandomData[10], RandomData[11]);
		return strKeyString;
	}

	strKeyString.Format(_T("%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"),
		GuidData.Data1, GuidData.Data2, GuidData.Data3,
		GuidData.Data4[0], GuidData.Data4[1], GuidData.Data4[2], GuidData.Data4[3],
		GuidData.Data4[4], GuidData.Data4[5], GuidData.Data4[6], GuidData.Data4[7]);

	return strKeyString;
}


BOOL CCommonModule::IsNum(CString strString)
{
	BOOL Re = TRUE;

	strString.Trim();
	strString.Replace(_T("-"), _T(""));
	strString.Replace(_T("+"), _T(""));

	for (int i = 0; i<strString.GetLength(); i++) {
		if (0 == _istdigit(strString[i])) {
			Re = FALSE;
			break;
		}
	}
	return Re;

}

CString CCommonModule::GetLastErrorString()
{
	CString strError;
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	strError = (LPCTSTR)lpMsgBuf;
	LocalFree(lpMsgBuf);

	strError.Replace(_T("\r"), _T(""));
	strError.Replace(_T("\n"), _T(""));

	return strError;

}
BOOL CCommonModule::SetDeleteFile(CString strFilePath)
{
	BOOL bRe = TRUE;

	CFileFind	Find;
	if (TRUE == Find.FindFile(LPCTSTR(strFilePath))) {

		DWORD dwAttrs = ::GetFileAttributes(strFilePath);
		if (dwAttrs & FILE_ATTRIBUTE_READONLY) {
			::SetFileAttributes(strFilePath, dwAttrs & ~FILE_ATTRIBUTE_READONLY);
		}

		if (FALSE == ::DeleteFile(strFilePath)) {
			CString strRenameRemove;
			strRenameRemove.Format(_T("%s.%lu"), LPCTSTR(strFilePath), UINT(_time32(NULL)));
			::DeleteFile(strRenameRemove);
			if (0 != _trename(strFilePath, strRenameRemove)) {
				bRe = FALSE;
			}
			else {
				MoveFileEx(strRenameRemove, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
			}
		}
	}
	Find.Close();

	return bRe;
}

BOOL CCommonModule::SetDeleteFileEx(CString strFilePath)
{
	BOOL bRe = TRUE;

	if (TRUE == PathFileExists(strFilePath)) {
		DWORD dwAttrs = ::GetFileAttributes(strFilePath);
		if (dwAttrs & FILE_ATTRIBUTE_READONLY) {
			::SetFileAttributes(strFilePath, dwAttrs & ~FILE_ATTRIBUTE_READONLY);
		}
		bRe = FALSE;
		for (int i = 0; i < 3; i++) {
			if (TRUE == ::DeleteFile(strFilePath)) {
				bRe = TRUE;
				break;
			}
			Sleep(500);
		}
	}

	return bRe;
}

CString CCommonModule::GetTimeString()
{
	CString strReData;

	_tzset();
	struct __timeb32 tstruct;
	_ftime32_s(&tstruct);

	struct tm ConvertTime;

	if (0 == _localtime32_s(&ConvertTime, &tstruct.time)) {
		strReData.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
			ConvertTime.tm_year + 1900,
			ConvertTime.tm_mon + 1,
			ConvertTime.tm_mday, ConvertTime.tm_hour,
			ConvertTime.tm_min,
			ConvertTime.tm_sec);
	}

	return strReData;
}

CString CCommonModule::GetTimeStringForFileName()
{
	CString strReData;

	_tzset();
	struct __timeb32 tstruct;
	_ftime32_s(&tstruct);

	struct tm ConvertTime;

	if (0 == _localtime32_s(&ConvertTime, &tstruct.time)) {
		strReData.Format(_T("%02d_%02d_%02d_%03d"),
			ConvertTime.tm_hour,
			ConvertTime.tm_min,
			ConvertTime.tm_sec,
			tstruct.millitm);
	}
	return strReData;
}

BOOL CCommonModule::IsSystemDirectory(CString strCheckDirectory)
{
	strCheckDirectory.Trim();
	strCheckDirectory.Replace(_T("\\"), _T(""));
	strCheckDirectory.MakeUpper();


	TCHAR tchBuffer[MAX_PATH] = { 0 };
	ExpandEnvironmentStrings(_T("%SystemRoot%"), tchBuffer, MAX_PATH);
	CString strSystemDir = tchBuffer;
	strSystemDir.Trim();
	strSystemDir.Replace(_T("\\"), _T(""));
	strSystemDir.MakeUpper();

	if (0 == strSystemDir.CompareNoCase(strCheckDirectory)) {
		return TRUE;
	}
	strSystemDir = strSystemDir + _T("System32");
	if (0 == strSystemDir.CompareNoCase(strCheckDirectory)) {
		return TRUE;
	}

	ZeroMemory(tchBuffer, sizeof(tchBuffer));
	ExpandEnvironmentStrings(_T("%ProgramFiles%"), tchBuffer, MAX_PATH);
	strSystemDir = tchBuffer;
	strSystemDir.Trim();
	strSystemDir.Replace(_T("\\"), _T(""));
	strSystemDir.MakeUpper();

	if (0 == strSystemDir.CompareNoCase(strCheckDirectory)) {
		return TRUE;
	}

	return FALSE;
}
void CCommonModule::SetFileRemoveRecurseSubdirectories(CString strSrcDir)
{
	if (3 >= strSrcDir.GetLength()) {
		return;
	}

	if (TRUE == IsSystemDirectory(strSrcDir)) {
		return;
	}

	CFileFind finderdel;
	CString TempDel;
	TempDel = strSrcDir + _T("\\*.*");
	BOOL bWorking = finderdel.FindFile(TempDel);
	while (bWorking) {
		bWorking = finderdel.FindNextFile();
		if (finderdel.IsDots()) {
			continue;
		}

		if (TRUE == finderdel.IsArchived()) {
			CString DstFile;
			DstFile.Format(_T("%s\\%s"), strSrcDir, finderdel.GetFileName());
			if (FALSE == SetDeleteFile(DstFile)) {
				TRACE(_T("%s[%s]"), GetLastErrorString(), DstFile);
			}
		}
		if (TRUE == finderdel.IsDirectory()) {
			SetFileRemoveRecurseSubdirectories(finderdel.GetFilePath());
			::RemoveDirectory(finderdel.GetFilePath());
		}
		Sleep(100);
	}
	finderdel.Close();
	::RemoveDirectory(strSrcDir);
}

CString CCommonModule::SetBase64EncodeForFile(CString strFilePath)
{
	errno_t err;
	FILE *fp = NULL;
	CString strEncodeData;

	char* pFileData = NULL;
	char* pBase64Data = NULL;


	try
	{
		err = _tfopen_s(&fp, LPCTSTR(strFilePath), _T("rb"));
		if (0 == err) {
			fseek(fp, 0, SEEK_END);
			UINT nFileByteLen = UINT(ftell(fp));
			fseek(fp, 0, SEEK_SET);

			if (0 < nFileByteLen) {
				pFileData = new char[nFileByteLen];
				ZeroMemory((void*)pFileData, nFileByteLen);
				fread(pFileData, nFileByteLen, 1, fp);

				CBase64 Base64;
				int nBase64EncodeLen = Base64.EncodeGetRequiredLength(nFileByteLen);
				pBase64Data = new char[nBase64EncodeLen + 2];
				ZeroMemory(pBase64Data, nBase64EncodeLen + 2);

				if (TRUE == Base64.Encode((unsigned char*)pFileData, nFileByteLen, pBase64Data, &nBase64EncodeLen)) {
					_SafeDeleteArray(pFileData);
					strEncodeData = AtoWConverter(pBase64Data);
					_SafeDeleteArray(pBase64Data);
				}
				else {
					_SafeDeleteArray(pFileData);
					_SafeDeleteArray(pBase64Data);
				}

			}
			fclose(fp);
		}
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		_SafeDeleteArray(pFileData);
		_SafeDeleteArray(pBase64Data);
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
		_SafeDeleteArray(pFileData);
		_SafeDeleteArray(pBase64Data);
	}
	catch (...)
	{
		_SafeDeleteArray(pFileData);
		_SafeDeleteArray(pBase64Data);
	}
	return strEncodeData;

}

void CCommonModule::SetDeleteFileWithSubDir(CString strSrcDir, CString strRemoveFormat)
{
	if (3 >= strSrcDir.GetLength()) {
		return;
	}

	if (TRUE == IsSystemDirectory(strSrcDir)) {
		return;
	}
	if (0 == strRemoveFormat.GetLength()) {
		strRemoveFormat = _T("*.*");
	}

	CFileFind finderdel;
	CString TempDel;
	TempDel.Format(_T("%s\\%s"),strSrcDir, strRemoveFormat);
	BOOL bWorking = finderdel.FindFile(TempDel);
	while (bWorking) {
		bWorking = finderdel.FindNextFile();
		if (finderdel.IsDots()) {
			continue;
		}
		if (TRUE == finderdel.IsArchived()) {
			CString DstFile;
			DstFile.Format(_T("%s\\%s"), strSrcDir, finderdel.GetFileName());
			SetDeleteFile(DstFile);
		}
		if (TRUE == finderdel.IsDirectory()) {
			SetDeleteFileWithSubDir(finderdel.GetFilePath(), strRemoveFormat);
			//::RemoveDirectory(finderdel.GetFilePath());
		}
		Sleep(10);
	}
	finderdel.Close();

}

BOOL CCommonModule::GetRegKeyExists(CString strKey, CString strSubkey, BOOL bRegType32)
{
	BOOL bRe = FALSE;

	HKEY phkResult = NULL;
	HKEY hKey = HKEY_LOCAL_MACHINE;

	if (0 == strKey.CompareNoCase(_T("HKEY_CLASSES_ROOT"))) {
		hKey = HKEY_CLASSES_ROOT;
	}
	else if (0 == strKey.CompareNoCase(_T("HKEY_CURRENT_CONFIG"))) {
		hKey = HKEY_CURRENT_CONFIG;
	}
	else if (0 == strKey.CompareNoCase(_T("HKEY_CURRENT_USER"))) {
		hKey = HKEY_CURRENT_USER;
	}
	else if (0 == strKey.CompareNoCase(_T("HKEY_LOCAL_MACHINE"))) {
		hKey = HKEY_LOCAL_MACHINE;
	}
	else if (0 == strKey.CompareNoCase(_T("HKEY_USERS"))) {
		hKey = HKEY_USERS;
	}
	else {
		return bRe;
	}

	if (HKEY_CURRENT_USER == hKey) {
		if (StartImpersonation()) {
			if (ERROR_SUCCESS == RegOpenCurrentUser(KEY_WRITE, &hKey)) {
				if (ERROR_SUCCESS == ::RegOpenKeyEx(hKey, strSubkey, 0, (FALSE == bRegType32 ? KEY_READ | KEY_WOW64_64KEY : KEY_READ), &phkResult)) {
					::RegCloseKey(phkResult);
					bRe = TRUE;
				}
				::RegCloseKey(hKey);
			}
			RevertToSelf();
		}
	}
	else {
		if (ERROR_SUCCESS == ::RegOpenKeyEx(hKey, strSubkey, 0, (FALSE == bRegType32 ? KEY_READ | KEY_WOW64_64KEY : KEY_READ), &phkResult)) {
			::RegCloseKey(phkResult);
			bRe = TRUE;
		}
	}
	return bRe;
}

BOOL CCommonModule::ReadRegistryValue(CString strKey, CString strSubkey, CString strDataName, BOOL bRegType32,CString& strValue)
{
	BOOL Re = FALSE;

	HKEY phkResult = NULL;

	HKEY hKey = HKEY_LOCAL_MACHINE;
	if (0 == strKey.CompareNoCase(_T("HKEY_CLASSES_ROOT"))) {
		hKey = HKEY_CLASSES_ROOT;
	}
	else if (0 == strKey.CompareNoCase(_T("HKEY_CURRENT_CONFIG"))) {
		hKey = HKEY_CURRENT_CONFIG;
	}
	else if (0 == strKey.CompareNoCase(_T("HKEY_CURRENT_USER"))) {
		hKey = HKEY_CURRENT_USER;
	}
	else if (0 == strKey.CompareNoCase(_T("HKEY_LOCAL_MACHINE"))) {
		hKey = HKEY_LOCAL_MACHINE;
	}
	else if (0 == strKey.CompareNoCase(_T("HKEY_USERS"))) {
		hKey = HKEY_USERS;
	}
	else {
		return TRUE; //HKEY_LOOP_TARGET
	}
	
	if (HKEY_CURRENT_USER == hKey) {
		if (StartImpersonation()) {
			if (ERROR_SUCCESS == RegOpenCurrentUser(KEY_WRITE, &hKey)) {
				if (ERROR_SUCCESS == ::RegOpenKeyEx(hKey, strSubkey, 0, (FALSE == bRegType32 ? KEY_READ | KEY_WOW64_64KEY : KEY_READ), &phkResult)) {
					DWORD dwType = REG_SZ; // REG_DWORD
					TCHAR szValue[1024] = { 0 };
					DWORD dwSize = 1024;
					if (ERROR_SUCCESS == ::RegQueryValueEx(phkResult, LPCTSTR(strDataName), NULL, &dwType, (BYTE*)szValue, &dwSize)) {
						if (REG_SZ == dwType || REG_EXPAND_SZ == dwType) {
							strValue = szValue;
						}
						if (REG_DWORD == dwType) {
							DWORD dwValue;
							memcpy((void*)&dwValue, szValue, sizeof(DWORD));
							strValue.Format(_T("%lu"), dwValue);
						}
						Re = TRUE;
					}
					::RegCloseKey(phkResult);
				}
				::RegCloseKey(hKey);
			}
			RevertToSelf();
		}
	}
	else {
		if (ERROR_SUCCESS == ::RegOpenKeyEx(hKey, strSubkey, 0, (FALSE == bRegType32 ? KEY_READ | KEY_WOW64_64KEY : KEY_READ), &phkResult)) {
			DWORD dwType = REG_SZ; // REG_DWORD
			TCHAR szValue[1024] = { 0 };
			DWORD dwSize = 1024;
			if (ERROR_SUCCESS == ::RegQueryValueEx(phkResult, LPCTSTR(strDataName), NULL, &dwType, (BYTE*)szValue, &dwSize)) {
				if (REG_SZ == dwType || REG_EXPAND_SZ == dwType) {
					strValue = szValue;
				}
				if (REG_DWORD == dwType) {
					DWORD dwValue;
					memcpy((void*)&dwValue, szValue, sizeof(DWORD));
					strValue.Format(_T("%lu"), dwValue);
				}
				Re = TRUE;
			}
			::RegCloseKey(phkResult);
		}
	}
	return Re;
}

BOOL CCommonModule::SetCreateDirectoryEx(CString strDirectory)
{
	BOOL Re = FALSE;

	int returecode = SHCreateDirectoryEx(NULL, strDirectory, NULL);
	if (ERROR_SUCCESS != returecode) {
		if (!(ERROR_FILE_EXISTS == returecode || ERROR_ALREADY_EXISTS == returecode)) {
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CCommonModule::CreateRegistryKey(CString KeyPath)
{
	CString FilePath;
	HKEY hKey = NULL;

	DWORD dwDisp;

	if (ERROR_SUCCESS != ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, LPCTSTR(KeyPath), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &hKey, &dwDisp)) {
		return FALSE;
	}
	::RegCloseKey(hKey);

	return TRUE;
}

BOOL CCommonModule::WriteRegistryStringValue(CString KeyPath, CString Name, CString Value)
{
	HKEY hKey = NULL;

	DWORD dwReturn = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, LPCTSTR(KeyPath), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if (ERROR_SUCCESS != dwReturn) {
		return FALSE;
	}

	::RegSetValueEx(hKey,
		LPCTSTR(Name),
		0,
		REG_SZ,
		(CONST BYTE*)LPCTSTR(Value),
		Value.GetLength() * sizeof(TCHAR));

	::RegCloseKey(hKey);

	return TRUE;
}
BOOL CCommonModule::WriteRegistryBinaryValue(CString KeyPath, CString Name, char* pValue, UINT PValueLen)
{
	HKEY hKey = NULL;

	DWORD dwReturn = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, LPCTSTR(KeyPath), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if (ERROR_SUCCESS != dwReturn) {
		return FALSE;
	}

	::RegSetValueEx(hKey,
		LPCTSTR(Name),
		0,
		REG_BINARY,
		(CONST BYTE*)pValue,
		PValueLen);

	::RegCloseKey(hKey);

	return TRUE;
}

BOOL CCommonModule::WriteRegistryDwordValue(CString KeyPath, CString Name, DWORD dwValue)
{
	HKEY hKey = NULL;

	DWORD dwReturn = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, LPCTSTR(KeyPath), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

	if (ERROR_SUCCESS != dwReturn) {
		return FALSE;
	}
	BOOL bRe = FALSE;
	LSTATUS dwRe = ::RegSetValueEx(hKey, LPCTSTR(Name), 0, REG_DWORD, (CONST BYTE*)&dwValue, sizeof(dwValue));
	if (ERROR_SUCCESS == dwRe) {
		bRe = TRUE;
	}
	::RegCloseKey(hKey);

	return bRe;

}

BOOL CCommonModule::DeleteRegistryValue(CString KeyPath, CString Name)
{
	BOOL bRe = FALSE;
	HKEY hKey = NULL;

	if (ERROR_SUCCESS != ::RegOpenKey(HKEY_LOCAL_MACHINE, LPCTSTR(KeyPath), &hKey)) {
		return bRe;
	}

	LONG nError = ::RegDeleteValue(hKey, LPCTSTR(Name));
	if (ERROR_SUCCESS == nError) {
		bRe = TRUE;
	}
	::RegCloseKey(hKey);

	return bRe;
}

BOOL CCommonModule::ReadRegistryStringValue(CString KeyPath, CString Name, CString& Value)
{
	HKEY hKey = NULL;

	if (ERROR_SUCCESS != ::RegOpenKey(HKEY_LOCAL_MACHINE, LPCTSTR(KeyPath), &hKey)) {
		return FALSE;
	}

	DWORD dwType = REG_SZ;
	TCHAR szValue[1024] = { 0 };
	DWORD dwSize = 1024;
	if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, LPCTSTR(Name), NULL, &dwType, (BYTE*)szValue, &dwSize)) {
		Value = LPTSTR(szValue);
	}
	::RegCloseKey(hKey);

	return TRUE;
}
BOOL CCommonModule::ReadRegistryDwordValue(CString KeyPath, CString Name, DWORD& Value)
{
	BOOL Re = FALSE;
	HKEY hKey = NULL;

	if (ERROR_SUCCESS != ::RegOpenKey(HKEY_LOCAL_MACHINE, LPCTSTR(KeyPath), &hKey)) {
		return Re;
	}

	DWORD dwType = 0;
	TCHAR szValue[256] = { 0 };
	DWORD dwSize = 256;
	if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, LPCTSTR(Name), NULL, &dwType, (BYTE*)szValue, &dwSize)) {
		if (REG_DWORD == dwType) {
			memcpy(&Value, szValue, sizeof(DWORD));
			Re = TRUE;
		}
	}
	::RegCloseKey(hKey);

	return Re;
}

UINT CCommonModule::GetRandNum(UINT MinValue, UINT MaxValue)
{
	UINT ReData = 0;
	srand((unsigned int)GetTickCount());

	ReData = rand() % ((MaxValue + 1) - MinValue) + MinValue;

	return ReData;
}
CString CCommonModule::IPConvertString(unsigned long IP)
{
	IN_ADDR addr;
	addr.s_addr = IP;

	TCHAR szAddr[50] = { 0 };
	InetNtop(AF_INET, &addr, szAddr, 50);

	CString strIPAddr;
	strIPAddr.Format(L"%s", szAddr);
	return strIPAddr;
}

unsigned long CCommonModule::IPConvertString(CString strAddr)
{
	unsigned long nRe = 0;

	SOCKADDR_IN ServerAddrInfo;
	int nAddrLen = sizeof(ServerAddrInfo);

	memset(&ServerAddrInfo, 0, sizeof(SOCKADDR_IN));

	WSAStringToAddress((LPTSTR)LPCTSTR(strAddr), AF_INET, NULL, (LPSOCKADDR)&ServerAddrInfo, &nAddrLen);

	if (INADDR_NONE != ServerAddrInfo.sin_addr.s_addr)
	{
		nRe = ServerAddrInfo.sin_addr.S_un.S_addr;
	}

	return nRe;
}

BOOL CCommonModule::WriteProfileIntEx(LPCTSTR lpSession, LPCTSTR lpszEntry, int nValue, LPCTSTR lpFilePath)
{
	CString strValue;
	strValue.Format(_T("%d"), nValue);
	return WritePrivateProfileString(lpSession, lpszEntry, LPCTSTR(strValue), lpFilePath);
}
BOOL CCommonModule::WriteProfileUINTEx(LPCTSTR lpSession, LPCTSTR lpszEntry, UINT nValue, LPCTSTR lpFilePath)
{
	CString strValue;
	strValue.Format(_T("%lu"), nValue);
	return WritePrivateProfileString(lpSession, lpszEntry, LPCTSTR(strValue), lpFilePath);
}
BOOL CCommonModule::WriteProfileStringEx(LPCTSTR lpSession, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFilePath)
{
	return WritePrivateProfileString(lpSession, lpKeyName, LPCTSTR(lpString), lpFilePath);
}

CString CCommonModule::GetProfileStringEx(LPCTSTR lpSession, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPCTSTR lpFilePath)
{
	TCHAR TempBuffer[1024] = { 0 };
	GetPrivateProfileString(lpSession, lpKeyName, lpDefault, TempBuffer, 1024, lpFilePath);
	return TempBuffer;
}
UINT CCommonModule::GetProfileIntEx(LPCTSTR lpSession, LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFilePath)
{
	TCHAR TempBuffer[1024] = { 0 };
	CString strDefault;
	strDefault.Format(_T("%d"), nDefault);
	GetPrivateProfileString(lpSession, lpKeyName, strDefault, TempBuffer, 1024, lpFilePath);
	return UINT(atoi(WtoAConverter(TempBuffer)));
}

CString CCommonModule::GetFileDir(CString strFilePath)
{
	CString strDir;

	TCHAR pFilename[MAX_PATH] = { 0 };
	TCHAR pExt[MAX_PATH] = { 0 };
	TCHAR pDrive[MAX_PATH] = { 0 };
	TCHAR pDir[MAX_PATH] = { 0 };

	_tsplitpath_s(strFilePath, pDrive, pDir, pFilename, pExt);
	
	strDir.Format(_T("%s"), pDir);
	strDir.Trim();

	return strDir;
}

CString CCommonModule::GetCurrentDirectoryEx()
{
	CString strDir;

	TCHAR pDir[MAX_PATH + 2] = { 0 };
	DWORD dwDirLen = MAX_PATH + 2;

	if (0 < GetCurrentDirectory(dwDirLen, pDir)) {
		strDir = pDir;
	}
	return strDir;
}

void CCommonModule::SetCurrentDirectoryEx(CString strDir)
{
	if (0 < strDir.GetLength()) {
		SetCurrentDirectory(strDir);
	}

}
DWORD CCommonModule::WTSGetActiveConsoleSessionIdEx()
{
	DWORD conSessId = WTSGetActiveConsoleSessionId();

	PWTS_SESSION_INFO ppSessionInfo = NULL;
	DWORD     pCount = 0;
	WTS_SESSION_INFO  wts;

	WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &ppSessionInfo, &pCount);
	for (DWORD i = 0; i < pCount; i++)
	{
		wts = ppSessionInfo[i];
		if (wts.State == WTSActive)
		{
			conSessId = wts.SessionId;
		}
	}
	WTSFreeMemory(ppSessionInfo);

	return conSessId;


}
BOOL CCommonModule::StartImpersonation()
{
	// impersonate 참고
	//		http://www.codeproject.com/KB/vista-security/VistaSessions.aspx?msg=3198293

	DWORD dwExplorerLogonPid = 0;

	DWORD dwSessionId = WTSGetActiveConsoleSessionId();
	
	//////////////////////////////////////////
	// Find the explorer process
	////////////////////////////////////////

	PROCESSENTRY32 procEntry;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation::CreateToolhelp32Snapshot error = %d"), GetLastError());
		if (hSnap) CloseHandle(hSnap);
		return FALSE;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry))
	{
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation::Process32First error = %d"), GetLastError());
		if (hSnap) CloseHandle(hSnap);
		return FALSE;
	}

	DWORD dwExplorerSessId = 0;
	DWORD dwSessID_Temp = -1;
	do
	{
		if (_wcsicmp(procEntry.szExeFile, _T("explorer.exe")) == 0)
		{
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &dwExplorerSessId))
			{
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation::dwExplorerSessId = %d, ProcessID = %d (get)"), dwExplorerSessId, procEntry.th32ProcessID);

				if (dwExplorerSessId == dwSessionId)
				{
					dwExplorerLogonPid = procEntry.th32ProcessID;
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation::dwExplorerSessId = %d, ProcessID = %d (correct)"), dwExplorerSessId, procEntry.th32ProcessID);
					break;
				}
				else
				{
					if (dwSessID_Temp == -1 || dwExplorerSessId <= dwSessID_Temp)
					{
						dwExplorerLogonPid = procEntry.th32ProcessID;
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation::dwExplorerSessId = %d, ProcessID = %d (temp)"), dwExplorerSessId, procEntry.th32ProcessID);
					}
					dwSessID_Temp = dwExplorerSessId;
				}
			}
		}

	} while (Process32Next(hSnap, &procEntry));

	if (hSnap) CloseHandle(hSnap);

	////////////////////////////////////////////////////////////////////////
	HANDLE hProcess = NULL;
	HANDLE hPToken = NULL;

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation > OpenProcess"));


	hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, dwExplorerLogonPid);
	if (hProcess == NULL)
	{
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation::OpenProcess error = %d"), GetLastError());
		return FALSE;
	}
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation > OpenProcessToken"));

	if (!::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY| TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_SESSIONID| TOKEN_READ | TOKEN_WRITE, &hPToken))
	{
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation::OpenProcessToken error = %d"), GetLastError());
		if (hProcess) CloseHandle(hProcess);
		return FALSE;
	}
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation > hPToken == NULL"));

	if (hPToken == NULL)
	{
		if (hProcess) CloseHandle(hProcess);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation::OpenProcessToken hPToken NULL"));
		return FALSE;
	}
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation > ImpersonateLoggedOnUser"));

	BOOL bRet = ImpersonateLoggedOnUser(hPToken);

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation > ImpersonateLoggedOnUser end > %d"), bRet);

	if (bRet == FALSE) {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation::ImpersonateLoggedOnUser error = %d"), GetLastError());
	}

	if (hProcess) CloseHandle(hProcess);
	if (hPToken) CloseHandle(hPToken);

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Impersonation > %d"), bRet);
	
	return bRet;
}

BOOL CCommonModule::IsIPAddr(CString strAddr)
{
	BOOL bRe = FALSE;

	SOCKADDR_IN ServerAddrInfo;
	int nAddrLen = sizeof(ServerAddrInfo);

	memset(&ServerAddrInfo, 0, sizeof(SOCKADDR_IN));

	WSAStringToAddress((LPTSTR)LPCTSTR(strAddr), AF_INET, NULL, (LPSOCKADDR)&ServerAddrInfo, &nAddrLen);
	
	if (INADDR_NONE != ServerAddrInfo.sin_addr.s_addr)
	{
		bRe = TRUE;
	}

	return bRe;
}

CString CCommonModule::SetCreateProcesseForResult(CString strProcess, UINT nTimeOutSec)
{
	CString strResult = _T("");

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE h_pipe_write, h_pipe_read;
	SECURITY_ATTRIBUTES security_attributes;

	// 선언된 구조체 변수들을 초기화한다.
	ZeroMemory(&security_attributes, sizeof(SECURITY_ATTRIBUTES));

	// 현재 프로세스가 생성한 파이프 객체를 실행될 프로세스가 사용할수 있도록
	// 보안 설정을 한다.
	security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	security_attributes.bInheritHandle = TRUE;
	security_attributes.lpSecurityDescriptor = NULL;
	// 리다이렉션에 사용할 파이프를 생성한다.
	if (FALSE == CreatePipe(&h_pipe_read, &h_pipe_write, &security_attributes, PRIMX_MAX_PIPE_BUFF_SIZE)) {
		return strResult;
	}
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdOutput = h_pipe_write;
	si.hStdError = h_pipe_write;

	if (!CreateProcess(NULL, (LPTSTR)LPCTSTR(strProcess), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
		CloseHandle(h_pipe_read);
		CloseHandle(h_pipe_write);
		return strResult;
	}

	BOOL bTimeOut = FALSE;
	__time64_t tStartTime = _time64(NULL);
	// 프로세스 실행이 완료될때까지 대기
	while (WaitForSingleObject(pi.hProcess, 0) != WAIT_OBJECT_0)
	{
		__time64_t tCheckTime = _time64(NULL);

		if (nTimeOutSec < (tCheckTime - tStartTime)) {
			bTimeOut = TRUE;
			break;
		}
		Sleep(500);
	}

	if (FALSE == bTimeOut) {
		// 파이프를 통해 전달받은 정보를 저장하기 위한 변수를 선언한다.
		char pipe_data[PRIMX_MAX_PIPE_BUFF_SIZE] = { 0 };
		DWORD total_bytes_available = 0;
		DWORD num_bytes_read = 0;
		DWORD bytes_left_this_message = 0;

		// 명시된 파이프에 출력된 정보를 얻어온다.
		//파이프에서 데이터를 삭제하지 않고 명명한 파이프에서 데이터를 복제할 때 사용한다. 이함수는 리버스 셸에 자주 사용한다.
		BOOL bRe = PeekNamedPipe(h_pipe_read, pipe_data, 1, &num_bytes_read, &total_bytes_available, &bytes_left_this_message);

		if (TRUE == bRe) {
			// 읽을 정보가 실제로 있는 경우
			if (num_bytes_read > 0) {
				// pipe_data 에 읽은 정보를 저장한다.
				bRe = ReadFile(h_pipe_read, pipe_data, PRIMX_MAX_PIPE_BUFF_SIZE - 1, &num_bytes_read, NULL);

				if (TRUE == bRe) {
					// 읽어들인 정보의 맨 뒤에 '\0' 를 추가하여 문자열의 끝임을 명시한다.
					pipe_data[num_bytes_read] = '\0';

					//동적할당
					TCHAR* sPipeData = new TCHAR[PRIMX_MAX_PIPE_BUFF_SIZE];
					::MultiByteToWideChar(CP_ACP, 0, pipe_data, -1, sPipeData, PRIMX_MAX_PIPE_BUFF_SIZE);
					strResult = sPipeData;
					//메모리 해제
					delete[] sPipeData;
				}
			}
		}
	}

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	CloseHandle(h_pipe_read);
	CloseHandle(h_pipe_write);

	return strResult;
}

BOOL CCommonModule::GetWindowsSystemUUID(CString& strUUID)
{
	BOOL bRe = FALSE;
	
	CString strResult = SetCreateProcesseForResult(_T("wmic csproduct get uuid /value"));
	strResult.Trim();
	strResult.Replace(_T("\r"), _T(""));
	strResult.Replace(_T("\n"), _T(""));
	if (0 < strResult.GetLength()) {

		int nIndex = strResult.Find(_T("="));

		if (0 < nIndex) {
			int nLen = strResult.GetLength();
			CString strKey = strResult.Left(nIndex);
			CString strValue = strResult.Right(nLen - nIndex - 1);
			strKey.Trim();
			strValue.Trim();

			if (0 == strKey.CompareNoCase(_T("UUID"))) {
				if (0 < strValue.GetLength()) {
					strValue.Trim();
					strUUID = strValue;
					bRe = TRUE;
				}
			}
		}
	}
	return bRe;
}
BOOL CCommonModule::SetCreateProcesseEx(CString strProcess, DWORD& dwExitCode, DWORD dwTimeout)
{
	BOOL bRe = FALSE;
	dwExitCode = ERROR_INSTALL_FAILURE;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
	si.wShowWindow = SW_HIDE;

	if (!CreateProcess(NULL,   // No module name (use command line)
		(LPTSTR)LPCTSTR(strProcess),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		return bRe;
	}

	if (WAIT_OBJECT_0 == WaitForSingleObject(pi.hProcess, dwTimeout * 1000)) {
		if (FALSE == GetExitCodeProcess(pi.hProcess, &dwExitCode)) {
			dwExitCode = ERROR_INSTALL_FAILURE;
		}
		else {
			bRe = TRUE;
		}
	}

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return bRe;
}
CString CCommonModule::GetDateTimeForGMT()
{
	CString strDateTime;

	__time64_t t;
	t = _time64(NULL);
	struct tm *ConvertTime = _gmtime64(&t);

	if (NULL != ConvertTime) {
		strDateTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
			ConvertTime->tm_year + 1900,
			ConvertTime->tm_mon + 1,
			ConvertTime->tm_mday,
			ConvertTime->tm_hour,
			ConvertTime->tm_min,
			ConvertTime->tm_sec);

	}
	return strDateTime;
}

void CCommonModule::SetProcessCommandLineCheckForKill(CString strProcessName, CString strCommandLineString)
{

	CString strCmd;
	strCmd.Format(_T("wmic process where name=\"%s\" get processid,CommandLine /value"), strProcessName);
	CString strResult = SetCreateProcesseForResult(strCmd);

	strResult.Trim();
	strResult.Replace(_T("\r"), _T(""));
	strResult = strResult + _T("\n\n\n");

	if (0 < strResult.GetLength()) {
		int nStartIndex = 0;
		int nIndex = strResult.Find(_T("\n\n\n"));
		if (0 <= nIndex) {
			while (1) {
				CString strTemp = strResult.Mid(nStartIndex, nIndex - nStartIndex);
				GetProcessData3(strTemp, strCommandLineString);
				nStartIndex = nIndex + 3;
				nIndex = strResult.Find(_T("\n\n\n"), nStartIndex);
				if (0 > nIndex) break;
			}
		}
	}

}

BOOL CCommonModule::GetProcessData3(CString strData, CString strCommandLineString)
{
	UINT nProcessID = 0;
	CString strCommandLine;

	CString strColumn = _T("");
	int nIndex = 0;
	BOOL bExtract = TRUE;
	while (TRUE == bExtract) {
		bExtract = AfxExtractSubString(strColumn, (LPCTSTR)strData, nIndex, '\n');
		if (TRUE == bExtract) {
			strColumn.Trim();
			int nIndex = strColumn.Find(_T("="));

			if (0 < nIndex) {
				int nLen = strColumn.GetLength();
				CString strKey = strColumn.Left(nIndex);
				CString strValue = strColumn.Right(nLen - nIndex - 1);
				strKey.Trim();
				strValue.Trim();

				if (0 == strKey.CompareNoCase(_T("CommandLine"))) {
					strCommandLine = strValue;
				}
				if (0 == strKey.CompareNoCase(_T("processid"))) {
					nProcessID = (UINT)_ttof(LPCTSTR(strValue));
				}
			}
		}
		nIndex++;
	}

	if (0 < nProcessID && nProcessID != GetCurrentProcessId()) {
		strCommandLineString.MakeLower();
		strCommandLine.MakeLower();
		if (0 <= strCommandLine.Find(strCommandLineString)) {
			CString strCommand;
			DWORD dwExitCode = 0;
			strCommand.Format(_T("taskkill /F /PID %lu"), nProcessID);
			SetCreateProcesseEx(strCommand, dwExitCode,3);
		}
	}

	return TRUE;
}
// 리소스에 존재하는 파일 내용을 해당 경로에 저장.
BOOL CCommonModule::SetMakeFile(WORD iResource, CString strPath, CString strModule)
{
	HRSRC hRes = NULL;
	hRes = FindResource(NULL, MAKEINTRESOURCE(iResource), strModule);
	if (hRes == NULL) {
		return FALSE;
	}

	LPVOID mem = LoadResource(NULL, hRes);
	if (mem == NULL) {
		return FALSE;
	}

	DWORD dwBufferSize = SizeofResource(NULL, hRes);
	HANDLE handle = CreateFile(strPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	DWORD dwWriteSize = 0;
	BOOL bResult = WriteFile(handle, mem, dwBufferSize, &dwWriteSize, NULL);

	CloseHandle(handle);

	return bResult;
}

BOOL CCommonModule::SetUserData(CString strUserXMLData)
{
	BOOL bRe = FALSE;

	CIPCChannel* pIPCChannel = new CIPCChannel;

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"사용자 정보 저장 요청");

	if (TRUE == pIPCChannel->SetConnect(PRIMX_IPC_AGNET_CS_KEY, 1024 * 10)) {
		CARIAEncryption ARIAEncryption;
		CString strEncData = ARIAEncryption.EncryptString(strUserXMLData);
		
		UINT nDataLen = PRIMXCS_REQUEST_DATA_LEN + ((strEncData.GetLength() + 1) * sizeof(TCHAR));
		char* pData = new char[nDataLen];
		ZeroMemory(pData, nDataLen);

		PRIMXCS_REQUEST_DATA* pRegData = (PRIMXCS_REQUEST_DATA*)pData;
		pRegData->nRequestType = PRIMX_REQUEST_TYPE_USER_DATA_SAVE;

		_tcsncpy_s((TCHAR*)(pData+ PRIMXCS_REQUEST_DATA_LEN),(nDataLen- PRIMXCS_REQUEST_DATA_LEN), (LPCTSTR)strEncData, _TRUNCATE);

		if (PRIMX_IPC_ERROR_NOT_SUCCESS == pIPCChannel->SetSendData(pData, nDataLen, 10)) {

			__time64_t tCheckTime = _time64(NULL);
			while (1) {
				__time64_t tCurrentTime = _time64(NULL);
				if ((tCheckTime + 30) < tCurrentTime) {
					break;
				}

				if (0 < pIPCChannel->GetReceiveDataCount()) {
					char* pData = NULL;
					UINT nDataLen = 0;
					if (TRUE == pIPCChannel->GetReceiveData(pData, nDataLen)) {
						PRIMXCS_REQUEST_DATA* pResponseData = (PRIMXCS_REQUEST_DATA*)pData;
						if (1 == pResponseData->nResult) {
							theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"사용자 정보 저장 요청 > 저장 성공");
							bRe = TRUE;
						}
					}
					break;
				}
				Sleep(200);
			}
		}
		else{
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"사용자 정보 저장 요청 > 요청 명령 전송 실패");
		}

		pIPCChannel->SetClose();
		_SafeDelete(pRegData);
	}
	else{
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"사용자 정보 저장 요청 > IPC 연결 실패");
	}
	_SafeDelete(pIPCChannel);
	return bRe;
}