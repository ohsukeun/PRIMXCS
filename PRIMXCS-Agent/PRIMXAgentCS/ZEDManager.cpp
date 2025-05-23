#include "stdafx.h"
#include "ZEDManager.h"
#include "CIDPWGenerator.h"
#include "ProgressExDlg.h"

CZEDManager::CZEDManager()
{
	m_pZEDAPI_ProgressRoutine_Ptr = CZEDManager::Callback_ZedFileProgress;
}


CZEDManager::~CZEDManager()
{
}

BOOL CZEDManager::CreateZedFile(CString strZedFilePath, CString strOrgFilePath)
{
	BOOL bRe = FALSE;

	HRESULT hResult = -1;
	ZEDAPI_HANDLE zedHdl = ZEDAPI_INVALID_HANDLE;
	
	if (FALSE == FileExist(strOrgFilePath)) {
		m_strErrorString.LoadString(IDS_ERROR_STRING22);
		return bRe;
	}

	if (TRUE == FileExist(strZedFilePath)) {
		::DeleteFile(strZedFilePath);
	}

	hResult = ZEDAPI_CreateContainer(strZedFilePath, ZEDAPI_REPLACE_EXISTING, &zedHdl);
	if (SUCCEEDED(hResult)){
		
		hResult = ZEDAPI_AddAccessPassword2(zedHdl, L"PRIMXAgent", L"Rimahr9280!", 0x00, ZEDAPI_ATTRIBUTE_NONE,NULL);
		if (SUCCEEDED(hResult)){
			
			hResult = ZEDAPI_AddFile(zedHdl, strOrgFilePath, SetSplitPathFileName(strOrgFilePath), ZEDAPI_REPLACE_EXISTING, m_pZEDAPI_ProgressRoutine_Ptr, (void*)this);
			if (SUCCEEDED(hResult))
			{
				bRe = TRUE;
			}
			else {
				m_strErrorString = GetZEDErrorMessage(hResult);
			}
		}
		else {
			m_strErrorString = GetZEDErrorMessage(hResult);
		}
		ZEDAPI_CloseHandle(zedHdl);

		if (FALSE == bRe) {
			if (TRUE == FileExist(strZedFilePath)) {
				::DeleteFile(strZedFilePath);
			}
		}
	}
	else {
		m_strErrorString = GetZEDErrorMessage(hResult);
	}

	return bRe;
}

BOOL CZEDManager::CreateZedFile(CString strZedFilePath, CList<PRIMX_ORG_FILE_INFO*>& OrgFileList, CString& strID, CString& strPwd, CProgressExDlg* pProgressExDlg)
{
	BOOL bRe = FALSE;

	HRESULT hResult = -1;
	ZEDAPI_HANDLE zedHdl = ZEDAPI_INVALID_HANDLE;

	if (0 == OrgFileList.GetCount()) {
		m_strErrorString.LoadString(IDS_ERROR_STRING22);
		return bRe;
	}

	if (TRUE == FileExist(strZedFilePath)) {
		::DeleteFile(strZedFilePath);
	}

	CreateRandomIDPW(strID, strPwd);

	if (0 == strID.GetLength()) {
		strID = L"PRIMXAgent";
	}
	if (0 == strPwd.GetLength()) {
		strID = L"Rimahr9280!";
	}
	
	hResult = ZEDAPI_CreateContainer(strZedFilePath, ZEDAPI_REPLACE_EXISTING, &zedHdl);
	if (SUCCEEDED(hResult)) {		
		hResult = ZEDAPI_AddAccessPassword2(zedHdl, strID, strPwd, 0x00, ZEDAPI_ATTRIBUTE_NONE, NULL);
		if (SUCCEEDED(hResult)) {

			POSITION Pos = OrgFileList.GetHeadPosition();
			while (NULL != Pos) {
				PRIMX_ORG_FILE_INFO* pOrgFile = OrgFileList.GetNext(Pos);
				if (NULL != pOrgFile) {
					if (TRUE == FileExist(pOrgFile->strFilePath)) {

						if (NULL != pProgressExDlg) {
							CString strTemp;
							strTemp.Format(PRIMX_PROGRESS_MSG1, pOrgFile->strFilePath);
							pProgressExDlg->SetProgressValue(0, strTemp);
						}

						hResult = ZEDAPI_AddFile(zedHdl, pOrgFile->strFilePath, SetSplitPathFileName(pOrgFile->strFilePath), ZEDAPI_REPLACE_EXISTING, m_pZEDAPI_ProgressRoutine_Ptr, (void*)this);
						if (SUCCEEDED(hResult))
						{
							bRe = TRUE;
						}
						else {
							bRe = FALSE;
							m_strErrorString = GetZEDErrorMessage(hResult);
							break;
						}
					}
					else{
						bRe = FALSE;
						m_strErrorString.LoadString(IDS_ERROR_STRING22);
						break;
					}
				}

			}
		}
		else {
			m_strErrorString = GetZEDErrorMessage(hResult);
		}
		ZEDAPI_CloseHandle(zedHdl);

		if (FALSE == bRe) {
			if (TRUE == FileExist(strZedFilePath)) {
				::DeleteFile(strZedFilePath);
			}
		}
	}
	else {
		m_strErrorString = GetZEDErrorMessage(hResult);
	}

	return bRe;
}

HRESULT CZEDManager::Callback_ZedFileProgress(void* pvData, unsigned __int64 ui64Size, unsigned __int64 ui64Done)
{
	CZEDManager* pModuleObj = (CZEDManager*)pvData;


	return S_OK;
}

CString CZEDManager::GetZEDErrorMessage(HRESULT hrResult)
{
	CString strMsg;
	unsigned int cch = 0;
	ZEDAPI_FormatMessage(hrResult, NULL, &cch);
	if (0 < cch) {
		WCHAR* pwzMsg = (WCHAR*)malloc(cch * sizeof(*pwzMsg));
		ZEDAPI_FormatMessage(hrResult, pwzMsg, &cch);
		strMsg = pwzMsg;
		free(pwzMsg);
	}
	else {
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			hrResult,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL
		);
		strMsg = (LPCTSTR)lpMsgBuf;
		LocalFree(lpMsgBuf);
		strMsg.Replace(_T("\r"), _T(""));
		strMsg.Replace(_T("\n"), _T(""));
	}
	return strMsg;
}

BOOL CZEDManager::FileExist(CString strPath)
{
	BOOL bRe = FALSE;
	CFileFind	Find;
	bRe = Find.FindFile(LPCTSTR(strPath));
	Find.Close();
	return bRe;
}

CString CZEDManager::SetSplitPathFileName(CString strPath)
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

BOOL CZEDManager::CreateRandomIDPW(CString& strRandomID, CString& strRandomPW)
{
	std::string szID;
	std::string szPW;

	CIDPWGenerator IDPWGen;

	IDPWGen.Generate_ID(szID, 3, 4, TRUE);
	IDPWGen.Generate_Password(szPW, 12, 2, 2, 1, 1, TRUE);
	
	strRandomID = AtoWConverter(szID.c_str());
	strRandomPW = AtoWConverter(szPW.c_str());
	strRandomID.Trim();
	strRandomPW.Trim();

	return TRUE;
}