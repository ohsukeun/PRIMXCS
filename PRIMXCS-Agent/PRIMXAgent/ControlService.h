/**
@file ControlService.h
@brief 윈도우 서비스 프로그램의 서비스 관리 클래스.
*/
#pragma once
#include "ntservice.h"
#include "CommonModule.h"

class CPRIMXAgentManager;
/// @class CControlService
/// @brief 윈도우 서비스 프로그램의 서비스 관리 클래스
class CControlService : public CNTService , public CCommonModule
{
public:
	/**
	@brief	CControlService 생성자.
	@return	없음.
	*/
	CControlService(const TCHAR* szServiceName/**<  서비스이름 */,const TCHAR* szServiceDisName/**< 서비스 표시 이름 */,const TCHAR* szServiceDesc/**<  서비스 설명 */);

	/**
	@brief	CControlService 소멸자.
	@return	없음.
	*/
	virtual ~CControlService();

	/**
	@brief	서비스 메인에서 들어온 파라미터를 처리하는 함수.
	@return	성공여부
	*/
	BOOL ParseStandardArgs(int argc/**<  파라미터의 갯수 */, TCHAR* argv[]/**<  파라미터 */);

	/**
	@brief	서비스 초기화 함수
	@return	성공여부
	*/
	virtual BOOL OnInit();								

	/**
	@brief	서비스 메인 쓰레드 함수.
	@return	void
	*/
	virtual void Run();									

	/**
	@brief	사용자 저의컨트롤을 처리하는 함수,
	@return	성공여부
	*/
	virtual BOOL OnUserControl(DWORD dwOpcode/**<  사용자 정의 컨트롤 정의값 */);			

	/**
	@brief	서비스 중지 명령을 처리하는 함수.
	@return	void
	*/
	virtual void OnStop();								

	/**
	@brief	서비스 메인 추상화 함수.
	@return	void
	*/
	virtual void ServiceMainEx(DWORD dwArgc/**<  파라미터 갯수 */, LPTSTR* lpszArgv/**<  파라미터 */);

	virtual void OnShutdown();

	/**
	@brief	모든 서비스를 중지하는 함수.
	@return	없음
	*/
	void SetServiceAllStop();

	void SetPRIMXAgentCSRun(BOOL bNow = FALSE);
	void SetPRIMXEncMonitorRun();

	void GetConfig();
	CString GetLocalPath();

private:

	/**
	@brief	서비스 종료시 객체를 정리하는 함수.
	@return	없음
	*/
	void SetClose();

protected:
	
	/**
	@brief	각종 객체 생성.
	@return	성공 여부
	*/
	BOOL SetCreateObject();								

	/**
	@brief	각종 객체 삭제.
	@return	성공 여부
	*/
	void SetDeleteObject();					

	void SetWindowsFirewall();
	

	CPRIMXAgentManager*			m_pPRIMXAgentManager;
	__time64_t					m_tAgentCSRunCheckTime;


};
