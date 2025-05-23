#pragma once
#include "CommonModule.h"

class CProgressExDlg;
class CExportFileManager : public CCommonModule
{
public:
	CExportFileManager();
	~CExportFileManager();
	
	BOOL SetExportFileCreate(CString strExportFilePath, CList<PRIMX_ORG_FILE_INFO*>& OrgFileList, CString& strKey, CProgressExDlg* pProgressExDlg);

	CString GetErrorString();

protected:

	CString GetExportFileAuthXMLData(CString& strMasterKey);
	void SetTrim();
	BOOL SetDecryptFileUnZip();
	BOOL SetFileDataAdd(HANDLE hDstFile, CString strSrcFilePath);


	CString m_strID;
	CString m_strPwd;
	CString m_strZEDPath;
	CString m_strZEDFilePath;
	CString m_strExportFileKey;

	CString m_strDecryptFilePath;

	CString  m_strAgentID;
	CString  m_strAuthServerAddr;
	unsigned short  m_nAuthPort;
	CString  m_strCompanyID;
	CString  m_strCreateDate;
	CString  m_strHashData;
	CString  m_strUserID;

	CString m_strErrorString;


};

