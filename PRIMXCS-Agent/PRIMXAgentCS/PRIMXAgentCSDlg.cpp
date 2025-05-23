
// PRIMXAgentCSDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include <winsvc.h>
#include "PRIMXAgentCS.h"
#include "PRIMXAgentCSDlg.h"
#include "afxdialogex.h"

#include "ProductInfoPage.h"
#include "ExportFilePage.h"
#include "UserInfoPage.h"
#include "TrayRefreshEx.h"
#include "UserRegDlg.h"
#include "ConfigDataManager.h"
#include "IPCChannel.h"
#include "FileDBManager.h"

// 트레이메뉴 position을 위해 정의
enum ENUM_POPUP_POSITION
{
	POPUP_POS_ZC_MENU = 0,
	POPUP_POS_OR_MENU,
	POPUP_POS_CH_MENU,
	POPUP_POS_SEPERATE_3,
	POPUP_POS_CS_SHOW,
	POPUP_POS_SEPERATE_2,
	POPUP_POS_WEB_MANAGER,
	POPUP_POS_SEPERATE_1,
	POPUP_POS_EXPORT_FILE_GEN
};


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const UINT WM_TI_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPRIMXAgentCSDlg 대화 상자



CPRIMXAgentCSDlg::CPRIMXAgentCSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PRIMXAGENTCS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_enSelectMenu = PRIMX_PRODUCT_INFO;
	m_nMainMenuBtnWidth = 0;
	m_nMainMenuBtnHeight = 0;
	m_nMainMenuBtnTop = 0;
	m_nMainMenuBtnLeft = 0;
	m_nBarHeight = 0;
	m_nBarWidth = 0;
	m_bTrayCreateSuccess = FALSE;
	m_hIcons[0] = NULL;
	m_hIcons[1] = NULL;
	m_hIcons[2] = NULL;
	m_bInstallMode = FALSE;
	m_bHideStart = FALSE;

	m_bZonecentrolSetup = FALSE;
	m_bOrizoneSetup = FALSE;
	m_bCryhodSetup = FALSE;

	for (int i = 0; i < _CountOfArray(m_pMainPage); i++) {
		m_pMainPage[i] = NULL;
	}

	InitializeCriticalSectionAndSpinCount(&m_csPolicyDataLock, 4000);
}

CPRIMXAgentCSDlg::~CPRIMXAgentCSDlg()
{
	DeleteCriticalSection(&m_csPolicyDataLock);
	
}

void CPRIMXAgentCSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRODUCT_INFO_BTN, m_ctrlProductInfoBtn);
	DDX_Control(pDX, IDC_EXPORT_FILE_BTN, m_ctrlExportFileBtn);
	DDX_Control(pDX, IDC_USER_INFO_BTN, m_ctrlUserInfoBtn);
}

BEGIN_MESSAGE_MAP(CPRIMXAgentCSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PRODUCT_INFO_BTN, &CPRIMXAgentCSDlg::OnBnClickedProductInfoBtn)
	ON_BN_CLICKED(IDC_EXPORT_FILE_BTN, &CPRIMXAgentCSDlg::OnBnClickedExportFileBtn)
	ON_BN_CLICKED(IDC_USER_INFO_BTN, &CPRIMXAgentCSDlg::OnBnClickedUserInfoBtn)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_TRAY_EVENT, OnTrayMessage)
	ON_REGISTERED_MESSAGE(WM_TI_TASKBARCREATED, OnTaskBarCreated)

	ON_WM_TIMER()
	ON_COMMAND(ID_TRAY_PRIMX_AGENT, &CPRIMXAgentCSDlg::OnTrayPrimxAgent)
	ON_COMMAND(ID_TRAY_WEB_MANAGER_RUN, &CPRIMXAgentCSDlg::OnTrayWebManagerRun)
	ON_COMMAND(ID_ZC_ACCESS_SHOW, &CPRIMXAgentCSDlg::OnZcAccessShow)
	ON_COMMAND(ID_ZC_ALL_CLOSE, &CPRIMXAgentCSDlg::OnZcAllClose)
	ON_COMMAND(ID_ZC_ENCRYPTION_CONFIRM, &CPRIMXAgentCSDlg::OnZcEncryptionConfirm)
	ON_COMMAND(ID_ZC_ENCRYPTION_CREATE, &CPRIMXAgentCSDlg::OnZcEncryptionCreate)
	ON_COMMAND(ID_ZC_ZONE_SHOW, &CPRIMXAgentCSDlg::OnZcZoneShow)
	ON_COMMAND(ID_ZC_OPT_SHOW, &CPRIMXAgentCSDlg::OnZcOptShow)
	ON_COMMAND(ID_OZ_OPEN_FILE_MANAGER, &CPRIMXAgentCSDlg::OnOzOpenFileManager)
	ON_COMMAND(ID_OZ_CHECK_ENCRYPTION, &CPRIMXAgentCSDlg::OnOzCheckEncryption)
	ON_COMMAND(ID_CY_OPEN_CRYPTION_CENTER, &CPRIMXAgentCSDlg::OnCyOpenCryptionCenter)
	ON_COMMAND(ID_TRAY_EXPORT_FILE, &CPRIMXAgentCSDlg::OnTrayExportFile)
END_MESSAGE_MAP()


// CPRIMXAgentCSDlg 메시지 처리기

BOOL CPRIMXAgentCSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"OnInitDialog");

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	m_bInstallMode = FALSE;
	if (1 < __argc) {
		CString strMode = __targv[1];
		strMode.Trim();
		if (0 == strMode.CompareNoCase(_T("-InstallMode"))) {
			m_bInstallMode = TRUE;
		}
		if (0 == strMode.CompareNoCase(_T("-h"))) {
			m_bHideStart = TRUE;
		}
	}

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"OnInitDialog > m_bInstallMode [%lu] > m_bHideStart [%lu]", m_bInstallMode, m_bHideStart);

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_TOP_PRODUCT_NORMAL);

	BITMAP bmp;
	Bitmap.GetObject(sizeof(BITMAP), (LPVOID)&bmp);

	m_nMainMenuBtnWidth = (int)bmp.bmWidth;  // 그림의 폭 
	m_nMainMenuBtnHeight = (int)bmp.bmHeight; // 그림의 넓이
		
	CRect rect;
	m_ctrlProductInfoBtn.GetWindowRect(&rect);
	ScreenToClient(&rect);

	CRect rc;
	GetClientRect(rc);

	m_nMainMenuBtnTop = rect.top;
	m_nMainMenuBtnLeft = rect.left;

	m_ctrlProductInfoBtn.LoadBitmaps(IDB_TOP_PRODUCT_NORMAL, IDB_TOP_PRODUCT_NORMAL, IDB_TOP_PRODUCT_NORMAL, IDB_TOP_PRODUCT_NORMAL);
	m_ctrlProductInfoBtn.SizeToContent();

	m_ctrlExportFileBtn.LoadBitmaps(IDB_TOP_EXPORT_NORMAL, IDB_TOP_EXPORT_NORMAL, IDB_TOP_EXPORT_NORMAL, IDB_TOP_EXPORT_NORMAL);
	m_ctrlExportFileBtn.SizeToContent();

	m_ctrlUserInfoBtn.LoadBitmaps(IDB_TOP_USER_NORMAL, IDB_TOP_USER_NORMAL, IDB_TOP_USER_NORMAL, IDB_TOP_USER_NORMAL);
	m_ctrlUserInfoBtn.SizeToContent();
	
	m_ctrlProductInfoBtn.MoveWindow(m_nMainMenuBtnLeft, m_nMainMenuBtnTop, m_nMainMenuBtnWidth, m_nMainMenuBtnHeight);
	m_ctrlUserInfoBtn.MoveWindow(m_nMainMenuBtnLeft + ((m_nMainMenuBtnWidth)), m_nMainMenuBtnTop, m_nMainMenuBtnWidth, m_nMainMenuBtnHeight);
	m_ctrlExportFileBtn.MoveWindow(m_nMainMenuBtnLeft + ((m_nMainMenuBtnWidth) * 2), m_nMainMenuBtnTop, m_nMainMenuBtnWidth, m_nMainMenuBtnHeight);

	CreatePageView();

	SetMenuButtonSelect(PRIMX_PRODUCT_INFO);
	
	rc.bottom += 100;
	MoveWindow(rc);

	GetPolicyAuthority();

	CString strTitle;
	strTitle.LoadString(IDS_APP_TITLE);
	SetWindowText(strTitle);
		
	if (TRUE == m_bInstallMode) {
		MoveWindow(1, 1, 1, 1, FALSE);
		ShowWindow(SW_SHOWMINIMIZED);
		PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
		CUserRegDlg UserRegDlg;
		UserRegDlg.SetMode(TRUE);
		UserRegDlg.DoModal();
	}
	else {

		CFileDBManager FileDBManager;
		FileDBManager.SetInitDataBaseForExportFile();
		CExportFilePage* pExportFilePage = (CExportFilePage*)m_pMainPage[PRIMX_EXPORT_FILE];
		pExportFilePage->GetExportFileData();

		if (NULL == m_hIcons[0]) m_hIcons[0] = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	//	if (NULL == m_hIcons[1]) m_hIcons[1] = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_LOGOUT));
	//	if (NULL == m_hIcons[2]) m_hIcons[2] = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_ALARM));
		// 트레이 갱신
		UpdateTrayArea();
		CreateTrayIcon();	
		SetTimer(PRIMX_TIMER_USER_ID_CHECK, PRIMX_TIMER_USER_ID_CHECK_TIME, 0);
	}

	if (TRUE == m_bHideStart) {
		ShowWindow(SW_SHOWMINIMIZED);
	//	PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
	}

	CenterWindow(this);

	SetTimer(PRIMX_TIMER_AUTH_CHECK, PRIMX_TIMER_AUTH_CHECK_TIME, 0);

	return TRUE; 
}

void CPRIMXAgentCSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPRIMXAgentCSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);

		CRect rect;
		GetClientRect(rect);

		//Draw title bar
		CDC cdcBg;

		cdcBg.CreateCompatibleDC(&dc);
		cdcBg.SelectObject(&m_bitmapBar);

		for (int i = 0; i < rect.Width(); i = i + m_nBarWidth) {
			dc.BitBlt(i, m_nMainMenuBtnHeight + m_nMainMenuBtnTop - m_nBarHeight, m_nBarWidth, m_nBarHeight, &cdcBg, 0, 0, SRCCOPY);
		}
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CPRIMXAgentCSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CPRIMXAgentCSDlg::SetTrayCreateSuccess(const BOOL & bSet)
{
	m_bTrayCreateSuccess = bSet;
}

BOOL CPRIMXAgentCSDlg::GetTrayCreateSuccess() const
{
	return m_bTrayCreateSuccess;
}

void CPRIMXAgentCSDlg::CreateTrayIcon()
{
	if (TRUE == GetTrayCreateSuccess()){
		return;
	}
	
	ZeroMemory(&m_nid, sizeof(NOTIFYICONDATAEX));

	m_nid.hWnd = m_hWnd;
	m_nid.hIcon = m_hIcons[0];	
	m_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_SHOWTIP;
	m_nid.uID = IDR_MAINFRAME;

	m_nid.uCallbackMessage = WM_TRAY_EVENT;

	wcscpy_s(m_nid.szTip, L"PRIMXAgentCS"); 

	Shell_NotifyIcon(NIM_DELETE, (PNOTIFYICONDATA)&m_nid);

	if (::Shell_NotifyIcon(NIM_ADD, (PNOTIFYICONDATA)&m_nid) == FALSE)
	{
		Shell_NotifyIcon(NIM_DELETE, (PNOTIFYICONDATA)&m_nid);
		SetTrayCreateSuccess(FALSE);
	}
	else {
		SetTrayCreateSuccess(TRUE);
	}

}

void CPRIMXAgentCSDlg::EraseTrayIcon()
{
	Shell_NotifyIcon(NIM_DELETE, (PNOTIFYICONDATA)&m_nid);
	SetTrayCreateSuccess(FALSE);
}

BOOL CPRIMXAgentCSDlg::UpdateTrayArea()
{
	CTrayRefreshEx 	TrayRefreshEx;
	TrayRefreshEx.Refresh();
	return TRUE;
}

LRESULT CPRIMXAgentCSDlg::OnTrayMessage(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(lParam) == WM_LBUTTONDBLCLK){
		ShowWindow(SW_SHOW);
		ShowWindow(SW_RESTORE);
	}
	else if (LOWORD(lParam) == WM_RBUTTONUP)
	{
		// 트레이 메뉴 팝업
		ShowTrayMenu();
	}

	return TRUE;
}

LRESULT CPRIMXAgentCSDlg::OnTaskBarCreated(WPARAM wParam, LPARAM lParam)
{
	EraseTrayIcon();
	CreateTrayIcon();
	return 0;
}

BOOL CPRIMXAgentCSDlg::ShowTrayMenu()
{
	CMenu menu, *pMenu;
	CPoint pt;
	menu.LoadMenu(IDR_MENU_TRAY);
	pMenu = menu.GetSubMenu(0);
	GetCursorPos(&pt);

	SetForegroundWindow();

	// 옵션별, 상태별 보여줘야할 메뉴 조정
	ModifyTrayMenuByOption(pMenu);

	UINT uMenuID = pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
	
	::PostMessage(m_hWnd, WM_NULL, 0, 0); 

	return TRUE;
}

void CPRIMXAgentCSDlg::ModifyTrayMenuByOption(CMenu * pMenu)
{
	if (NULL == pMenu) return;

	if (!(TRUE == m_bZonecentrolSetup && TRUE == m_PolicyData.bAuthExportFile)) {
		pMenu->DeleteMenu(POPUP_POS_EXPORT_FILE_GEN, MF_BYPOSITION);
		pMenu->DeleteMenu(POPUP_POS_SEPERATE_1, MF_BYPOSITION);
	}

	if (FALSE == m_PolicyData.bWebManagerConsoleMenu) {
		pMenu->DeleteMenu(POPUP_POS_WEB_MANAGER, MF_BYPOSITION);
		pMenu->DeleteMenu(POPUP_POS_SEPERATE_2, MF_BYPOSITION);
	}

	if (TRUE == m_PolicyData.bProductMenu) {
		if (FALSE == m_bZonecentrolSetup && FALSE == m_bOrizoneSetup && FALSE == m_bCryhodSetup) pMenu->DeleteMenu(POPUP_POS_SEPERATE_3, MF_BYPOSITION);
		if (FALSE == m_bCryhodSetup) pMenu->DeleteMenu(POPUP_POS_CH_MENU, MF_BYPOSITION);
		if (FALSE == m_bOrizoneSetup) pMenu->DeleteMenu(POPUP_POS_OR_MENU, MF_BYPOSITION);
		if (FALSE == m_bZonecentrolSetup) pMenu->DeleteMenu(POPUP_POS_ZC_MENU, MF_BYPOSITION);
	}
	else {
		pMenu->DeleteMenu(POPUP_POS_SEPERATE_3, MF_BYPOSITION);
		pMenu->DeleteMenu(POPUP_POS_CH_MENU, MF_BYPOSITION);
		pMenu->DeleteMenu(POPUP_POS_OR_MENU, MF_BYPOSITION);
		pMenu->DeleteMenu(POPUP_POS_ZC_MENU, MF_BYPOSITION);
	}

}

CString CPRIMXAgentCSDlg::GetLoadStringTable(UINT nID)
{
	CString strData;
	strData.LoadString(nID);
	return strData;
}

void CPRIMXAgentCSDlg::CreatePageView()
{
	CRect rc;
	GetClientRect(&rc);
	rc.left += 10;
	rc.top += (m_nMainMenuBtnHeight + m_nMainMenuBtnTop);
	rc.right -= 25;
	rc.bottom += (m_nMainMenuBtnHeight + m_nMainMenuBtnTop - 15);

	CCreateContext cc;

	m_pMainPage[PRIMX_PRODUCT_INFO] = (CView*)RUNTIME_CLASS(CProductInfoPage)->CreateObject();
	ZeroMemory(&cc, sizeof(cc));

	CProductInfoPage* pProductInfoPage = (CProductInfoPage*)m_pMainPage[PRIMX_PRODUCT_INFO];
	//pProductInfoPage->SetParentObject(this);

	m_pMainPage[PRIMX_PRODUCT_INFO]->Create(nullptr, nullptr, WS_CHILD, rc, this, IDD_PRODUCT_INFO_PAGE, &cc);
	m_pMainPage[PRIMX_PRODUCT_INFO]->OnInitialUpdate();



	m_pMainPage[PRIMX_EXPORT_FILE] = (CView*)RUNTIME_CLASS(CExportFilePage)->CreateObject();
	ZeroMemory(&cc, sizeof(cc));

	CExportFilePage* pExportFilePage = (CExportFilePage*)m_pMainPage[PRIMX_EXPORT_FILE];
	//pExportFilePage->SetParentObject(this);

	m_pMainPage[PRIMX_EXPORT_FILE]->Create(nullptr, nullptr, WS_CHILD, rc, this, IDD_EXPORT_FILE_PAGE, &cc);
	m_pMainPage[PRIMX_EXPORT_FILE]->OnInitialUpdate();
	

	m_pMainPage[PRIMX_USER_INFO] = (CView*)RUNTIME_CLASS(CUserInfoPage)->CreateObject();
	ZeroMemory(&cc, sizeof(cc));

	CUserInfoPage* pUserInfoPage = (CUserInfoPage*)m_pMainPage[PRIMX_USER_INFO];
	//pUserInfoPage->SetParentObject(this);

	m_pMainPage[PRIMX_USER_INFO]->Create(nullptr, nullptr, WS_CHILD, rc, this, IDD_USER_INFO_PAGE, &cc);
	m_pMainPage[PRIMX_USER_INFO]->OnInitialUpdate();

	

	m_pMainPage[PRIMX_PRODUCT_INFO]->ShowWindow(SW_SHOW);

}

void CPRIMXAgentCSDlg::SetMenuButtonUnSelect(enMENU_TYPE enType)
{
	if (PRIMX_PRODUCT_INFO == enType) {
		m_ctrlProductInfoBtn.LoadBitmaps(IDB_TOP_PRODUCT_NORMAL, IDB_TOP_PRODUCT_NORMAL, IDB_TOP_PRODUCT_NORMAL, IDB_TOP_PRODUCT_NORMAL);
		m_ctrlProductInfoBtn.SizeToContent();
		m_ctrlProductInfoBtn.Invalidate();
	}
	else if (PRIMX_EXPORT_FILE == enType) {
		m_ctrlExportFileBtn.LoadBitmaps(IDB_TOP_EXPORT_NORMAL, IDB_TOP_EXPORT_NORMAL, IDB_TOP_EXPORT_NORMAL, IDB_TOP_EXPORT_NORMAL);
		m_ctrlExportFileBtn.SizeToContent();
		m_ctrlExportFileBtn.Invalidate();
	}
	else if (PRIMX_USER_INFO == enType) {
		m_ctrlUserInfoBtn.LoadBitmaps(IDB_TOP_USER_NORMAL, IDB_TOP_USER_NORMAL, IDB_TOP_USER_NORMAL, IDB_TOP_USER_NORMAL);
		m_ctrlUserInfoBtn.SizeToContent();
		m_ctrlUserInfoBtn.Invalidate();
	}
}

void CPRIMXAgentCSDlg::SetMenuButtonSelect(enMENU_TYPE enType)
{
	SetMenuButtonUnSelect(m_enSelectMenu);

	m_enSelectMenu = enType;

	if (PRIMX_PRODUCT_INFO == enType) {
		m_ctrlProductInfoBtn.LoadBitmaps(IDB_TOP_PRODUCT_SEL, IDB_TOP_PRODUCT_SEL, IDB_TOP_PRODUCT_SEL, IDB_TOP_PRODUCT_SEL);
		m_ctrlProductInfoBtn.SizeToContent();
		m_ctrlProductInfoBtn.Invalidate();
	}
	else if (PRIMX_EXPORT_FILE == enType) {
		m_ctrlExportFileBtn.LoadBitmaps(IDB_TOP_EXPORT_SEL, IDB_TOP_EXPORT_SEL, IDB_TOP_EXPORT_SEL, IDB_TOP_EXPORT_SEL);
		m_ctrlExportFileBtn.SizeToContent();
		m_ctrlExportFileBtn.Invalidate();
	}
	else if (PRIMX_USER_INFO == enType) {
		m_ctrlUserInfoBtn.LoadBitmaps(IDB_TOP_USER_SEL, IDB_TOP_USER_SEL, IDB_TOP_USER_SEL, IDB_TOP_USER_SEL);
		m_ctrlUserInfoBtn.SizeToContent();
		m_ctrlUserInfoBtn.Invalidate();
	}
	
	for (int i = 0; i < _CountOfArray(m_pMainPage); i++) {
		if (NULL != m_pMainPage[i]) {
			if (i == m_enSelectMenu) {
				m_pMainPage[i]->ShowWindow(SW_SHOW);
			}
			else {
				m_pMainPage[i]->ShowWindow(SW_HIDE);
			}
		}
	}

}


void CPRIMXAgentCSDlg::OnBnClickedProductInfoBtn()
{
	SetMenuButtonSelect(PRIMX_PRODUCT_INFO);
}


void CPRIMXAgentCSDlg::OnBnClickedExportFileBtn()
{
	SetMenuButtonSelect(PRIMX_EXPORT_FILE);
}


void CPRIMXAgentCSDlg::OnBnClickedUserInfoBtn()
{
	SetMenuButtonSelect(PRIMX_USER_INFO);
}


int CPRIMXAgentCSDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bitmapBar.LoadBitmap(IDB_SIDE);

	BITMAP bmp;
	m_bitmapBar.GetObject(sizeof(BITMAP), (LPVOID)&bmp);

	m_nBarWidth = (int)bmp.bmWidth;  // 그림의 폭 
	m_nBarHeight = (int)bmp.bmHeight; // 그림의 넓이

	return 0;
}


HBRUSH CPRIMXAgentCSDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode(TRANSPARENT);
	hbr = m_brushBack;

	return hbr;
}


void CPRIMXAgentCSDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	for (int i = 0; i < _CountOfArray(m_pMainPage); i++) {
		if (NULL != m_pMainPage[i]) {
			m_pMainPage[i]->DestroyWindow();
			m_pMainPage[i] = NULL;
		}
	}

	UpdateTrayArea();
	EraseTrayIcon();

}


BOOL CPRIMXAgentCSDlg::OnEraseBkgnd(CDC* pDC)
{
	COLORREF clrBack = RGB(255, 255, 255);

	if (!m_brushBack.GetSafeHandle() || clrBack != m_clrBack)
	{
		m_brushBack.DeleteObject();
		m_brushBack.CreateSolidBrush(clrBack);
		m_clrBack = clrBack;
	}

	CRect rect;
	GetClientRect(&rect);

	pDC->FillSolidRect(rect, m_clrBack);

	return TRUE; // CDialogEx::OnEraseBkgnd(pDC);
}


BOOL CPRIMXAgentCSDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
	{
		return FALSE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CPRIMXAgentCSDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CDialogEx::OnClose();

	if (FALSE == GetTrayCreateSuccess())
		CreateTrayIcon();

	BOOL bRet = ShowWindow(SW_HIDE);
}

void CPRIMXAgentCSDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (PRIMX_TIMER_USER_ID_CHECK == nIDEvent) {

		KillTimer(PRIMX_TIMER_USER_ID_CHECK);

		CConfigDataManager ConfigDataManager;
		CString strUserID = ConfigDataManager.GetUserID();
		UINT nType = ConfigDataManager.GetSystemType();

		if (0 == strUserID.GetLength()) {
			if (!(PRIMX_SYS_TYPE_COMM_SETUP == nType || PRIMX_SYS_TYPE_COMM == nType)) {
				CUserRegDlg UserRegDlg;
				UserRegDlg.SetMode(FALSE);
				if (IDOK == UserRegDlg.DoModal()) {
					CUserInfoPage* pUserInfoPage = (CUserInfoPage*)m_pMainPage[PRIMX_USER_INFO];
					if (NULL != pUserInfoPage) {
						pUserInfoPage->GetUserData();
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"사용자 정보 등록 > 등록 정보 표시");
					}
				}
			}
		}
		SetTimer(PRIMX_TIMER_USER_ID_CHECK, PRIMX_TIMER_USER_ID_CHECK_TIME,0);
	}

	if (PRIMX_TIMER_AUTH_CHECK == nIDEvent) {
		KillTimer(PRIMX_TIMER_AUTH_CHECK);
		GetPolicyAuthority();
		SetTimer(PRIMX_TIMER_AUTH_CHECK, PRIMX_TIMER_AUTH_CHECK_TIME, 0);
	}

	CDialogEx::OnTimer(nIDEvent);
}
DWORD CPRIMXAgentCSDlg::GetRegQueryValueDword(HKEY hKey, CString strValueName)
{
	DWORD dwData = 0;
	DWORD dwBufferSize = sizeof(dwData);
	LSTATUS nResult = RegQueryValueEx(hKey, strValueName, NULL, NULL, (LPBYTE)&dwData, &dwBufferSize);
	if (ERROR_SUCCESS != nResult) {
		dwData = 0;
	}
	return dwData;
}
CString CPRIMXAgentCSDlg::GetRegQueryValueString(HKEY hKey, CString strValueName)
{
	CString strData;
	DWORD dwBufferSize = 0;
	LSTATUS nResult = RegQueryValueEx(hKey, strValueName, NULL, NULL, NULL, &dwBufferSize);
	if (ERROR_SUCCESS == nResult) {
		TCHAR *pBuffer = new TCHAR[dwBufferSize + 1];
		memset(pBuffer, 0, (dwBufferSize + 1) * sizeof(TCHAR));
		nResult = RegQueryValueEx(hKey, strValueName, NULL, NULL, (LPBYTE)pBuffer, &dwBufferSize);
		if (ERROR_SUCCESS == nResult) {
			strData = pBuffer;
			strData.Trim();
		}
		delete[] pBuffer;
		pBuffer = NULL;
	}
	return strData;
}

CString CPRIMXAgentCSDlg::GetOZAccountsGUID()
{
	CString strKey;

	HKEY hKey = HKEY_CURRENT_USER;
	if (ERROR_SUCCESS == RegOpenCurrentUser(KEY_READ, &hKey)) {
		HKEY phkResult = NULL;
		CString strSubkey;
		strSubkey.Format(_T("Software\\Prim'X\\Orizon\\%s\\Accounts"), m_strOrizoneVersion);

		if (ERROR_SUCCESS == ::RegOpenKeyEx(hKey, strSubkey, 0, KEY_READ, &phkResult)) {
			TCHAR SubKey[MAX_PATH + 1] = { 0 };
			DWORD dwSize = MAX_PATH;
			DWORD dwCount = 0;
			LONG lResult = RegEnumKey(phkResult, dwCount, SubKey, dwSize);
			if (ERROR_SUCCESS == lResult) {
				strKey = SubKey;
			}
			::RegCloseKey(phkResult);
		}
		::RegCloseKey(hKey);
	}

	return strKey;
}

void CPRIMXAgentCSDlg::GetPRIMXProductInstallInfo()
{
	HKEY hKey;
	DWORD	dwCnt = 0;
	CString strPath = _T("software\\microsoft\\windows\\currentversion\\uninstall");

	LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, strPath, 0, KEY_READ, &hKey);

	if (ERROR_SUCCESS != lResult) {
		return;
	}
	
	while (TRUE) {
		// 키 나열
		TCHAR SubKey[MAX_PATH + 1] = { 0 };

		FILETIME fileTime;
		DWORD dwSize = MAX_PATH + 1;

		lResult = ::RegEnumKeyEx(hKey, dwCnt, SubKey, &dwSize, NULL, NULL, NULL, &fileTime);

		if (ERROR_SUCCESS != lResult) {
			break;
		}
		else {
			CString strKeyPath;
			CString strDisplayName;
			strKeyPath.Format(_T("%s\\%s"), strPath, SubKey);

			HKEY hSubKey;
			LONG nRe = RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKeyPath, 0, KEY_READ, &hSubKey);

			if (ERROR_SUCCESS == nRe) {

				CString strDisplayName = GetRegQueryValueString(hSubKey, _T("DisplayName"));

				if (0 == strDisplayName.CompareNoCase(_T("ORIZON"))) {
					m_bOrizoneSetup = TRUE;
					m_strOrizonePath = GetRegQueryValueString(hSubKey, _T("InstallLocation"));
					m_strOrizoneVersion.Format(_T("%lu.%lu"), GetRegQueryValueDword(hSubKey, _T("VersionMajor")), GetRegQueryValueDword(hSubKey, _T("VersionMinor")));					
				}
				else if (0 == strDisplayName.CompareNoCase(_T("ZONECENTRAL"))) {
					m_bZonecentrolSetup = TRUE;
					m_strZonecentrolPath = GetRegQueryValueString(hSubKey, _T("InstallLocation"));
				}
				else if (0 == strDisplayName.CompareNoCase(_T("CRYHOD"))) {
					m_bCryhodSetup = TRUE;
					m_strCryhodPath = GetRegQueryValueString(hSubKey, _T("InstallLocation"));
				}

				RegCloseKey(hSubKey);
			}
		}
		++dwCnt;
	}
	RegCloseKey(hKey);

}

BOOL CPRIMXAgentCSDlg::CheckPRIMXService(CString strServiceName)
{
	BOOL bResult = FALSE;

	SC_HANDLE schSCManager = OpenSCManagerW(NULL, NULL, GENERIC_READ);
	if (schSCManager != NULL)
	{
		SC_HANDLE schService = OpenServiceW(schSCManager, strServiceName, GENERIC_READ);
		if (schService != NULL)
		{
			CloseServiceHandle(schService);
			bResult = TRUE;
		}
		CloseServiceHandle(schSCManager);
	}
	return bResult;
}

void CPRIMXAgentCSDlg::GetPolicyAuthority()
{
	EnterCriticalSection(&m_csPolicyDataLock);

	CConfigDataManager ConfigDataManager;
	ConfigDataManager.GetPolicyData(m_PolicyData);
	GetPRIMXProductInstallInfo();

	m_bZonecentrolSetup = CheckPRIMXService(L"ZCP");
	m_bOrizoneSetup = CheckPRIMXService(L"OZS");

#ifdef _DEBUG
	m_ctrlExportFileBtn.ShowWindow(SW_SHOW);
#else
	if (FALSE == m_bZonecentrolSetup || FALSE == m_PolicyData.bAuthExportFile) {
		m_ctrlExportFileBtn.ShowWindow(SW_HIDE);
		if (PRIMX_EXPORT_FILE == m_enSelectMenu) {
			SetMenuButtonSelect(PRIMX_PRODUCT_INFO);
		}
	}
	else {
		if (TRUE == m_bZonecentrolSetup && TRUE == m_PolicyData.bAuthExportFile) {
			m_ctrlExportFileBtn.ShowWindow(SW_SHOW);
		}		
	}
#endif

	LeaveCriticalSection(&m_csPolicyDataLock);
}

void CPRIMXAgentCSDlg::OnTrayPrimxAgent()
{
	ShowWindow(SW_SHOW);
	ShowWindow(SW_RESTORE);
}


void CPRIMXAgentCSDlg::OnTrayWebManagerRun()
{
	EnterCriticalSection(&m_csPolicyDataLock);
	CString strWebManager = m_PolicyData.strAuthWebManagerURL;
	LeaveCriticalSection(&m_csPolicyDataLock);
	
	strWebManager.Trim();
	if (0 < strWebManager.GetLength()) {
		// 기본 브라우저 경로 획득
		DWORD dwSize = 1024;
		TCHAR cBrowserPath[1024] = { 0 };
		HRESULT res = AssocQueryString(0, ASSOCSTR_EXECUTABLE, _T(".htm"), _T("Open"), cBrowserPath, &dwSize);
		if (S_OK == res){
			ShellExecute(NULL, L"open", cBrowserPath, (LPTSTR)(LPCTSTR)strWebManager, NULL, SW_SHOWNORMAL);
		}
		else{
			ShellExecute(NULL, L"open", L"iexplore.exe", (LPTSTR)(LPCTSTR)strWebManager, NULL, SW_SHOWNORMAL);
		}
	}
	else {
		ALERT_MSGBOX(IDS_ERROR_STRING7);
	}

}

BOOL CPRIMXAgentCSDlg::SetCreateProcesseEx(CString strProcess, WORD wShowWindow)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
	si.wShowWindow = SW_SHOW;

	if (!CreateProcess(NULL,   // No module name (use command line)
		(LPTSTR)LPCTSTR(strProcess),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		return FALSE;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return TRUE;
}

void CPRIMXAgentCSDlg::OnZcZoneShow()
{
	CString strCommand;
	strCommand.Format(_T("\"%s\\zcgu.exe\" -p 1"), m_strZonecentrolPath);
	SetCreateProcesseEx(strCommand);
}

void CPRIMXAgentCSDlg::OnZcAccessShow()
{
	CString strCommand;
	strCommand.Format(_T("\"%s\\zcgu.exe\" -p 2"), m_strZonecentrolPath);
	SetCreateProcesseEx(strCommand);

}

void CPRIMXAgentCSDlg::OnZcOptShow()
{
	CString strCommand;
	strCommand.Format(_T("\"%s\\zcgu.exe\" -p 3"), m_strZonecentrolPath);
	SetCreateProcesseEx(strCommand);
}

void CPRIMXAgentCSDlg::OnZcEncryptionCreate()
{
	CString strCommand;
	strCommand.Format(_T("\"%s\\zcapply.exe\" -encrypt"), m_strZonecentrolPath);
	SetCreateProcesseEx(strCommand);
}

void CPRIMXAgentCSDlg::OnZcAllClose()
{
	CString strCommand;
	strCommand.Format(_T("\"%s\\zcucmd.exe\" ck -all"), m_strZonecentrolPath);
	SetCreateProcesseEx(strCommand, SW_HIDE);
	Sleep(1000);
	strCommand.Format(_T("\"%s\\zcucmd.exe\" cz - all"), m_strZonecentrolPath);
	SetCreateProcesseEx(strCommand, SW_HIDE);
}

void CPRIMXAgentCSDlg::OnZcEncryptionConfirm()
{
	CString strCommand;
	strCommand.Format(_T("\"%s\\zcapply.exe\" -applydirectives"), m_strZonecentrolPath);
	SetCreateProcesseEx(strCommand);
}

void CPRIMXAgentCSDlg::OnOzOpenFileManager()
{
	CString strCommand;
	strCommand.Format(_T("\"%s\\orizon.exe\""), m_strOrizonePath);
	SetCreateProcesseEx(strCommand);
}

void CPRIMXAgentCSDlg::OnOzCheckEncryption()
{
	CString strCommand;
	strCommand.Format(_T("\"%s\\orizon.exe\" -checkdirectives -account %s"), m_strOrizonePath, m_strOZAccountsGUID);
	SetCreateProcesseEx(strCommand);
}

void CPRIMXAgentCSDlg::OnCyOpenCryptionCenter()
{
	CString strCommand;
	strCommand.Format(_T("\"%s\\cycenter.exe\""), m_strCryhodPath);
	SetCreateProcesseEx(strCommand);
}

void CPRIMXAgentCSDlg::OnTrayExportFile()
{
	OnTrayPrimxAgent();

	SetMenuButtonSelect(PRIMX_EXPORT_FILE);

	((CExportFilePage*)m_pMainPage[PRIMX_EXPORT_FILE])->SetExportFileCreate();

}
