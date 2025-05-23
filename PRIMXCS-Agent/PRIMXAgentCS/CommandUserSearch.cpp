#include "stdafx.h"
#include "CommandUserSearch.h"
#include "ConfigDataManager.h"

CCommandUserSearch::CCommandUserSearch()
{
	m_nServerPort = 0;
}

CCommandUserSearch::~CCommandUserSearch()
{
	m_ServerAddrList.RemoveAll();
}

BOOL CCommandUserSearch::SetConnect()
{
	BOOL bRe = FALSE;

	CConfigDataManager ConfigDataManager;
	int nAddrCount = ConfigDataManager.GetServerIPEx(m_ServerAddrList);
	m_nServerPort = ConfigDataManager.GetServerPort();
		
	if (0 == nAddrCount) {
		CString strConfigPath;
		strConfigPath.Format(_T("%s\\%s"), LPCTSTR(GetLocalPath()), PRIMX_CONFIG_FILE_PATH);

		DWORD dwAttrs = GetFileAttributes(strConfigPath);
		if (dwAttrs != INVALID_FILE_ATTRIBUTES) {
			if (dwAttrs & FILE_ATTRIBUTE_READONLY) {
				SetFileAttributes(strConfigPath, dwAttrs & ~FILE_ATTRIBUTE_READONLY);
			}
		}
		CString strServerIP = GetProfileStringEx(PRIMX_CONFIG_SERVICE_SESSION, PRIMX_CONFIG_SERVER_ADDR, _T(""), strConfigPath);
		m_nServerPort = unsigned short(GetProfileIntEx(PRIMX_CONFIG_SERVICE_SESSION, PRIMX_CONFIG_SERVER_PORT, 50101, strConfigPath));
		unsigned short nUpdatePort = unsigned short(GetProfileIntEx(PRIMX_CONFIG_SERVICE_SESSION, PRIMX_CONFIG_UPDATE_PORT, 50102, strConfigPath));
		unsigned short nAuthPort = unsigned short(GetProfileIntEx(PRIMX_CONFIG_SERVICE_SESSION, PRIMX_CONFIG_AUTH_PORT, 50103, strConfigPath));

		ConfigDataManager.SetServerIP(strServerIP);
		ConfigDataManager.SetServerPort(m_nServerPort);
		ConfigDataManager.SetUpdatePort(nUpdatePort);
		ConfigDataManager.SetAuthPort(nAuthPort);

		nAddrCount = ConfigDataManager.GetServerIPEx(m_ServerAddrList);
		m_nServerPort = ConfigDataManager.GetServerPort();
	}
		
	CWaitCursor WaitCursor;

	POSITION Pos = m_ServerAddrList.GetHeadPosition();
	while (NULL != Pos) {
		CString strServerIP = m_ServerAddrList.GetNext(Pos);
		if (TRUE == CCommandManagerEx::SetConnect(strServerIP, m_nServerPort)) {
			bRe = TRUE;
			break;
		}
		Sleep(500);
	}

	return bRe;
}


BOOL CCommandUserSearch::SetUserDataSearch(CString strUserID)
{
	CWaitCursor WaitCursor;
	m_strSearchUserID = strUserID;
	BOOL bRe = SetCommandDataRequest(_T("UserData"), _T("Search"), _T(""), strUserID,_T(""));
	return TRUE;
}

UINT CCommandUserSearch::SetResponseDataTypeParser(CString& strXML)
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
					if (0 == strELName.CompareNoCase(_T("DT:UserData"))) {
						XMLMarkup.IntoElem();
						while (true == XMLMarkup.FindElem()) {
							CString strELNameReply = XMLMarkup.GetTagName();
							if (0 == strELNameReply.CompareNoCase(_T("UserID"))) {
								m_strUserID = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("ID"))) {
								if (0 == m_strUserID.GetLength()) {
									m_strUserID = XMLMarkup.GetData();
								}
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("Name"))) {
								m_strUserName = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("Tel"))) {
								m_strTel = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("CellPhone"))) {
								m_strCellPhone = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("ExtNo"))) {
								m_strExtNo = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("Mail"))) {
								m_strMail = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("Department"))) {
								m_strDepartment = XMLMarkup.GetData();
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

	return nRe;
}

CString CCommandUserSearch::GetUserID()
{
	return m_strUserID;
}

CString CCommandUserSearch::GetUserName()
{
	return m_strUserName;
}

CString CCommandUserSearch::GetTel()
{
	return m_strTel;
}

CString CCommandUserSearch::GetCellPhone()
{
	return m_strCellPhone;
}

CString CCommandUserSearch::GetExtNo()
{
	return m_strExtNo;
}

CString CCommandUserSearch::GetMail()
{
	return m_strMail;
}

CString CCommandUserSearch::GetDepartment()
{
	return m_strDepartment;
}

BOOL CCommandUserSearch::GetUserData(PRIMX_USER_DATA& UserData)
{
	BOOL bRe = FALSE;

	UserData.strUserID = m_strUserID;
	UserData.strUserName = m_strUserName;
	UserData.strTel = m_strTel;
	UserData.strCellPhone = m_strCellPhone;
	UserData.strExtNo = m_strExtNo;
	UserData.strMail = m_strMail;
	UserData.strDepartment = m_strDepartment;

	return bRe;
}