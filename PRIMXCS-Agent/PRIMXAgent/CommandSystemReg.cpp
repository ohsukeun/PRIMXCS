#include "stdafx.h"
#include "CommandSystemReg.h"
#include "ConfigDataManager.h"

CCommandSystemReg::CCommandSystemReg()
{
	m_nModifyFlag = UINT_MAX;
}

CCommandSystemReg::~CCommandSystemReg()
{

}

BOOL CCommandSystemReg::SetSystemDataReg(CString strUserID,UINT nSystemType, PRIMX_SYSTEM_DATA& SystemData)
{
	CConfigDataManager ConfigDataManager;
	m_strAgentID = ConfigDataManager.GetAgentID();
	m_strAgentVersion = ConfigDataManager.GetAgentVersion();
	m_strUserID = strUserID;
	m_nSystemType = nSystemType;
	m_strCode = _T("SystemData");
	m_strSubCode = _T("Insert");
	m_nModifyFlag = UINT_MAX;

	m_SystemData = SystemData;
	BOOL bRe = SetCommandDataRequest(m_strCode, m_strSubCode, m_strAgentID, m_strUserID, _T(""));
	return bRe;
}

BOOL CCommandSystemReg::SetSystemDataModify(PRIMX_SYSTEM_DATA& SystemData, UINT nModifyFlag)
{
	CConfigDataManager ConfigDataManager;
	m_strAgentID = ConfigDataManager.GetAgentID();
	m_strAgentVersion = ConfigDataManager.GetAgentVersion();
	m_strUserID = ConfigDataManager.GetUserID();
	m_nSystemType = ConfigDataManager.GetSystemType();
	m_strCode = _T("SystemData");
	m_strSubCode = _T("Update");
	m_nModifyFlag = nModifyFlag;

	m_SystemData = SystemData;
	BOOL bRe = SetCommandDataRequest(m_strCode, m_strSubCode, m_strAgentID, m_strUserID, _T(""));
	return bRe;
}

UINT CCommandSystemReg::SetResponseDataTypeParser(CString& strXML)
{

#ifdef _DEBUG
	CString strResponseXMLPath;
	strResponseXMLPath.Format(_T("%s\\XML\\%s_%s_Response_%s.XML"), GetLocalPath(), m_strCode, m_strSubCode, GetTimeStringForFileName());
	CMarkup XMLMarkup;
	XMLMarkup.SetDoc(strXML);
	XMLMarkup.Save(strResponseXMLPath);
#endif
	
	return PRIMX_ERROR_SUCCESS;
}

UINT CCommandSystemReg::SetRequestDataTypeGenerate(CMarkup& XMLMarkup)
{
	UINT nRe = PRIMX_ERROR_XML_10001;

	XMLMarkup.AddElem(_T("DT:SystemData"));
	XMLMarkup.IntoElem(); // DT:SystemData

	if (PRIMX_SYS_MOD_WIN_VERSION & m_nModifyFlag) {
		XMLMarkup.AddElem(_T("WinVersion"));
		XMLMarkup.IntoElem(); // WinVersion
		XMLMarkup.AddElem(_T("Name"), m_SystemData.WindowsVersion.strVersionFullName, CMarkup::MNF_WITHCDATA);
		XMLMarkup.AddElem(_T("UUID"), m_SystemData.WindowsVersion.strUUID);
		XMLMarkup.AddElem(_T("MajorVersion"), m_SystemData.WindowsVersion.nMajorVersion);
		XMLMarkup.AddElem(_T("MinorVersion"), m_SystemData.WindowsVersion.nMinorVersion);
		XMLMarkup.AddElem(_T("BuildNumber"), m_SystemData.WindowsVersion.strBuildNumber);
		XMLMarkup.AddElem(_T("ServicePackMajor"), m_SystemData.WindowsVersion.nServicePackMajor);
		XMLMarkup.AddElem(_T("ServicePackMinor"), m_SystemData.WindowsVersion.nServicePackMinor);
		XMLMarkup.AddElem(_T("ProcessorArchitecture"), m_SystemData.WindowsVersion.nProcessorArchitecture);
		XMLMarkup.AddElem(_T("ReleaseID"), m_SystemData.WindowsVersion.strReleaseID);
		XMLMarkup.OutOfElem(); // WinVersion
	}

	if (PRIMX_SYS_MOD_DISK & m_nModifyFlag) {
		if (0 < m_SystemData.DiskDriveList.GetCount()) {
			XMLMarkup.AddElem(_T("DiskDrives"));
			XMLMarkup.IntoElem();
			POSITION Pos = m_SystemData.DiskDriveList.GetHeadPosition();
			while (NULL != Pos) {
				PRIMX_DISK_DRIVE_DATA* pData = m_SystemData.DiskDriveList.GetNext(Pos);
				if (NULL != pData) {
					XMLMarkup.AddElem(_T("DiskDrive"));
					XMLMarkup.IntoElem();
					XMLMarkup.AddElem(_T("Name"), pData->strName);
					XMLMarkup.AddElem(_T("Type"), pData->nType);
					XMLMarkup.AddElem(_T("VolumeName"), pData->strVolumeName);
					XMLMarkup.AddElem(_T("FileSystemName"), pData->strFileSystemName);
					XMLMarkup.AddElem(_T("TotalSize"), pData->nTotalSize);
					XMLMarkup.AddElem(_T("FreeSize"), pData->nFreeSize);
					XMLMarkup.OutOfElem();
				}
			}
			XMLMarkup.OutOfElem();
		}
	}
	if (PRIMX_SYS_MOD_NIC & m_nModifyFlag) {
		if (0 < m_SystemData.NICList.GetCount()) {
			XMLMarkup.AddElem(_T("NICs"));
			XMLMarkup.IntoElem();
			POSITION Pos = m_SystemData.NICList.GetHeadPosition();
			while (NULL != Pos) {
				PRIMX_NIC_DATA* pData = m_SystemData.NICList.GetNext(Pos);
				if (NULL != pData) {
					XMLMarkup.AddElem(_T("NIC"));
					XMLMarkup.AddAttrib(_T("Name"), pData->strDescription);
					XMLMarkup.AddAttrib(_T("MAC"), pData->strMAC);
					XMLMarkup.AddAttrib(_T("DHCP"), (0 == pData->strDHCPServer.GetLength()) ? _T("No") : _T("Yes"));
					XMLMarkup.IntoElem();
					XMLMarkup.AddElem(_T("IP"), pData->strIP);
					XMLMarkup.AddElem(_T("GW"), pData->strGW);
					XMLMarkup.AddElem(_T("DNS"), pData->strDNS);
					XMLMarkup.AddElem(_T("DHCPServer"), pData->strDHCPServer);
					XMLMarkup.OutOfElem();
				}
			}
			XMLMarkup.OutOfElem();
		}
	}
	if (PRIMX_SYS_NETBIOS & m_nModifyFlag) {
		XMLMarkup.AddElem(_T("NetBios"));
		XMLMarkup.IntoElem(); // NetBios
		XMLMarkup.AddElem(_T("ComputerName"), m_SystemData.NetBiosData.strComputerName, CMarkup::MNF_WITHCDATA);
		XMLMarkup.AddElem(_T("WorkGroupName"), m_SystemData.NetBiosData.strWorkGroupName, CMarkup::MNF_WITHCDATA);
		XMLMarkup.OutOfElem(); // NetBios		
	}

	if (PRIMX_SYS_TYPE_COMM == m_nSystemType || PRIMX_SYS_TYPE_COMM_SETUP == m_nSystemType) {
		XMLMarkup.AddElem(_T("SharePC"), _T("Yes"));
	}
	else{
		XMLMarkup.AddElem(_T("SharePC"), _T("No"));
	}

	XMLMarkup.AddElem(_T("AgentVersion"), m_strAgentVersion);

	XMLMarkup.OutOfElem(); // DT:SystemData
	
	return PRIMX_ERROR_SUCCESS;

}