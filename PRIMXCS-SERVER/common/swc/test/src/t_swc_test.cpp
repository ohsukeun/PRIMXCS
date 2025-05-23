/*****************************************************************************
 *                                                                           *
 * File Name : t_swc_test.cpp                                                *
 * Theme     : 'swc (standard template wrapper class)' test                  *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '18. 01. 24                     *
 *                                                                           *
 *****************************************************************************/

#include <t_base.h>
#include <filesystem>

#ifndef __T_SWC_TEST_CPP__
#define __T_SWC_TEST_CPP__

//-----------------------------------------------------------------------------
// CLASS DEFINITIONS
//-----------------------------------------------------------------------------
// test env. class ------------------------------------------------------------
class CTestEnv
{
public:
	static CTestEnv* GetInstancePtr()
	{
		if(NULL == m_pInstance) m_pInstance = new CTestEnv();
		return m_pInstance;
	}
	static VOID ReleaseInstance()
	{
		if(NULL != m_pInstance)
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
	CTestEnv() { Init(); };
	~CTestEnv() { Cleanup(); };

private:
	static CTestEnv* m_pInstance;
};

// test class -----------------------------------------------------------------
class CSwcTest : public testing::Test
{
protected:
	CTestEnv* pT;

	virtual VOID SetUp()
	{
		pT = CTestEnv::GetInstancePtr();
	};
	virtual VOID TearDown() {};
};

//-----------------------------------------------------------------------------
// TEST CASE DEFINITIONS
//-----------------------------------------------------------------------------
// init. env
CTestEnv* CTestEnv::m_pInstance = NULL;

TEST_F(CSwcTest, operators)
{
	WCWString swStr1 = L"", swStr2 = L"", swStr3 = L"";
	
	swStr1 = L"(CASE 1) 이건 테스트야~!!";
	swStr2 = L"This is a TEST~!!";
	swStr3 = swStr1;
	swStr3 += L" / ";
	swStr3 += swStr2;

	swStr1.AppendFormat(L" (length:%d)", swStr1.GetLength());
	swStr2.AppendFormat(L" (length:%d)", swStr2.GetLength());
	swStr3.AppendFormat(L" (length:%d)", swStr3.GetLength());
	
	std::wcout << LPCWSTR(swStr1) << std::endl;
	std::wcout << LPCWSTR(swStr2) << std::endl;
	std::wcout << LPCWSTR(swStr3) << std::endl;
}

TEST_F(CSwcTest, WStrToStr)
{
	WCWString swStr = L"";
	WCString sStr = "";

	swStr = L"(CASE 2) 이건 테스트야~!!";
	//sStr = swStr.ToUTF8("UTF-32");
	sStr = swStr.ToMBTS();

	std::wcout << LPCWSTR(swStr) << std::endl;
	std::cout << LPCSTR(sStr) << std::endl;
}

TEST_F(CSwcTest, StrToWStr)
{
/*
	WCWString swStr1 = L"", swStr2 = L"";
	WCString sStr1 = "", sStr2 = "";

	sStr1  = "(CASE 3) 이건테스트야~!!";
	swStr1 = LPCSTR(sStr1);
	sStr2  = swStr1.ToMBTS();
	swStr2 = sStr1.ToWStr("UTF8");

	std::cout << LPCSTR(sStr1) << std::endl;
	printf("sStr1.GetLength()=%d\n", sStr1.GetLength());
	std::wcout << LPCWSTR(swStr1) << std::endl;
	printf("swStr1.GetLength()=%d\n", swStr1.GetLength());
	std::cout << LPCSTR(sStr2) << std::endl;
	printf("sStr2.GetLength()=%d\n", sStr2.GetLength());
	std::wcout << LPCWSTR(swStr2) << std::endl;
	printf("swStr2.GetLength()=%d\n", swStr2.GetLength());
*/
}

TEST_F(CSwcTest, EscapeChar)
{
	WCString sStr = "THIS IS A ' TEST. ' ' ''' END";
	WCString sCopy = sStr;
	sCopy.Replace("'", "\\'");

	std::cout << LPCSTR(sStr) << std::endl;
	std::cout << LPCSTR(sCopy) << std::endl;
}

TEST_F(CSwcTest, EscapeChar2)
{
	WCString sStr = "INSERT INTO T_DISK_INFO ( C_AGENT_ID, C_DISK_NAME, C_DISK_TYPE, C_VOLUMN_NAME, C_FILE_SYSTEM_NAME, C_TOTAL_SIZE, C_FREE_SIZE, C_INSERT_DATE) VALUES (  'BDD74F5B-932B-4F44-A652-BDB8CFF98C94', 'C:\\', 3, 'OS', 'NTFS',  907191, 804742,  UTC_TIMESTAMP()) ON DUPLICATE KEY UPDATE C_DISK_TYPE = 3, C_VOLUMN_NAME = 'OS', C_FILE_SYSTEM_NAME = 'NTFS', C_TOTAL_SIZE = 907191, C_FREE_SIZE = 804742, C_UPDATE_DATE = UTC_TIMESTAMP()";
	WCString sCopy = sStr;
	sCopy.Replace(92, 92, 2);
	//sCopy.Replace("\\'", "\'");

	std::cout << LPCSTR(sStr) << std::endl;
	std::cout << LPCSTR(sCopy) << std::endl;
}

TEST_F(CSwcTest, EscapeChar3)
{
	WCString sSrc = "\"AAAAAA \\'BBBB' 'CCCC' '%' ";
	WCString sResult = "";
	WCString sDoubleQuote = "", sPercentSign = "", sSingleQuote = "";
	WCString sDoubleQuoteReplace = "", sPercentSignReplace = "", sSingleQuoteReplace = "";

	sDoubleQuote += (CHAR)34; sDoubleQuote += '\0';
	sPercentSign += (CHAR)37; sPercentSign += '\0';
	sSingleQuote += (CHAR)39; sSingleQuote += '\0';
	sDoubleQuoteReplace += (CHAR)92; sDoubleQuoteReplace += (CHAR)34; sDoubleQuoteReplace += '\0';
	sPercentSignReplace += (CHAR)92; sPercentSignReplace += (CHAR)37; sPercentSignReplace += '\0';
	sSingleQuoteReplace += (CHAR)92; sSingleQuoteReplace += (CHAR)39; sSingleQuoteReplace += '\0';

	sResult = sSrc;
	sResult.Replace(92, 92, 2); // '\' -> '\\'
	sResult.Replace(sDoubleQuote, sDoubleQuoteReplace);
	sResult.Replace(sPercentSign, sPercentSignReplace);
	sResult.Replace(sSingleQuote, sSingleQuoteReplace);

	std::cout << "length:" << sSrc.GetLength()    << " / " << LPCSTR(sSrc) << std::endl;
	std::cout << "length:" << sResult.GetLength() << " / " << LPCSTR(sResult) << std::endl;
}

TEST_F(CSwcTest, Parse)
{
	WCArray<WCString> arrPath;
	WCString sTest = "C:\\Program Files|C:\\Program Files (x86)|C:\\ProgramData|C:\\Windows|C:\\Temp";
	std::stringstream sstream(sTest.m_Str);
	std::string sCurPath = "";

	while(std::getline(sstream, sCurPath, '|'))
	{
		WCString sCur = "";
		sCur = sCurPath;
		arrPath.Add(sCur);
	}

	for(UINT nID = 0; nID < arrPath.GetCount(); nID++)
	{
		std::cout << LPCSTR(arrPath[nID]) << std::endl;
	}
}

TEST_F(CSwcTest, FileExist)
{
	if (true == std::filesystem::exists("/PRIMXSERVER/CONF/CERTS/MARIADB_CLIENT/client-cert.pem"))
	{
		std::cout << "file exists" << std::endl;
	}
	else
	{
		std::cout << "file dose not exist" << std::endl;
	}
}

TEST_F(CSwcTest, EndOfTest)
{
	CTestEnv::ReleaseInstance();
}

//-----------------------------------------------------------------------------
#endif // __T_SWC_TEST_CPP__
