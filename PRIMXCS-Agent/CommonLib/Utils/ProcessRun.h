#pragma once


#define PRIMX_SW_FORCEMINIMIZE 		11 		// Minimizes a window, even if the thread that owns the window is not responding. This flag should only be used when minimizing windows from a different thread.
#define PRIMX_SW_HIDE 				0  		// Hides the window and activates another window.
#define PRIMX_SW_MAXIMIZE 			3  		// Maximizes the specified window.
#define PRIMX_SW_MINIMIZE 			6  		// Minimizes the specified window and activates the next top-level window in the Z order.
#define PRIMX_SW_RESTORE 			9  		// Activates and displays the window. If the window is minimized or maximized, the system restores it to its original size and position. An application should specify this flag when restoring a minimized window.
#define PRIMX_SW_SHOW 				5  		// Activates the window and displays it in its current size and position.
#define PRIMX_SW_SHOWDEFAULT 		10 		// Sets the show state based on the SW_ value specified in the STARTUPINFO structure passed to the CreateProcess function by the program that started the application.
#define PRIMX_SW_SHOWMAXIMIZED 		3  		// Activates the window and displays it as a maximized window.
#define PRIMX_SW_SHOWMINIMIZED 		2  		// Activates the window and displays it as a minimized window.
#define PRIMX_SW_SHOWMINNOACTIVE 	7  		// Displays the window as a minimized window. This value is similar to SW_SHOWMINIMIZED, except the window is not activated.
#define PRIMX_SW_SHOWNA 				8  		// Displays the window in its current size and position. This value is similar to SW_SHOW, except that the window is not activated.
#define PRIMX_SW_SHOWNOACTIVATE 		4  		// Displays a window in its most recent size and position. This value is similar to SW_SHOWNORMAL, except that the window is not activated.
#define PRIMX_SW_SHOWNORMAL 			1  		// Activates and displays a window. If the window is minimized or maximized, the system restores it to its original size and position. An application should specify this flag when displaying the window for the first time.

class CProcessRun
{
public:
	CProcessRun(void);
	~CProcessRun(void);

	CString GetLocalPath();
	BOOL isProcess(CString strProcess);
	BOOL isProcessSessionWithKill(CString strProcess,DWORD dwSessionID);
	BOOL CreateProcessOnAdminAccount(CString strProcess, WORD wShowWindow = PRIMX_SW_SHOW);
	BOOL CreateProcessOnUserAccount(CString strProcess, WORD wShowWindow = PRIMX_SW_SHOW);
	BOOL CreateProcessOnUserAccountEx(CString strProcess, WORD wShowWindow = PRIMX_SW_SHOW);
	BOOL CreateProcessOnAdminAccountOfLockIgnore(CString strProcess, WORD wShowWindow = PRIMX_SW_SHOW);
	BOOL CreateProcessOnAdminOrUser(CString strProcess, WORD wShowWindow = PRIMX_SW_SHOW);

	void SetKillProcess(CString strProcessName);
	BOOL SetKillProcess(CString strProcessName, CString& strKillProcessPath, CString& strErrorString);
	BOOL SetKillProcess(DWORD PID, CString& strKillProcessPath, CString& strErrorString);
	BOOL SetCreateProcesseEx(CString strProcess, BOOL bWait = FALSE, WORD wShowWindow = SW_HIDE);
	CString GetLastErrorString();
	CString GetLastErrorStringEx();
	DWORD WTSGetActiveConsoleSessionIdEx();

	BOOL CreateProcessOnAdminAccountForThr(CString strProcess, WORD wShowWindow = PRIMX_SW_SHOW);
	BOOL CreateProcessOnUserAccountForThr(CString strProcess, WORD wShowWindow = PRIMX_SW_SHOW);

	DWORD GetProcessPID(CString strProcessName);
	
	CString GetProcessUserName();
	BOOL IsAdministrator();

	void SetWaitExitCodeOption(BOOL bOption = FALSE, DWORD  dwMilliseconds = 5000) { m_bWaitExitCodeOption = bOption; m_dwMilliseconds = dwMilliseconds; };
	DWORD GetExitCode() { return m_dwExitCode; }

	BOOL IsDesktopOpen();

	BOOL CreateProcessOnAdminAccountEx(CString strProcess, WORD wShowWindow = PRIMX_SW_SHOW);
	BOOL SetCreateProcesseOnAdmin(CString strProcess, DWORD& dwExitCode, DWORD dwTimeout = 600);

protected:
	BOOL CreateProcessOnAdminEx(CString strProcess, WORD wShowWindow = PRIMX_SW_SHOW);
	BOOL isWindowsVistaLater();
	BOOL IsAdminUser(CString strUserName);
	BOOL GetCurrentSessionAdmin();
	BOOL GetUACEnable();
	BOOL GetProcessElevation(TOKEN_ELEVATION_TYPE *pElevationType, BOOL *pIsAdmin);

	BOOL CreateProcessOnAdminAccountOfLockIgnore();
	BOOL CreateProcessOnAdminAccountForThr();
	BOOL CreateProcessOnUserAccountForThr();


	static unsigned __stdcall __LOCK_IGNORE_RUN__(void* /**<  void형 포인터 */);
	static unsigned __stdcall __USER_ACCOUNT_RUN__(void* /**<  void형 포인터 */);
	static unsigned __stdcall __ADMIN_ACCOUNT_RUN__(void* /**<  void형 포인터 */);

	CString			m_strProcess;
	WORD			m_wShowWindow;
	DWORD			m_dwExitCode;
	BOOL			m_bWaitExitCodeOption;
	DWORD			m_dwMilliseconds;
	CString			m_strLastErrorString;

private:
	BOOL LaunchAppIntoDifferentSession(LPCTSTR lpAppName);
	BOOL SendCloseMessage(const DWORD & dwPid);
	BOOL CompareStringWithWildCard(CString strNomal, CString strWildCard);
	BOOL IsWildCard(CString strData);
	CString GetReplaceByRegExp(CString strData); // 정규표현식으로 치환
	BOOL GetOSVersion(OSVERSIONINFOEX* OSVer);
};