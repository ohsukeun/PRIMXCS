#pragma once
#include "SNIDSocket.h"
#include "CommonModule.h"
#include "Markup.h"
#include "PRIMXAgent_if.h"

class CCommandManagerEx : public CSNIDSocket , public CCommonModule
{
public:
	CCommandManagerEx();
	~CCommandManagerEx();

	int GetResult();
	CString	GetFailCode();
	CString	GetFailString();
	CString GetResponseXMLData();

	BOOL SetConnect(CString strIP, unsigned short nPort, BOOL bSSL = TRUE);
	BOOL SetConnect(BOOL bSSL = TRUE);

protected:
	CString GetRequestCommandXMLData(CString strCode, CString strSubCode, CString strAgentID, CString strUserID);
	BOOL GetRequestDataBuffer(CString strRequestXML, char*& pRequestData, UINT& nRequestDataLen);
	BOOL CompressedData(char*& pCompressedData, int& nCompressedDataLen);
	BOOL UnCompressedData(char* pOrgData, UINT nOrgDataLen, char*& pUnCompressedData, UINT& nUnCompressedDataLen);
	BOOL SetCommandHeaderCheck(PRIMX_CMD_HD* pHeader);
	BOOL SetRequestCommandSend(CString strRequestXML);
	BOOL SetResponseCommandReceive(CString& strResponseXML);
	UINT SetCommandReplyParser(CString& strXML);
	BOOL SetCommandDataRequest(CString strCode, CString strSubCode, CString strAgentID, CString strUserID,CString strDownloadFilePath);
	CString GetAgentID();
	CString GetMACAddr(CString strIPAddr);

	virtual UINT SetResponseDataTypeParser(CString& strXML);
	virtual UINT SetRequestDataTypeGenerate(CMarkup& XMLMarkup);

	UINT		m_nCompressedType;

	CString		m_strAgentID;
	CString		m_strAgentVersion;
	CString		m_strUserID;
	UINT		m_nSystemType;
	CString		m_strCompanyID;
	CString		m_strAgentPolicyStamp;

	CString		m_strCommnadCode;
	CString		m_strSubCode;
	CString		m_strCommnadDateTime;
	CString		m_strDownloadFilePath;
	CString		m_strFailCode;
	CString		m_strFailString;
	int			m_nResult;
	CString		m_strResponseXML;

	CString		m_strConnectIPAddr;
	CString		m_strConnectMAC;
};

