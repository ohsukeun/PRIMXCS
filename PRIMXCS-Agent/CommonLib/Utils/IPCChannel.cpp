#include "StdAfx.h"
#include "IPCChannel.h"
#include  <Aclapi.h>

CIPCChannel::CIPCChannel(void)
{
	for(int i=0;i<PRIMX_IPC_TH_MAX;i++){
		m_hThread[i] = NULL;
		m_dwThreadID[i] = NULL;
		m_bThreadRun[i] = FALSE;
	}

	for(int i=0;i<PRIMX_IPC_MAX;i++){
		m_pMapMemory[i] = NULL;
		m_hMutexLock[i] = NULL;
	}

	m_hFileMapping = NULL;
	m_hFile = NULL;
	m_pNotificationFunc = NULL;
	m_lpObject = NULL;
	m_nMaxSize = 0;
	m_bConnect = FALSE;
	m_nMode = 0;

	InitializeCriticalSectionAndSpinCount(&m_csLock,4000);
}


CIPCChannel::~CIPCChannel(void)
{
	SetClose();
	DeleteCriticalSection(&m_csLock);
}
CString CIPCChannel::GetLastErrorString()
{
	return m_strLastErrorString;
}

UINT32 CIPCChannel::GetGMTime()
{
	__time32_t t;
	t = _time32(NULL);
	return UINT32(t);
}

CString CIPCChannel::GetLocalPath()
{
	CString Path;
	CString FileName;
	int pos = 0;
	int pos_b = 0;

	TCHAR szWindowsPath[MAX_PATH*2 + 1] = {0};

	GetModuleFileNameW(NULL,szWindowsPath,MAX_PATH*2);
	Path = FileName = szWindowsPath;

	while(1){		
		pos = FileName.Find(_T("\\"),pos); 

		if(-1 == pos){
			FileName = FileName.Right(FileName.GetLength() - (pos_b));   
			break;
		}
		else{
			pos = pos + 1;
			pos_b = pos;
		}			
	}

	Path = Path.Left(Path.GetLength() - (FileName.GetLength()+1)); 
	return Path;
}


CString CIPCChannel::GetErrorToString()
{
	CString strError;
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	strError = (LPCTSTR)lpMsgBuf;
	LocalFree( lpMsgBuf );

	strError.Replace(_T("\r"), _T(""));
	strError.Replace(_T("\n"), _T(""));

	return strError;
}

void CIPCChannel::SetRemoveAll()
{
	EnterCriticalSection(&m_csLock);
	POSITION Pos = m_ReceiveDataList.GetHeadPosition();
	while(NULL != Pos){
		PRIMX_DATA_INFO* pInfo = m_ReceiveDataList.GetNext(Pos);
		delete pInfo;
		pInfo = NULL;
	}
	m_ReceiveDataList.RemoveAll();
	LeaveCriticalSection(&m_csLock);	
}

BOOL CIPCChannel::SetReceiveNotification(IPC_NOTIFICATION_FUNCTION pNotificationFunc,LPVOID lpObject)
{
	m_pNotificationFunc = pNotificationFunc;
	m_lpObject = lpObject;
	
	m_bThreadRun[PRIMX_IPC_NOTI_TH] = TRUE;

	m_hThread[PRIMX_IPC_NOTI_TH] = (HANDLE) _beginthreadex(0,0,CIPCChannel::__THREAD_NOTI_POINT,this,0,(unsigned *)&m_dwThreadID[PRIMX_IPC_NOTI_TH]);
	if(NULL == m_hThread[PRIMX_IPC_NOTI_TH]) return FALSE;

	return TRUE;
}

unsigned __stdcall CIPCChannel::__THREAD_ENTRY_POINT(void* pVoid)
{
	CIPCChannel* pThis = (CIPCChannel*)pVoid;
	pThis->Run();
	_endthreadex(0);
	return 0;
}

unsigned __stdcall CIPCChannel::__THREAD_NOTI_POINT(void* pVoid)
{
	CIPCChannel* pThis = (CIPCChannel*)pVoid;
	pThis->RunForNoti();
	_endthreadex(0);
	return 0;
}

BOOL CIPCChannel::SetReceiveStart()
{
	m_hThread[PRIMX_IPC_RECEIVE_TH] = (HANDLE) _beginthreadex(0,0,CIPCChannel::__THREAD_ENTRY_POINT,this,0,(unsigned *)&m_dwThreadID[PRIMX_IPC_RECEIVE_TH]);
	if(NULL == m_hThread[PRIMX_IPC_RECEIVE_TH]) return FALSE;
	return TRUE;
}

BOOL CIPCChannel::SetReceiveTermination(DWORD dwMilliSeconds /*= INFINITE*/)
{
	for(int i=0;i<PRIMX_IPC_TH_MAX;i++){
		if(NULL != m_hThread[i]){
			if(WAIT_OBJECT_0 != ::WaitForSingleObject(m_hThread[i], dwMilliSeconds)){
				::TerminateThread(m_hThread[i], 0);
				CloseHandle(m_hThread[i]);
				m_hThread[i] = NULL;
			}			
		}
	}
	return TRUE;
}

BOOL CIPCChannel::SetSecurityAttributes(SECURITY_ATTRIBUTES& sa)
{
	DWORD dwRes;
	PSID pEveryoneSID = NULL, pAdminSID = NULL;
	PACL pACL = NULL;
	PSECURITY_DESCRIPTOR pSD = NULL;
	EXPLICIT_ACCESS ea;
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;  

	// Create a well-known SID for the Everyone group.
	if(!AllocateAndInitializeSid(&SIDAuthWorld, 1,SECURITY_WORLD_RID,0, 0, 0, 0, 0, 0, 0,&pEveryoneSID))
	{
		TRACE(_T("AllocateAndInitializeSid error : %s"),GetErrorToString());	
		return FALSE;
	}

	// Initialize an EXPLICIT_ACCESS structure for an ACE.
	// The ACE will allow Everyone read access to the key.
	ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
	ea.grfAccessPermissions = GENERIC_ALL;
	ea.grfAccessMode = SET_ACCESS;
	ea.grfInheritance= NO_INHERITANCE;
	ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
	ea.Trustee.ptstrName  = (LPTSTR) pEveryoneSID;

	// Create a SID for the BUILTIN\Administrators group.
	if(! AllocateAndInitializeSid(&SIDAuthNT, 2,SECURITY_BUILTIN_DOMAIN_RID,DOMAIN_ALIAS_RID_ADMINS,0, 0, 0, 0, 0, 0,&pAdminSID))
	{
		TRACE(_T("AllocateAndInitializeSid error : %s"),GetErrorToString());	
		return FALSE;
	}
	// Create a new ACL that contains the new ACEs.
	dwRes = SetEntriesInAcl(1, &ea, NULL, &pACL);
	if (ERROR_SUCCESS != dwRes)
	{
		TRACE(_T("SetEntriesInAcl error : %s"),GetErrorToString());	
		return FALSE;
	}

	// Initialize a security descriptor.
	pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (NULL == pSD)
	{
		TRACE(_T("LocalAlloc error : %s"),GetErrorToString());	
		return FALSE;
	}

	if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
	{
		TRACE(_T("InitializeSecurityDescriptor error : %s"),GetErrorToString());	
		return FALSE;
	}

	// Add the ACL to the security descriptor.
	if (!SetSecurityDescriptorDacl(pSD,
		TRUE,     // bDaclPresent flag
		pACL,
		FALSE))   // not a default DACL
	{
		TRACE(_T("SetSecurityDescriptorDacl error : %s"),GetErrorToString());	
		return FALSE;
	}

	// Initialize a security attributes structure.
	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = FALSE;

	return TRUE;
}

void CIPCChannel::SetMapFileDelete(CString strIPCKeyName)
{
	CString strMapFilePath;
	CString strLocalPath = GetMapFileCreatePath();
	strMapFilePath.Format(_T("%s\\%s*.pma"),strLocalPath,strIPCKeyName);
	
	CFileFind FileFind;
	BOOL bWorking = FileFind.FindFile(strMapFilePath);
	while (bWorking){
		bWorking = FileFind.FindNextFile();
		if (FileFind.IsDots()){
			continue;
		}
		if(TRUE == FileFind.IsDirectory()){
			continue;
		}
		if(TRUE == FileFind.IsArchived()){
			CString strFullPath;
			strFullPath.Format(_T("%s\\%s"),strLocalPath,FileFind.GetFileName());
			::DeleteFile(strFullPath);
		}
		Sleep(100);
	}
	FileFind.Close();

}
BOOL CIPCChannel::SetCreateDirectoryEx(CString strDirectory)
{
	BOOL Re = FALSE;

	int returecode = SHCreateDirectoryEx(NULL, strDirectory, NULL);
	if (ERROR_SUCCESS != returecode) {
		if (!(ERROR_FILE_EXISTS == returecode || ERROR_ALREADY_EXISTS == returecode)) {
			return FALSE;
		}
	}
	return TRUE;
}
CString CIPCChannel::GetMapFileCreatePath()
{
	CString strMapFileCreatePath;

	TCHAR szPath[MAX_PATH] = { 0 };
	if (TRUE == SHGetSpecialFolderPath(NULL, szPath, CSIDL_COMMON_APPDATA, 0)) {
		strMapFileCreatePath.Format(PRIMX_AGNET_INSTALL_PATH_FORMAT, szPath);
	}
	else {
		if (0 < ::ExpandEnvironmentStrings(_T("%ProgramData%"), szPath, MAX_PATH)) {
			strMapFileCreatePath.Format(PRIMX_AGNET_INSTALL_PATH_FORMAT, szPath);
		}
		else {
			if (0 != GetWindowsDirectory(szPath, MAX_PATH)) {
				TCHAR pDrive[MAX_PATH] = { 0 };
				TCHAR pDir[MAX_PATH] = { 0 };
				TCHAR pFileName[MAX_PATH] = { 0 };
				TCHAR pFileExt[MAX_PATH] = { 0 };
				_tsplitpath_s(szPath, pDrive, MAX_PATH, pDir, MAX_PATH, pFileName, MAX_PATH, pFileExt, MAX_PATH);
				strMapFileCreatePath.Format(PRIMX_AGNET_INSTALL_PROGRAMDATA_PATH_FORMAT, pDrive);
			}
			else {
				strMapFileCreatePath.Format(PRIMX_AGNET_INSTALL_PROGRAMDATA_PATH_FORMAT, _T("c:"));
			}
		}
	}

	SetCreateDirectoryEx(strMapFileCreatePath);

	return strMapFileCreatePath;
}
BOOL CIPCChannel::SetServerStart(CString strIPCKeyName,UINT nMaxSize)
{
	if(0 != m_nMode){
		m_strLastErrorString = _T("duplication initialize.");
		return FALSE;
	}
	m_strLastErrorString = _T("");

	SetMapFileDelete(strIPCKeyName);

	EnterCriticalSection(&m_csLock);
	
	BOOL bSecurityAttributes = FALSE;
	SECURITY_ATTRIBUTES sa;
	bSecurityAttributes = SetSecurityAttributes(sa);

	m_strMMFFilePath.Format(_T("%s\\%s_%lu_%lu.pma"), GetMapFileCreatePath(),strIPCKeyName,nMaxSize,GetGMTime());
	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"프로세스간 통신 > IPC File Path > %s", m_strMMFFilePath);

	m_hFile = CreateFile(m_strMMFFilePath,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(m_hFile == INVALID_HANDLE_VALUE){
		LeaveCriticalSection(&m_csLock);
		m_strLastErrorString = GetErrorToString();
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"프로세스간 통신 > CreateFile error > %s", m_strLastErrorString);
		return FALSE;
	}
	m_strIPCKeyName = strIPCKeyName;
	m_nMaxSize = nMaxSize;

	UINT nMaxSizeEx = (nMaxSize+sizeof(PRIMX_BUFFER_HEADER)) * 2;
	
	CString strKey;
	strKey = _T("Session\\") + strIPCKeyName;

	if(TRUE == bSecurityAttributes){
		m_hFileMapping = ::CreateFileMapping(m_hFile,&sa,PAGE_READWRITE,0, nMaxSizeEx,LPCTSTR(strKey));
	}
	else{
		m_hFileMapping = ::CreateFileMapping(m_hFile,NULL,PAGE_READWRITE,0,	nMaxSizeEx,LPCTSTR(strKey));
	}

	if(NULL != m_hFileMapping){
		unsigned char* pMapMemory = (unsigned char*)::MapViewOfFile(m_hFileMapping,FILE_MAP_WRITE,0,0,nMaxSizeEx);
		if(NULL == pMapMemory){
			m_strLastErrorString = GetErrorToString();
			theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"프로세스간 통신 > MapViewOfFile error > %s", m_strLastErrorString);

			if(NULL != m_hFileMapping){
				CloseHandle(m_hFileMapping);
				m_hFileMapping = NULL;
			}
			if(NULL != m_hFile){
				CloseHandle(m_hFile);
				m_hFile = NULL;
			}
			LeaveCriticalSection(&m_csLock);
			return FALSE;
		}
		else{
			m_pMapMemory[PRIMX_IPC_READ] = (LPVOID)pMapMemory;
			m_pMapMemory[PRIMX_IPC_WRITE] = (LPVOID)(pMapMemory + (nMaxSize+sizeof(PRIMX_BUFFER_HEADER)));
		}
	}
	else{
		m_strLastErrorString = GetErrorToString();
		theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"프로세스간 통신 > CreateFileMapping error > %s", m_strLastErrorString);
		LeaveCriticalSection(&m_csLock);
		return FALSE;
	}

	CString strMutexReadLockName;
	CString strMutexWriteLockName;

	strMutexReadLockName.Format(_T("%s_MUTEX_LOCK_READ"),strIPCKeyName);
	strMutexWriteLockName.Format(_T("%s_MUTEX_LOCK_WRITE"),strIPCKeyName);

	m_hMutexLock[PRIMX_IPC_READ] = ::CreateMutex(NULL,FALSE,strMutexReadLockName);
	m_hMutexLock[PRIMX_IPC_WRITE] = ::CreateMutex(NULL,FALSE,strMutexWriteLockName);

	if(NULL == m_hMutexLock[PRIMX_IPC_READ] || NULL == m_hMutexLock[PRIMX_IPC_WRITE]){
		if(NULL != m_hFileMapping){
			CloseHandle(m_hFileMapping);
			m_hFileMapping = NULL;
		}
		if(NULL != m_hFile){
			CloseHandle(m_hFile);
			m_hFile = NULL;
		}
		for(UINT i=0;i<PRIMX_IPC_MAX;i++){
			if (NULL != m_hMutexLock[i]){
				::CloseHandle(m_hMutexLock[i]);
				m_hMutexLock[i] = NULL;
			}
		}
		m_strLastErrorString = _T("invalid ipc memory.");
		LeaveCriticalSection(&m_csLock);
		return FALSE;
	}
	
	m_bThreadRun[PRIMX_IPC_RECEIVE_TH] = TRUE;
	if(FALSE == SetReceiveStart()){
		if(NULL != m_hFileMapping){
			CloseHandle(m_hFileMapping);
			m_hFileMapping = NULL;
		}
		if(NULL != m_hFile){
			CloseHandle(m_hFile);
			m_hFile = NULL;
		}
		for(UINT i=0;i<PRIMX_IPC_MAX;i++){
			if (NULL != m_hMutexLock[i]){
				::CloseHandle(m_hMutexLock[i]);
				m_hMutexLock[i] = NULL;
			}
		}
		m_strLastErrorString = _T("receive thread start fail.");
		LeaveCriticalSection(&m_csLock);
		return FALSE;
	}

	LeaveCriticalSection(&m_csLock);
	
	m_nMode = 100;

	theLogManager.LogPrint(LOG_INF, GetCurrentThreadId(), L"프로세스간 통신 > IPC KeyName > %s", strIPCKeyName);

	return TRUE;
}

void CIPCChannel::SetServerStop()
{
	SetClose();
}

BOOL CIPCChannel::SetConnect(CString strIPCKeyName,UINT nMaxSize)
{
	m_nMode = 200;
	m_strIPCKeyName = strIPCKeyName;
	m_nMaxSize = nMaxSize;
	m_strLastErrorString = _T("");

	EnterCriticalSection(&m_csLock);

	CString strKey;
	strKey = _T("Session\\") + strIPCKeyName;

	m_hFileMapping = OpenFileMapping(SECTION_ALL_ACCESS, TRUE, LPCTSTR(strKey));
	if(NULL != m_hFileMapping){
		unsigned char* pMapMemory = (unsigned char*)MapViewOfFile(m_hFileMapping, FILE_MAP_WRITE, 0, 0, 0); 

		m_pMapMemory[PRIMX_IPC_WRITE] = (LPVOID)pMapMemory;
		m_pMapMemory[PRIMX_IPC_READ] = (LPVOID)(pMapMemory + (nMaxSize+sizeof(PRIMX_BUFFER_HEADER)));
		
	}
	else{
		LeaveCriticalSection(&m_csLock);
		m_strLastErrorString = GetErrorToString();
		TRACE(_T("OpenFileMapping error : %s"),m_strLastErrorString);	
		return FALSE;
	}

	CString strMutexReadLockName;
	CString strMutexWriteLockName;

	strMutexReadLockName.Format(_T("%s_MUTEX_LOCK_READ"),strIPCKeyName);
	strMutexWriteLockName.Format(_T("%s_MUTEX_LOCK_WRITE"),strIPCKeyName);

	m_hMutexLock[PRIMX_IPC_WRITE] = ::CreateMutex(NULL,FALSE,strMutexReadLockName);
	m_hMutexLock[PRIMX_IPC_READ] = ::CreateMutex(NULL,FALSE,strMutexWriteLockName);
	
	m_bThreadRun[PRIMX_IPC_RECEIVE_TH] = TRUE;
	if(FALSE == SetReceiveStart()){
		if(NULL != m_hFileMapping){
			CloseHandle(m_hFileMapping);
			m_hFileMapping = NULL;
		}
		for(UINT i=0;i<PRIMX_IPC_MAX;i++){
			if (NULL != m_hMutexLock[i]){
				::CloseHandle(m_hMutexLock[i]);
				m_hMutexLock[i] = NULL;
			}
		}
		m_strLastErrorString = _T("receive thread start fail.");
		LeaveCriticalSection(&m_csLock);
		return FALSE;
	}

	LeaveCriticalSection(&m_csLock);
	m_bConnect = TRUE;
	return TRUE;
}

void CIPCChannel::SetClose()
{
	//CriticalSection 겹침 현상으로 인해 주석처리
	//EnterCriticalSection(&m_csLock);

	SetReceiveTermination(500);

	if(NULL != m_hFileMapping){
		CloseHandle(m_hFileMapping);
		m_hFileMapping = NULL;
	}
	if(NULL != m_pMapMemory[PRIMX_IPC_READ]){
		::UnmapViewOfFile(m_pMapMemory[PRIMX_IPC_READ]);
		m_pMapMemory[PRIMX_IPC_READ] = NULL;
		m_pMapMemory[PRIMX_IPC_WRITE] = NULL;
	}

	for(UINT i=0;i<PRIMX_IPC_MAX;i++){
		if (NULL != m_hMutexLock[i]){
			::CloseHandle(m_hMutexLock[i]);
			m_hMutexLock[i] = NULL;
		}
	}
	if(NULL != m_hFile){
		::CloseHandle(m_hFile);
		m_hFile = NULL;
	}
	::DeleteFile(m_strMMFFilePath);

	//LeaveCriticalSection(&m_csLock);

	SetRemoveAll();
	m_nMode = 0;
}

BOOL CIPCChannel::SetReceiveDataSave(char* pBuffer,UINT nSize)
{
	BOOL bRe = FALSE;
	if(NULL == pBuffer || 0 == nSize){
		return bRe;
	}
	EnterCriticalSection(&m_csLock);
	PRIMX_DATA_INFO* pData = new PRIMX_DATA_INFO;
	pData->nBufferSize = nSize;
	pData->pData = pBuffer;

	if(NULL == m_ReceiveDataList.AddHead(pData)){
		delete pData;
	}
	else{
		bRe = TRUE;
	}

	LeaveCriticalSection(&m_csLock);
	return bRe;
}

void CIPCChannel::Run()
{
	while(m_bThreadRun[PRIMX_IPC_RECEIVE_TH]){

		if(200 == m_nMode){
			if(FALSE == m_bConnect){
				if(0 < m_strIPCKeyName.GetLength() && 0 < m_nMaxSize){
					if(TRUE == SetConnect(m_strIPCKeyName,m_nMaxSize)){
						
					}
				}
			}
		}

		if(NULL != m_pMapMemory[PRIMX_IPC_READ]){
			UINT nCommand = 0;
			UINT nIntegrityID = 0;
			if (::WaitForSingleObject(m_hMutexLock[PRIMX_IPC_READ],500) == WAIT_OBJECT_0) {		
				PRIMX_BUFFER_HEADER* pHeader = (PRIMX_BUFFER_HEADER*)m_pMapMemory[PRIMX_IPC_READ];
				if(pHeader->nIdentity == (0xff ^ pHeader->nCommand ^ pHeader->nBufferSize) && 0 == pHeader->nResultCode){
					if(0 != pHeader->nCommand && 0 < pHeader->nBufferSize){
						char* pBuffer = new char[pHeader->nBufferSize];
						memcpy(pBuffer,((char*)m_pMapMemory[PRIMX_IPC_READ])+sizeof(PRIMX_BUFFER_HEADER),pHeader->nBufferSize);
						if(TRUE == SetReceiveDataSave(pBuffer,pHeader->nBufferSize)){
							pHeader->nResultCode = 100;
						}
						else{
							pHeader->nResultCode = 200;
						}
					}
				}
				::ReleaseMutex(m_hMutexLock[PRIMX_IPC_READ]);
			}
		}		
		Sleep(50);
	}

}

void CIPCChannel::RunForNoti()
{
	while(m_bThreadRun[PRIMX_IPC_NOTI_TH]){

		if(NULL != m_pNotificationFunc){
			char* pData = NULL;
			UINT nDataLen = 0;
			if(TRUE == GetReceiveData(pData,nDataLen)){
				m_pNotificationFunc(pData,nDataLen,m_lpObject);
				delete [] pData;
				pData = NULL;
				nDataLen = 0;
			}
		}
		Sleep(100);
	}
}

int CIPCChannel::SetSendData(char* pData,UINT nDataSize,UINT nTimeOutSec)
{
	if(0 == m_nMode){
		return PRIMX_IPC_ERROR_NOT_INITIALIZED;
	}
	if(200 == m_nMode){
		if(FALSE == m_bConnect){
			if(0 < m_strIPCKeyName.GetLength() && 0 < m_nMaxSize){
				if(FALSE == SetConnect(m_strIPCKeyName,m_nMaxSize)){
					return PRIMX_IPC_ERROR_NOT_CONNECT;
				}
			}
			else{
				return PRIMX_IPC_ERROR_INVALID_KEY;
			}
		}
	}
	
	EnterCriticalSection(&m_csLock);

	int nRe = PRIMX_IPC_ERROR_INVALID_MEMPRY;
	if(NULL != m_pMapMemory[PRIMX_IPC_WRITE]){
		if (::WaitForSingleObject(m_hMutexLock[PRIMX_IPC_WRITE],nTimeOutSec*1000) == WAIT_OBJECT_0) {	
			PRIMX_BUFFER_HEADER* pHeader = (PRIMX_BUFFER_HEADER*)m_pMapMemory[PRIMX_IPC_WRITE];
			pHeader->nCommand = 100;
			pHeader->nBufferSize = nDataSize;
			pHeader->nIdentity = (0xff ^ pHeader->nCommand ^ pHeader->nBufferSize);
			pHeader->nResultCode = 0;

			char* pBufferData = (char*)m_pMapMemory[PRIMX_IPC_WRITE]+sizeof(PRIMX_BUFFER_HEADER);
			memcpy(pBufferData,pData,nDataSize);

			::ReleaseMutex(m_hMutexLock[PRIMX_IPC_WRITE]);

			if(TRUE == GetCommandWait(nTimeOutSec)){
				nRe = PRIMX_IPC_ERROR_NOT_SUCCESS;
			}
		}
		else{
			nRe = PRIMX_IPC_ERROR_TIME_OUT;
		}
	}	
	LeaveCriticalSection(&m_csLock);

	return nRe;
}

BOOL CIPCChannel::GetCommandWait(int nTimeOutSec)
{
	BOOL bRe = FALSE;
	__time32_t tStartTime = _time32(NULL);

	while(TRUE){
		if(NULL != m_pMapMemory[PRIMX_IPC_WRITE]){
			PRIMX_BUFFER_HEADER* pHeader = (PRIMX_BUFFER_HEADER*)m_pMapMemory[PRIMX_IPC_WRITE];
			UINT nCommand = 0;
			BOOL bResult = FALSE;
			if (::WaitForSingleObject(m_hMutexLock[PRIMX_IPC_WRITE],nTimeOutSec*1000) == WAIT_OBJECT_0) {		
				if(0 != pHeader->nResultCode){
					bRe = TRUE;
				}
				::ReleaseMutex(m_hMutexLock[PRIMX_IPC_WRITE]);
			}
			else{
				break;
			}
			if(TRUE == bRe){
				break;
			}
			__time32_t tTime = _time32(NULL);
			if((tTime - tStartTime) > nTimeOutSec){
				break;
			}
		}	
		Sleep(100);
	}

	return bRe;
}

UINT CIPCChannel::GetReceiveDataCount()
{
	UINT nCount = 0;
	EnterCriticalSection(&m_csLock);
	nCount = (UINT)m_ReceiveDataList.GetCount();
	LeaveCriticalSection(&m_csLock);
	return nCount;
}

BOOL CIPCChannel::GetReceiveData(char*& pData,UINT& nDataLen)
{
	BOOL bRe = FALSE;
	pData = NULL;
	nDataLen = 0;

	EnterCriticalSection(&m_csLock);
	
	if(0 < m_ReceiveDataList.GetCount()){
		PRIMX_DATA_INFO* pBufferInfo = m_ReceiveDataList.RemoveTail();
		if(NULL != pBufferInfo){
			nDataLen = pBufferInfo->nBufferSize;
			pData = pBufferInfo->pData;
			pBufferInfo->nBufferSize = 0;
			pBufferInfo->pData = NULL;
			delete pBufferInfo;
			pBufferInfo = NULL;
			bRe = TRUE;
		}
	}
	LeaveCriticalSection(&m_csLock);
	return bRe;
}