#include "stdafx.h"
#include "SystemDataModifyCheck.h"
#include "FileDBManager.h"
#include "SystemDataManager.h"
#include "CommandSystemReg.h"

CSystemDataModifyCheck::CSystemDataModifyCheck()
{

}

CSystemDataModifyCheck::~CSystemDataModifyCheck()
{

}

void CSystemDataModifyCheck::GetSystemData()
{
	CFileDBManager FileDBManager;
	m_SystemLastData.Clear();
	FileDBManager.GetSystemData(m_ConfigDataManager.GetAgentID(), m_SystemLastData);
}

void CSystemDataModifyCheck::SetSystemDataModifyCheck()
{
	PRIMX_SYSTEM_DATA CurrentSystemData;
	CSystemDataManager SystemDataManager;
	if (TRUE == SystemDataManager.GetSystemDataCollection(CurrentSystemData)) {

		UINT nModResult = SetSystemDataModifyCheck(m_SystemLastData, CurrentSystemData);
		if (0 != nModResult) {

			CCommandSystemReg CommandSystemModify;
			CStringList ServerAddrList;
			int nAddrCount = m_ConfigDataManager.GetServerIPEx(ServerAddrList);
			unsigned short nServerPort = m_ConfigDataManager.GetServerPort();

			if (0 < nAddrCount) {
				POSITION Pos = ServerAddrList.GetHeadPosition();
				while (NULL != Pos) {
					CString strServerIP = ServerAddrList.GetNext(Pos);
					if (TRUE == CommandSystemModify.SetConnect(strServerIP, nServerPort)) {
						if (TRUE == CommandSystemModify.SetSystemDataModify(CurrentSystemData, nModResult)) {
							m_SystemLastData = CurrentSystemData;
							CFileDBManager CFileDBManager;
							CFileDBManager.SetSystemDataDBSave(m_ConfigDataManager.GetAgentID(), m_ConfigDataManager.GetUserID(), m_ConfigDataManager.GetSystemType(), m_SystemLastData, TRUE);
							theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"시스템 정보 변경 > 성공 > %s:%lu > %lu", strServerIP, nServerPort, nModResult);
							break;
						}
						else {
							CString strFailCode = CommandSystemModify.GetFailCode();
							CString strFailString = CommandSystemModify.GetFailString();
							theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"시스템 정보 변경 > 실패 > %s:%lu[%s] %s", strServerIP, nServerPort, strFailCode, strFailString);
						}
					}
					Sleep(500);
				}
				ServerAddrList.RemoveAll();
			}
			else{
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"시스템 정보 변경 > 실패 > 서버 주소 없음");
			}	
			
		}
	}
}

UINT CSystemDataModifyCheck::SetSystemDataModifyCheck(PRIMX_SYSTEM_DATA& LastSystemData, PRIMX_SYSTEM_DATA& CurrentSystemData)
{
	UINT nMod = 0;

	if (LastSystemData.nSharePC != CurrentSystemData.nSharePC) {
		nMod = nMod | PRIMX_SYS_SHARE_PC;
	}

	if (FALSE == LastSystemData.NetBiosData.Compare(CurrentSystemData.NetBiosData)) {
		nMod = nMod | PRIMX_SYS_NETBIOS;
	}

	if (LastSystemData.NICList.GetCount() != CurrentSystemData.NICList.GetCount()) {
		nMod = nMod | PRIMX_SYS_MOD_NIC;
	}
	else {
		POSITION Pos = LastSystemData.NICList.GetHeadPosition();
		while (NULL != Pos) {
			PRIMX_NIC_DATA* pData = LastSystemData.NICList.GetNext(Pos);
			if (NULL != pData) {
				BOOL bExist = FALSE;
				POSITION CurrentPos = CurrentSystemData.NICList.GetHeadPosition();
				while (NULL != CurrentPos) {
					PRIMX_NIC_DATA* pCurrentData = CurrentSystemData.NICList.GetNext(CurrentPos);
					if (NULL != pCurrentData) {
						if (0 == pData->strAdapterGUID.CompareNoCase(pCurrentData->strAdapterGUID)) {
							if (FALSE == pData->Compare(pCurrentData)) {
								nMod = nMod | PRIMX_SYS_MOD_NIC;
							}
							bExist = TRUE;
							break;
						}
					}
				}
				if (FALSE == bExist) {
					nMod = nMod | PRIMX_SYS_MOD_NIC;
					break;
				}
			}
		}
	}

	if (LastSystemData.DiskDriveList.GetCount() != CurrentSystemData.DiskDriveList.GetCount()) {
		nMod = nMod | PRIMX_SYS_MOD_DISK;
	}
	else {
		POSITION Pos = LastSystemData.DiskDriveList.GetHeadPosition();
		while (NULL != Pos) {
			PRIMX_DISK_DRIVE_DATA* pData = LastSystemData.DiskDriveList.GetNext(Pos);
			if (NULL != pData) {
				BOOL bExist = FALSE;
				POSITION CurrentPos = CurrentSystemData.DiskDriveList.GetHeadPosition();
				while (NULL != CurrentPos) {
					PRIMX_DISK_DRIVE_DATA* pCurrentData = CurrentSystemData.DiskDriveList.GetNext(CurrentPos);
					if (NULL != pCurrentData) {
						if (0 == pData->strName.CompareNoCase(pCurrentData->strName)) {
							if (FALSE == pData->Compare(pCurrentData)) {
								nMod = nMod | PRIMX_SYS_MOD_DISK;
							}
							bExist = TRUE;
							break;
						}
					}
				}
				if (FALSE == bExist) {
					nMod = nMod | PRIMX_SYS_MOD_DISK;
					break;
				}
			}
		}
	}

	if (FALSE == LastSystemData.WindowsVersion.Compare(CurrentSystemData.WindowsVersion)) {
		nMod = nMod | PRIMX_SYS_MOD_WIN_VERSION;
	}

	return nMod;
}

