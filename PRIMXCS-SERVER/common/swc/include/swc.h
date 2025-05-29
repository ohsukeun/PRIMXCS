/*****************************************************************************
 *                                                                           *
 * File Name : swc.h                                                         *
 * Theme     : std:list wrapper class                                        *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '10. 03. 25                     *
 *             2. modifief by Park, sung soo '21. 08. 31                     *
 *              (1) swc_wstring.h add                                        *
 *                                                                           *
 *****************************************************************************/

/*
 version : 1.0.2
 history  
  1. date : 2013. 11. 11
   (1) version 1.0.1
   (2) class CSSLIONode 
       m_pXmlBuf add
       Get/Set method add
   (3) class WCString 
       constructor WCString(std::string str) add
  2. date : 201i4. 10. 08
   (1) version 1.0.2
   (2) swc_ssl_ionode.h, cpp -> receive timeout methods add
*/

/* TIP
 edit /usr/share/vim/vim74/syntax/cpp.vim as below
 <from...>
 syn keyword cppType     bool wchar_t 
 <to ...>
 syn keyword cppType     bool wchar_t SOCKET DWORD PDWORD LPDWORD DWORD32 FLOAT HANDLE HCALL HRESULT WORD VOID PVOID LPVOID INT INT8 INT16 INT32 UINT UINT8 UINT16 UINT32 UINT64 INT64 LONG_PTR ULONG_PTR SIZE_T DOUBLE LONG LONG32 LONG64 ULONG ULONG64 SHORT USHORT CHAR PCHAR UCHAR STRING BYTE PBYTE LPBYTE SWC_BOOL BOOL PBOOL LPBOOL BOOLEAN PBOOLEAN IN OUT WCArray WCList WCMap WCMapPtrToPtr WCMapStrToPtr WCMapStrToStr WCMemPool CPairContainer CTriadContainer WCPtrArray WCPtrList WCQueue WCSock WCString WCUDSock WCWString CIONode CSSLIONode CSSLHandler CSSLWorkTh CUDSHandler CWorkTh SWC TRUE FALSE INVALID_SOCKET
 */

#include <swc_base.h>
#include <swc_ptr_list.h>
#include <swc_tool.h>
#include <swc_debug.h>
#include <swc_sock.h>
#include <swc_udsock.h>
#include <swc_map_ptr_to_ptr.h>
#include <swc_map_str_to_ptr.h>
#include <swc_map_str_to_str.h>
#include <swc_pseudo_set.h>
#include <swc_ptr_list.h>
#include <swc_ptr_array.h>
#include <swc_queue.h>
#include <swc_ptr_queue.h>
#include <swc_string.h>
#include <swc_wstring.h>
#include <swc_mem_pool.h>
#include <swc_ip_obj.h>
#include <swc_safty.h>

#ifndef __SWC_H__
#define __SWC_H__

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#endif // __SWC_H__
 
