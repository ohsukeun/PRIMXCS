/// @file pmx_cuconf.cpp
/// @author Park, sung soo
/// @date 2023-06-16
/// primx-cs control & update server configue tool 


/*
SYNOPSIS
	pmx_cuconf help
	pmx_cuconf show [SHOW-OPTION]
	pmx_cuconf set [SET-OPTION] [KEY] [VALUE]

SHOW-OPTIOS
	all:		 Display network and database configure
	database:	 Display database configure
	network:	 Display network configure

SET-OPTIONS
	database:	 Set database configure
	network:	 Set network configure

KEYS
	<database>
	db-ip:       System database ip address (default: 127.0.0.1)
	db-port:     System database port number (default: 53306)
	db-account:  System database account 
	db-password: System database password on account

	<network>
	ctrl-ip:     Control server listen ip address
	ctrl-port:   Control server listen port number (default 50101)
	update-ip:   Update server listen ip address
	update-port: Update server listen port number (default 50102)

AUTHOR
	Rimahr. All Right Reserved. Unlimited
	Version 1.0.0 Build No. 01
*/

#include <termios.h>

#include <pmx_conf_mgr.h>

using namespace std;

#define PMX_CONF_STR_ALL         "all"
#define PMX_CONF_STR_CTRL_IP     "ctrl-ip"
#define PMX_CONF_STR_CTRL_PORT   "ctrl-port"
#define PMX_CONF_STR_DATABASE    "database"
#define PMX_CONF_STR_DB_ACCOUNT  "db-account"
#define PMX_CONF_STR_DB_IP       "db-ip"
#define PMX_CONF_STR_DB_PORT     "db-port"
#define PMX_CONF_STR_DB_PASSWORD "db-password"
#define PMX_CONF_STR_HELP        "help"
#define PMX_CONF_STR_NETWORK     "network"
#define PMX_CONF_STR_SET         "set"
#define PMX_CONF_STR_SHOW        "show"
#define PMX_CONF_STR_UPDATE_IP   "update-ip"
#define PMX_CONF_STR_UPDATE_PORT "update-port"

VOID PrintHelp(IN SWC_BOOL bError = FALSE, IN WCString sErrMsg = (CHAR*)"")
{
	if (TRUE == bError)
	{
		if(0 == sErrMsg.GetLength())
		{
			cout << "\033[5;31merror: invalid option or format\033[0m\n\n";
		}
		else
		{
			cout << "\033[5;31merror: " << LPCSTR(sErrMsg) << "\033[0m\n\n";
		}
	}

	cout << "\033[4mPRIMX-CS control and update server environment set up tool\033[0m" << endl << endl;

	cout << "\033[1mSYNOPSIS\033[0m\n";
	cout << "\tpmx_cuconf \033[4mhelp\033[0m\n";
	cout << "\tpmx_cuconf \033[4mshow\033[0m [\033[1mSHOW-OPTION\033[0m]\n";
	cout << "\tpmx_cuconf \033[4mset\033[0m [\033[1mKEY\033[0m] [\033[1mVALUE\033[0m]\n\n";

	cout << "\033[1mPASSWORD\033[0m\n";
	cout << "\tPredefined PRIMX-CS system password\n\n";

	cout << "\033[1mSHOW-OPTIOS\033[0m\n";
	cout << "\t\033[4mall\033[0m:          Display network and database configuration\n";
	cout << "\t\033[4mdatabase\033[0m:     Display database configuration\n";
	cout << "\t\033[4mnetwork\033[0m:      Display network configuration\n\n";

	cout << "\033[1mKEYS\033[0m\n";
    cout << "\t<database>\n";
    cout << "\t\033[4mdb-ip\033[0m:        System database ip address (default: 127.0.0.1)\n";
    cout << "\t\033[4mdb-port\033[0m:      System database port number (default: 53306)\n";
    cout << "\t\033[4mdb-account\033[0m:   System database account (default: root)\n";
    cout << "\t\033[4mdb-password\033[0m:  System database password on account\n\n";

    cout << "\t<network>\n";
    cout << "\t\033[4mctrl-ip\033[0m:      Control server listening ip address (or 'ANY')\n";
    cout << "\t\033[4mctrl-port\033[0m:    Control server listening port number (default 50101)\n";
    cout << "\t\033[4mupdate-ip\033[0m:    Update server listening ip address (or 'ANY')\n";
    cout << "\t\033[4mupdate-port\033[0m:  Update server listening port number (default 50102)\n\n";

	cout << "\033[1mAUTHOR\033[0m\n";
	cout << "\tRimahr Co. All Right Reserved. Unlimited\n";

	WCString sVersion = "";
	sVersion.Format("\tVersion %d.%d.%d Build No. %d (Built Date: %s %s)\n\n", 
		MAJOR_VER, MINOR1_VER, MINOR2_VER, BUILD_NO_VER, BUILD_DATE_STR, BUILD_TIME_STR);
	cout << LPCSTR(sVersion);
}

SWC_BOOL IsIPStr(IN WCString sIP)
{
	WCString sTemp = "";
	sTemp = sIP;
	sTemp.MakeUpper();
	if (0 == sTemp.CompareNoCase("ANY")) return TRUE;

	for (INT nID = 0; nID < sIP.GetLength(); nID++)
	{
		if (0 == std::isdigit(sIP[nID])) 
		{
			if ('.' != sIP[nID]) return FALSE;
		}
	}
	return TRUE;
}

SWC_BOOL IsNumStr(IN WCString sNum)
{
	for (INT nID = 0; nID < sNum.GetLength(); nID++)
	{
		if (0 == std::isdigit(sNum[nID])) return FALSE;
	}
	return TRUE;
}

SWC_BOOL CheckArgs(IN PCHAR Argv[], 
				   OUT WCString& sCommand, 
				   OUT WCString& sOption, 
				   OUT WCString& sKey, 
				   OUT WCString& sValue)
{
	if (NULL == Argv[1]) { PrintHelp(TRUE); return FALSE; }

	WCString sBuf = "";
	sBuf = Argv[1]; sBuf.Trim();
	if (sBuf == PMX_CONF_STR_HELP) { PrintHelp(FALSE); return FALSE; }

	if (NULL == Argv[1]) { PrintHelp(TRUE); return FALSE; }
	if (NULL == Argv[2]) { PrintHelp(TRUE); return FALSE; }

	sCommand  = Argv[1]; sCommand.Trim();

	if (sCommand == PMX_CONF_STR_SET)
	{
		if (NULL == Argv[2]) { PrintHelp(TRUE, "empty key");   return FALSE; }
		if (NULL == Argv[3]) { PrintHelp(TRUE, "empty value"); return FALSE; }

		sKey   = Argv[2]; sKey.Trim();
		sValue = Argv[3]; sValue.Trim();

		if (sKey != PMX_CONF_STR_CTRL_IP     && 
			sKey != PMX_CONF_STR_CTRL_PORT   && 
			sKey != PMX_CONF_STR_UPDATE_IP   && 
			sKey != PMX_CONF_STR_UPDATE_PORT &&
			sKey != PMX_CONF_STR_DB_IP       && 
			sKey != PMX_CONF_STR_DB_PORT     && 
			sKey != PMX_CONF_STR_DB_ACCOUNT  && 
			sKey != PMX_CONF_STR_DB_PASSWORD)
		{
			PrintHelp(TRUE, "invalid key");
			return FALSE;
		}

		if (sKey == PMX_CONF_STR_DB_IP   ||
			sKey == PMX_CONF_STR_CTRL_IP || 
			sKey == PMX_CONF_STR_UPDATE_IP)
		{
			if (FALSE == IsIPStr(sValue))
			{
				PrintHelp(TRUE, "invalid ip string");
				return FALSE;
			}
		}
		
		if (sKey == PMX_CONF_STR_DB_PORT   ||
			sKey == PMX_CONF_STR_CTRL_PORT || 
			sKey == PMX_CONF_STR_UPDATE_PORT)
		{
			if (FALSE == IsNumStr(sValue))
			{
				PrintHelp(TRUE, "invalid port number");
				return FALSE;
			}
		}
	}
	else if (sCommand == PMX_CONF_STR_SHOW)
	{
		sOption = Argv[2]; sOption.Trim();
		
		if (sOption != PMX_CONF_STR_ALL     && 
			sOption != PMX_CONF_STR_NETWORK && 
			sOption != PMX_CONF_STR_DATABASE)
		{
			PrintHelp(TRUE, "invalid SHOW-OPTION"); 
			return FALSE;
		}
	}
	else
	{
		PrintHelp(TRUE);
		return FALSE;
	}
	return TRUE;
}

VOID DisplayDatabaseConfig(IN CPmxCUConfMgr* pConfMgr)
{
	cout << "\033[1m[database]\033[0m\n";
	cout << "db-ip: "       << LPCSTR(pConfMgr->GetDatabaseHost())    << endl;
	cout << "db-port: "     << LPCSTR(pConfMgr->GetDatabasePortStr()) << endl;
	cout << "db-account: "  << LPCSTR(pConfMgr->GetDatabaseUser())    << endl;
	cout << "db-password: " << LPCSTR(pConfMgr->GetDatabasePwd())     << endl << endl;
}

VOID DisplayNetworkConfig(IN CPmxCUConfMgr* pConfMgr)
{
	cout << "\033[1m[network]\033[0m\n";
	cout << "ctrl-ip: "     << LPCSTR(pConfMgr->GetNetworkCtrlSvrIP())        << endl;
	cout << "ctrl-port: "   << LPCSTR(pConfMgr->GetNetworkCtrlSvrPortStr())   << endl;
	cout << "update-ip: "   << LPCSTR(pConfMgr->GetNetworkUpdateSvrIP())      << endl;
	cout << "update-port: " << LPCSTR(pConfMgr->GetNetworkUpdateSvrPortStr()) << endl << endl;
}

VOID SetConfig(IN OUT CPmxCUConfMgr* pConfMgr, IN WCString sKey, IN WCString sValue)
{
	if      (sKey == PMX_CONF_STR_DB_IP      ) pConfMgr->SetDatabaseHost(sValue);
	else if (sKey == PMX_CONF_STR_DB_PORT    ) pConfMgr->SetDatabasePort(sValue);
	else if (sKey == PMX_CONF_STR_DB_ACCOUNT ) pConfMgr->SetDatabaseUser(sValue);
	else if (sKey == PMX_CONF_STR_DB_PASSWORD) pConfMgr->SetDatabasePwd(sValue);
	else if (sKey == PMX_CONF_STR_CTRL_IP    ) pConfMgr->SetNetworkCtrlSvrIP(sValue);
	else if (sKey == PMX_CONF_STR_CTRL_PORT  ) pConfMgr->SetNetworkCtrlSvrPort(sValue);
	else if (sKey == PMX_CONF_STR_UPDATE_IP  ) pConfMgr->SetNetworkUpdateSvrIP(sValue);
	else if (sKey == PMX_CONF_STR_UPDATE_PORT) pConfMgr->SetNetworkUpdateSvrPort(sValue);

	if (FALSE == pConfMgr->Save())
	{
		cout << "critical error: config file writing failed\n\n";
	}
	else
	{
		cout << "applied" << endl;
	}
}

VOID SetStdioEcho(IN SWC_BOOL bEnable = TRUE)
{
	struct termios tty;

	tcgetattr(STDIN_FILENO, &tty);

	if(FALSE == bEnable) tty.c_lflag &= ~ECHO;
	else                 tty.c_lflag |= ECHO;

	(VOID) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

SWC_BOOL CheckPassword()
{
	SWC_BOOL bRet = FALSE;
	string sPassword;

	cout << "\033[1mEnter Password: \033[0m";
	SetStdioEcho(FALSE);
	getline(std::cin, sPassword);
	SetStdioEcho(TRUE);

	WCString sSystemPassword = PMX_SYSTEM_PASSOWRD;

	if (sSystemPassword != sPassword)
	{
		cout << endl;
		cout << "\033[5;31merror: incorrect password\033[0m\n";
	}
	else
	{
		bRet = TRUE;
		cout << "\033[4mpassed\033[0m\n";
	}

	return bRet;
}

INT main(IN INT Argc, IN PCHAR Argv[]) 
{
	WCString sCommand = "", sOption = "", sKey = "", sValue = "";

	if (FALSE == CheckArgs(Argv, sCommand, sOption, sKey, sValue)) return -1;
	if (FALSE == CheckPassword()) return -1;

	CPmxCUConfMgr* pConfMgr = NULL;

	pConfMgr = new CPmxCUConfMgr();
	if (FALSE == pConfMgr->Load())
	{
		cout << "critical error: config file reading failed\n\n";
		return -1;
	}

	if (sCommand == PMX_CONF_STR_SHOW)
	{
		cout << endl;
		if      (sOption == PMX_CONF_STR_ALL     ) { DisplayDatabaseConfig(pConfMgr); DisplayNetworkConfig(pConfMgr); }
		else if (sOption == PMX_CONF_STR_DATABASE) DisplayDatabaseConfig(pConfMgr);
		else if (sOption == PMX_CONF_STR_NETWORK ) DisplayNetworkConfig(pConfMgr);
	}
	else if (sCommand == PMX_CONF_STR_SET)
	{
		SetConfig(pConfMgr, sKey, sValue);
	}

	if (NULL != pConfMgr) delete pConfMgr;

	return 0;
}
