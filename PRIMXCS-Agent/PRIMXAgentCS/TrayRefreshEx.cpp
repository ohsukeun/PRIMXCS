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
	
	// 화살표 버튼 누르기전 트레이에 남은 아이콘을 지운다. (기존 방법으로 트레이의 잔상을 지운다.)
	hWnd_TrayToolbar = FindTrayToolbarWindow();
	if (NULL != hWnd_TrayToolbar){
		RemoveTrashTrayIcon(hWnd_TrayToolbar);
//		::CloseHandle(hWnd_TrayToolbar);
		hWnd_TrayToolbar = NULL;
	}
	
	// 화살표 버튼을 눌렀을 때 나타나는 윈도우에 남은 아이콘을 지운다.
	hWnd_TrayToolButton = FindTrayToolbarButton();
	if (NULL != hWnd_TrayToolButton){
		HWND hWnd_Overflow = NULL;
		// 버튼의 윈도우 핸들에 Down, Up 즉 한번의 클릭 동작이 일어나도록 한다. (윈도우 창 열기)
		::PostMessage(hWnd_TrayToolButton, WM_LBUTTONDOWN, 0, 0);
		::PostMessage(hWnd_TrayToolButton, WM_LBUTTONUP, 0, 0);
		hWnd_Overflow = FindTrayNotifyiconOverflowWindow();
		if (NULL != hWnd_Overflow){
			RemoveTrashTrayIcon(hWnd_Overflow);
//			::CloseHandle(hWnd_Overflow);
			hWnd_Overflow = NULL;
		}
		// 버튼의 윈도우 핸들에 Down, Up 즉 한번의 클릭 동작이 일어나도록 한다. (윈도우 창 닫기)

		::PostMessage(hWnd_TrayToolButton, WM_LBUTTONDOWN, 0, 0);
		::PostMessage(hWnd_TrayToolButton, WM_LBUTTONUP, 0, 0);
//		::CloseHandle(hWnd_TrayToolButton);
		hWnd_TrayToolButton = NULL;
	}

}

// 기본 트레이 윈도우 핸들을 얻는 함수
HWND CTrayRefreshEx::FindTrayToolbarWindow(void) 
{
	HWND hWnd_ToolbarWindow32 = NULL;
	HWND hWnd_ShellTrayWnd = NULL;

	hWnd_ShellTrayWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);

	if (NULL != hWnd_ShellTrayWnd){
		HWND hWnd_TrayNotifyWnd = ::FindWindowEx(hWnd_ShellTrayWnd, NULL,_T("TrayNotifyWnd"), NULL);
		if (NULL != hWnd_TrayNotifyWnd){
			HWND hWnd_SysPager = ::FindWindowEx(hWnd_TrayNotifyWnd, NULL,_T("SysPager"), NULL);

			// WinXP 에서는 SysPager 까지 추적            
			if (NULL != hWnd_SysPager){
				hWnd_ToolbarWindow32 = ::FindWindowEx(hWnd_SysPager, NULL,_T("ToolbarWindow32"), NULL);
			}
			else{ // Win2000 일 경우에는 SysPager 가 없음
				hWnd_ToolbarWindow32 = ::FindWindowEx(hWnd_TrayNotifyWnd, NULL,	_T("ToolbarWindow32"), NULL);
			}
		}
	}
	return hWnd_ToolbarWindow32;
}

// 트레이 확장 버튼의 핸들을 얻는 함수
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

// 트레이 확장 윈도우 핸들을 얻는 함수
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

// 아이콘 위치에 WM_MOUSEMOVE 넘기는 함수
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