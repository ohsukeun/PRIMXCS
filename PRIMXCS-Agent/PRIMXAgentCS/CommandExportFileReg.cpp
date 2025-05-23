#include "stdafx.h"
#include "CommandExportFileReg.h"
#include "ConfigDataManager.h"
#include "SHA512.h"

CCommandExportFileReg::CCommandExportFileReg()
{
	m_pOrgFileList = NULL;

}

CCommandExportFileReg::~CCommandExportFileReg()
{

}
UINT CCommandExportFileReg::SetResponseDataTypeParser(CString& strXML)
{
#ifdef _DEBUG
	CString strResponseXMLPath;
	strResponseXMLPath.Format(_T("%s\\XML\\%s_%s_Response_%s.XML"), GetLocalPath(), m_strCommnadCode, m_strSubCode, GetTimeStringForFileName());
	CMarkup XMLMarkup;
	XMLMarkup.SetDoc(strXML);
	XMLMarkup.Save(strResponseXMLPath);
#endif

	return PRIMX_ERROR_SUCCESS;
}

UINT CCommandExportFileReg::SetRequestDataTypeGenerate(CMarkup& XMLMarkup)
{
	UINT nRe = PRIMX_ERROR_XML_10001;
		
	CSHA512 SHA512;

	XMLMarkup.AddElem(_T("DT:ExportFile"));
	XMLMarkup.IntoElem(); // DT:ExportFile

	XMLMarkup.AddElem(_T("FileKey"), m_strFileKey, CMarkup::MNF_WITHCDATA);
	XMLMarkup.AddElem(_T("AgentID"), m_strAgentID, CMarkup::MNF_WITHCDATA);
	XMLMarkup.AddElem(_T("UserID"), m_strUserID, CMarkup::MNF_WITHCDATA);


	if (NULL != m_pOrgFileList && 0 < m_pOrgFileList->GetCount()) {
		XMLMarkup.AddElem(_T("OrgFiles"));
		XMLMarkup.IntoElem(); // OrgFiles

		POSITION Pos = m_pOrgFileList->GetHeadPosition();
		while (NULL != Pos) {
			PRIMX_ORG_FILE_INFO* pFileData = m_pOrgFileList->GetNext(Pos);
			if (NULL != pFileData) {
				XMLMarkup.AddElem(_T("FileName"), SetSplitPathFileName(pFileData->strFilePath), CMarkup::MNF_WITHCDATA);
				CString strTemp;
				strTemp.Format(_T("%I64u"), pFileData->nFileSize);
				XMLMarkup.AddAttrib(_T("Size"), strTemp);
			}
		}

		XMLMarkup.OutOfElem(); // OrgFiles

	}
	XMLMarkup.AddElem(_T("CreateDate") , GetDateTimeForGMT());
	XMLMarkup.AddElem(_T("FileHash"), SHA512.GetShaBase64Data(m_strExportFilePath),  CMarkup::MNF_WITHCDATA);
	XMLMarkup.AddElem(_T("FileSize"), m_strExportFileSize);
	XMLMarkup.AddElem(_T("FileName"), m_strExportFileName);

	XMLMarkup.OutOfElem(); // DT:ExportFile

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"이동 파일 등록 > %s", m_strFileKey);

	return PRIMX_ERROR_SUCCESS;
}

BOOL CCommandExportFileReg::SetExportFileReg(CString strFileKey, CString strExportFilePath, LONGLONG nExportFileSize, CList<PRIMX_ORG_FILE_INFO*>* pOrgFileList)
{
	CConfigDataManager ConfigDataManager;
	m_strAgentID = ConfigDataManager.GetAgentID();
	m_strAgentVersion = ConfigDataManager.GetAgentVersion();
	m_strUserID = ConfigDataManager.GetUserID();
	m_nSystemType = ConfigDataManager.GetSystemType();
	m_strCommnadCode = _T("ExDecryption");
	m_strSubCode = _T("Insert");

	m_strFileKey = strFileKey;
	m_strExportFilePath = strExportFilePath;
	m_strExportFileName = SetSplitPathFileName(m_strExportFilePath);
	m_pOrgFileList = pOrgFileList;
	m_strExportFileSize.Format(_T("%I64u"),nExportFileSize);

	BOOL bRe = FALSE;
	
	bRe = SetCommandDataRequest(m_strCommnadCode, m_strSubCode, m_strAgentID, m_strUserID, _T(""));
	
	if (TRUE == bRe) {
		return TRUE;
	}

	return FALSE;
}
