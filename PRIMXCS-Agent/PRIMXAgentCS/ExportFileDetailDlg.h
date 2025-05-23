#pragma once
#include "afxcmn.h"
#include "ListCtrlEx.h"
#include "CommonModule.h"


// CExportFileDetailDlg ��ȭ �����Դϴ�.

class CExportFileDetailDlg : public CDialogEx, public CCommonModule
{
	DECLARE_DYNAMIC(CExportFileDetailDlg)

public:
	CExportFileDetailDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CExportFileDetailDlg();


	void SetExportFileData(PRIMX_EXPORT_FILE_INFO* pExportFileData);


// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT_FILE_DETAIL_DLG };
#endif

protected:
	HICON						m_hIcon;
	CBrush						m_brushBack;									/**< ��׶��� �귯��	*/
	COLORREF					m_clrBack;									/**< ��׶��� �귯�� �÷�	*/
	PRIMX_EXPORT_FILE_INFO*		m_pExportFileData;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
