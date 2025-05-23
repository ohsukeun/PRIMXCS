#pragma once
#include "CommandManagerEx.h"


class CCommandUpdateFile : public CCommandManagerEx
{
public:
	CCommandUpdateFile();
	~CCommandUpdateFile();

	BOOL GetUpdateFile(CString strProductType, CString strUpdateFilePath);

	CString GetUpdateFileHaseData();
	CString GetUpdateVersion();

protected:
	CString		m_strCode;
	CString		m_strSubCode;
	CString		m_strProductType;
	CString		m_strUpdateFilePath;


	CString		m_strResProductType;
	CString		m_strResUpdateVersion;
	CString		m_strResHashdata;
	UINT		m_nResUpdateFileSize;
	CString		m_strFileName;
	CStringList m_DownloadURLList;
	BOOL		m_bFileDownload;

	PRIMX_PRODUCT_DATA		m_ZonecentrolData;
	PRIMX_PRODUCT_DATA		m_OrizoneData;
	PRIMX_PRODUCT_DATA		m_CryhodData;


	virtual UINT SetResponseDataTypeParser(CString& strXML);
	virtual UINT SetRequestDataTypeGenerate(CMarkup& XMLMarkup);

	BOOL GetUpdateFileDownload(CString strURL, CString strSaveFilePath);

};

