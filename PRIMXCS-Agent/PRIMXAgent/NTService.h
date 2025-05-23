/**
@file NTService.h
@brief 윈도우즈 서비스 기본 모듈 클래스
@date 2010/04/01
@version 0.1
@author hylee
*/
#pragma once

#ifndef _NTSERVICE_H_
#define _NTSERVICE_H_

#define SERVICE_CONTROL_USER					128

/// @class CNTService
/// @brief  윈도우즈 서비스 기본 모듈 클래스
class CNTService
{
public:
	/**
	@brief	CNTService 생성자.
	@return	없음.
	*/
	CNTService(const TCHAR* szServiceName/**<  서비스이름 */,const TCHAR* szServiceDisName/**< 서비스 표시 이름 */,const TCHAR* szServiceDesc/**<  서비스 설명 */);

	/**
	@brief	CNTService 소멸자.
	@return	없음.
	*/
	virtual ~CNTService();

	/**
	@brief 서비스 설치 여부를 확인하는 함수
	@return 설치 여부
	*/
    BOOL IsInstalled();

	// 서비스 구동 여부 리턴
	BOOL IsServiceStart();

	// 마지막 오류 문자열 리턴
	CString GetLastErrorString();

	////////////////////////////////////////////////////////////////


	/**
	@brief 서비스를 설치하는 함수
	@return 설치성공 여부
	*/
	BOOL Install();
    
	/**
	@brief 서비스를 삭제하는 함수
	@return 삭제 성공 여부
	*/
	BOOL Uninstall();

	/**
	@brief 이벤트뷰어에 로그를 저장하는 함수.
	@return void
	*/
	void LogEvent(WORD wType, DWORD dwID,
                  const TCHAR* pszS1 = NULL,
                  const TCHAR* pszS2 = NULL,
                  const TCHAR* pszS3 = NULL);
	/**
	@brief 서비스를 시작하는 함수.
	@return 성공 여부
	*/
    BOOL StartService();
	
	/**
	@brief 서비스의 상태를 설정하는 함수
	@return void
	*/
    void SetStatus(DWORD dwState /**<  상태코드 */);
	
	/**
	@brief 서비스를 초기화하는 함수
	@return 성공여부
	*/
    BOOL Initialize();
	
	/**
	@brief 서비스 메인 쓰레드 추상화 함수
	@return void
	*/
    virtual void Run();
	
	/**
	@brief 서비스 구동시 초기화하는 추상화 함수
	@return 성공여부
	*/
	virtual BOOL OnInit();
	
	/**
	@brief 서비스 중지 추상화 함수
	@return void
	*/
    virtual void OnStop();
	
	/**
	@brief 서비스 Interrogate 추상화 함수
	@return void
	*/
    virtual void OnInterrogate();
	
	/**
	@brief 서비스 잠시중지 추상화 함수
	@return void
	*/
    virtual void OnPause();
	
	/**
	@brief 서비스 다시시작 추상화 함수
	@return void
	*/
    virtual void OnContinue();
	
	/**
	@brief 서비스 Shutdown 추상화 함수
	@return void
	*/
    virtual void OnShutdown();
	
	/**
	@brief 서비스 UserControl 추상화 함수
	@return 성공여부
	*/
    virtual BOOL OnUserControl(DWORD dwOpcode/**<  사용자 정의 컨트롤값 */);
	
	/**
	@brief Copyright 출력 함수.
	@return void
	*/
 	virtual void PutCopyright();
	
	/**
	@brief 서비스 메인 함수로 들어온 파라미터 처리 함수.
	@return 성공여부
	*/
	virtual BOOL ParseStandardArgs(int argc, TCHAR* argv[]);
	
	/**
	@brief 서비스 메인 추상화 함수
	@return void
	*/
    virtual void ServiceMainEx(DWORD dwArgc, LPTSTR* lpszArgv);

	virtual BOOL GetPatchInfoCollectorStart();

    // static member functions
    static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    static void WINAPI Handler(DWORD dwOpcode);

    // data members
    BOOL					m_bIsRunning;	/**<  서비스 메인 쓰레드 구동 플래그값 */
	SERVICE_STATUS			m_Status;		


    // static data
    static CNTService*		m_pThis; // nasty hack to get object ptr

private:
	HANDLE					m_hEventSource;

protected:

	SERVICE_STATUS_HANDLE	m_hServiceStatus;

	TCHAR					m_szServiceName[256];			/**<  서비스 이름 */
	TCHAR					m_szServiceDisName[256];		/**<  서비스 표시이름 */
	TCHAR					m_szServiceDesc[1024];			/**<  서비스 설졍 */
	CString					m_strErrorString;				// 마지막 오류 메시지.

};

#endif // _NTSERVICE_H_
