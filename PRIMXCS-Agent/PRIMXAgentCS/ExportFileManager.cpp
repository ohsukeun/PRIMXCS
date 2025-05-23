#include "stdafx.h"
#include "ExportFileManager.h"
#include "Markup.h"
#include "ZEDManager.h"
#include "ConfigDataManager.h"
#include "SHA512.h"
#include "ZIPFile.h"
#include "CommandExportFileReg.h"
#include "FileDBManager.h"
#include "ProgressExDlg.h"

#define PRIMX_READ_FILE_LEN				(1024*1024)

CExportFileManager::CExportFileManager()
{
	m_nAuthPort = 0;
}

CExportFileManager::~CExportFileManager()
{

}

CString CExportFileManager::GetErrorString()
{
	return m_strErrorString;
}

void CExportFileManager::SetTrim()
{
	m_strID.Trim();
	m_strPwd.Trim();
	m_strZEDPath.Trim();
	m_strZEDFilePath.Trim();
	m_strExportFileKey.Trim();

	m_strAgentID.Trim();
	m_strAuthServerAddr.Trim();
	m_strCompanyID.Trim();
	m_strCreateDate.Trim();
	m_strHashData.Trim();
}

BOOL CExportFileManager::SetDecryptFileUnZip()
{
	BOOL bRe = FALSE;
	CString strZipFilePath;
	strZipFilePath.Format(_T("%s\\%s.zip"), m_strZEDPath, m_strExportFileKey);
	SetDeleteFile(strZipFilePath);

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"��ȣȭ APP ���� ���� > %s", strZipFilePath);


	if (TRUE == SetMakeFile(IDR_DECRYPT_FILE_ZIP, strZipFilePath, _T("EXE"))) {

		if (TRUE == FileExist(strZipFilePath)) {
			CZIPFile ZIPFile;
			CStringList UnZipFileList;
			if (TRUE == ZIPFile.DoUnzipFile(strZipFilePath, m_strZEDPath, UnZipFileList)) {
				if (1 <= UnZipFileList.GetCount()) {
					CString strFileName = UnZipFileList.GetHead();
					if (0 < strFileName.GetLength()) {
						m_strDecryptFilePath.Format(_T("%s\\%s"), m_strZEDPath, strFileName);
						if (FALSE == FileExist(m_strDecryptFilePath)) {
							m_strDecryptFilePath = _T("");
							theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"��ȣȭ APP ���� > ���� ���� �� �ش� ������ ����");
						}
						else {
							bRe = TRUE;
						}
					}
					else {
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"��ȣȭ APP ���� > ���� ���� ���ϸ��� ����");
					}
				}
				else {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"��ȣȭ APP ���� > ���� ���� ������ ����");
				}
			}
			else {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"��ȣȭ APP ���� > ���� ���� Fail");
			}
		}
		else {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"��ȣȭ APP ���� > ������ �������� ���� > %s", strZipFilePath);
		}
	}
	else{
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"��ȣȭ APP ���� > SetMakeFile Fail > %s", GetLastErrorString());
	}

	return bRe;
}

/*
cd $(ProjectDir)exec\x64
del PRIMXDecryptFile.zip
tar -cvzf PRIMXDecryptFile.zip PRIMXDecryptFile.exe

cd $(ProjectDir)exec\x86
del PRIMXDecryptFile.zip
tar -cvzf PRIMXDecryptFile.zip PRIMXDecryptFile.exe

*/

BOOL CExportFileManager::SetExportFileCreate(CString strExportFilePath, CList<PRIMX_ORG_FILE_INFO*>& OrgFileList, CString& strKey, CProgressExDlg* pProgressExDlg)
{
	BOOL bRe = FALSE;

	CWaitCursor WaitCursor;

	CZEDManager ZEDManager;
	m_strExportFileKey = GetCreateKey();
	strKey = m_strExportFileKey;
	m_strZEDPath.Format(_T("%s\\ExportFileTemp"), GetProgramDataPath());
	m_strZEDFilePath.Format(_T("%s\\%s.zed"), m_strZEDPath, m_strExportFileKey);

	SetCreateDirectoryEx(m_strZEDPath);

	CString strZipFilePath;
	strZipFilePath.Format(_T("%s\\%s.zip"), m_strZEDPath, m_strExportFileKey);

	LONGLONG nExportFileSize = 0;
	
	if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(0, GetLoadStringTable(PRIMX_PROGRESS_MSG14));

	if (TRUE == SetDecryptFileUnZip()) {
		if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(0, GetLoadStringTable(PRIMX_PROGRESS_MSG13));

		if (0 < m_strDecryptFilePath.GetLength()) {

			// ZED ���� ����
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > ZED ���� > %s", m_strZEDFilePath);

			if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(0, GetLoadStringTable(PRIMX_PROGRESS_MSG12));

			if (TRUE == ZEDManager.CreateZedFile(m_strZEDFilePath, OrgFileList, m_strID, m_strPwd, pProgressExDlg)) {

				// ���� XML ����
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > �̵����� ���� > �������� XML ����");
				CString strMasterKey;
				CString strAuthXML = GetExportFileAuthXMLData(strMasterKey);

				if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(0, GetLoadStringTable(PRIMX_PROGRESS_MSG11));
				
				// �̵����� ����
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > �̵����� ���� > %s", strExportFilePath);
				HANDLE hExportFile = CreateFile(strExportFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

				if (INVALID_HANDLE_VALUE != hExportFile) {

					// ��ȣȭ APP ����
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > �̵����� ���� > ��ȣȭ APP �߰�");
					if (TRUE == SetFileDataAdd(hExportFile, m_strDecryptFilePath)) {

						DWORD dwWriteSize = 0;
						DWORD dwMasterKeyLen = strMasterKey.GetLength();
						DWORD dwAuthXMLLen = strAuthXML.GetLength();
						LONGLONG nZEDFileLen = GetFileSizeEx(m_strZEDFilePath);

						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > �̵����� ���� > ���� ���� �߰�");
						if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(0, GetLoadStringTable(PRIMX_PROGRESS_MSG10));

						unsigned char MagicData[10] = { '\r','\n','\r','\n',0x07,0x0e,0x07,0x0e,0x07,0x0e };
						WriteFile(hExportFile, (void*)MagicData, 10, &dwWriteSize, NULL);

						WriteFile(hExportFile, (void*)&dwMasterKeyLen, sizeof(DWORD), &dwWriteSize, NULL);
						WriteFile(hExportFile, (void*)&dwAuthXMLLen, sizeof(DWORD), &dwWriteSize, NULL);
						WriteFile(hExportFile, (void*)&nZEDFileLen, sizeof(LONGLONG), &dwWriteSize, NULL);
						WriteFile(hExportFile, (LPCSTR)WtoAConverter(strMasterKey), dwMasterKeyLen, &dwWriteSize, NULL);
						WriteFile(hExportFile, (LPCSTR)WtoAConverter(strAuthXML), dwAuthXMLLen, &dwWriteSize, NULL);

						if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(0, GetLoadStringTable(PRIMX_PROGRESS_MSG9));

						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > �̵����� ���� > ZED ���� �߰�");
						if (TRUE == SetFileDataAdd(hExportFile, m_strZEDFilePath)) {
							if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(0, GetLoadStringTable(PRIMX_PROGRESS_MSG8));
							bRe = TRUE;
						}			
						else{
							theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > �̵����� ���� > ZED ���� �߰� ����");
							if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(95, GetLoadStringTable(PRIMX_PROGRESS_MSG6));
						}
					}
					else{
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > �̵����� ���� > ��ȣȭ APP �߰� ����");
						if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(95, GetLoadStringTable(PRIMX_PROGRESS_MSG5));
					}
					
					if(FALSE == bRe){
						m_strErrorString.LoadString(IDS_ERROR_STRING17); // �̵����Ͽ� ��ȣȭ ���� ������ ���� �߽��ϴ�.
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� ���� > %s", m_strErrorString);
						if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(95, m_strErrorString);
					}

					LARGE_INTEGER fileSize;
					memset(&fileSize, 0x0, sizeof(LARGE_INTEGER));
					if (TRUE == ::GetFileSizeEx(hExportFile, &fileSize)) {
						nExportFileSize = fileSize.QuadPart;
					}					

					CloseHandle(hExportFile);
				}
				else {
					m_strErrorString = GetLastErrorString();
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > �̵����� ���� ���� > %s", m_strErrorString);
					if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(95, m_strErrorString);
				}
			}
			else {
				m_strErrorString.LoadString(IDS_ERROR_STRING16); // ZED���� ������ ���� �߽��ϴ�.
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > ZED ���� ���� > %s", m_strErrorString);
				if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(95, m_strErrorString);
			}
		}

		if (TRUE == bRe) {
			CCommandExportFileReg CommandExportFileReg;
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > �̵����� ���� ��� > ���� ����");
			if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(0, GetLoadStringTable(PRIMX_PROGRESS_MSG4));

			if (TRUE == CommandExportFileReg.SetConnect()) {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > �̵����� ���� ��� > ���� ���");
				if (FALSE == CommandExportFileReg.SetExportFileReg(m_strExportFileKey, strExportFilePath, nExportFileSize, &OrgFileList)) {
					m_strErrorString.LoadString(IDS_ERROR_STRING18); // ������ �̵������� ������ ���� ��Ͽ� ���� �߽��ϴ�.
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� ���� > %s", m_strErrorString);
					if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(95, m_strErrorString);
					bRe = FALSE;
				}
				else {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > �̵����� ���� ��� > ����DB ����");
					if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(0, GetLoadStringTable(PRIMX_PROGRESS_MSG3));
					CFileDBManager FileDBManager;
					FileDBManager.SetExportFileDataSave(m_strExportFileKey, SetSplitPathFileName(strExportFilePath), nExportFileSize, OrgFileList);
					bRe = TRUE;
				}
			}
			else {
				m_strErrorString.LoadString(IDS_ERROR_STRING19); // ������ �̵������� ���� ����� ���� �������ῡ ���� �߽��ϴ�.
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� > �̵����� ���� ��� > ���� ���� ����");
				if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(95, m_strErrorString);
			}
		}

	}
	else{
		m_strErrorString.LoadString(IDS_ERROR_STRING15); // ��ȣȭ ���α׷� ���� ������ ���� �߽��ϴ�.
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"�̵����� ���� ���� > %s", m_strErrorString);
		if (NULL != pProgressExDlg) pProgressExDlg->SetProgressValue(95, m_strErrorString);

	}

	SetDeleteFileWithSubDir(m_strZEDPath);

	return bRe;
}

BOOL CExportFileManager::SetFileDataAdd(HANDLE hDstFile,CString strSrcFilePath)
{
	BOOL bRe = FALSE;
	if (INVALID_HANDLE_VALUE == hDstFile) {
		return 	bRe;
	}

	HANDLE hSrcFile = CreateFile(strSrcFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hSrcFile) {

		LARGE_INTEGER fileSize;
		memset(&fileSize, 0x0, sizeof(LARGE_INTEGER));
		if (FALSE == ::GetFileSizeEx(hSrcFile, &fileSize)) {
			CloseHandle(hSrcFile);
			return bRe;
		}

		LONGLONG nReadDataLen = 0;
		LONGLONG nSrcFileSize = fileSize.QuadPart;
		UINT bReadSize = (PRIMX_READ_FILE_LEN < nSrcFileSize ? PRIMX_READ_FILE_LEN : (UINT)nSrcFileSize);
		char* pReadBuffer = new char[PRIMX_READ_FILE_LEN];

		while (1) {
			ZeroMemory(pReadBuffer, PRIMX_READ_FILE_LEN);
			DWORD dwNumberOfBytesRead = 0;
			if (TRUE == ReadFile(hSrcFile, pReadBuffer, PRIMX_READ_FILE_LEN, &dwNumberOfBytesRead, NULL)) {
				DWORD dwWriteSize = 0;
				nReadDataLen = nReadDataLen + dwNumberOfBytesRead;
				WriteFile(hDstFile, pReadBuffer, dwNumberOfBytesRead, &dwWriteSize, NULL);
			}
			if (nSrcFileSize <= nReadDataLen) {
				bRe = TRUE;
				break;
			}
		}
		CloseHandle(hSrcFile);
	}
	return bRe;
}

CString CExportFileManager::GetExportFileAuthXMLData(CString& strMasterKey)
{
	CString strAuthData;

	if (FALSE == FileExist(m_strZEDFilePath)) return strAuthData;

	CConfigDataManager ConfigDataManager;
	CSHA512 SHA512;

	m_strAgentID = ConfigDataManager.GetAgentID();
	m_strAuthServerAddr = ConfigDataManager.GetAuthServerAddr();
	m_nAuthPort = ConfigDataManager.GetAuthPort();
	m_strCompanyID = ConfigDataManager.GetCompanyID();
	m_strCreateDate = GetDateTimeForGMT();
	m_strHashData = SHA512.GetShaBase64Data(m_strZEDFilePath);
	m_strUserID = ConfigDataManager.GetUserID();

	SetTrim();

	CMarkup XMLMarkup;
	
	XMLMarkup.SetDoc(_T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"));
	XMLMarkup.IntoElem();
	XMLMarkup.AddElem(_T("ExportFile"));
	XMLMarkup.IntoElem();
	XMLMarkup.AddElem(_T("Key"), m_strExportFileKey, CMarkup::MNF_WITHCDATA);
	XMLMarkup.AddElem(_T("AgentID"), m_strAgentID, CMarkup::MNF_WITHCDATA);
	XMLMarkup.AddElem(_T("ExpiryDate"),_T("") );
	XMLMarkup.AddElem(_T("CreateDate"), m_strCreateDate);
	XMLMarkup.AddElem(_T("CompanyID"), m_strCompanyID);
	XMLMarkup.AddElem(_T("UserID"), m_strUserID);
	XMLMarkup.AddElem(_T("AuthServer"), m_strAuthServerAddr);
	XMLMarkup.AddElem(_T("AuthServerPort"), m_nAuthPort);
	XMLMarkup.AddElem(_T("DecryptID"), m_strID, CMarkup::MNF_WITHCDATA);
	XMLMarkup.AddElem(_T("DecryptPwd"), m_strPwd, CMarkup::MNF_WITHCDATA);
	XMLMarkup.AddElem(_T("ZedFileHash"), m_strHashData, CMarkup::MNF_WITHCDATA);
	XMLMarkup.OutOfElem();
	XMLMarkup.OutOfElem();	

	CARIAEncryption ARIAEncryption;
	strMasterKey = ARIAEncryption.SetBaseKeyCreateData();

	CString strAuthDataTemp = ARIAEncryption.EncryptString(XMLMarkup.GetDoc());
	CString strAuthDataHash = ARIAEncryption.GetSaltSHA512(strAuthDataTemp);
	strAuthData.Format(_T("%s@%s"), strAuthDataHash, strAuthDataTemp);

	return strAuthData;

}