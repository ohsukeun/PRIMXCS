// ProgressExDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PRIMXAgentCS.h"
#include "ProgressExDlg.h"
#include "afxdialogex.h"
#include "ExportFileGenerateDlg.h"

// CProgressExDlg 대화 상자입니다.

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


// CProgressExDlg 메시지 처리기입니다.

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

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_ctrlProgress.SetRange(0, m_nMaxRange);
	m_ctrlProgress.SetPos(1);

	Start();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
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