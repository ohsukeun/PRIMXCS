/*****************************************************************************
 *                                                                           *
 * File Name : logger.cpp                                                    *
 * Theme     : log writer                                                    *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '14. 04. 25                     *
 *                                                                           *
 *****************************************************************************/

/// @file logger.cpp
/// @author Park, sung soo
/// @date 2014-04-25
/// @brief log writer

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>

#include <algorithm>
#include <vector>

#include <logger.h>

#ifndef __LOGGER_CPP__
#define __LOGGER_CPP__

//-----------------------------------------------------------------------------
// LOCAL DEFINITIONS
//-----------------------------------------------------------------------------
// local variables 
#define LR_LOOP_CHECK_CNT       50
#define LR_INT_STR_BUF_LEN      20
#define LR_VA_ARG_BUF_LEN       1024
#define LR_DEFAULT_MAX_LINE_LEN 1024

// error strings
std::string _LR_ERR_STR[] = 
{
	"Successed.",                                        // LR_ERR_SUCCESS
	"Empty module name.",                                // LR_ERR_EMPTY_MODULE_NAME
	"Empty configure file path.",                        // LR_ERR_EMPTY_CONFIG_PATH
	"Empty registry root.",                              // LR_ERR_EMPTY_REG_ROOT
	"Empty registry path.",                              // LR_ERR_EMPTY_REG_PATH
	"Empty log file path.",                              // LR_ERR_EMPTY_LOG_PATH
	"Invalid function arg(s) for read configure file.",  // LR_ERR_EMPTY_ARG_READ_CONF
	"Configure file opening failed.",                    // LR_ERR_CONF_OPEN_FAILED
	"Key reading failed from the configure file.",       // LR_ERR_CONF_READ_KEY_FAILED
	"Invalid funcrion arg(s) for write configure file.", // LR_ERR_EMPTY_ARG_WRITE_CONF
	"Value writing failed to the configure file.",       // LR_ERR_CONF_WRITE_VAL_FAILED
	"Log file opening failed",                           // LR_ERR_LOG_OPEN_FAILED
	"Log file size check failed. retry if",              // LR_ERR_LOG_SIZE_CHECK_REOPEN
	"Log file size check failed. fatal error.",          // LR_ERR_LOG_SIZE_CHECK_FATAL
	"Log file rename failed.",                           // LR_ERR_LOG_FILE_RENAME_FAILED
	"Log writing failed.",                               // LR_ERR_LOG_WRITE_FAILED
	"Log string flushing failed,",                       // LR_ERR_LOG_FLUSH_FAILED
	"Maybe HDD full. Check HDD or system status.",       // LR_ERR_MAYBE_HDD_FULL
	"Log writting thread starting failed.",              // LR_ERR_LOG_WRITE_TH_FAILED
};

// init. sigleton object pointer
CLogger* CLogger::m_pInstance = NULL;

// exception handling definition
#define __CATCH__ \
	catch(std::exception& e) \
	{ \
		std::cerr << "exception caugh: " << e.what() << '\n'; \
	}

// log rotation related
typedef struct _LOG_FILE_INFO
{
	char   _name[NAME_MAX+1];
	time_t _ctime;
} LOG_FILE_INFO, *pLOG_FILE_INFO;
#define LOG_FILE_INFO_LEN    sizeof(LOG_FILE_INFO)

bool CompareObj(pLOG_FILE_INFO pprev, pLOG_FILE_INFO pnext)
{
	return (bool)(pprev->_ctime < pnext->_ctime);
}

//-----------------------------------------------------------------------------
// METHOD DEFINITIONS
//-----------------------------------------------------------------------------
CLogger& CLogger::GetInstance()
{
	if(NULL == m_pInstance) m_pInstance = new CLogger();
	return *m_pInstance;
}

CLogger* CLogger::GetInstancePtr()
{
	if(NULL == m_pInstance) m_pInstance = new CLogger();
	return m_pInstance;
}

void CLogger::ReleaseInstance() 
{ 
	if(NULL != m_pInstance) 
	{ 
		delete m_pInstance; 
		m_pInstance = NULL; 
	} 
}

/* ----------------------------------------------------------------------------
	description : creators & destoryers
	parameters  :
	return      : 
	history     :
		- 2014. 05. 02 created by Park. Sung soo
---------------------------------------------------------------------------- */
CLogger::CLogger()
{
	try
	{
		m_bRun                  = true;
		m_ThreadID              = 0;
		m_nMode                 = (LR_MODE_READ_CONFIG | LR_MODE_WRITE_TO_FILE | LR_MODE_RELOAD_CONFIG | LR_MODE_AUTO_LF);
		m_nLogFileCount         = LR_LOG_FILE_COUNT_DEFAULT;
		m_nLogFileMaxSize       = LR_LOG_FILE_MAX_SIZE_DEFAULT;
		m_nLogLevel             = LR_LOG_LEVEL_INFORMATION;
		m_nLogOutPut            = LR_OUT_FILE;
		m_nConfigReloadInterval = LR_CONF_RELOAD_INTERVAL_DEFAULT;
		UpdateConfLoadTime();
		m_pFile                 = NULL;
		m_nLoopCnt              = 0;
		m_nLastError            = LR_ERR_SUCCESS;

		pid_t nPid = getpid();
		if(0 < nPid)
		{
			memset(m_sPid, 0x00, 20);
			snprintf(m_sPid, 20, "%u", (u_int)nPid);
		}
		pthread_mutex_init(&m_Lock, NULL);
	}
	__CATCH__
}

CLogger::~CLogger()
{
	try
	{
		m_bRun = false;
		Sleep(0, 1000);
		CloseLogFile();
		if(0 != m_ThreadID)
		{
			long nStatus = 0;
			pthread_join(m_ThreadID, (void**)nStatus);
			m_ThreadID = 0;
		}
		while(0 < m_qLog.size())
		{
			CLogObj* pDel = m_qLog.front();
			m_qLog.pop_front();
			if(NULL != pDel) delete pDel;
		}
		pthread_mutex_destroy(&m_Lock);
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
	description : get time full string 'YYYY-mm-dd HH:MM:SS.milli-seconds'
	parameters  :
		- ()
	return      : (const char*) return time string
	history     :
		- 2014. 05. 02 created by Park. Sung soo
---------------------------------------------------------------------------- */
const char* CLogObj::GetTimeStrFull()
{
	static std::string sRet;
	sRet.clear();
	try
	{
		struct tm tLog;
		char sTime[30] = {0}, sMiliSec[10] = {0};

		memcpy(&tLog, localtime(&m_tLog), sizeof(struct tm));
		strftime(sTime, 30, "%Y-%m-%d %H:%M:%S", &tLog);
		snprintf(sMiliSec, 10, "%03d", (int)(m_tsLog.tv_nsec / 1000000));
		sRet =  '[';
		sRet += sTime;
		sRet += '.';
		sRet += sMiliSec;
		sRet += ']';
	}
	__CATCH__
	return (const char*)sRet.data();
}

/* ----------------------------------------------------------------------------
	description : get time abbreviated string 'HH:MM:SS.milli-seconds'
	parameters  :
		- ()
	return      : (const char*) return time string
	history     :
		- 2014. 05. 02 created by Park. Sung soo
---------------------------------------------------------------------------- */
const char* CLogObj::GetTimeStrAbbr()
{
	static std::string sRet;
	sRet.clear();
	try
	{	
		struct tm tLog;
		char sTime[30] = {0}, sMiliSec[10] = {0};

		memcpy(&tLog, localtime(&m_tLog), sizeof(struct tm));
		strftime(sTime, 30, "%H:%M:%S", &tLog);
		snprintf(sMiliSec, 10, "%03d", (int)(m_tsLog.tv_nsec / 1000000));
		sRet =  '[';
		sRet += sTime;
		sRet += '.';
		sRet += sMiliSec;
		sRet += ']';
	}
	__CATCH__
	return (const char*)sRet.data();
}

/* ----------------------------------------------------------------------------
	description : reset error string and error number
	parameters  :
		- ()
	return      : ()
	history     :
		- 2014. 08. 19 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::ResetError()
{
	if(LR_ERR_SUCCESS != m_nLastError)
	{
		m_nLastError = LR_ERR_SUCCESS;
		m_sLastError = _LR_ERR_STR[m_nLastError];
	}
}

/* ----------------------------------------------------------------------------
	description : replace char from old to new one in a string
	parameters  :
		- (std::string&) target string
		- (char)         from char
		- (char)         to char
	return      : (int) replaced char count
	history     :
		- 2014. 05. 02 created by Park. Sung soo
---------------------------------------------------------------------------- */
int CLogger::Replace(std::string& sStr, char cOld, char cNew)
{
	bool bFind = false;
	try
	{
		int nRet = 0;

		do
		{
			size_t nFind = 0;
			if(sStr.npos != (nFind = sStr.find(cOld)))
			{
				bFind = true;
				sStr.replace(nFind, 1, 1, cNew);
				nRet++;
			}
			else bFind = false;
		} while (true == bFind);
	}
	__CATCH__
	return bFind;
};

/* ----------------------------------------------------------------------------
	description : trim a string
	parameters  :
		- (std::string&) target string
	return      : ()
	history     :
		- 2014. 05. 02 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::Trim(std::string& sStr)
{
	try
	{
		if(true == sStr.empty()) return;

		Replace(sStr, '\r', ' ');
		Replace(sStr, '\n', ' ');

		// trim left
		size_t nLeftFind = 0;
		nLeftFind = sStr.find_first_not_of(' ');
		if(nLeftFind != sStr.npos) sStr.erase(0, nLeftFind);
		else                       sStr.erase(sStr.begin(), sStr.end());

		// trim right
		size_t nRightFind = 0;
		nRightFind = sStr.find_last_not_of(' ');
		if(nRightFind != sStr.npos) sStr.erase(nRightFind+1);
		else                        sStr.erase(sStr.begin(), sStr.end());
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
	description : set default environmental variables
	parameters  :
		- (const char*) module name string
	return      : (bool) return result
	history     :
		- 2014. 05. 12 created by Park. Sung soo
---------------------------------------------------------------------------- */
bool CLogger::SetDefaultEnvironment(const char* sModuleName)
{
	bool bRet = true;
	try
	{
		if(false == SetModuleName(sModuleName)) return false;
		std::string sLogPath;
		std::string sConfPath;

		sConfPath  = m_sConfDir;;
		sConfPath += '/';
		sConfPath += m_sModuleName;
		sConfPath += ".conf";
		sLogPath   = m_sLogDir;
		sLogPath  += '/';
		sLogPath  += m_sModuleName; 
		sLogPath  += ".log";
		if(false == SetConfigInfo(sConfPath.data(), LR_CONF_RELOAD_INTERVAL_DEFAULT)) return false;
		if(false == SetLogFileInfo(sLogPath.data(), LR_LOG_FILE_MAX_SIZE_DEFAULT, LR_LOG_FILE_COUNT_DEFAULT)) return false; 
	}
	__CATCH__
	return bRet;
}

/* ----------------------------------------------------------------------------
	description : set module name
	parameters  :
		- (const char*) module name string
	return      : (bool) return result
	history     :
		- 2014. 05. 02 created by Park. Sung soo
---------------------------------------------------------------------------- */
bool CLogger::SetModuleName(const char* sName)
{
	bool bRet = true;
	try
	{
		if(NULL == sName) throw LR_ERR_EMPTY_MODULE_NAME;
		m_sModuleName = sName;
		if(true == m_sModuleName.empty()) throw LR_ERR_EMPTY_MODULE_NAME;
		Trim(m_sModuleName);
		if(true == m_sModuleName.empty()) throw LR_ERR_EMPTY_MODULE_NAME;
		ResetError();
	}
	catch(_LR_ERR_NO exception)
	{
		bRet = false;
		m_nLastError = exception;
		m_sLastError = _LR_ERR_STR[m_nLastError];
	}
	__CATCH__
	return bRet;
}

/* ----------------------------------------------------------------------------
	description : set config. file or registry info.
	parameters  :
		if LR_OS_UNIT 
		- (const char*) configure file path
		- (u_int)       configure reload interval time (seconds) if zero, not reload.
		else if LR_OS_WINDOWS
		- (const char*) configure registry root path
		- (const char*) configure registry sub path
	return      : (bool) return result
	history     :
		- 2014. 05. 12 created by Park. Sung soo
---------------------------------------------------------------------------- */
bool CLogger::SetConfigInfo(const char* sPath, u_int nConfigReloadInterval)
{
	bool bRet = true;
	try
	{
		if(NULL == sPath) throw LR_ERR_EMPTY_CONFIG_PATH;
		m_sConfPath = sPath;
		if(LR_MODE_RELOAD_CONFIG & m_nMode)
		{
			if(LR_MODE_DEBUG & m_nMode) m_nConfigReloadInterval = LR_CONF_RELOAD_INTERVAL_DEBUG;
			else
			{
				if(0 == nConfigReloadInterval) m_nConfigReloadInterval = LR_CONF_RELOAD_INTERVAL_DEFAULT;
				else                           m_nConfigReloadInterval = nConfigReloadInterval;
			}
		}
		ResetError();
	}
	catch(_LR_ERR_NO exception)
	{
		bRet = false;
		m_nLastError = exception;
		m_sLastError = _LR_ERR_STR[m_nLastError];
	}
	__CATCH__
	return bRet;
}

/* ----------------------------------------------------------------------------
	description : set log file info.
	parameters  :
		- (const char*) target log file path
		- (u_int)       max. log file size
		- (u_int)       maintain file count
	return      : (bool) return result
	history     :
		- 2014. 05. 12 created by Park. Sung soo
---------------------------------------------------------------------------- */
bool CLogger::SetLogFileInfo(const char* sPath, u_int nLogFileMaxSize, u_int nLogFileCount)
{
	bool bRet = true;
	try
	{
		if(NULL == sPath) throw LR_ERR_EMPTY_LOG_PATH;
		m_sLogPath           = sPath;
		m_nLogFileMaxSize    = nLogFileMaxSize;
		m_nLogFileCount      = nLogFileCount;
		ResetError();
	}
	catch(_LR_ERR_NO exception)
	{
		bRet = false;
		m_nLastError = exception;
		m_sLastError = _LR_ERR_STR[m_nLastError];
	}
	__CATCH__
	return bRet;
}

/* ----------------------------------------------------------------------------
	description : read from config file
	parameters  :
		- (const char*) target config file name
		- (const char*) section name
		- (const char*) key name
		- (char*)       out buffer
		- (int)         out buffer length
	return      : (bool) return result
	history     :
		- 2014. 05. 09 created by Park. Sung soo
---------------------------------------------------------------------------- */
bool CLogger::IniConfReadValue(const char* sFilename,
                               const char* sSection,
                               const char* sKey,
                               char*       sOutValue,
                               int         nBufLen)
{
	bool  bRet = true;
	FILE* pFile = NULL;

	try
	{
		bool bFind = false;
		char sLineBuf[LR_DEFAULT_MAX_LINE_LEN] = {0}, _stmp[1024] = {0}, _ssKey[1024] = {0};

		if((NULL == sFilename) || (NULL == sSection) || (NULL == sKey) || (NULL == sOutValue)) throw LR_ERR_EMPTY_ARG_READ_CONF;
		if(0 == nBufLen) throw LR_ERR_EMPTY_ARG_READ_CONF;
		if(NULL == (pFile = IniOpenFile(sFilename, "r"))) throw LR_ERR_CONF_OPEN_FAILED;

		snprintf(_stmp, 1024, "[%s]", sSection);
		while(NULL != fgets(sLineBuf, LR_DEFAULT_MAX_LINE_LEN, pFile))
		{
			if(NULL != strchr(sLineBuf, '#')) continue;
			if(NULL != strrchr(sLineBuf, '['))
			{
				if(NULL != strstr(sLineBuf, _stmp))
				{
					memset(sLineBuf, 0x00, LR_DEFAULT_MAX_LINE_LEN);
					while(NULL != fgets(sLineBuf, LR_DEFAULT_MAX_LINE_LEN, pFile))
					{
						snprintf(_ssKey, 256, "%s=", sKey);
						if(NULL != strchr(sLineBuf, '#')) continue;
						if(NULL != strstr(sLineBuf, _ssKey))
						{
							sLineBuf[strlen(sLineBuf)-1] = '\0';
							memset(sOutValue, 0x00, nBufLen);
							IniStrCpyLeft(sLineBuf, sOutValue, '=', nBufLen);
							bFind = true;
							break;
						}
					}
					break;
				}
			}
		}
		if(NULL != pFile) { fclose(pFile); pFile = NULL; }
		if(false == bFind) throw LR_ERR_CONF_READ_KEY_FAILED;
		ResetError();
	}
	catch(_LR_ERR_NO exception)
	{
		if(NULL != pFile) { fclose(pFile); pFile = NULL; }
		bRet = false;
		m_nLastError = exception;
		m_sLastError = _LR_ERR_STR[m_nLastError];
		if(LR_ERR_CONF_OPEN_FAILED == exception)
		{
			m_sLastError += " file=";
			m_sLastError += sFilename;
		}
		else
		{
			m_sLastError += " section=";
			m_sLastError += sSection;
			m_sLastError += " key=";
			m_sLastError += sKey;
		}
	}
	__CATCH__
	return bRet;
}

/* ----------------------------------------------------------------------------
    description : write to config file. if file does not exists, create one
    parameters  :
        - (const char*) file name
        - (const char*) section name
        - (const char*) key name
        - (const char*) value
    return      : (bool) return result 
    history     :
        - 2014. 05. 09 created by Park. Sung soo
---------------------------------------------------------------------------- */
bool CLogger::IniConfWriteValue(const char* sFilename,
                                const char* sSection,
                                const char* sKey,
                                const char* sValue)
{
	bool  bRet = true;
	FILE* pFile = NULL;

	try
	{
		char sLineBuf[LR_DEFAULT_MAX_LINE_LEN] = {0}, sec_buf[1024] = {0}, val_buf[1024] = {0};
		char _stmp[1024] = {0}, _ssKey[1024] = {0};
		bool b_sec_find = false, b_key_find = false, b_key_written = false;
		S_WR_CONF_STR str_snode;

		if((NULL == sFilename) || (NULL == sSection) || (NULL == sKey) || (NULL == sValue)) throw LR_ERR_EMPTY_ARG_WRITE_CONF;
		if(NULL == (pFile = IniOpenFile(sFilename, "r"))) throw LR_ERR_CONF_OPEN_FAILED;

		str_snode.strptr = NULL;
		str_snode.pnext = NULL;
		snprintf(_stmp, 1024, "[%s]", sSection);

		while(NULL != fgets(sLineBuf, LR_DEFAULT_MAX_LINE_LEN, pFile))
		{
			IniConfQueueStr(&str_snode, sLineBuf);

			if(NULL != strchr(sLineBuf, '#')) continue;
			else if(NULL != strrchr(sLineBuf, '['))
			{
				if(NULL != strstr(sLineBuf, _stmp))
				{
					memset(sLineBuf, 0x00, LR_DEFAULT_MAX_LINE_LEN);
					while(NULL != fgets(sLineBuf, LR_DEFAULT_MAX_LINE_LEN, pFile))
					{
						snprintf(_ssKey, 256, "%s=", sKey);
						if(NULL != strstr(sLineBuf, _ssKey))
						{
							memset(val_buf, 0x00, 1024);
							sprintf(val_buf, "%s=%s\n", sKey, sValue);
							IniConfQueueStr(&str_snode, val_buf);
							b_key_find    = true;
							b_key_written = true;
						}
						else if(NULL != strrchr(sLineBuf, '['))
						{
							if(false == b_key_find)
							{
								memset(val_buf, 0x00, 1024);
								sprintf(val_buf, "%s=%s\n", sKey, sValue);
								IniConfQueueStr(&str_snode, val_buf);
								b_key_written = true;
							}
							IniConfQueueStr(&str_snode, sLineBuf);
							break;
						}
						else
						{
							IniConfQueueStr(&str_snode, sLineBuf);
						}
					}
					b_sec_find = true;
				}
			}
		}
		if(NULL != pFile) { fclose(pFile); pFile = NULL; }

		if(false == b_sec_find)
		{
			// write sSection name
			memset(sec_buf, 0x00, 1024);
			sprintf(sec_buf, "[%s]\n", sSection);
			IniConfQueueStr(&str_snode, sec_buf);

			// write sValue
			memset(val_buf, 0x00, 1024);
			sprintf(val_buf, "%s=%s\n", sKey, sValue);
			IniConfQueueStr(&str_snode, val_buf);
		}
		else if(false == b_key_written)
		{
			memset(val_buf, 0x00, 1024);
			sprintf(val_buf, "%s=%s\n", sKey, sValue);
			IniConfQueueStr(&str_snode, val_buf);
		}

		if(NULL == (pFile = IniOpenFile(sFilename, "w+"))) throw LR_ERR_CONF_WRITE_VAL_FAILED;
		IniConfWriteStrList(pFile, &str_snode);
		if(NULL != pFile) { fclose(pFile); pFile = NULL; }
		ResetError();
	}
	catch(_LR_ERR_NO exception)
	{
		if(NULL != pFile) { fclose(pFile); pFile = NULL; }
		bRet = false;
		m_nLastError = exception;
		m_sLastError = _LR_ERR_STR[m_nLastError];
		if(LR_ERR_CONF_OPEN_FAILED == exception)
		{
			m_sLastError += " file=";
			m_sLastError += sFilename;
		}
		else
		{
			m_sLastError += " section=";
			m_sLastError += sSection;
			m_sLastError += " key=";
			m_sLastError += sKey;
		}
	}
	__CATCH__
	return bRet;
}

/* ----------------------------------------------------------------------------
	description : file open given file path and open-mode
	parameters  :
		- (const char*) target file path (name)
		- (const char*) file open mode
	return      : (FILE*) file descriptor. if NULL, file open failed
	history     :
		- 2014. 05. 09 created by Park. Sung soo
---------------------------------------------------------------------------- */
FILE* CLogger::IniOpenFile(const char* sFilename, const char* sMode)
{
	FILE* pRet = NULL;
	try
	{	
		if((NULL == sFilename) || (NULL == sMode)) return pRet;
		if((0 == strlen(sFilename)) || (0 == strlen(sMode))) return pRet;

		if(NULL == (pRet = fopen(sFilename, sMode))) return pRet;
	}
	__CATCH__
	return pRet;
}

/* ----------------------------------------------------------------------------
	description : copy left string given previous character
	parameters  :
		- (const char*) source string
		- (char*)       copy buffer
		- (char)        previous character
		- (int)         copy buffer length 
	return      : ()
	history     :
		- 2014. 05. 09 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::IniStrCpyLeft(const char* sSrc, char* sDst, char cFind, int nBufLen)
{
	try
	{
		char *pcFind;
		int  ncFind = 0;

		pcFind = (char*)strchr(sSrc, cFind);
		ncFind = pcFind - sSrc + 1;
		strncat(sDst, sSrc+ncFind, nBufLen);
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
	description : queuing strings to a list
	parameters  :
		- (PS_WR_CONF_STR) string list pointer
		- (const char*)    file open mode
	return      : ()
	history     :
		- 2014. 05. 09 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::IniConfQueueStr(PS_WR_CONF_STR pStart, const char* sData)
{
	try
	{
		if(NULL == pStart->pnext)
		{
			PS_WR_CONF_STR pnode = (PS_WR_CONF_STR)malloc(sizeof(S_WR_CONF_STR));
			if(NULL != pnode)
			{
				char* strnode = (char*)malloc(strlen(sData)+1);
				if(NULL != strnode)
				{
					sprintf(strnode, sData);
					pnode->strptr = strnode;
					pnode->pnext =NULL;
					pStart->pnext = pnode;
				}
			}
		}
		else
		{
			PS_WR_CONF_STR pcurnode = pStart->pnext;

			while(1)
			{
				if(NULL == pcurnode->pnext)
				{
					PS_WR_CONF_STR pnode = (PS_WR_CONF_STR)malloc(sizeof(S_WR_CONF_STR));
					char* strnode = (char*)malloc(strlen(sData)+1);
					if((NULL != strnode) && (NULL != pnode))
					{
						sprintf(strnode, sData);
						pnode->strptr = strnode;
						pnode->pnext =NULL;
						pcurnode->pnext = pnode;
					}
					break;
				}
				pcurnode = pcurnode->pnext;
			}
		}
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
	description : write queued string to file
	parameters  :
		- (FILE*)          target file descriptor
		- (PS_WR_CONF_STR) string list pointer
	return      : ()
	history     :
		- 2006. 12. 08 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::IniConfWriteStrList(FILE* pFile, PS_WR_CONF_STR pStart)
{
	try
	{
		PS_WR_CONF_STR pos = NULL, ppos = NULL;

		for(pos = pStart->pnext; pos != NULL; )
		{
			ppos = pos;
			pos = pos->pnext;

			if(NULL != ppos)
			{
				if(NULL != ppos->strptr)
				{
					fprintf(pFile, "%s", ppos->strptr);
					fflush(pFile);
					free((char*)ppos->strptr);
				}
				free((PS_WR_CONF_STR)ppos);
			}
		}
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
	description : lock
	parameters  :
		- ()
	return      : ()
	history     :
		- 2014. 05. 12 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::Lock()
{
	pthread_mutex_lock(&m_Lock);
}

/* ----------------------------------------------------------------------------
	description : unlock
	parameters  :
		- ()
	return      : ()
	history     :
		- 2014. 05. 12 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::Unlock()
{
	pthread_mutex_unlock(&m_Lock);
}

/* ----------------------------------------------------------------------------
	description : read config. from config file
	parameters  :
		- ()
	return      : ()
	history     :
		- 2014. 05. 12 created by Park. Sung soo
---------------------------------------------------------------------------- */
bool CLogger::ReadConfig()
{
	bool bRet = true;
	try
	{
		char sFileCount[LR_INT_STR_BUF_LEN] = {0}, sFileMaxSize[LR_INT_STR_BUF_LEN] = {0};
		char sLogLevel[LR_INT_STR_BUF_LEN]  = {0}, sLogOutPut[LR_INT_STR_BUF_LEN]   = {0};

		// read from config. file
		if(false == IniConfReadValue(m_sConfPath.data(), LR_SECTION_LOGOPTION, LR_KEY_LOGFILECOUNT, sFileCount, LR_INT_STR_BUF_LEN)) 
		{
			bRet = false; 
			m_nLogFileCount = LR_LOG_FILE_COUNT_DEFAULT; 
		}
		else 
		{
			m_nLogFileCount = (u_int)atoi(sFileCount);
			if(LR_LOG_FILE_COUNT_MAX < m_nLogFileCount) m_nLogFileCount = LR_LOG_FILE_COUNT_DEFAULT;
		}
		if(false == IniConfReadValue(m_sConfPath.data(), LR_SECTION_LOGOPTION, LR_KEY_LOGFILEMAXSIZE, sFileMaxSize, LR_INT_STR_BUF_LEN)) 
		{ 
			bRet = false; 
			m_nLogFileMaxSize = LR_LOG_FILE_MAX_SIZE_DEFAULT; 
		}
		else 
		{
			m_nLogFileMaxSize = ((u_int)atoi(sFileMaxSize) * (1024 * 1024));
			if(LR_LOG_FILE_MAX_SIZE_MAX < m_nLogFileMaxSize) m_nLogFileMaxSize = LR_LOG_FILE_MAX_SIZE_DEFAULT;
		}
		if(false == IniConfReadValue(m_sConfPath.data(), LR_SECTION_LOGOPTION, LR_KEY_LOGLEVEL, sLogLevel, LR_INT_STR_BUF_LEN)) 
		{ 
			bRet = false; 
			m_nLogLevel = LR_LOG_LEVEL_INFORMATION; 
		}
		else
		{
			m_nLogLevel = (u_int)atoi(sLogLevel);
			if(LR_LOG_LEVEL_DEBUG_INFO < m_nLogLevel) m_nLogLevel = LR_LOG_LEVEL_INFORMATION;
		}
		if(false == IniConfReadValue(m_sConfPath.data(), LR_SECTION_LOGOPTION, LR_KEY_LOGOUTPUT, sLogOutPut, LR_INT_STR_BUF_LEN))
		{ 
			bRet = false; 
			m_nLogOutPut = LR_OUT_FILE; 
		}
		else 
		{
			m_nLogOutPut = (u_int)atoi(sLogOutPut);
			if(LR_OUT_CONSOLE < m_nLogOutPut) m_nLogOutPut = LR_OUT_FILE;
		}

		// check mode by file count
		if(0 < m_nLogFileCount)
		{
			if(!(LR_MODE_LOG_ROTATE & m_nMode)) m_nMode |= LR_MODE_LOG_ROTATE;
		}
		else if(0 == m_nLogFileCount)
		{
			if(LR_MODE_LOG_ROTATE & m_nMode) m_nMode ^= LR_MODE_LOG_ROTATE;
		}

		// check mode by output 
		switch(m_nLogOutPut)
		{
			case LR_OUT_NOTHING :
				{
					if(LR_MODE_STDOUT             & m_nMode) m_nMode ^= LR_MODE_STDOUT;
					if(LR_MODE_STDERR             & m_nMode) m_nMode ^= LR_MODE_STDERR;
					if(LR_MODE_WRITE_TO_FILE      & m_nMode) m_nMode ^= LR_MODE_WRITE_TO_FILE;
					if(LR_MODE_SEND_TO_SYSLOG_SVR & m_nMode) m_nMode ^= LR_MODE_SEND_TO_SYSLOG_SVR;
					if(LR_MODE_SEND_SNMP_TRAP     & m_nMode) m_nMode ^= LR_MODE_SEND_SNMP_TRAP;
				}
				break;
			case LR_OUT_DEBUGMSG :
				{
					if(!(LR_MODE_STDOUT        & m_nMode)) m_nMode |= LR_MODE_STDOUT;
					if(!(LR_MODE_STDERR        & m_nMode)) m_nMode |= LR_MODE_STDERR;
					if(!(LR_MODE_WRITE_TO_FILE & m_nMode)) m_nMode |= LR_MODE_WRITE_TO_FILE;

					if(LR_MODE_SEND_TO_SYSLOG_SVR & m_nMode) m_nMode ^= LR_MODE_SEND_TO_SYSLOG_SVR;
					if(LR_MODE_SEND_SNMP_TRAP     & m_nMode) m_nMode ^= LR_MODE_SEND_SNMP_TRAP;

					if(!(LR_MODE_DEBUG & m_nMode)) m_nMode |= LR_MODE_DEBUG;
				}
				break;
			case LR_OUT_FILE :
				{
					if(LR_MODE_DEBUG & m_nMode)
					{
						if(!(LR_MODE_STDOUT & m_nMode)) m_nMode |= LR_MODE_STDOUT;
						if(!(LR_MODE_STDERR & m_nMode)) m_nMode |= LR_MODE_STDERR;
					}
					if(!(LR_MODE_WRITE_TO_FILE & m_nMode)) m_nMode |= LR_MODE_WRITE_TO_FILE;

					if(!(LR_MODE_DEBUG & m_nMode))
					{
						if(LR_MODE_STDOUT & m_nMode) m_nMode ^= LR_MODE_STDOUT;
						if(LR_MODE_STDERR & m_nMode) m_nMode ^= LR_MODE_STDERR;
					}
					if(LR_MODE_SEND_TO_SYSLOG_SVR & m_nMode) m_nMode ^= LR_MODE_SEND_TO_SYSLOG_SVR;
					if(LR_MODE_SEND_SNMP_TRAP     & m_nMode) m_nMode ^= LR_MODE_SEND_SNMP_TRAP;
				}
				break;
			case LR_OUT_CONSOLE :
				{
					if(!(LR_MODE_STDOUT & m_nMode)) m_nMode |= LR_MODE_STDOUT;
					if(!(LR_MODE_STDERR & m_nMode)) m_nMode |= LR_MODE_STDERR;

					if(LR_MODE_WRITE_TO_FILE      & m_nMode) m_nMode ^= LR_MODE_WRITE_TO_FILE;
					if(LR_MODE_SEND_TO_SYSLOG_SVR & m_nMode) m_nMode ^= LR_MODE_SEND_TO_SYSLOG_SVR;
					if(LR_MODE_SEND_SNMP_TRAP     & m_nMode) m_nMode ^= LR_MODE_SEND_SNMP_TRAP;
				}
				break;
			default : break;
		}
		UpdateConfLoadTime();
		//if(LR_MODE_DEBUG & m_nMode) fprintf(stdout, "log level : %u\n", m_nLogLevel);
	}
	__CATCH__
	return bRet;
}

/* ----------------------------------------------------------------------------
	description : open log file
	parameters  :
		- ()
	return      : (bool) return result
	history     :
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
bool CLogger::OpenLogFile()
{
	bool bRet = true;
	try
	{
		if(true == m_sLogPath.empty()) throw LR_ERR_EMPTY_LOG_PATH;
		if(NULL == m_pFile) 
		{
			if(NULL == (m_pFile = fopen((const char*)m_sLogPath.data(), "a+"))) throw LR_ERR_LOG_OPEN_FAILED;
			else DeleteOldFiles(m_sLogDir.c_str(), "log.gz", m_nLogFileCount);
		}
		ResetError();
	}
	catch(_LR_ERR_NO exception)
	{
		bRet = false;
		m_nLastError = exception;
		m_sLastError = _LR_ERR_STR[m_nLastError];
		if(LR_ERR_LOG_OPEN_FAILED == exception)
		{
			m_sLastError += " file=";
			m_sLastError += m_sLogPath;
			m_sLastError += " native-error=";
			m_sLastError += strerror(errno);
		}
	}
	__CATCH__
	return bRet;
}

/* ----------------------------------------------------------------------------
	description : close log file
	parameters  :
		- ()
	return      : ()
	history     :
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::CloseLogFile()
{
	try
	{
		if(NULL != m_pFile)
		{
			fflush(m_pFile);
			fclose(m_pFile);
			m_pFile = NULL;
		}	
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
	description : check file size over
	parameters  :
		- ()
	return      : (bool)
	history     :
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
bool CLogger::IsFileSizeOver()
{
	bool bRet = false;
	try
	{
		struct stat st;

		if(true == m_sLogPath.empty()) return false;
		memset(&st, 0x00, sizeof(struct stat));
		if(0 != stat((const char*)m_sLogPath.data(), &st))
		{
			if((EBADF == errno) || (EFAULT == errno) || (ENOENT == errno)) throw LR_ERR_LOG_SIZE_CHECK_REOPEN;
			else                                                           throw LR_ERR_LOG_SIZE_CHECK_FATAL;
		}
		if(m_nLogFileMaxSize < (u_int)st.st_size) bRet = true;
		ResetError();
	}
	catch(_LR_ERR_NO exception)
	{
		m_nLastError = exception;
		m_sLastError = _LR_ERR_STR[m_nLastError];
		m_sLastError += " file=";
		m_sLastError += m_sLogPath;
		m_sLastError += " native-error=";
		m_sLastError += strerror(errno);
	}
	__CATCH__
	return bRet;
}

/* ----------------------------------------------------------------------------
	description : push log to the queue
	parameters  :
		- (u_int)       log level
		- (const char*) log string
	return      : ()
	history     :
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::PushLog(u_int nLevel, const char* sLog)
{
	if(NULL == sLog) return;
	CLogObj* pLog = new CLogObj(nLevel, sLog);

	Lock();
	try
	{
		u_int nQCnt = 0;
		nQCnt = m_qLog.size();
		if(nQCnt < LR_MAX_LOG_QUEUE_COUNT) m_qLog.push_back(pLog);
		else if(nQCnt >= LR_MAX_LOG_QUEUE_COUNT)
		{
			m_qLog.pop_front();
			m_qLog.push_back(pLog);
		}
	}
	__CATCH__
	Unlock();
}

/* ----------------------------------------------------------------------------
	description : pop log from the queue
	parameters  :
		- ()
	return      : (CLogObj*) return log object
	history     :
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
CLogObj* CLogger::PopLog()
{
	CLogObj* pRet = NULL;
	Lock();
	try
	{
		if(0 < m_qLog.size())
		{
			pRet = m_qLog.front();
			m_qLog.pop_front();
		}
	}
	__CATCH__
	Unlock();
	return pRet;
}

/* ----------------------------------------------------------------------------
	description : write log (stdout, stderr or file)
	parameters  :
		- (u_int)       log level
		- (const char*) log string
	return      : ()
	history     :
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::WriteLog(u_int nLevel, const char* sLog)
{
	try
	{
		if(!(LR_MODE_DEBUG & m_nMode)) 
		{
			if(m_nLogLevel < nLevel) return;
		}

		CLogObj CurLog(nLevel, sLog);
		std::string sToStd, sToFile;

		if((LR_MODE_STDOUT & m_nMode) || (LR_MODE_STDERR & m_nMode))
		{
			std::string sTimeStamp(CurLog.GetTimeStrAbbr());
			MakeLogLine((const char*)sTimeStamp.data(), sLog, sToStd);

			if     (LR_MODE_STDOUT & m_nMode) fprintf(stdout, "%s", sToStd.data());
			else if(LR_MODE_STDERR & m_nMode) fprintf(stderr, "%s", sToStd.data());
		}
		if(LR_MODE_DEBUG & m_nMode)
		{
			if(m_nLogLevel < nLevel) return;
		}
		if(LR_MODE_WRITE_TO_FILE & m_nMode)
		{
			std::string sTimeStamp(CurLog.GetTimeStrFull());
			MakeLogLine((const char*)sTimeStamp.data(), sLog, sToFile);

			WriteToLogFile((const char*)sToFile.data());
			CheckLogFile();
		}
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
	description : write log (stdout, stderr or file)
	parameters  :
		- (CLogObj*) log object pointer
	return      : ()
	history     :
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::WriteLog(CLogObj* pLog)
{
	try
	{
		if(NULL == pLog) return;
		if(!(LR_MODE_DEBUG & m_nMode)) 
		{
			if(m_nLogLevel < pLog->GetLevel()) return;
		}

		std::string sToStd, sToFile;

		if((LR_MODE_STDOUT & m_nMode) || (LR_MODE_STDERR & m_nMode))
		{
			std::string sTimeStamp(pLog->GetTimeStrAbbr());
			MakeLogLine((const char*)sTimeStamp.data(), pLog->GetLog(), sToStd);

			if     (LR_MODE_STDOUT & m_nMode) fprintf(stdout, "%s", sToStd.data());
			else if(LR_MODE_STDERR & m_nMode) fprintf(stderr, "%s", sToStd.data());
		}
		if(LR_MODE_DEBUG & m_nMode)
		{
			if(m_nLogLevel < pLog->GetLevel()) return;
		}
		if(LR_MODE_WRITE_TO_FILE & m_nMode)
		{
			std::string sTimeStamp(pLog->GetTimeStrFull());
			MakeLogLine((const char*)sTimeStamp.data(), pLog->GetLog(), sToFile);

			WriteToLogFile((const char*)sToFile.data());
			CheckLogFile();
		}
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
	description : make one log string (line)
	parameters  :
		- (const char*)  timestamp string
		- (const char*)  target log string
		- (std::string&) out full log string
	return      : (CLogObj*) return log object
	history     :
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::MakeLogLine(const char* sTimeStamp, const char* sLog, std::string& sOut)
{
	try
	{
		sOut  = sTimeStamp;
		sOut += '(';
		sOut += m_sPid;
		sOut += ')';
		sOut += ' ';
		sOut += sLog;
		if(LR_MODE_AUTH_CR & m_nMode) sOut += '\r';
		if(LR_MODE_AUTO_LF & m_nMode) sOut += '\n';
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
	description : check log file size. if size over, backup and reopen
	parameters  :
		- ()
	return      : ()
	history     :
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::CheckLogFile()
{
	try
	{
		if(LR_MODE_LOG_ROTATE & m_nMode) 
		{
			if(LR_LOOP_CHECK_CNT <= m_nLoopCnt)
			{
				m_nLoopCnt = 0;
				if(true == IsFileSizeOver())
				{
					CloseLogFile();
					BackupOldLogFile();
					OpenLogFile();
					DeleteOldFiles(m_sLogDir.c_str(), "log.gz", m_nLogFileCount);
				}
			}
			else m_nLoopCnt++;
		}
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
	description : backup log file with backup timestamp
	parameters  :
		- ()
	return      : ()
	history     :
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::BackupOldLogFile()
{
	try
	{
		// make time string
		time_t tCur = 0;
		struct tm tCurTm;
		char  sTime[30] = {0};

		time(&tCur);
		memcpy(&tCurTm, localtime(&tCur), sizeof(struct tm));
		strftime(sTime, 30, "%Y%m%d_%H%M%S", &tCurTm);

		// make backup file path
		std::string sNewPath;
		sNewPath  = m_sLogDir;
		sNewPath += '/';
		sNewPath += m_sModuleName;
		sNewPath += '_';
		sNewPath += sTime;
		sNewPath += ".log";
		
		// rename file path and gzip file
		try 
		{
			if(0 != rename((const char*)m_sLogPath.data(), (const char*)sNewPath.data())) throw LR_ERR_LOG_FILE_RENAME_FAILED;

			std::string sSysCmd;

			sSysCmd = "gzip ";
			sSysCmd += sNewPath;
			cmd_system(sSysCmd.data());

			ResetError();
		}
		catch(_LR_ERR_NO exception)
		{
			m_nLastError = exception;
			m_sLastError = _LR_ERR_STR[m_nLastError];
			m_sLastError += " old path=";
			m_sLastError += m_sLogPath;
			m_sLastError += " new path=";
			m_sLastError += sNewPath;
			m_sLastError += " native-error=";
			m_sLastError += strerror(errno);
		}
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
	description : write log to log file
	parameters  :
		- (const char*)  target log string
	return      : (bool) return result
	history     :
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
bool CLogger::WriteToLogFile(const char* sLog)
{
	bool bRet = true;
	try
	{	
		if(NULL == m_pFile) OpenLogFile();
		try 
		{
			int nWritten = 0;

			if(NULL == m_pFile)                               throw LR_ERR_LOG_OPEN_FAILED;
			if(0 > (nWritten = fprintf(m_pFile, "%s", sLog))) throw LR_ERR_LOG_WRITE_FAILED;
			if(0 == nWritten) { if(0 == strlen(sLog))         throw LR_ERR_MAYBE_HDD_FULL; }
			if(0 != fflush(m_pFile))                          throw LR_ERR_LOG_FLUSH_FAILED;
			ResetError();
		}
		catch(_LR_ERR_NO exception)
		{
			bRet = false;
			m_nLastError = exception;
			m_sLastError = _LR_ERR_STR[m_nLastError];
			m_sLastError += " native-error=";
			m_sLastError += strerror(errno);

			if     (LR_ERR_LOG_WRITE_FAILED == exception) OpenLogFile();	
			else if(LR_ERR_MAYBE_HDD_FULL   == exception) fprintf(stderr, "%s~!!!!", _LR_ERR_STR[m_nLastError].c_str());
		}
	}
	catch(std::exception& e) 
	{ 
		std::cerr << "exception caugh: " << e.what() << '\n'; 
		if(ENOENT == errno)
		{
			if(false == IsFileExist(m_sLogPath.c_str()))
			{
				OpenLogFile();
			}
		}
	}
	return bRet;
}

/* ----------------------------------------------------------------------------
	description : format string
	parameters  :
		- (std::string&) out formatted string
		- (int)          formatted length
		- (const char*)  format
		- (va_List)      varlable arguments
	return      : (int) formatted string length
	history     :
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
int CLogger::FormatArgList(std::string& sOut, int nLen, const char* format, va_list args)
{
	int nResult = 0;
	try
	{
		char* pBuf = NULL;

		if(!format) return -1;
		pBuf = new char[nLen + 1];
		memset(pBuf, 0x00, (nLen + 1));
		nResult = vsnprintf(pBuf, nLen, format, args);
		sOut = pBuf;
		delete [] pBuf;
	}
	__CATCH__
	return nResult;
}

/* ----------------------------------------------------------------------------
	description : make and write a log
	parameters  :
		- (u_int)       log level
		- (const char*) comments, ...
	history     :
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::Write(u_int nLevel, const char* comments, ...)
{
	try
	{
		// make log string
		std::string sLog;

		va_list args;
		va_start(args, comments);
		
		int nLen    = LR_VA_ARG_BUF_LEN;
		int nResult = FormatArgList(sLog, nLen, comments, args);
		va_end(args);

		if(LR_VA_ARG_BUF_LEN <= nResult)
		{
			va_start(args, comments);
			FormatArgList(sLog, (nResult+1), comments, args);
			va_end(args);
		}

		// write log
		if(LR_MODE_WRITE_DIRECT & m_nMode) WriteLog(nLevel, (const char*)sLog.data());
		else                               PushLog(nLevel, (const char*)sLog.data());
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
	description : run log writting thread
	parameters  :
		- ()
	history     : (bool) return result
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
bool CLogger::Run()
{
	bool bRet = true;

	if(LR_MODE_WRITE_DIRECT & m_nMode) return bRet;
	try
	{
		if(0 > pthread_create(&m_ThreadID, NULL, LogWriteThreadFunc, this)) throw LR_ERR_LOG_WRITE_TH_FAILED;
		ResetError();
	}
	catch(_LR_ERR_NO exception)
	{
		bRet = false;
		m_nLastError = exception;
		m_sLastError = _LR_ERR_STR[m_nLastError];
		m_sLastError += " native-error=";
		m_sLastError += strerror(errno);
	}
	__CATCH__
	return bRet;
}

/* ----------------------------------------------------------------------------
	description : log writting thread function
	parameters  :
		- (void*) threaed argument
	history     : (void*)
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
void* CLogger::LogWriteThreadFunc(void* pArg)
{
	CLogger* pObj = NULL;
	try
	{
		int nLoopChk = 0;
		unsigned long lMask = 1; // first core

		if(NULL == pArg) return NULL;
		pObj = (CLogger*)pArg;

		if(0 > pthread_setaffinity_np(pthread_self(), sizeof(lMask), (cpu_set_t*)&lMask)) return NULL;

		pObj->m_ThreadID = pthread_self();
		pthread_detach(pObj->m_ThreadID);

		while(true == pObj->m_bRun)
		{
			if(0 < pObj->GetQueueCount())
			{
				CLogObj* pLog = pObj->PopLog();
				if(NULL != pLog) 
				{
					pObj->WriteLog(pLog);
					delete pLog;
				}

				nLoopChk++;
				if(50 < nLoopChk)
				{
					nLoopChk = 0;
					pObj->Sleep(0, 1);
					pObj->CheckTimeAndUpdateConf();
				}
			}
			else 
			{
				pObj->Sleep(0, 1000);
				pObj->CheckTimeAndUpdateConf();
			}
		}
		pthread_exit((void*)NULL);
	}
	__CATCH__
	return NULL;
}

/* ----------------------------------------------------------------------------
	description : sleep method using nanosleep()
	parameters  :
		- (int) seconds
		- (int) micro seconds
	history     : (void*)
		- 2014. 05. 13 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::Sleep(int nSec, int nUSec)
{
    struct timespec req;

    if((0 == nSec) && (0 == nUSec))
    {
        req.tv_sec = 0;
        req.tv_nsec = 999999999L;
    }
    else
    {
        if(10000 > nUSec) nUSec = 10000;
        req.tv_sec = nSec;
        req.tv_nsec = nUSec * 1000;
    }
    nanosleep(&req, &req);
}

/* ----------------------------------------------------------------------------
	description : update last configuration loaded time
	parameters  :
		- ()
	history     : (time_t) return upated time
		- 2014. 12. 15 created by Park. Sung soo
---------------------------------------------------------------------------- */
time_t CLogger::UpdateConfLoadTime()
{
	time_t tRet = 0;

	time(&tRet);
	m_tLastLoaded = tRet;
	return tRet;
}

/* ----------------------------------------------------------------------------
	description : check last update time and update configuration
	parameters  :
		- ()
	history     : (time_t) return upated time
		- 2014. 12. 15 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::CheckTimeAndUpdateConf()
{
	try
	{
		time_t tCur = 0;

		time(&tCur);
		if(m_nConfigReloadInterval <= (u_int)abs(tCur - m_tLastLoaded))
		{
			ReadConfig();
		}
	}
	__CATCH__
}

/* ----------------------------------------------------------------------------
	description : check if given file exist
	parameters  :
		- (const char*) input file name (file path)
	history     : (time_t) return upated time
		- 2018. 01. 22 created by Park. Sung soo
---------------------------------------------------------------------------- */
bool CLogger::IsFileExist(const char* sFilename)
{
	bool bRet = true;
	try
	{
		struct stat buffer;
		bRet = (stat(sFilename, &buffer) == 0);
	}
	__CATCH__
	return bRet;
}

/* ----------------------------------------------------------------------------
	description : system command runner
	parameters  :
		- (const char*) command 
	history     : (time_t) return upated time
		- 2018. 01. 22 created by Park. Sung soo
---------------------------------------------------------------------------- */
bool CLogger::cmd_system(const char* scmd)
{
	pid_t pid;
	int nstatus = 0;
	struct rlimit limit;
	int i = 0;

	if(NULL == scmd) return false;
	pid = fork();
	usleep(10000);
	if(0 > pid) return false;

	switch(pid)
	{
		case 0 :
			{
				getrlimit(RLIMIT_NOFILE, &limit);
				if(RLIM_INFINITY == limit.rlim_max) limit.rlim_max = 1024;
				for(i = 0; (u_int)i < limit.rlim_max; i++)
				{
					close(i);
				}
				execl("/bin/sh", "sh", "-c", scmd, (char*)0);
				break;
			}

		default :
			{
				while(0 > waitpid(pid, &nstatus, 0))
				{
					if(EINTR != errno)
						return false;
				}
				break;
			}
	}
	return true;
}

/* ----------------------------------------------------------------------------
	description : delete old log file(s)
	parameters  :
		- (const char*) log directory
		- (const char*) matching file name sub-string
		- (u_int)       remain file count
	history     : 
		- 2018. 04. 05 created by Park. Sung soo
---------------------------------------------------------------------------- */
void CLogger::DeleteOldFiles(const char* sDir, const char* sFind, u_int nRemain)
{
	try
	{
		if((NULL == sDir) || (NULL == sFind)) return;
		if((0 == strlen(sDir)) || (0 == strlen(sFind))) return;

		DIR*           pdir = NULL;
		struct dirent* pentry  = NULL;
		struct stat    statbuf;
		char           sorgpath[1024] = {0};
		u_int          i = 0, ntotal = 0;
		std::vector<pLOG_FILE_INFO> arrfiles;

		// extract file list
		getcwd(sorgpath, 1024);
		if(NULL == (pdir = opendir(sDir))) return;
		chdir(sDir);

		while(NULL != (pentry = (readdir(pdir))))
		{
			stat(pentry->d_name, &statbuf);
			if(0 == S_ISDIR(statbuf.st_mode))
			{
				std::string sfile(pentry->d_name);

				if(0 <= (int)sfile.find(sFind))
				{
					pLOG_FILE_INFO pinfo = (pLOG_FILE_INFO)malloc(LOG_FILE_INFO_LEN);
					snprintf(pinfo->_name, NAME_MAX+1, "%s", pentry->d_name);
					pinfo->_ctime = statbuf.st_ctime;
					arrfiles.push_back(pinfo);
				}
			}
		}
		closedir(pdir);
		chdir(sorgpath);

		// check and remove file(s)
		ntotal = arrfiles.size();
		if(ntotal > nRemain)
		{
			std::sort(arrfiles.begin(), arrfiles.end(), CompareObj);

			for(i = 0; i < (ntotal - nRemain); i++)
			{
				std::string spath, stemp;
				pLOG_FILE_INFO pcur = arrfiles.at(i);

				spath = sDir;
				spath += '/';
				spath += pcur->_name;
				unlink(spath.data());
			}
		}

		for(i = 0; i < ntotal; i++)
		{
			pLOG_FILE_INFO pdel = arrfiles.at(i);
			if(NULL != pdel) free(pdel);
		}
		arrfiles.clear();
	}
	__CATCH__
}

//-----------------------------------------------------------------------------
#endif // __LOGGER_CPP__
 
