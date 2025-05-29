/*****************************************************************************
 *                                                                           *
 * File Name : swc_udsock.cpp                                                *
 * Theme     : unix domain socket class                                      *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 11. 23                     *
 *                                                                           *
 *****************************************************************************/

/// @file swc_udsock.cpp
/// @author Park, sung soo
/// @date 2009-11-23
/// @brief unix domain socket class 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>

#include <logger.h>
#include <swc_tool.h>
#include <swc_debug.h>
#include <swc_udsock.h>

#ifndef __SWC_UDSOCK_CPP__
#define __SWC_UDSOCK_CPP__

//-----------------------------------------------------------------------------
// METHOD DEFINITIONS
//-----------------------------------------------------------------------------
/* ----------------------------------------------------------------------------
    description : WCUDSock creator
    parameters  :
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
WCUDSock::WCUDSock()
{
    memset(m_sIoFile, 0x00, SWC_MAX_PATH_LEN);
    m_ConnectSfd = INVALID_SOCKET;
    m_ListenSfd  = INVALID_SOCKET;
}

/* ----------------------------------------------------------------------------
    description : WCUDSock creator
    parameters  :
        (const CHAR*) I/O file path
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
WCUDSock::WCUDSock(IN const CHAR* siofile)
{
    if(NULL != siofile)
    	snprintf(m_sIoFile, SWC_MAX_PATH_LEN, "%s", siofile);

    m_ConnectSfd = INVALID_SOCKET;
    m_ListenSfd  = INVALID_SOCKET;
}

/* ----------------------------------------------------------------------------
    description : WCUDSock destroyer
    parameters  :
        (const CHAR*) I/O file path
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
WCUDSock::~WCUDSock()
{
    if(0 < m_ConnectSfd)
    {
        WCUDSock::sock_close(m_ConnectSfd);
        m_ConnectSfd = INVALID_SOCKET;
    }
    if(0 < m_ListenSfd)
    {
        WCUDSock::sock_close(m_ListenSfd);
        m_ConnectSfd = INVALID_SOCKET;
        Destroy();
    }
}

/* ----------------------------------------------------------------------------
    description : set I/O file path
    parameters  :
        (const CHAR*) I/O file path
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCUDSock::SetIOFile(IN const CHAR* siofile)
{
    if(NULL == siofile) return;
    snprintf(m_sIoFile, SWC_MAX_PATH_LEN, "%s", siofile);
}

/* ----------------------------------------------------------------------------
    description : I/O file remove
    parameters  :
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCUDSock::Destroy()
{
    if((0 < m_ListenSfd) && (0 < strlen(m_sIoFile)))
    {
        WCUDSock::sock_destory_uds(m_sIoFile);
        memset(m_sIoFile, 0x00, SWC_MAX_PATH_LEN);
    }
}

/* ----------------------------------------------------------------------------
    description : listen as a server
    parameters  :
    return      : (SOCKET) return listen socket descritpor
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
SOCKET WCUDSock::Listen()
{
    SOCKET nret = INVALID_SOCKET;

    if(0 == strlen(m_sIoFile)) return INVALID_SOCKET;
    if(0 >= (nret = WCUDSock::sock_listen_uds(m_sIoFile)))
    {
		_LOG_ERR("failed");
        SockErrMsg(nret);
        return nret;
    }
    m_ListenSfd = nret;
    return nret;
}

/* ----------------------------------------------------------------------------
    description : connect to the listening server
    parameters  :
    return      : (SOCKET) rerurn client socket descriptor
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
SOCKET WCUDSock::Connect()
{
    SOCKET nret = INVALID_SOCKET;

    if(0 == strlen(m_sIoFile)) return INVALID_SOCKET;
    if(0 >= (nret = WCUDSock::sock_connect_uds(m_sIoFile)))
    {
		_LOG_ERR("failed");
        SockErrMsg(nret);
        return nret;
    }
    m_ConnectSfd = nret;
    return nret;
}

/* ----------------------------------------------------------------------------
    description : close client socket 
    parameters  :
    return      :
    history     :
        - 2018. 10. 04 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCUDSock::CloseConnectSfd()
{
	if(INVALID_SOCKET != m_ConnectSfd)
	{
		Close(m_ConnectSfd);
		m_ConnectSfd = INVALID_SOCKET;
	}
}

/* ----------------------------------------------------------------------------
    description : close socket descriptor
    parameters  :
        (SOCKET) target socket descriptor
    return      : 
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCUDSock::Close(IN SOCKET nsfd)
{
    if(0 < nsfd) WCUDSock::sock_close(nsfd);
}

/* ----------------------------------------------------------------------------
    description : send data 
    parameters  :
        (SOCKET) target socket descriptor
        (VOID*)    sending buffer pointer
        (INT)      sending buffer length
        (INT)      time-out
    return      : (INT) sending size. if under zero, error occured
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCUDSock::Send(IN SOCKET nsfd, IN VOID* pbuf, IN INT nlen, IN INT ntmout)
{
    INT nret = 0;

    if(0 >= nsfd)
    {
		_LOG_ERR("invalid socket descriptor");
        return 0;
    }
    if(0 >= (nret = WCUDSock::sock_write(nsfd, (CHAR*)pbuf, nlen, ntmout)))
    {
		_LOG_ERR("failed");
        SockErrMsg(nret);
        return nret;
    }
    return nret;
}

/* ----------------------------------------------------------------------------
    description : receive data 
    parameters  :
        (SOCKET) target socket descriptor
        (VOID*)  receiving buffer pointer
        (INT)    receiving buffer length
        (INT)    time-out
    return      : (INT) receiving size. if under zero, error occured
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCUDSock::Recv(IN SOCKET nsfd, OUT VOID* pbuf, IN INT nlen, IN INT ntmout)
{
    INT nret = 0;

    if(0 >= nsfd)
    {
		_LOG_ERR("invalid socket descriptor");
        return 0;
    }
    if(0 >= (nret = WCUDSock::sock_read(nsfd, (CHAR*)pbuf, nlen, ntmout)))
    {
		if(D_SOCK_ETIMEOUT != nret)
		{
			_LOG_ERR("failed");
        	SockErrMsg(nret);
		}
        return nret;
    }
    return nret;
}

/* ----------------------------------------------------------------------------
    description : set socket descriptor as non-block mode
    parameters  :
        (SOCKET) target socket descriptor
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCUDSock::SetNonBlockMode(IN SOCKET nsfd)
{
    INT opts = 0;

    if(0 >= nsfd) return;

    opts = fcntl(nsfd, F_GETFL);
    if(0 > opts)
    {
		_LOG_ERR("fcntl(F_GETFL)");
        return;
    }
    opts |= O_NONBLOCK;
    if(0 > fcntl(nsfd, F_SETFL, opts))
    {
		_LOG_ERR("fcntl(F_SETFL)");
        return;
    }
}

/* ----------------------------------------------------------------------------
    description : socket close
    parameters  :
        (INT) socket descriptor
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCUDSock::sock_close(IN INT fd)
{
	CHAR _tbuf[1024];

	if(0 >= fd) return;
	struct linger tmp = {1, 0};
	setsockopt(fd, SOL_SOCKET, SO_LINGER, (CHAR*)&tmp, sizeof(struct linger));
	shutdown(fd, SHUT_RDWR);
	WCUDSock::sock_read(fd, _tbuf, 1024, 5);
	close(fd);	
}

/* ----------------------------------------------------------------------------
    description : socket destory for unix domain socket
    parameters  :
        (const CHAR*) pipe file path
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCUDSock::sock_destory_uds(IN const CHAR* str_file)
{
	if(0 == access(str_file, F_OK)) unlink(str_file);
}

/* ----------------------------------------------------------------------------
    description : socket listen on unix domain socket
    parameters  :
        (const CHAR*) pipe file path
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCUDSock::sock_listen_uds(IN const CHAR* str_file)
{
	struct sockaddr_un name;
	INT sock = -1;
	size_t size = 0;

	sock = socket(PF_UNIX, SOCK_STREAM, 0);
	if(0 > sock)
	{
		return D_SOCK_ECREATESOCK;
	}

#ifdef __SWC_OS_LINUX__
	name.sun_family = AF_FILE;
#elif __SWC_OS_MAC__
	name.sun_family = AF_UNIX;
#endif
	strcpy(name.sun_path, str_file);
	size = sizeof(name);
	if(0 > bind(sock, (struct sockaddr*)&name, size))
	{
		return D_SOCK_EBIND;
	}

	if(-1 == listen(sock, 10))
	{
		close(sock);
		return D_SOCK_ELISTEN;
	}
	return sock;
}

/* ----------------------------------------------------------------------------
    description : socket connect on unix domain socket
    parameters  :
        (const CHAR*) pipe file path
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCUDSock::sock_connect_uds(IN const CHAR* str_file)
{
	struct sockaddr_un name;
	INT sock = -1;
	size_t size = 0;

	sock = socket(PF_UNIX, SOCK_STREAM, 0);
	if(0 > sock)
	{
		return D_SOCK_ECREATESOCK;
	}

#ifdef __SWC_OS_LINUX__
	name.sun_family = AF_FILE;
#elif __SWC_OS_MAC__
	name.sun_family = AF_UNIX;
#endif
	strcpy(name.sun_path, str_file);
	size = sizeof(name);
	if(0 > connect(sock, (struct sockaddr*)&name, size))
	{
		close(sock);
		return D_SOCK_ECONN;
	}
	return sock;
}

/* ----------------------------------------------------------------------------
    description : socket write to unix domain socket
    parameters  :
        (const CHAR*) pipe file path
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCUDSock::sock_write(IN INT sfd, IN CHAR *buf, IN INT len, IN INT timeout)
{
	time_t prev_time = 0, cur_time = 0;
	INT sent = 0, remain = 0, tsum = 0;
	CHAR* cp;

	if(sfd < 0) return sfd;

	remain = len;
	cp = buf;
	len = 0;

	time(&cur_time);
	prev_time = cur_time;

	while(remain > 0)
	{
		usleep(1);

		if(0 < remain)
		{
			sent = send(sfd, cp, remain, 0);

			if(sent < 0)
			{
				if(errno == EWOULDBLOCK)
				{
					time(&cur_time);
					if(prev_time != cur_time)
					{
						prev_time = cur_time;
						tsum++;
					}

					if(timeout < tsum)
					{
						return D_SOCK_ETIMEOUT;
					}
					else continue;
				}
				else return D_SOCK_ECLOSE;
			}
			else if(sent == 0) break;

			remain -= sent;
			cp += sent;
			len += sent;
		}
	}
	return len;
}

/* ----------------------------------------------------------------------------
    description : socket read from unix domain socket
    parameters  :
        (const CHAR*) pipe file path
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCUDSock::sock_read(IN INT sfd, OUT CHAR *buf, IN INT len, IN INT timeout)
{
	struct timeval  tv, *ptv;
	fd_set sock_set;
	INT    got;
	INT    ret;
	INT    remain;
	CHAR   *cp;
	//INT  val;

	if(sfd < 0)
	{
		return sfd;
	}

	//fprintf(stdout, "sock_read len : %d\n", len);
	remain = len;
	cp = buf;
	len = 0;
	//fprintf(stdout, "reamin len : %d\n", remain);
	while (remain > 0)
	{
		FD_ZERO(&sock_set);
		FD_SET(sfd, &sock_set);

		if (timeout > 0)
		{
			tv.tv_sec = (UINT)timeout;
			tv.tv_usec = 0;
			ptv = &tv;
		}
		else
		{
			ptv = NULL;
		}

		ret = select(sfd + 1, &sock_set, NULL, NULL, ptv);

		if (ret < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			else
			{
			//  perror("recv");
				return D_SOCK_ECLOSE;
			}
		}
		else if (ret == 0)
		{
			return D_SOCK_ETIMEOUT;
		}

		got = read(sfd, cp, remain);

		if (got < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			else
			{
				return D_SOCK_ECLOSE;
			}
		}
		else if (got == 0)
		{
			break;
		}

		remain -= got;
		cp += got;
		len += got;
	}

	return len;
}

//-----------------------------------------------------------------------------
#endif // __SWC_UDSOCK_CPP__
