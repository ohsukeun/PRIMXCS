/*****************************************************************************
 *                                                                           *
 * File Name : swc_sock. cpp                                                 *
 * Theme     : socket class                                                  *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '09. 11. 24                     *
 *                                                                           *
 *****************************************************************************/

#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/errno.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/un.h>

#include <logger.h>

#include <swc_tool.h>
#include <swc_debug.h>
#include <swc_sock.h>

#ifndef __SWC_SOCK_CPP__
#define __SWC_SOCK_CPP__

//-----------------------------------------------------------------------------
// METHOD DEFINITIONS
//-----------------------------------------------------------------------------
/* ----------------------------------------------------------------------------
    description : WCSock creator
    parameters  :
        - ()
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
WCSock::WCSock()
{
	m_SockFd        = INVALID_SOCKET;
	m_ListenFd      = INVALID_SOCKET;
	m_bNonBlockMode = FALSE;
	m_bListenSock   = FALSE;
	m_nErrNo        = 0;
}

/* ----------------------------------------------------------------------------
    description : WCSock destroyer
    parameters  :
        - (const CHAR*) I/O file path
    return      :
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
WCSock::~WCSock()
{
	if(INVALID_SOCKET != m_ListenFd) 
	{ Close(m_ListenFd); m_ListenFd = INVALID_SOCKET; }
	if(INVALID_SOCKET != m_SockFd)   
	{ Close(); m_SockFd = INVALID_SOCKET; }
}

/* ----------------------------------------------------------------------------
    description : WCSock listen
    parameters  :
		- (USHORT)      listen port
        - (const CHAR*) option (0 = any / 1 = specific ip)
		- (const CHAR*) listening ip string (can be null)
    return      : (SOCKET) listen port number
    history     :
        - 2009. 11. 24 created by Park. Sung soo
---------------------------------------------------------------------------- */
SOCKET WCSock::Listen(IN USHORT nport, IN INT option, IN const CHAR* strip)
{
	struct  sockaddr_in serv_addr;
	INT     serv_sock, rBind;
	INT     on=1;

	signal(SIGPIPE, SIG_IGN);

	// create socket 
	if(( serv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		m_nErrNo = D_SOCK_ECREATESOCK;
		return D_SOCK_ECREATESOCK;
	}

	// set socket option
	if(setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (CHAR *)&on, sizeof(INT)) == -1)
	{
		close(serv_sock);
		m_nErrNo = D_SOCK_ECREATESOCK;
		return D_SOCK_ESETSOCKOPT;
	}

	memset((CHAR *) &serv_addr, 0x00, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	if(0 == option) serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else            serv_addr.sin_addr.s_addr = inet_addr(strip);
	serv_addr.sin_port = htons(nport);

	// bind 
	do
	{
		rBind = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
		if(rBind == -1)
		{
			if(errno == EADDRINUSE)
			{ 
				// port already open
				_LOG_ERR("port(%d) already open", nport);
			}
			else
			{
				_LOG_ERR("port(%d) open failed", nport);
			}
			close(serv_sock);
			m_nErrNo = D_SOCK_EBIND;
			return D_SOCK_EBIND;
		}
	}while(rBind == -1);

	// listen 
	if (listen(serv_sock, 10) == -1)
	{
		close(serv_sock);
		m_nErrNo = D_SOCK_ELISTEN;
		return D_SOCK_ELISTEN;
	}

	m_nErrNo = 0;
	m_ListenFd = serv_sock;
	return serv_sock;
}

/* ----------------------------------------------------------------------------
    description : read from socket descriptor
    parameters  :
		- (SOCKET) socket descriptor
		- (CHAR*)  reading buffer
		- (INT)    buffer length
		- (INT)    timeout (second)
    return      : read data length
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCSock::Read(IN SOCKET sfd, OUT CHAR* buf, IN INT len, IN INT timeout)
{
	struct timeval  tv, *ptv;
	fd_set sock_set;
	INT    got;
	INT    ret;
	INT    remain;
	CHAR   *cp;

	if(sfd < 0) return sfd;
	remain = len;
	cp = buf;
	len = 0;
	while (remain > 0)
	{
		FD_ZERO(&sock_set);
		FD_SET(sfd, &sock_set);

		if (timeout > 0)
		{
			tv.tv_sec = timeout;
			tv.tv_usec = 0;
			ptv = &tv;
		}
		else ptv = NULL;
		ret = select(sfd + 1, &sock_set, NULL, NULL, ptv);
		if (ret < 0)
		{
			if (errno == EINTR) continue;
			else
			{
				m_nErrNo = D_SOCK_ECLOSE;
				return D_SOCK_ECLOSE;
			}
		}
		else if (ret == 0) 
		{
			m_nErrNo = D_SOCK_ETIMEOUT;
			return D_SOCK_ETIMEOUT;
		}
		got = read(sfd, cp, remain);
		if (got < 0)
		{
			if (errno == EINTR) continue;
			else
			{
				m_nErrNo = D_SOCK_ECLOSE;
				return D_SOCK_ECLOSE;
			}
		}
		else if (got == 0) break;
		remain -= got;
		cp += got;
		len += got;
	}
	m_nErrNo = 0;
	return len;
}

/* ----------------------------------------------------------------------------
    description : read from socket descriptor 
    parameters  :
		- (CHAR*) reading buffer
		- (INT)   buffer length
		- (INT)   timeout (second)
    return      : read data length
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCSock::Read(OUT CHAR* buf, IN INT len, IN INT timeout)
{
	if(INVALID_SOCKET == m_SockFd) return 0;
	return Read(m_SockFd, buf, len, timeout);
}

/* ----------------------------------------------------------------------------
    description : receive from socket descriptor 
    parameters  :
		- (SOCKET) socket descriptor
		- (CHAR*)  reading buffer
		- (INT)    buffer length
		- (INT)    timeout (second)
    return      : read data length
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCSock::Receive(IN SOCKET sfd, OUT CHAR* buf, IN INT len, IN INT timeout)
{
	return Read(sfd, buf, len, timeout);
}

/* ----------------------------------------------------------------------------
    description : receive from socket descriptor 
    parameters  :
		- (SOCKET) socket descriptor
		- (CHAR*)  reading buffer
		- (INT)    buffer length
		- (INT)    timeout (second)
    return      : read data length
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCSock::Receive(OUT CHAR* buf, IN INT len, IN INT timeout)
{
	if(INVALID_SOCKET == m_SockFd) return 0;
	return Read(m_SockFd, buf, len, timeout);
}

/* ----------------------------------------------------------------------------
    description : write data to the socket descriptor 
    parameters  :
		- (SOCKET) socket descriptor
		- (CHAR*)  sending buffer
		- (INT)    buffer length
		- (INT)    timeout (second)
    return      : write data length
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCSock::Write(IN SOCKET sfd, IN CHAR* buf, IN INT len, IN INT timeout)
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
		SWC::swc_sleep(0, 1);

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
						m_nErrNo = D_SOCK_ETIMEOUT;
						return D_SOCK_ETIMEOUT;
					}
					else continue;
				}
				else 
				{
					m_nErrNo = D_SOCK_ECLOSE;
					return D_SOCK_ECLOSE;
				}
			}
			else if(sent == 0) break;

			remain -= sent;
			cp += sent;
			len += sent;
		}
	}
	m_nErrNo = 0;
	return len;
}

/* ----------------------------------------------------------------------------
    description : write data to the socket descriptor 
    parameters  :
		- (CHAR*) sending buffer
		- (INT)   buffer length
		- (INT)   timeout (second)
    return      : write data length
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCSock::Write(IN CHAR* buf, IN INT len, IN INT timeout)
{
	if(INVALID_SOCKET == m_SockFd) return 0;
	return Write(m_SockFd, buf, len, timeout);
}

/* ----------------------------------------------------------------------------
    description : send data to the socket descriptor 
    parameters  :
		- (SOCKET) socket descriptor
		- (CHAR*)  sending buffer
		- (INT)    buffer length
		- (INT)    timeout (second)
    return      : sent data length
    history     : 
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCSock::Send(IN SOCKET sfd, IN CHAR* buf, IN INT len, IN INT timeout)
{
	return Write(sfd, buf, len, timeout);
}

/* ----------------------------------------------------------------------------
    description : send data to the socket descriptor 
    parameters  :
		- (CHAR*) sending buffer
		- (INT)   buffer length
		- (INT)   timeout (second)
    return      : (INT) sent data length
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
INT WCSock::Send(IN CHAR* buf, IN INT len, IN INT timeout)
{
	if(INVALID_SOCKET == m_SockFd) return 0;
	return Write(m_SockFd, buf, len, timeout);
}

/* ----------------------------------------------------------------------------
    description : connect to the server
    parameters  :
		- (const CHAR*) destination address
		- (USHORT)      destination port
    return      : (SOCKET) connected socket descriptor
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
SOCKET WCSock::Connect(IN const CHAR* dstaddr, IN USHORT dstport)
{
	struct sockaddr_in  sin;
	struct hostent      *hostent;
	INT                 rfd = 0;
	INT                 n;

	memset((CHAR *) &sin,0x00,  sizeof(sin));

	if ((sin.sin_addr.s_addr = inet_addr(dstaddr)) == (in_addr_t)-1)
	{
		if ((hostent = gethostbyname(dstaddr)) == NULL)
		{
			m_nErrNo = D_SOCK_EINVADDR;
			return D_SOCK_EINVADDR;
		}

		memcpy(&sin.sin_addr, hostent->h_addr, hostent->h_length);
	}

	sin.sin_port = htons(dstport);
	sin.sin_family = AF_INET;

	if ((rfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		m_nErrNo = D_SOCK_ECREATESOCK;
		return D_SOCK_ECREATESOCK;
	}

	n = sizeof(struct sockaddr_in);

	if (connect(rfd, (struct sockaddr *)&sin, n) == -1)
	{
		close(rfd);
		m_nErrNo = D_SOCK_ECONN;
		return D_SOCK_ECONN;
	}

	return rfd;
}

/* ----------------------------------------------------------------------------
    description : connect to the server with timeout
    parameters  :
		- (const CHAR*) destination address
		- (USHORT)      destination port
		- (const CHAR*) bind address
		- (UINT)        timeout (second)
    return      : (SOCKET) connected socket descriptor
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
SOCKET WCSock::ConnectEx(IN const CHAR* dstaddr, IN USHORT dstport, IN const CHAR* bndaddr, IN UINT ltimeout)
{
	struct sockaddr_in sin, bnd;
	struct hostent* hostent;
	INT sfd = 0, opts = 0, rfd = 0, lerr = 0, lret = 0, nbnd = 0, on = 1;
	socklen_t n = 0;
	struct timeval _tv;
	fd_set _fdset;

	if((sin.sin_addr.s_addr = inet_addr(dstaddr)) == (in_addr_t)-1)
	{
		if(NULL == (hostent = gethostbyname(dstaddr))) 
		{
			m_nErrNo = D_SOCK_EINVADDR;
			return D_SOCK_EINVADDR;
		}
		memcpy(&sin.sin_addr, hostent->h_addr, hostent->h_length);
	}
	sin.sin_port = htons(dstport);
	sin.sin_family = AF_INET;
	if(0 > (sfd = socket(AF_INET, SOCK_STREAM, 0))) 
	{
		m_nErrNo = D_SOCK_ECREATESOCK;
		return D_SOCK_ECREATESOCK;
	}
	n = sizeof(struct sockaddr_in);
	if(NULL != bndaddr)
	{
		if(-1 == setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (CHAR *)&on, sizeof(INT)))
		{ 
			close(sfd); 
			m_nErrNo = D_SOCK_ESETSOCKOPT;
			return D_SOCK_ESETSOCKOPT; 
		}
		memset(&bnd, 0x00, sizeof(bnd));
		bnd.sin_family = AF_INET;
		bnd.sin_addr.s_addr = inet_addr(bndaddr);
		nbnd = bind(sfd, (struct sockaddr*)&bnd, sizeof(struct sockaddr));
		if(-1 == nbnd) 
		{ 
			close(sfd); 
			m_nErrNo = D_SOCK_EBIND;
			return D_SOCK_EBIND; 
		}
	}
	opts = fcntl(sfd, F_GETFL);
	if(0 > opts) 
	{ 
		close(sfd); 
		m_nErrNo = D_SOCK_ESETSOCKOPT;
		return D_SOCK_ESETSOCKOPT; 
	}
	opts |= O_NONBLOCK;
	if(0 > fcntl(sfd, F_SETFL, opts)) 
	{ 
		close(sfd); 
		m_nErrNo = D_SOCK_ESETSOCKOPT;
		return D_SOCK_ESETSOCKOPT; 
	}
	if(0 > connect(sfd, (struct sockaddr*)&sin, n))
	{
		if((EAGAIN == errno) || (EINPROGRESS == errno))
		{
			FD_ZERO(&_fdset);
			FD_SET(sfd, &_fdset);
			_tv.tv_sec = ltimeout;
			_tv.tv_usec = 0;
			rfd = select(FD_SETSIZE, NULL, &_fdset, NULL, &_tv);
			if(0 >= rfd) 
			{ 
				close(sfd); 
				m_nErrNo = D_SOCK_ECONN;
				return D_SOCK_ECONN; 
			}
		}
		else 
		{
			m_nErrNo = D_SOCK_ECONN;
			return D_SOCK_ECONN;
		}
	}
	n = sizeof(lerr);
	if(0 > (lret = getsockopt(sfd, SOL_SOCKET, SO_ERROR, &lerr, &n)))
	{
		close(sfd);
		m_nErrNo = D_SOCK_ECONN;
		return D_SOCK_ECONN;
	}
	if(0 != lerr)
	{
		printf("sock_connect_ex() - %s\n", strerror(lerr));
		close(sfd);
		return D_SOCK_ECONN;
	}
	opts = fcntl(sfd, F_GETFL);
	if(0 > opts) 
	{ 
		close(sfd); 
		m_nErrNo = D_SOCK_ESETSOCKOPT;
		return D_SOCK_ESETSOCKOPT; 
	}
	if(O_NONBLOCK & opts) opts ^= O_NONBLOCK;
	if(0 > fcntl(sfd, F_SETFL, opts)) 
	{ 
		close(sfd); 
		m_nErrNo = D_SOCK_ESETSOCKOPT;
		return D_SOCK_ESETSOCKOPT; 
	}
	m_nErrNo = 0;
	return sfd;
}

/* ----------------------------------------------------------------------------
    description : close socket forcely
    parameters  :
		- (SOCKET) socket descriptor
    return      : ()
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCSock::Close(IN SOCKET sfd)
{
	CHAR _tbuf[1024];

	if(0 >= sfd) return;
	struct linger tmp = {1, 0};
	setsockopt(sfd, SOL_SOCKET, SO_LINGER, (CHAR*)&tmp, sizeof(struct linger));
	shutdown(sfd, SHUT_RDWR);
	Read(sfd, _tbuf, 1024, 5);
	close(sfd);
}

/* ----------------------------------------------------------------------------
    description : close socket 
    parameters  :
		- (SOCKET) socket descriptor
    return      : ()
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCSock::CloseNoForce(IN SOCKET sfd)
{
	if(0 >= sfd) return;
	shutdown(sfd, 0);
	close(sfd);
}

/* ----------------------------------------------------------------------------
    description : close socket forcely
    parameters  :
    return      : ()
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCSock::Close()
{
	CHAR _tbuf[1024];

	if(0 >= m_SockFd) return;
	struct linger tmp = {1, 0};
	setsockopt(m_SockFd, SOL_SOCKET, SO_LINGER, (CHAR*)&tmp, sizeof(struct linger));
	shutdown(m_SockFd, SHUT_RDWR);
	Read(m_SockFd, _tbuf, 1024, 5);
	close(m_SockFd);
}

/* ----------------------------------------------------------------------------
    description : close socket
    parameters  :
    return      : ()
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCSock::CloseNoFprce()
{
	if(0 >= m_SockFd) return;
	shutdown(m_SockFd, 0);
	close(m_SockFd);
}

/* ----------------------------------------------------------------------------
    description : set non-block mode
    parameters  :
		- (SOCKET) socket descriptor
    return      : ()
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCSock::SetNonBlockMode(IN SOCKET sfd)
{
	INT opts = 0;

	if(0 >= sfd) return;

	opts = fcntl(sfd, F_GETFL);
	if(0 > opts)
	{
		_LOG_ERR("fcntl(F_GETFL) failed");
		return;
	}
	opts |= O_NONBLOCK;
	if(0 > fcntl(sfd, F_SETFL, opts))
	{
		_LOG_ERR("fcntl(F_SETFL) failed");
		return;
	}
}

/* ----------------------------------------------------------------------------
    description : set non-block mode
    parameters  :
		- (SOCKET) socket descriptor
    return      : ()
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCSock::SetNonBlockMode()
{
	INT opts = 0;

	if(0 >= m_SockFd) return;

	opts = fcntl(m_SockFd, F_GETFL);
	if(0 > opts)
	{
		_LOG_ERR("fcntl(F_GETFL) failed");
		return;
	}
	opts |= O_NONBLOCK;
	if(0 > fcntl(m_SockFd, F_SETFL, opts))
	{
		_LOG_ERR("fcntl(F_SETFL) failed");
		return;
	}
}

/* ----------------------------------------------------------------------------
    description : set block mode
    parameters  :
		- (SOCKET) socket descriptor
    return      : ()
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
VOID WCSock::SetBlockMode(IN SOCKET nsfd)
{
	INT opts = 0;

	if(0 >= nsfd) return;

	opts = fcntl(nsfd, F_GETFL);
	if(0 > opts)
	{
		_LOG_ERR("fcntl(F_GETFL) failed");
		return;
	}
	if(opts & O_NONBLOCK) 
	{
		opts ^= O_NONBLOCK;
		if(0 > fcntl(nsfd, F_SETFL, opts))
		{
			_LOG_ERR("fcntl(F_SETFL) failed");
			return;
		}
	}
}

/* ----------------------------------------------------------------------------
    description : check if connected to the server
    parameters  :
		- ()
    return      : ()
    history     :
        - 2009. 11. 25 created by Park. Sung soo
---------------------------------------------------------------------------- */
SWC_BOOL WCSock::IsConnected()
{
	if(INVALID_SOCKET == m_SockFd) return FALSE;
	return TRUE;
}

//-----------------------------------------------------------------------------
#endif // __SWC_SOCK_CPP__
