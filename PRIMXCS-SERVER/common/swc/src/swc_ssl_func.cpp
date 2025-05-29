/*****************************************************************************
 *                                                                           *
 * File Name : swc_ssl_func.cpp                                              *
 * Theme     : wrapped functions of ssl related functions                    *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '14. 07. 10                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_ssl_func.cpp  
/// @author Park, sung soo
/// @date 2014-07-10
/// @brief wrapped functions of ssl related functions

#include <pthread.h>
#include <swc_ssl_func.h>

#ifndef __SWC_SSL_FUNC_CPP__
#define __SWC_SSL_FUNC_CPP__

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// LOCAL DEFINITION
//-----------------------------------------------------------------------------
bool SWC_G_IS_SSL_MUTEX_INIT = false;
bool SWC_G_IS_SSL_MUTEX_EXIT = false;
pthread_mutex_t SWC_G_SSL_LOCK;

//-----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------
void WF_SSL_init()
{
	if(true == SWC_G_IS_SSL_MUTEX_INIT) return;
	pthread_mutex_init(&SWC_G_SSL_LOCK, NULL);
	SWC_G_IS_SSL_MUTEX_INIT = true;
}

void WF_SSL_exit()
{
	if(true == SWC_G_IS_SSL_MUTEX_EXIT) return;
	pthread_mutex_destroy(&SWC_G_SSL_LOCK);
	SWC_G_IS_SSL_MUTEX_EXIT = true;
}

SSL* WF_SSL_new(SSL_CTX *ctx)
{
	SSL* pRet = NULL;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	pRet = SSL_new(ctx);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return pRet;
}

void WF_SSL_free(SSL *ssl)
{
	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	SSL_free(ssl);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
}

int WF_SSL_accept(SSL *ssl)
{
	int nRet = 0;
	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_accept(ssl);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_connect(SSL *ssl)
{
	int nRet = 0;
	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_connect(ssl);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_get_fd(const SSL *ssl)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_get_fd(ssl);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_get_rfd(const SSL *ssl)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_get_rfd(ssl);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_get_wfd(const SSL *ssl)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_get_wfd(ssl);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_set_fd(SSL *ssl, int fd)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_set_fd(ssl, fd);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_set_rfd(SSL *ssl, int fd)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_set_rfd(ssl, fd);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_set_wfd(SSL *ssl, int fd)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_set_wfd(ssl, fd);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_shutdown(SSL *ssl)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_shutdown(ssl);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

void WF_SSL_set_shutdown(SSL *ssl, int mode)
{
	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	SSL_set_shutdown(ssl, mode);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
}

int WF_SSL_get_shutdown(const SSL *ssl)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_get_shutdown(ssl);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_get_state(SSL *ssl )
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_get_state(ssl);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_get_error(const SSL *ssl, int ret)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_get_error(ssl, ret);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_write(SSL *ssl, const void *buf, int num)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_write(ssl, buf, num);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_read(SSL *ssl, void *buf, int num)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_read(ssl, buf, num);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

SSL_CTX* WF_SSL_CTX_new(const SSL_METHOD* method)
{
	SSL_CTX* pRet = NULL;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	pRet = SSL_CTX_new((SSL_METHOD*)method);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return pRet;
}

void WF_SSL_CTX_free(SSL_CTX *ctx)
{
	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	SSL_CTX_free(ctx);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
}

int WF_SSL_CTX_set_cipher_list(SSL_CTX *ctx, const char *str)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_CTX_set_cipher_list(ctx, str);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_CTX_use_certificate_file(SSL_CTX *ctx, const char *file, int type)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_CTX_use_certificate_file(ctx, file, type);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

void WF_SSL_CTX_set_default_passwd_cb_userdata(SSL_CTX *ctx, void *u)
{
	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	SSL_CTX_set_default_passwd_cb_userdata(ctx, u);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
}

int WF_SSL_CTX_check_private_key(const SSL_CTX *ctx)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_CTX_check_private_key(ctx);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_CTX_use_PrivateKey_file(SSL_CTX *ctx, const char *file, int type)
{
	int nRet = 0;

	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = SSL_CTX_use_PrivateKey_file(ctx, file, type);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

int WF_SSL_set_mode(SSL *ssl, long mode)
{
	int nRet = 0;
	pthread_mutex_lock(&SWC_G_SSL_LOCK);
	nRet = (int)SSL_set_mode(ssl, mode);
	pthread_mutex_unlock(&SWC_G_SSL_LOCK);
	return nRet;
}

//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif // __SWC_SSL_FUNC_CPP__
