/*****************************************************************************
 *                                                                           *
 * File Name : swc_ssl_func.h                                                *
 * Theme     : wrapped functions of ssl related functions                    *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '14. 07. 10                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_ssl_func.h
/// @author Park, sung soo
/// @date 2014-07-10
/// @brief wrapped functions of ssl related functions

#include <openssl/ssl.h>
#include <openssl/err.h>

#ifndef __SWC_SSL_FUNC_H__
#define __SWC_SSL_FUNC_H__

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// GLOBAL DEFINITIONS
//-----------------------------------------------------------------------------
/*
-------------------------------------------------------------------------------
original ssl functions                 => wrapped functions
-------------------------------------------------------------------------------
SSL_new                                | WF_SSL_new
SSL_free                               | WF_SSL_free
SSL_accept                             | WF_SSL_accept
SSL_connect                            | WF_SSL_connect
SSL_get_fd                             | WF_SSL_get_fd
SSL_get_rfd                            | WF_SSL_get_rfd
SSL_get_wfd                            | WF_SSL_get_wfd
SSL_set_fd                             | WF_SSL_set_fd
SSL_set_rfd                            | WF_SSL_set_rfd
SSL_set_wfd                            | WF_SSL_set_wfd
SSL_shutdown                           | WF_SSL_shutdown
SSL_set_shutdown                       | WF_SSL_set_shutdown
SSL_get_shutdown                       | WF_SSL_get_shutdown
SSL_state                              | WF_SSL_state
SSL_get_error                          | WF_SSL_get_error
SSL_write                              | WF_SSL_write
SSL_read                               | WF_SSL_read
SSL_CTX_new                            | WF_SSL_CTX_new
SSL_CTX_free                           | WF_SSL_CTX_free
SSL_CTX_set_cipher_list                | WF_SSL_CTX_set_cipher_list
SSL_CTX_use_certificate_file           | WF_SSL_CTX_use_certificate_file
SSL_CTX_set_default_passwd_cb_userdata | WF_SSL_CTX_set_default_passwd_cb_userdata
SSL_CTX_check_private_key              | WF_SSL_CTX_check_private_key
SSL_CTX_use_PrivateKey_file            | WF_SSL_CTX_use_PrivateKey_file
-------------------------------------------------------------------------------
*/

extern void     WF_SSL_init();
extern void     WF_SSL_exit();
extern SSL*     WF_SSL_new(SSL_CTX *ctx);
extern void     WF_SSL_free(SSL *ssl);
extern int      WF_SSL_accept(SSL *ssl);
extern int      WF_SSL_connect(SSL *ssl);
extern int      WF_SSL_get_fd(const SSL *ssl);
extern int      WF_SSL_get_rfd(const SSL *ssl);
extern int      WF_SSL_get_wfd(const SSL *ssl);
extern int      WF_SSL_set_fd(SSL *ssl, int fd);
extern int      WF_SSL_set_rfd(SSL *ssl, int fd);
extern int      WF_SSL_set_wfd(SSL *ssl, int fd);
extern int      WF_SSL_shutdown(SSL *ssl);
extern void     WF_SSL_set_shutdown(SSL *ssl, int mode);
extern int      WF_SSL_get_shutdown(const SSL *ssl);
extern int      WF_SSL_get_state(SSL *ssl );
extern int      WF_SSL_get_error(const SSL *ssl, int ret);
extern int      WF_SSL_write(SSL *ssl, const void *buf, int num);
extern int      WF_SSL_read(SSL *ssl, void *buf, int num);
extern SSL_CTX* WF_SSL_CTX_new(const SSL_METHOD *method);
extern void     WF_SSL_CTX_free(SSL_CTX *ctx);
extern int      WF_SSL_CTX_set_cipher_list(SSL_CTX *ctx, const char *str);
extern int      WF_SSL_CTX_use_certificate_file(SSL_CTX *ctx, const char *file, int type);
extern void     WF_SSL_CTX_set_default_passwd_cb_userdata(SSL_CTX *ctx, void *u);
extern int      WF_SSL_CTX_check_private_key(const SSL_CTX *ctx);
extern int      WF_SSL_CTX_use_PrivateKey_file(SSL_CTX *ctx, const char *file, int type);
extern int      WF_SSL_set_mode(SSL *ssl, long mode);

//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif // __SWC_SSL_FUNC_H__
