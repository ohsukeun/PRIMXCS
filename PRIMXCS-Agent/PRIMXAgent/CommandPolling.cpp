#include "stdafx.h"
#include "CommandPolling.h"
#include "ConfigDataManager.h"
#include "SystemDataManager.h"



CCommandPolling::CCommandPolling()
{
	m_nKeepAliveCycle = 0;			
	m_nLogCycle = 0;				
	m_bAgentUpdate = FALSE;         
	m_bZonecentralUpdate = FALSE;   
	m_bOrizonUpdate = FALSE;        
	m_bCryhodUpdate = FALSE;        
	m_nAuthServerPort = 0; 
}

CCommandPolling::~CCommandPolling()
{
	m_AgentPolicyData.Clear();
}

UINT CCommandPolling::SetResponseDataTypeParser(CString& strXML)
{
	UINT nRe = PRIMX_ERROR_XML_10001;

	CMarkup XMLMarkup;

	if (TRUE == XMLMarkup.SetDoc(strXML)) {
		
		m_AgentPolicyData.Clear();

		if (true == XMLMarkup.FindElem()) {
			CString strRequestName = XMLMarkup.GetTagName();
			if (0 == strRequestName.CompareNoCase(_T("response"))) {
				XMLMarkup.IntoElem();
				while (true == XMLMarkup.FindElem()) {
					CString strELName = XMLMarkup.GetTagName();
					if (0 == strELName.CompareNoCase(_T("DT:KeepAliveResponse"))) {
						XMLMarkup.IntoElem();
						while (true == XMLMarkup.FindElem()) {
							CString strELNameReply = XMLMarkup.GetTagName();

							if (0 == strELNameReply.CompareNoCase(_T("ExpandCommand"))) {
								m_strExpandCommand = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("KeepAliveCycle"))) {
								m_nKeepAliveCycle = AtoUI(XMLMarkup.GetData());
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("LogCycle"))) {
								m_nLogCycle = AtoUI(XMLMarkup.GetData());
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("AgentUpdate"))) {
								m_bAgentUpdate = GetBool(XMLMarkup.GetData(),FALSE);
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("ZonecentralUpdate"))) {
								m_bZonecentralUpdate = GetBool(XMLMarkup.GetData(), FALSE);
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("OrizonUpdate"))) {
								m_bOrizonUpdate = GetBool(XMLMarkup.GetData(), FALSE);
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("CryhodUpdate"))) {
								m_bCryhodUpdate = GetBool(XMLMarkup.GetData(), FALSE);
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("CompanyID"))) {
								m_strCompanyID = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("AuthServerAddr"))) {
								m_strAuthServerAddr = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("AuthServerPort"))) {
								m_nAuthServerPort = AtoUI(XMLMarkup.GetData());
								nRe = PRIMX_ERROR_SUCCESS;
							}

						}
						XMLMarkup.OutOfElem();
					}
				
					if (0 == strELName.CompareNoCase(_T("DT:PolicyData"))) {

						XMLMarkup.IntoElem();
						while (true == XMLMarkup.FindElem()) {
							CString strELNameReply = XMLMarkup.GetTagName();

							if (0 == strELNameReply.CompareNoCase(_T("AuthWebManager"))) {
								m_AgentPolicyData.bAuthWebManager = GetBool(XMLMarkup.GetData(),FALSE);
								m_AgentPolicyData.strAuthWebManagerURL = XMLMarkup.GetAttrib(_T("URL"));
								m_AgentPolicyData.strAuthWebManagerURL.Trim();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("AuthExportFile"))) {
								m_AgentPolicyData.bAuthExportFile = GetBool(XMLMarkup.GetData(), FALSE);
								m_AgentPolicyData.nShelfLife = AtoUI(XMLMarkup.GetAttrib(_T("ShelfLife")));
								m_AgentPolicyData.strServerDateTime = XMLMarkup.GetAttrib(_T("ServerTime"));
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("Menu"))) {								
								XMLMarkup.IntoElem();
								while (true == XMLMarkup.FindElem()) {
									CString strELName = XMLMarkup.GetTagName();
									if (0 == strELName.CompareNoCase(_T("ProductMenu"))) {
										m_AgentPolicyData.bProductMenu = GetBool(XMLMarkup.GetData(), FALSE);
									}
									if (0 == strELName.CompareNoCase(_T("WebManagerConsoleMenu"))) {
										m_AgentPolicyData.bWebManagerConsoleMenu = GetBool(XMLMarkup.GetData(), FALSE);
									}
									if (0 == strELName.CompareNoCase(_T("ExportFileMenu"))) {
										m_AgentPolicyData.bExportFileMenu = GetBool(XMLMarkup.GetData(), FALSE);
									}
								}
								XMLMarkup.OutOfElem();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("AuthNotEncryption"))) {
								m_AgentPolicyData.bAuthNotEncryption = GetBool(XMLMarkup.GetAttrib(_T("Mode")),FALSE);
								XMLMarkup.IntoElem();
								while (true == XMLMarkup.FindElem()) {
									CString strELName = XMLMarkup.GetTagName();
									if (0 == strELName.CompareNoCase(_T("Path"))) {
										CString strPath = XMLMarkup.GetData();
										strPath.Trim();
										strPath.MakeLower();
										m_AgentPolicyData.AuthNotEncryptionPathList.AddTail(strPath);
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


#ifdef _DEBUG
	CString strResponseXMLPath;
	strResponseXMLPath.Format(_T("%s\\XML\\%s_%s_Response_%s.XML"), GetProgramDataPath(), m_strCode, m_strSubCode, GetTimeStringForFileName());
	CMarkup XMLMarkupLog;
	XMLMarkupLog.SetDoc(strXML);
	XMLMarkupLog.Save(strResponseXMLPath);
#endif

	if (TRUE == nRe) {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Agent 정책 > Web Manager 호출 권한 > %d > %s"), m_AgentPolicyData.bAuthWebManager, m_AgentPolicyData.strAuthWebManagerURL);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Agent 정책 > 이동파일 생성 권한 > %d > 유효기간 > %d"), m_AgentPolicyData.bAuthExportFile, m_AgentPolicyData.nShelfLife);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Agent 정책 > 서버시간 > %s"), m_AgentPolicyData.strServerDateTime);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Agent 정책 > PRIMX 제품 Menu 표시 > %d"), m_AgentPolicyData.bProductMenu);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Agent 정책 > Web Manager Console Menu 표시 > %d"), m_AgentPolicyData.bWebManagerConsoleMenu);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Agent 정책 > 이동파일 Menu 표시 > %d"), m_AgentPolicyData.bExportFileMenu);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Agent 정책 > 비암호화 영역 설정 > %d"), m_AgentPolicyData.bAuthNotEncryption);
		if (TRUE == m_AgentPolicyData.bAuthNotEncryption) {
			POSITION Pos = m_AgentPolicyData.AuthNotEncryptionPathList.GetHeadPosition();
			while (NULL != Pos) {
				CString strPath = m_AgentPolicyData.AuthNotEncryptionPathList.GetNext(Pos);
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Agent 정책 > 비암호화 경로 > %s"), strPath);
			}
		}
	}

	return nRe;
}

UINT CCommandPolling::SetRequestDataTypeGenerate(CMarkup& XMLMarkup)
{
	UINT nRe = PRIMX_ERROR_XML_10001;

	XMLMarkup.AddElem(_T("DT:ModuleState"));
	XMLMarkup.IntoElem(); // DT:ModuleState

	if (TRUE == m_ZonecentrolData.bInstall) {
		XMLMarkup.AddElem(_T("ZONECENTRAL"));
		XMLMarkup.IntoElem(); // ZONECENTRAL
		XMLMarkup.AddElem(_T("Install"), _T("Yes"));
		XMLMarkup.AddElem(_T("Version"), m_ZonecentrolData.strDisplayVersionEx);
		XMLMarkup.AddElem(_T("LastPolicyDate"), m_ZonecentrolData.strLastPolicyDate);
		XMLMarkup.AddElem(_T("LastPolicyID"), m_ZonecentrolData.strLastPolicyID);
		XMLMarkup.OutOfElem(); // ZONECENTRAL
	}
	else {
		XMLMarkup.AddElem(_T("ZONECENTRAL"));
		XMLMarkup.IntoElem(); // ZONECENTRAL
		XMLMarkup.AddElem(_T("Install"), _T("No"));
		XMLMarkup.OutOfElem(); // ZONECENTRAL
	}

	if (TRUE == m_OrizoneData.bInstall) {
		XMLMarkup.AddElem(_T("ORIZON"));
		XMLMarkup.IntoElem(); // ORIZON
		XMLMarkup.AddElem(_T("Install"), _T("Yes"));
		XMLMarkup.AddElem(_T("Version"), m_OrizoneData.strDisplayVersionEx);
		XMLMarkup.AddElem(_T("LastPolicyDate"), m_OrizoneData.strLastPolicyDate);
		XMLMarkup.AddElem(_T("LastPolicyID"), m_OrizoneData.strLastPolicyID);
		XMLMarkup.OutOfElem(); // ORIZON
	}
	else {
		XMLMarkup.AddElem(_T("ORIZON"));
		XMLMarkup.IntoElem(); // ORIZON
		XMLMarkup.AddElem(_T("Install"), _T("No"));
		XMLMarkup.OutOfElem(); // ORIZON
	}

	if (TRUE == m_CryhodData.bInstall) {
		XMLMarkup.AddElem(_T("CRYHOD"));
		XMLMarkup.IntoElem(); // CRYHOD
		XMLMarkup.AddElem(_T("Install"), _T("Yes"));
		XMLMarkup.AddElem(_T("Version"), m_CryhodData.strDisplayVersionEx);
		XMLMarkup.AddElem(_T("LastPolicyDate"), m_CryhodData.strLastPolicyDate);
		XMLMarkup.AddElem(_T("LastPolicyID"), m_CryhodData.strLastPolicyID);
		XMLMarkup.OutOfElem(); // CRYHOD
	}
	else {
		XMLMarkup.AddElem(_T("CRYHOD"));
		XMLMarkup.IntoElem(); // CRYHOD
		XMLMarkup.AddElem(_T("Install"), _T("No"));
		XMLMarkup.OutOfElem(); // CRYHOD
	}

	if (PRIMX_SYS_TYPE_COMM == m_nSystemType || PRIMX_SYS_TYPE_COMM_SETUP == m_nSystemType) {
		XMLMarkup.AddElem(_T("SharePC"), _T("Yes"));
	}
	else {
		XMLMarkup.AddElem(_T("SharePC"), _T("No"));
	}

	XMLMarkup.OutOfElem(); // DT:ModuleState
	

	return PRIMX_ERROR_SUCCESS;
}

BOOL CCommandPolling::SetShutdown()
{
	CConfigDataManager ConfigDataManager;
	m_strAgentID = ConfigDataManager.GetAgentID();
	m_strAgentVersion = ConfigDataManager.GetAgentVersion();
	m_strUserID = ConfigDataManager.GetUserID();
	m_nSystemType = ConfigDataManager.GetSystemType();
	m_strCode = _T("KAData");
	m_strSubCode = _T("Shutdown");

	BOOL bRe = FALSE;
	CSystemDataManager SystemDataManager;
	if (TRUE == SystemDataManager.GetPRIMXProductInfo(m_ZonecentrolData, m_OrizoneData, m_CryhodData)) {
		bRe = SetCommandDataRequest(m_strCode, m_strSubCode, m_strAgentID, m_strUserID, _T(""));
	}
	Close();
	return bRe;
}

BOOL CCommandPolling::SetUninstall()
{
	CConfigDataManager ConfigDataManager;
	m_strAgentID = ConfigDataManager.GetAgentID();
	m_strAgentVersion = ConfigDataManager.GetAgentVersion();
	m_strUserID = ConfigDataManager.GetUserID();
	m_nSystemType = ConfigDataManager.GetSystemType();
	m_strCode = _T("KAData");
	m_strSubCode = _T("Uninstall");

	BOOL bRe = FALSE;
	CSystemDataManager SystemDataManager;
	if (TRUE == SystemDataManager.GetPRIMXProductInfo(m_ZonecentrolData, m_OrizoneData, m_CryhodData)) {
		bRe = SetCommandDataRequest(m_strCode, m_strSubCode, m_strAgentID, m_strUserID, _T(""));
	}
	Close();

	return bRe;
}

BOOL CCommandPolling::SetPolling()
{
	CConfigDataManager ConfigDataManager;
	m_strAgentID = ConfigDataManager.GetAgentID();
	m_strAgentVersion = ConfigDataManager.GetAgentVersion();
	m_strUserID = ConfigDataManager.GetUserID();
	m_nSystemType = ConfigDataManager.GetSystemType();
	m_strCode = _T("KAData");
	m_strSubCode = _T("Update");

	BOOL bRe = FALSE;
	CSystemDataManager SystemDataManager;
	if (TRUE == SystemDataManager.GetPRIMXProductInfo(m_ZonecentrolData, m_OrizoneData, m_CryhodData)) {
		bRe = SetCommandDataRequest(m_strCode, m_strSubCode, m_strAgentID, m_strUserID, _T(""));
	}
	
	Close();	
	return bRe;
}

CString CCommandPolling::GetExpandCommand()
{
	m_strExpandCommand.Trim();
	return m_strExpandCommand;
}

UINT CCommandPolling::GetKeepAliveCycle()
{
	return m_nKeepAliveCycle;
}

UINT CCommandPolling::GetLogCycle()
{
	return m_nLogCycle;
}

BOOL CCommandPolling::GetAgentUpdate()
{
	return m_bAgentUpdate;
}

BOOL CCommandPolling::GetZonecentralUpdate()
{
	return m_bZonecentralUpdate;
}

BOOL CCommandPolling::GetOrizonUpdate()
{
	return m_bOrizonUpdate;
}

BOOL CCommandPolling::GetCryhodUpdate()
{
	return m_bCryhodUpdate;
}


CString CCommandPolling::GetCompanyID()
{
	m_strCompanyID.Trim();
	return m_strCompanyID;
}

CString CCommandPolling::GetAuthServerAddr()
{
	m_strAuthServerAddr.Trim();
	return m_strAuthServerAddr;
}

UINT CCommandPolling::GetAuthServerPort()
{
	return m_nAuthServerPort;
}

BOOL CCommandPolling::GetPolicyData(PRIMX_POLICY_DATA& PolicyData)
{	
	PolicyData = m_AgentPolicyData;
	return TRUE;
}
