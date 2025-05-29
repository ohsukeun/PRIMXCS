/*****************************************************************************
 *                                                                           *
 * File Name : swc_debug.h                                                   *
 * Theme     : debugging messaging & etc ...                                 *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 11. 24                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_debug.h
/// @author Park, sung soo
/// @date 2009-11-24
/// @brief debugging messaging & etc 

#ifndef __SWC_DEBUG_H__
#define __SWC_DEBUG_H__

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

// debugging message options :
#define DMSG_M     (1 << 0)   // message
#define DMSG_E     (1 << 1)   // error

#define DMSG_S     (1 << 8)   // print to the std. out
#define DMSG_W     (1 << 9)   // print to the log file
#define DMSG_P     (1 << 10)  // perror

#define DMSG_MS    DMSG_M | DMSG_S
#define DMSG_MW    DMSG_M | DMSG_W
#define DMSG_MP    DMSG_M | DMSG_P
#define DMSG_ES    DMSG_E | DMSG_S
#define DMSG_EP    DMSG_E | DMSG_P

#define DMSG_MSW   DMSG_M | DMSG_S | DMSG_W
#define DMSG_MWS   DMSG_M | DMSG_S | DMSG_W

#define DMSG_ESW   DMSG_E | DMSG_S | DMSG_W
#define DMSG_EWS   DMSG_E | DMSG_S | DMSG_W

#define DMSG_ESP   DMSG_E | DMSG_S | DMSG_P
#define DMSG_EPS   DMSG_E | DMSG_S | DMSG_P

#define DMSG_ESWP  DMSG_E | DMSG_S | DMSG_W | DMSG_P
#define DMSG_ESPW  DMSG_E | DMSG_S | DMSG_W | DMSG_P
#define DMSG_EPSW  DMSG_E | DMSG_S | DMSG_W | DMSG_P
#define DMSG_EPWS  DMSG_E | DMSG_S | DMSG_W | DMSG_P
#define DMSG_EWSP  DMSG_E | DMSG_S | DMSG_W | DMSG_P
#define DMSG_EWPS  DMSG_E | DMSG_S | DMSG_W | DMSG_P

// socket error rumbers :
#define D_SOCK_ECREATESOCK          -1
#define D_SOCK_ESETSOCKOPT          -2
#define D_SOCK_EBIND                -3
#define D_SOCK_ELISTEN              -4
#define D_SOCK_ECLOSE               -5
#define D_SOCK_ECONN                -6
#define D_SOCK_ETIMEOUT             -7
#define D_SOCK_EMAKEHEADER          -8
#define D_SOCK_ESENDHEADER          -9
#define D_SOCK_ESENDAGENT          -10
#define D_SOCK_ESENDFILE           -11
#define D_SOCK_EFILEOPEN           -12
#define D_SOCK_EGETMSGLEN          -13
#define D_SOCK_ERCVHEADER          -14
#define D_SOCK_ERCVAGENT           -15
#define D_SOCK_ERCV                -16
#define D_SOCK_EINVADDR            -17
#define D_SOCK_ECONN_DMGR          -18
#define D_SOCK_ESEND_TO_DMGR       -19
#define D_SOCK_ERCV_FROM_DMGR      -20
#define D_SOCK_ESEND_TO_DMGR       -19
#define D_SOCK_ERCV_FROM_DMGR      -20
#define D_SOCK_SERR_INIT           -21
#define D_SOCK_SERR_GET_HOSTNAME   -22
#define D_SOCK_SERR_GET_HOST_INFO  -23
#define D_SOCK_SERR_GET_PASSPHRASE -24

extern unsigned int OpenDebugFile  (const char* spath, unsigned int nlimit);
extern void         CloseDebugFile (void);
extern void         DMsg           (int option, const char* comments, ...);
extern void         SockErrMsg     (int errnum);
extern void         SetDebugModeOn (void);
extern void         SetDebugModeOff(void);
extern void         SetMsgOn       (void);
extern void         SetMsgOff      (void);
extern unsigned int IsDMsgOn       (void);

//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif // __SWC_DEBUG_H__
 
