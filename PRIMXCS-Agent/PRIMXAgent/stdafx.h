// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // ��ȭ ���ڿ��� MFC ��Ʈ�ѿ� ���� ������ �����մϴ�.

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include <afx.h>
#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
#include <afxtempl.h>
#include <process.h>
#include <Winsock2.h>
#pragma comment(lib,"Ws2_32.lib")
#include <Winsvc.h>
#include <afxmt.h>
#include <windows.h>
#include <time.h>
#include <sys/timeb.h>
#include "Resource.h"
#include "StrCnvt.h"
#include <crtdbg.h>

#include "CommonDefine.h"

#include "./EventLog/EVENT_MSG.h" // Event message ids
#include "ARIAEncryption.h"

//#pragma comment(lib,"legacy_stdio_definitions.lib")
//extern "C" {FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

#include "LogManager.h"
extern CLogManager theLogManager;


