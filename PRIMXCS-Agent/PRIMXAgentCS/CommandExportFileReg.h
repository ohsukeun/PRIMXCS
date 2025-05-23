#pragma once
#include "../CommonLib/Utils/CommandManagerEx.h"

class CCommandExportFileReg : public CCommandManagerEx
{
public:
	CCommandExportFileReg();
	~CCommandExportFileReg();

	BOOL SetExportFileReg(CString strFileKey, CString strExportFilePath, LONGLONG nExportFileSize, CList<PRIMX_ORG_FILE_INFO*>* pOrgFileList);


protected:

	virtual UINT SetResponseDataTypeParser(CString& strXML);
	virtual UINT SetRequestDataTypeGenerate(CMarkup& XMLMarkup);

	CList<PRIMX_ORG_FILE_INFO*>*		m_pOrgFileList;
	CString								m_strFileKey;
	CString								m_strExportFilePath;
	CString								m_strExportFileName;
	CString								m_strExportFileSize;

};

