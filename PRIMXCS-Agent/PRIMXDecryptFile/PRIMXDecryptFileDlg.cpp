
// PRIMXDecryptFileDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "PRIMXDecryptFile.h"
#include "PRIMXDecryptFileDlg.h"
#include "afxdialogex.h"

#include "CommonModule.h"
#include "ExportFileDecryptManager.h"
#include "CommandExportFileAuth.h"
#include "ZIPFile.h"
#include "SHA512.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CPRIMXDecryptFileDlg 대화 상자



CPRIMXDecryptFileDlg::CPRIMXDecryptFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PRIMXDECRYPTFILE_DIALOG, pParent)
	, m_strMsg(_T(""))
{
	m_bSelfDelete = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_accelerators_key = ::LoadAccelerators(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pExportFileDecryptManager = new CExportFileDecryptManager;
}

CPRIMXDecryptFileDlg::~CPRIMXDecryptFileDlg()
{
	if (NULL != m_pExportFileDecryptManager) {
		delete m_pExportFileDecryptManager;
		m_pExportFileDecryptManager = NULL;
	}

	SetFileLsitRemoveAll();
	SetDeleteFile(m_strZedDllPath);
}

void CPRIMXDecryptFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_LIST, m_ctrlFileList);
	DDX_Text(pDX, IDC_STATIC_MSG, m_strMsg);
	DDX_Control(pDX, IDC_ZED_EXTRACT_FILE_BTN, m_ctrlExtractBtn);
}

BEGIN_MESSAGE_MAP(CPRIMXDecryptFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ZED_EXTRACT_FILE_BTN, &CPRIMXDecryptFileDlg::OnBnClickedZedExtractFileBtn)
	ON_WM_TIMER()
	ON_COMMAND(ID_AC_LIST_SELECT_ALL, &CPRIMXDecryptFileDlg::OnAcListSelectAll)
	ON_BN_CLICKED(IDOK, &CPRIMXDecryptFileDlg::OnBnClickedOk)
	ON_NOTIFY(NM_RCLICK, IDC_FILE_LIST, &CPRIMXDecryptFileDlg::OnNMRClickFileList)
	ON_NOTIFY(NM_DBLCLK, IDC_FILE_LIST, &CPRIMXDecryptFileDlg::OnNMDblclkFileList)
	ON_COMMAND(ID_MENU_EXTRACT_FILE, &CPRIMXDecryptFileDlg::OnMenuExtractFile)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPRIMXDecryptFileDlg 메시지 처리기

BOOL CPRIMXDecryptFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

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

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	
	m_ctrlExtractBtn.EnableWindow(FALSE);

	m_ctrlFileList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS);
	m_ctrlFileList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_ctrlFileList.SetColumnWithImage(IDS_ORG_FILE_LIST_COLUMN2, IDB_EXPORT_FILE_ITEM, 20, 20);

	if (FALSE == SetZedDllFileSave()) {
		ALERT_MSGBOX(IDS_ERROR_STRING6);
		exit(-1);
		return FALSE;
	}
	

	SetTimer(100, 500, 0);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CPRIMXDecryptFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPRIMXDecryptFileDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CPRIMXDecryptFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPRIMXDecryptFileDlg::OnBnClickedZedExtractFileBtn()
{
	SetExtractFile();
}

void CPRIMXDecryptFileDlg::SetExtractFile()
{
	if (0 > m_ctrlFileList.GetSelectionMark()) {
		ALERT_MSGBOX(IDS_ERROR_STRING4);
	}
	else 
	{
		m_strExportFolder = _T("");
		m_ExtractionFileList.RemoveAll();

		CFolderPickerDialog FolderDialog(NULL,OFN_FILEMUSTEXIST,this);
		
		if (IDOK == FolderDialog.DoModal()) {
			m_strExportFolder = FolderDialog.GetFolderPath();
			
			if (-1 == ::GetFileAttributes(m_strExportFolder)) {
				ALERT_MSGBOX(GetLastErrorString());
				return;
			}

			POSITION Pos = m_ctrlFileList.GetFirstSelectedItemPosition();
			while (NULL != Pos) {
				int nItem = m_ctrlFileList.GetNextSelectedItem(Pos);
				PRIMX_ORG_FILE_INFO* pFileData = (PRIMX_ORG_FILE_INFO*)m_ctrlFileList.GetItemData(nItem);
				if (NULL != pFileData) {
					m_ExtractionFileList.AddTail(pFileData->strFilePath);
				}
			}

			int nFileCount = m_ExtractionFileList.GetCount();
			CProgressDlg ProgressDlg(this);
			ProgressDlg.SetProgressType(PRIMX_PROGRESS_TYPE_FILE_DECRYPT);

			if (100 < nFileCount) {
				ProgressDlg.SetMaxRange(nFileCount + (nFileCount / 3.0));
			}

			ProgressDlg.DoModal();
			int nResult = ProgressDlg.m_nResult;

			if (0 == nResult) {
				CString strMsg;
				if (1 == nFileCount) {
					strMsg.Format(IDS_EXTRACT_FILE_OK, m_ExtractionFileList.GetHead());
				}
				else {
					strMsg.Format(IDS_EXTRACT_FILE_OK2, m_ExtractionFileList.GetHead(), nFileCount);
				}
				INFOR_MSGBOX(strMsg);
			}
			else {
				CString strMsg;
				if (1 == nFileCount) {
					strMsg.Format(IDS_EXTRACT_FILE_FAIL, m_ExtractionFileList.GetHead());
				}
				else {
					strMsg.Format(IDS_EXTRACT_FILE_FAIL2, m_ExtractionFileList.GetHead(), nFileCount);
				}
				ALERT_MSGBOX(strMsg);
			}
			m_ExtractionFileList.RemoveAll();
		}
	}

}

void CPRIMXDecryptFileDlg::SetFileLsitRemoveAll()
{
	POSITION Pos = m_FileLsit.GetHeadPosition();

	while (NULL != Pos) {
		PRIMX_ORG_FILE_INFO* pData = m_FileLsit.GetNext(Pos);
		_SafeDelete(pData);
	}
	m_FileLsit.RemoveAll();
}

void CPRIMXDecryptFileDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(100);

	CProgressDlg ProgressDlg(this);
	ProgressDlg.DoModal();
	int nResult = ProgressDlg.m_nResult;
	
	if (0 == nResult) {
		m_ctrlExtractBtn.EnableWindow(TRUE);
	}
	else {
		if (PRIMX_EX_FILE_AUTH_FAIL == nResult || PRIMX_EX_FILE_DATA_DAMAGE_ERROR == nResult) {
			SetSelfDeleteFlag(TRUE);
		}
		UpdateData(FALSE);
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

void CPRIMXDecryptFileDlg::OnAcListSelectAll()
{
	m_ctrlFileList.SelectAll(TRUE);
}


BOOL CPRIMXDecryptFileDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (NULL != m_accelerators_key) {
		if (::TranslateAccelerator(m_hWnd, m_accelerators_key, pMsg)) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

BOOL CPRIMXDecryptFileDlg::SetZedDllFileSave()
{
	BOOL bRe = FALSE;
	CString strZipFilePath, strZipFileFullPath;
	strZipFilePath.Format(_T("%s\\ExFileData"), GetProgramDataPath());
	strZipFileFullPath.Format(_T("%s\\%s.zip"), strZipFilePath, GetCreateKey());

	SetCreateDirectoryEx(strZipFilePath);
	SetDeleteFileWithSubDir(strZipFilePath);
	SetDeleteFile(strZipFileFullPath);

	if (TRUE == SetMakeFile(IDR_ZED_DLL, strZipFileFullPath, _T("ZED_DLL"))) {
		CZIPFile ZIPFile;
		CStringList UnZipFileList;
		CString strLocalPath = GetLocalPath();

		if (TRUE == ZIPFile.DoUnzipFile(strZipFileFullPath, strLocalPath, UnZipFileList)) {
			if (1 == UnZipFileList.GetCount()) {
				CString strFileName = UnZipFileList.GetHead();
				if (0 < strFileName.GetLength()) {
					m_strZedDllPath.Format(_T("%s\\%s"), GetLocalPath(), strFileName);
					if (TRUE == FileExist(m_strZedDllPath)) {
						bRe = TRUE;
					}
				}
			}
		}
	}

	SetDeleteFileWithSubDir(strZipFilePath);

	return bRe;
}

void CPRIMXDecryptFileDlg::SetSelfDeleteFlag(BOOL bFlag)
{
	m_bSelfDelete = bFlag;
}

BOOL CPRIMXDecryptFileDlg::GetSelfDeleteFlag()
{
	return m_bSelfDelete;
}

void CPRIMXDecryptFileDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CPRIMXDecryptFileDlg::OnNMRClickFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (-1 == pNMItemActivate->iItem) return;

	CPoint Pt2;
	GetCursorPos(&Pt2);
	ScreenToClient(&Pt2);

	CMenu menu, *pMenu;

	menu.LoadMenu(IDR_POPUP);

	pMenu = menu.GetSubMenu(0);

	if (NULL == pMenu) return;

	CPoint Pt;
	GetCursorPos(&Pt);

	UINT uMenuID = pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Pt.x, Pt.y, this);

	menu.DestroyMenu();

	*pResult = 0;
}


void CPRIMXDecryptFileDlg::OnNMDblclkFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (-1 == pNMItemActivate->iItem) return;

	SetExtractFile();

	*pResult = 0;
}


void CPRIMXDecryptFileDlg::OnMenuExtractFile()
{
	SetExtractFile();
}


HBRUSH CPRIMXDecryptFileDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushBack;
	}
	return hbr;
}


BOOL CPRIMXDecryptFileDlg::OnEraseBkgnd(CDC* pDC)
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

	return TRUE;
}

void CPRIMXDecryptFileDlg::SetMsg(CString strMsg)
{
	m_strMsg = strMsg;
}

int CPRIMXDecryptFileDlg::SetExtractFile(CProgressDlg* pProgressDlg)
{
	int nRe = 0;
	CString strErrorString;
	if (FALSE == m_pExportFileDecryptManager->SetExtractionFile(m_strExportFolder, m_ExtractionFileList, strErrorString, pProgressDlg)) {
		nRe = PRIMX_EX_FILE_DECRYPT_FAIL;
	}

	return nRe;
}

int CPRIMXDecryptFileDlg::SetAuth(CProgressDlg* pProgressDlg)
{
	int nRe = 0;

	if (NULL != pProgressDlg) {
		pProgressDlg->SetProgressValueEx(5 , GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG1));
	}

	if (FALSE == m_pExportFileDecryptManager->SetInitDll()) {
		if (NULL != pProgressDlg) {
			pProgressDlg->SetProgressValueEx(85, GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG2));
			nRe = PRIMX_EX_FILE_DLL_CREATE_ERROR;
			Sleep(1000 * 1);
		}
	}
	else {
		if (NULL != pProgressDlg) {
			pProgressDlg->SetProgressValueEx(10, GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG3));
		}

		if (TRUE == m_pExportFileDecryptManager->SetAuthDataDecrypt()) {

			CSHA512 SHA512;
			CString strExportFileHash = SHA512.GetShaBase64Data(GetLocalFullPath());

			CCommandExportFileAuth CommandExportFileAuth;
			CommandExportFileAuth.SetConnectTimeOut(10);

			if (NULL != pProgressDlg) {
				pProgressDlg->SetProgressValueEx(45, GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG4));
			}

			if (TRUE == CommandExportFileAuth.SetConnect(m_pExportFileDecryptManager->m_strAuthServer, AtoI(m_pExportFileDecryptManager->m_strAuthServerPort))) 
			{

				BOOL bRe = CommandExportFileAuth.SetExportFileAuth(m_pExportFileDecryptManager->m_strAuthServer,
					AtoI(m_pExportFileDecryptManager->m_strAuthServerPort),
					m_pExportFileDecryptManager->m_strKey,
					m_pExportFileDecryptManager->m_strCompanyID,
					m_pExportFileDecryptManager->m_strAgentID,
					m_pExportFileDecryptManager->m_strUserID,
					strExportFileHash);

				if (TRUE == bRe) {
					if (NULL != pProgressDlg) {
						pProgressDlg->SetProgressValueEx(65, GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG5));
					}
					m_pExportFileDecryptManager->SetZedFileDecrypt();
					m_pExportFileDecryptManager->GetZedFileInfo(m_FileLsit);

					POSITION Pos = m_FileLsit.GetHeadPosition();
					while (NULL != Pos) {
						PRIMX_ORG_FILE_INFO* pFileData = m_FileLsit.GetNext(Pos);
						if (NULL != pFileData) {
							int nItemIndex = m_ctrlFileList.InsertItem(0, pFileData->strFilePath, 0);
							m_ctrlFileList.SetItemText(nItemIndex, 1, SizeToA(pFileData->nFileSize));
							m_ctrlFileList.SetItemData(nItemIndex, DWORD_PTR(pFileData));
						}
					}
				}
				else {
					if (NULL != pProgressDlg) {
						CString strMsg = GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG6);
						CString strFailCode = CommandExportFileAuth.GetFailCode();
						if (0 == strFailCode.CompareNoCase(L"E40002")) strMsg = GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG7);
						if (0 == strFailCode.CompareNoCase(L"E40003")) strMsg = GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG8);
						if (0 == strFailCode.CompareNoCase(L"E40004")) strMsg = GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG9);
						if (0 == strFailCode.CompareNoCase(L"E40005")) strMsg = GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG10);
						if (0 == strFailCode.CompareNoCase(L"E40006")) strMsg = GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG11);
						if (0 == strFailCode.CompareNoCase(L"E40007")) strMsg = GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG12);
						pProgressDlg->SetProgressValueEx(85, strMsg);
						nRe = PRIMX_EX_FILE_AUTH_FAIL;
						Sleep(1000 * 1);
					}
				}
			}
			else 
			{
				if (NULL != pProgressDlg) {
					pProgressDlg->SetProgressValueEx(85, GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG13));
					nRe = PRIMX_EX_FILE_SERVER_CONNECT_FAIL;
					Sleep(1000 * 1);
				}
			}
		}
		else {
			if (NULL != pProgressDlg) {
				pProgressDlg->SetProgressValueEx(85, GetLoadStringTable(IDS_PRIMX_PROGRESS_MSG14));
				nRe = PRIMX_EX_FILE_DATA_DAMAGE_ERROR;
				Sleep(1000 * 1);
			}
		}
	}

	return nRe;
}