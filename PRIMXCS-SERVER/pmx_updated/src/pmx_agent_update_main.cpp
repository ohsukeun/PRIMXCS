/// @file pmx_agent_update_main.cpp
/// @author Park, sung soo
/// @date 2023-07-20
/// PRIMX-CS agent update service main

#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/time.h>

#include <pmx_agent_update_svr.h>

#ifndef __PMX_AGENT_UPDATE_MAIN_CPP__
#define __PMX_AGENT_UPDATE_MAIN_CPP__

//-----------------------------------------------------------------------------
// LOCAL DEFINITIONS
//-----------------------------------------------------------------------------
SWC_BOOL G_BRUN = TRUE;

VOID DoSigTerm           (IN INT nSigNum);
VOID DoSigInt            (IN INT nSigNUm);
VOID EnlargeFileOpenLimit();
VOID EnlargeStackSize    ();
VOID PrintHelp           ();

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------
INT main(IN INT argc, PCHAR argv[])
{
	BOOL bDebug = FALSE;
	INT nOpt = -1, nIndex = 0;
	struct option options[] = 
	{
		{"DEBUG",   0, 0, 0},
		{"version", 0, 0, 0}, 
		{"help",    0, 0, 0},
		{"daemon",  0, 0, 0},
		{NULL,      0, 0, 0}
	};

	if (NULL != argv[1])
	{
		WCString sOpt;

		sOpt = argv[1];
		sOpt.Trim();
		sOpt.MakeUpper();

		if (sOpt == "-V")
		{
			std::cout << "Version: " << MAJOR_VER << "." << MINOR1_VER << "." << MINOR2_VER << "." << BUILD_NO_VER << std::endl;
			return EXIT_SUCCESS;
		}
	}

	if (-1 != (nOpt = getopt_long(argc, argv, "", options, &nIndex)))
	{
		switch(nOpt)
		{
			case 0:
			{
				switch(nIndex)
				{
					// DEBUG
					case 0: 
						bDebug = TRUE;
						break;
					// version
					case 1:
						std::cout<< "version string: " << MAJOR_VER << "." << MINOR1_VER << "." << MINOR2_VER << " build no. " << BUILD_NO_VER << " (built date " << BUILD_DATE_STR << " " << BUILD_TIME_STR << ")" << std::endl;
						return EXIT_SUCCESS;
					// help
					case 2:
						PrintHelp();
						return EXIT_SUCCESS;
					// daemon
					case 3:
						break;
					default:
						PrintHelp();
						return EXIT_FAILURE;
				}
				break;
			}
			default:
				PrintHelp();
				return EXIT_SUCCESS;
		}
	}
	else
	{
		if (NULL != argv[1])
		{
			std::cout << BIN_STR << ": invalid option " << argv[1] << std::endl;
			PrintHelp();
			return EXIT_FAILURE;
		}
		else
		{
			bDebug = FALSE;
		}
	}

	// service mode
	if (FALSE == bDebug)
	{
		pid_t forkpid = fork();

		// run as process 1
		SWC::swc_sleep(0, 10000); // delay for a low performance system

		if (0 > forkpid) exit(0);
		else if (0 != forkpid) exit(0);
		umask(0);
		setsid();

		chdir("/");
		SetDebugModeOff();
		SetMsgOff();

		_LOGGER_INIT_(BIN_STR, PMX_CU_CONF_FILE_DIR, PMX_PATH_PRIMXSERVER_LOGS);
		_LOGGER_RELEASE;
	}
	// debug mode
	else
	{
		SetDebugModeOn();

		_LOGGER_INIT_(BIN_STR, PMX_CU_CONF_FILE_DIR, PMX_PATH_PRIMXSERVER_LOGS);
		_LOGGER_DEBUG;
	}

	// check if same process already running ...
	if (TRUE == SWC::swc_chk_proc_exist(BIN_STR))
	{
		_LOG_ERR("same process already running. exit");
		SWC::swc_sleep(0, 100000);
		return EXIT_FAILURE;
	}

	// write pid file
	if (FALSE == bDebug)
	{
		WCString sCmd = "";
		sCmd.Format("echo %u > %s/%s.pid", getpid(), PMX_CU_PID_FILE_DIR_PATH, BIN_STR);

		SWC::swc_system(LPCSTR(sCmd));
	}

	// signam handling
	signal(SIGPIPE, SIG_IGN);   // ignore SIGPIPE
	signal(SIGCHLD, SIG_IGN);   // ignore SIGCHLD
	signal(SIGHUP,  SIG_IGN);   // ignore SIGHUP
	signal(SIGALRM, SIG_IGN);   // ignore SIGALRM
	signal(SIGTERM, DoSigTerm);
	if (TRUE == bDebug) signal(SIGINT, DoSigInt);
	else                signal(SIGINT, SIG_IGN);

	// run main
	CAgentUpdateSvr* pObj = new CAgentUpdateSvr();
	if (FALSE == pObj->Init())
	{
		_LOG_ERR("initionalization failed. exit");
		delete pObj;
		return EXIT_FAILURE;
	}
	EnlargeFileOpenLimit();
	EnlargeStackSize();

	if (FALSE == pObj->Run())
	{
		_LOG_ERR("running failed. exit");
		delete pObj;
		return EXIT_FAILURE;
	}

	SWC::swc_sleep(0, 1000);
	_LOG_MSG("\033[1m%s version %d.%d.%d build no. %d (built date %s %s) started\033[0m", 
		BIN_STR, MAJOR_VER, MINOR1_VER, MINOR2_VER, BUILD_NO_VER, BUILD_DATE_STR, BUILD_TIME_STR);

	while(TRUE == G_BRUN)
	{
		SWC::swc_sleep(0, 10000);
	}
	delete pObj;

	_LOGGER_BYE;

	return EXIT_SUCCESS;
}

VOID DoSigTerm(IN INT nSigNum)
{
	G_BRUN = FALSE;
	SWC::swc_sleep(0, 10000);
	_LOG_INF("terminated normally (SIGTERM)");
	SWC::swc_sleep(0, 10000);
	exit(0);
}

VOID DoSigInt(IN INT nSigNum)
{
	G_BRUN = FALSE;
	SWC::swc_sleep(0, 10000);
	_LOG_INF("terminated forcibly (SIGINT)");
	SWC::swc_sleep(0, 10000);
	exit(0);
}

// enlarge file open limit
VOID EnlargeFileOpenLimit()
{
	struct rlimit limit;

	getrlimit(RLIMIT_NOFILE, &limit);
	_LOG_DBG("current file open limit = %lu / max = %lu",  (u_long)limit.rlim_cur, (u_long)limit.rlim_max);

	limit.rlim_cur = 40960lu;
	limit.rlim_max = 40960lu;
	setrlimit(RLIMIT_NOFILE, &limit);
	_LOG_DBG("file open limit enlarged = %lu", (u_long)limit.rlim_max);
}

// enlarge stack size
VOID EnlargeStackSize()
{
	const rlim_t nstacksize = 32 * 1024 * 1024; // min. stack size = 32 MB
	struct rlimit limit;

	getrlimit(RLIMIT_STACK, &limit);
	_LOG_DBG("current stack min. size = %u, enlarged as %u", limit.rlim_cur, nstacksize);
	limit.rlim_cur = nstacksize;
	setrlimit(RLIMIT_STACK, &limit);
}

// print help message
VOID PrintHelp()
{
	std::cout << "\nusage : " << BIN_STR << " [\033[1mOPTION\033[0m]" << std::endl << std::endl;
	std::cout << "\033[1mOPTION\033[0m" << std::endl;
	std::cout << "\t\033[4m--D, --DEBUG\033[0m            run as debugging mode" << std::endl;
	std::cout << "\t\033[4m--d, --daemon\033[0m           run as daemon mode (or no argument)" << std::endl;
	std::cout << "\t\033[4m--v, --version\033[0m          print version" << std::endl;
	std::cout << "\t\033[4m--h, --help\033[0m             print help message" << std::endl << std::endl;

	std::cout << "\033[1mAUTHOR\033[0m" << std::endl;
	std::cout << "\tRimahr Co. All Right Reserved. Unlimited" << std::endl;

	WCString sVersion = "";
	sVersion.Format("\tVersion %d.%d.%d Build No. %d (Built Date: %s %s)",
			MAJOR_VER, MINOR1_VER, MINOR2_VER, BUILD_NO_VER, BUILD_DATE_STR, BUILD_TIME_STR);
	std::cout << LPCSTR(sVersion) << std::endl << std::endl;
}

//-----------------------------------------------------------------------------
#endif // __PMX_AGENT_UPDATE_MAIN_CPP__
