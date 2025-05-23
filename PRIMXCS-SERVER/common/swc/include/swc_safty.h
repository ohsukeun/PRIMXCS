/*****************************************************************************
 *                                                                           *
 * File Name : swc_safty.h                                                   *
 * Theme     : safty functions                                               *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '13. 01. 15                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_safty.h
/// @author Park, sung soo
/// @date 2013-01-15
/// @brief safty functions

#ifndef __SWC_SAFTY_H__
#define __SWC_SAFTY_H__

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
#define SWC_SAFE(X) \
	X; \

#define SWC_SAFEA(X) \
	swc_mutex_lock(); \
	X; \
	swc_mutex_unlock();

//-----------------------------------------------------------------------------
// GLOBAL FUNCTIONS
//-----------------------------------------------------------------------------
extern void  swc_mutex_init     ();
extern void  swc_mutex_lock     ();
extern void  swc_mutex_unlock   ();
extern void  swc_mutex_destroy  ();
extern void* swc_malloc         (size_t nsize);
extern void* swc_realloc        (void* pptr, size_t nsize);
extern void  swc_free           (void* pptr);
extern int   swc_mutex_is_locked();
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif // __SWC_SAFTY_H__
 
