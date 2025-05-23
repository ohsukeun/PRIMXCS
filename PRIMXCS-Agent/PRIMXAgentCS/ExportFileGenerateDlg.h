#pragma once
#include "ListCtrlEx.h"
#include "afxcmn.h"
#include "CommonModule.h"
#include "afxwin.h"
#include "ListDropTarget.h"

#define PRIMX_EX_FILE_MAX_SIZE			(1024*1024*1024)			//1GB


// CExportFileGenerateDlg ��ȭ �����Դϴ�.
class CProgressExDlg;
class CExportFileGenerateDlg : public CDialogEx , public CCommonModule
{
	DECLARE_DYNAMIC(CExportFileGenerateDlg)

public:
	CExportFileGenerateDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CExportFileGenerateDlg();

	BOOL IsFilePathCheck(CString strFilePath);
	void SetOrgFileListRemoveAll(CList<PRIMX_ORG_FILE_INFO*>& OrgFileList);
	int SetExportFileCreate(CProgressExDlg* pProgressExDlg);
	void SetAddFile(CStringList& FileList);
	BOOL IsExistFileData(CString strFilePath);

	CString				m_strExFileKey;


// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT_FILE_GEN_DLG };
#endif
protected:
	HICON			m_hIcon;
	CBrush			m_brushBack;								/**< ��׶��� �귯��	*/
	COLORREF		m_clrBack;									/**< ��׶��� �귯�� �÷�	*/
	HACCEL			m_accelerators_key;
	CListDropTarget m_ListDropTarget;

	CList<PRIMX_ORG_FILE_INFO*>			m_OrgFileList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ctrlOrgFileList;
	afx_msg void OnBnClickedOrgFileAddBtn();
	afx_msg void OnBnClickedExportFilePathBtn();
	CString m_strExportFilePath;
	afx_msg void OnBnClickedOrgFileDelBtn();
	afx_msg void OnLvnKeydownOrgFileList(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedOk();
	afx_msg void OnAcListSelectAll();
	CButton m_ctrlExportFileBTN;
	CButton m_ctrlOrgFileAddBTN;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
