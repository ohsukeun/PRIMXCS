
// PRIMXAgentCSDlg.h : 헤더 파일
//

#pragma once
#include "afxext.h"
#include "afxwin.h"

#define PRIMX_TIMER_USER_ID_CHECK			1001
#define PRIMX_TIMER_USER_ID_CHECK_TIME		(1000*5)

#define PRIMX_TIMER_AUTH_CHECK				1002
#define PRIMX_TIMER_AUTH_CHECK_TIME			(1000*10)


enum enMENU_TYPE
{
	PRIMX_PRODUCT_INFO = 0,
	PRIMX_USER_INFO,
	PRIMX_EXPORT_FILE
};

// 트레이 구조체
struct NOTIFYICONDATAEX
{
	DWORD cbSize;          // Size of this structure, in bytes.
	HWND hWnd;             // Handle to the window that will receive notification messages associated with an icon in the taskbar status area. The shell uses hWnd and uID to identify which icon is to be operated on when Shell_NotifyIcon is invoked.
	UINT uID;              // Application-defined identifier of the taskbar icon. The shell uses hWnd and uID to identify which icon is to be operated on when Shell_NotifyIcon is invoked. You can have multiple icons associated with a single hWnd by assigning each a different uID.
	UINT uFlags;           // Array of flags that indicate which of the other members contain valid data
	UINT uCallbackMessage; // Application-defined message identifier
	HICON hIcon;           // Handle to the icon to be added, modified, or deleted.
	TCHAR szTip[128];      // Pointer to a NULL-terminated string with the text for a standard tooltip.
	DWORD dwState;         // State of the icon
	DWORD dwStateMask;     // A value that specifies which bits of the state member will be retrieved or modified.
	TCHAR szInfo[256];     // Pointer to a NULL-terminated string with the text for a balloon-style tooltip
	union {
		UINT  uTimeout;    // The timeout value, in milliseconds, for a balloon-style tooltip.
		UINT  uVersion;    // Used to specify whether the shell notify icon interface should use Windows 95 or Windows 2000 behavior.
	} DUMMYUNIONNAME;
	TCHAR szInfoTitle[64]; // Pointer to a NULL-terminated string containing a title for a balloon tooltip.
	DWORD dwInfoFlags;     // Flags that can be set to add an icon to a balloon tooltip
};

class CPRIMXAgentCSDlg : public CDialogEx
{
// 생성입니다.
public:
	CPRIMXAgentCSDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	~CPRIMXAgentCSDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRIMXAGENTCS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	
	BOOL m_bInstallMode;
	BOOL m_bHideStart;

// 구현입니다.
protected:
	HICON m_hIcon;

	// 트레이 아이콘 관련
	HICON m_hIcons[3];
	NOTIFYICONDATAEX m_nid;
	BOOL m_bTrayCreateSuccess;
	void SetTrayCreateSuccess(const BOOL &bSet);
	BOOL GetTrayCreateSuccess() const;

	void CreateTrayIcon();
	void EraseTrayIcon();
	BOOL ShowTrayMenu();
	void ModifyTrayMenuByOption(CMenu *pMenu);
	BOOL UpdateTrayArea();
	
	CString GetLoadStringTable(UINT nID);

	void SetMenuButtonSelect(enMENU_TYPE enType);
	void SetMenuButtonUnSelect(enMENU_TYPE enType);
	void CreatePageView();
	void GetPolicyAuthority();
	void GetPRIMXProductInstallInfo();
	BOOL CheckPRIMXService(CString strServiceName);
	CString GetRegQueryValueString(HKEY hKey, CString strValueName);
	DWORD GetRegQueryValueDword(HKEY hKey, CString strValueName);
	CString GetOZAccountsGUID();
	BOOL SetCreateProcesseEx(CString strProcess, WORD wShowWindow = SW_SHOW);
	

	enMENU_TYPE			m_enSelectMenu;
	CBrush				m_brushBack;									/**< 백그라운드 브러쉬	*/
	COLORREF			m_clrBack;										/**< 백그라운드 브러쉬 컬러	*/
	int					m_nMainMenuBtnWidth;
	int					m_nMainMenuBtnHeight;

	int					m_nMainMenuBtnTop;
	int					m_nMainMenuBtnLeft;
	CBitmap				m_bitmapBar;
	int					m_nBarHeight;
	int					m_nBarWidth;

	CView*				m_pMainPage[3];
	
	CRITICAL_SECTION	m_csPolicyDataLock;

	PRIMX_POLICY_DATA	m_PolicyData;
	BOOL				m_bZonecentrolSetup;
	BOOL				m_bOrizoneSetup;
	BOOL				m_bCryhodSetup;
	
	CString				m_strZonecentrolPath;
	CString				m_strOrizonePath;
	CString				m_strCryhodPath;

	CString				m_strOZAccountsGUID;
	CString				m_strOrizoneVersion;

	// 생성된 메시지 맵 함수
	LRESULT OnTaskBarCreated(WPARAM wParam, LPARAM lParam);
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_ctrlProductInfoBtn;
	CBitmapButton m_ctrlExportFileBtn;
	CBitmapButton m_ctrlUserInfoBtn;
	afx_msg void OnBnClickedProductInfoBtn();
	afx_msg void OnBnClickedExportFileBtn();
	afx_msg void OnBnClickedUserInfoBtn();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	LRESULT OnTrayMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTrayPrimxAgent();
	afx_msg void OnTrayWebManagerRun();
	afx_msg void OnZcAccessShow();
	afx_msg void OnZcAllClose();
	afx_msg void OnZcEncryptionConfirm();
	afx_msg void OnZcEncryptionCreate();
	afx_msg void OnZcZoneShow();
	afx_msg void OnZcOptShow();
	afx_msg void OnOzOpenFileManager();
	afx_msg void OnOzCheckEncryption();
	afx_msg void OnCyOpenCryptionCenter();
	afx_msg void OnTrayExportFile();
};
