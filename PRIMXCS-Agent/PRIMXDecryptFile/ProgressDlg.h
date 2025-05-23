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
	CProgressDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
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

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROGRESS_DIALOG };
#endif

protected:
	HICON			m_hIcon;
	CBrush			m_brushBack;								/**< 백그라운드 브러쉬	*/
	COLORREF		m_clrBack;									/**< 백그라운드 브러쉬 컬러	*/

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
