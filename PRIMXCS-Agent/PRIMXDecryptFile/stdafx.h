
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


#pragma comment(lib, "legacy_stdio_definitions.lib")
extern "C"
{
	FILE __iob_func[3] = { stdin ,stdout,stderr };
}


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
#include "Resource.h"

#define 	ALERT_MSGBOX(str)		AfxMessageBox(str,MB_ICONEXCLAMATION)
#define 	QUESTION_MSGBOX(str)	AfxMessageBox(str,MB_ICONQUESTION | MB_YESNO)
#define 	STOP_MSGBOX(str)		AfxMessageBox(str,MB_ICONSTOP)
#define 	INFOR_MSGBOX(str)		AfxMessageBox(str,MB_ICONINFORMATION)


#define PRIMX_EX_FILE_DLL_CREATE_ERROR			100			// DLL ���� ����
#define PRIMX_EX_FILE_DATA_DAMAGE_ERROR			101			// �̵����� �ּ�
#define PRIMX_EX_FILE_SERVER_CONNECT_FAIL		102			// ���� ���� ����
#define PRIMX_EX_FILE_AUTH_FAIL					200			// ���� ����
#define PRIMX_EX_FILE_DECRYPT_FAIL				300			// ���� ���� ����



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


