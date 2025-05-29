/// @file pmx_agent_update_svr.h
/// @author Park, sung soo
/// @date 2023-07-20
/// service main

#include <pmx_agent_update_io.h>

#ifndef __PMX_AGENT_UPDATE_SVR_H__
#define __PMX_AGENT_UPDATE_SVR_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
class CAgentUpdateSvr
{
public:
	SWC_BOOL Init();
	SWC_BOOL Run ();
	VOID     Exit();

	CAgentUpdateSvr();
	~CAgentUpdateSvr();

private:
	VOID     DelCoreFiles       (IN const CHAR* sPath, IN const CHAR* sFind, IN UINT nRemain);
	VOID     CalcWorkThreadCount();
	SWC_BOOL LoadConfig         (OUT WCString& sAgentUpdateIP,
								 OUT USHORT&   nAgentUpdatePort,
								 OUT WCString& sDBAddr,
								 OUT USHORT&   nDBPort, 
								 OUT WCString& sDBUser,
								 OUT WCString& sDBPwd,
								 OUT WCString& sDBName);
	
private:
	CSSLHandler*   m_pAgentUpdateHd;     ///< agent control channel handler
	
	UINT           m_nConcurrentSession; ///< concurrent session limit
	UINT           m_nWorkThreadCount;   ///< work thread count
	USHORT         m_nAgentUpdatePort;   ///< agent update port number
};

//-----------------------------------------------------------------------------
#endif // __PMX_AGENT_UPDATE_SVR_H__
