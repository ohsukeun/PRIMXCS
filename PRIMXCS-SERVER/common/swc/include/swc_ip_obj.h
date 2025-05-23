/*****************************************************************************
 *                                                                           *
 * File Name : swc_ip_obj.h                                                  *
 * Theme     : SWC - ip object definitions                                   *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '11. 02. 08                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_ip_obj.h
/// @author Park, sung soo
/// @date 2011-02-08
/// @brief ip object definitions

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <swc_base.h>

#ifndef __SWC_IP_OBJ_H__
#define __SWC_IP_OBJ_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

#ifndef SWC_IP_STR_LEN 
	#define SWC_IP_STR_LEN    16
#endif

/// ip object class
class WCIP
{
public :
	VOID SetIP         (IN const UINT lip)
	{
		struct in_addr ip_addr;

		m_lIP = lip;
		if(0 == lip)
		{
			snprintf(m_sIP, SWC_IP_STR_LEN, "0.0.0.0");
		}
		else
		{
			ip_addr.s_addr = htonl(lip);
			snprintf(m_sIP, SWC_IP_STR_LEN, "%s", inet_ntoa(ip_addr));
		}
	};
	VOID SetSIP        (IN const CHAR* sip)
	{
		if(NULL == sip) return;
		if(0 == strcmp("0.0.0.0", sip))
		{
			m_lIP = 0;
			snprintf(m_sIP, SWC_IP_STR_LEN, "0.0.0.0");
		}
		else
		{
			m_lIP = htonl(inet_addr(sip));
			snprintf(m_sIP, SWC_IP_STR_LEN, "%s", sip);
		}
	}
	VOID SetAgentID    (IN const UINT64 llagid)  { m_llAgentID = llagid; };
	VOID SetGroupID    (IN const UINT   lgrpid)  { m_lGroupID  = lgrpid; };
	VOID SetAgentObjPtr(IN VOID*        pag)     { m_pAgentObj = pag; };
	VOID SetGroupObjPtr(IN VOID*        pgrp)    { m_pGroupObj = pgrp; };
	VOID SetUpdateTime (IN time_t       tupdate) { m_tUpdate   = tupdate; };
	VOID SetUpdateTime ()
	{
		time_t tcur = 0;

		time(&tcur);
		m_tUpdate = tcur;
	}
	VOID Init          ()
	{
		m_lIP       = 0;
		memset(m_sIP, 0x00, SWC_IP_STR_LEN);
		m_llAgentID = 0;
		m_lGroupID  = 0;
		m_pAgentObj = 0;
		m_pGroupObj = 0;
		time(&m_tUpdate);
	}

	const UINT   GetLIP        () const { return m_lIP; };
	const CHAR*  GetSIP        () const { return m_sIP; };
	const UINT64 GetAgentID    () const { return m_llAgentID; };
	const UINT   GetGroupID    () const { return m_lGroupID; };
	VOID*        GetAgentObjPtr() const { return m_pAgentObj; };
	VOID*        GetGroupObjPtr() const { return m_pGroupObj; };
	const time_t GetUpdateTime () const { return m_tUpdate; };

	WCIP() { Init(); }
	WCIP(IN UINT lip)
	{
		SetIP(lip);
		m_llAgentID = 0;
		m_lGroupID  = 0;
		m_pAgentObj = NULL;
		m_pGroupObj = NULL;
	}
	WCIP(IN const UINT   lip,
         IN const UINT64 llagid,
         IN const UINT   lgrpid,
         IN VOID*        pag,
         IN VOID*        pgrp)
	{
		SetIP(lip);
		m_llAgentID = llagid;
		m_lGroupID  = lgrpid;
		m_pAgentObj = pag;
		m_pGroupObj = pgrp;
	}
	virtual ~WCIP() {};

private :
	UINT   m_lIP;
	CHAR   m_sIP[SWC_IP_STR_LEN];
	UINT64 m_llAgentID;
	UINT   m_lGroupID;
	VOID*  m_pAgentObj;
	VOID*  m_pGroupObj;
	time_t m_tUpdate;
};

//-----------------------------------------------------------------------------
#endif // __SWC_IP_OBJ_H__
 
