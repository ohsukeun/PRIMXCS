// PRIMXEncMonitor.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "PRIMXEncMonitor.h"
#include "LogManager.h"
#include "FileChangeMonitor.h"
#include <conio.h>
#include <stdio.h>
#include "FileVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������ ���� ���α׷� ��ü�Դϴ�.

CWinApp theApp;
CLogManager theLogManager;

using namespace std;

int main()
{
    int nRetCode = 0;

	///////////////////////////////////////////////////////////////////////////
	CString LocaleString;
	LANGID SysLangID = GetSystemDefaultLangID();

	if (MAKELANGID(LANG_KOREAN, SUBLANG_KOREAN) == SysLangID) {
		LocaleString.Format(_T("Korean"));
	}
	else if (MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) == SysLangID) {
		LocaleString.Format(_T("English"));
	}
	else if (MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED) == SysLangID) {
		LocaleString.Format(_T("chinese-simplified"));
	}
	else if (MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL) == SysLangID) {
		LocaleString.Format(_T("chinese-traditional"));
	}
	else if (MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT) == SysLangID) {
		LocaleString.Format(_T("Japanese"));
	}
	else {
		LocaleString.Format(_T("English"));
	}
	_tsetlocale(LC_ALL, LPCTSTR(LocaleString));

	///////////////////////////////////////////////////////////////////////////
	
    HMODULE hModule = ::GetModuleHandle(nullptr);
	
    if (hModule != nullptr)
    {
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            wprintf(L"�ɰ��� ����: MFC�� �ʱ�ȭ���� ���߽��ϴ�.\n");
            nRetCode = 1;
        }
        else
        {
			CFileChangeMonitor FileChangeMonitor;
			CFileVersion FileVersion;
			FileVersion.Init();
			theLogManager.SetPath(_T("Monitor"), FileChangeMonitor.GetProgramDataPath());
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("=========================================================================================="));
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("PRIMXEncMonitor Start. v%s"), FileVersion.GetFileVersion());
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("=========================================================================================="));
			
			if (FALSE == FileChangeMonitor.SetMonitorStart(4)) {
				FileChangeMonitor.SetMonitorStop();
			}
			else {
				int nLogRemoveHour = 0;
				UINT nCount = 0;

				while (1) {
					nCount++;
					if (10000 < nCount) {
						COleDateTime tCurrentTime = COleDateTime::GetCurrentTime();
						if (nLogRemoveHour != tCurrentTime.GetHour()) {
							theLogManager.SetLogFileDelete(30);
							nLogRemoveHour = tCurrentTime.GetHour();
						}
						nCount = 0;
					}

					if (0 < _kbhit())
					{
						int Key = _getch();
						if (27 == Key)
						{
							theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("The service stop [ESC key down] .....!!"));
							break;
						}
						else if (13 == Key)
						{
							_ftprintf(stderr, _T("\n"));
						}
						else if (45 == Key)
						{
							theLogManager.LogPrintLine();
						}
						else if (8 == Key)
						{
							system("cls");
						}
						else
						{
							_ftprintf(stderr, _T("%c"), Key);
						}
					}
					Sleep(100);
				}
				FileChangeMonitor.SetMonitorStop();
			}
        }
    }
    else
    {
        wprintf(L"�ɰ��� ����: GetModuleHandle ����\n");
        nRetCode = 1;
    }

    return nRetCode;
}
