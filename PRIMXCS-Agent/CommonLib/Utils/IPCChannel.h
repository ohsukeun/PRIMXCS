#pragma once


#define PRIMX_IPC_ERROR_NOT_SUCCESS				1		// 성공
#define PRIMX_IPC_ERROR_NOT_INITIALIZED			-100		// 초기화 되지 않음.			
#define PRIMX_IPC_ERROR_NOT_CONNECT				-200		// IPC 서버와 연결이 안됨.
#define PRIMX_IPC_ERROR_INVALID_KEY				-300		// IPC 연결 KEY가 유효하지 않음.
#define PRIMX_IPC_ERROR_INVALID_MEMPRY			-400		// IPC 메모리가 유효하지 않음.
#define PRIMX_IPC_ERROR_TIME_OUT				-500		// 명령 타임 아웃


// 수신한 데이터를 통보 받을 함수의 원형.
typedef VOID (WINAPI *IPC_NOTIFICATION_FUNCTION)(const char* pData,UINT nDataLen,LPVOID lpObject);

class CIPCChannel
{
public:
	CIPCChannel(void);
	~CIPCChannel(void);

	// IPC 서버 시작
	BOOL SetServerStart(CString strIPCKeyName,UINT nMaxSize);
	
	// IPC 서버 중지
	void SetServerStop();
	
	// IPC서버에 연결을 시도
	BOOL SetConnect(CString strIPCKeyName,UINT nMaxSize);

	// 연결된 IPC 서버와 연결 종료.
	void SetClose();

	// 수신한 데이터를 통보 받을 함수 포인트를 설정한다.
	BOOL SetReceiveNotification(IPC_NOTIFICATION_FUNCTION pNotificationFunc,LPVOID lpObject);

	// 데이터 전송 함수
	int SetSendData(char* pData,UINT nDataSize,UINT nTimeOutSec);
	
	// 수신한 데이터의 갯수 반환
	UINT GetReceiveDataCount();

	// 수신한 데이터를 반환하는 함수. SetReceiveNotification 설정 시 GetReceiveData함수는 의미가 없음.
	// SetReceiveNotification를 설정하면 데이터 수신 시 설정된 함수를 자동으로 호출하여 수신한 데이터를 전달함.
	// SetReceiveNotification를 설정하지 않은 상태헤서 주기적으로 GetReceiveData 호출하여 수신한 데이터를 잔달 받는 경우만 사용.
	BOOL GetReceiveData(char*& pData,UINT& nDataLen);

	// 마지막 오류 메시지 반환
	CString GetLastErrorString();


private:

	enum PRIMX_IPC_IO_TYPE
	{
		PRIMX_IPC_READ = 0,
		PRIMX_IPC_WRITE,
		PRIMX_IPC_MAX
	};
	enum PRIMX_IPC_THREAD_TYPE
	{
		PRIMX_IPC_RECEIVE_TH = 0,
		PRIMX_IPC_NOTI_TH,
		PRIMX_IPC_TH_MAX
	};
	struct PRIMX_BUFFER_HEADER{
		UINT    nIdentity;
		UINT	nCommand;
		UINT	nBufferSize;
		UINT	nResultCode;
	};
	struct PRIMX_DATA_INFO{
		UINT	nBufferSize;
		char*	pData;

		PRIMX_DATA_INFO(){
			nBufferSize = 0;
			pData = NULL;
		};
		~PRIMX_DATA_INFO(){
			nBufferSize = 0;
			if(NULL != pData){
				delete [] pData;
				pData = NULL;
			}
		};
	};

	BOOL		m_bThreadRun[PRIMX_IPC_TH_MAX];
	DWORD		m_dwThreadID[PRIMX_IPC_TH_MAX];		//  thread ID 
	HANDLE		m_hThread[PRIMX_IPC_TH_MAX];			//  thread 핸들 

	BOOL SetReceiveStart();
	BOOL SetReceiveTermination(DWORD dwMilliSeconds = INFINITE);
	CString GetErrorToString();
	CString GetLocalPath();
	UINT32 GetGMTime();
	BOOL GetCommandWait(int nTimeOutSec);
	BOOL SetReceiveDataSave(char* pBuffer,UINT nSize);
	BOOL SetSecurityAttributes(SECURITY_ATTRIBUTES& sa);
	void SetRemoveAll();
	void SetMapFileDelete(CString strIPCKeyName);
	BOOL SetCreateDirectoryEx(CString strDirectory);
	CString GetMapFileCreatePath();

	void Run();
	void RunForNoti();

	CString						m_strLastErrorString;
	LPVOID						m_pMapMemory[PRIMX_IPC_MAX]; // 0 : Read Buffer / 1 : Write Buffer
	HANDLE						m_hFileMapping;
	HANDLE						m_hMutexLock[PRIMX_IPC_MAX];
	HANDLE						m_hFile;
	CRITICAL_SECTION			m_csLock;
	CString						m_strMMFFilePath;
	IPC_NOTIFICATION_FUNCTION	m_pNotificationFunc;
	LPVOID						m_lpObject;
	CString						m_strIPCKeyName;
	UINT						m_nMaxSize;

	BOOL						m_bConnect;
	UINT						m_nMode;

	CList<PRIMX_DATA_INFO*>		m_ReceiveDataList;
	
	static unsigned __stdcall __THREAD_ENTRY_POINT(void*);
	static unsigned __stdcall __THREAD_NOTI_POINT(void*);
	
};

