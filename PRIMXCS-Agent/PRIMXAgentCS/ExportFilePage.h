#pragma once
#include "ListCtrlEx.h"
#include "afxcmn.h"
#include "CommonModule.h"

// CExportFilePage 폼 뷰입니다.

class CExportFilePage : public CFormView , public CCommonModule
{
	DECLARE_DYNCREATE(CExportFilePage)

protected:
	CExportFilePage();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CExportFilePage();

	void SetExportFileListRemoveAll();
	void GetExportFileData(CString strKey);

	CBrush						m_brushBack;									/**< 백그라운드 브러쉬	*/
	COLORREF					m_clrBack;									/**< 백그라운드 브러쉬 컬러	*/

	CList<PRIMX_EXPORT_FILE_INFO*>			m_ExFileList;

public:

	void GetExportFileData();
	void SetExportFileCreate();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT_FILE_PAGE };
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
	CListCtrlEx m_ctrlExportList;
	afx_msg void OnBnClickedExportFileCreateBtn();
	afx_msg void OnNMDblclkExportFileList(NMHDR *pNMHDR, LRESULT *pResult);
};


