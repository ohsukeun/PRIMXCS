#pragma once


#include <windows.h>
#include <crtdbg.h>
#include <netfw.h>
#include <objbase.h>
#include <oleauto.h>


#define FIREWALL_REGISTRY_PATH_MAIN_STANDARD		_T("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile")
#define FIREWALL_REGISTRY_PATH_APP_STANDARD			_T("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile\\AuthorizedApplications\\List")
#define FIREWALL_REGISTRY_PATH_PORT_STANDARD		_T("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile\\GloballyOpenPorts\\List")
#define FIREWALL_REGISTRY_PATH_MAIN_DOMAIN			_T("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\DomainProfile")
#define FIREWALL_REGISTRY_PATH_APP_DOMAIN			_T("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\DomainProfile\\AuthorizedApplications\\List")
#define FIREWALL_REGISTRY_PATH_PORT_DOMAIN			_T("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\DomainProfile\\GloballyOpenPorts\\List")
#define FIREWALL_REGISTRY_PROTOCOL_TCP				1
#define FIREWALL_REGISTRY_PROTOCOL_UDP				2

// @class CWindowsFirewall
// @brief 윈도우 방화벽 제어 클래스.
class CWindowsFirewall
{
public:
	/**
	@brief	CWindowsFirewall 생성자.
	@return	없음.
	*/
	CWindowsFirewall(void);

	/**
	@brief	CWindowsFirewall 소멸자.
	@return	없음.
	*/
	~CWindowsFirewall(void);

	BOOL SetRegWinXPSP2StandardFirewallEnableApp(CString strDesc, CString strPath, BOOL bEnable);
	BOOL SetRegWinXPSP2StandardFirewallEnablePort(CString strDesc, unsigned short nPort, int nProtocol, BOOL bEnable);
	BOOL SetRegWinXPSP2StandardFirewallEnable(BOOL bEnable);
	BOOL SetRegWinXPSP2StandardAllowException(BOOL bEnable);

	BOOL SetRegWinXPSP2DomainFirewallEnableApp(CString strDesc, CString strPath, BOOL bEnable);
	BOOL SetRegWinXPSP2DomainFirewallEnablePort(CString strDesc, unsigned short nPort, int nProtocol, BOOL bEnable);
	BOOL SetRegWinXPSP2DomainFirewallEnable(BOOL bEnable);
	BOOL SetRegWinXPSP2DomainAllowException(BOOL bEnable);

	BOOL IsWinXPSP2StandardFirewall();
	BOOL IsWinXPSP2DomainFirewall();

	
	void SetFirewallRuleAdd(CString strRuleName,CString strRuledesc,UINT nPort,NET_FW_IP_PROTOCOL enProtocol);
	HRESULT WFCOMInitialize(INetFwPolicy2** ppNetFwPolicy2);	

	HRESULT WindowsFirewallPortAdd(
		IN INetFwProfile* fwProfile,
		IN LONG portNumber,
		IN NET_FW_IP_PROTOCOL ipProtocol,
		IN const wchar_t* name
		);
	HRESULT WindowsFirewallInitialize(OUT INetFwProfile** fwProfile);
	void WindowsFirewallCleanup(IN INetFwProfile* fwProfile);
	HRESULT WindowsFirewallPortIsEnabled(
		IN INetFwProfile* fwProfile,
		IN LONG portNumber,
		IN NET_FW_IP_PROTOCOL ipProtocol,
		OUT BOOL* fwPortEnabled
		);

};
