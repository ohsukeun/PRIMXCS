#include "stdafx.h"
#include "TrayRefreshEx.h"

CTrayRefreshEx::CTrayRefreshEx()
{

}


CTrayRefreshEx::~CTrayRefreshEx()
{

}

void CTrayRefreshEx::Refresh()
{
	HWND hWnd_TrayToolbar = NULL;
	HWND hWnd_TrayToolButton = NULL;
	
	// ȭ��ǥ ��ư �������� Ʈ���̿� ���� �������� �����. (���� ������� Ʈ������ �ܻ��� �����.)
	hWnd_TrayToolbar = FindTrayToolbarWindow();
	if (NULL != hWnd_TrayToolbar){
		RemoveTrashTrayIcon(hWnd_TrayToolbar);
//		::CloseHandle(hWnd_TrayToolbar);
		hWnd_TrayToolbar = NULL;
	}
	
	// ȭ��ǥ ��ư�� ������ �� ��Ÿ���� �����쿡 ���� �������� �����.
	hWnd_TrayToolButton = FindTrayToolbarButton();
	if (NULL != hWnd_TrayToolButton){
		HWND hWnd_Overflow = NULL;
		// ��ư�� ������ �ڵ鿡 Down, Up �� �ѹ��� Ŭ�� ������ �Ͼ���� �Ѵ�. (������ â ����)
		::PostMessage(hWnd_TrayToolButton, WM_LBUTTONDOWN, 0, 0);
		::PostMessage(hWnd_TrayToolButton, WM_LBUTTONUP, 0, 0);
		hWnd_Overflow = FindTrayNotifyiconOverflowWindow();
		if (NULL != hWnd_Overflow){
			RemoveTrashTrayIcon(hWnd_Overflow);
//			::CloseHandle(hWnd_Overflow);
			hWnd_Overflow = NULL;
		}
		// ��ư�� ������ �ڵ鿡 Down, Up �� �ѹ��� Ŭ�� ������ �Ͼ���� �Ѵ�. (������ â �ݱ�)

		::PostMessage(hWnd_TrayToolButton, WM_LBUTTONDOWN, 0, 0);
		::PostMessage(hWnd_TrayToolButton, WM_LBUTTONUP, 0, 0);
//		::CloseHandle(hWnd_TrayToolButton);
		hWnd_TrayToolButton = NULL;
	}

}

// �⺻ Ʈ���� ������ �ڵ��� ��� �Լ�
HWND CTrayRefreshEx::FindTrayToolbarWindow(void) 
{
	HWND hWnd_ToolbarWindow32 = NULL;
	HWND hWnd_ShellTrayWnd = NULL;

	hWnd_ShellTrayWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);

	if (NULL != hWnd_ShellTrayWnd){
		HWND hWnd_TrayNotifyWnd = ::FindWindowEx(hWnd_ShellTrayWnd, NULL,_T("TrayNotifyWnd"), NULL);
		if (NULL != hWnd_TrayNotifyWnd){
			HWND hWnd_SysPager = ::FindWindowEx(hWnd_TrayNotifyWnd, NULL,_T("SysPager"), NULL);

			// WinXP ������ SysPager ���� ����            
			if (NULL != hWnd_SysPager){
				hWnd_ToolbarWindow32 = ::FindWindowEx(hWnd_SysPager, NULL,_T("ToolbarWindow32"), NULL);
			}
			else{ // Win2000 �� ��쿡�� SysPager �� ����
				hWnd_ToolbarWindow32 = ::FindWindowEx(hWnd_TrayNotifyWnd, NULL,	_T("ToolbarWindow32"), NULL);
			}
		}
	}
	return hWnd_ToolbarWindow32;
}

// Ʈ���� Ȯ�� ��ư�� �ڵ��� ��� �Լ�
HWND CTrayRefreshEx::FindTrayToolbarButton(void) 
{
	HWND hWnd_ToolbarButton = NULL;
	HWND hWnd_ShellTrayWnd = NULL;
	
	hWnd_ShellTrayWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
	if (NULL != hWnd_ShellTrayWnd){
		HWND hWnd_TrayNotifyWnd = ::FindWindowEx(hWnd_ShellTrayWnd, NULL,_T("TrayNotifyWnd"), NULL);
		if (NULL != hWnd_TrayNotifyWnd)	{
			hWnd_ToolbarButton = ::FindWindowEx(hWnd_TrayNotifyWnd, NULL,_T("Button"), NULL);
		}
	}
	return hWnd_ToolbarButton;
}

// Ʈ���� Ȯ�� ������ �ڵ��� ��� �Լ�
HWND CTrayRefreshEx::FindTrayNotifyiconOverflowWindow(void) 
{
	HWND hWnd_NotifyiconOverflowWindow = NULL;
	HWND hWnd_ToolbarWindow32 = NULL;

	hWnd_NotifyiconOverflowWindow = ::FindWindow(_T("NotifyiconOverflowWindow"), NULL);

	if (hWnd_NotifyiconOverflowWindow){
		hWnd_ToolbarWindow32 = ::FindWindowEx(hWnd_NotifyiconOverflowWindow, NULL,_T("ToolbarWindow32"), NULL);
	}
	return hWnd_ToolbarWindow32;

}

// ������ ��ġ�� WM_MOUSEMOVE �ѱ�� �Լ�
void CTrayRefreshEx::RemoveTrashTrayIcon(HWND hTrayWnd) 
{
	RECT rtToolBar;
	memset(&rtToolBar, 0x00, sizeof(RECT));

	if (FALSE != ::GetClientRect(hTrayWnd, &rtToolBar))	{
		for (int nX = 0; nX < rtToolBar.right - rtToolBar.left; nX += 16){
			for (int nY = 0; nY < rtToolBar.bottom - rtToolBar.top; nY += 16){
				::PostMessage(hTrayWnd, WM_MOUSEMOVE, 0, MAKELPARAM(nX, nY));
			}
		}
	}
}