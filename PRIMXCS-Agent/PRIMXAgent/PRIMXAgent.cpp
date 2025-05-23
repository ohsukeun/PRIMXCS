// PRIMXAgent.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "PRIMXAgent.h"
#include "LogManager.h"
#include "ControlService.h"
#include "SNIDSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 유일한 응용 프로그램 개체입니다.

CWinApp theApp;
CLogManager theLogManager;

BOOL CSNIDSocket::m_bSocketInit = FALSE;
BOOL CSNIDSocket::m_bSSLInit = FALSE;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    int nRetCode = 0;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	CString LocaleString = _T("");
	LANGID SysLangID = GetSystemDefaultLangID();

	if (MAKELANGID(LANG_KOREAN, SUBLANG_KOREAN) == SysLangID) {
		LocaleString = _T("Korean");
	}
	else {
		LocaleString = _T("English");
	}
	_tsetlocale(LC_ALL, LPCTSTR(LocaleString));
	
    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 오류 코드를 필요에 따라 수정합니다.
            wprintf(L"심각한 오류: MFC를 초기화하지 못했습니다.\n");
            nRetCode = 1;
        }
        else
        {
#if _DEBUG
			system("cls");
			CString strServiceNameConsole;
			strServiceNameConsole.LoadString(IDS_SERVICE_NAME);
			SetConsoleTitle(strServiceNameConsole);
			SMALL_RECT windowSize = { 0, 0, 119, 60 };
			SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
			COORD bufferSize = { 120, 300 };
			SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);
#endif
			if (argc == 2 && _tcsicmp(argv[1], _T("/n")) == 0) {
				for (int i = 0; envp[i] != NULL; ++i) {
					_ftprintf(stderr, _T("%3d: %s\n"), i, envp[i]);
					TRACE(_T("%3d: %s\n"), i, envp[i]);
				}
			}
			else {
				BOOL bDebug = FALSE;
				BOOL bDebugWrite = FALSE;
				for (int i = 1; i<argc; i++) {
					if (_tcsicmp(argv[i], _T("-debug")) == 0) {
						bDebug = TRUE;
					}
					if (_tcsicmp(argv[i], _T("-lw")) == 0) {
						bDebugWrite = TRUE;
					}
				}

				CString StringTable;
				CString strServiceName;
				CString strServiceDispayName;

				StringTable.LoadString(IDS_SERVICE_DESCRIPTION);
				strServiceDispayName.LoadString(IDS_SERVICE_SERVICE_DISPLAY_NAME);
				strServiceName.LoadString(IDS_SERVICE_NAME);

				CControlService Service(strServiceName, strServiceDispayName, StringTable);

				theLogManager.SetPath(_T("Agent"), Service.GetProgramDataPath());
				if (TRUE == bDebug) {
					theLogManager.SetPrintLog();

					if (TRUE == bDebugWrite) theLogManager.SetDebugLogWrite();

					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s Service Debug Mode Start......[%d] "), strServiceName, FD_SETSIZE);
					Service.m_bIsRunning = TRUE;

					if (FALSE == Service.OnInit()) {
						theLogManager.LogPrint(LOG_ERR, GetCurrentThreadId(), _T("OnInit Error"));
					}
					else {
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("Run"));
						Service.Run();
						Service.OnStop();
					}
				}
				else {
					if (!Service.ParseStandardArgs(argc, argv)) {
						Service.StartService();
					}
				}
				return Service.m_Status.dwWin32ExitCode;
			}
		}
    }
    else
    {
        // TODO: 오류 코드를 필요에 따라 수정합니다.
        wprintf(L"심각한 오류: GetModuleHandle 실패\n");
        nRetCode = 1;
    }

    return nRetCode;
}
