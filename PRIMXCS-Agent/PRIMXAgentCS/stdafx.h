
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.
#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.

#define EVMSG_ERROR			1
#define EVMSG_ALERT			2
#define EVMSG_INFOR			3	


#include "LogManager.h"
extern CLogManager theLogManager;

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����
#include <time.h>
#include <sys/timeb.h>

#include "CommonDefine.h"
#include "StrCnvt.h"
#include "ARIAEncryption.h"
#include "Resource.h"
#include <vector>
#include <afxole.h>
using namespace std;

extern UINT g_uCustomClipbrdFormat;


#define WM_TRAY_EVENT					WM_USER + 200	// Ʈ���� �̺�Ʈ ó�� ���� ����


#define 	ALERT_MSGBOX(str)		AfxMessageBox(str,MB_ICONEXCLAMATION)
#define 	QUESTION_MSGBOX(str)	AfxMessageBox(str,MB_ICONQUESTION | MB_YESNO)
#define 	STOP_MSGBOX(str)		AfxMessageBox(str,MB_ICONSTOP)
#define 	INFOR_MSGBOX(str)		AfxMessageBox(str,MB_ICONINFORMATION)


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


