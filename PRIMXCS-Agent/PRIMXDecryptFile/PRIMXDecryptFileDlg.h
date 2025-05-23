
// PRIMXDecryptFileDlg.h : 헤더 파일
//

#pragma once
#include "CommonModule.h"
#include "ExportFileDecryptManager.h"
#include "afxcmn.h"
#include "ListCtrlEx.h"
#include "afxwin.h"

class CProgressDlg;
// CPRIMXDecryptFileDlg 대화 상자
class CPRIMXDecryptFileDlg : public CDialogEx , public CCommonModule
{
// 생성입니다.
public:
	CPRIMXDecryptFileDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	~CPRIMXDecryptFileDlg();

	BOOL GetSelfDeleteFlag();
	void SetSelfDeleteFlag(BOOL bFlag);
	int SetAuth(CProgressDlg* pProgressDlg);
	int SetExtractFile(CProgressDlg* pProgressDlg);
	void SetMsg(CString strMsg);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRIMXDECRYPTFILE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	void SetFileLsitRemoveAll();
	BOOL SetZedDllFileSave();
	void SetExtractFile();

// 구현입니다.
protected:
	HICON			m_hIcon;
	CBrush			m_brushBack;									/**< 백그라운드 브러쉬	*/
	COLORREF		m_clrBack;									/**< 백그라운드 브러쉬 컬러	*/
	HACCEL			m_accelerators_key;


	CString			m_strExportFolder;
	CStringList		m_ExtractionFileList;


	CExportFileDecryptManager*		m_pExportFileDecryptManager;
	CList<PRIMX_ORG_FILE_INFO*>		m_FileLsit;

	CString							m_strZedDllPath;
	BOOL							m_bSelfDelete;


	// 생성된 메시지 맵 함수
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
