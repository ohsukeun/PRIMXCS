#pragma once
#include "ThreadEx.h"
#include "afxcmn.h"

#define PRIMX_WM_UI_PROGRESS						(WM_USER + 1012)

#define PRIMX_PROGRESS_TYPE_DATA_LOAD				1					
#define PRIMX_PROGRESS_TYPE_FILE_DECRYPT			2	

class CPRIMXDecryptFileDlg;
class CProgressDlg : public CDialogEx, public CThreadEx
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CProgressDlg();

	void SetMaxRange(short nMaxRange);
	void SetProgressValueEx(UINT nValue, CString strMsg);
	void SetProgressType(UINT nType);

	int			m_nResult;

protected:

	void Run();

	CPRIMXDecryptFileDlg*	m_pMainDlg;
	UINT					m_nProgressValue;
	UINT					m_nType;
	short					m_nMaxRange;

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROGRESS_DIALOG };
#endif

protected:
	HICON			m_hIcon;
	CBrush			m_brushBack;								/**< ��׶��� �귯��	*/
	COLORREF		m_clrBack;									/**< ��׶��� �귯�� �÷�	*/

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	LRESULT OnProgessValue(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	CProgressCtrl m_ctrlProgress;
	CString m_strMsg;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
