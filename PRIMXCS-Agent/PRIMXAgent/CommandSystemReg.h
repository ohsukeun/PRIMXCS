#pragma once
#include "CommandManagerEx.h"


class CCommandSystemReg : public CCommandManagerEx
{
public:
	CCommandSystemReg();
	~CCommandSystemReg();

	BOOL SetSystemDataReg(CString strUserID, UINT nSystemType, PRIMX_SYSTEM_DATA& SystemData);
	BOOL SetSystemDataModify(PRIMX_SYSTEM_DATA& SystemData,UINT nModifyFlag);

protected:
	CString		m_strCode;
	CString		m_strSubCode;
	UINT		m_nModifyFlag;

	virtual UINT SetResponseDataTypeParser(CString& strXML);
	virtual UINT SetRequestDataTypeGenerate(CMarkup& XMLMarkup);

	PRIMX_SYSTEM_DATA			m_SystemData;


};

