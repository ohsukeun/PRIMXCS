#pragma once
#include "CommonModule.h"
#include "zedapi.h"

class CProgressDlg;
class CExportFileDecryptManager : public CCommonModule
{
	typedef HRESULT(*ZEDAPI_FormatMessage)(HRESULT hrCode,WCHAR * pwzMessage,unsigned int * pcch);
	typedef HRESULT(*ZEDAPI_OpenContainer)(const WCHAR * pcwzContainerName, DWORD dwFlags, const WCHAR * pcwzCredName, const WCHAR * pcwzCredSecret, ZEDAPI_HANDLE * pzedhdl);
	typedef HRESULT(*ZEDAPI_ProgressRoutine_Ptr)(void * pvData, unsigned __int64 ui64Size, unsigned __int64 ui64Done);
	typedef HRESULT(*ZEDAPI_CloseHandle)(ZEDAPI_HANDLE zedhdl);
	typedef HRESULT(*ZEDAPI_ExtractFile)(ZEDAPI_HANDLE zedhdl, const WCHAR * pcwzExistingFileName, const WCHAR * pcwzNewFileName, DWORD dwFlags, ZEDAPI_ProgressRoutine_Ptr pfnProgressRoutine, void * pvData);
	typedef HRESULT(*ZEDAPI_FindFirstFile)(ZEDAPI_HANDLE zedhdl,const WCHAR * pcwzSearchFileName,ZEDAPI_FIND_HANDLE * phFindFile,ZEDAPI_FIND_DATA * pstFindFileData);
	typedef HRESULT(*ZEDAPI_FindNextFile)(ZEDAPI_HANDLE zedhdl,	ZEDAPI_FIND_HANDLE hFindFile,ZEDAPI_FIND_DATA * pstFindFileData);
	typedef HRESULT(*ZEDAPI_FindClose)(ZEDAPI_HANDLE zedhdl,ZEDAPI_FIND_HANDLE hFindFile);
	
public:
	CExportFileDecryptManager();
	~CExportFileDecryptManager();
	
	BOOL SetInitDll();

	BOOL SetAuthDataDecrypt();
	BOOL SetZedFileDecrypt();
	void GetZedFileInfo(CList<PRIMX_ORG_FILE_INFO*>& FileLsit);

	BOOL SetExtractionFile(CString strSavePath, CStringList& ExtractionFileLsit, CString& strErrorString, CProgressDlg* pProgressDlg);

	CString			m_strKey;
	CString			m_strAgentID;
	CString			m_strExpiryDate;
	CString			m_strCreateDate;
	CString			m_strCompanyID;
	CString			m_strAuthServer;
	CString			m_strAuthServerPort;
	CString			m_strDecryptID;
	CString			m_strDecryptPwd;
	CString			m_strZedFileHash;
	CString			m_strZedFilePath;
	CString			m_strUserID;

	CString			m_strErrorString;


protected:

	BOOL SetAuthXMLParser(CString strXMLData);
	CString ZEDAPI_FormatMessageEx(HRESULT hr);

	ZEDAPI_FormatMessage			m_pZEDAPI_FormatMessage = NULL;
	ZEDAPI_OpenContainer			m_pZEDAPI_OpenContainer = NULL;
	ZEDAPI_ProgressRoutine_Ptr		m_pZEDAPI_ProgressRoutine_Ptr = NULL;
	ZEDAPI_CloseHandle				m_pZEDAPI_CloseHandle = NULL;
	ZEDAPI_ExtractFile				m_pZEDAPI_ExtractFile = NULL;
	ZEDAPI_FindFirstFile			m_pZEDAPI_FindFirstFile = NULL;
	ZEDAPI_FindNextFile				m_pZEDAPI_FindNextFile = NULL;
	ZEDAPI_FindClose				m_pZEDAPI_FindClose = NULL;
	HMODULE							m_hZedDll = NULL;
};

