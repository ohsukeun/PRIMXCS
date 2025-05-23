/*****************************************************************************
 *                                                                           *
 * File Name : t_crypto_test.cpp                                             *
 * Theme     : 'swc (standard template wrapper class)' test                  *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '18. 01. 24                     *
 *                                                                           *
 *****************************************************************************/

#include <t_base.h>

#ifndef __T_CRYPTO_TEST_CPP__
#define __T_CRYPTO_TEST_CPP__

//-----------------------------------------------------------------------------
// CLASS DEFINITIONS
//-----------------------------------------------------------------------------
// test env. class ------------------------------------------------------------
class CTestEnv
{
public:
	static CTestEnv* GetInstancePtr()
	{
		if (NULL == m_pInstance) m_pInstance = new CTestEnv();
		return m_pInstance;
	}
	static VOID ReleaseInstance()
	{
		if (NULL != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	VOID Init()
	{

	}

	VOID Cleanup()
	{

	}

private:
	CTestEnv() { Init(); }
	~CTestEnv() { Cleanup(); }

private:
	static CTestEnv* m_pInstance;
};

// test class -----------------------------------------------------------------
class CCryptoTest: public testing::Test
{
protected:
	CTestEnv* pT;

	virtual VOID SetUp()
	{
		pT = CTestEnv::GetInstancePtr();
	}
	virtual VOID TearDown() {}
};

//-----------------------------------------------------------------------------
// TEST CASE DEFINITIONS
//-----------------------------------------------------------------------------
// Init. env
CTestEnv* CTestEnv::m_pInstance = NULL;

WCString sConn = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n\
<connection-info object=\"database\" type=\"mysql\">\n\
	<version no=\"undefined\"></version>\n\
	<protocol>tcp</protocol>\n\
	<host>192.168.0.186</host>\n\
	<port>3306</port>\n\
	<username>root</username>\n\
	<password>fcore1234~!</password>\n\
	<characterset>UTF-8</characterset>\n\
	<language>UTF-8</language>\n\
	<server-process>dedicated</server-process>\n\
	<connect-method type=\"sid\">null</connect-method>\n\
	<schema>APPENDIXDB</schema>\n\
	<connection-string>\n\
		<![CDATA[(DESCRIPTION = (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.0.186)(PORT = 3306))(CONNECT_DATA = (SERVER = DEDICATED)(SID = null)))]]>\n\
	</connection-string>\n\
</connection-info>";

TEST_F(CCryptoTest, encrypred_file)
{
	CARIAEncryption* pAria = new CARIAEncryption();

	WCString sEnc = pAria->EncryptString(sConn);

	fprintf(stdout, "[RAW]\n%s\n\n", LPCSTR(sConn));
	fprintf(stdout, "[ENCRYPTED]\n%s\n\n", LPCSTR(sEnc));

	WCString sDec = pAria->DecryptString(sEnc);
	fprintf(stdout, "[DECRYPTED]\n%s\n\n", LPCSTR(sDec));

	if (NULL != pAria) delete pAria;

	ASSERT_STREQ(LPCSTR(sConn), LPCSTR(sDec));
}

TEST_F(CCryptoTest, EndOfTest)
{
	CTestEnv::ReleaseInstance();
}

//-----------------------------------------------------------------------------
#endif
