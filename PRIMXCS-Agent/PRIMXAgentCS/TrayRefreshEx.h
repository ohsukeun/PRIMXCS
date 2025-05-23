#pragma once

class CTrayRefreshEx
{
public:
	CTrayRefreshEx();
	~CTrayRefreshEx();

	void Refresh();

protected:
	HWND FindTrayToolbarWindow(void);				// 기본 트레이 윈도우 핸들을 얻는 함수
	HWND FindTrayToolbarButton(void);				// 트레이 확장 버튼의 핸들을 얻는 함수
	HWND FindTrayNotifyiconOverflowWindow(void);	// 트레이 확장 윈도우 핸들을 얻는 함수
	void RemoveTrashTrayIcon(HWND hTrayWnd);		// 아이콘 위치에 WM_MOUSEMOVE 넘기는 함수

};

