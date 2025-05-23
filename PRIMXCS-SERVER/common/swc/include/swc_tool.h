/*****************************************************************************
 *                                                                           *
 * File Name : swc_tool.h                                                    *
 * Theme     : tool functions                                                *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 11. 23                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_tool.h 
/// @author Park, sung soo
/// @date 2009-11-23
/// @brief tool functions

#include <swc_base.h>
#include <swc_string.h>
#include <swc_array.h>

#ifndef __SWC_TOOL_H__
#define __SWC_TOOL_H__

#define UL2S(X) LPCSTR(ul2s(X))
#define L2S(X)  LPCSTR(l2s(X))
#define I2S(X)  LPCSTR(i2s(X))
#define SH2S(X) LPCSTR(sh2s(X))

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------

/**
 * @file swc_array.h
 * @brief SWC name spaced tool functions
 */

#if defined(__LP64__) || defined(_LP64)
/// SWC name space & tool function
namespace SWC
{
	SWC_BOOL swc_chmod                (IN const CHAR* spath, IN INT own, IN INT grp, IN INT other);
	SWC_BOOL swc_chmod                (IN WCString spath, IN INT own, IN INT grp, IN INT other);
	SWC_BOOL swc_fchmod               (IN INT fd, IN INT own, IN INT grp, IN INT other);
	VOID     swc_chown_chgrp          (IN const CHAR* spath, IN const CHAR* owner, IN const CHAR* group);
	VOID     swc_chown_chgrp          (IN WCString spath, IN WCString owner, IN WCString group);
	VOID     swc_fchown_fchgrp        (IN INT fd, IN const CHAR* owner, IN const CHAR* group);
	VOID     swc_fchown_fchgrp        (IN INT fd, IN WCString owner, IN WCString group);
	VOID     swc_chown                (IN const CHAR* spath, IN const CHAR* owner);
	VOID     swc_chown                (IN WCString spath, IN WCString owner);
	VOID     swc_fchown               (IN INT fd, IN const CHAR* owner);
	VOID     swc_fchown               (IN INT fd, IN WCString owner);
	VOID     swc_chgrp                (IN const CHAR* spath, IN const CHAR* group);
	VOID     swc_chgrp                (IN WCString spath, IN WCString group);
	VOID     swc_fchgrp               (IN INT fd, IN const CHAR* group);
	VOID     swc_fchgrp               (IN INT fd, IN WCString group);
	SWC_BOOL swc_parse_file_name_from_path(IN const CHAR* spath, OUT CHAR* sout, IN INT nlen);
	SWC_BOOL swc_chk_proc_exist       (IN const CHAR* proc);
	SWC_BOOL swc_chk_proc_exist       (IN WCString proc);
	VOID     swc_send_signal_to_proc  (IN const CHAR* proc, IN INT signo);
	VOID     swc_send_signal_to_proc  (IN WCString proc, IN INT signo);
	CHAR*    swc_cls_apostrophe       (IN CHAR* strbuf);
	CHAR*    swc_cls_comma            (IN CHAR* strbuf);
	CHAR*    swc_cls_linefeed         (IN CHAR* strbuf);
	CHAR*    swc_cvt_chr_chr          (IN CHAR* strbuf, IN CHAR cfrom, IN CHAR cto);
	CHAR*    swc_str_chr_right        (IN const CHAR* src, IN const CHAR find, OUT CHAR** ptrptr);
	CHAR*    swc_cvt_chr_str          (IN const CHAR* src, 
                                       IN CHAR        *dst,
                                       IN INT         ndstlen,
                                       IN const CHAR  cfind,
                                       IN const CHAR* scvt);
	CHAR*    swc_str_chr_right_csv    (IN const CHAR* src, IN const CHAR find, OUT CHAR** ptrptr);
	INT      swc_iconv                (IN CHAR*       inbuf,
                                       OUT CHAR*      outbuf,
                                       IN INT         outlen,
                                       IN const CHAR* setfrom,
                                       IN const CHAR* setto);
	CHAR*    swc_toupper              (IN CHAR* strbuf);
	SWC_BOOL swc_is_file_exists       (IN const CHAR* sdir, IN const CHAR* sfile);
	SWC_BOOL swc_is_file_exists       (IN WCString sdir, IN WCString sfile);
	UINT     swc_cnt_match_file_in_dir(IN const CHAR* sdir, IN const CHAR* slike);
	UINT     swc_cnt_match_file_in_dir(IN WCString sdir, IN WCString slike);
	UINT     swc_get_matched_extension_files_in_dir(IN const CHAR* sDir, IN const CHAR* sExt, OUT WCArray<WCString>& arrPath);
	SWC_BOOL swc_is_dir_exists        (IN const CHAR* sdir);
	SWC_BOOL swc_is_dir_exists        (IN WCString sdir);
	SWC_BOOL swc_rmdir                (IN const CHAR* sdir);
	UINT     swc_get_file_size        (IN const CHAR* spath);
	UINT     swc_get_file_size        (IN WCString spath);
	SWC_BOOL swc_is_file_size_over    (IN const CHAR* spath, IN UINT lsize);
	SWC_BOOL swc_is_file_size_over    (IN WCString spath, IN UINT lsize);
	SWC_BOOL swc_is_unicode_file      (IN const CHAR* spath);
	VOID     swc_sleep                (IN INT _sec, IN INT _usec);
	SWC_BOOL swc_get_remote_status    (IN const CHAR* seth, IN const CHAR* srcip, IN const CHAR* dstip);
	SWC_BOOL swc_get_remote_status    (IN WCString seth, IN WCString srcip, IN WCString dstip);
	SWC_BOOL swc_get_remote_status    (IN const CHAR* seth, IN UINT srcip, IN UINT dstip);
	SWC_BOOL swc_is_eth_exists        (IN WCString seth);
	UINT     swc_get_1st_mac          (OUT UCHAR cmac[]);
	SWC_BOOL swc_get_dns              (OUT CHAR* sdns, IN INT nlen);
	WCString swc_get_dns              ();
	VOID     swc_set_dns              (IN const CHAR* sdns);
	VOID     swc_set_dns              (IN WCString sdns);
	UINT     swc_get_eth_ip           (IN const CHAR* seth, OUT CHAR* sout, IN INT nlen);
	UINT     swc_get_eth_ip           (IN WCString seth, OUT WCString& sout, IN INT nlen);
	VOID     swc_get_local_time       (IN time_t ltime, OUT WCString& sret);
	VOID     swc_get_global_time      (IN time_t ltime, OUT WCString& sret);
	time_t   swc_lt_to_gt             (IN time_t lt);
	time_t   swc_gt_to_lt             (IN time_t gt);
	WCString ul2s                     (IN UINT ulval);
	WCString l2s                      (IN INT lval);
	WCString i2s                      (IN INT ival);
	WCString sh2s                     (IN SHORT shval);
	VOID     time_ltos                (IN UINT ltime, OUT CHAR* stime, IN INT len); 
	time_t   time_vtol                (IN INT year, IN INT mon, IN INT day, IN INT hour, IN INT min, IN INT sec); 
	time_t   time_stol                (IN const CHAR* strdate); 
	INT      time_get_year            (IN time_t ltime); 
	INT      time_get_mon             (IN time_t ltime); 
	INT      time_get_day             (IN time_t ltime); 
	INT      time_get_hour            (IN time_t ltime); 
	INT      time_get_min             (IN time_t ltime); 
	INT      time_get_sec             (IN time_t ltime); 
	INT      time_get_day_of_week     (IN time_t ltime);
	INT      time_get_week_of_mon     (IN time_t ltime);
	VOID     swc_get_smac_s           (OUT CHAR* smac, IN INT nlen, IN UINT lmacu, IN UINT lmacl);
	VOID     swc_get_smac_w           (OUT WCString& smac, IN UINT lmacu, IN UINT lmacl);
	UINT     swc_get_peer_ip          (IN INT sfd);
	UINT     swc_get_cpu_count        ();
	VOID     swc_smac_to_cmac         (IN const CHAR* smac, OUT UCHAR cmac[]);
	VOID     swc_smac_insert_semicolon(IN const CHAR* smac, OUT CHAR* sout, IN INT nlen);
	SWC_BOOL swc_system               (IN const CHAR* scmd);
	SWC_BOOL swc_isnumber_str         (IN WCString sNum);
}
#else
namespace SWC
{
	SWC_BOOL swc_chmod                (IN const CHAR* spath, IN INT own, IN INT grp, IN INT other);
	SWC_BOOL swc_chmod                (IN WCString spath, IN INT own, IN INT grp, IN INT other);
	SWC_BOOL swc_fchmod               (IN INT fd, IN INT own, IN INT grp, IN INT other);
	VOID     swc_chown_chgrp          (IN const CHAR* spath, IN const CHAR* owner, IN const CHAR* group);
	VOID     swc_chown_chgrp          (IN WCString spath, IN WCString owner, IN WCString group);
	VOID     swc_fchown_fchgrp        (IN INT fd, IN const CHAR* owner, IN const CHAR* group);
	VOID     swc_fchown_fchgrp        (IN INT fd, IN WCString owner, IN WCString group);
	VOID     swc_chown                (IN const CHAR* spath, IN const CHAR* owner);
	VOID     swc_chown                (IN WCString spath, IN WCString owner);
	VOID     swc_fchown               (IN INT fd, IN const CHAR* owner);
	VOID     swc_fchown               (IN INT fd, IN WCString owner);
	VOID     swc_chgrp                (IN const CHAR* spath, IN const CHAR* group);
	VOID     swc_chgrp                (IN WCString spath, IN WCString group);
	VOID     swc_fchgrp               (IN INT fd, IN const CHAR* group);
	VOID     swc_fchgrp               (IN INT fd, IN WCString group);
	SWC_BOOL swc_parse_file_name_from_path(IN const CHAR* spath, OUT CHAR* sout, IN INT nlen);
	SWC_BOOL swc_chk_proc_exist       (IN const CHAR* proc);
	SWC_BOOL swc_chk_proc_exist       (IN WCString proc);
	VOID     swc_send_signal_to_proc  (IN const CHAR* proc, IN INT signo);
	VOID     swc_send_signal_to_proc  (IN WCString proc, IN INT signo);
	CHAR*    swc_cls_apostrophe       (IN CHAR* strbuf);
	CHAR*    swc_cls_comma            (IN CHAR* strbuf);
	CHAR*    swc_cls_linefeed         (IN CHAR* strbuf);
	CHAR*    swc_cvt_chr_chr          (IN CHAR* strbuf, IN CHAR cfrom, IN CHAR cto);
	CHAR*    swc_str_chr_right        (IN const CHAR* src, IN const CHAR find, OUT CHAR** ptrptr);
	CHAR*    swc_cvt_chr_str          (IN const CHAR* src, 
                                       OUT CHAR      *dst,
                                       IN INT         ndstlen,
                                       IN const CHAR  cfind,
                                       IN const CHAR* scvt);
	CHAR*   swc_str_chr_right_csv     (IN const CHAR* src, IN const CHAR find, OUT CHAR** ptrptr);
	INT     swc_iconv                 (IN CHAR*       inbuf,
                                       OUT CHAR*       outbuf,
                                       IN INT         outlen,
                                       IN const CHAR* setfrom,
                                       IN const CHAR* setto);
	CHAR*    swc_toupper              (IN CHAR* strbuf);
	SWC_BOOL swc_is_file_exists       (IN const CHAR* sdir, IN const CHAR* sfile);
	SWC_BOOL swc_is_file_exists       (IN WCString sdir, IN WCString sfile);
	UINT     swc_cnt_match_file_in_dir(IN const CHAR* sdir, IN const CHAR* slike);
	UINT     swc_cnt_match_file_in_dir(IN WCString sdir, IN WCString slike);
	UINT     swc_get_matched_extension_files_in_dir(IN const CHAR* sDir, IN const CHAR* sExt, OUT WCArray<WCString>& arrPath);
	SWC_BOOL swc_is_dir_exists        (IN const CHAR* sdir);
	SWC_BOOL swc_is_dir_exists        (IN WCString sdir);	
	SWC_BOOL swc_rmdir                (IN const CHAR* sdir);
	UINT     swc_get_file_size        (IN const CHAR* spath);
	UINT     swc_get_file_size        (IN WCString spath);
	SWC_BOOL swc_is_file_size_over    (IN const CHAR* spath, IN UINT lsize);
	SWC_BOOL swc_is_file_size_over    (IN WCString spath, IN UINT lsize);
	SWC_BOOL swc_is_unicode_file      (IN const CHAR* spath);
	VOID     swc_sleep                (IN INT _sec, IN INT _usec);
	SWC_BOOL swc_get_remote_status    (IN const CHAR* seth, IN const CHAR* srcip, IN const CHAR* dstip);
	SWC_BOOL swc_get_remote_status    (IN WCString seth, IN WCString srcip, IN WCString dstip);
	SWC_BOOL swc_get_remote_status    (IN const CHAR* seth, IN UINT srcip, IN UINT dstip);
	SWC_BOOL swc_is_eth_exists        (IN WCString seth);
	UINT     swc_get_1st_mac          (OUT UCHAR cmac[]);
	SWC_BOOL swc_get_dns              (OUT CHAR* sdns, IN INT nlen);
	WCString swc_get_dns              ();
	VOID     swc_set_dns              (IN const CHAR* sdns);
	VOID     swc_set_dns              (IN WCString sdns);
	UINT     swc_get_eth_ip           (IN const CHAR* seth, OUT CHAR* sout, IN INT nlen);
	UINT     swc_get_eth_ip           (IN WCString seth, OUT WCString& sout, IN INT nlen);
	VOID     swc_get_local_time       (IN time_t ltime, OUT WCString& sret);
	VOID     swc_get_global_time      (IN time_t ltime, OUT WCString& sret);
	time_t   swc_lt_to_gt             (IN time_t lt);
	time_t   swc_gt_to_lt             (IN time_t gt);
	WCString ul2s                     (IN UINT ulval);
	WCString l2s                      (IN INT lval);
	WCString i2s                      (IN INT ival);
	WCString sh2s                     (IN SHORT shval);
	VOID     time_ltos                (IN UINT ltime, OUT CHAR* stime, IN INT len); 
	time_t   time_vtol                (IN INT year, IN INT mon, IN INT day, IN INT hour, IN INT min, IN INT sec); 
	time_t   time_stol                (IN const CHAR* strdate); 
	INT      time_get_year            (IN time_t ltime); 
	INT      time_get_mon             (IN time_t ltime); 
	INT      time_get_day             (IN time_t ltime); 
	INT      time_get_hour            (IN time_t ltime); 
	INT      time_get_min             (IN time_t ltime); 
	INT      time_get_sec             (IN time_t ltime); 
	INT      time_get_day_of_week     (IN time_t ltime);
	INT      time_get_week_of_mon     (IN time_t ltime);
	VOID     swc_get_smac_s           (OUT CHAR* smac, IN INT nlen, IN UINT lmacu, IN UINT lmacl);
	VOID     swc_get_smac_w           (OUT WCString& smac, IN UINT lmacu, IN UINT lmacl);
	UINT     swc_get_peer_ip          (IN INT sfd);
	UINT     swc_get_cpu_count        ();
	VOID     swc_smac_to_cmac         (IN const CHAR* smac, OUT UCHAR cmac[]);
	VOID     swc_smac_insert_semicolon(IN const CHAR* smac, OUT CHAR* sout, IN INT nlen);
	SWC_BOOL swc_system               (IN const CHAR* scmd);
	SWC_BOOL swc_isnumber_str         (IN WCString sNum);
}
#endif

//-----------------------------------------------------------------------------
#endif // __SWC_TOOL_H__
 
