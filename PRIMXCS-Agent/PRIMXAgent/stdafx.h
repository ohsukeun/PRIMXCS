// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // 대화 상자에서 MFC 컨트롤에 대한 지원을 제거합니다.

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include <afx.h>
#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows 공용 컨트롤에 대한 MFC 지원입니다.
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


