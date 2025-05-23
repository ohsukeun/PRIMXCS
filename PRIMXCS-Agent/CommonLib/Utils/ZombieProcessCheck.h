#pragma once


class CZombieProcessCheck
{
public:
	CZombieProcessCheck();
	~CZombieProcessCheck();

	BOOL SetZombieProcessCheck(CString strProcessName);
	BOOL SetZombieProcessCheck();
	
	void SetZombieProcessKill();
	void SetZombieProcessKill(CList<DWORD, DWORD>& ZombieIDList);

protected:

	int GetProcessID(CString strProcessName);
	BOOL GetProcessThreadCount(DWORD dwProcessID,UINT& nThreadCount);
	BOOL GetProcessHandleCountEx(DWORD dwProcessID, UINT& nHandleCount);
	CString SetSplitPathFileName(CString strPath);
	BOOL SetCreateProcesseEx(CString strProcess, BOOL bWait = FALSE, WORD wShowWindow = SW_HIDE);

	CList<DWORD, DWORD>			m_ProcessIDList;
	CList<DWORD, DWORD>			m_ZombieIDList;

};

