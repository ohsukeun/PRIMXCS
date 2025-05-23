/// @file pmx_agent_auth_io.h
/// @author Park, sung soo
/// @date 2023-07-25
/// authenticate channel i/o handling

#include <swc_ssl_handler.h>
#include <pmx_conf_mgr.h>
#include <pmx_io.h>
#include <pmx_mysql_obj.h>

#ifndef __PMX_AGENT_AUTH_IO_H__
#define __PMX_AGENT_AUTH_IO_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

class CAgentAuthIO : public CSSLWorkTh
{
public :
	SWC_BOOL ConnectToDatabase   (IN WCString sHost, IN USHORT nPort, IN WCString sUser, IN WCString sPwd, IN WCString sDBName);
	SWC_BOOL CmdOperate          (IN CSSLIONode* pNode);
	VOID     RunJob              (IN CSSLIONode* pNode);
	VOID     RunCheck            (UINT nInterval);


	CAgentAuthIO();
	~CAgentAuthIO();

private:
	SWC_BOOL CmdAuthenticateExDecryption(IN CSSLIONode* pNode, IN S_PMX_AGENT_CMD_HD& hd, IN CPmxXmlIO& objXml);

	UINT     GetBodyLen          (IN CSSLIONode* pNode);
	SWC_BOOL CheckMark           (IN const UCHAR* sMark);
	SWC_BOOL CheckHeadValidity   (IN CSSLIONode* pNode);
	SWC_BOOL SendReply           (IN CSSLIONode* pNode, IN S_PMX_AGENT_CMD_HD& hd, IN WCString sReplyXml);

private:
	CMySQLDBObj* m_pDBObj; ///< database connection object
};


//-----------------------------------------------------------------------------
#endif // __PMX_AGENT_AUTH_IO_H__
