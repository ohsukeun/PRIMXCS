#pragma once
#include "CommandManagerEx.h"


class CCommandExportFileAuth : public CCommandManagerEx
{
public:
	CCommandExportFileAuth();
	~CCommandExportFileAuth();

	BOOL SetExportFileAuth(CString strServerIP,unsigned short nPort, CString strFileKey, CString strCompanyID, CString strAgentID, CString strUserID,CString strFileHash);


protected:

	virtual UINT SetResponseDataTypeParser(CString& strXML);
	virtual UINT SetRequestDataTypeGenerate(CMarkup& XMLMarkup);

	CString				m_strFileKey;
	CString				m_strCompanyID;
	CString				m_strAgentID;
	CString				m_strFileHash;
	CString				m_strUserID;


};

