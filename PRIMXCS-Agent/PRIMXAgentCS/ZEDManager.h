#pragma once

#include "zedapi.h"
#pragma comment(lib, "zedapi.lib")

class CProgressExDlg;
class CZEDManager
{
public:
	CZEDManager();
	~CZEDManager();

	BOOL CreateZedFile(CString strZedFilePath, CString strOrgFilePath);
	BOOL CreateZedFile(CString strZedFilePath, CList<PRIMX_ORG_FILE_INFO*>& OrgFileList,CString& strID,CString& strPwd, CProgressExDlg* pProgressExDlg);
	CString GetZEDErrorMessage(HRESULT hrResult);

protected:

	static HRESULT __stdcall Callback_ZedFileProgress(void* pvData, unsigned __int64 ui64Size, unsigned __int64 ui64Done);
	ZEDAPI_ProgressRoutine_Ptr m_pZEDAPI_ProgressRoutine_Ptr;
	CString SetSplitPathFileName(CString strPath);
	BOOL FileExist(CString strPath);
	BOOL CreateRandomIDPW(CString& strRandomID, CString& strRandomPW);

	CString			m_strErrorString;

};

