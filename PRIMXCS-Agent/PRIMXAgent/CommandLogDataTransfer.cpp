#include "stdafx.h"
#include "CommandLogDataTransfer.h"
#include "ConfigDataManager.h"

CCommandLogDataTransfer::CCommandLogDataTransfer()
{
	m_pLogDataList = NULL;
}


CCommandLogDataTransfer::~CCommandLogDataTransfer()
{

}

BOOL CCommandLogDataTransfer::SetLogDataTransfer(CList<PRIMX_LOG_DATA*>* pLogDataList)
{
	CConfigDataManager ConfigDataManager;
	m_strAgentID = ConfigDataManager.GetAgentID();
	m_strAgentVersion = ConfigDataManager.GetAgentVersion();
	m_strUserID = ConfigDataManager.GetUserID();
	m_nSystemType = ConfigDataManager.GetSystemType();

	m_strCode = _T("LogData");
	m_strSubCode = _T("Insert");
	m_pLogDataList = pLogDataList;

	BOOL bRe = SetCommandDataRequest(m_strCode, m_strSubCode, m_strAgentID, m_strUserID, _T(""));
	return bRe;
}

UINT CCommandLogDataTransfer::SetResponseDataTypeParser(CString& strXML)
{



	return 1;
}

UINT CCommandLogDataTransfer::SetRequestDataTypeGenerate(CMarkup& XMLMarkup)
{
	UINT nRe = PRIMX_ERROR_XML_10001;
	
	UINT nLogCount = 0;
	if (NULL != m_pLogDataList) {
		nLogCount = UINT(m_pLogDataList->GetCount());
	}

	UINT nTransferLogCount = 0;
	XMLMarkup.AddElem(_T("DT:LogData"));
	XMLMarkup.AddAttrib(_T("LogCount"), nLogCount);
	XMLMarkup.IntoElem(); // DT:LogData

	if (NULL != m_pLogDataList) {
		POSITION Pos = m_pLogDataList->GetHeadPosition();
		while (NULL != Pos) {
			PRIMX_LOG_DATA* pLogData = m_pLogDataList->GetNext(Pos);
			if (NULL != pLogData) {
				nTransferLogCount++;
				XMLMarkup.AddElem(_T("LogData"));
				XMLMarkup.AddAttrib(_T("Index"), nTransferLogCount);
				XMLMarkup.IntoElem(); // LogData
				if (PRIMX_PRODUCT_TYPE_ZONECENTRAL == pLogData->nProductType) {
					XMLMarkup.AddElem(_T("ProductType"), _T("ZONECENTRAL"));
				}
				else if (PRIMX_PRODUCT_TYPE_ORIZON == pLogData->nProductType) {
					XMLMarkup.AddElem(_T("ProductType"), _T("ORIZON"));
				}
				else if (PRIMX_PRODUCT_TYPE_CRYHOD == pLogData->nProductType) {
					XMLMarkup.AddElem(_T("ProductType"), _T("CRYHOD"));
				}
				else {
					XMLMarkup.AddElem(_T("ProductType"), _T("UNKNOWN"));
				}
				XMLMarkup.AddElem(_T("LogType"), pLogData->strLogType);
				XMLMarkup.AddElem(_T("LogCode"), pLogData->strLogCode);
				XMLMarkup.AddElem(_T("LogDateTime"), pLogData->strLogDate);
				XMLMarkup.AddElem(_T("LogLevel"), pLogData->strLogLevel);
				XMLMarkup.AddElem(_T("LogDesc"), pLogData->strLogData, CMarkup::MNF_WITHCDATA);
				XMLMarkup.OutOfElem(); // LogData
			}
		}
	}
	XMLMarkup.OutOfElem(); // DT:LogData

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"로그 전송 개수 > %lu", nTransferLogCount);
	
	return PRIMX_ERROR_SUCCESS;
}
