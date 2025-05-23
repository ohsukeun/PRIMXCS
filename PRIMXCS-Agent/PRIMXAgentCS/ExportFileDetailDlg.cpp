// ExportFileDetailDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PRIMXAgentCS.h"
#include "ExportFileDetailDlg.h"
#include "afxdialogex.h"


// CExportFileDetailDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CExportFileDetailDlg, CDialogEx)

CExportFileDetailDlg::CExportFileDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXPORT_FILE_DETAIL_DLG, pParent)
	, m_strExFileName(_T(""))
	, m_strExFileSize(_T(""))
	, m_strExFileCreateDate(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pExportFileData = NULL;
}

CExportFileDetailDlg::~CExportFileDetailDlg()
{
}

void CExportFileDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EXPORT_FILE_PATH, m_strExFileName);
	DDX_Text(pDX, IDC_EXPORT_FILE_PATH2, m_strExFileSize);
	DDX_Text(pDX, IDC_EXPORT_FILE_PATH3, m_strExFileCreateDate);
	DDX_Control(pDX, IDC_ORG_FILE_LIST, m_ctrlOrgFileList);
}


BEGIN_MESSAGE_MAP(CExportFileDetailDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CExportFileDetailDlg 메시지 처리기입니다.


BOOL CExportFileDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	
	m_ctrlOrgFileList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS);
	m_ctrlOrgFileList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_ctrlOrgFileList.SetColumnWithImage(IDS_ORG_FILE_LIST_COLUMN2, IDB_EXPORT_FILE_ITEM, 20, 20);


	if (NULL != m_pExportFileData) {
		m_strExFileName = m_pExportFileData->strExportFileName;
		m_strExFileSize = SizeToA(m_pExportFileData->nExportFileSize);
		m_strExFileCreateDate = m_pExportFileData->strCreateDate;

		POSITION Pos = m_pExportFileData->OrgFileList.GetHeadPosition();
		while (NULL != Pos) {
			PRIMX_ORG_FILE_INFO* pOrgFileData = m_pExportFileData->OrgFileList.GetNext(Pos);
			if (NULL != pOrgFileData) {
				int nItemIndex = m_ctrlOrgFileList.InsertItem(0, pOrgFileData->strFilePath, 0);
				m_ctrlOrgFileList.SetItemText(nItemIndex, 1, SizeToA(pOrgFileData->nFileSize));
			}
		}
	}

	UpdateData(FALSE);

	return TRUE;  
}

void CExportFileDetailDlg::SetExportFileData(PRIMX_EXPORT_FILE_INFO* pExportFileData)
{
	m_pExportFileData = pExportFileData;
}


HBRUSH CExportFileDetailDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushBack;
	}

	return hbr;
}


BOOL CExportFileDetailDlg::OnEraseBkgnd(CDC* pDC)
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
