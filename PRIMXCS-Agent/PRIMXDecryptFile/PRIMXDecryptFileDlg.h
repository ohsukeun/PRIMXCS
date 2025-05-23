
// PRIMXDecryptFileDlg.h : ��� ����
//

#pragma once
#include "CommonModule.h"
#include "ExportFileDecryptManager.h"
#include "afxcmn.h"
#include "ListCtrlEx.h"
#include "afxwin.h"

class CProgressDlg;
// CPRIMXDecryptFileDlg ��ȭ ����
class CPRIMXDecryptFileDlg : public CDialogEx , public CCommonModule
{
// �����Դϴ�.
public:
	CPRIMXDecryptFileDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CPRIMXDecryptFileDlg();

	BOOL GetSelfDeleteFlag();
	void SetSelfDeleteFlag(BOOL bFlag);
	int SetAuth(CProgressDlg* pProgressDlg);
	int SetExtractFile(CProgressDlg* pProgressDlg);
	void SetMsg(CString strMsg);

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRIMXDECRYPTFILE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.
	void SetFileLsitRemoveAll();
	BOOL SetZedDllFileSave();
	void SetExtractFile();

// �����Դϴ�.
protected:
	HICON			m_hIcon;
	CBrush			m_brushBack;									/**< ��׶��� �귯��	*/
	COLORREF		m_clrBack;									/**< ��׶��� �귯�� �÷�	*/
	HACCEL			m_accelerators_key;


	CString			m_strExportFolder;
	CStringList		m_ExtractionFileList;


	CExportFileDecryptManager*		m_pExportFileDecryptManager;
	CList<PRIMX_ORG_FILE_INFO*>		m_FileLsit;

	CString							m_strZedDllPath;
	BOOL							m_bSelfDelete;


	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedZedExtractFileBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListCtrlEx m_ctrlFileList;
	afx_msg void OnAcListSelectAll();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMRClickFileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkFileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuExtractFile();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CString m_strMsg;
	CButton m_ctrlExtractBtn;
};
