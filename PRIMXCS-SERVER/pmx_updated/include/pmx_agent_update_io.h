/// @file pmx_update_ctrl_io.h
/// @author Park, sung soo
/// @date 2023-07-21
/// update channel i/o handling

#include <swc_ssl_handler.h>
#include <pmx_conf_mgr.h>
#include <pmx_io.h>
#include <pmx_mysql_obj.h>

#ifndef __PMX_AGENT_UPDATE_IO_H__
#define __PMX_AGENT_UPDATE_IO_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

class CAgentUpdateIO : public CSSLWorkTh
{
public :
	SWC_BOOL ConnectToDatabase   (IN WCString sHost, IN USHORT nPort, IN WCString sUser, IN WCString sPwd, IN WCString sDBName);
	SWC_BOOL CmdOperate          (IN CSSLIONode* pNode);
	VOID     RunJob              (IN CSSLIONode* pNode);
	VOID     RunCheck            (UINT nInterval);

	CAgentUpdateIO();
	~CAgentUpdateIO();

private:
	SWC_BOOL CmdRequestUpdateFile  (IN CSSLIONode* pNode, IN S_PMX_AGENT_CMD_HD& hd, IN CPmxXmlIO& objXml);
	SWC_BOOL ExecTransactionQueries(IN WCArray<WCString>& arrQueries, OUT WCString& sError);

	UINT     GetBodyLen          (IN CSSLIONode* pNode);
	SWC_BOOL CheckMark           (IN const UCHAR* sMark);
	SWC_BOOL CheckHeadValidity   (IN CSSLIONode* pNode);
	SWC_BOOL SendReply           (IN CSSLIONode* pNode, IN S_PMX_AGENT_CMD_HD& hd, IN WCString sReplyXml);

private:
	CMySQLDBObj* m_pDBObj; ///< database connection object
};


//-----------------------------------------------------------------------------
#endif // __PMX_AGENT_UPDATE_IO_H__
