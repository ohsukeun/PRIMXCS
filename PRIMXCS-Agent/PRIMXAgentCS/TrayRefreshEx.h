#pragma once

class CTrayRefreshEx
{
public:
	CTrayRefreshEx();
	~CTrayRefreshEx();

	void Refresh();

protected:
	HWND FindTrayToolbarWindow(void);				// �⺻ Ʈ���� ������ �ڵ��� ��� �Լ�
	HWND FindTrayToolbarButton(void);				// Ʈ���� Ȯ�� ��ư�� �ڵ��� ��� �Լ�
	HWND FindTrayNotifyiconOverflowWindow(void);	// Ʈ���� Ȯ�� ������ �ڵ��� ��� �Լ�
	void RemoveTrashTrayIcon(HWND hTrayWnd);		// ������ ��ġ�� WM_MOUSEMOVE �ѱ�� �Լ�

};

