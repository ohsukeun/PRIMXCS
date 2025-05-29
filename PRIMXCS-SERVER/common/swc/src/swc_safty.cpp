/*****************************************************************************
 *                                                                           *
 * File Name : swc_safty.cpp                                                 *
 * Theme     : safty functions                                               *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '13. 01. 15                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_safty.cpp 
/// @author Park, sung soo
/// @date 2013-01-15
/// @brief safty functions

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <swc_safty.h>

#ifndef __SWC_SAFTY_CPP__
#define __SWC_SAFTY_CPP__

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// LOCAL DEFINIONS
//-----------------------------------------------------------------------------
pthread_mutex_t G_LOCK;

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------
void swc_mutex_init()
{
	pthread_mutex_init(&G_LOCK, NULL);
}

void swc_mutex_lock()
{
#ifndef NDEBUG
	fprintf(stdout, "swc_mutex_lock()\n");
#endif
	pthread_mutex_lock(&G_LOCK);
}

void swc_mutex_unlock()
{
#ifndef NDEBUG
	fprintf(stdout, "swc_mutex_unlock()\n");
#endif
	pthread_mutex_unlock(&G_LOCK);
}

void swc_mutex_destroy()
{
	pthread_mutex_destroy(&G_LOCK);
}

void* swc_malloc(size_t nsize)
{
	void* pret = NULL;

	if(0 == nsize) return NULL;
	swc_mutex_lock();
	pret = (void*)malloc(nsize);
	swc_mutex_unlock();
	return pret;
}

void* swc_realloc(void* pptr, size_t nsize)
{
	void* pret = NULL;

	if((NULL == pptr) || (0 == nsize)) return NULL;
	swc_mutex_lock();
	pret = (void*)realloc(pptr, nsize);
	swc_mutex_unlock();
	return pret;
}

void swc_free(void* pptr)
{
	swc_mutex_lock();
	free(pptr);
	swc_mutex_unlock();
}

int swc_mutex_is_locked()
{
//	if(1 <= G_LOCK.__data.__lock) return 1;
	return 0;
}

//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif // __SWC_SAFTY_CPP__
