// UserRegDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PRIMXAgentCS.h"
#include "UserRegDlg.h"
#include "afxdialogex.h"
#include "CommandUserSearch.h"
#include "ConfigDataManager.h"
#include "IPCChannel.h"


// CUserRegDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserRegDlg, CDialogEx)

CUserRegDlg::CUserRegDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_USER_REG_DLG, pParent)
	, m_strUserID(_T(""))
	, m_nUserPC(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInstallMode = FALSE;
}

CUserRegDlg::~CUserRegDlg()
{
}

void CUserRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USER_ID, m_strUserID);
	DDX_Control(pDX, IDC_USER_ID, m_ctrlUserID);
	DDX_Control(pDX, IDC_USER_SEARCH_BTN, m_ctrlUserSearch);
	DDX_Radio(pDX, IDC_USER_PC_RADIO, m_nUserPC);
	DDX_Control(pDX, IDC_USER_INFO_LIST, m_ctrlUserDataList);
}


BEGIN_MESSAGE_MAP(CUserRegDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CUserRegDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUserRegDlg::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_USER_PC_RADIO, &CUserRegDlg::OnBnClickedUserPcRadio)
	ON_BN_CLICKED(IDC_COMM_PC_RADIO, &CUserRegDlg::OnBnClickedCommPcRadio)
	ON_BN_CLICKED(IDC_USER_SEARCH_BTN, &CUserRegDlg::OnBnClickedUserSearchBtn)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CUserRegDlg 메시지 처리기입니다.

void CUserRegDlg::SetMode(BOOL bInstallMode)
{
	m_bInstallMode = bInstallMode;
}

void CUserRegDlg::OnBnClickedOk()
{
	UpdateData();

	if (0 == m_nUserPC) {
		if (TRUE == SetSystemUserReg()) {

			CDialogEx::OnOK();
		}
		else {
			return;
		}

	}
	else {
		if (TRUE == SetSystemCommonReg()) {
			CDialogEx::OnOK();
		}
		else {
			return;
		}
	}

}

BOOL CUserRegDlg::SetSystemUserReg()
{
	UpdateData();
	BOOL bRe = FALSE;
	m_strRegUserID.Trim();

	if (0 == m_strRegUserID.GetLength()) {
		ALERT_MSGBOX(IDS_ERROR_STRING4);
		m_ctrlUserID.SetSel(0, -1);
		m_ctrlUserID.SetFocus();
		return bRe;
	}
	else {
		CString strMsg;
		strMsg.Format(IDS_USER_DATA_REG_MESSAGE, m_strRegUserID);
		if (IDYES == QUESTION_MSGBOX(strMsg)) {
			if (TRUE == m_bInstallMode) {
				CConfigDataManager ConfigDataManager;
				if (FALSE == ConfigDataManager.SetUserData(m_strResponseXMLData)) {
					SetUserData(m_strResponseXMLData);
				}
				ConfigDataManager.SetSystemType(PRIMX_SYS_TYPE_USER_SETUP); 
				ConfigDataManager.SetUserID(m_strRegUserID);
				exit(100);
			}
			else {
				if (TRUE == SetUserRegRequest(m_strRegUserID)) {
					CConfigDataManager ConfigDataManager;
					if (FALSE == ConfigDataManager.SetUserData(m_strResponseXMLData)) {
						SetUserData(m_strResponseXMLData);
					}
					bRe = TRUE;
				}
			}
		}
		else {
			m_strRegUserID = _T("");
		}
	}
	return bRe;
}

BOOL CUserRegDlg::SetSystemCommonReg()
{
	UpdateData();
	BOOL bRe = FALSE;

	CString strMsg;
	strMsg.LoadString(IDS_USER_DATA_REG_MESSAGE2);
	if (IDYES == QUESTION_MSGBOX(strMsg)) {
		if (TRUE == m_bInstallMode) {
			CConfigDataManager ConfigDataManager;
			ConfigDataManager.SetUserIDRemove();
			ConfigDataManager.SetSystemType(PRIMX_SYS_TYPE_COMM_SETUP);
			exit(100);
		}
	}

	return bRe;
}


void CUserRegDlg::OnBnClickedCancel()
{
	if (TRUE == m_bInstallMode) {
		exit(0);
	}
	CDialogEx::OnCancel();
}


BOOL CUserRegDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	if (FALSE == m_bInstallMode) {
		CWnd* pItem = GetDlgItem(IDC_COMM_PC_RADIO);
		if (NULL != pItem) {
			pItem->EnableWindow(FALSE);
		}
		m_nUserPC = 0;
		UpdateData(FALSE);
	}

	m_ctrlUserDataList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS);
	m_ctrlUserDataList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES); // LVS_EX_GRIDLINES
	m_ctrlUserDataList.SetColumnWithImage(IDS_USER_DATA_LIST_COLUMN, IDB_EXPORT_FILE_ITEM, 20, 20);
	
	CenterWindow(this);

	return TRUE;  
}

HBRUSH CUserRegDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode(TRANSPARENT);
	hbr = m_brushBack;

	return hbr;
}

BOOL CUserRegDlg::OnEraseBkgnd(CDC* pDC)
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

BOOL CUserRegDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
	{
		return FALSE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CUserRegDlg::OnBnClickedUserPcRadio()
{
	UpdateData();

	if (0 == m_nUserPC) {
		m_ctrlUserID.EnableWindow(TRUE);
		m_ctrlUserSearch.EnableWindow(TRUE);
		m_ctrlUserDataList.EnableWindow(TRUE);
	}
	else {
		m_ctrlUserID.EnableWindow(FALSE);
		m_ctrlUserSearch.EnableWindow(FALSE);
		m_ctrlUserDataList.EnableWindow(FALSE);
	}

}


void CUserRegDlg::OnBnClickedCommPcRadio()
{
	OnBnClickedUserPcRadio();
}


void CUserRegDlg::OnBnClickedUserSearchBtn()
{
	UpdateData();
	m_strUserID.Trim();

	if (0 == m_strUserID.GetLength()) {
		ALERT_MSGBOX(IDS_ERROR_STRING1);
		m_ctrlUserID.SetSel(0, -1);
		m_ctrlUserID.SetFocus();
		return;
	}

	m_ctrlUserDataList.DeleteAllItems();

	CCommandUserSearch CommandUserSearch;
	
	m_strRegUserID = _T("");
	
	if (TRUE == CommandUserSearch.SetConnect( )) {
		if (TRUE == CommandUserSearch.SetUserDataSearch(m_strUserID)) {
			if (1 == CommandUserSearch.GetResult()) {
				m_strResponseXMLData = CommandUserSearch.GetResponseXMLData();
				m_strRegUserID = CommandUserSearch.GetUserID();
				SetUserDataListInsert(IDS_USER_DATA_USER_MAIL, CommandUserSearch.GetMail());
				SetUserDataListInsert(IDS_USER_DATA_USER_DEPARTMENT, CommandUserSearch.GetDepartment());
				SetUserDataListInsert(IDS_USER_DATA_USER_EXT_NO, CommandUserSearch.GetExtNo());
				SetUserDataListInsert(IDS_USER_DATA_USER_CELLPHONE, CommandUserSearch.GetCellPhone());
				SetUserDataListInsert(IDS_USER_DATA_USER_TEL, CommandUserSearch.GetTel());
				SetUserDataListInsert(IDS_USER_DATA_USER_NAME, CommandUserSearch.GetUserName());
				SetUserDataListInsert(IDS_USER_DATA_USER_ID, CommandUserSearch.GetUserID());
			}
			else {
				m_strRegUserID = _T("");
				CString strFailCode = CommandUserSearch.GetFailString();
				ALERT_MSGBOX(strFailCode);
				m_ctrlUserID.SetSel(0, -1);
				m_ctrlUserID.SetFocus();
			}
		}
		else {
			ALERT_MSGBOX(IDS_ERROR_STRING2);
			m_ctrlUserID.SetSel(0, -1);
			m_ctrlUserID.SetFocus();
		}
	}
	else {
		ALERT_MSGBOX(IDS_ERROR_STRING3);
//		ALERT_MSGBOX(IDS_ERROR_STRING3);
	}

}

void CUserRegDlg::SetUserDataListInsert(UINT nResourceID, CString strValue)
{
	CString strValueName;
	strValueName.LoadString(nResourceID);
	int nItemIndex = m_ctrlUserDataList.InsertItem(0, strValueName, 0);
	m_ctrlUserDataList.SetItemText(nItemIndex, 1, strValue);
}

CString CUserRegDlg::GetUserID()
{
	return m_strRegUserID;
}


BOOL CUserRegDlg::SetUserRegRequest(CString strUserID)
{
	BOOL bRe = FALSE;

	CIPCChannel* pIPCChannel = new CIPCChannel;

	if (TRUE == pIPCChannel->SetConnect(PRIMX_IPC_AGNET_CS_KEY, 1024 * 10)) {
		CARIAEncryption ARIAEncryption;
		CString strEncUSerID = ARIAEncryption.EncryptString(strUserID);
		PRIMXCS_REQUEST_DATA* pRegData = new PRIMXCS_REQUEST_DATA;
		pRegData->nRequestType = PRIMX_REQUEST_TYPE_USER_REG;
		_tcsncpy_s(pRegData->strUserID, PRIMX_USER_ID_LEN, (LPCTSTR)strEncUSerID, _TRUNCATE);
		if (PRIMX_IPC_ERROR_NOT_SUCCESS == pIPCChannel->SetSendData((char*)pRegData, PRIMXCS_REQUEST_DATA_LEN, 10)) {

			CWaitCursor WaitCursor;
			__time64_t tCheckTime = _time64(NULL);
			while (1) {
				__time64_t tCurrentTime = _time64(NULL);
				if ((tCheckTime+30) < tCurrentTime) {
					//ALERT_MSGBOX(L"등록 실패 > 서버 응답 없음");
					break;
				}

				if (0 < pIPCChannel->GetReceiveDataCount()) {
					char* pData = NULL;
					UINT nDataLen = 0;
					if (TRUE == pIPCChannel->GetReceiveData(pData, nDataLen)) {
						PRIMXCS_REQUEST_DATA* pResponseData = (PRIMXCS_REQUEST_DATA*)pData;
						if (1 == pResponseData->nResult) {
							//INFOR_MSGBOX(L"성공");
							bRe = TRUE;
						}
						else {
							//ALERT_MSGBOX(pResponseData->strFailMessage);
						}
					}
					else {
						//ALERT_MSGBOX(L"등록 실패");
					}
					break;
				}

				Sleep(200);
			}

		}
		else {
			//ALERT_MSGBOX(L"등록 실패 > 데이터 전송 실패");
		}

		pIPCChannel->SetClose();
		_SafeDelete(pRegData);
	}
	else {
		//ALERT_MSGBOX(L"등록 실패 > 서버 연결 안됨");
	}

	_SafeDelete(pIPCChannel);

	return bRe;

}