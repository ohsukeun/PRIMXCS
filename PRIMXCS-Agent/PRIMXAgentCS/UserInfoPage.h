#pragma once

#include "CommonModule.h"

// CUserInfoPage 폼 뷰입니다.

class CUserInfoPage : public CFormView , public CCommonModule
{
	DECLARE_DYNCREATE(CUserInfoPage)

protected:
	CUserInfoPage();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CUserInfoPage();

	CBrush						m_brushBack;									/**< 백그라운드 브러쉬	*/
	COLORREF					m_clrBack;									/**< 백그라운드 브러쉬 컬러	*/

	CString GetSystemTypeString(UINT nType);

public:
	void GetUserData();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USER_INFO_PAGE };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_strSystemType;
	CString m_strUserID;
	CString m_strUserName;
	CString m_strTel;
	CString m_strCellPhoneNo;
	CString m_strExtNo;
	CString m_strDeptName;
	CString m_strGradeName;
	CString m_strMailAddr;
	afx_msg void OnBnClickedUserDataRefresh();
};


