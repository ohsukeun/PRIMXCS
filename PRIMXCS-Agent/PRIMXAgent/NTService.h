/**
@file NTService.h
@brief �������� ���� �⺻ ��� Ŭ����
@date 2010/04/01
@version 0.1
@author hylee
*/
#pragma once

#ifndef _NTSERVICE_H_
#define _NTSERVICE_H_

#define SERVICE_CONTROL_USER					128

/// @class CNTService
/// @brief  �������� ���� �⺻ ��� Ŭ����
class CNTService
{
public:
	/**
	@brief	CNTService ������.
	@return	����.
	*/
	CNTService(const TCHAR* szServiceName/**<  �����̸� */,const TCHAR* szServiceDisName/**< ���� ǥ�� �̸� */,const TCHAR* szServiceDesc/**<  ���� ���� */);

	/**
	@brief	CNTService �Ҹ���.
	@return	����.
	*/
	virtual ~CNTService();

	/**
	@brief ���� ��ġ ���θ� Ȯ���ϴ� �Լ�
	@return ��ġ ����
	*/
    BOOL IsInstalled();

	// ���� ���� ���� ����
	BOOL IsServiceStart();

	// ������ ���� ���ڿ� ����
	CString GetLastErrorString();

	////////////////////////////////////////////////////////////////


	/**
	@brief ���񽺸� ��ġ�ϴ� �Լ�
	@return ��ġ���� ����
	*/
	BOOL Install();
    
	/**
	@brief ���񽺸� �����ϴ� �Լ�
	@return ���� ���� ����
	*/
	BOOL Uninstall();

	/**
	@brief �̺�Ʈ�� �α׸� �����ϴ� �Լ�.
	@return void
	*/
	void LogEvent(WORD wType, DWORD dwID,
                  const TCHAR* pszS1 = NULL,
                  const TCHAR* pszS2 = NULL,
                  const TCHAR* pszS3 = NULL);
	/**
	@brief ���񽺸� �����ϴ� �Լ�.
	@return ���� ����
	*/
    BOOL StartService();
	
	/**
	@brief ������ ���¸� �����ϴ� �Լ�
	@return void
	*/
    void SetStatus(DWORD dwState /**<  �����ڵ� */);
	
	/**
	@brief ���񽺸� �ʱ�ȭ�ϴ� �Լ�
	@return ��������
	*/
    BOOL Initialize();
	
	/**
	@brief ���� ���� ������ �߻�ȭ �Լ�
	@return void
	*/
    virtual void Run();
	
	/**
	@brief ���� ������ �ʱ�ȭ�ϴ� �߻�ȭ �Լ�
	@return ��������
	*/
	virtual BOOL OnInit();
	
	/**
	@brief ���� ���� �߻�ȭ �Լ�
	@return void
	*/
    virtual void OnStop();
	
	/**
	@brief ���� Interrogate �߻�ȭ �Լ�
	@return void
	*/
    virtual void OnInterrogate();
	
	/**
	@brief ���� ������� �߻�ȭ �Լ�
	@return void
	*/
    virtual void OnPause();
	
	/**
	@brief ���� �ٽý��� �߻�ȭ �Լ�
	@return void
	*/
    virtual void OnContinue();
	
	/**
	@brief ���� Shutdown �߻�ȭ �Լ�
	@return void
	*/
    virtual void OnShutdown();
	
	/**
	@brief ���� UserControl �߻�ȭ �Լ�
	@return ��������
	*/
    virtual BOOL OnUserControl(DWORD dwOpcode/**<  ����� ���� ��Ʈ�Ѱ� */);
	
	/**
	@brief Copyright ��� �Լ�.
	@return void
	*/
 	virtual void PutCopyright();
	
	/**
	@brief ���� ���� �Լ��� ���� �Ķ���� ó�� �Լ�.
	@return ��������
	*/
	virtual BOOL ParseStandardArgs(int argc, TCHAR* argv[]);
	
	/**
	@brief ���� ���� �߻�ȭ �Լ�
	@return void
	*/
    virtual void ServiceMainEx(DWORD dwArgc, LPTSTR* lpszArgv);

	virtual BOOL GetPatchInfoCollectorStart();

    // static member functions
    static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    static void WINAPI Handler(DWORD dwOpcode);

    // data members
    BOOL					m_bIsRunning;	/**<  ���� ���� ������ ���� �÷��װ� */
	SERVICE_STATUS			m_Status;		


    // static data
    static CNTService*		m_pThis; // nasty hack to get object ptr

private:
	HANDLE					m_hEventSource;

protected:

	SERVICE_STATUS_HANDLE	m_hServiceStatus;

	TCHAR					m_szServiceName[256];			/**<  ���� �̸� */
	TCHAR					m_szServiceDisName[256];		/**<  ���� ǥ���̸� */
	TCHAR					m_szServiceDesc[1024];			/**<  ���� ���� */
	CString					m_strErrorString;				// ������ ���� �޽���.

};

#endif // _NTSERVICE_H_
