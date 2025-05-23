#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ListCtrlEx.h"
#include "CommonModule.h"

// CUserRegDlg ��ȭ �����Դϴ�.

class CUserRegDlg : public CDialogEx , public CCommonModule
{
	DECLARE_DYNAMIC(CUserRegDlg)

public:
	CUserRegDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUserRegDlg();

	void SetMode(BOOL bInstallMode);
	CString GetUserID();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USER_REG_DLG };
#endif
protected:
	HICON				m_hIcon;
	CBrush				m_brushBack;									/**< ��׶��� �귯��	*/
	COLORREF			m_clrBack;										/**< ��׶��� �귯�� �÷�	*/
	BOOL				m_bInstallMode;
	CString				m_strRegUserID;
	CString				m_strResponseXMLData;

	void SetUserDataListInsert(UINT nResourceID, CString strValue);
	BOOL SetUserRegRequest(CString strUserID);
	BOOL SetSystemUserReg();
	BOOL SetSystemCommonReg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
