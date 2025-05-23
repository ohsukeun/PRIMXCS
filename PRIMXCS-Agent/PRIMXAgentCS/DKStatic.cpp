// VCPStatic.cpp : implementation file
//

#include "stdafx.h"
#include "DKStatic.h"


// CVCPStatic

IMPLEMENT_DYNAMIC(CDKStatic, CStatic)

CDKStatic::CDKStatic()
{
	GdiplusStartupInput gdiStartupInput;
	m_ulGdiToken = 0;

	Status status = GdiplusStartup(&m_ulGdiToken, &gdiStartupInput, NULL);

	m_hAlign = StringAlignmentNear;
	m_vAlign = StringAlignmentNear;

	// set default font
	m_fontSize = 9.0;
	m_pFont = new Gdiplus::Font(FontFamily::GenericSansSerif(), m_fontSize, FontStyleRegular, UnitPixel);

	m_crText = RGB(0,0,0);
	m_crBg = RGB(255,255,255);
	m_bTransparent = TRUE;
}

CDKStatic::~CDKStatic()
{
	if(NULL != m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}

	GdiplusShutdown(m_ulGdiToken);
}


BEGIN_MESSAGE_MAP(CDKStatic, CStatic)
	ON_MESSAGE(WM_SETTEXT,OnSetText)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CVCPStatic message handlers

LRESULT CDKStatic::OnSetText(WPARAM wParam,LPARAM lParam)
{
	LRESULT Result = Default();
	CRect Rect;
	GetWindowRect(&Rect);
	GetParent()->ScreenToClient(&Rect);
	GetParent()->InvalidateRect(&Rect);
	GetParent()->UpdateWindow();
	return Result;
}


HBRUSH CDKStatic::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetBkMode(m_bTransparent ? TRANSPARENT : OPAQUE);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CDKStatic::SetTextColor(COLORREF crText)
{
	m_crText = crText;
}

void CDKStatic::SetBackgroundColor(COLORREF crBg)
{
	m_crBg = crBg;
	m_bTransparent = FALSE;
}

void CDKStatic::SetTrasparent(BOOL bTrans /* = TRUE */)
{
	m_bTransparent = bTrans;
}


void CDKStatic::SetFont(LPCTSTR lpszFontName, int nSize, BOOL bBold)
{
	if(0 == _tcslen(lpszFontName) || 0 > nSize) return;

	if(NULL != m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}

	FontFamily fontFamily(lpszFontName);
	if (fontFamily.IsAvailable())
		m_pFont = new Gdiplus::Font(&fontFamily, (REAL)nSize, (bBold ? FontStyleBold : FontStyleRegular), UnitPixel);
	else
		m_pFont = new Gdiplus::Font(FontFamily::GenericSansSerif(), (REAL)nSize, (bBold ? FontStyleBold : FontStyleRegular), UnitPixel);

	if(NULL == m_pFont) return;
}

void CDKStatic::SetFontSize(int nSize, BOOL bBold)
{
	if (m_pFont)
	{
		FontFamily fontFamily;
		m_pFont->GetFamily(&fontFamily);

		delete m_pFont;
		m_pFont = new Gdiplus::Font(&fontFamily, (REAL)nSize, (bBold ? FontStyleBold : FontStyleRegular), UnitPixel);
	}
}

void CDKStatic::SetAlignment(DKS_TEXT_ALIGNMENT hAlign, DKS_TEXT_ALIGNMENT vAlign)
{
	m_hAlign = (Gdiplus::StringAlignment)hAlign;
	m_vAlign = (Gdiplus::StringAlignment)vAlign;
}

void CDKStatic::PreSubclassWindow()
{
	ModifyStyle(0, SS_OWNERDRAW);

	CFont *pParentFont = GetParent()->GetFont();
	if(NULL != pParentFont)
	{
		LOGFONT lf;
		pParentFont->GetLogFont(&lf);

		m_pFont = new Gdiplus::Font(lf.lfFaceName, 10.0, FontStyleRegular, UnitPoint);
	}	

	CStatic::PreSubclassWindow();
}

void CDKStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CRect rc;
	GetClientRect(rc);

	// get window rectF
	RectF rectF((REAL)rc.left, (REAL)rc.top, (REAL)rc.Width(), (REAL)rc.Height());
	
	Graphics graphics(pDC->m_hDC);
	
	CString strText;
	GetWindowText(strText);

	if(NULL != m_pFont)
	{
		SolidBrush sbrText(Color(255, GetRValue(m_crText), GetGValue(m_crText), GetBValue(m_crText)));
		SolidBrush sbrBg(Color(255, GetRValue(m_crBg), GetGValue(m_crBg), GetBValue(m_crBg)));

		StringFormat stringFormat;
		stringFormat.SetAlignment(m_hAlign);
		stringFormat.SetLineAlignment(m_vAlign);

		if (!m_bTransparent)
			graphics.FillRectangle(&sbrBg, rectF);

		graphics.DrawString(strText, -1, m_pFont, rectF, &stringFormat, &sbrText);
	}
}

BOOL CDKStatic::OnEraseBkgnd(CDC* pDC)
{
	/*
	if (m_Bmp.GetSafeHandle() == NULL)
	{
		CRect Rect;
		GetWindowRect(&Rect);
		CWnd *pParent = GetParent();
		ASSERT(pParent);
		pParent->ScreenToClient(&Rect);  //convert our corrdinates to our parents

		//copy what's on the parents at this point
		CDC *pParentDC = pParent->GetDC();
		CDC MemDC;
		MemDC.CreateCompatibleDC(pParentDC);
		m_Bmp.CreateCompatibleBitmap(pParentDC, Rect.Width(), Rect.Height());
		CBitmap *pOldBmp = MemDC.SelectObject(&m_Bmp);
		MemDC.BitBlt(0, 0, Rect.Width(), Rect.Height(), pParentDC, Rect.left, Rect.top, SRCCOPY);
		MemDC.SelectObject(pOldBmp);
		pParent->ReleaseDC(pParentDC);
	}
	else //copy what we copied off the parent the first time back onto the parent
	{
		CRect Rect;
		GetClientRect(Rect);
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp = MemDC.SelectObject(&m_Bmp);
		pDC->BitBlt(0,0,Rect.Width(),Rect.Height(),&MemDC,0,0,SRCCOPY);
		MemDC.SelectObject(pOldBmp);
	}

	return TRUE;
	*/
	return CStatic::OnEraseBkgnd(pDC);
}

CString CDKString::LoadResString(UINT nResourceID)
{
	CString strRet;
	strRet.LoadString(nResourceID);
	return strRet;
}