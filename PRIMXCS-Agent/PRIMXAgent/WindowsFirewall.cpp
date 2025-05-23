#include "stdafx.h"
#include "WindowsFirewall.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWindowsFirewall::CWindowsFirewall(void)
{
}

CWindowsFirewall::~CWindowsFirewall(void)
{
}

BOOL CWindowsFirewall::SetRegWinXPSP2StandardFirewallEnable(BOOL bEnable)
{
	HKEY hKey;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwData = 1;
	if(!bEnable) dwData = 0;
	else  dwData = 1;


	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, FIREWALL_REGISTRY_PATH_MAIN_STANDARD, 0, KEY_ALL_ACCESS, &hKey))
	{
		return FALSE;
	}

	if(ERROR_SUCCESS != RegSetValueEx(hKey, _T("EnableFirewall"), 0, REG_DWORD, (PBYTE)&dwData, dwSize))
	{
		RegCloseKey(hKey);
		return FALSE;
	}

	RegCloseKey(hKey);
	return TRUE;

}

BOOL CWindowsFirewall::SetRegWinXPSP2StandardFirewallEnablePort(CString strDesc, unsigned short nPort, int nProtocol, BOOL bEnable)
{
	if(!IsWinXPSP2StandardFirewall())
	{
		return FALSE;
	}

	HKEY hKey;
	DWORD dwSize;
	CString strName;
	CString strData;

	switch(nProtocol)
	{
	case FIREWALL_REGISTRY_PROTOCOL_TCP:
		strName.Format(_T("%d:TCP"), nPort);
		if(bEnable)	strData.Format(_T("%d:TCP:*:Enabled:%s"), nPort, strDesc);
		else strData.Format(_T("%d:TCP:*:Disabled:%s"), nPort, strDesc);
		break;

	case FIREWALL_REGISTRY_PROTOCOL_UDP:
		strName.Format(_T("%d:UDP"), nPort);
		if(bEnable)	strData.Format(_T("%d:UDP:*:Enabled:%s"), nPort, strDesc);
		else strData.Format(_T("%d:UDP:*:Disabled:%s"), nPort, strDesc);
		break;

	default:
		return FALSE;
	}

	if(ERROR_SUCCESS != RegCreateKey(HKEY_LOCAL_MACHINE, FIREWALL_REGISTRY_PATH_PORT_STANDARD, &hKey))
	{
		return FALSE;
	}

	dwSize = (strData.GetLength() + 1) * sizeof(TCHAR);
	if(ERROR_SUCCESS != RegSetValueEx(hKey, (LPCTSTR)strName.GetBuffer(0), 0, REG_SZ, (PBYTE)strData.GetBuffer(0), dwSize))
	{
		RegCloseKey(hKey);
		strName.ReleaseBuffer();
		strData.ReleaseBuffer();
		return FALSE;
	}

	RegCloseKey(hKey);
	strName.ReleaseBuffer();
	strData.ReleaseBuffer();
	return TRUE;

}

BOOL CWindowsFirewall::SetRegWinXPSP2StandardAllowException(BOOL bEnable)
{
	HKEY hKey;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwData = 1;
	if(bEnable) dwData = 0;
	else  dwData = 1;


	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, FIREWALL_REGISTRY_PATH_MAIN_STANDARD, 0, KEY_ALL_ACCESS, &hKey))
	{
		return FALSE;
	}

	if(ERROR_SUCCESS != RegSetValueEx(hKey, _T("DoNotAllowExceptions"), 0, REG_DWORD, (PBYTE)&dwData, dwSize))
	{
		RegCloseKey(hKey);
		return FALSE;
	}

	RegCloseKey(hKey);
	return TRUE;
}

BOOL CWindowsFirewall::SetRegWinXPSP2DomainFirewallEnable(BOOL bEnable)
{
	HKEY hKey;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwData = 1;
	if(!bEnable) dwData = 0;
	else  dwData = 1;


	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, FIREWALL_REGISTRY_PATH_MAIN_DOMAIN, 0, KEY_ALL_ACCESS, &hKey))
	{
		return FALSE;
	}

	if(ERROR_SUCCESS != RegSetValueEx(hKey, _T("EnableFirewall"), 0, REG_DWORD, (PBYTE)&dwData, dwSize))
	{
		RegCloseKey(hKey);
		return FALSE;
	}

	RegCloseKey(hKey);
	return TRUE;
}

BOOL CWindowsFirewall::SetRegWinXPSP2DomainFirewallEnablePort(CString strDesc, unsigned short nPort, int nProtocol, BOOL bEnable)
{
	if(!IsWinXPSP2DomainFirewall())
	{
		return FALSE;
	}

	HKEY hKey;
	DWORD dwSize;
	CString strName;
	CString strData;

	switch(nProtocol)
	{
	case FIREWALL_REGISTRY_PROTOCOL_TCP:
		strName.Format(_T("%d:TCP"), nPort);
		if(bEnable)	strData.Format(_T("%d:TCP:*:Enabled:%s"), nPort, strDesc);
		else strData.Format(_T("%d:TCP:*:Disabled:%s"), nPort, strDesc);
		break;

	case FIREWALL_REGISTRY_PROTOCOL_UDP:
		strName.Format(_T("%d:UDP"), nPort);
		if(bEnable)	strData.Format(_T("%d:UDP:*:Enabled:%s"), nPort, strDesc);
		else strData.Format(_T("%d:UDP:*:Disabled:%s"), nPort, strDesc);
		break;

	default:
		return FALSE;
	}

	if(ERROR_SUCCESS != RegCreateKey(HKEY_LOCAL_MACHINE, FIREWALL_REGISTRY_PATH_PORT_DOMAIN, &hKey))
	{
		return FALSE;
	}

	dwSize = (strData.GetLength() + 1) * sizeof(TCHAR);
	if(ERROR_SUCCESS != RegSetValueEx(hKey, (LPCTSTR)strName.GetBuffer(0), 0, REG_SZ, (PBYTE)strData.GetBuffer(0), dwSize))
	{
		RegCloseKey(hKey);
		strName.ReleaseBuffer();
		strData.ReleaseBuffer();
		return FALSE;
	}

	RegCloseKey(hKey);
	strName.ReleaseBuffer();
	strData.ReleaseBuffer();
	return TRUE;
}

BOOL CWindowsFirewall::SetRegWinXPSP2DomainAllowException(BOOL bEnable)
{
	HKEY hKey;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwData = 1;
	if(bEnable) dwData = 0;
	else  dwData = 1;


	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, FIREWALL_REGISTRY_PATH_MAIN_DOMAIN, 0, KEY_ALL_ACCESS, &hKey))
	{
		return FALSE;
	}

	if(ERROR_SUCCESS != RegSetValueEx(hKey, _T("DoNotAllowExceptions"), 0, REG_DWORD, (PBYTE)&dwData, dwSize))
	{
		RegCloseKey(hKey);
		return FALSE;
	}

	RegCloseKey(hKey);
	return TRUE;
}

BOOL CWindowsFirewall::IsWinXPSP2DomainFirewall()
{
	HKEY hKey;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwData = 1;

	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, FIREWALL_REGISTRY_PATH_MAIN_DOMAIN, 0, KEY_ALL_ACCESS, &hKey))
	{
		return FALSE;
	}

	RegCloseKey(hKey);
	return TRUE;

}

BOOL CWindowsFirewall::IsWinXPSP2StandardFirewall()
{
	HKEY hKey;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwData = 1;

	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, FIREWALL_REGISTRY_PATH_MAIN_STANDARD, 0, KEY_ALL_ACCESS, &hKey))
	{
		return FALSE;
	}

	RegCloseKey(hKey);
	return TRUE;
}

BOOL CWindowsFirewall::SetRegWinXPSP2StandardFirewallEnableApp(CString strDesc, CString strPath, BOOL bEnable)
{
	if(!IsWinXPSP2DomainFirewall())
	{
		return FALSE;
	}

	HKEY hKey;
	DWORD dwSize;
	CString strName;
	CString strData;

	strName.Format(_T("%s"), strPath);
	if(bEnable)	strData.Format(_T("%s:*:Enabled:%s"), strPath, strDesc);
	else strData.Format(_T("%s:*:Disabled:%s"), strPath, strDesc);

	if(ERROR_SUCCESS != RegCreateKey(HKEY_LOCAL_MACHINE, FIREWALL_REGISTRY_PATH_APP_STANDARD, &hKey))
	{
		return FALSE;
	}

	dwSize = (strData.GetLength() + 1) * sizeof(TCHAR);
	if(ERROR_SUCCESS != RegSetValueEx(hKey, (LPCTSTR)strName.GetBuffer(0), 0, REG_SZ, (PBYTE)strData.GetBuffer(0), dwSize))
	{
		RegCloseKey(hKey);
		strName.ReleaseBuffer();
		strData.ReleaseBuffer();
		return FALSE;
	}

	RegCloseKey(hKey);
	strName.ReleaseBuffer();
	strData.ReleaseBuffer();
	return TRUE;
}

BOOL CWindowsFirewall::SetRegWinXPSP2DomainFirewallEnableApp(CString strDesc, CString strPath, BOOL bEnable)
{
	if(!IsWinXPSP2DomainFirewall())
	{
		return FALSE;
	}

	HKEY hKey;
	DWORD dwSize;
	CString strName;
	CString strData;

	strName.Format(_T("%s"), strPath);
	if(bEnable)	strData.Format(_T("%s:*:Enabled:%s"), strPath, strDesc);
	else strData.Format(_T("%s:*:Disabled:%s"), strPath, strDesc);

	if(ERROR_SUCCESS != RegCreateKey(HKEY_LOCAL_MACHINE, FIREWALL_REGISTRY_PATH_APP_DOMAIN, &hKey))
	{
		return FALSE;
	}

	dwSize = (strData.GetLength() + 1) * sizeof(TCHAR);
	if(ERROR_SUCCESS != RegSetValueEx(hKey, (LPCTSTR)strName.GetBuffer(0), 0, REG_SZ, (PBYTE)strData.GetBuffer(0), dwSize))
	{
		RegCloseKey(hKey);
		strName.ReleaseBuffer();
		strData.ReleaseBuffer();
		return FALSE;
	}

	RegCloseKey(hKey);
	strName.ReleaseBuffer();
	strData.ReleaseBuffer();
	return TRUE;
}

void CWindowsFirewall::SetFirewallRuleAdd(CString strRuleName,CString strRuledesc,UINT nPort,NET_FW_IP_PROTOCOL enProtocol)
{
	HRESULT hrComInit = S_FALSE;
	HRESULT hr = S_FALSE;

	INetFwRule *pFwRule = NULL;
	INetFwRules *pFwRules = NULL;
	INetFwPolicy2 *pNetFwPolicy2 = NULL;

	long CurrentProfilesBitMask = 0;
	CString strErr = _T("");

	BSTR bstrRuleName = NULL;	
	BSTR bstrRuleDescription = NULL;	
	BSTR bstrRuleApplication = NULL;
	BSTR bstrRuleLPorts = NULL;

	CString strPort;
	strPort.Format(_T("%lu"), nPort);

	TCHAR strFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL,strFilePath,MAX_PATH+1);
	
	bstrRuleName = SysAllocString(strRuleName);
	bstrRuleDescription = SysAllocString(strRuledesc);	
	bstrRuleApplication = SysAllocString(strFilePath);	
	bstrRuleLPorts = SysAllocString(strPort);

	// Initialize COM.
	hrComInit = CoInitializeEx(0, /*COINIT_APARTMENTTHREADED*/ COINIT_MULTITHREADED);
	if (hrComInit != RPC_E_CHANGED_MODE)
	{
		if (FAILED(hrComInit))
		{
			goto _function_finish;
		}
	}		


	// INetFwPolicy2 검색 
	hr = WFCOMInitialize(&pNetFwPolicy2);
	if (FAILED(hr))
	{
		goto _function_finish;
	}

	// INetFwRules 검색
	hr = pNetFwPolicy2->get_Rules(&pFwRules);
	if (FAILED(hr))
	{
		goto _function_finish;
	}

	//이미 존재 하면 삭제 후 추가 한다.(중복생성 방지)
	hr = pFwRules->Remove(bstrRuleName);
	if (FAILED(hr))
	{
		goto _function_finish;
	}

	// 현재 프로필 비트 마스크를 검색
	hr = pNetFwPolicy2->get_CurrentProfileTypes(&CurrentProfilesBitMask);
	if (FAILED(hr))
	{
		goto _function_finish;
	}	

	if ((CurrentProfilesBitMask & NET_FW_PROFILE2_ALL)
		&& (CurrentProfilesBitMask != NET_FW_PROFILE2_ALL))
	{
		CurrentProfilesBitMask |= NET_FW_PROFILE2_ALL;
	}

	// 새로운 방화벽 규칙 개체를 만듭니다.
	hr = CoCreateInstance(
		__uuidof(NetFwRule),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(INetFwRule),
		(void**)&pFwRule);

	if (FAILED(hr))
	{
		goto _function_finish;
	}	

	// 방화벽 규칙 개체를 채 웁니다
	pFwRule->put_Name(bstrRuleName);
	pFwRule->put_Description(bstrRuleDescription);
	pFwRule->put_ApplicationName(bstrRuleApplication);
	//pFwRule->put_ServiceName(bstrRuleService);
	pFwRule->put_Protocol(enProtocol);
	pFwRule->put_LocalPorts(bstrRuleLPorts);
	pFwRule->put_Profiles(NET_FW_PROFILE2_ALL);
	pFwRule->put_Action(NET_FW_ACTION_ALLOW);
	pFwRule->put_Enabled(VARIANT_TRUE);

	// 방화벽 규칙을 추가
	hr = pFwRules->Add(pFwRule);
	if (FAILED(hr))
	{
		goto _function_finish;
	}	


_function_finish:

	// Free BSTR's
	if (lstrlen(bstrRuleName) > 0)
	{
		SysFreeString(bstrRuleName);
	}
	if (lstrlen(bstrRuleDescription) > 0)
	{
		SysFreeString(bstrRuleDescription);
	}
	if (lstrlen(bstrRuleApplication) > 0)
	{
		SysFreeString(bstrRuleApplication);
	}
	if (lstrlen(bstrRuleLPorts) > 0)
	{
		SysFreeString(bstrRuleLPorts);
	}	
	//SysFreeString(bstrRuleService);

	if (pNetFwPolicy2 != NULL)
	{
		pNetFwPolicy2->Release();
		pNetFwPolicy2 = NULL;
	}
	if (pFwRules != NULL)
	{
		pFwRules->Release();
		pFwRules = NULL;
	}
	if (pFwRule != NULL)
	{
		pFwRule->Release();
		pFwRule = NULL;
	}		

	CoUninitialize();
	return;
}

// Instantiate INetFwPolicy2
HRESULT CWindowsFirewall::WFCOMInitialize(INetFwPolicy2** ppNetFwPolicy2)
{
	HRESULT hr = S_OK;

	hr = CoCreateInstance(
		__uuidof(NetFwPolicy2), 
		NULL, 
		CLSCTX_INPROC_SERVER, 
		__uuidof(INetFwPolicy2), 
		(void**)ppNetFwPolicy2);

	if (FAILED(hr))
	{
		return hr;   
	}
	return hr; 
}
HRESULT CWindowsFirewall::WindowsFirewallPortIsEnabled(
	IN INetFwProfile* fwProfile,
	IN LONG portNumber,
	IN NET_FW_IP_PROTOCOL ipProtocol,
	OUT BOOL* fwPortEnabled
	)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL fwEnabled;
	INetFwOpenPort* fwOpenPort = NULL;
	INetFwOpenPorts* fwOpenPorts = NULL;

	if(fwProfile == NULL) return -1;
	if(fwPortEnabled == NULL) return -1;

	*fwPortEnabled = FALSE;

	// Retrieve the globally open ports collection.
	hr = fwProfile->get_GloballyOpenPorts(&fwOpenPorts);
	if (FAILED(hr))
	{
		// Release the globally open port.
		if (fwOpenPort != NULL)
		{
			fwOpenPort->Release();
		}

		// Release the globally open ports collection.
		if (fwOpenPorts != NULL)
		{
			fwOpenPorts->Release();
		}

	}

	// Attempt to retrieve the globally open port.
	hr = fwOpenPorts->Item(portNumber, ipProtocol, &fwOpenPort);
	if (SUCCEEDED(hr))
	{
		// Find out if the globally open port is enabled.
		hr = fwOpenPort->get_Enabled(&fwEnabled);
		if (FAILED(hr))
		{
			// Release the globally open port.
			if (fwOpenPort != NULL)
			{
				fwOpenPort->Release();
			}

			// Release the globally open ports collection.
			if (fwOpenPorts != NULL)
			{
				fwOpenPorts->Release();
			}

		}

		if (fwEnabled != VARIANT_FALSE)
		{
			// The globally open port is enabled.
			*fwPortEnabled = TRUE;
		}
		else
		{
		}
	}
	else
	{
		// The globally open port was not in the collection.
		hr = S_OK;
	}

	// Release the globally open port.
	if (fwOpenPort != NULL)
	{
		fwOpenPort->Release();
	}

	// Release the globally open ports collection.
	if (fwOpenPorts != NULL)
	{
		fwOpenPorts->Release();
	}

	return hr;
}

HRESULT CWindowsFirewall::WindowsFirewallPortAdd(
	IN INetFwProfile* fwProfile,
	IN LONG portNumber,
	IN NET_FW_IP_PROTOCOL ipProtocol,
	IN const wchar_t* name
	)
{
	HRESULT hr = S_OK;
	BOOL fwPortEnabled;
	BSTR fwBstrName = NULL;
	INetFwOpenPort* fwOpenPort = NULL;
	INetFwOpenPorts* fwOpenPorts = NULL;

	if(fwProfile == NULL) return -1;
	if(name == NULL) return -1;

	// First check to see if the port is already added.
	hr = WindowsFirewallPortIsEnabled(
		fwProfile,
		portNumber,
		ipProtocol,
		&fwPortEnabled
		);
	if (FAILED(hr))
	{
		// Free the BSTR.
		SysFreeString(fwBstrName);

		// Release the open port instance.
		if (fwOpenPort != NULL)
		{
			fwOpenPort->Release();
		}

		// Release the globally open ports collection.
		if (fwOpenPorts != NULL)
		{
			fwOpenPorts->Release();
		}

		return hr;
	}

	// Only add the port if it isn't already added.
	if (!fwPortEnabled)
	{
		// Retrieve the collection of globally open ports.
		hr = fwProfile->get_GloballyOpenPorts(&fwOpenPorts);
		if (FAILED(hr))
		{
			// Free the BSTR.
			SysFreeString(fwBstrName);

			// Release the open port instance.
			if (fwOpenPort != NULL)
			{
				fwOpenPort->Release();
			}

			// Release the globally open ports collection.
			if (fwOpenPorts != NULL)
			{
				fwOpenPorts->Release();
			}

			return hr;
		}
		
		// Create an instance of an open port.
		hr = CoCreateInstance(
			__uuidof(NetFwOpenPort),
			NULL,
			CLSCTX_INPROC_SERVER,
			__uuidof(INetFwOpenPort),
			(void**)&fwOpenPort
			);
		if (FAILED(hr))
		{
			// Free the BSTR.
			SysFreeString(fwBstrName);

			// Release the open port instance.
			if (fwOpenPort != NULL)
			{
				fwOpenPort->Release();
			}

			// Release the globally open ports collection.
			if (fwOpenPorts != NULL)
			{
				fwOpenPorts->Release();
			}

			return hr;
		}

		// Set the port number.
		hr = fwOpenPort->put_Port(portNumber);
		if (FAILED(hr))
		{
			// Free the BSTR.
			SysFreeString(fwBstrName);

			// Release the open port instance.
			if (fwOpenPort != NULL)
			{
				fwOpenPort->Release();
			}

			// Release the globally open ports collection.
			if (fwOpenPorts != NULL)
			{
				fwOpenPorts->Release();
			}

			return hr;
		}

		// Set the IP protocol.
		hr = fwOpenPort->put_Protocol(ipProtocol);
		if (FAILED(hr))
		{
			// Free the BSTR.
			SysFreeString(fwBstrName);

			// Release the open port instance.
			if (fwOpenPort != NULL)
			{
				fwOpenPort->Release();
			}

			// Release the globally open ports collection.
			if (fwOpenPorts != NULL)
			{
				fwOpenPorts->Release();
			}

			return hr;
		}

		// Allocate a BSTR for the friendly name of the port.
		fwBstrName = SysAllocString(name);
		if (SysStringLen(fwBstrName) == 0)
		{
			hr = E_OUTOFMEMORY;
			// Free the BSTR.
			SysFreeString(fwBstrName);

			// Release the open port instance.
			if (fwOpenPort != NULL)
			{
				fwOpenPort->Release();
			}

			// Release the globally open ports collection.
			if (fwOpenPorts != NULL)
			{
				fwOpenPorts->Release();
			}

			return hr;
		}

		// Set the friendly name of the port.
		hr = fwOpenPort->put_Name(fwBstrName);
		if (FAILED(hr))
		{
			// Free the BSTR.
			SysFreeString(fwBstrName);

			// Release the open port instance.
			if (fwOpenPort != NULL)
			{
				fwOpenPort->Release();
			}

			// Release the globally open ports collection.
			if (fwOpenPorts != NULL)
			{
				fwOpenPorts->Release();
			}

			return hr;
		}

		// Opens the port and adds it to the collection.
		hr = fwOpenPorts->Add(fwOpenPort);
		if (FAILED(hr))
		{
			// Free the BSTR.
			SysFreeString(fwBstrName);

			// Release the open port instance.
			if (fwOpenPort != NULL)
			{
				fwOpenPort->Release();
			}

			// Release the globally open ports collection.
			if (fwOpenPorts != NULL)
			{
				fwOpenPorts->Release();
			}

			return hr;
		}
	}

	// Free the BSTR.
	SysFreeString(fwBstrName);

	// Release the open port instance.
	if (fwOpenPort != NULL)
	{
		fwOpenPort->Release();
	}

	// Release the globally open ports collection.
	if (fwOpenPorts != NULL)
	{
		fwOpenPorts->Release();
	}

	return hr;
}

HRESULT CWindowsFirewall::WindowsFirewallInitialize(OUT INetFwProfile** fwProfile)
{
	HRESULT hr = S_OK;
	INetFwMgr* fwMgr = NULL;
	INetFwPolicy* fwPolicy = NULL;

	if(fwProfile == NULL) return -1;

	*fwProfile = NULL;

	// Create an instance of the firewall settings manager.
	hr = CoCreateInstance(
		__uuidof(NetFwMgr),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(INetFwMgr),
		(void**)&fwMgr
		);

	if (FAILED(hr)) {
		// Release the local firewall policy.
		if (fwPolicy != NULL) {
			fwPolicy->Release();
		}

		// Release the firewall settings manager.
		if (fwMgr != NULL) {
			fwMgr->Release();
		}

		return hr;
	}

	// Retrieve the local firewall policy.
	hr = fwMgr->get_LocalPolicy(&fwPolicy);
	if (FAILED(hr))
	{
		// Release the local firewall policy.
		if (fwPolicy != NULL) {
			fwPolicy->Release();
		}

		// Release the firewall settings manager.
		if (fwMgr != NULL)	{
			fwMgr->Release();
		}

		return hr;
	}

	// Retrieve the firewall profile currently in effect.
	hr = fwPolicy->get_CurrentProfile(fwProfile);
	if (FAILED(hr))
	{
		// Release the local firewall policy.
		if (fwPolicy != NULL) {
			fwPolicy->Release();
		}

		// Release the firewall settings manager.
		if (fwMgr != NULL) {
			fwMgr->Release();
		}

		return hr;
	}

	// Release the local firewall policy.
	if (fwPolicy != NULL) {
		fwPolicy->Release();
	}

	// Release the firewall settings manager.
	if (fwMgr != NULL) {
		fwMgr->Release();
	}

	return hr;
}


void CWindowsFirewall::WindowsFirewallCleanup(IN INetFwProfile* fwProfile)
{
	// Release the firewall profile.
	if (fwProfile != NULL) {
		fwProfile->Release();
	}
}
