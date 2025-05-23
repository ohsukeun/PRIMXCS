/**
@file ControlService.h
@brief ������ ���� ���α׷��� ���� ���� Ŭ����.
*/
#pragma once
#include "ntservice.h"
#include "CommonModule.h"

class CPRIMXAgentManager;
/// @class CControlService
/// @brief ������ ���� ���α׷��� ���� ���� Ŭ����
class CControlService : public CNTService , public CCommonModule
{
public:
	/**
	@brief	CControlService ������.
	@return	����.
	*/
	CControlService(const TCHAR* szServiceName/**<  �����̸� */,const TCHAR* szServiceDisName/**< ���� ǥ�� �̸� */,const TCHAR* szServiceDesc/**<  ���� ���� */);

	/**
	@brief	CControlService �Ҹ���.
	@return	����.
	*/
	virtual ~CControlService();

	/**
	@brief	���� ���ο��� ���� �Ķ���͸� ó���ϴ� �Լ�.
	@return	��������
	*/
	BOOL ParseStandardArgs(int argc/**<  �Ķ������ ���� */, TCHAR* argv[]/**<  �Ķ���� */);

	/**
	@brief	���� �ʱ�ȭ �Լ�
	@return	��������
	*/
	virtual BOOL OnInit();								

	/**
	@brief	���� ���� ������ �Լ�.
	@return	void
	*/
	virtual void Run();									

	/**
	@brief	����� ������Ʈ���� ó���ϴ� �Լ�,
	@return	��������
	*/
	virtual BOOL OnUserControl(DWORD dwOpcode/**<  ����� ���� ��Ʈ�� ���ǰ� */);			

	/**
	@brief	���� ���� ����� ó���ϴ� �Լ�.
	@return	void
	*/
	virtual void OnStop();								

	/**
	@brief	���� ���� �߻�ȭ �Լ�.
	@return	void
	*/
	virtual void ServiceMainEx(DWORD dwArgc/**<  �Ķ���� ���� */, LPTSTR* lpszArgv/**<  �Ķ���� */);

	virtual void OnShutdown();

	/**
	@brief	��� ���񽺸� �����ϴ� �Լ�.
	@return	����
	*/
	void SetServiceAllStop();

	void SetPRIMXAgentCSRun(BOOL bNow = FALSE);
	void SetPRIMXEncMonitorRun();

	void GetConfig();
	CString GetLocalPath();

private:

	/**
	@brief	���� ����� ��ü�� �����ϴ� �Լ�.
	@return	����
	*/
	void SetClose();

protected:
	
	/**
	@brief	���� ��ü ����.
	@return	���� ����
	*/
	BOOL SetCreateObject();								

	/**
	@brief	���� ��ü ����.
	@return	���� ����
	*/
	void SetDeleteObject();					

	void SetWindowsFirewall();
	

	CPRIMXAgentManager*			m_pPRIMXAgentManager;
	__time64_t					m_tAgentCSRunCheckTime;


};
