// UserInfoPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PRIMXAgentCS.h"
#include "UserInfoPage.h"
#include "CommandUserSearch.h"
#include "ConfigDataManager.h"

// CUserInfoPage

IMPLEMENT_DYNCREATE(CUserInfoPage, CFormView)

CUserInfoPage::CUserInfoPage()
	: CFormView(IDD_USER_INFO_PAGE)
	, m_strSystemType(_T(""))
	, m_strUserID(_T(""))
	, m_strUserName(_T(""))
	, m_strTel(_T(""))
	, m_strCellPhoneNo(_T(""))
	, m_strExtNo(_T(""))
	, m_strDeptName(_T(""))
	, m_strGradeName(_T(""))
	, m_strMailAddr(_T(""))
{

}

CUserInfoPage::~CUserInfoPage()
{
}

void CUserInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STSTEM_TYPE, m_strSystemType);
	DDX_Text(pDX, IDC_USER_ID, m_strUserID);
	DDX_Text(pDX, IDC_USER_NAME, m_strUserName);
	DDX_Text(pDX, IDC_TEL, m_strTel);
	DDX_Text(pDX, IDC_CELL_PHONE_NO, m_strCellPhoneNo);
	DDX_Text(pDX, IDC_EXT_NO, m_strExtNo);
	DDX_Text(pDX, IDC_DEPT_NAME, m_strDeptName);
	DDX_Text(pDX, IDC_GRADE_NAME, m_strGradeName);
	DDX_Text(pDX, IDC_EMAIL_ADDR, m_strMailAddr);
}

BEGIN_MESSAGE_MAP(CUserInfoPage, CFormView)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_USER_DATA_REFRESH, &CUserInfoPage::OnBnClickedUserDataRefresh)
END_MESSAGE_MAP()


// CUserInfoPage 진단입니다.

#ifdef _DEBUG
void CUserInfoPage::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CUserInfoPage::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CUserInfoPage 메시지 처리기입니다.


void CUserInfoPage::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetUserData();
}


BOOL CUserInfoPage::OnEraseBkgnd(CDC* pDC)
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

	return TRUE; //CFormView::OnEraseBkgnd(pDC);
}


HBRUSH CUserInfoPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushBack;
	}
	return hbr;
}

void CUserInfoPage::GetUserData()
{
	CConfigDataManager ConfigDataManager;
	CString strUserDataXML = ConfigDataManager.GetUserData();
	strUserDataXML.Trim();

	m_strSystemType = GetSystemTypeString(ConfigDataManager.GetSystemType());
	m_strUserID = ConfigDataManager.GetUserID();

	if (0 < strUserDataXML.GetLength()) {
		CCommandUserSearch UserData;
		if (PRIMX_ERROR_SUCCESS == UserData.SetResponseDataTypeParser(strUserDataXML)) {
			m_strUserName = UserData.GetUserName();
			m_strTel = UserData.GetTel();
			m_strCellPhoneNo = UserData.GetCellPhone();
			m_strExtNo = UserData.GetExtNo();
			m_strMailAddr = UserData.GetMail();
			m_strDeptName = UserData.GetDepartment();
		}
	}

	UpdateData(FALSE);
}

CString CUserInfoPage::GetSystemTypeString(UINT nType)
{
	CString strSystemType;
	strSystemType.LoadString(IDS_SYSTEM_TYPE_UNKNOWN);
	
	if (PRIMX_SYS_TYPE_COMM == nType || PRIMX_SYS_TYPE_COMM_SETUP == nType) {
		strSystemType.LoadString(IDS_SYSTEM_TYPE_COMM_PC);
	}
	if (PRIMX_SYS_TYPE_USER == nType || PRIMX_SYS_TYPE_USER_SETUP == nType) {
		strSystemType.LoadString(IDS_SYSTEM_TYPE_USER_PC);
	}

	return strSystemType;
}

void CUserInfoPage::OnBnClickedUserDataRefresh()
{
	if (0 == m_strUserID.GetLength()) {
		CConfigDataManager ConfigDataManager;
		m_strUserID = ConfigDataManager.GetUserID();
		if (0 == m_strUserID.GetLength()) {
			return;
		}
	}

	CCommandUserSearch CommandUserSearch;
	if (TRUE == CommandUserSearch.SetConnect()) {
		if (TRUE == CommandUserSearch.SetUserDataSearch(m_strUserID)) {
			if (1 == CommandUserSearch.GetResult()) {
				CString strUserXMLData = CommandUserSearch.GetResponseXMLData();
				if (0 < strUserXMLData.GetLength()) {
					CConfigDataManager ConfigDataManager;
					if (FALSE == ConfigDataManager.SetUserData(strUserXMLData)) {
						SetUserData(strUserXMLData);
					}
				}
				m_strUserName = CommandUserSearch.GetUserName();
				m_strTel = CommandUserSearch.GetTel();
				m_strCellPhoneNo = CommandUserSearch.GetCellPhone();
				m_strExtNo = CommandUserSearch.GetExtNo();
				m_strMailAddr = CommandUserSearch.GetMail();
				m_strDeptName = CommandUserSearch.GetDepartment();
				UpdateData(FALSE);
			}
		}
	}


}
