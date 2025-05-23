// ProgressDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PRIMXDecryptFile.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"
#include "PRIMXDecryptFileDlg.h"

// CProgressDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROGRESS_DIALOG, pParent)
	, m_strMsg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pMainDlg = (CPRIMXDecryptFileDlg*)pParent;
	m_nProgressValue = 0;
	m_nType = PRIMX_PROGRESS_TYPE_DATA_LOAD;
	m_nMaxRange = 100;

}

CProgressDlg::~CProgressDlg()
{
	WaitForTermination(100);
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
	DDX_Text(pDX, IDC_PRORGESS_MSG, m_strMsg);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
	ON_MESSAGE(PRIMX_WM_UI_PROGRESS, OnProgessValue)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CProgressDlg 메시지 처리기입니다.
LRESULT CProgressDlg::OnProgessValue(WPARAM wParam, LPARAM lParam)
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
	else {
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

BOOL CProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	if (PRIMX_PROGRESS_TYPE_DATA_LOAD == m_nType) {
		CString strTitle;
		strTitle.LoadString(IDS_PRIMX_PROGRESS_TYPE_DATA_LOAD_TITLE);
		SetWindowText(strTitle);
	}
	if (PRIMX_PROGRESS_TYPE_FILE_DECRYPT == m_nType) {
		CString strTitle;
		strTitle.LoadString(IDS_PRIMX_PROGRESS_TYPE_FILE_DECRYPT_TITLE);
		SetWindowText(strTitle);
	}

	m_ctrlProgress.SetRange(0, m_nMaxRange);
	m_ctrlProgress.SetPos(1);

	Start();

	return TRUE;  
}


BOOL CProgressDlg::PreTranslateMessage(MSG* pMsg)
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

void CProgressDlg::SetProgressValueEx(UINT nValue, CString strMsg)
{
	if (0 < strMsg.GetLength()) {
		m_strMsg = strMsg;
		if (NULL != m_pMainDlg) {
			m_pMainDlg->SetMsg(m_strMsg);
		}
	}
	
	::PostMessage(m_hWnd, PRIMX_WM_UI_PROGRESS, nValue, 0);
}

void CProgressDlg::SetMaxRange(short nMaxRange)
{
	if (100 > nMaxRange) {
		m_nMaxRange = 100;
	}
	else {
		m_nMaxRange = nMaxRange;
	}
}

void CProgressDlg::SetProgressType(UINT nType)
{
	m_nType = nType;
}

void CProgressDlg::Run()
{
	if (NULL != m_pMainDlg) {
		if (PRIMX_PROGRESS_TYPE_DATA_LOAD == m_nType) {
			m_nResult = m_pMainDlg->SetAuth(this);
		}
		if (PRIMX_PROGRESS_TYPE_FILE_DECRYPT == m_nType) {
			m_nResult = m_pMainDlg->SetExtractFile(this);
		}
	}

	SetProgressValueEx(99,L"");
	Sleep(500);
	SetProgressValueEx(100, L"");

}


HBRUSH CProgressDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushBack;
	}
	return hbr;
}


BOOL CProgressDlg::OnEraseBkgnd(CDC* pDC)
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
