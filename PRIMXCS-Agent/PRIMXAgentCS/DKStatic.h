#pragma once

#include <GdiPlus.h>
#pragma comment(lib, "GdiPlus.lib")
using namespace Gdiplus;


enum DKS_TEXT_ALIGNMENT
{
	DKS_TEXT_ALIGNMENT_LEFT = StringAlignmentNear,
	DKS_TEXT_ALIGNMENT_CENTER = StringAlignmentCenter,
	DKS_TEXT_ALIGNMENT_RIGHT = StringAlignmentFar,
	DKS_TEXT_ALIGNMENT_TOP = StringAlignmentNear,
	DKS_TEXT_ALIGNMENT_MIDDLE = StringAlignmentCenter,
	DKS_TEXT_ALIGNMENT_BOTTOM = StringAlignmentFar
};

// predefined colors
#define DKCOLOR_BLACK			RGB(0, 0, 0)
#define DKCOLOR_BLUE			RGB(0, 0, 255)
#define DKCOLOR_BROWN			RGB(153, 102, 51)
#define DKCOLOR_CYON			RGB(0, 255, 255)
#define DKCOLOR_DARKGRAY		RGB(76, 76, 76)
#define DKCOLOR_GRAY			RGB(127, 127, 127)
#define DKCOLOR_GREEN			RGB(0, 255, 0)
#define DKCOLOR_LIGHTGRAY		RGB(170, 170, 170)
#define DKCOLOR_MAGENTA			RGB(255, 0, 255)
#define DKCOLOR_ORANGE			RGB(255, 127, 0)
#define DKCOLOR_PURPLE			RGB(127, 0, 127)
#define DKCOLOR_RED				RGB(255, 0, 0)
#define DKCOLOR_WHITE			RGB(255, 255, 255)
#define DKCOLOR_YELLOW			RGB(255, 255, 0)

// CVCPStatic

class CDKStatic : public CStatic
{
	DECLARE_DYNAMIC(CDKStatic)

public:
	void	SetTextColor(COLORREF crText);
	void	SetBackgroundColor(COLORREF crBg);
	void	SetTrasparent(BOOL bTrans = TRUE);
	void	SetFont(LPCTSTR lpszFontName, int nSize, BOOL bBold);
	void	SetFontSize(int nSize, BOOL bBold = FALSE);
	void	SetAlignment(DKS_TEXT_ALIGNMENT hAlign, DKS_TEXT_ALIGNMENT vAlign);



protected:
	COLORREF	m_crText, m_crBg;
	BOOL	m_bTransparent;
	Gdiplus::Font	*m_pFont;
	REAL		m_fontSize;
	StringAlignment m_hAlign, m_vAlign;

private:
	ULONG_PTR	m_ulGdiToken;

public:
	CDKStatic();
	virtual ~CDKStatic();

protected:
	afx_msg LRESULT OnSetText(WPARAM,LPARAM);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

class CDKString : public CString
{
public:

	static CString LoadResString(UINT nResourceID);
};

