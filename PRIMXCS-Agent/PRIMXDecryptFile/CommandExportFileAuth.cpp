#include "stdafx.h"
#include "CommandExportFileAuth.h"


CCommandExportFileAuth::CCommandExportFileAuth()
{
}


CCommandExportFileAuth::~CCommandExportFileAuth()
{
}

UINT CCommandExportFileAuth::SetResponseDataTypeParser(CString& strXML)
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

UINT CCommandExportFileAuth::SetRequestDataTypeGenerate(CMarkup& XMLMarkup)
{
	UINT nRe = PRIMX_ERROR_XML_10001;
	
	XMLMarkup.AddElem(_T("DT:DecryptionAuthRequest"));
	XMLMarkup.IntoElem(); // DT:DecryptionAuthRequest

	XMLMarkup.AddElem(_T("FileKey"), m_strFileKey, CMarkup::MNF_WITHCDATA);
	XMLMarkup.AddElem(_T("CompanyID"), m_strCompanyID, CMarkup::MNF_WITHCDATA);
	XMLMarkup.AddElem(_T("FileHash"), m_strFileHash, CMarkup::MNF_WITHCDATA);
	XMLMarkup.AddElem(_T("IP"), m_strConnectIPAddr);
	XMLMarkup.AddElem(_T("MAC"), m_strConnectMAC);

	XMLMarkup.OutOfElem(); // DT:DecryptionAuthRequest
	
	return PRIMX_ERROR_SUCCESS;
}

BOOL CCommandExportFileAuth::SetExportFileAuth(CString strServerIP, unsigned short nPort, CString strFileKey, CString strCompanyID, CString strAgentID, CString strUserID,CString strFileHash)
{
	m_strAgentID = strAgentID;
	m_strCommnadCode = _T("ExDecryption");
	m_strSubCode = _T("Authentication");
	m_strFileKey = strFileKey;
	m_strCompanyID = strCompanyID;
	m_strFileHash = strFileHash;
	m_strUserID = strUserID;

	BOOL bRe = FALSE;

	bRe = SetCommandDataRequest(m_strCommnadCode, m_strSubCode, m_strAgentID, m_strUserID, _T(""));

	if (TRUE == bRe) {
		return TRUE;
	}

	return FALSE;
}
