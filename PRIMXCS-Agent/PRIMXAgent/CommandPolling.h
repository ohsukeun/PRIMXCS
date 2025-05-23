#pragma once
#include "CommandManagerEx.h"


class CCommandPolling : public CCommandManagerEx
{
public:
	CCommandPolling();
	~CCommandPolling();

	BOOL SetPolling();
	BOOL SetShutdown();
	BOOL SetUninstall();

	CString GetExpandCommand();
	UINT GetKeepAliveCycle();
	UINT GetLogCycle();
	BOOL GetAgentUpdate();
	BOOL GetZonecentralUpdate();
	BOOL GetOrizonUpdate();
	BOOL GetCryhodUpdate();
	CString GetCompanyID();
	CString GetAuthServerAddr();
	UINT GetAuthServerPort();

	BOOL GetPolicyData(PRIMX_POLICY_DATA& PolicyData);

protected:
	CString		m_strCode;
	CString		m_strSubCode;

	virtual UINT SetResponseDataTypeParser(CString& strXML);
	virtual UINT SetRequestDataTypeGenerate(CMarkup& XMLMarkup);

	PRIMX_PRODUCT_DATA		m_ZonecentrolData;
	PRIMX_PRODUCT_DATA		m_OrizoneData;
	PRIMX_PRODUCT_DATA		m_CryhodData;

	// 응답 데이터
	CString			m_strExpandCommand;			// 확장 명령
	UINT			m_nKeepAliveCycle;			// KeepAlive 주기
	UINT			m_nLogCycle;				// 로그 전송 주기
	BOOL			m_bAgentUpdate;             // PRIMX Agent 업데이트 여부
	BOOL			m_bZonecentralUpdate;       // PRIMX ZONECENTRAL 업데이트 여부
	BOOL			m_bOrizonUpdate;            // PRIMX ORIZON 업데이트 여부
	BOOL			m_bCryhodUpdate;            // PRIMX CRYHOD 업데이트 여부 
	CString			m_strCompanyID;             // 회사 ID
	CString			m_strAuthServerAddr;        // 반출 파일 인증 서버 주소 
	UINT			m_nAuthServerPort;          // 반출 파일 인증 서버 Port 

	PRIMX_POLICY_DATA	m_AgentPolicyData;


};

