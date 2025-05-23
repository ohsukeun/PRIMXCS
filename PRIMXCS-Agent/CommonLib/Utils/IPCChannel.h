#pragma once


#define PRIMX_IPC_ERROR_NOT_SUCCESS				1		// ����
#define PRIMX_IPC_ERROR_NOT_INITIALIZED			-100		// �ʱ�ȭ ���� ����.			
#define PRIMX_IPC_ERROR_NOT_CONNECT				-200		// IPC ������ ������ �ȵ�.
#define PRIMX_IPC_ERROR_INVALID_KEY				-300		// IPC ���� KEY�� ��ȿ���� ����.
#define PRIMX_IPC_ERROR_INVALID_MEMPRY			-400		// IPC �޸𸮰� ��ȿ���� ����.
#define PRIMX_IPC_ERROR_TIME_OUT				-500		// ��� Ÿ�� �ƿ�


// ������ �����͸� �뺸 ���� �Լ��� ����.
typedef VOID (WINAPI *IPC_NOTIFICATION_FUNCTION)(const char* pData,UINT nDataLen,LPVOID lpObject);

class CIPCChannel
{
public:
	CIPCChannel(void);
	~CIPCChannel(void);

	// IPC ���� ����
	BOOL SetServerStart(CString strIPCKeyName,UINT nMaxSize);
	
	// IPC ���� ����
	void SetServerStop();
	
	// IPC������ ������ �õ�
	BOOL SetConnect(CString strIPCKeyName,UINT nMaxSize);

	// ����� IPC ������ ���� ����.
	void SetClose();

	// ������ �����͸� �뺸 ���� �Լ� ����Ʈ�� �����Ѵ�.
	BOOL SetReceiveNotification(IPC_NOTIFICATION_FUNCTION pNotificationFunc,LPVOID lpObject);

	// ������ ���� �Լ�
	int SetSendData(char* pData,UINT nDataSize,UINT nTimeOutSec);
	
	// ������ �������� ���� ��ȯ
	UINT GetReceiveDataCount();

	// ������ �����͸� ��ȯ�ϴ� �Լ�. SetReceiveNotification ���� �� GetReceiveData�Լ��� �ǹ̰� ����.
	// SetReceiveNotification�� �����ϸ� ������ ���� �� ������ �Լ��� �ڵ����� ȣ���Ͽ� ������ �����͸� ������.
	// SetReceiveNotification�� �������� ���� �����켭 �ֱ������� GetReceiveData ȣ���Ͽ� ������ �����͸� �ܴ� �޴� ��츸 ���.
	BOOL GetReceiveData(char*& pData,UINT& nDataLen);

	// ������ ���� �޽��� ��ȯ
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
	HANDLE		m_hThread[PRIMX_IPC_TH_MAX];			//  thread �ڵ� 

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

