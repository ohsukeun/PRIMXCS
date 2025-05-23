#pragma once
#include "CommandManagerEx.h"


class CCommandLogDataTransfer : public CCommandManagerEx
{
public:
	CCommandLogDataTransfer();
	~CCommandLogDataTransfer();

	BOOL SetLogDataTransfer(CList<PRIMX_LOG_DATA*>*	pLogDataList);

protected:
	CString		m_strCode;
	CString		m_strSubCode;

	virtual UINT SetResponseDataTypeParser(CString& strXML);
	virtual UINT SetRequestDataTypeGenerate(CMarkup& XMLMarkup);

	CList<PRIMX_LOG_DATA*>*		m_pLogDataList;

};

