/**
@brief 리스트 컨틀롤의 필드을 정렬할때 사용하는 클래스
@date 2016/08/01
@version 1.0

*/

#pragma once



enum ListCtrlSortType
{
	xxtSortInt = 1,  // Sort type integer.
	xxtSortString,   // Sort type string.
	xxtSortDateTime, // Sort type date / time.
	xxtSortDecimal   // Sort type decimal.
};

// 리스트 컨틀롤의 필드을 정렬할때 사용하는 클래스
class CListCtrlSortClass
{
public:
	CListCtrlSortClass(CListCtrl* pListCtrl, const int nCol, PFNLVCOMPARE pfnCompare = NULL);
	virtual ~CListCtrlSortClass(void);

	virtual void Sort(bool bAsc, int iType);

	static int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	static int AFX_CDECL RemoveNonNumeric(CString& strSource);

	static int AFX_CDECL SortInt(LPCTSTR lpszItem1, LPCTSTR lpszItem2, const short sOrder);

	static int AFX_CDECL SortString(LPCTSTR lpszItem1, LPCTSTR lpszItem2, const short sOrder);

	static int AFX_CDECL SortDateTime(LPCTSTR lpszItem1, LPCTSTR lpszItem2, const short sOrder);

	static int AFX_CDECL SortDecimal(LPCTSTR lpszItem1, LPCTSTR lpszItem2, const short sOrder);

	struct ITEMDATA
	{
		CString   strItem; 
		DWORD_PTR dwData;  
	};

protected:
	CListCtrl*   m_pListCtrl;  
	PFNLVCOMPARE m_pfnCompare; 

};
