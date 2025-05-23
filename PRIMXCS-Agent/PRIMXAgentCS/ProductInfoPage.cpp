// ProductInfoPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PRIMXAgentCS.h"
#include "ProductInfoPage.h"
#include "FileDBManager.h"
#include "ConfigDataManager.h"

// CProductInfoPage

IMPLEMENT_DYNCREATE(CProductInfoPage, CFormView)

CProductInfoPage::CProductInfoPage()
	: CFormView(IDD_PRODUCT_INFO_PAGE)
	, m_strAgentLastConnectTime(_T(""))
	, m_strAgentVersion(_T(""))
	, m_strAgentID(_T(""))
{

}

CProductInfoPage::~CProductInfoPage()
{
}

void CProductInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_AGENT_TITLE, m_ctrlAgentTitle);
	DDX_Control(pDX, IDC_ST_AGENT_VERSION, m_ctrlAgentVersion);
	DDX_Control(pDX, IDC_ST_AGENT_ID, m_ctrlAgentID);
	DDX_Control(pDX, IDC_ST_AGENT_LAST_CONNECT_TIME, m_ctrlAgentLastConnectTime);
	DDX_Control(pDX, IDC_ST_PRODUCT_TITLE01, m_ctrlProductTitle[0]);
	DDX_Control(pDX, IDC_ST_PRODUCT_VERSION01, m_ctrlProductVersion[0]);
	DDX_Control(pDX, IDC_ST_PRODUCT_TITLE02, m_ctrlProductTitle[1]);
	DDX_Control(pDX, IDC_ST_PRODUCT_VERSION02, m_ctrlProductVersion[1]);
	DDX_Control(pDX, IDC_ST_PRODUCT_TITLE03, m_ctrlProductTitle[2]);
	DDX_Control(pDX, IDC_ST_PRODUCT_VERSION03, m_ctrlProductVersion[2]);	
	DDX_Text(pDX, IDC_ST_AGENT_LAST_CONNECT_TIME, m_strAgentLastConnectTime);
	DDX_Text(pDX, IDC_ST_AGENT_VERSION, m_strAgentVersion);
	DDX_Text(pDX, IDC_ST_AGENT_ID, m_strAgentID);
	DDX_Text(pDX, IDC_ST_PRODUCT_TITLE01, m_strProductTitle[0]);
	DDX_Text(pDX, IDC_ST_PRODUCT_VERSION01, m_strProductVersion[0]);
	DDX_Text(pDX, IDC_ST_PRODUCT_TITLE02, m_strProductTitle[1]);
	DDX_Text(pDX, IDC_ST_PRODUCT_VERSION02, m_strProductVersion[1]);
	DDX_Text(pDX, IDC_ST_PRODUCT_TITLE03, m_strProductTitle[2]);
	DDX_Text(pDX, IDC_ST_PRODUCT_VERSION03, m_strProductVersion[2]);


}

BEGIN_MESSAGE_MAP(CProductInfoPage, CFormView)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProductInfoPage 진단입니다.

#ifdef _DEBUG
void CProductInfoPage::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CProductInfoPage::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CProductInfoPage 메시지 처리기입니다.


void CProductInfoPage::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();


	CFont *pDlgFont = GetFont();
	LOGFONT lf = { 0 };
	pDlgFont->GetLogFont(&lf);

	m_ctrlAgentTitle.SetTextColor(RGB(0, 0, 0));
	m_ctrlAgentTitle.SetFont(lf.lfFaceName, 11, TRUE);
	m_ctrlAgentTitle.SetAlignment(DKS_TEXT_ALIGNMENT_LEFT,DKS_TEXT_ALIGNMENT_CENTER );
	m_ctrlAgentTitle.SetTrasparent(TRUE);

	for (int i = 0; i < 3; i++) {
		m_ctrlProductTitle[i].SetTextColor(RGB(0, 0, 0));
		m_ctrlProductTitle[i].SetFont(lf.lfFaceName, 11, TRUE);
		m_ctrlProductTitle[i].SetAlignment(DKS_TEXT_ALIGNMENT_LEFT, DKS_TEXT_ALIGNMENT_CENTER);
		m_ctrlProductTitle[i].SetTrasparent(TRUE);
	}

	GetProductData();

	SetTimer(PRIMX_TIMER_PRODUCT_CHECK, PRIMX_TIMER_PRODUCT_CHECK_TIME, 0);
}


BOOL CProductInfoPage::OnEraseBkgnd(CDC* pDC)
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


HBRUSH CProductInfoPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushBack;
	}
	return hbr;
}

void CProductInfoPage::GetProductData()
{
	CFileDBManager FileDBManager;
	CConfigDataManager ConfigDataManager;

	CString strAgentID = ConfigDataManager.GetAgentID();
	CString strAgentVersion = ConfigDataManager.GetAgentVersion();
	CString strLastPollingTime = ConfigDataManager.GetLastPollingTime();

	m_strAgentVersion.Format(IDS_PRODUCT_INFO_VERSION, strAgentVersion);
	m_strAgentID.Format(IDS_PRODUCT_INFO_AGENTID, strAgentID);
	m_strAgentLastConnectTime.Format(IDS_PRODUCT_INFO_CONNECT_TIME, strLastPollingTime);
	
	PRIMX_PRODUCT_DATA Zonecentrol;
	PRIMX_PRODUCT_DATA Orizone;
	PRIMX_PRODUCT_DATA Cryhod;

	if (TRUE == FileDBManager.GetPRIMXProductData(strAgentID, Zonecentrol, Orizone, Cryhod)) {
		int nIndex = 0;

		if (TRUE == Zonecentrol.bInstall) {
			m_ctrlProductTitle[nIndex].ShowWindow(SW_SHOW);
			m_ctrlProductVersion[nIndex].ShowWindow(SW_SHOW);

			m_strProductTitle[nIndex].Format(IDS_PRODUCT_INFO_TITLE ,Zonecentrol.strDisplayName);
			m_strProductVersion[nIndex].Format(IDS_PRODUCT_INFO_VERSION,Zonecentrol.strDisplayVersionEx);
			nIndex++;
		}
		if (TRUE == Orizone.bInstall) {
			m_ctrlProductTitle[nIndex].ShowWindow(SW_SHOW);
			m_ctrlProductVersion[nIndex].ShowWindow(SW_SHOW);

			m_strProductTitle[nIndex].Format(IDS_PRODUCT_INFO_TITLE, Orizone.strDisplayName);
			m_strProductVersion[nIndex].Format(IDS_PRODUCT_INFO_VERSION, Orizone.strDisplayVersionEx);
			nIndex++;
		}
		if (TRUE == Cryhod.bInstall) {
			m_ctrlProductTitle[nIndex].ShowWindow(SW_SHOW);
			m_ctrlProductVersion[nIndex].ShowWindow(SW_SHOW);

			m_strProductTitle[nIndex].Format(IDS_PRODUCT_INFO_TITLE, Cryhod.strDisplayName);
			m_strProductVersion[nIndex].Format(IDS_PRODUCT_INFO_VERSION, Cryhod.strDisplayVersionEx);
			nIndex++;
		}
	//	if(0 < nIndex) nIndex--;

		for (int i = nIndex;i < 3; i++)	{
			m_ctrlProductTitle[i].ShowWindow(SW_HIDE);
			m_ctrlProductVersion[i].ShowWindow(SW_HIDE);
		}

		UpdateData(FALSE);
	}
}

void CProductInfoPage::OnTimer(UINT_PTR nIDEvent)
{
	if (PRIMX_TIMER_PRODUCT_CHECK == nIDEvent) {
		KillTimer(PRIMX_TIMER_PRODUCT_CHECK);
		GetProductData();
		SetTimer(PRIMX_TIMER_PRODUCT_CHECK, PRIMX_TIMER_PRODUCT_CHECK_TIME, 0);
	}
	
	CFormView::OnTimer(nIDEvent);
}
