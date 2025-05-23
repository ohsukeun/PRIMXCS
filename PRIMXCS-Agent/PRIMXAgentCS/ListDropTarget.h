
#pragma once

struct IDropTargetHelper;   // forward reference, in case the latest PSDK isn't installed.

// CListDropTarget 명령 대상입니다.
class CExportFileGenerateDlg;
class CListDropTarget : public COleDropTarget
{
	DECLARE_DYNAMIC(CListDropTarget)

public:
	CListDropTarget(CExportFileGenerateDlg* pList);
	virtual ~CListDropTarget();

protected:
	CExportFileGenerateDlg*		m_pListCtrl;
	IDropTargetHelper*			m_piDropHelper;
	bool						m_bUseDnDHelper;

	BOOL ReadHdropData(COleDataObject* pDataObject);


protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave(CWnd* pWnd);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
};


