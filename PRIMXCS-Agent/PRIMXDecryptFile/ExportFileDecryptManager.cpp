#include "stdafx.h"
#include "ExportFileDecryptManager.h"
#include "ARIAEncryption.h"
#include "Markup.h"
#include "sha512.h"
#include "ProgressDlg.h"


//#include "zedapi.h"
//#pragma comment(lib, "zedxr.lib")



CExportFileDecryptManager::CExportFileDecryptManager()
{

}

CExportFileDecryptManager::~CExportFileDecryptManager()
{
	if (NULL != m_hZedDll) {
		::FreeLibrary(m_hZedDll);
	}
	SetDeleteFile(m_strZedFilePath);
}

BOOL CExportFileDecryptManager::SetInitDll()
{
	BOOL bRe = FALSE;

	m_hZedDll = ::LoadLibrary(L"zedxr.dll");

	if (NULL != m_hZedDll) {
		//HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
		//if (SUCCEEDED(hres))
		{
			m_pZEDAPI_FormatMessage = (ZEDAPI_FormatMessage)::GetProcAddress(m_hZedDll, "ZEDAPI_FormatMessage");
			m_pZEDAPI_OpenContainer = (ZEDAPI_OpenContainer)::GetProcAddress(m_hZedDll, "ZEDAPI_OpenContainer");
			m_pZEDAPI_ProgressRoutine_Ptr = (ZEDAPI_ProgressRoutine_Ptr)::GetProcAddress(m_hZedDll, "ZEDAPI_ProgressRoutine_Ptr");
			m_pZEDAPI_CloseHandle = (ZEDAPI_CloseHandle)::GetProcAddress(m_hZedDll, "ZEDAPI_CloseHandle");
			m_pZEDAPI_ExtractFile = (ZEDAPI_ExtractFile)::GetProcAddress(m_hZedDll, "ZEDAPI_ExtractFile");
			m_pZEDAPI_FindFirstFile = (ZEDAPI_FindFirstFile)::GetProcAddress(m_hZedDll, "ZEDAPI_FindFirstFile");
			m_pZEDAPI_FindNextFile = (ZEDAPI_FindNextFile)::GetProcAddress(m_hZedDll, "ZEDAPI_FindNextFile");
			m_pZEDAPI_FindClose = (ZEDAPI_FindClose)::GetProcAddress(m_hZedDll, "ZEDAPI_FindClose");			
			bRe = TRUE;
		}
	}
	return bRe;
}

CString CExportFileDecryptManager::ZEDAPI_FormatMessageEx(HRESULT hr)
{
	CString strErrorString;
	if (NULL == m_pZEDAPI_OpenContainer) return strErrorString;

	WCHAR wzMessage[1024] = { 0 };
	
	unsigned int cch = 0;
	m_pZEDAPI_FormatMessage(hr, NULL, &cch);

	WCHAR* pwzMsg = new WCHAR[cch+1];
	m_pZEDAPI_FormatMessage(hr, pwzMsg, &cch);
	strErrorString = pwzMsg;
	delete [] pwzMsg;

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZEDAPI_FormatMessage > %s"), strErrorString);

	return strErrorString;
}

BOOL CExportFileDecryptManager::SetAuthXMLParser(CString strXMLData)
{
	BOOL bRe = FALSE;

	CMarkup XMLMarkup;

	if (TRUE == XMLMarkup.SetDoc(strXMLData)) {
		
		if (true == XMLMarkup.FindElem()) {
			CString strElName = XMLMarkup.GetTagName();
			if (0 == strElName.CompareNoCase(_T("ExportFile"))) {
				XMLMarkup.IntoElem();
				while (true == XMLMarkup.FindElem()) {
					CString strDataame = XMLMarkup.GetTagName();

					if (0 == strDataame.CompareNoCase(_T("Key"))) {
						m_strKey = XMLMarkup.GetData();
					}
					if (0 == strDataame.CompareNoCase(_T("AgentID"))) {
						m_strAgentID = XMLMarkup.GetData();
					}
					if (0 == strDataame.CompareNoCase(_T("ExpiryDate"))) {
						m_strExpiryDate = XMLMarkup.GetData();
					}
					if (0 == strDataame.CompareNoCase(_T("CreateDate"))) {
						m_strCreateDate = XMLMarkup.GetData();
					}
					if (0 == strDataame.CompareNoCase(_T("CompanyID"))) {
						m_strCompanyID = XMLMarkup.GetData();
					}
					if (0 == strDataame.CompareNoCase(_T("UserID"))) {
						m_strUserID = XMLMarkup.GetData();
					}
					if (0 == strDataame.CompareNoCase(_T("AuthServer"))) {
						m_strAuthServer = XMLMarkup.GetData();
					}
					if (0 == strDataame.CompareNoCase(_T("AuthServerPort"))) {
						m_strAuthServerPort = XMLMarkup.GetData();
					}
					if (0 == strDataame.CompareNoCase(_T("DecryptID"))) {
						m_strDecryptID = XMLMarkup.GetData();
					}
					if (0 == strDataame.CompareNoCase(_T("DecryptPwd"))) {
						m_strDecryptPwd = XMLMarkup.GetData();
					}
					if (0 == strDataame.CompareNoCase(_T("ZedFileHash"))) {
						m_strZedFileHash = XMLMarkup.GetData();
					}
				}
				XMLMarkup.OutOfElem();
			}
		}
		XMLMarkup.ResetPos();
	}


	if (0 < m_strKey.GetLength() &&
		0 < m_strAgentID.GetLength() &&
		0 < m_strAuthServer.GetLength() &&
		0 < m_strAuthServerPort.GetLength() &&
		0 < m_strDecryptID.GetLength() &&
		0 < m_strDecryptPwd.GetLength() &&
		0 < m_strZedFileHash.GetLength()) 
	{
		bRe = TRUE;
	}

	return bRe;
}

BOOL CExportFileDecryptManager::SetAuthDataDecrypt()
{
#ifdef _DEBUG
	CString strFilePath = L"c:\\temp1\\TEST01.exe";
#else
	CString strFilePath = GetLocalFullPath();
#endif

	BOOL bRe = FALSE;
	HANDLE hFile = CreateFile((LPCTSTR)strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile) {
		m_strErrorString.LoadString(IDS_ERROR_STRING1);
		return bRe;
	}

	HANDLE hMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
	if (NULL == hMap) {
		m_strErrorString.LoadString(IDS_ERROR_STRING1);
		CloseHandle(hFile);
		return bRe;
	}

	void *pBase = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	if (NULL == pBase) {
		m_strErrorString.LoadString(IDS_ERROR_STRING1);
		CloseHandle(hMap);
		CloseHandle(hFile);
		return bRe;
	}

	unsigned char MagicData[10] = { '\r','\n','\r','\n',0x07,0x0e,0x07,0x0e,0x07,0x0e };

	LONGLONG luSize = GetFileSizeEx(hFile);

	unsigned char* pSeek = (unsigned char*)pBase;

	CString strMasterKey;
	CString strAuthXML;

	BOOL bExportFile = FALSE;
	for (LONGLONG i = 0; i < (luSize-10); i++) {

		if (0 == memcmp(MagicData, pSeek, 10)) {
			bExportFile = TRUE;
			m_strErrorString.LoadString(IDS_ERROR_STRING3);

			pSeek = pSeek + 10;

			DWORD dwMasterKeyLen = 0;
			DWORD dwAuthXMLLen = 0;
			LONGLONG nZEDFileLen = 0;

			memcpy((void*)&dwMasterKeyLen, pSeek, sizeof(DWORD));
			pSeek = pSeek + sizeof(DWORD);

			memcpy((void*)&dwAuthXMLLen, pSeek, sizeof(DWORD));
			pSeek = pSeek + sizeof(DWORD);

			memcpy((void*)&nZEDFileLen, pSeek, sizeof(LONGLONG));
			pSeek = pSeek + sizeof(LONGLONG);

			if (0 < dwMasterKeyLen) {
				unsigned char* pMasterKey = new unsigned char[dwMasterKeyLen + 1];
				ZeroMemory(pMasterKey, dwMasterKeyLen + 1);
				memcpy(pMasterKey, pSeek, dwMasterKeyLen);
				strMasterKey = AtoWConverter((char*)pMasterKey);
				delete[] pMasterKey;
				pMasterKey = NULL;
				pSeek = pSeek + dwMasterKeyLen;

				if (0 < dwAuthXMLLen) {
					unsigned char* pAuthXML = new unsigned char[dwAuthXMLLen + 1];
					ZeroMemory(pAuthXML, dwAuthXMLLen + 1);
					memcpy(pAuthXML, pSeek, dwAuthXMLLen);
					strAuthXML = AtoWConverter((char*)pAuthXML);
					delete[] pAuthXML;
					pAuthXML = NULL;
					pSeek = pSeek + dwAuthXMLLen;
				
					int nIndex = strAuthXML.Find(_T("@"));
					if (0 < nIndex) {
						CString strEncryptData = strAuthXML.Right(strAuthXML.GetLength() - (nIndex + 1));
						CString strSha512 = strAuthXML.Left(nIndex);

						CARIAEncryption ARIAEncryption;
						ARIAEncryption.SetBaseKeyData(strMasterKey);
						CString strAuthDataHash = ARIAEncryption.GetSaltSHA512(strEncryptData);

						if (0 == strAuthDataHash.CompareNoCase(strSha512)) {
							CString strDecryptAuthData = ARIAEncryption.DecryptString(strEncryptData);
							if (TRUE == SetAuthXMLParser(strDecryptAuthData)) {
								m_strErrorString = _T("");
								bRe = TRUE;
							}
						}
					}		
				}				
			}
			break;
		}
		pSeek++;
	}

	if (FALSE == bExportFile) {
		m_strErrorString.LoadString(IDS_ERROR_STRING2);
	}

	CloseHandle(hMap);
	CloseHandle(hFile);

	return bRe;
}

BOOL CExportFileDecryptManager::SetZedFileDecrypt()
{
#ifdef _DEBUG
	CString strFilePath = L"c:\\temp1\\TEST01.exe";
#else
	CString strFilePath = GetLocalFullPath();
#endif

	BOOL bRe = FALSE;
	HANDLE hFile = CreateFile((LPCTSTR)strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile) {
		return bRe;
	}

	HANDLE hMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
	if (NULL == hMap) {
		CloseHandle(hFile);
		return bRe;
	}

	void *pBase = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	if (NULL == pBase) {
		CloseHandle(hMap);
		CloseHandle(hFile);
		return bRe;
	}

	unsigned char MagicData[10] = { '\r','\n','\r','\n',0x07,0x0e,0x07,0x0e,0x07,0x0e };

	LONGLONG luSize = GetFileSizeEx(hFile);

	unsigned char* pSeek = (unsigned char*)pBase;

	CString strMasterKey;
	CString strAuthXML;

	for (LONGLONG i = 0; i < luSize; i++) {

		if (0 == memcmp(MagicData, pSeek, 10)) {

			pSeek = pSeek + 10;

			DWORD dwMasterKeyLen = 0;
			DWORD dwAuthXMLLen = 0;
			LONGLONG nZEDFileLen = 0;

			memcpy((void*)&dwMasterKeyLen, pSeek, sizeof(DWORD));
			pSeek = pSeek + sizeof(DWORD);

			memcpy((void*)&dwAuthXMLLen, pSeek, sizeof(DWORD));
			pSeek = pSeek + sizeof(DWORD);

			memcpy((void*)&nZEDFileLen, pSeek, sizeof(LONGLONG));
			pSeek = pSeek + sizeof(LONGLONG);

			if (0 < dwMasterKeyLen) {
				unsigned char* pMasterKey = new unsigned char[dwMasterKeyLen + 1];
				ZeroMemory(pMasterKey, dwMasterKeyLen + 1);
				memcpy(pMasterKey, pSeek, dwMasterKeyLen);
				strMasterKey = AtoWConverter((char*)pMasterKey);
				delete[] pMasterKey;
				pMasterKey = NULL;
				pSeek = pSeek + dwMasterKeyLen;
				
				if (0 < dwAuthXMLLen) {
					unsigned char* pAuthXML = new unsigned char[dwAuthXMLLen + 1];
					ZeroMemory(pAuthXML, dwAuthXMLLen + 1);
					memcpy(pAuthXML, pSeek, dwAuthXMLLen);
					strAuthXML = AtoWConverter((char*)pAuthXML);
					delete[] pAuthXML;
					pAuthXML = NULL;
					pSeek = pSeek + dwAuthXMLLen;

					if (0 < nZEDFileLen) {

						CString strTempJobPath;
						CString strZedFilePath;
						strTempJobPath.Format(_T("%s\\ExFileData"), GetProgramDataPath());
						strZedFilePath.Format(_T("%s\\ExFileData\\%s.zip"), GetProgramDataPath(), GetCreateKey());

						SetCreateDirectoryEx(strTempJobPath);
						SetDeleteFile(strZedFilePath);

						HANDLE hZedFile = CreateFile(strZedFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

						if (INVALID_HANDLE_VALUE != hZedFile) {

							DWORD dwWriteSize = 0;
							WriteFile(hZedFile, (void*)pSeek, nZEDFileLen, &dwWriteSize, NULL);
							CloseHandle(hZedFile);
							m_strZedFilePath = strZedFilePath;

							int nIndex = strAuthXML.Find(_T("@"));
							if (0 < nIndex) {
								CString strEncryptData = strAuthXML.Right(strAuthXML.GetLength() - (nIndex + 1));
								CString strSha512 = strAuthXML.Left(nIndex);

								CARIAEncryption ARIAEncryption;
								ARIAEncryption.SetBaseKeyData(strMasterKey);
								CString strAuthDataHash = ARIAEncryption.GetSaltSHA512(strEncryptData);

								if (0 == strAuthDataHash.CompareNoCase(strSha512)) {
									CString strAuthDataTemp = ARIAEncryption.DecryptString(strEncryptData);

									if (TRUE == SetAuthXMLParser(strAuthDataTemp)) {
										CSHA512 SHA512;
										CString strHashData = SHA512.GetShaBase64Data(strZedFilePath);
										if (0 == strHashData.CompareNoCase(m_strZedFileHash)) {
											bRe = TRUE;
										}
									}
								}
							}
						}
					}
				}
			}
			break;
		}
		pSeek++;
	}

	CloseHandle(hMap);
	CloseHandle(hFile);

	return bRe;
}

void CExportFileDecryptManager::GetZedFileInfo(CList<PRIMX_ORG_FILE_INFO*>& FileLsit)
{
	if (NULL == m_pZEDAPI_OpenContainer) return;
	if (NULL == m_pZEDAPI_FindFirstFile) return;
	if (NULL == m_pZEDAPI_FindNextFile) return;
	if (NULL == m_pZEDAPI_FindClose) return;
	if (NULL == m_pZEDAPI_CloseHandle) return;


	ZEDAPI_HANDLE ghZed = NULL;

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZEDAPI_OpenContainer > 호출"));

	HRESULT hr = m_pZEDAPI_OpenContainer(m_strZedFilePath, ZEDAPI_CRED_PWD, m_strDecryptID, m_strDecryptPwd, &ghZed);
	if (FAILED(hr)) {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZEDAPI_OpenContainer > 호출 > 실패"));
		return;
	}

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZEDAPI_OpenContainer > 호출 > 성공"));

	ZEDAPI_FIND_DATA fd;
	ZEDAPI_FIND_HANDLE hFind;
	WCHAR extractPath[MAX_PATH] = { 0 };

	hr = m_pZEDAPI_FindFirstFile(ghZed, L"*", &hFind, &fd);
	if (FAILED(hr) && hr != E_FILE_NOT_FOUND) {
		return;
	}
	while (SUCCEEDED(hr)) {
		if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
			TRACE(L"- Folder %ls\n", fd.cFileName);
		}
		else {
			PRIMX_ORG_FILE_INFO* pData = new PRIMX_ORG_FILE_INFO;
			pData->strFilePath = fd.cFileName;
			pData->nFileSize = (fd.nFileSizeHigh * (MAXDWORD + 1)) + fd.nFileSizeLow;
			if (NULL == FileLsit.AddTail(pData)) {
				_SafeDelete(pData);
			}
		}		
		/* Next file */
		hr = m_pZEDAPI_FindNextFile(ghZed, hFind, &fd);
		if (FAILED(hr) && hr != E_NO_MORE_FILES) {
			break;
		}
	}

	hr = m_pZEDAPI_FindClose(ghZed, hFind);
	
	if (ghZed != NULL) {
		hr = m_pZEDAPI_CloseHandle(ghZed);
		ghZed = NULL;
	}
}

BOOL CExportFileDecryptManager::SetExtractionFile(CString strSavePath,CStringList& ExtractionFileLsit, CString& strErrorString, CProgressDlg* pProgressDlg)
{
	if (NULL == m_pZEDAPI_OpenContainer) return FALSE;
	if (NULL == m_pZEDAPI_ExtractFile) return FALSE;
	if (NULL == m_pZEDAPI_CloseHandle) return FALSE;

	if (NULL != pProgressDlg) pProgressDlg->SetProgressValueEx(0, GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG15));

	ZEDAPI_HANDLE ghZed = NULL;
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZEDAPI_OpenContainer > 호출"));
	HRESULT hr = m_pZEDAPI_OpenContainer(m_strZedFilePath, ZEDAPI_CRED_PWD, m_strDecryptID, m_strDecryptPwd, &ghZed);
	if (FAILED(hr)) {
		strErrorString = ZEDAPI_FormatMessageEx(hr);
		if (NULL != pProgressDlg) pProgressDlg->SetProgressValueEx(95, strErrorString);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZEDAPI_OpenContainer > 호출 > 실패"));
		return FALSE;
	}
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZEDAPI_OpenContainer > 호출 > 성공"));

	BOOL bRe = TRUE;
	POSITION Pos = ExtractionFileLsit.GetHeadPosition();
	while (NULL != Pos) {
		CString strFileName = ExtractionFileLsit.GetNext(Pos);
		CString strExtractFilePath;
		strExtractFilePath.Format(_T("%s\\%s"), strSavePath, strFileName);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZEDAPI_ExtractFile > 호출"));

		HRESULT hr = m_pZEDAPI_ExtractFile(ghZed, strFileName, strExtractFilePath,ZEDAPI_REPLACE_EXISTING, NULL, NULL);
		if (SUCCEEDED(hr)) {
			CString strMsg;
			strMsg.Format(IDS_EXTRACT_FILE_MSG1, strFileName);
			if (NULL != pProgressDlg) pProgressDlg->SetProgressValueEx(0, strMsg);
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZEDAPI_ExtractFile > 호출 > 성공"));
		}
		else {
			bRe = FALSE;
			strErrorString = ZEDAPI_FormatMessageEx(hr);
			CString strMsg;
			strMsg.Format(IDS_EXTRACT_FILE_MSG2, strFileName, strErrorString);
			if (NULL != pProgressDlg) pProgressDlg->SetProgressValueEx(95, strMsg);
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ZEDAPI_ExtractFile > 호출 > 실패"));
			break;
		}
	}

	if (ghZed != NULL) {
		hr = m_pZEDAPI_CloseHandle(ghZed);
		ghZed = NULL;
	}
	
	CString strMsg;
	strMsg.LoadString(IDS_EXTRACT_FILE_MSG3);
	if (NULL != pProgressDlg) pProgressDlg->SetProgressValueEx(95, strMsg);

	return bRe;
}