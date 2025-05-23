#include "stdafx.h"
#include "CommandUpdateFile.h"
#include "ConfigDataManager.h"
#include "SystemDataManager.h"
#include "RyeolHttpClient.h"

using namespace Ryeol;


CCommandUpdateFile::CCommandUpdateFile()
{
	m_nResUpdateFileSize = 0;
	m_bFileDownload = FALSE;

}

CCommandUpdateFile::~CCommandUpdateFile()
{
	m_DownloadURLList.RemoveAll();
}

CString CCommandUpdateFile::GetUpdateFileHaseData()
{
	return m_strResHashdata;
}

CString CCommandUpdateFile::GetUpdateVersion()
{
	return m_strResUpdateVersion;
}

BOOL CCommandUpdateFile::GetUpdateFile(CString strProductType, CString strUpdateFilePath)
{
	CConfigDataManager ConfigDataManager;
	m_strAgentID = ConfigDataManager.GetAgentID();
	m_strAgentVersion = ConfigDataManager.GetAgentVersion();
	m_strUserID = ConfigDataManager.GetUserID();
	m_nSystemType = ConfigDataManager.GetSystemType();
	m_strCode = _T("UpdateFile");
	m_strSubCode = _T("Request");
	m_strProductType = strProductType;
	m_strUpdateFilePath = strUpdateFilePath;

	BOOL bRe = FALSE;
	CSystemDataManager SystemDataManager;
	if (TRUE == SystemDataManager.GetPRIMXProductInfo(m_ZonecentrolData, m_OrizoneData, m_CryhodData)) {
		bRe = SetCommandDataRequest(m_strCode, m_strSubCode, m_strAgentID, m_strUserID, m_strUpdateFilePath);
	}

	if (TRUE == bRe && TRUE == m_bFileDownload) {
		return TRUE;
	}

	return FALSE;
}

UINT CCommandUpdateFile::SetResponseDataTypeParser(CString& strXML)
{
	UINT nRe = PRIMX_ERROR_XML_10001;

	CMarkup XMLMarkup;

	if (TRUE == XMLMarkup.SetDoc(strXML)) {

		if (true == XMLMarkup.FindElem()) {
			CString strRequestName = XMLMarkup.GetTagName();
			if (0 == strRequestName.CompareNoCase(_T("response"))) {
				XMLMarkup.IntoElem();
				while (true == XMLMarkup.FindElem()) {
					CString strELName = XMLMarkup.GetTagName();
					if (0 == strELName.CompareNoCase(_T("DT:UpdateFileResponse"))) {
						XMLMarkup.IntoElem();
						while (true == XMLMarkup.FindElem()) {
							CString strELNameReply = XMLMarkup.GetTagName();

							if (0 == strELNameReply.CompareNoCase(_T("Type"))) {
								m_strResProductType = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("UpdateVersion"))) {
								m_strResUpdateVersion = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("Hashdata"))) {
								m_strResHashdata = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("FileSize"))) {
								m_nResUpdateFileSize = AtoUI(XMLMarkup.GetData());
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("DownloadInfo"))) {
								//m_strFileName = XMLMarkup.GetAttrib(_T("FileName"));
								XMLMarkup.IntoElem();
								while (true == XMLMarkup.FindElem()) {
									CString strDownloadELName = XMLMarkup.GetTagName();
									if (0 == strDownloadELName.CompareNoCase(_T("DownloadURL"))) {
										CString strDownloadURL = XMLMarkup.GetData();
										strDownloadURL.Trim();
										if (0 < strDownloadURL.GetLength()) {
											strDownloadURL = strDownloadURL;
											m_DownloadURLList.AddTail(strDownloadURL);
										}
									}
								}
								XMLMarkup.OutOfElem();
								nRe = PRIMX_ERROR_SUCCESS;
							}
						}
						XMLMarkup.OutOfElem();
					}

				}
				XMLMarkup.OutOfElem();
			}
		}
		XMLMarkup.ResetPos();
	}
	else {
		nRe = PRIMX_ERROR_XML_10000;
	}

	if (PRIMX_ERROR_SUCCESS == nRe) {
			
		m_bFileDownload = FALSE;

		int nURLCount = int(m_DownloadURLList.GetCount());

		if (0 < nURLCount) {
			if (1 < nURLCount) {
				srand(_time32(NULL));
				for (int i = 0; i < nURLCount + _Random(10, 50); i++) {
					int nRemoveNo = _Random(0, nURLCount - 1);
					int nInsertNo = _Random(0, nURLCount - 2);
					POSITION Pos = m_DownloadURLList.FindIndex(nRemoveNo);
					CString strURL = m_DownloadURLList.GetAt(Pos);
					m_DownloadURLList.RemoveAt(Pos);
					Pos = m_DownloadURLList.FindIndex(nInsertNo);
					m_DownloadURLList.InsertBefore(Pos, strURL);
				}
			}

			POSITION Pos = m_DownloadURLList.GetHeadPosition();
			while (NULL != Pos) {
				CString strURL = m_DownloadURLList.GetNext(Pos);
				strURL.Trim();
				if (0 < strURL.GetLength()) {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"업데이트 파일 다운로드(Web) 시작 > %s > %s", strURL, m_strUpdateFilePath);
					if (TRUE == GetUpdateFileDownload(strURL, m_strUpdateFilePath)) {
						m_bFileDownload = TRUE;
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"업데이트 파일 다운로드(Web) 성공 > %s > %s", strURL, m_strUpdateFilePath);
						break;
					}
					else{
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"업데이트 파일 다운로드(Web) 실패 > %s > %s", strURL, m_strUpdateFilePath);
					}
				}
			}
		}

		if (FALSE == m_bFileDownload) {
			if (0 < m_nResUpdateFileSize) {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"업데이트 파일 다운로드 시작 > %lu Byte > %s", m_nResUpdateFileSize, m_strUpdateFilePath);

				if (m_nResUpdateFileSize != ReceiveFile(m_strUpdateFilePath, m_nResUpdateFileSize)) {
					nRe = PRIMX_ERROR_XML_10000;
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"업데이트 파일 다운로드 실패 > %lu Byte > %s", m_nResUpdateFileSize, m_strUpdateFilePath);
				}
				else {
					m_bFileDownload = TRUE;
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"업데이트 파일 다운로드 성공 > %lu Byte > %s", m_nResUpdateFileSize, m_strUpdateFilePath);
				}
			}
			else {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"업데이트 파일 다운로드 실패 > 파일 사이즈 설정 오류 > %lu Byte", m_nResUpdateFileSize);
			}
		}		
	}

	return nRe;
}

UINT CCommandUpdateFile::SetRequestDataTypeGenerate(CMarkup& XMLMarkup)
{
	UINT nRe = PRIMX_ERROR_XML_10001;

	XMLMarkup.AddElem(_T("DT:UpdateFileRequest"));
	XMLMarkup.IntoElem(); // DT:UpdateFileRequest
		
	XMLMarkup.AddElem(_T("Type"), m_strProductType);
		
	if (0 == m_strProductType.CompareNoCase(_T("Agent"))) {
		XMLMarkup.AddElem(_T("UpdateVersion"), m_strAgentVersion);
	}
	else if (0 == m_strProductType.CompareNoCase(_T("ZONECENTRAL"))) {
		XMLMarkup.AddElem(_T("UpdateVersion"), m_ZonecentrolData.strDisplayVersionEx);
	}
	else if (0 == m_strProductType.CompareNoCase(_T("ORIZON"))) {
		XMLMarkup.AddElem(_T("UpdateVersion"), m_OrizoneData.strDisplayVersionEx);
	}
	else if (0 == m_strProductType.CompareNoCase(_T("CRYHOD"))) {
		XMLMarkup.AddElem(_T("UpdateVersion"), m_CryhodData.strDisplayVersionEx);
	}
		
	XMLMarkup.OutOfElem(); // DT:UpdateFileRequest

	return PRIMX_ERROR_SUCCESS;

}

BOOL CCommandUpdateFile::GetUpdateFileDownload(CString strURL, CString strSaveFilePath)
{
	BOOL bRe = FALSE;

	CHttpClient* objHttpReq = new CHttpClient;
	CHttpResponse* pobjHttpRes = NULL;

	HINTERNET hInternet = NULL;
	HINTERNET hConnection = NULL;

	try {
		pobjHttpRes = objHttpReq->RequestGet(strURL);

		DWORD dwContSize = 0;
		DWORD cbBuff = 1024 * 10;
		if (TRUE == pobjHttpRes->GetContentLength(dwContSize)) {

			DWORD nStatus = pobjHttpRes->GetStatus();
			if (200 != nStatus) {
				if (NULL != objHttpReq) {
					delete objHttpReq;
					objHttpReq = NULL;
				}
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Web File Download Fail > %s > %s"), strURL, pobjHttpRes->GetStatusText());
				if (NULL != pobjHttpRes) delete pobjHttpRes;
				return bRe;
			}

			if (0 < dwContSize) {
				if (1024 * 1024 * 5 < dwContSize) {
					cbBuff = 1024 * 1024;
				}
				CFile DownloadFile;
				DWORD dwDownloadFileSize = 0;

				if (TRUE == DownloadFile.Open(strSaveFilePath, CFile::modeCreate | CFile::modeWrite)) {
					BYTE* byBuff = new BYTE[cbBuff];
					DWORD dwRead = 0;

					while (dwRead = pobjHttpRes->ReadContent(byBuff, cbBuff)) {
						DownloadFile.Write(byBuff, dwRead);
						dwDownloadFileSize = dwDownloadFileSize + dwRead;
						ZeroMemory(byBuff, cbBuff);
						Sleep(0);
					}
					DownloadFile.Close();

					if (dwDownloadFileSize == dwContSize) {
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Web File Download OK > %s > %s"), strURL, strSaveFilePath);
						bRe = TRUE;
					}
					else {
						::DeleteFile(strSaveFilePath);
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Web File Download Fail > %s > 다운로드한 파일의 길이 오류"), strURL);
					}

					delete[] byBuff;
					byBuff = NULL;
				}
			}
			else {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Web File Download Fail > %s > 다운로드 할 파일이 길이가 0"), strURL);

			}
		}
	}
	catch (CHttpClient::Exception & e) {
		CString strErrorString;
		strErrorString.Format(_T("%s"), e.errmsg());
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Web File Download Fail > %s > %s"), strURL, strErrorString);

		if (NULL != pobjHttpRes) delete pobjHttpRes;
		if (NULL != objHttpReq) {
			objHttpReq->Cancel();
			delete objHttpReq;
			objHttpReq = NULL;
		}
		return bRe;
	}

	if (NULL != pobjHttpRes) delete pobjHttpRes;

	if (NULL != objHttpReq) {
		objHttpReq->Cancel();
		delete objHttpReq;
		objHttpReq = NULL;
	}

	return bRe;
}
