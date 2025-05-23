// ListDropTarget.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PRIMXAgentCS.h"
#include "ListDropTarget.h"
#include "ExportFileGenerateDlg.h"

// CListDropTarget

IMPLEMENT_DYNAMIC(CListDropTarget, CObject)


CListDropTarget::CListDropTarget(CExportFileGenerateDlg* pList) :
m_pListCtrl(pList), m_piDropHelper(NULL), m_bUseDnDHelper(false)
{
	if (SUCCEEDED(CoCreateInstance(CLSID_DragDropHelper, NULL, CLSCTX_INPROC_SERVER, IID_IDropTargetHelper, (void**)&m_piDropHelper))) {
		m_bUseDnDHelper = true;
	}
}

CListDropTarget::~CListDropTarget()
{
	if (NULL != m_piDropHelper) {
		m_piDropHelper->Release();
		m_bUseDnDHelper = false;
	}
}


BEGIN_MESSAGE_MAP(CListDropTarget, COleDropTarget)
END_MESSAGE_MAP()



// CListDropTarget 메시지 처리기입니다.


DROPEFFECT CListDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	DROPEFFECT dwEffect = DROPEFFECT_NONE;

	// Check for our own custom clipboard format in the data object.  If it's
	// present, then the DnD was initiated from our own window, and we won't
	// accept the drop.
	// If it's not present, then we check for CF_HDROP data in the data object.

	if (NULL == pDataObject->GetGlobalData(g_uCustomClipbrdFormat))
	{
		// Look for CF_HDROP data in the data object, and accept the drop if
		// it's there.

		if (NULL != pDataObject->GetGlobalData(CF_HDROP))
			dwEffect = DROPEFFECT_COPY;
	}

	// Call the DnD helper.

	if (m_bUseDnDHelper)
	{
		// The DnD helper needs an IDataObject interface, so get one from
		// the COleDataObject.  Note that the FALSE param means that
		// GetIDataObject will not AddRef() the returned interface, so 
		// we do not Release() it.

		IDataObject* piDataObj = pDataObject->GetIDataObject(FALSE);

		m_piDropHelper->DragEnter(pWnd->GetSafeHwnd(), piDataObj, &point, dwEffect);
	}

	return dwEffect;

	//return COleDropTarget::OnDragEnter(pWnd, pDataObject, dwKeyState, point);
}


void CListDropTarget::OnDragLeave(CWnd* pWnd)
{

	if (m_bUseDnDHelper)
	{
		m_piDropHelper->DragLeave();
	}
	//	COleDropTarget::OnDragLeave(pWnd);
}


DROPEFFECT CListDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	DROPEFFECT dwEffect = DROPEFFECT_NONE;
	
	//if (NULL != m_pListCtrl) {
	//	UINT nFlags;
	//	int nIndex = m_pExeTreeList->HitTest(point, &nFlags);
	//	m_pExeTreeList->SelectAll(FALSE);
	//	m_pExeTreeList->SetSelectionMark(nIndex);
	//	m_pExeTreeList->SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	//	m_pExeTreeList->EnsureVisible(nIndex, TRUE);
	//}

	if (NULL == pDataObject->GetGlobalData(g_uCustomClipbrdFormat))
	{
		// Look for CF_HDROP data in the data object, and accept the drop if
		// it's there.

		if (NULL != pDataObject->GetGlobalData(CF_HDROP))
			dwEffect = DROPEFFECT_COPY;
	}

	// Call the DnD helper.

	if (m_bUseDnDHelper)
	{
		m_piDropHelper->DragOver(&point, dwEffect);
	}
	return dwEffect;
	//	return COleDropTarget::OnDragOver(pWnd, pDataObject, dwKeyState, point);
}


BOOL CListDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	BOOL bRet;

	// Read the CF_HDROP data and put the files in the main window's list.

	bRet = ReadHdropData(pDataObject);

	// Call the DnD helper.

	if (m_bUseDnDHelper)
	{
		// The DnD helper needs an IDataObject interface, so get one from
		// the COleDataObject.  Note that the FALSE param means that
		// GetIDataObject will not AddRef() the returned interface, so 
		// we do not Release() it.

		IDataObject* piDataObj = pDataObject->GetIDataObject(FALSE);

		m_piDropHelper->Drop(piDataObj, &point, dropEffect);
	}

	return bRet;
	//return COleDropTarget::OnDrop(pWnd, pDataObject, dropEffect, point);

}

BOOL CListDropTarget::ReadHdropData(COleDataObject* pDataObject)
{
	HGLOBAL     hg;
	HDROP       hdrop;
	UINT        uNumFiles;
	TCHAR       szNextFile[MAX_PATH];

	hg = pDataObject->GetGlobalData(CF_HDROP);

	if (NULL == hg)
	{
		return FALSE;
	}

	hdrop = (HDROP)GlobalLock(hg);

	if (NULL == hdrop)
	{
		GlobalUnlock(hg);
		return FALSE;
	}

	CStringList FileList;

	uNumFiles = DragQueryFile(hdrop, -1, NULL, 0);

	for (UINT uFile = 0; uFile < uNumFiles; uFile++)
	{
		ZeroMemory(szNextFile, sizeof(szNextFile));
		if (DragQueryFile(hdrop, uFile, szNextFile, MAX_PATH) > 0)
		{
			CString strFile = szNextFile;
			FileList.AddTail(strFile);

		}
	}

	GlobalUnlock(hg);
	
	if (NULL != m_pListCtrl) m_pListCtrl->SetAddFile(FileList);

	FileList.RemoveAll();

	return TRUE;
}
