#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ListCtrlEx.h"
#include "CommonModule.h"

// CUserRegDlg 대화 상자입니다.

class CUserRegDlg : public CDialogEx , public CCommonModule
{
	DECLARE_DYNAMIC(CUserRegDlg)

public:
	CUserRegDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserRegDlg();

	void SetMode(BOOL bInstallMode);
	CString GetUserID();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USER_REG_DLG };
#endif
protected:
	HICON				m_hIcon;
	CBrush				m_brushBack;									/**< 백그라운드 브러쉬	*/
	COLORREF			m_clrBack;										/**< 백그라운드 브러쉬 컬러	*/
	BOOL				m_bInstallMode;
	CString				m_strRegUserID;
	CString				m_strResponseXMLData;

	void SetUserDataListInsert(UINT nResourceID, CString strValue);
	BOOL SetUserRegRequest(CString strUserID);
	BOOL SetSystemUserReg();
	BOOL SetSystemCommonReg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strUserID;
	CEdit m_ctrlUserID;
	CButton m_ctrlUserSearch;
	afx_msg void OnBnClickedUserPcRadio();
	afx_msg void OnBnClickedCommPcRadio();
	int m_nUserPC;
	afx_msg void OnBnClickedUserSearchBtn();
	CListCtrlEx m_ctrlUserDataList;
};
