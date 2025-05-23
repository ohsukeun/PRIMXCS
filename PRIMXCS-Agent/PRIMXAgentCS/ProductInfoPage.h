#pragma once
#include "afxwin.h"
#include "DKStatic.h"


#define PRIMX_TIMER_PRODUCT_CHECK			1002
#define PRIMX_TIMER_PRODUCT_CHECK_TIME		(1000*10)

// CProductInfoPage 폼 뷰입니다.

class CProductInfoPage : public CFormView
{
	DECLARE_DYNCREATE(CProductInfoPage)

protected:
	CProductInfoPage();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CProductInfoPage();

	CBrush						m_brushBack;									/**< 백그라운드 브러쉬	*/
	COLORREF					m_clrBack;									/**< 백그라운드 브러쉬 컬러	*/

	void GetProductData();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRODUCT_INFO_PAGE };
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

	CDKStatic m_ctrlAgentTitle;
	CStatic m_ctrlAgentVersion;
	CStatic m_ctrlAgentID;
	CStatic m_ctrlAgentLastConnectTime;

	CDKStatic m_ctrlProductTitle[3];
	CStatic m_ctrlProductVersion[3];
	
	CString m_strAgentLastConnectTime;
	CString m_strAgentVersion;
	CString m_strAgentID;

	CString m_strProductTitle[3];
	CString m_strProductVersion[3];


	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


