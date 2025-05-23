#pragma once
#include "ThreadEx.h"
#include "afxcmn.h"

#define PRIMX_WM_UI_PROGRESS						(WM_USER + 1012)


class CExportFileGenerateDlg;
class CProgressExDlg : public CDialogEx, public CThreadEx
{
	DECLARE_DYNAMIC(CProgressExDlg)

public:
	CProgressExDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CProgressExDlg();

	void SetMaxRange(short nMaxRange);
	void SetProgressValue(UINT nValue, CString strMsg);
	CString GetErrorString();

	int			m_nResult;

protected:
	HICON			m_hIcon;
	CBrush			m_brushBack;									/**< ��׶��� �귯��	*/
	COLORREF		m_clrBack;									/**< ��׶��� �귯�� �÷�	*/

	void Run();

	UINT					m_nType;
	CExportFileGenerateDlg*	m_pMainDlg;
	UINT					m_nProgressValue;
	CString					m_strErrorString;
	short					m_nMaxRange;

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROGRESS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	LRESULT OnProgessValue(WPARAM wParam, LPARAM lParam);

public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CString m_strMsg;
	CProgressCtrl m_ctrlProgress;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
