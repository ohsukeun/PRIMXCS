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

	// ���� ������
	CString			m_strExpandCommand;			// Ȯ�� ���
	UINT			m_nKeepAliveCycle;			// KeepAlive �ֱ�
	UINT			m_nLogCycle;				// �α� ���� �ֱ�
	BOOL			m_bAgentUpdate;             // PRIMX Agent ������Ʈ ����
	BOOL			m_bZonecentralUpdate;       // PRIMX ZONECENTRAL ������Ʈ ����
	BOOL			m_bOrizonUpdate;            // PRIMX ORIZON ������Ʈ ����
	BOOL			m_bCryhodUpdate;            // PRIMX CRYHOD ������Ʈ ���� 
	CString			m_strCompanyID;             // ȸ�� ID
	CString			m_strAuthServerAddr;        // ���� ���� ���� ���� �ּ� 
	UINT			m_nAuthServerPort;          // ���� ���� ���� ���� Port 

	PRIMX_POLICY_DATA	m_AgentPolicyData;


};

