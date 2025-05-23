#pragma once
#include "../CommonLib/Utils/CommandManagerEx.h"

class CCommandUserSearch : public CCommandManagerEx
{
public:
	CCommandUserSearch();
	~CCommandUserSearch();

	BOOL SetConnect();
	BOOL SetUserDataSearch(CString strUserID);
	BOOL GetUserData(PRIMX_USER_DATA& UserData);
	CString GetUserID();
	CString GetUserName();
	CString GetTel();
	CString GetCellPhone();
	CString GetExtNo();
	CString GetMail();
	CString GetDepartment();

	virtual UINT SetResponseDataTypeParser(CString& strXML);
		
protected:
	CString			m_strSearchUserID;
	CString			m_strUserID;
	CString			m_strUserName;
	CString			m_strTel;
	CString			m_strCellPhone;
	CString			m_strExtNo;
	CString			m_strMail;
	CString			m_strDepartment;

	CStringList		m_ServerAddrList;
	unsigned short	m_nServerPort;
};

