// ListCtrlEx.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ListCtrlEx.h"
#include "ListCtrlSortClass.h"

// CListCtrlEx

IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

CListCtrlEx::CListCtrlEx()
{
	m_rowColor = RGB(250,255,238);

	m_SortCol = -1;
	m_Ascending = false;
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CListCtrlEx::OnLvnColumnclick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlEx::OnNMCustomdraw)
END_MESSAGE_MAP()



// CListCtrlEx 메시지 처리기입니다.

void CListCtrlEx::SetLVCheck(WPARAM ItemIndex, BOOL bCheck)
{
	ListView_SetItemState (	m_hWnd, 
		ItemIndex, 
		UINT((int(bCheck) + 1) << 12), 
		LVIS_STATEIMAGEMASK);

}

BOOL CListCtrlEx::GetLVCheck(int Index)
{
	return ListView_GetCheckState(m_hWnd,Index);

}

int CListCtrlEx::SetSelectItem(int Index)
{
	SetItemState(Index, LVIS_SELECTED | LVIS_FOCUSED,LVIS_SELECTED | LVIS_FOCUSED);   
	SetSelectionMark(Index); 
	return Index;
}
bool CListCtrlEx::SortColumn(int nCol, bool bAscending)
{
	// virtual lists cannot be sorted with this method
	if (GetStyle() & LVS_OWNERDATA)
		return false;

	if (GetItemCount()<=0)
		return true;

	CWaitCursor waitCursor;

	CListCtrlSortClass csc(this,m_SortCol);
	csc.Sort (m_Ascending, xxtSortString);

	return true;
}

void CListCtrlEx::SetSortArrow(int nCol, bool bAscending)
{
	if (IsCommonControlsEnabled())
	{
#if (_WIN32_WINNT >= 0x501)
		for(int i = 0; i < GetHeaderCtrl()->GetItemCount(); ++i)
		{
			HDITEM hditem = {0};
			hditem.mask = HDI_FORMAT;
			VERIFY( GetHeaderCtrl()->GetItem( i, &hditem ) );
			hditem.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
			if (i == nCol)
			{
				hditem.fmt |= bAscending ? HDF_SORTUP : HDF_SORTDOWN;
			}
			VERIFY( CListCtrl::GetHeaderCtrl()->SetItem( i, &hditem ) );
		}
#endif
	}
	else
	{
		for(int i = 0; i < GetHeaderCtrl()->GetItemCount(); ++i)
		{
			HDITEM hditem = {0};
			hditem.mask = HDI_BITMAP | HDI_FORMAT;
			VERIFY( GetHeaderCtrl()->GetItem( i, &hditem ) );
			if (hditem.fmt & HDF_BITMAP && hditem.fmt & HDF_BITMAP_ON_RIGHT)
			{
				if (hditem.hbm)
				{
					VERIFY( DeleteObject(hditem.hbm) );
					hditem.hbm = NULL;
				}
				hditem.fmt &= ~(HDF_BITMAP|HDF_BITMAP_ON_RIGHT);
				VERIFY( CListCtrl::GetHeaderCtrl()->SetItem( i, &hditem ) );
			}
			if (i == nCol)
			{
				hditem.fmt |= HDF_BITMAP|HDF_BITMAP_ON_RIGHT;
				//UINT bitmapID = m_Ascending ? IDB_DOWNARROW : IDB_UPARROW; 
				//hditem.hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0,0, LR_LOADMAP3DCOLORS);
				hditem.hbm = CreateSortBitmap(bAscending);
				VERIFY( hditem.hbm!=NULL );
				VERIFY( CListCtrl::GetHeaderCtrl()->SetItem( i, &hditem ) );
			}
		}
	}
}
bool CListCtrlEx::IsCommonControlsEnabled()
{
	bool commoncontrols = false;

	// Test if application has access to common controls
	HMODULE hinstDll = ::LoadLibrary(_T("comctl32.dll"));
	if (hinstDll)
	{
		DLLGETVERSIONPROC pDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hinstDll, "DllGetVersion");
		if (pDllGetVersion != NULL)
		{
			DLLVERSIONINFO dvi = {0};
			dvi.cbSize = sizeof(dvi);
			HRESULT hRes = pDllGetVersion ((DLLVERSIONINFO *) &dvi);
			if (SUCCEEDED(hRes))
				commoncontrols = dvi.dwMajorVersion >= 6;
		}
		::FreeLibrary(hinstDll);
	}
	return commoncontrols;
}
HBITMAP CListCtrlEx::CreateSortBitmap(bool bAscending)
{
	// Aquire the Display DC
	CDC* pDC = CDC::FromHandle(::GetDC(::GetDesktopWindow()));
	//create a memory dc
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	//Create a memory bitmap
	CBitmap newbmp;
	CRect rcIcon(0, 0, 16, 16);
	newbmp.CreateCompatibleBitmap(pDC, rcIcon.Height(), rcIcon.Width());

	//select the bitmap in the memory dc
	CBitmap *pOldBitmap = memDC.SelectObject(&newbmp);

	//make the bitmap white to begin with
	memDC.FillSolidRect(rcIcon.top,rcIcon.left,rcIcon.bottom,rcIcon.right,::GetSysColor(COLOR_3DFACE));

	// Set up pens to use for drawing the triangle
	CPen penLight(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
	CPen penShadow(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
	CPen *pOldPen = memDC.SelectObject( &penLight );

	int iOffset = (rcIcon.bottom - rcIcon.top) / 4;

	if( bAscending )
	{
		// draw the arrow pointing upwards.
		memDC.MoveTo( rcIcon.right - 2 * iOffset, iOffset);
		memDC.LineTo( rcIcon.right - iOffset, rcIcon.bottom - iOffset - 1 );
		memDC.LineTo( rcIcon.right - 3 * iOffset - 2, rcIcon.bottom - iOffset - 1 );
		(void)memDC.SelectObject( &penShadow );
		memDC.MoveTo( rcIcon.right - 3 * iOffset - 1, rcIcon.bottom - iOffset - 1 );
		memDC.LineTo( rcIcon.right - 2 * iOffset, iOffset - 1);		
	}
	else
	{
		// draw the arrow pointing downwards.
		memDC.MoveTo( rcIcon.right - iOffset - 1, iOffset );
		memDC.LineTo( rcIcon.right - 2 * iOffset - 1, rcIcon.bottom - iOffset );
		(void)memDC.SelectObject( &penShadow );
		memDC.MoveTo( rcIcon.right - 2 * iOffset - 2, rcIcon.bottom - iOffset );
		memDC.LineTo( rcIcon.right - 3 * iOffset - 1, iOffset );
		memDC.LineTo( rcIcon.right - iOffset - 1, iOffset );		
	}

	// Restore the pen
	memDC.SelectObject(pOldPen);

	//select old bitmap back into the memory dc
	memDC.SelectObject(pOldBitmap);

	return (HBITMAP)newbmp.Detach();
}

void CListCtrlEx::SetInit(UINT nID)
{
	m_font.CreatePointFont(FONT_SIZE, FONT_TYPE);
	SetFont(&m_font);
	GetHeaderCtrl()->SetFont(&m_font);

	ModifyStyle(NULL, LVS_SINGLESEL);

	DWORD List_Style = GetExtendedStyle();
	List_Style |= LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_HEADERDRAGDROP ; // LVS_EX_GRIDLINES
	SetExtendedStyle(List_Style);

	CImageList ilHost;
	CBitmap	bmHost;
	bmHost.LoadBitmap(nID);
	ilHost.Create(16, 16, ILC_COLORDDB|ILC_MASK, 0, 1);
	ilHost.Add(&bmHost, RGB(255, 0, 255));
	SetImageList(&ilHost, LVSIL_SMALL);
	ilHost.Detach();
	bmHost.Detach();
}

void CListCtrlEx::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( GetFocus() != this )
		SetFocus();	// Force focus to finish editing

	int nCol = pNMLV->iSubItem;

	if (m_SortCol==nCol){
		m_Ascending = !m_Ascending;
	}
	else{
		m_SortCol = nCol;
		m_Ascending = true;
	}

	if (SortColumn(m_SortCol, m_Ascending)){
		SetSortArrow(m_SortCol, m_Ascending);
	}

	*pResult = 0;

}


void CListCtrlEx::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = CDRF_DODEFAULT;
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage ){
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage ){

		int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
		if(-1 != nItem){
			if(0 == (nItem % 2)){
				pLVCD->clrTextBk = m_rowColor;
			}
		}

		//COLORREF crText;
		//crText = pLVCD->clrText;
		//if(TRUE == m_ItemDisable){
		//	if(m_ItemDisableData != GetItemText(pLVCD->nmcd.dwItemSpec ,m_ItemDisableCol)){
		//		*pResult = 0;
		//		return;
		//	}
		//	crText = DISABLE_COLOR;
		//	pLVCD->nmcd.uItemState  = CDIS_GRAYED ;  //CDIS_DISABLED ;
		//}
		//if(TRUE == m_Grayed){
		//	if(m_GrayedData != GetItemText(pLVCD->nmcd.dwItemSpec ,m_GrayedCol)){
		//		*pResult = 0;
		//		return;
		//	}
		//	crText = DISABLE_COLOR;
		//}

		//      pLVCD->clrText = crText;
		*pResult = CDRF_DODEFAULT;

	}
}
void CListCtrlEx::SetColumnWithImage(UINT nColumnResourceID, UINT nBitmapResourceID, int cx, int cy)
{
	CString strColumn = _T("");
	CString strColumnsString = _T("");
	strColumnsString.LoadString(nColumnResourceID);

	BOOL bCkeckBox = (0 == (GetExtendedStyle() & LVS_EX_CHECKBOXES) ? FALSE : TRUE);

	m_font.CreatePointFont(FONT_SIZE, FONT_TYPE);
	SetFont(&m_font);
	GetHeaderCtrl()->SetFont(&m_font);

	//ModifyStyle(NULL, LVS_SINGLESEL);

	DWORD List_Style = GetExtendedStyle();
	List_Style |= LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_HEADERDRAGDROP; // LVS_EX_GRIDLINES
	SetExtendedStyle(List_Style);

	int nIndex = 0;
	BOOL bExtract = TRUE;
	while (TRUE == bExtract) {
		bExtract = AfxExtractSubString(strColumn, (LPCTSTR)strColumnsString, nIndex, '|');
		if (TRUE == bExtract) {
			CString strColumnName;
			int strColumnSize = 100;
			int FindIndex = strColumn.Find(_T("/"));
			if (0 < FindIndex) {
				strColumnName = strColumn.Left(FindIndex);
				strColumnSize = int(_tstof(strColumn.Right(strColumn.GetLength() - (FindIndex + 1))));
			}
			else {
				strColumnName = strColumn;
			}

			if (0 == nIndex && TRUE == bCkeckBox) {
				InsertColumn(nIndex, strColumnName, LVCFMT_CENTER, strColumnSize);
			}
			else {
				InsertColumn(nIndex, strColumnName, LVCFMT_LEFT, strColumnSize);
			}

		}
		nIndex++;
	}

	if (0 < nBitmapResourceID) {
		CImageList ilHost;
		CBitmap	bmHost;
		bmHost.LoadBitmap(nBitmapResourceID);
		ilHost.Create(cx, cy, ILC_COLOR24 | ILC_MASK, 2, 1); // ILC_COLORDDB
		ilHost.Add(&bmHost, RGB(0, 255, 0)); // RGB(255, 0, 255));
		SetImageList(&ilHost, LVSIL_SMALL);
		ilHost.Detach();
		bmHost.Detach();
	}

}

void CListCtrlEx::SelectAll(BOOL bSelect /*= TRUE*/)
{
	DWORD dwCurStyle = GetStyle();
	if (dwCurStyle & LVS_SINGLESEL) return;

	BeginWaitCursor();
	int nCount = GetItemCount();
	for (int i = 0; i < nCount; i++) {
		if (bSelect) {
			SetItemState(i, TVIS_SELECTED, TVIS_SELECTED);
		}
		else {
			SetItemState(i, 0, TVIS_SELECTED);
		}
	}
	EndWaitCursor();
}
