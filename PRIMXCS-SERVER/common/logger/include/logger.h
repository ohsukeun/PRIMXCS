/*****************************************************************************
 *                                                                           *
 * File Name : logger.h                                                      *
 * Theme     : log writter                                                   *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '14. 04. 25                     *
 *             2. modified by PSS. '23. 06. 16                               *
 *              - struct timeb => time_t & struct timespec                   *
 *                                                                           *
 *****************************************************************************/

/// @file logger.h
/// @author Park, sung soo
/// @date 2018-01-23
/// @brief log writter

#include <sys/time.h>
#include <sys/types.h>

#include <stdio.h>
#include <iostream>
#include <deque>
#include <pthread.h>

#ifndef __LOGGER_H__
#define __LOGGER_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
#define LR_MAX_LOG_QUEUE_COUNT                  10000

// default values -------------------------------------------------------------
// configure related
#define LR_CONF_RELOAD_INTERVAL_DEFAULT         60 // 60 seconds
#define LR_CONF_RELOAD_INTERVAL_DEBUG           10 // 10 seconds

/*
[LogOption]
LogFileCount=100  (maintaining archived file count)
LogFileMaxSize=10 (file max size. mega bytes. if over size, compress and archive)
LogLevel=4        (0: error, 1: warning, 2:information, 3:defail info, 4:debug)
LogOutPut=2       (0:do nothing, 1:print display message, 2:write to log file, 3: print to the console)
*/
#define LR_SECTION_LOGOPTION                    "LogOption"
#define LR_KEY_LOGFILECOUNT                     "LogFileCount"
#define LR_KEY_LOGFILEMAXSIZE                   "LogFileMaxSize"
#define LR_KEY_LOGLEVEL                         "LogLevel"
#define LR_KEY_LOGOUTPUT                        "LogOutPut"

// log file related
#define LR_LOG_FILE_COUNT_MIN                   3
#define LR_LOG_FILE_COUNT_DEFAULT               30
#define LR_LOG_FILE_COUNT_MAX                   100

#define LR_LOG_FILE_MAX_SIZE_MIN                (1024 * 1024)       // 1 MBytes
#define LR_LOG_FILE_MAX_SIZE_DEFAULT            (1024 * 1024 * 20)  // 20 Mbytes
#define LR_LOG_FILE_MAX_SIZE_MAX                (1024 * 1024 * 100) // 100 Mbytes

// option flags
#define LR_MODE_READ_CONFIG                     (1 << 0)  // read config
#define LR_MODE_WRITE_DIRECT                    (1 << 1)  // write directly, not use writing thread

#define LR_MODE_STDOUT                          (1 << 8)  // print standard out
#define LR_MODE_STDERR                          (1 << 9)  // print standard error
#define LR_MODE_WRITE_TO_FILE                   (1 << 10) // write to the log file
#define LR_MODE_SEND_TO_SYSLOG_SVR              (1 << 11) // (on the construction)
#define LR_MODE_SEND_SNMP_TRAP                  (1 << 12) // (on the construction)

#define LR_MODE_DEBUG                           (1 << 16) // enable debugging mode (stdout all log levels)
#define LR_MODE_RELOAD_CONFIG                   (1 << 17) // enable reload log config file by interval
#define LR_MODE_LOG_ROTATE                      (1 << 18) // enable log rotate (on the construction)
#define LR_MODE_COMPRESS_BACKUP                 (1 << 19) // compress backup file (on the construction)

#define LR_MODE_AUTO_LF                         (1 << 24) // auto LF (line feed - '\n' 0x0A) option 
#define LR_MODE_AUTH_CR                         (1 << 25) // auto CR (carriage return - '\r' 0x0D) option 

// log level
enum _LR_LEVEL
{
	LR_LOG_LEVEL_MESSAGE            = 0,
	LR_LOG_LEVEL_ERROR              = 0, 
	LR_LOG_LEVEL_WARNING,          // 1
	LR_LOG_LEVEL_INFORMATION,      // 2
	LR_LOG_LEVEL_DETAIL_INFO,      // 3
	LR_LOG_LEVEL_DEBUG_INFO,       // 4
};

#define LR_LOG_LEVEL_MESSAGE_STR      "[INF]"
#define LR_LOG_LEVEL_ERROR_STR        "[ERR]"
#define LR_LOG_LEVEL_WARNING_STR      "[WRN]"
#define LR_LOG_LEVEL_INFORMATION_STR  "[INF]"
#define LR_LOG_LEVEL_DEBUG_INFO_STR   "[DBG]"

// log output
enum _LR_OUTPUT
{
	LR_OUT_NOTHING                  = 0,
 	LR_OUT_DEBUGMSG,               // 1
	LR_OUT_FILE,                   // 2
	LR_OUT_CONSOLE,                // 3
};

// error code
enum _LR_ERR_NO
{
	LR_ERR_SUCCESS                  = 0,
	LR_ERR_EMPTY_MODULE_NAME,      // 1
	LR_ERR_EMPTY_CONFIG_PATH,      // 2
	LR_ERR_EMPTY_REG_ROOT,         // 3
	LR_ERR_EMPTY_REG_PATH,         // 4
	LR_ERR_EMPTY_LOG_PATH,         // 5
	LR_ERR_EMPTY_ARG_READ_CONF,    // 6
	LR_ERR_CONF_OPEN_FAILED,       // 7 
	LR_ERR_CONF_READ_KEY_FAILED,   // 8
	LR_ERR_EMPTY_ARG_WRITE_CONF,   // 9
	LR_ERR_CONF_WRITE_VAL_FAILED,  // 10
	LR_ERR_LOG_OPEN_FAILED,        // 11
	LR_ERR_LOG_SIZE_CHECK_REOPEN,  // 12
	LR_ERR_LOG_SIZE_CHECK_FATAL,   // 13
	LR_ERR_LOG_FILE_RENAME_FAILED, // 14
	LR_ERR_LOG_WRITE_FAILED,       // 15
	LR_ERR_LOG_FLUSH_FAILED,       // 16
	LR_ERR_MAYBE_HDD_FULL,         // 17
	LR_ERR_LOG_WRITE_TH_FAILED,    // 18
	LR_ERR_EMPTY_LOG_WRITE_TH_ARG, // 19
};

// variable definitions
// list structure for config writing operation --------------------------------
struct __wr_conf_str {
    char*                 strptr;
    struct __wr_conf_str* pnext;
};
typedef struct __wr_conf_str S_WR_CONF_STR, *PS_WR_CONF_STR;
//-----------------------------------------------------------------------------

// class definitions ----------------------------------------------------------
class CLogObj
{
public :
	void SetLog  (const char* sLog) { if(NULL != sLog) m_sLog = sLog; };
	void SetLevel(u_int nLevel)     { m_nLevel = nLevel; };

	const char* GetLog        () { return (const char*)m_sLog.data(); };
	const u_int GetLevel      () { return m_nLevel; };
	const char* GetTimeStrFull();
	const char* GetTimeStrAbbr();

	CLogObj() 
	{ 
		m_nLevel = 0; 
		time(&m_tLog); 
		clock_gettime(CLOCK_MONOTONIC, &m_tsLog); 
	};
	CLogObj(u_int nLevel, const char* sLog) 
	{ 
		SetLog(sLog); 
		SetLevel(nLevel); 
		time(&m_tLog); 
		clock_gettime(CLOCK_MONOTONIC, &m_tsLog); 
	};
	~CLogObj() {};

private :
	std::string     m_sLog;
	u_int           m_nLevel;
	time_t          m_tLog;
	struct timespec m_tsLog;
};

class CLogger 
{
public :
	static CLogger& GetInstance    ();
	static CLogger* GetInstancePtr ();
	static void     ReleaseInstance();

	void        Write                  (u_int nLevel, const char* comments, ...);
	bool        Run                    ();
	void        Sleep                  (int nSec, int nUSec);
	int         GetQueueCount          () { return m_qLog.size(); };
	CLogObj*    PopLog                 ();
	void        WriteLog               (CLogObj* pLog);
	bool        OpenLogFile            ();
	u_int       GetLastErrorNumber     () { return m_nLastError; };
	const char* GetLastErrorString     () { return (const char*)m_sLastError.data(); };
	void        SetMode                (u_int nMode)       { m_nMode = nMode; };
	u_int       GetMode                ()              { return m_nMode; };
	u_int       GetLogLevel            () { return m_nLogLevel; };
	bool        SetDefaultEnvironment  (const char* sModuleName);
	bool        SetModuleName          (const char* sName);
	const char* GetModuleName          ()              { return m_sModuleName.data(); };
	bool        SetConfigInfo          (const char* sPath,                  // 'LR_MODE_READ_CONFIG' is on. configure file path
                                        u_int nConfigReloadInterval = 0);   // 'LR_MODE_RELOAD_CONFIG' is on. configure reload interval
	const char* GetConfigPath          () { return m_sConfPath.data(); };
	void        SetConfigReloadInterval(u_int nVal) { m_nConfigReloadInterval = nVal; };
	u_int       GetConfigReloadInterval() { return m_nConfigReloadInterval; };
	void        SetConfigDir           (const char* sDir) { m_sConfDir = sDir; }
	bool        SetLogFileInfo         (const char* sPath,           // 'LR_MODE_WRITE_TO_FILE' is on. log file path
	                                    u_int       nLogFileMaxSize, // 'LR_MODE_WRITE_TO_FILE' is on. log file max. size
                                        u_int       nLogFileCount);  // 'LR_MODE_WRITE_TO_FILE' is on. maintain file count
	const char* GetLogFilePath         () { return m_sLogPath.data(); };
	u_int       GetLogFileMaxSize      () { return m_nLogFileMaxSize; };
	u_int       GetLogFileCount        () { return m_nLogFileCount; };
	void        SetLogDir              (const char* sDir) { m_sLogDir = sDir; }
	bool        ReadConfig             ();
	bool        IniConfReadValue       (const char* sFilename, const char* sSection, const char* sKey, char* sOutValue, int nBufLen);
	bool        IniConfWriteValue      (const char* sFilename, const char* sSection, const char* sKey, const char* sValue);
	void        CheckTimeAndUpdateConf ();

private :
	void Trim   (std::string& sStr);
	int  Replace(std::string& sStr, char cOld, char cNew);
	void Lock   ();
	void Unlock ();

	FILE* IniOpenFile        (const char* sFilename, const char* sMode);
	void  IniStrCpyLeft      (const char* sSrc,      char* sDst, char cFind, int nBufLen);
	void  IniConfWriteStrList(FILE*       pFile,     PS_WR_CONF_STR pStart);
	void  IniConfQueueStr    (PS_WR_CONF_STR pStart, const char* sData);

	time_t UpdateConfLoadTime();

	void PushLog         (u_int nLevel, const char* sLog);
	void WriteLog        (u_int nLevel, const char* sLog);
	void MakeLogLine     (const char* sTimeStamp, const char* sLog, std::string& sOut);
	bool WriteToLogFile  (const char* sLog);
	bool IsFileSizeOver  ();
	void CloseLogFile    ();
	void BackupOldLogFile();
	void CheckLogFile    ();
	int  FormatArgList   (std::string& sOut, int nLen, const char* formet, va_list args);
	void ResetError      ();
	bool IsFileExist     (const char* sFilename);
	bool cmd_system      (const char* scmd);
	void DeleteOldFiles  (const char* sDir, const char* sFind, u_int nRemain);

	static void* LogWriteThreadFunc(void* pArg);

	CLogger();
	~CLogger();

public :
	bool                 m_bRun;	
	pthread_t            m_ThreadID;

private :
	static CLogger*      m_pInstance;
	u_int                m_nMode;
	std::string          m_sModuleName;
	char                 m_sPid[20];

	u_int                m_nLogFileCount;
	u_int                m_nLogFileMaxSize;
	u_int                m_nLogLevel;
	u_int                m_nLogOutPut;

	std::string          m_sConfPath;
	std::string          m_sConfDir;
	u_int                m_nConfigReloadInterval;
	time_t               m_tLastLoaded;

	std::string          m_sLogPath;
	std::string          m_sLogDir;
	FILE*                m_pFile;

	std::deque<CLogObj*> m_qLog;

	u_int                m_nLastError;
	std::string          m_sLastError;

	u_int                m_nLoopCnt;
	pthread_mutex_t      m_Lock;
};

// external global log function -----------------------------------------------
/*
 * init.
 *
 * parameter : (const char*) MODULE_NAME
 *             (const char*) CONFIG_DIR
 *             (const char*) LOG_DIR
*/
#define _LOGGER_INIT_(MODULE_NAME, CONFIG_DIR, LOG_DIR) \
{ \
	CLogger::GetInstancePtr()->SetConfigDir(CONFIG_DIR); \
	CLogger::GetInstancePtr()->SetLogDir(LOG_DIR); \
	CLogger::GetInstancePtr()->SetMode(LR_MODE_READ_CONFIG | \
                                       LR_MODE_WRITE_TO_FILE | \
                                       LR_MODE_RELOAD_CONFIG | \
                                       LR_MODE_AUTO_LF); \
	CLogger::GetInstancePtr()->SetDefaultEnvironment(MODULE_NAME); \
	CLogger::GetInstancePtr()->ReadConfig(); \
	CLogger::GetInstancePtr()->OpenLogFile(); \
	CLogger::GetInstancePtr()->Run(); \
}

// detroy 
#define _LOGGER_BYE CLogger::ReleaseInstance()

/*
 * modify mode
 *
 * parameter : (unsigned int) MODE
 */
#define _LOGGER_SET_MODE(MODE) CLogger::GetInstancePtr()->SetMode(MODE)

/*
 * get mode
 *
 * return : (unsigned int) current mode
 */
#define _LOGGER_GET_MODE CLogger::GetInstancePtr()->GetMode()

/*
 * set debug mode
 */
#define _LOGGER_DEBUG \
{ \
	u_int nMode = CLogger::GetInstancePtr()->GetMode(); \
	if(!(LR_MODE_DEBUG & nMode)) \
	{ \
		nMode |= LR_MODE_DEBUG; \
		if(!(LR_MODE_STDOUT & nMode)) nMode |= LR_MODE_STDOUT; \
		if(!(LR_MODE_STDERR & nMode)) nMode |= LR_MODE_STDERR; \
		CLogger::GetInstancePtr()->SetMode(nMode); \
	} \
	CLogger::GetInstancePtr()->SetConfigReloadInterval(LR_CONF_RELOAD_INTERVAL_DEBUG); \
}

/*
 * set release mode
 */
#define _LOGGER_RELEASE \
{ \
	u_int nMode = CLogger::GetInstancePtr()->GetMode(); \
	if(LR_MODE_STDOUT & nMode) nMode ^= LR_MODE_STDOUT; \
	if(LR_MODE_STDERR & nMode) nMode ^= LR_MODE_STDERR; \
	CLogger::GetInstancePtr()->SetMode(nMode); \
}

/*
 * get current log level
 */
#define _LOGGER_GET_LOGLEVEL CLogger::GetInstancePtr()->GetLogLevel()

/*
 * write a log
 *
 * parameters :
 *    (unsigned int) LOG_LEVEL
 *    (varrable argument) LOG_FORMAT, ...
 */
#define _LOG(LOG_LEVEL, LOG_FORMAT, ... ) CLogger::GetInstancePtr()->Write(LOG_LEVEL, LOG_FORMAT, ##__VA_ARGS__)

#define _LOG_MSG(LOG_FORMAT, ... )  _LOG(LR_LOG_LEVEL_MESSAGE,     "%s(%s:%d:%s()) " LOG_FORMAT, LR_LOG_LEVEL_MESSAGE_STR,     __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define _LOG_ERR(LOG_FORMAT, ... )  _LOG(LR_LOG_LEVEL_ERROR,       "%s(%s:%d:%s()) " LOG_FORMAT, LR_LOG_LEVEL_ERROR_STR,       __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define _LOG_WRN(LOG_FORMAT, ... )  _LOG(LR_LOG_LEVEL_WARNING,     "%s(%s:%d:%s()) " LOG_FORMAT, LR_LOG_LEVEL_WARNING_STR,     __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define _LOG_INF(LOG_FORMAT, ... )  _LOG(LR_LOG_LEVEL_INFORMATION, "%s(%s:%d:%s()) " LOG_FORMAT, LR_LOG_LEVEL_INFORMATION_STR, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define _LOG_INF2(LOG_FORMAT, ... ) _LOG(LR_LOG_LEVEL_DETAIL_INFO, "%s(%s:%d:%s()) " LOG_FORMAT, LR_LOG_LEVEL_INFORMATION_STR, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define _LOG_DBG(LOG_FORMAT, ... )  _LOG(LR_LOG_LEVEL_DEBUG_INFO,  "%s(%s:%d:%s()) " LOG_FORMAT, LR_LOG_LEVEL_DEBUG_INFO_STR,  __FILE__, __LINE__, __func__, ##__VA_ARGS__)

/* 
 * error check
 *
 * get error string & error number
 */
#define _LOGGER_LAST_ERRSTR CLogger::GetInstancePtr()->GetLastErrorString()
#define _LOGGER_LAST_ERRNUM CLogger::GetInstancePtr()->GetLastErrorNumber()


//-----------------------------------------------------------------------------
#endif // __LOGGER_H__
 
