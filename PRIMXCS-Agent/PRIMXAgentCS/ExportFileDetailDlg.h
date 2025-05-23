#pragma once
#include "afxcmn.h"
#include "ListCtrlEx.h"
#include "CommonModule.h"


// CExportFileDetailDlg 대화 상자입니다.

class CExportFileDetailDlg : public CDialogEx, public CCommonModule
{
	DECLARE_DYNAMIC(CExportFileDetailDlg)

public:
	CExportFileDetailDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CExportFileDetailDlg();


	void SetExportFileData(PRIMX_EXPORT_FILE_INFO* pExportFileData);


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT_FILE_DETAIL_DLG };
#endif

protected:
	HICON						m_hIcon;
	CBrush						m_brushBack;									/**< 백그라운드 브러쉬	*/
	COLORREF					m_clrBack;									/**< 백그라운드 브러쉬 컬러	*/
	PRIMX_EXPORT_FILE_INFO*		m_pExportFileData;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strExFileName;
	CString m_strExFileSize;
	CString m_strExFileCreateDate;
	CListCtrlEx m_ctrlOrgFileList;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
