
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#pragma comment(lib, "legacy_stdio_definitions.lib")
extern "C"
{
	FILE __iob_func[3] = { stdin ,stdout,stderr };
}


#include "LogManager.h"
extern CLogManager theLogManager;


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원


#include <time.h>
#include <sys/timeb.h>
#include "CommonDefine.h"
#include "StrCnvt.h"
#include "Resource.h"

#define 	ALERT_MSGBOX(str)		AfxMessageBox(str,MB_ICONEXCLAMATION)
#define 	QUESTION_MSGBOX(str)	AfxMessageBox(str,MB_ICONQUESTION | MB_YESNO)
#define 	STOP_MSGBOX(str)		AfxMessageBox(str,MB_ICONSTOP)
#define 	INFOR_MSGBOX(str)		AfxMessageBox(str,MB_ICONINFORMATION)


#define PRIMX_EX_FILE_DLL_CREATE_ERROR			100			// DLL 생성 실패
#define PRIMX_EX_FILE_DATA_DAMAGE_ERROR			101			// 이동파일 휘손
#define PRIMX_EX_FILE_SERVER_CONNECT_FAIL		102			// 서버 연결 실패
#define PRIMX_EX_FILE_AUTH_FAIL					200			// 인증 실패
#define PRIMX_EX_FILE_DECRYPT_FAIL				300			// 파일 추출 실패



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


