/// @file pmx_mysql_obj.h
/// @author Park, sung soo
/// @date 2023-06-12
/// @brief mysql database object definition

#include <iostream>
#include <mysql.h>
#include <sys/timeb.h>

#include <pmx_base_def.h>

#ifndef __PMX_MYSQL_OBJ_H__
#define __PMX_MYSQL_OBJ_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

// length definitions
#define DB_HOST_LEN             256
#define DB_NAME_LEN             256
#define DB_USER_LEN             256
#define DB_PWD_LEN              256
#define DB_ERR_STR_LEN         1024
#define DB_CONN_STR_LEN        1024
#define DB_CHARSET_NAME_LEN     256

#define DB_COLUMN_NAME_LEN      256

// environmental definitions
#define DB_LOGIN_TIMEOUT         20
#define DB_QRY_TIMEOUT          300
#define DB_QRY_LONG_TIMEOUT      50
#define DB_MAX_RETRY_CONN        60 // 60 seconds. retry 60 times

/// error definitions
enum _DB_ERR_NO
{
	DB_ERR_SUCCESS                            = 0,
	DB_ERR_EMPTY_DBNAME,                     // 1
	DB_ERR_EMPTY_USER,                       // 2
	DB_ERR_EMPTY_PWD,                        // 3
	DB_ERR_EMPTY_HOST,                       // 4
	DB_ERR_PORT_ZERO,                        // 5
	DB_ERR_EMPTY_CONNSTR,                    // 6
	DB_ERR_NULL_DB_PTR,                      // 7
	DB_ERR_AUTO_COMMIT,                      // 8
	DB_ERR_EMPTY_QRY,                        // 9
	DB_ERR_TIMEOUT,                          // 10
	DB_ERR_NULL_RSET_PTR,                    // 11
	DB_ERR_GENERIC,                          // 12
	DB_ERR_DATABASE,                         // 13
	DB_ERR_EMPTY_SID,                        // 14
	DB_ERR_EMPTY_SVCNAME,                    // 15
	DB_ERR_NULL_ENV_PTR,                     // 16
	DB_ERR_NULL_CONN_POOL_PTR,               // 17
	DB_ERR_NULL_CONN_PTR,                    // 18
	DB_ERR_OUT_OF_RANGE,                     // 19
	DB_ERR_INIT,                             // 20
	DB_ERR_ODBC_ALLOC_HANDLE_ENV,            // 21
	DB_ERR_ODBC_SET_ENV_ATTR,                // 22
	DB_ERR_ODBC_ALLOC_HANDLE_DBC,            // 23
	DB_ERR_ODBC_SET_CONN_ATTR_AUTOCOMMIT,    // 24
	DB_ERR_ODBC_SET_CONN_ATTR_LOGIN_TIMEOUT, // 25
	DB_ERR_CONNECT_FAIL,                     // 26
	DB_ERR_DISCONNECTED,                     // 27
	DB_ERR_DBMS_FAULT,                       // 28
	DB_ERR_NULL_DB_POOL,                     // 29
	DB_ERR_GET_CONN_FROM_DB_POOL_FAILED,     // 30
	DB_ERR_DB_POOL_CREATING_FAILED,          // 31
	DB_ERR_DB_POOL_FREE_FAILED,              // 32
};

/* 
	critical error definitions : based mysql error set

case 1 : server connection related error
1042	08S01		ER_BAD_HOST_ERROR
1043	08S01		ER_HANDSHAKE_ERROR
1053	08S01		ER_SERVER_SHUTDOWN
1077	HY000		ER_NORMAL_SHUTDOWN
1079	HY000		ER_SHUTDOWN_COMPLETE
1081	08S01		ER_IPSOCK_ERROR
1129	HY000		ER_HOST_IS_BLOCKED
1130	HY000		ER_HOST_NOT_PRIVILEGED
1158	08S01		ER_NET_READ_ERROR
1159	08S01		ER_NET_READ_INTERRUPTED
1160	08S01		ER_NET_ERROR_ON_WRITE
1161	08S01		ER_NET_WRITE_INTERRUPTED

2002				CR_CONNECTION_ERROR
2003				CR_CONN_HOST_ERROR
2004				CR_IPSOCK_ERROR
2005				CR_UNKNOWN_HOST
2006				CR_SERVER_GONE_ERROR -> try reconnection
2012				CR_SERVER_HANDSHAKE_ERR
2013				CR_SERVER_LOST
2055                CR_SERVER_LOST_EXTENDED

case 2 : deadlock related error
1213	40001		ER_LOCK_DEADLOCK
1479	XA102		ER_XA_RBDEADLOCK

case 3 : no recordset error (not error)
2053                CR_NO_RESULT_SET
*/

#ifndef CR_NO_RESULT_SET
	#define CR_NO_RESULT_SET 2053
#endif 

#define DEF_PMX_MYSQL_CHARSET_EUCKR    "euckr"
#define DEF_PMX_MYSQL_CHARSET_ASCII    "ascii"
#define DEF_PMX_MYSQL_CHARSET_UTF8     "utf8"
#define DEF_PMX_MYSQL_CHARSET_LATIN1   "latin1"

/// field class
class CField
{
public :
	// general info. ----------------------------------------------------------
	VOID           SetID        (IN INT         nID)   { m_nID   = nID; };
	VOID           SetName      (IN const CHAR* sName) { m_sName = sName; };
	VOID           SetName      (IN WCString    sName) { m_sName = sName; };
	VOID           SetType      (IN INT         nType) { m_nType = nType; };
	VOID           SetSize      (IN UINT        nSize) { m_nSize = nSize; };
	VOID           SetNativeType(IN INT nNativeType) { m_nNativeType = nNativeType; }
	VOID           SetData(IN INT nID, IN const CHAR* sName, IN INT nType, IN UINT nSize)
	{ SetID(nID); SetName(sName); SetType(nType); SetSize(nSize); };
	VOID           SetData(IN INT nID, IN WCString sName, IN INT nType, IN UINT nSize)
	{ SetID(nID); SetName(sName); SetType(nType); SetSize(nSize); };
	VOID           SetData(IN INT nID, IN const CHAR* sName, IN INT nType, IN INT nNativeType, IN UINT nSize)
	{ SetID(nID); SetName(sName); SetType(nType); SetSize(nSize); SetNativeType(nNativeType); };
	VOID           SetData(IN INT nID, IN WCString sName, IN INT nType, IN INT nNativeType, IN UINT nSize)
	{ SetID(nID); SetName(sName); SetType(nType); SetSize(nSize); SetNativeType(nNativeType); };

	const INT      GetID        () { return m_nID; };
	const WCString GetName      () { return m_sName; };
	const INT      GetType      () { return m_nType; };
	const INT      GetNativeType() { return m_nNativeType; }
	const UINT     GetSize      () { return m_nSize; };

	SWC_BOOL       IsArrayFetchMode() { return m_bUseArrayFetch; }
	UINT           GetFetchCellLen () { return m_nFetchCellLen; }
	PCHAR          GetFetchBufPtr  () { return m_pFetchBuf; }
	PCHAR          NextOptPtr      () { m_pOpt += m_nFetchCellLen; return m_pOpt; }
	PCHAR          GetOptPtr       () { return m_pOpt; }
	PCHAR          SetOptPtr       (IN PCHAR pPtr) { m_pOpt = pPtr; return m_pOpt; }

	const WCString ToString()
	{
		WCString sRet("");
		sRet.Format("column ID: %d column name: %s column type: %d column Length: %u",
				m_nID, LPCSTR(m_sName), m_nType, m_nSize);
		return sRet;
	}

	VOID Init()
	{
		m_nID = 0;
		m_sName.Empty();
		m_nType = 0;
		m_nNativeType = 0;
		m_nSize = 0;

		m_bUseArrayFetch = FALSE;
		m_nFetchCellLen = 0;
		m_pFetchBuf = m_pOpt = NULL;
	}

	// oracle only: array fetch operation info. -------------------------------
	/** @brief set use array fetch. caution: call after general info. applied @param input reserved row size */
	VOID SetUseArrayFetch(IN UINT nReservedRowSize)
	{
		if((0 == nReservedRowSize) || (0 == m_nSize)) return;

		m_bUseArrayFetch = TRUE;
		m_nFetchCellLen = m_nSize;
		if(NULL != m_pFetchBuf)
		{
			delete [] m_pFetchBuf;
			m_pFetchBuf = m_pOpt = NULL;
		}
		m_pFetchBuf = new CHAR[nReservedRowSize * m_nFetchCellLen];
		m_pOpt = m_pFetchBuf;
	}
	VOID Clear()
	{
		if(TRUE == m_bUseArrayFetch)
		{
			if(NULL != m_pFetchBuf) delete [] m_pFetchBuf;
			m_pFetchBuf = NULL;
		}
	}
	
	// creator / destroyer ----------------------------------------------------
	CField()  { Init(); }
	CField(IN INT nID, IN const CHAR* sName, IN INT nType, IN UINT nSize) { Init(); SetData(nID, sName, nType, nSize); };
	CField(IN INT nID, IN WCString    sName, IN INT nType, IN UINT nSize) { Init(); SetData(nID, sName, nType, nSize); };
	CField(IN INT nID, IN const CHAR* sName, IN INT nType, IN INT nNativeType, IN UINT nSize)
	{ Init(); SetData(nID, sName, nType, nNativeType, nSize); };
	CField(IN INT nID, IN WCString    sName, IN INT nType, IN INT nNativeType, IN UINT nSize)
	{ Init(); SetData(nID, sName, nType, nNativeType, nSize); };
	~CField() { Clear(); };

private :
    // general info. ----------------------------------------------------------
	INT      m_nID;            ///< field ID (zero based index)
	WCString m_sName;          ///< field name
	INT      m_nType;          ///< field type (enum _DB_DATA_TYPE)
	INT      m_nNativeType;    ///< field native data type (DBMS native)
	UINT     m_nSize;          ///< field length

	// oracle only: array fetch operation info. -------------------------------
	SWC_BOOL m_bUseArrayFetch; ///< use array fetch or not
	UINT     m_nFetchCellLen;  ///< fetch cell length (if string, m_nSize X 2)
	PCHAR    m_pFetchBuf;      ///< patch buffer
	PCHAR    m_pOpt;           ///< pointer shift operation pointer
};

/// record set wrapper class
class CMyRSet
{
public :
	// record iterator
	VOID            MoveFirstRow    ();
	SWC_BOOL        FetchRow        ();

	// field index based methods
	SWC_BOOL        IsNull          (IN INT nField);
	const CHAR*     GetStrValue     (IN INT nField);
	UINT            GetUIntValue    (IN INT nField);
	INT             GetIntValue     (IN INT nField);
	ULONG           GetULongValue   (IN INT nField);
	LONG            GetLongValue    (IN INT nField);
	UINT64          GetUInt64Value  (IN INT nField);
	DOUBLE          GetDFValue      (IN INT nField);

	// field name based methods
	SWC_BOOL        IsNull          (IN const CHAR* sField);
	const CHAR*     GetStrValue     (IN const CHAR* sField);
	UINT            GetUIntValue    (IN const CHAR* sField);
	INT             GetIntValue     (IN const CHAR* sField);
	ULONG           GetULongValue   (IN const CHAR* sField);
	LONG            GetLongValue    (IN const CHAR* sField);
	UINT64          GetUInt64Value  (IN const CHAR* sField);
	DOUBLE          GetDFValue      (IN const CHAR* sField);

	// 
	// row(column) index and field index based methods
	// not need record iterator.
	// 
	// warning !! : performance problem exists. use these carefully
	// 
	SWC_BOOL        IsNull          (IN INT nRow, IN INT nField);
	CHAR*           GetStrValue     (IN INT nRow, IN INT nField);
	ULONG           GetULongValue   (IN INT nRow, IN INT nField);
	LONG            GetLongValue    (IN INT nRow, IN INT nField);
	UINT64          GetUInt64Value  (IN INT nRow, IN INT nField);
	DOUBLE          GetDFValue      (IN INT nRow, IN INT nField);

	// records handling methods
	INT             GetFieldCount       ();
	INT             GetRowCount         ();	
	INT             GetFieldsSortedArray(IN WCArray<CField*>& arrField);
	VOID            Close               ();
	VOID            SetFieldsToMap      ();

	CMyRSet();
	~CMyRSet();

private :
	INT  GetFieldIndex (IN const CHAR* sField);

public :
	MYSQL_RES*    m_pRSet;
	MYSQL_ROW     m_Row;

private :
	WCMapStrToPtr m_mapField;  // key - field name : data - CField
};

/// database connection object
class CMySQLDBObj
{
public :
	SWC_BOOL                 Init                  (IN const CHAR* shost,
                                                    IN const CHAR* sdbname,
													IN const CHAR* sCharSet,
													IN const CHAR* sNCharSet,
                                                    IN const CHAR* suser,
                                                    IN const CHAR* spwd,
                                                    IN USHORT      nport);
	VOID                     InitSSL               (IN SWC_BOOL    buse,
                                                    IN const CHAR* sslkey,
                                                    IN const CHAR* sslcert,
                                                    IN const CHAR* sslca,
                                                    IN const CHAR* sslcapath,
                                                    IN const CHAR* sslcipher);
	VOID                     SetUseUTF8            () { m_bUseUTF8 = TRUE; };
	VOID                     SetNotUseUTF8         () { m_bUseUTF8 = FALSE; };
	VOID                     SetUseCharSet         (IN const CHAR* sset);
	VOID                     SetAutoCommit         (IN SWC_BOOL bset);

	const CHAR*              GetHost               () { return m_sHost; };
	const CHAR*              GetDBName             () { return m_sDBName; };
	const CHAR*              GetUser               () { return m_sUser; };
	const CHAR*              GetPwd                () { return m_sPwd; };
	const USHORT             GetPort               () { return m_nPort; };

	SWC_BOOL                 Connect               ();
	VOID                     Disconnect            ();
	SWC_BOOL                 Commit                ();
	SWC_BOOL                 Rollback              ();
	CMyRSet*                 ExecuteQuery          (IN SWC_BOOL brset, IN const CHAR* sqry, IN SWC_BOOL bUseArrayFetch = FALSE);
	CMyRSet*                 ExecuteQuery          (IN SWC_BOOL brset, IN WCString sqry, IN SWC_BOOL bUseArrayFetch = FALSE);
	SWC_BOOL                 IsConnected           () { return m_bConn; };
	ULONG                    GetLastErrorNumber    ();
	const CHAR*              GetLastErrorString    ();
	SWC_BOOL                 IsErrCritical         () { return m_bErrCritical; };
	SWC_BOOL                 IsErrDeadLock         () { return m_bErrDeadLock; };

	const SWC_BOOL           IsBusy                () const     { return m_bIsBusy; };
	VOID                     SetBusy               ()           { m_bIsBusy = TRUE; };
	VOID                     SetIdle               ()           { m_bIsBusy = FALSE; };

	VOID                     SetTimeOuted          ();
	time_t                   GetTimeLastQry        () { return m_tLastQry; };
	VOID                     UpdateTime            () { time(&m_tLastQry); };
	VOID                     GetQueryRunTime       (OUT UINT& nSec, OUT UINT& nMili);

	LONG                     GetDatabaseErrorNo    () { return m_lErrDatabase; };
	const CHAR*              GetDatabaseErrorString() { return GetLastErrorString(); }
	INT                      GetLastAffectedRows   () { return m_nAffectCnt; };

	VOID                     SetID                 (IN INT nid) { m_nID = nid; };
	const INT                GetID                 () const     { return m_nID; };

	CMySQLDBObj();
	virtual ~CMySQLDBObj();

private :
	VOID     SetErr      (IN SWC_BOOL bnative, IN ULONG lerr, IN const CHAR* sLastErr = NULL);
	VOID     FixString   (OUT WCString& sqry);
	SWC_BOOL TryReconnect();
	INT      RunQuery    (IN const CHAR* sQry);
	VOID     PrintMYSQL  (IN MYSQL* pMysql);

private:
	// connection info.--------------------------------------------------------
	// general info.
	SWC_BOOL m_bUseUTF8;             ///< use UTF-8 or not
	CHAR     m_sHost[DB_HOST_LEN];   ///< database host name
	CHAR     m_sDBName[DB_NAME_LEN]; ///< database name
	CHAR     m_sUser[DB_USER_LEN];   ///< user ID
	CHAR     m_sPwd[DB_PWD_LEN];     ///< password
	USHORT   m_nPort;                ///< port number
	WCString m_sCharSet;             ///< character set

	// ssl related
	SWC_BOOL m_bUseSSL;              ///< SSL use or not
	WCString m_SSLKey;               ///< SSL key string
	WCString m_SSLCert;              ///< SSL cert 
	WCString m_SSLCA;                ///< SSL CA 
	WCString m_SSLCAPath;            ///< SSL CA path
	WCString m_SSLCipher;            ///< SSL cipher
	//-------------------------------------------------------------------------

	MYSQL*   m_pConn;                ///< mysql connection object pointer
	SWC_BOOL m_bConn;                ///< connection status
	SWC_BOOL m_bAutoCommit;          ///< auto commit or not
	SWC_BOOL m_bIsBusy;              ///< busy or not
	INT      m_nID;                  ///< integer ID for managing
	time_t   m_tLastQry;             ///< last query time for check
	UINT     m_nAffectCnt;           ///< last affected record count by transaction query
	SWC_BOOL m_bTimeOut;             ///< query timeout or not

	struct timespec m_tQryStartTm;   ///< query start time
	struct timespec m_tQryEndTm;     ///< query end time

	// error related
	SWC_BOOL m_bNativeErr;
	ULONG    m_lErr;                 ///< last error number
	LONG     m_lErrDatabase;         ///< database error number
	SWC_BOOL m_bErrCritical;         ///< critical error ?
	SWC_BOOL m_bErrDeadLock;         ///< dead lock error ?
	CHAR     m_sErr[DB_ERR_STR_LEN]; ///< last error message
	WCString m_sLastErr;             ///< mysql last error string buffer
};

//-----------------------------------------------------------------------------
#endif // __PMX_MYSQL_OBJ_H__
 
