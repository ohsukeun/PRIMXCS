#include "stdafx.h"
#include "DataCollectManager.h"
#include "SystemDataManager.h"
#include "ConfigDataManager.h"
#include "CommandLogDataTransfer.h"
#include <winevt.h>
#pragma comment(lib, "wevtapi.lib")


static PRIMX_EVENT_DATA_TABLE PRIMX_LOG_LEVEL[] = {
	{_T("INFO"),									_T("") },
	{ _T("ERROR"),									_T("E00001") },
	{ _T("WARNING"),								_T("W00001") }
};

static PRIMX_EVENT_DATA_TABLE PRIMX_LOG_KEYWORD_ZONECENTRAL_MASTER[] = {
	{_T("Command:"),								_T("P01001")}
};

static PRIMX_EVENT_DATA_TABLE PRIMX_LOG_KEYWORD_ZONECENTRAL_AFSCMD[] = {
	{_T("Command:"),								_T("P01001")},
	{_T("Creation of access"),						_T("P01003")},
	{_T("Index file"),								_T("P01002")}
};

static PRIMX_EVENT_DATA_TABLE PRIMX_LOG_KEYWORD_ZONECENTRAL_ZCACMD[] = {
	{ _T("Command:"),								_T("P01001") }
};

static PRIMX_EVENT_DATA_TABLE PRIMX_LOG_KEYWORD_ZONECENTRAL_ZBOARD[] = {
	{ _T("Result of check"),						_T("P02003") }
};

static PRIMX_EVENT_DATA_TABLE PRIMX_LOG_KEYWORD_ZONECENTRAL_ZCAPPLY[] = {
	{ _T("Result of check"),						_T("P02003") },
	{ _T("Access to target zone"),					_T("P02004") },
	{ _T("Encrypted zone"),							_T("P02005") }
};


static PRIMX_EVENT_DATA_TABLE PRIMX_LOG_KEYWORD_ORIZON_ORIZON[] = {
	{ _T("File"),									_T("P02001") },
    { _T("Folder"),									_T("P02002") }
};

static PRIMX_EVENT_DATA_TABLE PRIMX_LOG_KEYWORD_ORIZON_MASTER[] = {
	{ _T("Command:"),								_T("P01001") }
};

static PRIMX_EVENT_DATA_TABLE PRIMX_LOG_KEYWORD_ORIZON_OZU[] = {
	{ _T("File"),									_T("P02001") },
	{ _T("Folder"),									_T("P02002") },
	{ _T("The cryptographic"),						_T("P02006") },
	{ _T("Mandatories accesses of shared folder"),	_T("P02007") }
};

static PRIMX_EVENT_DATA_TABLE PRIMX_LOG_KEYWORD_ORIZON_OZCMD[] = {
	{ _T("Command:"),								_T("P01001") }
};

static PRIMX_EVENT_DATA_TABLE PRIMX_LOG_KEYWORD_ORIZON_AFSCMD[] = {
	{ _T("Command:"),								_T("P01001") },
	{ _T("Creation of access"),						_T("P01003") },
	{ _T("Index file"),								_T("P01002") }
};

static PRIMX_EVENT_DATA_TABLE2 PRIMX_EVENT_ZONECENTRAL[] = {
	{ 257  , _T("P02008") },
	{ 258  , _T("P02008") },
	{ 259  , _T("P02004") },
	{ 260  , _T("P02004") },
	{ 261  , _T("P02009") },
	{ 262  , _T("P02006") },
	{ 263  , _T("P02006") },
	{ 264  , _T("P02009") },
	{ 265  , _T("P02002") },
	{ 272  , _T("P02002") },
	{ 273  , _T("P02002") },
	{ 274  , _T("P02002") },
	{ 275  , _T("P02002") },
	{ 277  , _T("P02004") },
	{ 278  , _T("P02004") },
	{ 280  , _T("P02012") },
	{ 289  , _T("P02010") },
	{ 290  , _T("P02010") },
	{ 291  , _T("P02010") },
	{ 292  , _T("P02010") },
	{ 293  , _T("P02010") },
	{ 294  , _T("P02010") },
	{ 295  , _T("P02009") },
	{ 296  , _T("P02009") },
	{ 297  , _T("P02004") },
	{ 304  , _T("P02011") },
	{ 305  , _T("P02011") },
	{ 311  , _T("P02004") },
	{ 312  , _T("P02004") },
	{ 313  , _T("P02004") },
	{ 320  , _T("P02004") },
	{ 321  , _T("P02004") },
	{ 322  , _T("P02004") },
	{ 323  , _T("P02004") },
	{ 324  , _T("P02004") },
	{ 325  , _T("P02004") },
	{ 326  , _T("P02011") },
	{ 327  , _T("P02011") },
	{ 328  , _T("P02011") },
	{ 339  , _T("P02008") },
	{ 340  , _T("P02008") },
	{ 341  , _T("P02008") },
	{ 342  , _T("P02011") },
	{ 343  , _T("P02011") },
	{ 355  , _T("E00001") },
	{ 356  , _T("E00001") },
	{ 357  , _T("P02010") },
	{ 358  , _T("P02008") },
	{ 359  , _T("P02004") },
	{ 361  , _T("P02011") },
	{ 368  , _T("P02011") },
	{ 369  , _T("P02003") },
	{ 370  , _T("P02003") },
	{ 371  , _T("P02003") },
	{ 372  , _T("P02010") },
	{ 375  , _T("P02010") },
	{ 376  , _T("P02010") },
	{ 384  , _T("P02012") },
	{ 385  , _T("P02010") },
	{ 386  , _T("P02010") },
	{ 387  , _T("P02011") },
	{ 388  , _T("P02004") },
	{ 389  , _T("P02004") },
	{ 390  , _T("P02004") },
	{ 391  , _T("P02004") },
	{ 392  , _T("P02004") },
	{ 401  , _T("P02004") },
	{ 402  , _T("P02009") },
	{ 514  , _T("E00001") },
	{ 515  , _T("E00001") },
	{ 516  , _T("P01001") },
	{ 518  , _T("P01001") },
	{ 519  , _T("E00001") },
	{ 521  , _T("P02013") },
	{ 522  , _T("P02013") },
	{ 523  , _T("P02013") },
	{ 524  , _T("P02013") },
	{ 525  , _T("P02013") },
	{ 526  , _T("P02014") },
	{ 527  , _T("P02014") },
	{ 528  , _T("E00001") },
	{ 529  , _T("P02013") },
	{ 530  , _T("P02013") },
	{ 531  , _T("P02013") },
	{ 532  , _T("P02013") },
	{ 533  , _T("P02013") },
	{ 534  , _T("P02013") },
	{ 535  , _T("P02010") },
	{ 536  , _T("P02010") },
	{ 537  , _T("P02004") },
	{ 544  , _T("P02021") },
	{ 545  , _T("P02021") },
	{ 546  , _T("P02021") },
	{ 547  , _T("P02021") },
	{ 548  , _T("P02021") },
	{ 549  , _T("P02021") },
	{ 550  , _T("P02021") },
	{ 551  , _T("P02021") },
	{ 552  , _T("P02021") },
	{ 553  , _T("P02021") },
	{ 560  , _T("P02021") },
	{ 561  , _T("P02021") },
	{ 562  , _T("P02021") },
	{ 563  , _T("P02021") },
	{ 564  , _T("P02021") },
	{ 565  , _T("P02021") },
	{ 566  , _T("P02021") },
	{ 567  , _T("P02021") },
	{ 568  , _T("P02021") },
	{ 569  , _T("P02021") },
	{ 576  , _T("P02021") },
	{ 577  , _T("P02021") },
	{ 578  , _T("P02021") },
	{ 579  , _T("P02021") },
	{ 580  , _T("P02021") },
	{ 16387  , _T("E00001") },
	{ 16896  , _T("P02015") },
	{ 16897  , _T("P02015") },
	{ 16898  , _T("P02015") },
	{ 16899  , _T("P02015") },
	{ 16900  , _T("P02015") },
	{ 16901  , _T("E00001") },
	{ 16902  , _T("P02011") },
	{ 16903  , _T("P02011") },
	{ 16904  , _T("P02016") },
	{ 16905  , _T("P02016") },
	{ 16906  , _T("P02017") },
	{ 16907  , _T("P02017") },
	{ 16908  , _T("P02017") },
	{ 16909  , _T("P02011") },
	{ 16910  , _T("P02011") },
	{ 16911  , _T("P02011") },
	{ 16912  , _T("P02011") },
	{ 16913  , _T("P02011") },
	{ 16914  , _T("P02011") },
	{ 16915  , _T("P02011") },
	{ 16916  , _T("P02011") },
	{ 16917  , _T("P02011") },
	{ 16918  , _T("P02011") },
	{ 16919  , _T("P02011") },
	{ 16920  , _T("P02011") },
	{ 16976  , _T("P02004") },
	{ 16977  , _T("P02004") },
	{ 16978  , _T("P02012") },
	{ 16979  , _T("P02012") },
	{ 16980  , _T("P02012") },
	{ 16981  , _T("P02012") },
	{ 16982  , _T("P02012") },
	{ 16983  , _T("P02011") },
	{ 16984  , _T("P02011") },
	{ 16985  , _T("P02011") },
	{ 17152  , _T("P02019") },
	{ 17153  , _T("P02019") },
	{ 17154  , _T("P02019") },
	{ 17155  , _T("P02019") },
	{ 17156  , _T("P02019") },
	{ 17157  , _T("P02019") },
	{ 17158  , _T("P02019") },
	{ 17159  , _T("E00001") },
	{ 17232  , _T("P02020") },
	{ 17233  , _T("P02020") },
	{ 17234  , _T("P02020") },
	{ 17235  , _T("P02020") },
	{ 17236  , _T("P02020") },
	{ 17237  , _T("P02020") },
	{ 17238  , _T("P02020") },
	{ 17239  , _T("P02020") },
	{ 17240  , _T("P02020") },
	{ 17241  , _T("P02020") },
	{ 17242  , _T("P02020") },
	{ 17243  , _T("P02020") },
	{ 17244  , _T("P02020") },
	{ 17245  , _T("P02020") },
	{ 17246  , _T("P02020") },
	{ 17247  , _T("P02020") },
	{ 17248  , _T("P02020") },
	{ 17249  , _T("P02020") },
	{ 17250  , _T("P02020") },
	{ 17251  , _T("P02020") },
	{ 17252  , _T("P02020") },
	{ 17253  , _T("P02020") },
	{ 17254  , _T("P02020") },
	{ 17255  , _T("P02020") },
	{ 17256  , _T("P02020") },
	{ 17257  , _T("P02020") },
	{ 17258  , _T("P02020") },
	{ 17408  , _T("P02011") },
	{ 17409  , _T("P02012") },
	{ 17410  , _T("P02012") },
	{ 17411  , _T("P02012") },
	{ 17412  , _T("P02012") },
	{ 17413  , _T("P02011") },
	{ 17414  , _T("P02012") },
	{ 17415  , _T("P02012") },
	{ 17488  , _T("E00001") },
	{ 17489  , _T("E00001") },
	{ 17490  , _T("E00001") },
	{ 17491  , _T("P01001") },
	{ 17492  , _T("P01001") },
	{ 17493  , _T("E00001") },
	{ 17494  , _T("E00001") },
	{ 17495  , _T("E00001") }
};

static PRIMX_EVENT_DATA_TABLE2 PRIMX_EVENT_ORIZON[] = {
	{ 17192 , _T("P02008") },
	{ 17193 , _T("P02008") },
	{ 17200 , _T("P02017") },
	{ 17201 , _T("P02017") },
	{ 17202 , _T("P02017") },
	{ 17203 , _T("P02017") },
	{ 17204 , _T("P02017") },
	{ 17205 , _T("P02017") },
	{ 17206 , _T("P02017") },
	{ 16914 , _T("P02011") },
	{ 16915 , _T("P02011") },
	{ 17159 , _T("E00001") },
	{ 17241 , _T("P02020") },
	{ 17244 , _T("P02020") },
	{ 17247 , _T("P02020") },
	{ 16922 , _T("P02018") },
	{ 17240 , _T("P02020") },
	{ 17243 , _T("P02020") }
};


CDataCollectManager::CDataCollectManager()
{
	m_nCollectTime[0] = 0;
	m_nCollectTime[1] = 0;
	m_nCollectTime[2] = 0;
	m_bRun = FALSE;
}

CDataCollectManager::~CDataCollectManager()
{
}

BOOL CDataCollectManager::SetStart()
{
	m_strZoneCentrolLogPath = GetPRIMXProductLogPath(PRIMX_PRODUCT_TYPE_ZONECENTRAL);
	m_strOrizonLogPath = GetPRIMXProductLogPath(PRIMX_PRODUCT_TYPE_ORIZON);
	m_strCryhodLogPath = GetPRIMXProductLogPath(PRIMX_PRODUCT_TYPE_CRYHOD);

	m_bRun = TRUE;
	return Start();
}

void CDataCollectManager::SetStop()
{
	m_bRun = FALSE;
	WaitForTermination(500);
}

void CDataCollectManager::Run()
{	
	while (m_bRun) {

		__time64_t tCurrentTime = _time64(NULL);
		if (m_nCollectTime[0] < tCurrentTime) {
			GetProductData();
			m_nCollectTime[0] = _time64(NULL) + PRIMX_PRODUCT_COLLECT_CYCLE;
		}
		tCurrentTime = _time64(NULL);
		if (m_nCollectTime[1] < tCurrentTime) {
			GetEventData();
			GetLogFileData();
			m_nCollectTime[1] = _time64(NULL) + PRIMX_LOG_COLLECT_CYCLE;
		}
		tCurrentTime = _time64(NULL);
		if (m_nCollectTime[2] < tCurrentTime) {
			SetLogDataTransfer();
			CConfigDataManager ConfigDataManager;
			m_nCollectTime[2] = _time64(NULL) + ConfigDataManager.GetLogCycle();
		}

		Sleep(100);
	}

}

void CDataCollectManager::GetProductData()
{
	CSystemDataManager SystemDataManager;

	PRIMX_PRODUCT_DATA Zonecentrol;
	PRIMX_PRODUCT_DATA Orizone;
	PRIMX_PRODUCT_DATA Cryhod;

	if (TRUE == SystemDataManager.GetPRIMXProductInfo(Zonecentrol, Orizone, Cryhod)) {
		CFileDBManager FileDBManager;
		CConfigDataManager ConfigDataManager;
		FileDBManager.SetPRIMXProductDataDBSave(ConfigDataManager.GetAgentID(),Zonecentrol, Orizone, Cryhod);
	}
}

CString CDataCollectManager::GetPRIMXProductLogPath(UINT nProductType)
{
	CString strLogPath;
	
	UINT PRIMX_LOG_PATH[2] = { 0 ,0 };

	if (PRIMX_PRODUCT_TYPE_ZONECENTRAL == nProductType) {
		PRIMX_LOG_PATH[0] = PRIMX_ZC_INSTALL_PATH_FORMAT;
		PRIMX_LOG_PATH[1] = PRIMX_ZC_INSTALL_PROGRAMDATA_PATH_FORMAT;
	}
	else if (PRIMX_PRODUCT_TYPE_ORIZON == nProductType) {
		PRIMX_LOG_PATH[0] = PRIMX_OR_INSTALL_PATH_FORMAT;
		PRIMX_LOG_PATH[1] = PRIMX_OR_INSTALL_PROGRAMDATA_PATH_FORMAT;

	}
	else if (PRIMX_PRODUCT_TYPE_CRYHOD == nProductType) {
		PRIMX_LOG_PATH[0] = PRIMX_CH_INSTALL_PATH_FORMAT;
		PRIMX_LOG_PATH[1] = PRIMX_CH_INSTALL_PROGRAMDATA_PATH_FORMAT;
	}
	else {
		PRIMX_LOG_PATH[0] = PRIMX_ZC_INSTALL_PATH_FORMAT;
		PRIMX_LOG_PATH[1] = PRIMX_ZC_INSTALL_PROGRAMDATA_PATH_FORMAT;
	}

	TCHAR szPath[MAX_PATH] = { 0 };
	if (TRUE == SHGetSpecialFolderPath(NULL, szPath, CSIDL_COMMON_APPDATA, 0)) {
		strLogPath.Format(PRIMX_LOG_PATH[0], szPath);
	}
	else {
		if (0 < ::ExpandEnvironmentStrings(_T("%ProgramData%"), szPath, MAX_PATH)) {
			strLogPath.Format(PRIMX_LOG_PATH[0], szPath);
		}
		else {
			if (0 != GetWindowsDirectory(szPath, MAX_PATH)) {
				TCHAR pDrive[MAX_PATH] = { 0 };
				TCHAR pDir[MAX_PATH] = { 0 };
				TCHAR pFileName[MAX_PATH] = { 0 };
				TCHAR pFileExt[MAX_PATH] = { 0 };
				_tsplitpath_s(szPath, pDrive, MAX_PATH, pDir, MAX_PATH, pFileName, MAX_PATH, pFileExt, MAX_PATH);
				strLogPath.Format(PRIMX_LOG_PATH[1], pDrive);
			}
			else {
				strLogPath.Format(PRIMX_LOG_PATH[1], _T("c:"));
			}
		}
	}
	return strLogPath;
}

void CDataCollectManager::GetEventData()
{
	EVT_HANDLE hEvtHandle = EvtOpenLog(NULL, _T("ZoneCentral"), EvtOpenChannelPath);
	if (NULL != hEvtHandle) {
		EvtClose(hEvtHandle);
		hEvtHandle = NULL;
		GetEventData(_T("ZoneCentral"));
	}

	hEvtHandle = EvtOpenLog(NULL, _T("ORIZON"), EvtOpenChannelPath);
	if (NULL != hEvtHandle) {
		EvtClose(hEvtHandle);
		hEvtHandle = NULL;
		GetEventData(_T("ORIZON"));
	}

	hEvtHandle = EvtOpenLog(NULL, _T("CRYHOD"), EvtOpenChannelPath);
	if (NULL != hEvtHandle) {
		EvtClose(hEvtHandle);
		hEvtHandle = NULL;
		GetEventData(_T("CRYHOD"));
	}
}

void CDataCollectManager::GetEventData(CString strSourceName)
{
	EVENTLOGRECORD *pevlr = NULL;
	UINT BufferSize = EVENT_LOG_BUFFER_SIZE;
	DWORD dwRead, dwNeeded;
	LPTSTR lpmessagetext = _T("");
	
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s Event Log 수집 시작"), strSourceName);
	
	HANDLE hEventLog = OpenEventLog(NULL, strSourceName); 
	
	if (NULL == hEventLog) {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s Event Log 수집 시작 > 이벤트 로그 오픈 실패 > %s"), strSourceName, GetLastErrorString());
		return;
	}

	BYTE* bBuffer = new BYTE[BufferSize];
	ZeroMemory(bBuffer, BufferSize);
	
	pevlr = (EVENTLOGRECORD*)bBuffer;

	CConfigDataManager ConfigDataManager;
	DWORD dwRecordNumber = 0;
	DWORD dwLastRecordNumber = ConfigDataManager.GetEventRecordNumber(strSourceName);
	BOOL bLastRecord = FALSE;

//	GetOldestEventLogRecord(hEventLog, &dwThisRecord);

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s Event Log 수집 시작 > 마지막 수집 로그 정보 > Record Number : %lu"), strSourceName, dwLastRecordNumber);

	UINT nEventLogCount = 0;
	CFileDBManager FileDBManager;
	if (TRUE == FileDBManager.OpenLogDB()) {
		while (TRUE) {
			dwRead = 0;
			dwNeeded = 0;
			BOOL Re = ReadEventLog(hEventLog, EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ, 0, pevlr, BufferSize, &dwRead, &dwNeeded);
			if (FALSE == Re) {
				DWORD Error = GetLastError();
				if (122 == Error) {
					delete[] bBuffer;
					bBuffer = NULL;
					BufferSize = dwNeeded;
					bBuffer = new BYTE[BufferSize];
					ZeroMemory(bBuffer, BufferSize);
					pevlr = (EVENTLOGRECORD*)bBuffer;
					dwRead = 0;
					dwNeeded = 0;
					if (FALSE == ReadEventLog(hEventLog, EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ, 0, pevlr, BufferSize, &dwRead, &dwNeeded)) {
						delete[] bBuffer;
						bBuffer = NULL;
						break;
					}
				}
				else {
					break;
				}
			}

			while (dwRead > 0) {
				if (dwLastRecordNumber < pevlr->RecordNumber) {
					if (0 == dwRecordNumber) {
						dwRecordNumber = pevlr->RecordNumber;
					}
					lpmessagetext = GetEventString(strSourceName,pevlr, ((LPTSTR)((LPBYTE)pevlr + sizeof(EVENTLOGRECORD))));
					if (TRUE == EventLogDataCheck(FileDBManager, pevlr->EventType, pevlr->EventID, pevlr->TimeGenerated, (LPTSTR)((LPBYTE)pevlr + sizeof(EVENTLOGRECORD)), lpmessagetext)) {
						nEventLogCount++;
					}
					dwRead -= pevlr->Length;
					pevlr = (EVENTLOGRECORD *)((LPBYTE)pevlr + pevlr->Length);
				}
				else {
					bLastRecord = TRUE;
					break;
				}
			}
			if (TRUE == bLastRecord) break;
			pevlr = (EVENTLOGRECORD*)bBuffer;
		}

		FileDBManager.CloseDB();
	}
	else {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s Event Log 수집 시작 > SQLite DB 오픈 실패"), strSourceName);
	}
	
	delete[] bBuffer;
	bBuffer = NULL;

	CloseEventLog(hEventLog);

	if (0 < dwRecordNumber) {
		ConfigDataManager.SetEventRecordNumber(strSourceName, dwRecordNumber);
	}
	
	if (0 < nEventLogCount) {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("%s Event Log 수집 > %lu개 수집 > SQLite 저장"), strSourceName, nEventLogCount);
	}

}


void CDataCollectManager::GetLogFileData()
{
	if (0 < m_strZoneCentrolLogPath.GetLength()) GetLogFileData(m_strZoneCentrolLogPath, PRIMX_PRODUCT_TYPE_ZONECENTRAL);
	if (0 < m_strOrizonLogPath.GetLength()) GetLogFileData(m_strOrizonLogPath, PRIMX_PRODUCT_TYPE_ORIZON);
	if (0 < m_strCryhodLogPath.GetLength()) GetLogFileData(m_strCryhodLogPath, PRIMX_PRODUCT_TYPE_CRYHOD);
}

void CDataCollectManager::GetLogFileData(CString strCollectDirectory, UINT nProductType)
{
	UINT nLogFileCount = 0;
	UINT nLogFileFailCount = 0;

	CString strFailDirectory;
	strFailDirectory.Format(_T("%s\\FailTransfer"), strCollectDirectory);
	SetCreateDirectoryEx(strFailDirectory);

	CString strProductName;
	if (PRIMX_PRODUCT_TYPE_ZONECENTRAL == nProductType) strProductName = _T("ZONECENTRAL");
	if (PRIMX_PRODUCT_TYPE_ORIZON == nProductType) strProductName = _T("ORIZON");
	if (PRIMX_PRODUCT_TYPE_CRYHOD == nProductType) strProductName = _T("CRYHOD");
	
	CFileDBManager FileDBManager;
	if (TRUE == FileDBManager.OpenLogDB()) {
		CFileFind FileFinder;
		CString strTemp;
		strTemp = strCollectDirectory + _T("\\*.log");
		BOOL bWorking = FileFinder.FindFile(strTemp);

		while (bWorking) {
			bWorking = FileFinder.FindNextFile();
			if (FileFinder.IsDots()) {
				continue;
			}

			if (TRUE == FileFinder.IsArchived()) {
				BOOL bCollectFile = FALSE;
				FILETIME TimeStamp;
				if (TRUE == FileFinder.GetCreationTime(&TimeStamp)) {
					SYSTEMTIME create_system_time, create_local_time;
					FileTimeToSystemTime(&TimeStamp, &create_system_time);
					SystemTimeToTzSpecificLocalTime(NULL, &create_system_time, &create_local_time);
					COleDateTime tTimeStamp(create_system_time);
					COleDateTime tCurrentTime = COleDateTime::GetCurrentTime();

					COleDateTimeSpan DateTimeSpan = tCurrentTime - tTimeStamp;
					if (30 <= DateTimeSpan.GetTotalSeconds()) {
						bCollectFile = TRUE;
					}
				}

				if (TRUE == bCollectFile) {
					
					CString strFileName = FileFinder.GetFileName();
					strFileName.Trim();
					CString strLogDate;
					if (TRUE == SetLogDateParser(strFileName, strLogDate)) {
						CString strLogFileFullData;
						CString strLogFilePath = FileFinder.GetFilePath();
						if (TRUE == LoadLogFile(strLogFilePath, strLogFileFullData)) {
							if (TRUE == SetLogDataDBSave(FileDBManager, nProductType, strLogFilePath, strLogDate, strLogFileFullData)) {
								SetDeleteFile(strLogFilePath);
								theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s 로그 파일 수집 > SQLite DB 저장 성공 > %s ", strProductName, strFileName);
								nLogFileCount++;
							}
							else {
								theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s 로그 파일 수집 > %s > SQLite DB 저장 실패 ", strProductName, strFileName);
								nLogFileFailCount++;
							}
						}
						else {
							// 파일 오픈 및 로딩 실패
							theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s 로그 파일 수집 > %s > 파일 오픈 및 로딩 실패", strProductName, strFileName);
							CString strDstFile;
							strDstFile.Format(_T("%s\\%s"), strFailDirectory, strFileName);
							if (TRUE == SetFileCopy(strLogFilePath, strDstFile)) {
								SetDeleteFile(strLogFilePath);
								theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s 로그 파일 수집 > %s > %s로 이동", strProductName, strFileName, strDstFile);
							}
							nLogFileFailCount++;
						}
					}
					else {
						// 로그파일 이름이 비정상 > 이름변경
						theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s 로그 파일 수집 > %s > 로그파일명 형식 오류", strProductName, strFileName);
						CString strDstFile;
						strDstFile.Format(_T("%s\\%s"), strFailDirectory, strFileName);
						CString strLogFilePath = FileFinder.GetFilePath();
						if (TRUE == SetFileCopy(strLogFilePath, strDstFile)) {
							SetDeleteFile(strLogFilePath);
							theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s 로그 파일 수집 > %s > %s로 이동", strProductName, strFileName, strDstFile);
						}
						nLogFileFailCount++;
					}
				}
			}
			Sleep(0);
		}
		FileFinder.Close();
		FileDBManager.CloseDB();
	}
	else {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s 로그 파일 수집 > SQLite DB 오픈 실패", strProductName);
	}

	if (0 < nLogFileCount || 0 < nLogFileFailCount) {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"%s 로그 파일 수집 > %s > 수집[%lu] > 오류[%lu]", strProductName,strCollectDirectory, nLogFileCount, nLogFileFailCount);
	}

}

BOOL CDataCollectManager::SetLogDateParser(CString strFileName, CString& strLogDate)
{
	BOOL bRe = FALSE;

	CStringList TokenList;
	CString strToken;
	int curPos = 0;
	strToken = strFileName.Tokenize(_T("_"), curPos);
	while (0 < strToken.GetLength()){
		TokenList.AddHead(strToken);
		strToken = strFileName.Tokenize(_T("_"), curPos);
	};
	
	if (4 <= TokenList.GetCount()) {
		strLogDate = TokenList.GetAt(TokenList.FindIndex(2));
		if (8 == strLogDate.GetLength()) {
			strLogDate.Insert(6, _T("-"));
			strLogDate.Insert(4, _T("-"));
			bRe = TRUE;
		}
	}
	return bRe;
}

BOOL CDataCollectManager::LoadLogFile(CString strFilePath, CString& strLogData)
{
	BOOL bRe = FALSE;

	FILE* fp = NULL;
	errno_t err = _tfopen_s(&fp, LPCTSTR(strFilePath), _T("rt,ccs=UTF-8"));
	if (ERROR_SUCCESS == err){
		fseek(fp, 0, SEEK_END);
		UINT nFileByteLen = UINT(ftell(fp));
		fseek(fp, 0, SEEK_SET);

		if (0 < nFileByteLen) {
			char* pFileData = new char[nFileByteLen + 2];
			ZeroMemory((void*)pFileData, nFileByteLen + 2);
			fread(pFileData, nFileByteLen, 1, fp);

			WORD BOM = 0;
			memcpy((void*)&BOM, pFileData,2);
			
			if (0xFEFF == BOM) {
				strLogData = (WCHAR*)(pFileData + 2);
			}
			else{
				strLogData = (WCHAR*)(pFileData);
			}			
			_SafeDeleteArray(pFileData);	
			bRe = TRUE;
		}
		else {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("로그파일 사이즈 0Byte > [%s]"), strFilePath);
		}
		fclose(fp);
	}
	else {
		TCHAR* pErrorString = _tcserror(err);
		if (NULL != pErrorString) {
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("로그 파일 open error > %s > [%s]"), pErrorString, strFilePath);
		}
	}
	return bRe;
}

BOOL CDataCollectManager::SetLogDataDBSave(CFileDBManager& FileDBManager, UINT nProductType,CString strFilePath, CString& strLogDate, CString& strLogFileFullData)
{
	BOOL bRe = FALSE;
	
	CString strTokenLine;
	int curPos = 0;

	strLogFileFullData.Replace(_T("\r"),_T(""));
	strTokenLine = strLogFileFullData.Tokenize(_T("\n"), curPos);
	while (0 < strTokenLine.GetLength()) {
		
		PRIMX_LOG_DATA DBSaveData;
		DBSaveData.nProductType = nProductType;
		DBSaveData.strLogDate = strLogDate;
		DBSaveData.strLogFileName = strFilePath;

		BOOL bDBSave = FALSE;
		int nIndex = 0;
		int curPos2 = 0;
		CString strUintData = strTokenLine.Tokenize(_T(" "), curPos2);
		while (0 < strUintData.GetLength()) {

			if (0 == nIndex) {
				strUintData.Replace(_T("["), _T(""));
				strUintData.Replace(_T("]"), _T(""));
				int nFind = strUintData.Find(_T(":"));
				if (0 < nFind) {
					DBSaveData.strLogType = strUintData.Left(nFind);
				}
				else{
					DBSaveData.strLogType = strUintData;
				}				
			}
			if (1 == nIndex) {
				DBSaveData.strLogTime = strUintData;
			}
			if (2 == nIndex) {
				if (TRUE == SetLevelDataCheck(strUintData, DBSaveData)) {
					DBSaveData.strLogData = strTokenLine.Right(strTokenLine.GetLength()- curPos2);
					DBSaveData.strLogData.Trim();
					if (0 == DBSaveData.strLogCode.GetLength()) {
						bDBSave = SetLogDataCheck(DBSaveData);
					}
					else {
						bDBSave = TRUE;
					}
				}
				break;
			}
			nIndex++;
			strUintData = strTokenLine.Tokenize(_T(" "), curPos2);
		};		

		if (TRUE == bDBSave) {
			if (TRUE == FileDBManager.SetLogDataSave(DBSaveData,1)) {
				
			}
		}

		DBSaveData.Clear();

		strTokenLine = strLogFileFullData.Tokenize(_T("\n"), curPos);
	};
	
	return TRUE;
}

BOOL CDataCollectManager::SetLevelDataCheck(CString strData, PRIMX_LOG_DATA& DBSaveData)
{
	BOOL bRe = FALSE;
	strData.Trim();
	for (int i = 0; i < _CountOfArray(PRIMX_LOG_LEVEL); i++) {
		if (0 == strData.CompareNoCase(PRIMX_LOG_LEVEL[i].strData)) {
			DBSaveData.strLogCode = PRIMX_LOG_LEVEL[i].strEventCode;
			DBSaveData.strLogLevel = PRIMX_LOG_LEVEL[i].strData;
			bRe = TRUE;
			break;
		}
	}
	return bRe;
}

BOOL CDataCollectManager::SetLogDataCheck(PRIMX_EVENT_DATA_TABLE EventTable[], UINT nEventTableSize, PRIMX_LOG_DATA& DBSaveData)
{
	BOOL bRe = FALSE;

	CString strMakeLower = DBSaveData.strLogData;
	strMakeLower.Trim().MakeLower();
	
	for (UINT i = 0; i < nEventTableSize; i++) {
		CString strKeyword = EventTable[i].strData;
		strKeyword.Trim().MakeLower();
		int nIndex = strMakeLower.Find(strKeyword);
		if (0 <= nIndex) {
			DBSaveData.strLogCode = EventTable[i].strEventCode;
			DBSaveData.strLogData = DBSaveData.strLogData.Mid(nIndex, DBSaveData.strLogData.GetLength() - nIndex);
			bRe = TRUE;
			break;
		}
	}

	return bRe;
}

BOOL CDataCollectManager::SetLogDataCheck(PRIMX_LOG_DATA& DBSaveData)
{
	BOOL bRe = FALSE;

	if (PRIMX_PRODUCT_TYPE_ZONECENTRAL == DBSaveData.nProductType) {
		if (0 == DBSaveData.strLogType.CompareNoCase(_T("MASTER.EXE"))) {
			bRe = SetLogDataCheck(PRIMX_LOG_KEYWORD_ZONECENTRAL_MASTER, _CountOfArray(PRIMX_LOG_KEYWORD_ZONECENTRAL_MASTER), DBSaveData);
		}
		else if (0 == DBSaveData.strLogType.CompareNoCase(_T("AFSCMD.EXE"))) {
			bRe = SetLogDataCheck(PRIMX_LOG_KEYWORD_ZONECENTRAL_AFSCMD, _CountOfArray(PRIMX_LOG_KEYWORD_ZONECENTRAL_AFSCMD), DBSaveData);
		}
		else if (0 == DBSaveData.strLogType.CompareNoCase(_T("ZCACMD.EXE"))) {
			bRe = SetLogDataCheck(PRIMX_LOG_KEYWORD_ZONECENTRAL_ZCACMD, _CountOfArray(PRIMX_LOG_KEYWORD_ZONECENTRAL_ZCACMD), DBSaveData);
		}
		else if (0 == DBSaveData.strLogType.CompareNoCase(_T("ZBOARD.EXE"))) {
			bRe = SetLogDataCheck(PRIMX_LOG_KEYWORD_ZONECENTRAL_ZBOARD, _CountOfArray(PRIMX_LOG_KEYWORD_ZONECENTRAL_ZBOARD), DBSaveData);
		}
		else if (0 == DBSaveData.strLogType.CompareNoCase(_T("ZCAPPLY.EXE"))) {
			bRe = SetLogDataCheck(PRIMX_LOG_KEYWORD_ZONECENTRAL_ZCAPPLY, _CountOfArray(PRIMX_LOG_KEYWORD_ZONECENTRAL_ZCAPPLY), DBSaveData);
		}
	}
	else if (PRIMX_PRODUCT_TYPE_ORIZON == DBSaveData.nProductType) {
		if (0 == DBSaveData.strLogType.CompareNoCase(_T("ORIZON.EXE"))) {
			bRe = SetLogDataCheck(PRIMX_LOG_KEYWORD_ORIZON_ORIZON, _CountOfArray(PRIMX_LOG_KEYWORD_ORIZON_ORIZON), DBSaveData);
		}
		else if (0 == DBSaveData.strLogType.CompareNoCase(_T("MASTER.EXE"))) {
			bRe = SetLogDataCheck(PRIMX_LOG_KEYWORD_ORIZON_MASTER, _CountOfArray(PRIMX_LOG_KEYWORD_ORIZON_MASTER), DBSaveData);
		}
		else if (0 == DBSaveData.strLogType.CompareNoCase(_T("OZU.EXE"))) {
			bRe = SetLogDataCheck(PRIMX_LOG_KEYWORD_ORIZON_OZU, _CountOfArray(PRIMX_LOG_KEYWORD_ORIZON_OZU), DBSaveData);
		}
		else if (0 == DBSaveData.strLogType.CompareNoCase(_T("OZCMD.EXE"))) {
			bRe = SetLogDataCheck(PRIMX_LOG_KEYWORD_ORIZON_OZCMD, _CountOfArray(PRIMX_LOG_KEYWORD_ORIZON_OZCMD), DBSaveData);
		}
		else if (0 == DBSaveData.strLogType.CompareNoCase(_T("AFSCMD.EXE"))) {
			bRe = SetLogDataCheck(PRIMX_LOG_KEYWORD_ORIZON_AFSCMD, _CountOfArray(PRIMX_LOG_KEYWORD_ORIZON_AFSCMD), DBSaveData);
		}
	}

	return bRe;
}

void CDataCollectManager::GetLogDataForDB(CList<PRIMX_LOG_DATA*>& LogDataList)
{
	CFileDBManager FileDBManager;
	FileDBManager.GetLogDataTransfer(LogDataList);

}

void CDataCollectManager::SetLogDataForDB(CList<PRIMX_LOG_DATA*>& LogDataList)
{
	CFileDBManager FileDBManager;
	FileDBManager.SetLogDataTransfer(LogDataList);
}

void CDataCollectManager::SetLogDataTransfer()
{
	CCommandLogDataTransfer CommandLogDataTransfer;

	CList<PRIMX_LOG_DATA*>	LogDataList;

	GetLogDataForDB(LogDataList);

	if (0 < LogDataList.GetCount()) {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"SQLite DB 저장된 로그 파일 수집 > %lu개 로그", LogDataList.GetCount());

		if (TRUE == CommandLogDataTransfer.SetConnect()) {
			if (TRUE == CommandLogDataTransfer.SetLogDataTransfer(&LogDataList)) {
				SetLogDataForDB(LogDataList);
			}
			CommandLogDataTransfer.Close();
		}

		POSITION Pos = LogDataList.GetHeadPosition();
		while (NULL != Pos) {
			PRIMX_LOG_DATA* pData = LogDataList.GetNext(Pos);
			_SafeDelete(pData);
		}
		LogDataList.RemoveAll();
	}
}

LPTSTR CDataCollectManager::GetEventString(CString strSourceName,EVENTLOGRECORD *pRecord, LPTSTR source)
{
	BOOL f;
	TCHAR szEvent[1000] = { 0 };
	TCHAR szBuffer[256] = { 0 };
	TCHAR **first_sz;

	HMODULE hEvt;
	LPTSTR lpP[] = { _T(""), _T(""), _T(""), _T(""), _T(""), _T(""),_T(""), _T(""), _T(""),_T(""), _T(""), _T(""),_T(""), _T(""), _T(""),_T(""), _T(""), _T(""),_T(""),_T(""), _T("") };
	LPTSTR    lpBuf = _T("");
	LPTSTR  lpstrlpBuf = _T("");
	TCHAR* pStr;

	if (pRecord->NumStrings) {
		pStr = (TCHAR*)((LPBYTE)pRecord + pRecord->StringOffset);
	}
	else {
		pStr = _T("");
	}
	if (pStr) {
		DWORD i;
		for (i = 0; i < pRecord->NumStrings; i++) {
			lpP[i] = (LPTSTR)pStr;
			pStr = _tcschr((TCHAR*)pStr, _T('\0')) + 1;
		}
	}
	//Get the file name(s) from the registry
	f = ReadEventSourceInfo(strSourceName,source, szEvent);
	if (_tcschr(szEvent, _T(';'))) {
		int i = 0, j, k, num_files = 0, last = 0;
		TCHAR *aux, *cad;
		aux = szEvent;
		while (aux = _tcschr(aux, _T(';'))) {
			num_files++;
			aux++;
		}
		aux = szEvent;

		for (j = 0; j <= num_files; j++) {
			int counter = 0;
			while ((szEvent[i] != _T(';')) && i) {
				i++;
				counter++;
			}
			first_sz = (TCHAR **)malloc(num_files * (sizeof(szEvent)));
			cad = (TCHAR *)malloc(counter + 1);
			for (k = 0; k < counter; k++) {
				cad[k] = szEvent[last + k];
			}
			cad[counter] = _T('\0');
			first_sz[j] = cad;
			i++;
			last = i;
			//Convert the %SystemRoot% stuff
			ExpandEnvironmentStrings(first_sz[j], szBuffer, 257);

			//We actually have to load an .exe or what not to read the messages
			hEvt = LoadLibraryEx(szBuffer, NULL, DONT_RESOLVE_DLL_REFERENCES);

			/* Load the event message file DLL */
			if (hEvt) {
				/* Get the event message with the paramater strings inserted */
				lpBuf = GetEventMessage(hEvt, pRecord->EventID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpP);
				FreeLibrary(hEvt);
			}
		}
		return (lpBuf);
	}
	else {
		ExpandEnvironmentStrings(szEvent, szBuffer, 257);
		hEvt = LoadLibraryEx(szBuffer, NULL, DONT_RESOLVE_DLL_REFERENCES);
		/* Load the event message file DLL */
		if (hEvt) {
			/* Get the event message with the paramater strings inserted */
			lpBuf = GetEventMessage(hEvt, pRecord->EventID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpP);
			FreeLibrary(hEvt);
		}
	}
	return (lpBuf);
}

BOOL CDataCollectManager::ReadEventSourceInfo(CString strSourceName, LPCTSTR lpszESName, LPTSTR lpszEvent)
{
	HKEY hKey;
	DWORD dwBytesReturned = 1000 * sizeof(TCHAR);
	CString strKeyName;
	strKeyName.Format(_T("System\\CurrentControlSet\\Services\\EventLog\\%s\\%s"), strSourceName, lpszESName );

	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKeyName, 0, KEY_READ, &hKey)) {
		return FALSE;
	}

	LONG lErr = RegQueryValueEx(hKey, _T("EventMessageFile"), NULL, NULL, (LPBYTE)lpszEvent, &dwBytesReturned);
	if (ERROR_SUCCESS != lErr) {

		lpszEvent[0] = _T('\0');
	}

	RegCloseKey(hKey);
	return TRUE;
}

LPTSTR CDataCollectManager::GetEventMessage(HMODULE hDll,DWORD dwEventIndex,DWORD dwLanguageID, LPTSTR* lpInserts) 
{
	DWORD  dwReturn;
	LPTSTR lpMsgBuf = NULL;
	DWORD  dwFlags = FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ALLOCATE_BUFFER;

	if (NULL != lpInserts) dwFlags |= FORMAT_MESSAGE_ARGUMENT_ARRAY;

	dwReturn = FormatMessage(dwFlags,
		hDll,
		dwEventIndex,
		dwLanguageID,
		(LPTSTR)&lpMsgBuf,
		0,
		(va_list *)lpInserts);

	return lpMsgBuf;
}

CString CDataCollectManager::GetEventTime(time_t EventDate)
{
	CString ReData = _T("");

	struct tm* ConvertTime = localtime(&EventDate);

	if (NULL != ConvertTime) {
		ReData.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), ConvertTime->tm_year + 1900,
			ConvertTime->tm_mon + 1,
			ConvertTime->tm_mday,
			ConvertTime->tm_hour,
			ConvertTime->tm_min,
			ConvertTime->tm_sec);
	}
	return ReData;
}

UINT CDataCollectManager::SetEventSourceNameCheck(CString strSourceName)
{
	UINT nProductType = 0;
	strSourceName.Trim();

	if (0 == strSourceName.CompareNoCase(_T("ZONECENTRAL"))) {
		nProductType = PRIMX_PRODUCT_TYPE_ZONECENTRAL;
	}
	else if (0 == strSourceName.CompareNoCase(_T("ZEDMAIL"))) {
		nProductType = PRIMX_PRODUCT_TYPE_ZONECENTRAL;
	}
	else if (0 == strSourceName.CompareNoCase(_T("ORIZON"))) {
		nProductType = PRIMX_PRODUCT_TYPE_ORIZON;
	}

	return nProductType;
}

CString CDataCollectManager::GetEventCodeConvert(UINT nProductType, UINT nEventID)
{
	CString strCode;
	strCode.Format(_T("Unknown-%lu"), nEventID);

	if (PRIMX_PRODUCT_TYPE_ZONECENTRAL == nProductType) {
		for (int i = 0; i < _CountOfArray(PRIMX_EVENT_ZONECENTRAL); i++) {
			if (nEventID == PRIMX_EVENT_ZONECENTRAL[i].nCode) {
				strCode = PRIMX_EVENT_ZONECENTRAL[i].strEventCode;
				break;
			}
		}
	}
	if (PRIMX_PRODUCT_TYPE_ORIZON == nProductType) {
		for (int i = 0; i < _CountOfArray(PRIMX_EVENT_ORIZON); i++) {
			if (nEventID == PRIMX_EVENT_ORIZON[i].nCode) {
				strCode = PRIMX_EVENT_ORIZON[i].strEventCode;
				break;
			}
		}
	}

	// 17241 :  정책 변경

	return strCode;
}

BOOL CDataCollectManager::EventLogDataCheck(CFileDBManager& FileDBManager,UINT nEventType, UINT nEventID, time_t tEventDate, CString strServiceName, CString strEventDesc)
{
	BOOL bRe = FALSE;
	UINT nProductType = SetEventSourceNameCheck(strServiceName);
	if (0 == nProductType) {
		return bRe;
	}

	PRIMX_LOG_DATA DBSaveData;
	DBSaveData.nProductType = nProductType;			// 0: 알 수 없음, 1: PRIMX Zonecentral, 2: PRIMX Orizon, 3: PRIMX Cryhod 4: PRIMX Agent
	DBSaveData.strLogDate = GetEventTime(tEventDate);
	DBSaveData.strLogType = strServiceName;
	DBSaveData.strLogCode = GetEventCodeConvert(nProductType, (nEventID & 0xFFFF));
	DBSaveData.strLogData = strEventDesc;
	
	if (EVENTLOG_ERROR_TYPE == nEventType) DBSaveData.strLogLevel = _T("ERROR");
	else if (EVENTLOG_WARNING_TYPE == nEventType) DBSaveData.strLogLevel = _T("WARNING");
	else if (EVENTLOG_INFORMATION_TYPE == nEventType) DBSaveData.strLogLevel = _T("INFO");
	else DBSaveData.strLogLevel = _T("WARNING");
	
	if (TRUE == FileDBManager.SetLogDataSave(DBSaveData,2)) {
		//theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("EventLog SQLite Save > %s, %s, %s, %s, %s "), 
		//	DBSaveData.strLogDate, 
		//	DBSaveData.strLogType, 
		//	DBSaveData.strLogLevel, 
		//	DBSaveData.strLogCode, 
		//	DBSaveData.strLogData.Left(10));
		bRe = TRUE;
	}
	return bRe;
}

