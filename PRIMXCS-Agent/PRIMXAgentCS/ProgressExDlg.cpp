// ProgressExDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PRIMXAgentCS.h"
#include "ProgressExDlg.h"
#include "afxdialogex.h"
#include "ExportFileGenerateDlg.h"

// CProgressExDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CProgressExDlg, CDialogEx)

CProgressExDlg::CProgressExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROGRESS_DIALOG, pParent)
	, m_strMsg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pMainDlg = (CExportFileGenerateDlg*)pParent;
	m_nProgressValue = 0;
	m_nMaxRange = 100;

}

CProgressExDlg::~CProgressExDlg()
{
	WaitForTermination(100);
}

void CProgressExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PRORGESS_MSG, m_strMsg);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
}


BEGIN_MESSAGE_MAP(CProgressExDlg, CDialogEx)
	ON_MESSAGE(PRIMX_WM_UI_PROGRESS, OnProgessValue)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CProgressExDlg �޽��� ó�����Դϴ�.

LRESULT CProgressExDlg::OnProgessValue(WPARAM wParam, LPARAM lParam)
{
	UINT nValue = UINT(wParam);

	UpdateData(FALSE);

	if (0 == nValue) {
		m_nProgressValue = m_nProgressValue + 1;
		if ((m_nMaxRange * 0.95) <= m_nProgressValue) {
			m_nProgressValue = (m_nMaxRange * 0.95);
		}
		m_ctrlProgress.SetPos(m_nProgressValue);
	}
	else{
		if (100 == nValue) {
			OnOK();
		}
		else {
			m_nProgressValue = float(m_nMaxRange) * (float(nValue) / 100.0);
			m_ctrlProgress.SetPos(m_nProgressValue);
		}
	}
	return 0;
}

BOOL CProgressExDlg::OnInitDialog()
{
	__super::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	m_ctrlProgress.SetRange(0, m_nMaxRange);
	m_ctrlProgress.SetPos(1);

	Start();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


HBRUSH CProgressExDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushBack;
	}
	return hbr;
}


BOOL CProgressExDlg::OnEraseBkgnd(CDC* pDC)
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


BOOL CProgressExDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message) {
		if (VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam) {
			pMsg->message = NULL;
		}
	}
	if (pMsg->message == WM_SYSKEYDOWN  && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

void CProgressExDlg::SetMaxRange(short nMaxRange)
{
	if (100 > nMaxRange) {
		m_nMaxRange = 100;
	}
	else {
		m_nMaxRange = nMaxRange;
	}
}

void CProgressExDlg::SetProgressValue(UINT nValue, CString strMsg)
{
	if (0 < strMsg.GetLength()) {
		m_strMsg = strMsg;
	}

	::PostMessage(m_hWnd, PRIMX_WM_UI_PROGRESS, nValue, 0);
}

CString CProgressExDlg::GetErrorString()
{
	return m_strErrorString;
}

void CProgressExDlg::Run()
{
	if (NULL != m_pMainDlg) {
		m_nResult = m_pMainDlg->SetExportFileCreate(this);
	}	

	SetProgressValue(99, L"");
	Sleep(500);
	SetProgressValue(100, L"");

}