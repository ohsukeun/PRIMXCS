/// @file pmx_agent_auth_svr.h
/// @author Park, sung soo
/// @date 2023-07-25
/// service main

#include <pmx_agent_auth_io.h>

#ifndef __PMX_AGENT_AUTH_SVR_H__
#define __PMX_AGENT_AUTH_SVR_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
class CAgentAuthSvr
{
public:
	SWC_BOOL Init();
	SWC_BOOL Run ();
	VOID     Exit();

	CAgentAuthSvr();
	~CAgentAuthSvr();

private:
	VOID     DelCoreFiles       (IN const CHAR* sPath, IN const CHAR* sFind, IN UINT nRemain);
	VOID     CalcWorkThreadCount();
	SWC_BOOL LoadConfig         (OUT WCString& sAgentAuthIP,
								 OUT USHORT&   nAgentAuthPort,
								 OUT WCString& sDBAddr,
								 OUT USHORT&   nDBPort, 
								 OUT WCString& sDBUser,
								 OUT WCString& sDBPwd,
								 OUT WCString& sDBName);
	
private:
	CSSLHandler*   m_pAgentAuthHd;       ///< agent control channel handler
	
	UINT           m_nConcurrentSession; ///< concurrent session limit
	UINT           m_nWorkThreadCount;   ///< work thread count
	USHORT         m_nAgentAuthPort;     ///< agent update port number
};

//-----------------------------------------------------------------------------
#endif // __PMX_AGENT_AUTH_SVR_H__
