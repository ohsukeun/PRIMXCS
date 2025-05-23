#include "stdafx.h"
#include "FileChangeMonitor.h"
#include "ConfigDataManager.h"

CFileChangeMonitor::CFileChangeMonitor()
{
	m_AuthNotEncryptionIndex = 0;
	m_bThreadRun = FALSE;
	m_hCompletion = NULL;
	m_Primxzc.Initalize();
	m_bRun = FALSE;

	InitializeCriticalSectionAndSpinCount(&m_csRemoveFileLock, 4000);
	InitializeCriticalSectionAndSpinCount(&m_csEncryptionPathLock, 4000);
	InitializeCriticalSectionAndSpinCount(&m_csLock, 4000);
}

CFileChangeMonitor::~CFileChangeMonitor()
{
	SetMonitorStop();

	m_bRun = FALSE;
	WaitForTermination(1000);

	SetCloseAll();

	SetRemoveAllContext();
	SetRemoveAllEncryptionPathList();
	m_RemoveFileList.RemoveAll();

	DeleteCriticalSection(&m_csRemoveFileLock);
	DeleteCriticalSection(&m_csEncryptionPathLock);
	DeleteCriticalSection(&m_csLock);
}

CString CFileChangeMonitor::GetErrToString()
{
	CString strError;
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	strError.Format(_T("[%d]%s"), GetLastError(), (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
	strError.Replace(_T("\r"), _T(""));
	strError.Replace(_T("\n"), _T(""));
	return strError;
}

unsigned int CALLBACK CFileChangeMonitor::MonitorRun(LPVOID pParams)
{
	CFileChangeMonitor*  pThis = (CFileChangeMonitor*)pParams;
	if (NULL == pThis) {
		_endthreadex(0);
		return 0;
	}
	pThis->MonitorRun();
	_endthreadex(0);
	return 0;
}

void CFileChangeMonitor::MonitorRun()
{
	while (TRUE == m_bThreadRun) {
		PRIMX_CONTEXT* pContextData = NULL;
		DWORD dwIoSize = 0;
		LPOVERLAPPED pOverlap = NULL;

		if (FALSE == GetQueuedCompletionStatus(m_hCompletion, &dwIoSize, (PULONG_PTR)&pContextData, (LPOVERLAPPED*)&pOverlap, INFINITE)) {
			if (NULL != pContextData) {
				DWORD dwLastError = GetLastError();
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("GetQueuedCompletionStatus fial > [%lu] %s"), dwLastError,GetLastErrorString() );
				if (ERROR_ACCESS_DENIED == dwLastError) {
					SetRemoveContext(pContextData);
				}
			}
			continue;
		}

		if (pContextData == NULL) {
			break;
		}
		
		if (0 == dwIoSize) {
			SetRemoveContext(pContextData);
			continue;
		}
		
		BYTE* pChageData = NULL;
		CString strDir = pContextData->strDirectory;
		if (0 < dwIoSize) {
			pChageData = new BYTE[dwIoSize];
			memcpy(pChageData, pContextData->lpBuffer, dwIoSize);			
		}

		ReadChangeData(pContextData);

		if (NULL != pChageData) {

			BYTE* pBase = pChageData;

			while (1){
				FILE_NOTIFY_INFORMATION& fni = (FILE_NOTIFY_INFORMATION&)*pBase;

				CString strFilename(fni.FileName, fni.FileNameLength / sizeof(wchar_t));

				//LPCWSTR wszFilename = PathFindFileNameW(wstrFilename);
				//int len = lstrlenW(wszFilename);
				//// The maximum length of an 8.3 filename is twelve, including the dot.
				//if (len <= 12 && wcschr(wszFilename, L'~'))
				//{
				//	// Convert to the long filename form. Unfortunately, this does not work for deletions, so it's an imperfect fix.
				//	wchar_t wbuf[MAX_PATH];
				//	if (::GetLongPathNameW(wstrFilename, wbuf, _countof(wbuf)) > 0)
				//		wstrFilename = wbuf;
				//}
				//m_pServer->m_pBase->Push(fni.Action, wstrFilename);
				
				if (FILE_ACTION_ADDED == fni.Action || FILE_ACTION_MODIFIED == fni.Action) {

					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("파일 추가 > %s > %s"), strDir,strFilename);

					CStringList SearchDataList;
					if (0 < GetClipboardDataSearch(strFilename, SearchDataList)) {
						POSITION Pos = SearchDataList.GetHeadPosition();
						while (NULL != Pos) {
							CString strPath = SearchDataList.GetNext(Pos);
							ZCAPI_FOLDER_TYPE enType = m_Primxzc.SetCloseZone(strPath);
							if (ZCAPI_FOLDER_ENCRYPTED_ZONE == enType) {
								theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("정책 위배 > 파일 추가 > %s %s > %s %s"), strPath, strFilename, strDir, strFilename);
								CString strFilePath;
								strFilePath.Format(_T("%s\\%s"), strDir, strFilename);
								SetRemoveFileData(strFilePath);
							}
						}
					}
				}

				if (!fni.NextEntryOffset) break;

				pBase += fni.NextEntryOffset;
			}

			delete[] pChageData;
			pChageData = NULL;
		}
	}
}

void CFileChangeMonitor::SetRemoveAllContext()
{
	EnterCriticalSection(&m_csLock);
	POSITION Pos = m_ContextList.GetHeadPosition();
	while(NULL != Pos) {
		PRIMX_CONTEXT* pContextData = m_ContextList.GetNext(Pos);
		_SafeDelete(pContextData);
	}
	m_ContextList.RemoveAll();
	LeaveCriticalSection(&m_csLock);
}
void CFileChangeMonitor::SetRemoveAllEncryptionPathList()
{
	EnterCriticalSection(&m_csEncryptionPathLock);
	POSITION Pos = m_EncryptionPathList.GetHeadPosition();
	while (NULL != Pos) {
		PRIMX_MONITOR_PATH_INFO* pData = m_EncryptionPathList.GetNext(Pos);
		_SafeDelete(pData);
	}
	m_EncryptionPathList.RemoveAll();
	LeaveCriticalSection(&m_csEncryptionPathLock);
}

void CFileChangeMonitor::SetRemoveContext(PRIMX_CONTEXT* pContextData)
{
	if (NULL != pContextData) {
		EnterCriticalSection(&m_csLock);
		POSITION Pos = m_ContextList.Find(pContextData);
		if (NULL != Pos) {
			m_ContextList.RemoveAt(Pos);
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("폴더 모니터링 중지 > %s"), pContextData->strDirectory);
			_SafeDelete(pContextData);
		}
		LeaveCriticalSection(&m_csLock);
	}
}
void CFileChangeMonitor::SetPathMonitorStop(CString strMonitorPath)
{
	strMonitorPath.Trim();
	if (0  < strMonitorPath.GetLength()) {
		EnterCriticalSection(&m_csLock);

		POSITION Pos = m_ContextList.GetHeadPosition();
		while (NULL != Pos) {
			PRIMX_CONTEXT* pContextData = m_ContextList.GetNext(Pos);
			if (NULL != pContextData) {
				if (0 == strMonitorPath.CompareNoCase(pContextData->strDirectory)) {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("폴더 모니터링 중지 > %s"), pContextData->strDirectory);
					CloseHandle(pContextData->hDirectory);
				}
			}
		}
		LeaveCriticalSection(&m_csLock);
	}
}

BOOL CFileChangeMonitor::SetMonitorStart(UINT nThreadCount)
{
	m_bRun = TRUE;
	Start();

	m_hCompletion = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_nThreadCount);
	if (INVALID_HANDLE_VALUE == m_hCompletion) {
		return FALSE;
	}

	m_nThreadCount = nThreadCount + 1;

	BOOL bCreateThread = TRUE;
	m_bThreadRun = TRUE;
	for (UINT i = 0; i<m_nThreadCount; i++) {
		UINT nThreadID = 0;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, MonitorRun, (LPVOID)this, 0, &nThreadID);
		if (NULL == hThread) {
			bCreateThread = FALSE;
		}
		else {
			m_ThradList.AddTail(hThread);
		}
	}
	if (FALSE == bCreateThread) {
		SetMonitorStop();
		return FALSE;
	}

	SetAuthNotEncryptionListCheck();

	return TRUE;
}

void CFileChangeMonitor::SetMonitorStop()
{
	DWORD dwBytesTransferred = 0;
	DWORD dwCompKey = 0;
	
	for (UINT i = 0; i < m_nThreadCount; i++) {
		PostQueuedCompletionStatus(m_hCompletion, dwBytesTransferred, dwCompKey, NULL);
	}

	m_bThreadRun = FALSE;
	POSITION Pos = m_ThradList.GetHeadPosition();
	while (NULL != Pos) {
		HANDLE hThread = m_ThradList.GetNext(Pos);
		if (NULL != hThread) {
			if (WAIT_OBJECT_0 != ::WaitForSingleObject(hThread, 500)) {
				::TerminateThread(hThread, 0);
			}
			CloseHandle(hThread);
			hThread = NULL;
		}
	}
	m_ThradList.RemoveAll();
	
	CloseHandle(m_hCompletion);
	m_hCompletion = NULL;
}

PRIMX_CONTEXT* CFileChangeMonitor::CreateContext()
{
	PRIMX_CONTEXT* pContextData = new PRIMX_CONTEXT;

	return pContextData;
}

void CFileChangeMonitor::SetCloseAll()
{
	EnterCriticalSection(&m_csLock);
	POSITION Pos = m_ContextList.GetHeadPosition();
	while (NULL != Pos) {
		PRIMX_CONTEXT* pContextData = m_ContextList.GetNext(Pos);
		if (NULL != pContextData) {
			CloseHandle(pContextData->hDirectory);
		}
	}
	LeaveCriticalSection(&m_csLock);
}

PRIMX_CONTEXT* CFileChangeMonitor::UpdateCompletionPort(CString strDirectory)
{
	PRIMX_CONTEXT* pContextData = CreateContext();
	if (NULL != pContextData) {
		pContextData->strDirectory = strDirectory;
		pContextData->hDirectory = ::CreateFileW(
			pContextData->strDirectory,			// pointer to the file name
			FILE_LIST_DIRECTORY,                // access (read/write) mode
			FILE_SHARE_READ						// share mode
			| FILE_SHARE_WRITE
			| FILE_SHARE_DELETE,
			NULL,                               // security descriptor
			OPEN_EXISTING,                      // how to create
			FILE_FLAG_BACKUP_SEMANTICS			// file attributes
			| FILE_FLAG_OVERLAPPED,
			NULL);                              // file with attributes to copy

		if (INVALID_HANDLE_VALUE == pContextData->hDirectory){
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("폴더 모니터링 시작 실패 > %s > %s"), strDirectory, GetLastErrorString());
			delete pContextData;
			pContextData = NULL;
			return NULL;
		}
	}
	else {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("폴더 모니터링 시작 실패 > %s > ContextData 생성 실패"), strDirectory);
		return NULL;
	}

	if (NULL == CreateIoCompletionPort(pContextData->hDirectory, m_hCompletion, (ULONG_PTR)pContextData, 0)) {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("폴더 모니터링 시작 실패 > %s > CreateIoCompletionPort Fail"), strDirectory);
		CloseHandle(pContextData->hDirectory);
		delete pContextData;
		pContextData = NULL;
		return NULL;
	}

	if (NULL == m_ContextList.AddTail(pContextData)) {
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("폴더 모니터링 시작 실패 > %s > ContextData 저장 실패"), strDirectory);
		CloseHandle(pContextData->hDirectory);
		delete pContextData;
		pContextData = NULL;
		return NULL;
	}

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("폴더 모니터링 시작 > %s"), pContextData->strDirectory);
	
	ReadChangeData(pContextData);

	return pContextData;
}

BOOL CFileChangeMonitor::ReadChangeData(PRIMX_CONTEXT* pContextData)
{
	BOOL bRe = FALSE;
	if (NULL != pContextData) {
		pContextData->SetBufferClear();
		bRe = ::ReadDirectoryChangesW(
			pContextData->hDirectory,
			pContextData->lpBuffer,
			pContextData->nBufferLength,
			pContextData->bWatchSubtree,
			pContextData->dwNotifyFilter,
			&pContextData->dwBytesReturned,
			&pContextData->OverLapped,
			NULL);
	}
	if (FALSE == bRe) {
		TRACE(_T("ReadDirectoryChangesW > FALSE > %s"), GetErrToString());
	}
	return bRe;
}

UINT CFileChangeMonitor::GetClipboardDataSearch(CString strSearchFileName, CStringList& SearchDataList)
{
	if (FALSE == IsClipboardFormatAvailable(CF_HDROP)) return 0;

	if (TRUE == ::OpenClipboard(NULL)) {

		HGLOBAL hGlobal = (HGLOBAL)GetClipboardData(CF_HDROP);
		if (hGlobal) {
			HDROP hDrop = (HDROP)GlobalLock(hGlobal);
			if (hDrop)
			{
				UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, 0, 0);
				UINT filenameLength;
				for (UINT i = 0; i < fileCount; ++i)
				{
					TCHAR lpszFileName[MAX_PATH] = { 0 };
					filenameLength = DragQueryFile(hDrop, i, 0, 0);
					DragQueryFile(hDrop, i, lpszFileName, filenameLength + 1);

					TCHAR pFilename[MAX_PATH] = { 0 };
					TCHAR pExt[MAX_PATH] = { 0 };
					TCHAR pDrive[MAX_PATH] = { 0 };
					TCHAR pDir[MAX_PATH] = { 0 };

					_tsplitpath_s(lpszFileName, pDrive, pDir, pFilename, pExt);

					CString strFileName;
					CString strPath;
					strFileName.Format(_T("%s%s"), pFilename, pExt);
					strPath.Format(_T("%s%s"), pDrive, pDir);
					strPath.MakeLower();

					if (0 == strFileName.CompareNoCase(strSearchFileName)) {
						if (NULL == SearchDataList.Find(strPath)) {
							SearchDataList.AddTail(strPath);
							theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"Clipboard > %s > %s", strSearchFileName, strPath);
						}
					}
				}
				GlobalUnlock(hGlobal);
			}
		}
		CloseClipboard();
	}
	return SearchDataList.GetCount();
}

CString CFileChangeMonitor::GetDirectoryCreateDateTime(CString strDirPath)
{
	CString strDate;

	strDirPath.Trim();

	if (TRUE == FileExist(strDirPath)) {
		HANDLE hDirectory = ::CreateFileW(
			strDirPath,			// pointer to the file name
			FILE_LIST_DIRECTORY,                // access (read/write) mode
			FILE_SHARE_READ,						// share mode
			NULL,                               // security descriptor
			OPEN_EXISTING,                      // how to create
			FILE_FLAG_BACKUP_SEMANTICS			// file attributes
			| FILE_FLAG_OVERLAPPED,
			NULL);                              // file with attributes to copy

		if (INVALID_HANDLE_VALUE != hDirectory) {
			FILETIME tCreationTime;
			FILETIME tLastAccessTime;
			FILETIME tLastWriteTime;

			tCreationTime.dwHighDateTime = 0;
			tLastAccessTime.dwHighDateTime = 0;
			tLastWriteTime.dwHighDateTime = 0;
			tCreationTime.dwLowDateTime = 0;
			tLastAccessTime.dwLowDateTime = 0;
			tLastWriteTime.dwLowDateTime = 0;

			if (TRUE == GetFileTime(hDirectory, &tCreationTime, &tLastAccessTime, &tLastWriteTime)) {
				CTime tFileTime(tCreationTime);
				strDate = tFileTime.Format(_T("%Y%m%d%H%M%S"));
			}
			CloseHandle(hDirectory);
		}
	}
	return strDate;
}


void CFileChangeMonitor::Run()
{
	__time64_t tAuthNotEncryptionCheckTime = (_time64(NULL) + 60);
	__time64_t tEncryptionPathCheckTime = (_time64(NULL) + 5);

	while(m_bRun) {
		if (tAuthNotEncryptionCheckTime < _time64(NULL)) {
			SetAuthNotEncryptionListCheck();
			tAuthNotEncryptionCheckTime = (_time64(NULL) + 60);
		}

		if (tEncryptionPathCheckTime < _time64(NULL)) {
			SetEncryptionPathCheck();
			tEncryptionPathCheckTime = (_time64(NULL) + 5);
		}

		CString strFilePath = GetRemoveFileData();
		if (0 < strFilePath.GetLength()) {
			if(FALSE == SetDeleteFileEx(strFilePath)) {
				SetRemoveFileData(strFilePath);
			}
			else {
				theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("복사된 파일 삭제 > %s"), strFilePath);
			}
		}

		Sleep(50);
	}
}

void CFileChangeMonitor::SetAuthNotEncryptionListCheck()
{	
	CConfigDataManager ConfigDataManager;
	PRIMX_POLICY_DATA PolicyData;
	if (TRUE == ConfigDataManager.GetPolicyData(PolicyData)) {
		if (0 != PolicyData.strAuthNotEncryptionStamp.CompareNoCase(m_strAuthNotEncryptionTimeStamp)) {
			
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("폴더 모니터링 정책 변경 > %s > %s"), m_strAuthNotEncryptionTimeStamp, PolicyData.strAuthNotEncryptionStamp);

			m_strAuthNotEncryptionTimeStamp = PolicyData.strAuthNotEncryptionStamp;

			int nIndex = 0;
			if (0 == m_AuthNotEncryptionIndex) {
				nIndex = 1;
			}
			else {
				nIndex = 0;
			}

			if (TRUE == PolicyData.bAuthNotEncryption) {
				m_AuthNotEncryptionList[nIndex].RemoveAll();

				POSITION Pos = PolicyData.AuthNotEncryptionPathList.GetHeadPosition();
				while (NULL != Pos) {
					CString strData = PolicyData.AuthNotEncryptionPathList.GetNext(Pos);
					strData.Trim();
					strData.MakeLower();
					m_AuthNotEncryptionList[nIndex].AddTail(strData);
				}
			}
			else{
				m_AuthNotEncryptionList[nIndex].RemoveAll();
			}
			m_AuthNotEncryptionIndex = nIndex;
			SetCloseAll();
			SetRemoveAllEncryptionPathList();

			POSITION Pos = PolicyData.AuthNotEncryptionPathList.GetHeadPosition();
			while (NULL != Pos) {
				CString strData = PolicyData.AuthNotEncryptionPathList.GetNext(Pos);
				strData.Trim();
				strData.MakeLower();

				BOOL bMonitorStart = FALSE;
				if (TRUE == FileExist(strData)) {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("폴더 모니터링 추가 > %s"), strData);
					if (NULL != UpdateCompletionPort(strData)) {
						bMonitorStart = TRUE;
					}
				}
				else {
					theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), _T("폴더 모니터링 추가 실패 > 해당 폴더 존재 안함 > %s"), strData);
				}
				SetEncryptionPathListInsert(strData, bMonitorStart);
		
			}
		}
	}
	
//	theLogManager.LogPrintLine();
}

void CFileChangeMonitor::SetEncryptionPathCheck()
{
	EnterCriticalSection(&m_csEncryptionPathLock);

	POSITION Pos = m_EncryptionPathList.GetHeadPosition();
	while (NULL != Pos) {
		PRIMX_MONITOR_PATH_INFO* pData = m_EncryptionPathList.GetNext(Pos);
		if (NULL != pData) {
			BOOL bExist = FileExist(pData->strDirectory);
			if (TRUE == bExist) {
				if (FALSE == pData->bMonitorStart) {
					if (NULL != UpdateCompletionPort(pData->strDirectory)) {
						pData->bMonitorStart = TRUE;
						if (0 == pData->strCreateDateTime.GetLength()) {
							pData->strCreateDateTime = GetDirectoryCreateDateTime(pData->strDirectory);
						}
					}
				}
				else {
					CString strCreateDateTime = GetDirectoryCreateDateTime(pData->strDirectory);
					if (0 != strCreateDateTime.CompareNoCase(pData->strCreateDateTime)) {
						if (NULL != UpdateCompletionPort(pData->strDirectory)) {
							pData->bMonitorStart = TRUE;
							pData->strCreateDateTime = strCreateDateTime;
						}
					}
				}
			}
			else{
				if (TRUE == pData->bMonitorStart) {
					SetPathMonitorStop(pData->strDirectory);
				}
			}
		}
		Sleep(0);
	}
	LeaveCriticalSection(&m_csEncryptionPathLock);
}

void CFileChangeMonitor::SetEncryptionPathListInsert(CString strPath, BOOL bMonitorStart)
{
	EnterCriticalSection(&m_csEncryptionPathLock);
	BOOL bExist = FALSE;
	POSITION Pos = m_EncryptionPathList.GetHeadPosition();
	while (NULL != Pos) {
		PRIMX_MONITOR_PATH_INFO* pData = m_EncryptionPathList.GetNext(Pos);
		if (NULL != pData) {
			if (0 == strPath.CompareNoCase(pData->strDirectory)) {
				pData->bMonitorStart = bMonitorStart;
				pData->strCreateDateTime = GetDirectoryCreateDateTime(pData->strDirectory);
				bExist = TRUE;
				break;
			}
		}
		Sleep(0);
	}
	if (FALSE == bExist) {
		PRIMX_MONITOR_PATH_INFO* pPath = new PRIMX_MONITOR_PATH_INFO;
		pPath->strDirectory = strPath;
		pPath->strCreateDateTime = GetDirectoryCreateDateTime(strPath);
		pPath->bMonitorStart = bMonitorStart;
		if (NULL == m_EncryptionPathList.AddTail(pPath)) {
			_SafeDelete(pPath);
		}
	}
	LeaveCriticalSection(&m_csEncryptionPathLock);

}

void CFileChangeMonitor::SetEncryptionPathListRemove(CString strPath)
{
	EnterCriticalSection(&m_csEncryptionPathLock);
	BOOL bExist = FALSE;
	POSITION PosDel = NULL;
	POSITION Pos = m_EncryptionPathList.GetHeadPosition();
	while (NULL != Pos) {
		PosDel = Pos;
		PRIMX_MONITOR_PATH_INFO* pData = m_EncryptionPathList.GetNext(Pos);
		if (NULL != pData) {
			if (0 == strPath.CompareNoCase(pData->strDirectory)) {
				m_EncryptionPathList.RemoveAt(PosDel);
				_SafeDelete(pData);
			}
		}
		Sleep(0);
	}
	LeaveCriticalSection(&m_csEncryptionPathLock);
}

void CFileChangeMonitor::SetRemoveFileData(CString strFilePath)
{
	EnterCriticalSection(&m_csRemoveFileLock);

	m_RemoveFileList.AddTail(strFilePath);

	LeaveCriticalSection(&m_csRemoveFileLock);
}

CString CFileChangeMonitor::GetRemoveFileData()
{
	CString strFilePath;
	EnterCriticalSection(&m_csRemoveFileLock);
	if (0 < m_RemoveFileList.GetCount()) {
		strFilePath = m_RemoveFileList.RemoveHead();
	}
	LeaveCriticalSection(&m_csRemoveFileLock);
	return strFilePath;
}