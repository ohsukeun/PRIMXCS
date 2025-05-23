// ExportFilePage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PRIMXAgentCS.h"
#include "ExportFilePage.h"
#include "ExportFileGenerateDlg.h"
#include "FileDBManager.h"
#include "ExportFileDetailDlg.h"


// CExportFilePage

IMPLEMENT_DYNCREATE(CExportFilePage, CFormView)

CExportFilePage::CExportFilePage()
	: CFormView(IDD_EXPORT_FILE_PAGE)
{

}

CExportFilePage::~CExportFilePage()
{
}

void CExportFilePage::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPORT_FILE_LIST, m_ctrlExportList);
}

BEGIN_MESSAGE_MAP(CExportFilePage, CFormView)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_EXPORT_FILE_CREATE_BTN, &CExportFilePage::OnBnClickedExportFileCreateBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_EXPORT_FILE_LIST, &CExportFilePage::OnNMDblclkExportFileList)
END_MESSAGE_MAP()


// CExportFilePage 진단입니다.

#ifdef _DEBUG
void CExportFilePage::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CExportFilePage::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CExportFilePage 메시지 처리기입니다.


void CExportFilePage::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_ctrlExportList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS);
	m_ctrlExportList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES); // LVS_EX_GRIDLINES
	m_ctrlExportList.SetColumnWithImage(IDS_EXPORT_FILE_LIST_COLUMN, IDB_EXPORT_FILE_ITEM, 20, 20);

	//GetExportFileData();
}


BOOL CExportFilePage::OnEraseBkgnd(CDC* pDC)
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

	return TRUE; //CFormView::OnEraseBkgnd(pDC);
}


HBRUSH CExportFilePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushBack;
	}
	return hbr;
}

void CExportFilePage::OnBnClickedExportFileCreateBtn()
{
	CExportFileGenerateDlg ExportFileGenerateDlg(this);
	if (IDOK == ExportFileGenerateDlg.DoModal()) {
		CString strKey = ExportFileGenerateDlg.m_strExFileKey;
		
		CFileDBManager FileDBManager;
		PRIMX_EXPORT_FILE_INFO* pExFileData = new PRIMX_EXPORT_FILE_INFO;
		if (TRUE == FileDBManager.GetExportFileData(strKey, pExFileData)) {
			m_ExFileList.AddTail(pExFileData);

			CString strTemp;
			PRIMX_ORG_FILE_INFO* pOrgFile = pExFileData->OrgFileList.GetHead();
			if (NULL != pOrgFile) {
				int nOrgFileCount = pExFileData->OrgFileList.GetCount();
				if (1 == nOrgFileCount) {
					strTemp.Format(_T("%s"), pOrgFile->strFilePath);
				}
				else {
					strTemp.Format(PRIMX_PROGRESS_MSG2, pOrgFile->strFilePath, (pExFileData->OrgFileList.GetCount() - 1));
				}
			}
			int nIndex = m_ctrlExportList.InsertItem(0, pExFileData->strExportFileName, 1);
			m_ctrlExportList.SetItemText(nIndex, 1, SizeToA(pExFileData->nExportFileSize));
			m_ctrlExportList.SetItemText(nIndex, 2, pExFileData->strCreateDate);
			m_ctrlExportList.SetItemText(nIndex, 3, strTemp);
			m_ctrlExportList.SetItemText(nIndex, 4, SizeToA(pExFileData->nOrgFileTotalSize));
			m_ctrlExportList.SetItemData(nIndex, DWORD_PTR(pExFileData));

		}
		else{
			_SafeDelete(pExFileData);
		}
	}	
}

void CExportFilePage::SetExportFileListRemoveAll()
{
	POSITION Pos = m_ExFileList.GetHeadPosition();
	while (NULL != Pos) {
		PRIMX_EXPORT_FILE_INFO* pData = m_ExFileList.GetNext(Pos);
		_SafeDelete(pData);
	}
	m_ExFileList.RemoveAll();

}
void CExportFilePage::GetExportFileData()
{
	CFileDBManager FileDBManager;

	SetExportFileListRemoveAll();

	FileDBManager.GetExportFileData(m_ExFileList);

	m_ctrlExportList.DeleteAllItems();

	POSITION Pos = m_ExFileList.GetHeadPosition();
	while (NULL != Pos) {
		PRIMX_EXPORT_FILE_INFO* pData = m_ExFileList.GetNext(Pos);
		if (NULL != pData) {
			CString strTemp;
			PRIMX_ORG_FILE_INFO* pOrgFile = pData->OrgFileList.GetHead();
			if (NULL != pOrgFile) {
				int nOrgFileCount = pData->OrgFileList.GetCount();
				if (1 == nOrgFileCount) {
					strTemp.Format(_T("%s"), pOrgFile->strFilePath);
				}
				else {
					strTemp.Format(PRIMX_PROGRESS_MSG2, pOrgFile->strFilePath, (pData->OrgFileList.GetCount() - 1));
				}
			}
			int nIndex = m_ctrlExportList.InsertItem(0, pData->strExportFileName, 1);
			m_ctrlExportList.SetItemText(nIndex, 1, SizeToA(pData->nExportFileSize));
			m_ctrlExportList.SetItemText(nIndex, 2, pData->strCreateDate);
			m_ctrlExportList.SetItemText(nIndex, 3, strTemp);
			m_ctrlExportList.SetItemText(nIndex, 4, SizeToA(pData->nOrgFileTotalSize));
			m_ctrlExportList.SetItemData(nIndex, DWORD_PTR(pData));

		}
	}
}

void CExportFilePage::GetExportFileData(CString strKey)
{
	CFileDBManager FileDBManager;
	FileDBManager.GetExportFileData(m_ExFileList);
	
	POSITION Pos = m_ExFileList.GetHeadPosition();
	while (NULL != Pos) {
		PRIMX_EXPORT_FILE_INFO* pData = m_ExFileList.GetNext(Pos);
		if (NULL != pData) {
			CString strTemp;			
			PRIMX_ORG_FILE_INFO* pOrgFile = pData->OrgFileList.GetHead();
			if (NULL != pOrgFile) {
				int nOrgFileCount = pData->OrgFileList.GetCount();
				if (1 == nOrgFileCount) {
					strTemp.Format(_T("%s"), pOrgFile->strFilePath);
				}
				else {
					strTemp.Format(PRIMX_PROGRESS_MSG2, pOrgFile->strFilePath, (pData->OrgFileList.GetCount()-1));
				}
			}
			int nIndex = m_ctrlExportList.InsertItem(0, pData->strExportFileName, 1);
			m_ctrlExportList.SetItemText(nIndex,1, SizeToA(pData->nExportFileSize));
			m_ctrlExportList.SetItemText(nIndex, 2, pData->strCreateDate);
			m_ctrlExportList.SetItemText(nIndex, 3, strTemp);
			m_ctrlExportList.SetItemText(nIndex, 4, SizeToA(pData->nOrgFileTotalSize));
			m_ctrlExportList.SetItemData(nIndex, DWORD_PTR(pData));

		}
	}
}

void CExportFilePage::OnNMDblclkExportFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if (0 <= pNMItemActivate->iItem) {
		PRIMX_EXPORT_FILE_INFO* pData = (PRIMX_EXPORT_FILE_INFO*)m_ctrlExportList.GetItemData(pNMItemActivate->iItem);
		if (NULL != pData) {
			CExportFileDetailDlg ExportFileDetailDlg;
			ExportFileDetailDlg.SetExportFileData(pData);
			ExportFileDetailDlg.DoModal();
		}
	}

	*pResult = 0;
}
void CExportFilePage::SetExportFileCreate()
{
	OnBnClickedExportFileCreateBtn();
}