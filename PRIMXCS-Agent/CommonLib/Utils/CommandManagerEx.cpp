#include "stdafx.h"
#include "CommandManagerEx.h"
#include "lz4.h"
#include "Base64.h"
#include "ConfigDataManager.h"
#include <iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")


CCommandManagerEx::CCommandManagerEx()
{
	m_nCompressedType = PRIMX_CONTENTS_TYPE_COMPRESSED;
	m_nSystemType = PRIMX_SYS_TYPE_USER;
	m_nResult = 0;



}

CCommandManagerEx::~CCommandManagerEx()
{
	Close();
}

BOOL CCommandManagerEx::SetConnect(CString strIP, unsigned short nPort, BOOL bSSL)
{
	BOOL bRe = FALSE;

	bRe = Connect(strIP, nPort, bSSL);
	if (TRUE == bRe) {
		m_strConnectIPAddr = IPConvertString(m_nLocalIP);
		m_strConnectIPAddr.Trim();
		if (0 < m_strConnectIPAddr.GetLength()) {
			m_strConnectMAC = GetMACAddr(m_strConnectIPAddr);
		}
	}

	return bRe;
}

BOOL CCommandManagerEx::SetConnect(BOOL bSSL)
{
	BOOL bRe = FALSE;
	CStringList	ServerAddrList;
	CConfigDataManager ConfigDataManager;
	int nAddrCount = ConfigDataManager.GetServerIPEx(ServerAddrList);
	unsigned short nPort = ConfigDataManager.GetServerPort();
	
	POSITION Pos = ServerAddrList.GetHeadPosition();
	while (NULL != Pos) {
		CString strIP = ServerAddrList.GetNext(Pos);
		bRe = Connect(strIP, nPort, bSSL);
		if (FALSE == bRe) {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"로그 전송 > 서버 연결 실패 > %s:%lu", strIP, nPort);
		}
		else{
			m_strConnectIPAddr = IPConvertString(m_nLocalIP);
			m_strConnectIPAddr.Trim();
			if (0 < m_strConnectIPAddr.GetLength()) {
				m_strConnectMAC = GetMACAddr(m_strConnectIPAddr);
			}
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"로그 전송 > 서버 연결 성공 > %s:%lu", strIP, nPort);
			break;
		}
		Sleep(500);
	}

	ServerAddrList.RemoveAll();

	return bRe;
}

UINT CCommandManagerEx::SetResponseDataTypeParser(CString& strXML)
{
	return PRIMX_ERROR_SUCCESS;
}

UINT CCommandManagerEx::SetRequestDataTypeGenerate(CMarkup& XMLMarkup)
{

	return PRIMX_ERROR_SUCCESS;
}

CString CCommandManagerEx::GetAgentID()
{
	CString strAgentID;

	CConfigDataManager ConfigDataManager;
	strAgentID = ConfigDataManager.GetAgentID();

	return strAgentID;
}

CString CCommandManagerEx::GetRequestCommandXMLData(CString strCode,CString strSubCode, CString strAgentID, CString strUserID)
{
	CMarkup XMLMarkup;

	strCode.Trim();
	strSubCode.Trim();
	strAgentID.Trim();
	strUserID.Trim();

	XMLMarkup.SetDoc(_T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"));
	XMLMarkup.IntoElem();
	XMLMarkup.AddElem(_T("request"));
	XMLMarkup.IntoElem();
	XMLMarkup.AddElem(_T("command"));
	XMLMarkup.IntoElem();
	XMLMarkup.AddElem(_T("code"), strCode);
	XMLMarkup.AddElem(_T("subcode"), strSubCode);
	XMLMarkup.AddElem(_T("AgentID"), strAgentID);
	XMLMarkup.AddElem(_T("AgentVersion"), m_strAgentVersion);
	if (0 < m_strCompanyID.GetLength()) XMLMarkup.AddElem(_T("CompanyID"), m_strCompanyID);

	if (0 == strCode.CompareNoCase(_T("KAData"))) {
		XMLMarkup.AddElem(_T("AgentPolicyStamp"), m_strAgentPolicyStamp);
	}
	
	if(0 < strUserID.GetLength()) XMLMarkup.AddElem(_T("UserID"), strUserID, CMarkup::MNF_WITHCDATA);
	XMLMarkup.AddElem(_T("DateTime"), GetDateTimeForGMT());
	
	XMLMarkup.OutOfElem();
	SetRequestDataTypeGenerate(XMLMarkup);
	XMLMarkup.OutOfElem();
	XMLMarkup.OutOfElem();
	

#ifdef _DEBUG
	if (0 != strCode.CompareNoCase(L"KAData")) 
	{
		CString strRequestXMLPath;
		strRequestXMLPath.Format(_T("%s\\XML\\%s_%s_Request_%s.xml"), GetProgramDataPath(), strCode, strSubCode, GetTimeStringForFileName());
		XMLMarkup.Save(strRequestXMLPath); 
	}
#endif

	///////////////////////////////////////////

	return XMLMarkup.GetDoc();
}

BOOL CCommandManagerEx::GetRequestDataBuffer(CString strRequestXML,char*& pRequestData, UINT& nRequestDataLen)
{
	BOOL bRe = FALSE;

	int nXMLDataUtf8Len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)strRequestXML, -1, NULL, 0, NULL, NULL);

	if (0 < nXMLDataUtf8Len) {
		nRequestDataLen = nXMLDataUtf8Len + PRIMX_CMD_HD_LEN;
		pRequestData = new char[nRequestDataLen];
		ZeroMemory(pRequestData, nRequestDataLen);
		WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)strRequestXML, -1, (pRequestData + PRIMX_CMD_HD_LEN), nXMLDataUtf8Len, NULL, NULL);
	}

	if (PRIMX_CONTENTS_TYPE_PLAIN_TEXT == m_nCompressedType) {
		PRIMX_CMD_HD* pHeader = (PRIMX_CMD_HD*)pRequestData;
		pHeader->Init();
		pHeader->CmdTime = GetGMTime();
		pHeader->ContentsType = PRIMX_CONTENTS_TYPE_PLAIN_TEXT;
		pHeader->BodyLen = nXMLDataUtf8Len;
		bRe = TRUE;
	}
	else {
		UINT nDecompressSize = nXMLDataUtf8Len;
		if (FALSE == CompressedData(pRequestData, nXMLDataUtf8Len)) {
			_SafeDeleteArray(pRequestData);
			nRequestDataLen = 0;
			return bRe;
		}		

		CBase64 Base64;
		int nBase64EncodeLen = Base64.EncodeGetRequiredLength(nXMLDataUtf8Len);
		char* pBase64Data = new char[nBase64EncodeLen + PRIMX_CMD_HD_LEN + 1];
		ZeroMemory(pBase64Data, nBase64EncodeLen + PRIMX_CMD_HD_LEN + 1);

		if (TRUE == Base64.Encode((unsigned char*)pRequestData + PRIMX_CMD_HD_LEN, nXMLDataUtf8Len, pBase64Data + PRIMX_CMD_HD_LEN, &nBase64EncodeLen)) {
			_SafeDeleteArray(pRequestData);
			pRequestData = (char*)pBase64Data;
			nXMLDataUtf8Len = int(nBase64EncodeLen);
			nRequestDataLen = nXMLDataUtf8Len + PRIMX_CMD_HD_LEN;
			
			PRIMX_CMD_HD* pHeader = (PRIMX_CMD_HD*)pRequestData;
			pHeader->Init();
			pHeader->CmdTime = GetGMTime();
			pHeader->BodyLen = nXMLDataUtf8Len;
			pHeader->ContentsType = m_nCompressedType;
			pHeader->DecompressSize = nDecompressSize;			
			if (0 < m_strAgentID.GetLength()) {
				strncpy_s((char*)pHeader->AgentID, _countof(pHeader->AgentID), WtoAConverter(m_strAgentID), _TRUNCATE);
			}
						
			bRe = TRUE;
		}
		else {
			_SafeDeleteArray(pBase64Data);
			_SafeDeleteArray(pRequestData);
			nRequestDataLen = 0;
		}
	}

	return bRe;
}

BOOL CCommandManagerEx::CompressedData(char*& pCompressedData, int& nCompressedDataLen)
{
	BOOL bRe = FALSE;
	int nMaxCompressedSize = LZ4_compressBound(nCompressedDataLen);

	char* pTempCompressedData = new char[nMaxCompressedSize + PRIMX_CMD_HD_LEN];
	ZeroMemory(pTempCompressedData, nMaxCompressedSize + PRIMX_CMD_HD_LEN);

	int nCompressedSize = LZ4_compress(pCompressedData + PRIMX_CMD_HD_LEN, pTempCompressedData + PRIMX_CMD_HD_LEN, nCompressedDataLen);

	if (0 < nCompressedSize) {
		_SafeDeleteArray(pCompressedData);
		pCompressedData = pTempCompressedData;
		pTempCompressedData = NULL;
		nCompressedDataLen = nCompressedSize;
		bRe = TRUE;
	}
	else {
		_SafeDeleteArray(pTempCompressedData);
		bRe = FALSE;
	}

	return bRe;
}

BOOL CCommandManagerEx::UnCompressedData(char* pOrgData, UINT nOrgDataLen, char*& pUnCompressedData, UINT& nUnCompressedDataLen)
{
	BOOL bRe = FALSE;
	if (NULL == pOrgData || 0 == nOrgDataLen) {
		return bRe;
	}

	if (0 == nUnCompressedDataLen) {
		nUnCompressedDataLen = ((nOrgDataLen / 30) * 100) + nOrgDataLen;
	}

	pUnCompressedData = new char[nUnCompressedDataLen + 2];
	ZeroMemory(pUnCompressedData, nUnCompressedDataLen + 2);

	int unLen = LZ4_decompress_safe(pOrgData, pUnCompressedData, nOrgDataLen, nUnCompressedDataLen);

	if (0 < unLen) {
		nUnCompressedDataLen = unLen;
		bRe = TRUE;
	}
	else {
		nUnCompressedDataLen = 0;
		_SafeDeleteArray(pUnCompressedData);
	}

	return bRe;
}

BOOL CCommandManagerEx::SetRequestCommandSend(CString strRequestXML)
{
	BOOL bRe = FALSE;

	char* pRequestData = NULL;
	UINT nRequestDataLen = 0;

	if (TRUE == GetRequestDataBuffer(strRequestXML, pRequestData, nRequestDataLen)) {
		PRIMX_CMD_HD* pHeader = (PRIMX_CMD_HD*)pRequestData;

//#ifdef _DEBUG
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("------  요청 헤더 ------------------------------------------"));
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("ContentsType   : %lu (100:COMPRESSED, 200:PLAIN_TEXT) "), pHeader->ContentsType);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("DecompressSize : %lu  "), pHeader->DecompressSize);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("TimeOut        : %lu  "), pHeader->TimeOut);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("BodyLen        : %lu  "), pHeader->BodyLen);
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("------------------------------------------------------------"));
		
		CString strRequestXML_DEBUG;
		if (1024 < strRequestXML.GetLength()) {
			strRequestXML_DEBUG = strRequestXML.Left(1024);
		}
		else {
			strRequestXML_DEBUG = strRequestXML;
		}
		strRequestXML_DEBUG.Replace(_T("\r\n\r\n"), _T("\r\n"));
		strRequestXML_DEBUG.Replace(_T("\n\n"), _T("\n"));
		//strRequestXML_DEBUG.Replace(_T("><"), _T(">\r\n<"));
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("----------- Request XML ------------------------- "));
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s"), strRequestXML_DEBUG);
//#endif	
		pHeader->SetByteOder();

		if (nRequestDataLen == Send(pRequestData, nRequestDataLen)) {
			bRe = TRUE;
		}
		_SafeDeleteArray(pRequestData);
	}
	return bRe;
}

BOOL CCommandManagerEx::SetCommandHeaderCheck(PRIMX_CMD_HD* pHeader)
{
	if (NULL == pHeader) {
		return FALSE;
	}
	pHeader->SetByteOder();
	if (PRIMX_PROTOCOL_VER_MAJOR  != pHeader->Version[0]) return FALSE;
	if (PRIMX_PROTOCOL_VER_MINOR1 != pHeader->Version[1]) return FALSE;
	if (PRIMX_PROTOCOL_VER_MINOR2 != pHeader->Version[2]) return FALSE;
	if (PRIMX_PROTOCOL_VER_MINOR3 != pHeader->Version[3]) return FALSE;

	if ('M' != pHeader->Mark[0]) return FALSE;
	if ('X' != pHeader->Mark[1]) return FALSE;
	if ('C' != pHeader->Mark[2]) return FALSE;
	if ('S' != pHeader->Mark[3]) return FALSE;

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(),_T("------ 응답 헤더 ------------------------------------------"));
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(),_T("ContentsType   : %lu (100:COMPRESSED, 200:PLAIN_TEXT)"), pHeader->ContentsType);
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(),_T("DecompressSize : %lu  "), pHeader->DecompressSize);
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(),_T("TimeOut        : %lu  "), pHeader->TimeOut);
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(),_T("BodyLen        : %lu  "), pHeader->BodyLen);
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(),_T("-----------------------------------------------------------"));


	return TRUE;
}

BOOL CCommandManagerEx::SetResponseCommandReceive(CString& strResponseXML)
{
	BOOL bRe = FALSE;

	PRIMX_CMD_HD* pHeader = new PRIMX_CMD_HD;
	pHeader->Clear();

	if (PRIMX_CMD_HD_LEN == Receive((char*)pHeader, PRIMX_CMD_HD_LEN)) {
		if (TRUE == SetCommandHeaderCheck(pHeader)) {
			if (0 < pHeader->BodyLen) {
				char* pBuffer = new char[pHeader->BodyLen + 2];
				ZeroMemory(pBuffer, pHeader->BodyLen + 2);
				int nReData = Receive(pBuffer, pHeader->BodyLen);
				if (pHeader->BodyLen == nReData) {

					if (PRIMX_CONTENTS_TYPE_PLAIN_TEXT == pHeader->ContentsType) {
						strResponseXML = Utf8toWConverter(pBuffer);
						SetCommandReplyParser(strResponseXML);
						SetResponseDataTypeParser(strResponseXML);
						bRe = TRUE;
					}
					else {
						CBase64 Base64Decoder;
						int nDecoeLen = Base64Decoder.DecodeGetRequiredLength(pHeader->BodyLen);
						unsigned char* pDecoeData = new unsigned char[nDecoeLen];
						ZeroMemory(pDecoeData, nDecoeLen);

						if (TRUE == Base64Decoder.Decode(pBuffer, pHeader->BodyLen, pDecoeData, &nDecoeLen)) {
							char* pUnCompressedData = NULL;
							UINT nUnCompressedDataLen = pHeader->DecompressSize;
							nUnCompressedDataLen = 0;
							if (TRUE == UnCompressedData((char*)pDecoeData, nDecoeLen, pUnCompressedData, nUnCompressedDataLen)) {
								strResponseXML = Utf8toWConverter(pUnCompressedData);
								SetCommandReplyParser(strResponseXML);
								SetResponseDataTypeParser(strResponseXML);
								_SafeDeleteArray(pUnCompressedData);
								bRe = TRUE;
							}
						}
						_SafeDeleteArray(pDecoeData);
					}
//#ifdef _DEBUG
					if (TRUE == bRe) {
						CString strResponseXML_DEBUG;
						if (1024 < strResponseXML.GetLength()) {
							strResponseXML_DEBUG = strResponseXML.Left(1024);
						}
						else {
							strResponseXML_DEBUG = strResponseXML;
						}
						//strResponseXML_DEBUG.Replace(_T("><"), _T(">\r\n<"));
						//strResponseXML_DEBUG = strResponseXML_DEBUG + _T("\r\n");
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("<<<<<<<< ResponseXML <<<<<<<<<<<<<<<<<<<<<"));
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s"),strResponseXML_DEBUG);
					}
//#endif
				}
				else {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("응답 바디 수신 실패 : 대기 바이트[%d], 수신 바이트[%d]"), pHeader->BodyLen, nReData);
				}
				_SafeDeleteArray(pBuffer);
			}
			else {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("응답 헤더의 Body 길이 0"));
				bRe = TRUE;
			}
		}
		else {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("응답 헤더의 데이터 구성 오류"));
		}
	}
	else {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("응답 헤더 수신 오류."));
	}
	_SafeDelete(pHeader);

	return bRe;
}

UINT CCommandManagerEx::SetCommandReplyParser(CString& strXML)
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
					if (0 == strELName.CompareNoCase(_T("reply"))) {
						XMLMarkup.IntoElem();
						while (true == XMLMarkup.FindElem()) {
							CString strELNameReply = XMLMarkup.GetTagName();

							if (0 == strELNameReply.CompareNoCase(_T("code"))) {
								m_strCommnadCode = XMLMarkup.GetData();
								m_strCommnadCode.Trim();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("SubCode"))) {
								m_strSubCode = XMLMarkup.GetData();
								m_strSubCode.Trim();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("DateTime"))) {
								m_strCommnadDateTime = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("result"))) {
								m_nResult = AtoI(XMLMarkup.GetData());
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("failcode"))) {
								m_strFailCode = XMLMarkup.GetData();
								nRe = PRIMX_ERROR_SUCCESS;
							}
							if (0 == strELNameReply.CompareNoCase(_T("failmsg"))) {
								m_strFailString = XMLMarkup.GetData();
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

BOOL CCommandManagerEx::SetCommandDataRequest(CString strCode, CString strSubCode, CString strAgentID, CString strUserID, CString strDownloadFilePath)
{
	BOOL bRe = FALSE;
	if (FALSE == IsConnect()) return bRe;
	m_strDownloadFilePath = strDownloadFilePath;
	m_strDownloadFilePath.Trim();
	strCode.Trim();
	strSubCode.Trim();
	strAgentID.Trim();
	strUserID.Trim();
	
	CString strRequestXML = GetRequestCommandXMLData(strCode, strSubCode, strAgentID, strUserID);
	
	if (TRUE == SetRequestCommandSend(strRequestXML)) {
		if (TRUE == SetResponseCommandReceive(m_strResponseXML)) {
			if (0 == m_strCommnadCode.CompareNoCase(strCode) && 0 == m_strSubCode.CompareNoCase(strSubCode) && 1 == m_nResult) {
				bRe = TRUE;
			}
			else {
				if (1 == m_nResult) {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"Command Code Fail.");
				}
				else {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"Command Result Fail > %s %s", m_strFailCode, m_strFailString);
				}				
			}
		}
		else {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"Command Data Recv Fail.");
		}
	}
	else {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"Command Data Send Fail.");
	}
	
	Close();

	return bRe;
}

int CCommandManagerEx::GetResult()
{
	return m_nResult;
}

CString	CCommandManagerEx::GetFailCode()
{
	return m_strFailCode;
}

CString	CCommandManagerEx::GetFailString()
{
	return m_strFailString;
}

CString CCommandManagerEx::GetResponseXMLData()
{
	return m_strResponseXML;
}

CString CCommandManagerEx::GetMACAddr(CString strIPAddr)
{
	CString strMAC;

	IP_ADAPTER_INFO* AdapterInfo = NULL;
	DWORD dwBufLen = 0;

	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	if (ERROR_BUFFER_OVERFLOW == dwStatus) {
		int nAdapterInfoLen = sizeof(IP_ADAPTER_INFO);
		int nNICount = (dwBufLen / nAdapterInfoLen) + (0<(dwBufLen % nAdapterInfoLen) ? 1 : 0);
		if (0 < nNICount) {
			AdapterInfo = new IP_ADAPTER_INFO[nNICount];
			dwBufLen = nNICount * sizeof(IP_ADAPTER_INFO);
		}

		dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
		if (ERROR_SUCCESS == dwStatus) {
			PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
			while (NULL != pAdapterInfo) {

				PIP_ADDR_STRING pIpAddressList = &(pAdapterInfo->IpAddressList);
				while (NULL != pIpAddressList) {
					CString strTemp = AtoWConverter(pIpAddressList->IpAddress.String);
					if (0 == strTemp.CompareNoCase(strIPAddr)) {						
						// MAC 수집
						for (UINT i = 0; i<pAdapterInfo->AddressLength; i++) {
							CString strTempMAC;
							if (0 == i) {
								strTempMAC.Format(_T("%02X"), pAdapterInfo->Address[i]);
							}
							else {
								strTempMAC.Format(_T("-%02X"), pAdapterInfo->Address[i]);
							}
							strMAC = strMAC + strTempMAC;
						}
						break;
					}
					pIpAddressList = pIpAddressList->Next;
				}	
				pAdapterInfo = pAdapterInfo->Next;
			}
		}
	}
	if (NULL != AdapterInfo) {
		delete[] AdapterInfo;
		AdapterInfo = NULL;
	}

	return strMAC;
}
