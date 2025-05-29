/// @file pmx_agent_ctrl_svr.h
/// @author Park, sung soo
/// @date 2023-06-18
/// service main

#include <pmx_agent_ctrl_io.h>
#include <pmx_agent_ctrl_log.h>

#ifndef __PMX_AGENT_CTRL_SVR_H__
#define __PMX_AGENT_CTRL_SVR_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
class CAgentCtrlSvr
{
public:
	SWC_BOOL Init();
	SWC_BOOL Run ();
	VOID     Exit();

	CAgentCtrlSvr();
	~CAgentCtrlSvr();

private:
	VOID     DelCoreFiles       (IN const CHAR* sPath, IN const CHAR* sFind, IN UINT nRemain);
	VOID     CalcWorkThreadCount();
	SWC_BOOL LoadConfig         (OUT WCString& sAgentCtrlIP,
								 OUT USHORT&   nAgentCtrlPort,
								 OUT WCString& sDBAddr,
								 OUT USHORT&   nDBPort, 
								 OUT WCString& sDBUser,
								 OUT WCString& sDBPwd,
								 OUT WCString& sDBName);
	
private:
	CSSLHandler*   m_pAgentCtrlHd;       ///< agent control channel handler
	CAgentCtrlLog* m_pAgentCtrlLog;      ///< primx product log writter
	
	UINT           m_nConcurrentSession; ///< concurrent session limit
	UINT           m_nWorkThreadCount;   ///< work thread count
	USHORT         m_nAgentCtrlPort;     ///< agent control port number
};

//-----------------------------------------------------------------------------
#endif // __PMX_AGENT_CTRL_SVR_H__
