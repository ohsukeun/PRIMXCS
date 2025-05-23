/**
@file NTService.cpp
@brief 윈도우즈 서비스 베이스 모듈 클래스
@date 2010/04/01
@version 0.1
@author hylee
*/

#include "stdafx.h"
#include "NTService.h"
#include "Resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// static variables
CNTService* CNTService::m_pThis = NULL;

CNTService::CNTService(const TCHAR* szServiceName,const TCHAR* szServiceDisName,const TCHAR* szServiceDesc)
{
    // copy the address of the current object so we can access it from
    // the static member callback functions. 
    // WARNING: This limits the application to only one CNTService object. 
    m_pThis = this;
    
	_tcsncpy_s(m_szServiceName,		_countof(m_szServiceName),		szServiceName,		_TRUNCATE );
	_tcsncpy_s(m_szServiceDisName,	_countof(m_szServiceDisName),	szServiceDisName,	_TRUNCATE );
	_tcsncpy_s(m_szServiceDesc,		_countof(m_szServiceDesc),		szServiceDesc,		_TRUNCATE );

    m_hEventSource		= NULL;

    // set up the initial service status 
    m_hServiceStatus					= NULL;
    m_Status.dwServiceType				= SERVICE_WIN32_OWN_PROCESS;
    m_Status.dwCurrentState				= SERVICE_STOPPED;
//    m_Status.dwControlsAccepted			= SERVICE_ACCEPT_STOP;
	m_Status.dwControlsAccepted = SERVICE_ACCEPT_POWEREVENT | SERVICE_ACCEPT_SESSIONCHANGE | SERVICE_ACCEPT_SHUTDOWN; // SCM의 중지버튼 비활성화, SERVICE_CONTROL_STOP 명령어가 처리되지 않음, 이 명령어 요청시 1052 에러가 발생함
	m_Status.dwWin32ExitCode			= 0;
    m_Status.dwServiceSpecificExitCode	= 0;
    m_Status.dwCheckPoint				= 0;
    m_Status.dwWaitHint					= 0;
    m_bIsRunning						= FALSE;

}

CNTService::~CNTService()
{
    if (m_hEventSource) {
        ::DeregisterEventSource(m_hEventSource);
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// Default command line argument parsing

// Returns TRUE if it found an arg it recognised, FALSE if not
// Note: processing some arguments causes output to stdout to be generated.
BOOL CNTService::ParseStandardArgs(int argc, TCHAR* argv[])
{
    return FALSE;
}

void CNTService::PutCopyright()
{
	CString Temp;
	Temp.LoadString(IDS_COPYRIGHT);
	_ftprintf(stderr,_T("%s"),LPCTSTR(Temp));
}

void CNTService::ServiceMainEx(DWORD dwArgc, LPTSTR* lpszArgv)
{

}

////////////////////////////////////////////////////////////////////////////////////////
// Install/uninstall routines

// Test if the service is currently installed
BOOL CNTService::IsInstalled()
{
    BOOL bResult = FALSE;

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (hSCM) {

        // Try to open the service
        SC_HANDLE hService = ::OpenService(hSCM,m_szServiceName,SERVICE_QUERY_CONFIG);
        if (hService) {
            bResult = TRUE;
            ::CloseServiceHandle(hService);

			IsServiceStart();
        }
		else{
			m_strErrorString = GetLastErrorString();
		}
        ::CloseServiceHandle(hSCM);
    }
	else{
		m_strErrorString = GetLastErrorString();
	}
    
    return bResult;
}

BOOL CNTService::Install()
{
    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM){
		m_strErrorString = GetLastErrorString();
		return FALSE;
	}

    // Get the executable file path
	TCHAR szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));

    // Create the service
    SC_HANDLE hService = ::CreateService(hSCM,
                                         m_szServiceName,
                                         m_szServiceDisName,
                                         SERVICE_ALL_ACCESS,
                                         SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS, //|SERVICE_INTERACTIVE_PROCESS,
                                         SERVICE_AUTO_START,        // start condition
                                         SERVICE_ERROR_NORMAL,
                                         szFilePath,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL);
    if (!hService) {
        ::CloseServiceHandle(hSCM);
		m_strErrorString = GetLastErrorString();
        return FALSE;
    }

	CString strServiceName;
	strServiceName.LoadString(IDS_SERVICE_DESCRIPTION);
	SERVICE_DESCRIPTION ServiceName;
	ServiceName.lpDescription = (LPTSTR)LPCTSTR(strServiceName);
	ChangeServiceConfig2(hService,SERVICE_CONFIG_DESCRIPTION,(LPVOID)&ServiceName);

	SERVICE_FAILURE_ACTIONS fa;
	fa.dwResetPeriod = 0; 
	fa.lpRebootMsg = NULL;
	fa.lpCommand = NULL;
	fa.cActions = 3;
	SC_ACTION sa[3];
	sa[0].Delay = 1; // One minute
	sa[0].Type = SC_ACTION_RESTART;
	sa[1].Delay = 1; // One minute
	sa[1].Type = SC_ACTION_RESTART;
	sa[2].Delay = 1; // One minute
	sa[2].Type = SC_ACTION_RESTART;
	fa.lpsaActions = sa;
	ChangeServiceConfig2(hService,SERVICE_CONFIG_FAILURE_ACTIONS,(LPVOID)&fa);

    // make registry entries to support logging messages
    // Add the source name as a subkey under the Application
    // key in the EventLog service portion of the registry.

    HKEY hKey = NULL;

	CString szKey;
	szKey.Format(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\%s"),m_szServiceName);

	if (::RegCreateKey(HKEY_LOCAL_MACHINE, LPCTSTR(szKey), &hKey) != ERROR_SUCCESS) {
        ::CloseServiceHandle(hService);
        ::CloseServiceHandle(hSCM);
		m_strErrorString = GetLastErrorString();
        return FALSE;
    }

    // Add the Event ID message-file name to the 'EventMessageFile' subkey.

	::RegSetValueEx(hKey,_T("EventMessageFile"),0,REG_EXPAND_SZ, (CONST BYTE*)szFilePath,_countof(szFilePath));

    // Set the supported types flags.
    DWORD dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
    ::RegSetValueEx(hKey,
                    _T("TypesSupported"),
                    0,
                    REG_DWORD,
                    (CONST BYTE*)&dwData,
                     sizeof(DWORD));
    ::RegCloseKey(hKey);

	CString strServiceDispayName;
	strServiceDispayName.LoadString(IDS_SERVICE_SERVICE_DISPLAY_NAME);

    LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_INSTALLED, strServiceDispayName);

    // tidy up
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return TRUE;
}

BOOL CNTService::Uninstall()
{
    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM){
		m_strErrorString = GetLastErrorString();
		return FALSE;
	}

	CString strServiceDispayName;
	strServiceDispayName.LoadString(IDS_SERVICE_SERVICE_DISPLAY_NAME);

    BOOL bResult = FALSE;
    SC_HANDLE hService = ::OpenService(hSCM,m_szServiceName,DELETE);
    if (hService) {
        if (::DeleteService(hService)) {
            LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_REMOVED, strServiceDispayName);
            bResult = TRUE;
        } else {
            LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_NOTREMOVED, strServiceDispayName);
        }
        ::CloseServiceHandle(hService);
    }
	else{
		m_strErrorString = GetLastErrorString();
	}
    
    ::CloseServiceHandle(hSCM);
    return bResult;
}

///////////////////////////////////////////////////////////////////////////////////////
// Logging functions

// This function makes an entry into the application event log
void CNTService::LogEvent(WORD wType, DWORD dwID,
                          const TCHAR* pszS1,
                          const TCHAR* pszS2,
                          const TCHAR* pszS3)
{
    const TCHAR* ps[3];
    ps[0] = pszS1;
    ps[1] = pszS2;
    ps[2] = pszS3;

    int iStr = 0;
    for (int i = 0; i < 3; i++) {
        if (ps[i] != NULL) iStr++;
    }
        
    // Check the event source has been registered and if
    // not then register it now
    if (!m_hEventSource) {
        m_hEventSource = ::RegisterEventSource(NULL,  // local machine
                                               m_szServiceName); // source name
    }

    if (m_hEventSource) {
        ::ReportEvent(m_hEventSource,
                      wType,
                      0,
                      dwID,
                      NULL, // sid
                      iStr,
                      0,
                      ps,
                      NULL);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Service startup and registration

BOOL CNTService::StartService()
{
    SERVICE_TABLE_ENTRY st[] = {
        {m_szServiceName, ServiceMain},
        {NULL, NULL}
    };
    BOOL b = ::StartServiceCtrlDispatcher(st);
    return b;
}

// static member function (callback)
void CNTService::ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	CNTService* pService = m_pThis;
	
	pService->ServiceMainEx(dwArgc,lpszArgv);

	pService->m_Status.dwCurrentState = SERVICE_START_PENDING;
    pService->m_hServiceStatus = RegisterServiceCtrlHandler(pService->m_szServiceName,Handler);
    if (pService->m_hServiceStatus == NULL) {
        pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_CTRLHANDLERNOTINSTALLED);
        return;
    }

    if (pService->Initialize()) {
        pService->m_bIsRunning = TRUE;
        pService->m_Status.dwWin32ExitCode = 0;
        pService->m_Status.dwCheckPoint = 0;
        pService->m_Status.dwWaitHint = 0;
        pService->Run();
    }
    pService->SetStatus(SERVICE_STOPPED);
	
	theLogManager.Close();

}

///////////////////////////////////////////////////////////////////////////////////////////
// status functions

void CNTService::SetStatus(DWORD dwState)
{
    m_Status.dwCurrentState = dwState;
    ::SetServiceStatus(m_hServiceStatus, &m_Status);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Service initialization

BOOL CNTService::Initialize()
{
    SetStatus(SERVICE_START_PENDING);
    
    BOOL bResult = OnInit(); 
    
    m_Status.dwWin32ExitCode = (FALSE == bResult?GetLastError():NO_ERROR);
    m_Status.dwCheckPoint = 0;
    m_Status.dwWaitHint = 0;
    if (!bResult) {
//      LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_FAILEDINIT);
        SetStatus(SERVICE_STOPPED);
        return FALSE;    
    }
    
    LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_STARTED);
    SetStatus(SERVICE_RUNNING);

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// main function to do the real work of the service

// This function performs the main work of the service. 
// When this function returns the service has stopped.
void CNTService::Run()
{
    while (m_bIsRunning) {
        Sleep(1000);
    }

}


//////////////////////////////////////////////////////////////////////////////////////
// Control request handlers

// static member function (callback) to handle commands from the
// service control manager
void CNTService::Handler(DWORD dwOpcode)
{
	// Get a pointer to the object
	CNTService* pService = m_pThis;

	switch (dwOpcode) {
	case SERVICE_CONTROL_STOP:			// 1
		pService->SetStatus(SERVICE_STOP_PENDING);
		pService->m_bIsRunning = FALSE;
		pService->OnStop();
		pService->LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_STOPPED);
		break;

	case SERVICE_CONTROL_PAUSE:			// 2
		pService->OnPause();
		break;

	case SERVICE_CONTROL_CONTINUE:		// 3
		pService->OnContinue();
		break;

	case SERVICE_CONTROL_INTERROGATE:	// 4
		pService->OnInterrogate();
		break;

	case SERVICE_CONTROL_SHUTDOWN:		// 5
		pService->OnShutdown();
		break;

	default:
		if (dwOpcode >= SERVICE_CONTROL_USER) {
			if (!pService->OnUserControl(dwOpcode)) {
				pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST);
			}
		} else {
			pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST);
		}
		break;
	}
	// Report current status
	::SetServiceStatus(pService->m_hServiceStatus, &pService->m_Status);

}
        
// Called when the service is first initialized
BOOL CNTService::OnInit()
{
	return TRUE;
}

// Called when the service control manager wants to stop the service
void CNTService::OnStop()
{
}

// called when the service is interrogated
void CNTService::OnInterrogate()
{
}

// called when the service is paused
void CNTService::OnPause()
{
}

// called when the service is continued
void CNTService::OnContinue()
{
}

// called when the service is shut down
void CNTService::OnShutdown()
{
}

// called when the service gets a user control message
BOOL CNTService::OnUserControl(DWORD dwOpcode)
{
    return FALSE; // say not handled
}

BOOL CNTService::GetPatchInfoCollectorStart()
{
	return FALSE;
}

BOOL CNTService::IsServiceStart()
{
    DWORD dwState = 0xFFFFFFFF;

    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if(NULL == hSCM){
		return FALSE;
	}

    SC_HANDLE hService = OpenService(hSCM,LPCTSTR(m_szServiceName),SERVICE_ALL_ACCESS);
    if(NULL == hService){
		CloseServiceHandle(hSCM);
		return FALSE;
	}   

    SERVICE_STATUS ss;
    memset(&ss, 0, sizeof(ss));
    BOOL b = QueryServiceStatus(hService,&ss);
    if(!b){
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCM);
		return FALSE;
    } 
	else {
        dwState = ss.dwCurrentState;
	}

	BOOL bStart = FALSE;

    switch (dwState) {
     case SERVICE_START_PENDING:
		bStart = TRUE;
		break;
    case SERVICE_RUNNING:
		bStart = TRUE;
		break;
    }
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);
	
	return bStart;
}


CString CNTService::GetLastErrorString()
{
	CString strError;

	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	strError = (LPCTSTR)lpMsgBuf;
	strError.Replace('\n',' ');
	strError.Replace('\r',' ');
	LocalFree( lpMsgBuf );

	return strError;
}

