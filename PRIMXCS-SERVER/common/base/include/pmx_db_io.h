/// @file pmx_db_io.h
/// @author Park, sung soo
/// @date 2023-06-19
/// @brief data i/o on database

#include <pmx_mysql_obj.h>

#ifndef __PMX_DB_IO_H__
#define __PMX_DB_IO_H__
//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
/// database i/o methods defined class
class CDBIO
{
public:
	static SWC_BOOL RunSessionMngQuery            (IN CMySQLDBObj* pDBObj, 
												   IN const CHAR* sModuleName);
	static SWC_BOOL RunTransaction                (IN CMySQLDBObj* pDBObj, 
												   IN WCString& sQuery, 
												   OUT WCString& sError); 
	static CMyRSet* ExtractUserData               (IN CMySQLDBObj* pDBObj, 
												   IN const CHAR* sCompanyID, 
												   IN const CHAR* sUserID, 
												   IN const CHAR* sAgentID, 
												   OUT WCString& sFailCode, 
												   OUT WCString& sFailMsg);
	static SWC_BOOL CheckInsertIntoUserSystemMatch(IN CMySQLDBObj* pDBObj, 
												   IN const CHAR* sCompanyID, 
												   IN const CHAR* sAgentID, 
												   IN const CHAR* sUserID, 
												   IN const CHAR* sSharePC, 
												   OUT WCString& sFailCode, 
												   OUT WCString& sFailMsg);
	static SWC_BOOL ExtractKeepAliveData          (IN CMySQLDBObj* pDBObj, 
												   IN const CHAR* sAgentID,                      // 1. Agent ID                      
												   IN const CHAR* sUserID,                       // 2. User ID
												   IN const CHAR* sCompanyID,                    // 3. Company ID
												   IN const CHAR* sAgentTimeStamp,               // 4. agent applied last policy timestamp
												   IN const CHAR* sSharePC,                      // 5. share pc or not (YES/NO)
												   IN const CHAR* sLastPolicyID,

												   IN const CHAR* sLastPolicyDate,

												   OUT WCString& sExpandCommmand,                // 6. extended command
												   OUT INT& nKeepAliveCycle,                     // 7. keep alive interval (seconds)
												   OUT INT& nLogCycle,                           // 8. log send interval (seconds)
												   OUT WCString& sAgentUpdate,                   // 9. PRIMX agent update or not (YES/NO)
												   OUT WCString& sZonecentralUpdate,             // 10. PRIMX ZoneCentral update or not (YES/NO)
												   OUT WCString& sOrizonUpdate,                  // 11. PRIMX Orizon update or not (YES/NO)
												   OUT WCString& sCryhodUpdate,                  // 12. PRIMX Cryhod update or not (YES/NO)
												   OUT WCString& sServerTime,                    // 13. current server time
												   OUT INT& nExportShelfLife,                    // 14. export file day limit (days)
												   OUT WCString& sOutCompanyID,                  // 15. out company ID
												   OUT WCString& sAuthServerAddr,                // 16. authentication server address
												   OUT INT& nAuthServerPort,                     // 17. authentication server port
												   OUT WCString& sProductMenuEnable,             // 18. product ment enable (YES/NO)
												   OUT WCString& sWebManagerConsoleMenuEnable,   // 19. web manager console menu enable (YES/NO)
												   OUT WCString& sWebmanagerConsoleURL,          // 20. web manager console url
												   OUT WCString& sExportMenuEnable,              // 21. export menu enable (YES/NO)
												   OUT WCString& sAuthWebManager,                // 22. file export request, confirm auth. (YES/NO)
												   OUT WCString& sAuthExportFile,                // 23. create export file auth. (YES/NO)
												   OUT WCString& sAuthNotEncryptionPath,         // 24. use not encryption path auth. (YES/NO)
												   OUT WCArray<WCString>& arrNotEncryptPathList, // 25. not encryption path list ('|' divied)
												   OUT WCString& sPolicyUpdateQuery,
												   OUT WCString& sPolicyFileName,
												   OUT WCString& sOutLastPolicyDate,        
												   OUT WCString& sOutLastPolicyID,      
												   OUT WCString& sFailCode,
												   OUT WCString& zTryMenuEnable,
                                                                         			   OUT WCString& zOpenArea,
 			                                                                           OUT WCString& zPersonalAccess,
                          			                                                   OUT WCString& zOptionDisplay,
                                                    			                           OUT WCString& zSafetyZone,
			                                                                           OUT WCString& zAreaClose,
                          			                                                   OUT WCString& zEnc,
												   OUT WCString& tZoneMap,
												   OUT WCString& sFailMsg);
	static SWC_BOOL ExtractProductUpdateInfo      (IN CMySQLDBObj* pDBObj, 
												   IN const CHAR* sAgentID,
												   IN INT nProductType, 
												   IN const CHAR* sInstalledVersion, 
												   OUT WCString& sNedPatchYN,
												   OUT WCString& sCurrentProductVersion,
												   OUT WCArray<WCString>& arrExServerURL,
												   OUT WCString& sFilePath,
												   OUT WCString& sFileHash,
												   OUT WCString& sFailCode,
												   OUT WCString& sFailMsg);
	static SWC_BOOL ExtractAuthenticateExDecrypt  (IN CMySQLDBObj* pDBObj,
												   IN const CHAR* sUserID,
												   IN const CHAR* sAgentID,
												   IN const CHAR* sFileKey,
												   IN const CHAR* sCompanyID,
												   IN const CHAR* sFileHash,
												   OUT WCString& sFailCode,
												   OUT WCString& sFailMsg);
	static SWC_BOOL ApplyAuthenticatedExDecrypt   (IN CMySQLDBObj* pDBObj,
												   IN const CHAR* sUserID,
												   IN const CHAR* sAgentID,
												   IN const CHAR* sFileKey,
												   IN const CHAR* sCompanyID,
												   IN const CHAR* sRequestIP,
												   IN const CHAR* sRequestMAC,
												   IN const CHAR* sResultCode,
												   IN const CHAR* sComment);
	static SWC_BOOL InsertExFileInfo              (IN CMySQLDBObj* pDBObj, 
												   IN const CHAR* sUserID,
												   IN const CHAR* sAgentID,
												   IN const CHAR* sFileKey,
												   IN const CHAR* sCompanyID,
												   IN const CHAR* sFileHash, 
												   IN const CHAR* sFileName,
												   IN UINT64 lFileSize, 
												   OUT WCString& sFailCode,
												   OUT WCString& sFailMsg);
	CDBIO() {};
	~CDBIO() {};

};


//-----------------------------------------------------------------------------
#endif // __PMX_DB_IO_H__
