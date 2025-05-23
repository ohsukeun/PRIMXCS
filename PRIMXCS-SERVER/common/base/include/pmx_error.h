/// @file xma_error.h
/// @author Park, sung soo
/// @date 2023-06-16
/// @brief error defined

#ifndef __PMX_ERROR_H__
#define __PMX_ERROR_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
#define PMX_ERROR_SUCCESS    "SUCCESS"
#define PMX_ERROR_E10001     "E10001"    // database error: con not connect to database
#define PMX_ERROR_E10002     "E10002"    // database error: transaction query failed
#define PMX_ERROR_E10003     "E10003"    // database error: select query failed
#define PMX_ERROR_E10004     "E10004"    // database error: procedure running failed
#define PMX_ERROR_E20001     "E20001"    // invalid value: user not found
#define PMX_ERROR_E20002     "E20002"    // invalid value: empty agent id
#define PMX_ERROR_E20003     "E20003"    // invalid value: empty user id
#define PMX_ERROR_E20004     "E20004"    // invalid value: xml parsing error
#define PMX_ERROR_E20005     "E20005"    // invalid value: empty file key
#define PMX_ERROR_E20006     "E20006"    // invalid value: empty company id
#define PMX_ERROR_E20007     "E20007"    // invalid value: empty file hash
#define PMX_ERROR_E30001     "E30001"    // system error: failed to write file
#define PMX_ERROR_E30002     "E30002"    // system error: file not found
#define PMX_ERROR_E30003     "E30003"    // system error: file size is zero
#define PMX_ERROR_E40001     "E40001"    // check result: no update required
#define PMX_ERROR_E40002     "E40002"    // check result: file key not found
#define PMX_ERROR_E40003     "E40003"    // check result: file hash mismatched
#define PMX_ERROR_E40004     "E40004"    // check result: expired file
#define PMX_ERROR_E40005     "E40005"    // check result: decryption limit exceeded
#define PMX_ERROR_E40006     "E40006"    // check result: user id mismatched
#define PMX_ERROR_E40007     "E40007"    // check result: agent id mismatched
#define PMX_ERROR_E40008     "E40008"    // check result: same file key already exists

#define PMX_ERROR_MSG_E10001 "database error: con not connect to database"
#define PMX_ERROR_MSG_E10002 "database error: transaction query failed"
#define PMX_ERROR_MSG_E10003 "database error: select query failed"
#define PMX_ERROR_MSG_E10004 "database error: procedure running failed"
#define PMX_ERROR_MSG_E20001 "invalid value: user not found"
#define PMX_ERROR_MSG_E20002 "invalid value: empty agent id"
#define PMX_ERROR_MSG_E20003 "invalid value: empty user id"
#define PMX_ERROR_MSG_E20004 "invalid value: xml parsing error"
#define PMX_ERROR_MSG_E20005 "invalid value: empty file key"
#define PMX_ERROR_MSG_E20006 "invalid value: empty company id"
#define PMX_ERROR_MSG_E20007 "invalid value: empty file hash"
#define PMX_ERROR_MSG_E30001 "system_error: failed to write file"
#define PMX_ERROR_MSG_E30002 "system error: file not found"
#define PMX_ERROR_MSG_E30003 "system error: file size is zero"
#define PMX_ERROR_MSG_E40001 "check result: no update required"
#define PMX_ERROR_MSG_E40002 "check result: file key not found"
#define PMX_ERROR_MSG_E40003 "check result: file hash mismatched"
#define PMX_ERROR_MSG_E40004 "check result: expired file"
#define PMX_ERROR_MSG_E40005 "check result: decryption limit exceeded"
#define PMX_ERROR_MSG_E40006 "check result: user id mismatched"
#define PMX_ERROR_MSG_E40007 "check result: agent id mismatched"
#define PMX_ERROR_MSG_E40008 "check result: same file key already exists"

//-----------------------------------------------------------------------------
#endif // __PMX_ERROR_H__
