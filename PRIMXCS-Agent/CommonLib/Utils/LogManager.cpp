
//----------------------------------------------------------------
// 로그를 파일로 저장하는 클래스.
//
//----------------------------------------------------------------

#include "stdafx.h"
#include "LogManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static TCHAR STR_PRIORYTY[8][5] = {
	{_T("M")},
	{_T("A")},
	{_T("C")},
	{_T("E")},
	{_T("W")},
	{_T("N")},
	{_T("I")},
	{_T("D")}
};

CLogManager::CLogManager()
{

	m_DebugLogWrite = TRUE;
	m_PrintLog		= TRUE;
	m_ThreadID		= 0;
	m_Path			= _T("C:");
	m_hEventSource	= NULL;
	ZeroMemory(m_szServiceName,sizeof(m_szServiceName));

	InitializeCriticalSectionAndSpinCount(&m_Lock,4000);

}

CLogManager::~CLogManager()
{
	Close();
	DeleteCriticalSection(&m_Lock);

}

void CLogManager::Close()
{
	EnterCriticalSection(&m_Lock);

	LeaveCriticalSection(&m_Lock);
}
void CLogManager::SetServiceName(TCHAR *Name)
{
	if(NULL == Name) return;
	_tcscpy_s(m_szServiceName,SERVICE_NAME_LEN,Name);

}
void CLogManager::LogPrint(UINT Prioryty, const TCHAR* pszFormat, ...)
{
	if(FALSE == m_PrintLog && FALSE == m_DebugLogWrite) 
	{
		return;
	}

	EnterCriticalSection(&m_Lock);

	TCHAR buf[1024] = {0};
	*buf = Prioryty;
	va_list arglist;
	va_start(arglist, pszFormat);
	_vsntprintf_s(buf,1024,_TRUNCATE,pszFormat, arglist);
	va_end(arglist);

	if(TRUE == m_PrintLog) 
	{
		_ftprintf(stderr,_T("%s %s [----] %s \n"), LPCTSTR(GetTime()), (7<Prioryty?_T("-"):STR_PRIORYTY[Prioryty]), buf);
	}

	if(TRUE == m_DebugLogWrite)
	{
		LogWrite(_T("%s [----] %s"), (7<Prioryty?_T("-"):STR_PRIORYTY[Prioryty]), buf);
	}
	
	LeaveCriticalSection(&m_Lock);
}

void CLogManager::LogPrint(UINT Prioryty,DWORD ThreadID,const TCHAR* pszFormat, ...)
{
	if(FALSE == m_PrintLog && FALSE == m_DebugLogWrite) 
	{
		return;
	}

	EnterCriticalSection(&m_Lock);
	TCHAR buf[1024] = {0};
	*buf = Prioryty;
	va_list arglist;
	va_start(arglist, pszFormat);
	_vsntprintf_s(buf,1024,_TRUNCATE,pszFormat, arglist);
	va_end(arglist);	
	if(TRUE == m_PrintLog) 
	{
		_ftprintf(stderr,_T("%s %s [%6lu] %s \n"),LPCTSTR(GetTime()),(7<Prioryty?_T("-"):STR_PRIORYTY[Prioryty]),ThreadID,buf);

		if(LOG_WAR >= Prioryty)
		{
			if(FALSE == m_DebugLogWrite)
			{
				LogWrite(_T("%s [%6lu] %s"),(7<Prioryty?_T("-"):STR_PRIORYTY[Prioryty]),ThreadID,buf);
			}
		}
	}

	if(TRUE == m_DebugLogWrite)
	{
		LogWrite(_T("%s [%6lu] %s"),(7<Prioryty?_T("-"):STR_PRIORYTY[Prioryty]),ThreadID,buf);
	}

	LeaveCriticalSection(&m_Lock);
}

void CLogManager::LogEventEx(UINT Prioryty,DWORD ThreadID,const TCHAR* pszFormat, ...)
{
#ifdef _WINDOWS_SERVICE_
	EnterCriticalSection(&m_Lock);
	TCHAR buf[2048] = {0};
	*buf = Prioryty;
	va_list arglist;
	va_start(arglist, pszFormat);
	_vsntprintf_s(buf,2048,_TRUNCATE,pszFormat, arglist);
	va_end(arglist);	
	_ftprintf(stderr,_T("%s %s [%6lu] %s \n"),LPCTSTR(GetTime()),(7<Prioryty?_T("-"):STR_PRIORYTY[Prioryty]),ThreadID,buf);

	if(TRUE == m_DebugLogWrite){
		LogWrite(_T("%s [%6lu] %s"),(7<Prioryty?_T("-"):STR_PRIORYTY[Prioryty]),ThreadID,buf);
	}

	if(LOG_ERR == Prioryty){
		LogEvent(EVENTLOG_ERROR_TYPE,EVMSG_ERROR,buf);
	}
	else if(LOG_WAR == Prioryty){
		LogEvent(EVENTLOG_WARNING_TYPE,EVMSG_ALERT,buf);
	}
	else if(LOG_INF == Prioryty){
		LogEvent(EVENTLOG_INFORMATION_TYPE,EVMSG_INFOR,buf);
	}
	LeaveCriticalSection(&m_Lock);
#endif
}

void CLogManager::LogEvent(WORD wType, DWORD dwID,
	const TCHAR* pszS1,
	const TCHAR* pszS2,
	const TCHAR* pszS3)
{
	EnterCriticalSection(&m_Lock);
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
		m_hEventSource = ::RegisterEventSource(NULL, m_szServiceName); 
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
	LeaveCriticalSection(&m_Lock);
}
void CLogManager::LogWrite(TCHAR *pszFormat, ...) 
{ 
	EnterCriticalSection(&m_Lock);
	TCHAR buf2[100] = {0};
	TCHAR buf[1024] = {0};
	CString Path;

	if (0 == m_strPrefixSting.GetLength()) {
		Path.Format(_T("%s\\%s\\%s.log"), LPCTSTR(m_Path), TMS_WORK_DIR_LOG, LPCTSTR(GetTime(FALSE)));
	}
	else {
		Path.Format(_T("%s\\%s\\%s_%s.log"), LPCTSTR(m_Path), TMS_WORK_DIR_LOG, m_strPrefixSting, LPCTSTR(GetTime(FALSE)));
	}

	_stprintf_s(buf2,100,_T("%s"),LPCTSTR(GetTime()));

	va_list arglist;
	va_start(arglist, pszFormat);
	_vsntprintf_s(buf,1024,_TRUNCATE,pszFormat, arglist);
	va_end(arglist);

	FILE *f = NULL; 
	if(0 == _tfopen_s(&f,LPCTSTR(Path), _T("at,ccs=UTF-8"))){
		_ftprintf(f,_T("%s %s\n"),buf2,buf); 
		fclose(f); 
	}
//	if(TRUE == m_PrintLog) _ftprintf(stderr,_T("%s - [000] %s \n"),LPCTSTR(GetTime()),buf);

	LeaveCriticalSection(&m_Lock);

} 

CString CLogManager::GetPath()
{
	return m_Path;
}

void CLogManager::SetPath(CString strPrefixSting,CString Path)
{
	EnterCriticalSection(&m_Lock);
	CString LogPath;
	m_Path = Path;
	m_strPrefixSting = strPrefixSting;
	m_strPrefixSting.Trim();
	LogPath.Format(_T("%s\\%s"),LPCTSTR(Path),TMS_WORK_DIR_LOG);
	CreateDirectory(LPCTSTR(LogPath),NULL);
	LeaveCriticalSection(&m_Lock);
}

CString CLogManager::GetTime(BOOL bTime)
{
	CString ReData;
	_tzset();
	struct __timeb64 tstruct;
	_ftime64_s( &tstruct );
	struct tm ConvertTime;

	if(0 == _localtime64_s(&ConvertTime,&tstruct.time))
	{
		if(TRUE == bTime)
		{
			ReData.Format(_T("%02d:%02d:%02d"),	ConvertTime.tm_hour,
				ConvertTime.tm_min,
				ConvertTime.tm_sec);
		}
		else
		{
			ReData.Format(_T("%04d-%02d-%02d"),	ConvertTime.tm_year+1900,
				ConvertTime.tm_mon+1,
				ConvertTime.tm_mday);			
		}
	}

	return ReData;
}

void CLogManager::LogPrintLine()
{
	if(FALSE == m_PrintLog && FALSE == m_DebugLogWrite) 
	{
		return;
	}

	EnterCriticalSection(&m_Lock);

	TCHAR TitleBuf[COMMAND_WINDOWS_SIZE-COMMAND_INFO_SIZE] = {0};

	wmemset(TitleBuf,_T('-'),COMMAND_WINDOWS_SIZE-COMMAND_INFO_SIZE-1);

	if(TRUE == m_PrintLog) 
	{
		_ftprintf(stderr,_T("%s * %s\n"),LPCTSTR(GetTime()),TitleBuf);
	}

	if(TRUE == m_DebugLogWrite)
	{
		LogWrite(_T("* %s"),TitleBuf);
	}		
	
	LeaveCriticalSection(&m_Lock);
}

void CLogManager::LogPrintTitle(const TCHAR* pszFormat, ...)
{
	if(FALSE == m_PrintLog && FALSE == m_DebugLogWrite) 
	{
		return;
	}

	EnterCriticalSection(&m_Lock);
	TCHAR buf[60] = {0};
	TCHAR TitleBuf[COMMAND_WINDOWS_SIZE-COMMAND_INFO_SIZE] = {0};

	va_list arglist;
	va_start(arglist, pszFormat);
	_vsntprintf_s(buf,60,_TRUNCATE,pszFormat, arglist);
	va_end(arglist);
		
	int Temp1 = (COMMAND_WINDOWS_SIZE - COMMAND_INFO_SIZE-1 - int(wcslen(buf)*2)) / 2;
	wmemset(TitleBuf,_T('-'),COMMAND_WINDOWS_SIZE-COMMAND_INFO_SIZE-1);
	memcpy(TitleBuf+Temp1,buf,wcslen(buf)*2);
	if(TRUE == m_PrintLog) 
	{
		_ftprintf(stderr,_T("%s * %s\n"),LPCTSTR(GetTime()),TitleBuf);
	}
				
	if(TRUE == m_DebugLogWrite)
	{
		LogWrite(_T("* %s"),TitleBuf);
	}

	LeaveCriticalSection(&m_Lock);
}

void CLogManager::SetDebugLogWrite()
{
	m_DebugLogWrite = TRUE;
}

void CLogManager::SetPrintLog()
{
	m_PrintLog = TRUE;
}
void CLogManager::LogPrintRawData(const char *pData, int nLen) 
{
	EnterCriticalSection(&m_Lock);
	unsigned char DisplayBuffer[100] = {0};
	unsigned char DisplayBuffer2[100] = {0};

	unsigned char* temp = (unsigned char*)pData;
	int Index = 0;

	
	LogPrintTitle(_T(" Memory Dump [Len:%d] "),nLen);

	int i = 0;
	for(i = 0; i < nLen ; i++){ 
		if( i%16 == 0 && 0 < i ){
			if(TRUE == m_DebugLogWrite) LogWrite(_T(": %s %s"),DisplayBuffer,DisplayBuffer2);
			fprintf(stderr,"%s : %s %s \n",(LPCSTR)WtoAConverter(LPCTSTR(GetTime())),DisplayBuffer,DisplayBuffer2);
			Index = 0; 
			ZeroMemory(DisplayBuffer,100);
			ZeroMemory(DisplayBuffer2,100);
		}
		sprintf_s((char*)DisplayBuffer+(Index*3),4 ,"%02x ",temp[i]);
		sprintf_s((char*)DisplayBuffer2+(Index),2,"%c",('\0' == temp[i])?'.':('\n' == temp[i]?'.' : temp[i]));
		++Index;
	} 

	if(i%16 != 0){
		if(48 > _mbslen(DisplayBuffer)){
			for(int j=int(_mbslen(DisplayBuffer));j<48;j++){
				*(DisplayBuffer+j) = '.';
			}
		}
	}

	fprintf(stderr,"%s : %s %s \n", (LPCSTR)WtoAConverter(LPCTSTR(GetTime())),DisplayBuffer,DisplayBuffer2);

	LogPrintLine();


	LeaveCriticalSection(&m_Lock);

} 

void CLogManager::SetLogFileDelete(int nKeepDay)
{
	if (0 >= nKeepDay) nKeepDay = 30;

	COleDateTimeSpan tkeppTimeSpan(nKeepDay, 0, 0, 0);
	COleDateTime tCurrentTime = COleDateTime::GetCurrentTime();
	COleDateTime tkeppTime = tCurrentTime - tkeppTimeSpan;

	CString strLogFilePath, strLogDir;
	strLogFilePath.Format(_T("%s\\Log\\*.log"), LPCTSTR(m_Path));
	strLogDir.Format(_T("%s\\Log"), LPCTSTR(m_Path));
	CFileFind FileFind;
	BOOL bWorking = FileFind.FindFile(strLogFilePath);
	while (bWorking) {
		bWorking = FileFind.FindNextFile();
		if (FileFind.IsDots()) {
			continue;
		}
		if (TRUE == FileFind.IsDirectory()) {
			continue;
		}
		if (TRUE == FileFind.IsArchived()) {
			CString strLogFileName = FileFind.GetFileName();
			long nLogYear = _ttol(strLogFileName.Left(4));
			long nLogMon = _ttol(strLogFileName.Mid(5, 2));
			long nLogDay = _ttol(strLogFileName.Mid(8, 2));

			if ((0 < nLogYear) && (0 < nLogMon) && (0 < nLogDay)) {
				COleDateTime tLogTime(nLogYear, nLogMon, nLogDay, 0, 0, 0);
				if (tkeppTime > tLogTime) {
					CString strFullPath;
					strFullPath.Format(_T("%s\\%s"), strLogDir, FileFind.GetFileName());
					::DeleteFile(strFullPath);
				}
			}
		}
		Sleep(100);
	}
	FileFind.Close();

}