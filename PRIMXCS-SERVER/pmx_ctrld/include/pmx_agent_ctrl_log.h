/// @file pmx_agent_ctrl_log.h
/// @author Park, sung soo
/// @date 2023-07-17
/// agent log writter

#include <pmx_io.h>
#include <pmx_mysql_obj.h>

#ifndef __PMX_AGENT_CTRL_LOG_H__
#define __PMX_AGENT_CTRL_LOG_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
class CAgentCtrlLog
{
public:
	SWC_BOOL Init         (IN WCString sHost, IN USHORT nPort, IN WCString sUser, IN WCString sPwd, IN WCString sDBName);
	SWC_BOOL Run          ();
	VOID     Exit         ();
	VOID     OperateLogXml();
	VOID     RunCheck     ();
	
	CAgentCtrlLog();
	~CAgentCtrlLog();

private:
	SWC_BOOL ConnectToDatabase     (IN WCString sHost, IN USHORT nPort, IN WCString sUser, IN WCString sPwd, IN WCString sDBName);
	SWC_BOOL OperateLogXmlFile     (IN WCString sFilePath);
	SWC_BOOL ExecTransactionQueries(IN WCArray<WCString>& arrQueries, OUT WCString& sError);

	static PVOID LogWriteTreadFunc(PVOID pArg);

public:
	pthread_t m_ThreadID;
	SWC_BOOL  m_bRun;
	
private:
	CMySQLDBObj* m_pDBObj;              ///< database connection object
	time_t       m_tPrevLogOperate;     ///< previous log file operated time
	time_t       m_tPrevRunChecked;     ///< previous module status checked time
	UINT         m_nLogOperateInterval; ///< log file operating time inverval (seconds)
	UINT         m_nRunCheckInterval;   ///< run check interval (seconds)
	WCString     m_sModuleName;         ///< module name
};

//-----------------------------------------------------------------------------
#endif // __PMX_AGENT_CTRL_LOG_H__
