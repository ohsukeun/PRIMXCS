/**
@brief 리스트 컨트롤 클래스
@date 2016/08/01
@version 1.0
*/

#pragma once

//////////////////////////////////////////////////////////////////////////////////
//  폰트 종류,사이즈
#define FONT_TYPE			_T("Tahoma")
#define FONT_SIZE			83

// 리스트 컨트롤 클래스
class CListCtrlEx : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEx)

public:
	CListCtrlEx();
	virtual ~CListCtrlEx();

	void SetInit(UINT nID);
	void SetLVCheck(WPARAM ItemIndex, BOOL bCheck);
	BOOL GetLVCheck(int Index);
	int SetSelectItem(int Index);
	void SetColumnWithImage(UINT nColumnResourceID, UINT nBitmapResourceID, int cx, int cy);
	void SelectAll(BOOL bSelect = TRUE);

protected:

	COLORREF		m_rowColor;

	int				m_SortCol;				//!< Rows are sorted according to this column
	bool			m_Ascending;			//!< Rows are sorted ascending / descending
	CFont           m_font;

	CImageList		m_ImageList;

	bool SortColumn(int nCol, bool bAscending);
	void SetSortArrow(int nCol, bool bAscending);
	HBITMAP CreateSortBitmap(bool bAscending);
	bool IsCommonControlsEnabled();



protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};


