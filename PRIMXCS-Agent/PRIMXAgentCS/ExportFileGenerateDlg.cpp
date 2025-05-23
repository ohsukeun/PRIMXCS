// ExportFileGenerateDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PRIMXAgentCS.h"
#include "ExportFileGenerateDlg.h"
#include "afxdialogex.h"
#include "ExportFileManager.h"
#include "ConfigDataManager.h"
#include "ProgressExDlg.h"


// CExportFileGenerateDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CExportFileGenerateDlg, CDialogEx)

CExportFileGenerateDlg::CExportFileGenerateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXPORT_FILE_GEN_DLG, pParent)
	, m_strExportFilePath(_T("")), m_ListDropTarget(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_accelerators_key = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

}

CExportFileGenerateDlg::~CExportFileGenerateDlg()
{

}

void CExportFileGenerateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORG_FILE_LIST, m_ctrlOrgFileList);
	DDX_Text(pDX, IDC_EXPORT_FILE_PATH, m_strExportFilePath);
	DDX_Control(pDX, IDC_EXPORT_FILE_PATH_BTN, m_ctrlExportFileBTN);
	DDX_Control(pDX, IDC_ORG_FILE_ADD_BTN, m_ctrlOrgFileAddBTN);
}


BEGIN_MESSAGE_MAP(CExportFileGenerateDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ORG_FILE_ADD_BTN, &CExportFileGenerateDlg::OnBnClickedOrgFileAddBtn)
	ON_BN_CLICKED(IDC_EXPORT_FILE_PATH_BTN, &CExportFileGenerateDlg::OnBnClickedExportFilePathBtn)
	ON_BN_CLICKED(IDC_ORG_FILE_DEL_BTN, &CExportFileGenerateDlg::OnBnClickedOrgFileDelBtn)
	ON_NOTIFY(LVN_KEYDOWN, IDC_ORG_FILE_LIST, &CExportFileGenerateDlg::OnLvnKeydownOrgFileList)
	ON_BN_CLICKED(IDOK, &CExportFileGenerateDlg::OnBnClickedOk)
	ON_COMMAND(ID_AC_LIST_SELECT_ALL, &CExportFileGenerateDlg::OnAcListSelectAll)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CExportFileGenerateDlg 메시지 처리기입니다.


BOOL CExportFileGenerateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	
	m_ctrlOrgFileList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS);
	m_ctrlOrgFileList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_ctrlOrgFileList.SetColumnWithImage(IDS_ORG_FILE_LIST_COLUMN, IDB_EXPORT_FILE_ITEM, 20, 20);

	m_ctrlOrgFileList.DragAcceptFiles(TRUE);
	m_ListDropTarget.Register(&m_ctrlOrgFileList);

	return TRUE;
}


void CExportFileGenerateDlg::OnBnClickedOrgFileAddBtn()
{
	CConfigDataManager ConfigDataManager;
	TCHAR szFilters[] = _T("All File(*.*)|*.*|");
	CFileDialog FileDialog(TRUE, NULL, _T("*.*"), OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT, szFilters, this);

	CString strbuf;
	FileDialog.m_ofn.lpstrFile = strbuf.GetBuffer(4096);
	FileDialog.m_ofn.nMaxFile = 4096;

	CString strDirectory = ConfigDataManager.GetExportFileLastDirectory();
	if (0 < strDirectory.GetLength()) {
		SetCurrentDirectory(strDirectory);
	}
	else {
		SetCurrentDirectory(_T("c:\\"));
	}

	if (IDOK == FileDialog.DoModal()) {
		POSITION Pos = FileDialog.GetStartPosition();
		while (NULL != Pos) {
			CString strFilePath = FileDialog.GetNextPathName(Pos);
			if (FALSE == IsExistFileData(strFilePath)) {
				PRIMX_ORG_FILE_INFO* pFileData = new PRIMX_ORG_FILE_INFO;
				pFileData->strFilePath = strFilePath;
				pFileData->strFilePath.Trim();
				pFileData->nFileSize = GetFileSizeEx(pFileData->strFilePath);
				int nItemIndex = m_ctrlOrgFileList.InsertItem(0, pFileData->strFilePath, 0);
				m_ctrlOrgFileList.SetItemText(nItemIndex, 1, SizeToA(pFileData->nFileSize));
				m_ctrlOrgFileList.SetItemData(nItemIndex, (DWORD_PTR)pFileData);
			}
		}
		strDirectory = FileDialog.GetFolderPath();
		ConfigDataManager.SetExportFileLastDirectory(strDirectory);
	}
}

void CExportFileGenerateDlg::OnBnClickedOrgFileDelBtn()
{
	if (0 > m_ctrlOrgFileList.GetSelectionMark()) {
		ALERT_MSGBOX(IDS_ERROR_STRING20);
	}
	else{
		POSITION Pos = m_ctrlOrgFileList.GetFirstSelectedItemPosition();

		while (NULL != Pos) {
			int nItem = m_ctrlOrgFileList.GetNextSelectedItem(Pos);
			PRIMX_ORG_FILE_INFO* pFileData = (PRIMX_ORG_FILE_INFO*)m_ctrlOrgFileList.GetItemData(nItem);
			_SafeDelete(pFileData);
			m_ctrlOrgFileList.DeleteItem(nItem);
			Pos = m_ctrlOrgFileList.GetFirstSelectedItemPosition();
		}
	}

}

void CExportFileGenerateDlg::OnBnClickedExportFilePathBtn()
{
	TCHAR szFilters[] = _T("Exe File(*.exe)|*.exe|");
	CFileDialog FileDialog(FALSE, _T("*.exe"), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER, szFilters, this);

	if (IDOK == FileDialog.DoModal()) {
		m_strExportFilePath = FileDialog.GetPathName();
		m_strExportFilePath.Trim();
		UpdateData(FALSE);
	}
}

void CExportFileGenerateDlg::OnLvnKeydownOrgFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

	if (VK_DELETE == pLVKeyDow->wVKey) {
		OnBnClickedOrgFileDelBtn();
	}

	*pResult = 0;
}

BOOL CExportFileGenerateDlg::DestroyWindow()
{
	int nCount = m_ctrlOrgFileList.GetItemCount();
	for (int nItem = 0; nItem < nCount; nItem++) {
		PRIMX_ORG_FILE_INFO* pFileData = (PRIMX_ORG_FILE_INFO*)m_ctrlOrgFileList.GetItemData(nItem);
		_SafeDelete(pFileData);
	}
	m_ctrlOrgFileList.DeleteAllItems();

	return __super::DestroyWindow();
}

void CExportFileGenerateDlg::OnAcListSelectAll()
{
	m_ctrlOrgFileList.SelectAll(TRUE);
}

BOOL CExportFileGenerateDlg::IsFilePathCheck(CString strFilePath)
{
	BOOL bRe = FALSE;

	if (FALSE == PathIsLFNFileSpec(strFilePath)){
		ALERT_MSGBOX(IDS_ERROR_STRING10);
		return FALSE;
	}

	TCHAR cTemp[MAX_PATH] = { 0 };
	_tcsncpy_s(cTemp, _countof(cTemp), strFilePath, _TRUNCATE);

	if (0 == PathRemoveFileSpec(cTemp)) {
		ALERT_MSGBOX(IDS_ERROR_STRING11);
		return FALSE;
	}

	if (FALSE == PathFileExists(cTemp)) {
		ALERT_MSGBOX(IDS_ERROR_STRING12);
		return FALSE;
	}
	_tcsncpy_s(cTemp, _countof(cTemp), strFilePath, _TRUNCATE);
	PathStripPath(cTemp);
	CString strTemp = cTemp;

	if (0 == strTemp.GetLength()) {
		ALERT_MSGBOX(IDS_ERROR_STRING13);
		return FALSE;
	}

	if (0 <= strTemp.FindOneOf(_T("\\/:*?\"<>|"))) {
		ALERT_MSGBOX(IDS_ERROR_STRING13);
		return FALSE;
	}

	return TRUE;
}

void CExportFileGenerateDlg::OnBnClickedOk()
{
	UpdateData();
	m_strExportFilePath.Trim();
	
	if (0 == m_strExportFilePath.GetLength()) {
		ALERT_MSGBOX(IDS_ERROR_STRING5);
		m_ctrlExportFileBTN.SetFocus();
		return;
	}

	if (FALSE == IsFilePathCheck(m_strExportFilePath)) {
		m_ctrlExportFileBTN.SetFocus();
		return;
	}

	if (0 == m_ctrlOrgFileList.GetItemCount()) {
		ALERT_MSGBOX(IDS_ERROR_STRING6);
		m_ctrlOrgFileAddBTN.SetFocus();
		return;
	}

	m_OrgFileList.RemoveAll();

	LONGLONG nTotalFileSize = 0;

	for (int i = 0; i < m_ctrlOrgFileList.GetItemCount(); i++) {
		PRIMX_ORG_FILE_INFO* pFileData = (PRIMX_ORG_FILE_INFO*)m_ctrlOrgFileList.GetItemData(i);
		if (NULL != pFileData) {
			nTotalFileSize = nTotalFileSize + pFileData->nFileSize;
			if (0 == m_strExportFilePath.CompareNoCase(pFileData->strFilePath)) {
				ALERT_MSGBOX(IDS_ERROR_STRING9);
				m_ctrlExportFileBTN.SetFocus();
				m_OrgFileList.RemoveAll();
				return;
			}
			m_OrgFileList.AddTail(pFileData);
		}
	}

	if (PRIMX_EX_FILE_MAX_SIZE < nTotalFileSize) {
		CString strMsg;
		strMsg.Format(IDS_ERROR_STRING21, SizeToA(PRIMX_EX_FILE_MAX_SIZE), SizeToA(nTotalFileSize), IntToCurrencyString(nTotalFileSize));
		ALERT_MSGBOX(strMsg);
		m_ctrlExportFileBTN.SetFocus();
		m_OrgFileList.RemoveAll();
		return;
	}
	
	if (IDYES == QUESTION_MSGBOX(IDS_EXPORT_FILE_GEN_MSG)) {

		if (TRUE == PathFileExists(m_strExportFilePath)) {
			if(IDYES != QUESTION_MSGBOX(IDS_ERROR_STRING8)) {
				m_OrgFileList.RemoveAll();
				return;
			}
		}
		int nFileCount = m_OrgFileList.GetCount();

		CProgressExDlg ProgressExDlg(this);
		if (100 < nFileCount) {
			ProgressExDlg.SetMaxRange(nFileCount + (nFileCount/3.0));
		}

		ProgressExDlg.DoModal();
		int nResult = ProgressExDlg.m_nResult;

		if (1 == nResult) {
			INFOR_MSGBOX(IDS_EXPORT_FILE_GEN_OK);
			CDialogEx::OnOK();
		}
		else {
			CString strTemp = ProgressExDlg.GetErrorString();
			if (0 == strTemp.GetLength()) {
				ALERT_MSGBOX(IDS_ERROR_STRING14);
			}
			else {
				CString strMsg;
				strTemp.LoadString(IDS_ERROR_STRING14);
				strMsg.Format(_T("%s\r\n[%s]"), strTemp, ProgressExDlg.GetErrorString());
				ALERT_MSGBOX(strMsg);
			}	
		}			
	}
	m_OrgFileList.RemoveAll();
}

void CExportFileGenerateDlg::SetOrgFileListRemoveAll(CList<PRIMX_ORG_FILE_INFO*>& OrgFileList)
{
	POSITION Pos = OrgFileList.GetHeadPosition();
	while (NULL != Pos) {
		PRIMX_ORG_FILE_INFO* pData = OrgFileList.GetNext(Pos);
		_SafeDelete(pData);
	}
	OrgFileList.RemoveAll();
}




HBRUSH CExportFileGenerateDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushBack;
	}
	return hbr;
}


BOOL CExportFileGenerateDlg::OnEraseBkgnd(CDC* pDC)
{
	COLORREF clrBack = RGB(255, 255, 255);

	if (!m_brushBack.GetSafeHandle() || clrBack != m_clrBack)
	{
		m_brushBack.DeleteObject();
		m_brushBack.CreateSolidBrush(clrBack);
		m_clrBack = clrBack;
	}

	CRect rect;
	GetClientRect(&rect);

	pDC->FillSolidRect(rect, m_clrBack);

	return TRUE;
}


BOOL CExportFileGenerateDlg::PreTranslateMessage(MSG* pMsg)
{
	if (NULL != m_accelerators_key) {
		if (::TranslateAccelerator(m_hWnd, m_accelerators_key, pMsg)) return TRUE;
	}
	return __super::PreTranslateMessage(pMsg);
}

int CExportFileGenerateDlg::SetExportFileCreate(CProgressExDlg* pProgressExDlg)
{
	int nRe = 0;
	CExportFileManager ExportFileManager;
	if (TRUE == ExportFileManager.SetExportFileCreate(m_strExportFilePath, m_OrgFileList, m_strExFileKey, pProgressExDlg)) {
		nRe = 1;
	}	
	return nRe;

}

void CExportFileGenerateDlg::SetAddFile(CStringList& FileList)
{
	POSITION Pos = FileList.GetHeadPosition();
	while (NULL != Pos) {
		CString strFilePath = FileList.GetNext(Pos);
		if (FALSE == IsExistFileData(strFilePath)) {
			PRIMX_ORG_FILE_INFO* pFileData = new PRIMX_ORG_FILE_INFO;
			pFileData->strFilePath = strFilePath;
			pFileData->strFilePath.Trim();
			pFileData->nFileSize = GetFileSizeEx(pFileData->strFilePath);
			int nItemIndex = m_ctrlOrgFileList.InsertItem(0, pFileData->strFilePath, 0);
			m_ctrlOrgFileList.SetItemText(nItemIndex, 1, SizeToA(pFileData->nFileSize));
			m_ctrlOrgFileList.SetItemData(nItemIndex, (DWORD_PTR)pFileData);
		}
	}
}

BOOL CExportFileGenerateDlg::IsExistFileData(CString strFilePath)
{
	BOOL bRe = FALSE;
	strFilePath.Trim();
	for (int i = 0; i < m_ctrlOrgFileList.GetItemCount(); i++) {
		PRIMX_ORG_FILE_INFO* pFileData = (PRIMX_ORG_FILE_INFO*)m_ctrlOrgFileList.GetItemData(i);
		if (NULL != pFileData) {
			if (0 == strFilePath.CompareNoCase(pFileData->strFilePath)) {
				bRe = TRUE;
				break;
			}
		}
	}
	return bRe;
}