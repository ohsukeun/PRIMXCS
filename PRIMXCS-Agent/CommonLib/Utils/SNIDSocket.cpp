#include "StdAfx.h"
#include "SNIDSocket.h"


CSNIDSocket::CSNIDSocket(void)
{
	m_Socket			= INVALID_SOCKET;
	m_bConnect			= FALSE;
	m_nLastError		= 0;
	m_nSvcPort			= 0;
	m_nTimeOut			= 60 * 5;
	m_nConnectTimeOut	= PRIMX_SOCK_CONNECT_TIMEOUT;

	m_SSLSocket			= NULL;
	m_pCTX				= NULL;
	m_pMeth				= NULL;
	m_bSSL				= TRUE;

	m_nLocalIP			= 0;

}

CSNIDSocket::~CSNIDSocket(void)
{

}

BOOL CSNIDSocket::SetSocketInit()
{
	if(FALSE == m_bSocketInit){
		WSADATA	wsaData;
		WORD	wVerRequested = MAKEWORD(2, 2);
	
		if (0 != WSAStartup(wVerRequested, &wsaData)) {
			return FALSE;
		}
		if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion)) {
			WSACleanup();
			return FALSE;
		}
		m_bSocketInit = TRUE;
	}	
	return TRUE;
}

BOOL CSNIDSocket::SetSSLSocketInit()
{
	if(FALSE == m_bSocketInit){
		if(FALSE == SetSocketInit()){
			return FALSE;
		}
	}
	if(FALSE == m_bSSLInit){		
		SSLeay_add_ssl_algorithms();
		SSL_load_error_strings();	
		m_bSSLInit = TRUE;
	}
	return TRUE;
}

BOOL CSNIDSocket::SetBind(CString strBindIP)
{
	SOCKADDR_IN Bind_Addr;
	int nAddrLen = sizeof(Bind_Addr);
	memset((char *) &Bind_Addr, 0x00, sizeof(SOCKADDR_IN));
	WSAStringToAddress((LPTSTR )LPCTSTR(strBindIP),AF_INET,NULL,(LPSOCKADDR)&Bind_Addr,&nAddrLen);
	Bind_Addr.sin_port = htons(0);
		
	if(-1 == bind(m_Socket, (SOCKADDR *)&Bind_Addr, sizeof(SOCKADDR_IN))){
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
		return FALSE;
	}
	return TRUE;
}

BOOL CSNIDSocket::Connect(CString strIP, unsigned short nPort,BOOL bSSL)
{
	m_bSSL = bSSL;

	if(TRUE == m_bSSL){
		if(FALSE == SetSSLSocketInit()){
			return FALSE;
		}
	}
	else{
		if(FALSE == SetSocketInit()){
			return FALSE;
		}
	}

	m_Socket = socket(AF_INET,SOCK_STREAM,0); 

	if(0 >= m_Socket){
		m_Socket = INVALID_SOCKET;
		return FALSE;
	}

	SOCKADDR_IN ServerAddrInfo;
	int nAddrLen = sizeof(ServerAddrInfo);
	m_strSvcIP = strIP;
	m_nSvcPort = nPort;

	memset(&ServerAddrInfo,0,sizeof(SOCKADDR_IN));
/*	ServerAddrInfo.sin_addr.s_addr = inet_addr(WtoAConverter(LPCTSTR(IP)));
	ServerAddrInfo.sin_port = htons(Port);
	ServerAddrInfo.sin_family = AF_INET;*/	

	WSAStringToAddress((LPTSTR )LPCTSTR(m_strSvcIP),AF_INET,NULL,(LPSOCKADDR)&ServerAddrInfo,&nAddrLen);
	ServerAddrInfo.sin_port = htons(m_nSvcPort);

	if (m_Socket <= 0 ) {
		return FALSE;
	}
	if(INADDR_NONE == ServerAddrInfo.sin_addr.s_addr)
	{
		//char ServerIP[1024] = { 0 };
		//strncpy_s(ServerIP, 1024, WtoAConverter(m_strSvcIP), 1024);
		//
		//hostent* pHostent = gethostbyname((char FAR*)ServerIP);
		//if (pHostent == NULL)
		//{
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
			return FALSE;
		//}
//		ServerAddrInfo.sin_addr.s_addr = ((struct in_addr FAR *)(pHostent->h_addr))->s_addr;
	}

	if(SOCKET_ERROR == ConnectEx(m_Socket,(struct sockaddr*)&ServerAddrInfo,sizeof(ServerAddrInfo),m_nConnectTimeOut)) {
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
		return FALSE;
	}

	int nBufsize;
	nBufsize = size_t(PRIMX_DEFAULT_SIZE); 
	if(SOCKET_ERROR == (setsockopt(m_Socket, SOL_SOCKET, SO_SNDBUF, (char*)&nBufsize, sizeof(nBufsize)))) {
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
		return FALSE;
	}

	nBufsize = size_t(PRIMX_DEFAULT_SIZE); 
	if(SOCKET_ERROR == (setsockopt(m_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&nBufsize, sizeof(nBufsize)))){
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
		return FALSE;
	}

	if(TRUE == m_bSSL){

		// TLS 1.2지원
		m_pMeth = (SSL_METHOD*)TLS_method(); // TLSv1_2_method();

		if( (m_pCTX = SSL_CTX_new( m_pMeth )) == NULL )
		{
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
			return FALSE;
		}

		// TLS 1.2 설정
		//if (0 == SSL_CTX_set_cipher_list(m_pCTX, CIPHER_FORMAT_STRING)) // CIPHER_FORMAT_STRING : "AES256+SHA256:3DES+SHA256:@STRENGTH"
		//{
		//	closesocket(m_Socket);
		//	m_Socket = INVALID_SOCKET;
		//	return FALSE;
		//}
		
		SSL_CTX_set_min_proto_version(m_pCTX, TLS1_2_VERSION);
		SSL_CTX_set_max_proto_version(m_pCTX, TLS1_2_VERSION);


		if( (m_SSLSocket = SSL_new(m_pCTX)) == NULL )
		{
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
			return FALSE;
		}

		SSL_set_fd(m_SSLSocket, int(m_Socket));

		if(SOCKET_ERROR == SSLConnectEx(m_Socket,m_SSLSocket,m_nConnectTimeOut))
		{
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
			return FALSE;
		}
	
	}

	SOCKADDR_IN LocalAddr;
	int nLocalAddrLen=sizeof(LocalAddr);

	if(SOCKET_ERROR != getsockname(m_Socket, (struct sockaddr *)&LocalAddr, &nLocalAddrLen)){		
		m_nLocalIP = LocalAddr.sin_addr.S_un.S_addr;
	}	

	unsigned long nMode = 1;
	int nErr = ioctlsocket(m_Socket,FIONBIO,&nMode);
	if(nErr == SOCKET_ERROR) return FALSE;

	return (m_bConnect = TRUE);
}

void CSNIDSocket::SetTimeOut(int nTimeOutSec)
{
	m_nTimeOut = nTimeOutSec;
}

int CSNIDSocket::Send(void *pSendBuf, int nBufLen, int nFlags)
{
	int nSendTotalLen = 0;
	int nRe = 0;
	if(PRIMX_UNIT_MAX_SEND_LEN < nBufLen){
		char *pTempSendBuf = (char*)pSendBuf;
		while(0 < nBufLen){
			int nSendLen = (PRIMX_UNIT_MAX_SEND_LEN <= nBufLen?PRIMX_UNIT_MAX_SEND_LEN:nBufLen);
			if(TRUE == m_bSSL){
				nRe = SSLWriteEx(pTempSendBuf,nSendLen,nFlags);
			}
			else{
				nRe = SendEx(pTempSendBuf,nSendLen,nFlags);
			}
			if(0 >= nRe){
				break;
			}
			nBufLen = nBufLen - nRe;
			pTempSendBuf = pTempSendBuf + nRe;
			nSendTotalLen = nSendTotalLen + nRe;
		}
	}
	else{
		if(TRUE == m_bSSL){
			nSendTotalLen = SSLWriteEx(pSendBuf,nBufLen,nFlags);
		}
		else{
			nSendTotalLen = SendEx(pSendBuf,nBufLen,nFlags);
		}
	}

	return nSendTotalLen;
}

int CSNIDSocket::SendEx(void *pSendBuf, int nBufLen, int nFlags)
{
	if(0 >= m_Socket) return 0;

	char *lpBuf = (char*)pSendBuf;
	unsigned long nSendSeekLen = nBufLen;
	unsigned long nTotalSendLen = 0;

	__time32_t tStartTime;
	__time32_t tEndTime;
	_time32(&tStartTime);

	while(1){
		if(0 < nSendSeekLen){
			Sleep(5);
			int nSendLen = send(m_Socket,lpBuf,nSendSeekLen,nFlags);
			if(0 == nSendLen) return 0;
			if(SOCKET_ERROR == nSendLen){
				m_nLastError = WSAGetLastError();
				if(WSAEWOULDBLOCK == m_nLastError){	
					if(0 < m_nTimeOut){
						_time32(&tEndTime);
						if((tEndTime - tStartTime) >= m_nTimeOut){
							return SOCKET_ERROR;
						}
					}
					continue;
				}
				else{
					m_strError = GetWSAErrToString();
				}
				return 0;//SOCKET_ERROR;
			}
			_time32(&tStartTime);
			nSendSeekLen = nSendSeekLen - nSendLen;
			nTotalSendLen = nTotalSendLen + nSendLen;
			lpBuf = lpBuf + nSendLen;
		}
		else{			
			break;
		}
	} 	

	return nTotalSendLen;
}

int CSNIDSocket::ReceiveFile(CString strFilePath,int nFileSize, int nFlags)
{
	int nRe = 0;
	int nTotalReceiveLen = 0;
	int nDataSize = nFileSize;
	if(PRIMX_RECEIVE_FILE_UNIT_MAX_LEN <= nDataSize){

		errno_t err =0;
		FILE *fp = NULL;
		err = _tfopen_s(&fp, LPCTSTR(strFilePath), _T("wb") );
		if(0 == err ){
			char* pBuffer = new char[PRIMX_RECEIVE_FILE_UNIT_MAX_LEN];
			ZeroMemory(pBuffer,PRIMX_RECEIVE_FILE_UNIT_MAX_LEN);
			while(0 < nDataSize){
				int nUnitLen = (PRIMX_RECEIVE_FILE_UNIT_MAX_LEN <= nDataSize?PRIMX_RECEIVE_FILE_UNIT_MAX_LEN:nDataSize);

				ZeroMemory(pBuffer,PRIMX_RECEIVE_FILE_UNIT_MAX_LEN);
				nRe = Receive(pBuffer,nUnitLen,nFlags); 
				if(nUnitLen == nRe){
					fwrite( pBuffer,nUnitLen, 1, fp );
				}
				else{
					if(0 == nRe){
						nTotalReceiveLen = 0;
					}
					break;
				}
				nDataSize = nDataSize - nUnitLen;
				nTotalReceiveLen = nTotalReceiveLen + nUnitLen;
			}
			fclose( fp );	
			_SafeDeleteArray(pBuffer);
		}
	}
	else{
		char* pBuffer = new char[nFileSize];
		ZeroMemory(pBuffer,nFileSize);
		nRe = Receive(pBuffer,nFileSize,nFlags); 
		if(nFileSize == nRe){
			errno_t err =0;
			FILE *fp = NULL;
			err = _tfopen_s(&fp, LPCTSTR(strFilePath), _T("wb") );
			if(0 == err ){
				fwrite( pBuffer,nFileSize, 1, fp );
				fclose( fp );
			}	
		}
		nTotalReceiveLen = nRe;
		_SafeDeleteArray(pBuffer);
	}
	if(nTotalReceiveLen != nFileSize){
		::DeleteFile(strFilePath);
	}
	return nTotalReceiveLen;

}

void CSNIDSocket::ReceiveProgress()
{

}

int CSNIDSocket::Receive(char* pRecvBuf, int nBufLen, int nFlags)
{
	if(0 >= m_Socket) return 0;

	char* lpBuf = pRecvBuf;
	unsigned long nRecvSeekLen = nBufLen;
	unsigned long nTotalRecvLen = 0;
	unsigned long nProgressSize = 0;

	__time32_t tStartTime;
	__time32_t tEndTime;
	_time32(&tStartTime);

	if(TRUE == m_bSSL){

		if (NULL == m_SSLSocket) return 0;

		SSL_set_mode(m_SSLSocket, SSL_MODE_AUTO_RETRY);

		while(1){
			if(0 < nRecvSeekLen){
				Sleep(5);
				int nRecvLen = SSL_read(m_SSLSocket,lpBuf,nRecvSeekLen);
				if (0 == nRecvLen) {
					return 0;
				}
				if(SOCKET_ERROR == nRecvLen){
					m_nLastError = WSAGetLastError();
					if(WSAEWOULDBLOCK == m_nLastError){	
						if(0 < m_nTimeOut){
							_time32(&tEndTime);
							if((tEndTime - tStartTime) >= m_nTimeOut){
								return SOCKET_ERROR;
							}
						}
						continue;
					}
					else{
						int nErrorNo = SSL_get_error(m_SSLSocket,nRecvLen);
						if(SSL_ERROR_WANT_READ == nErrorNo){
							if(0 < m_nTimeOut){
								_time32(&tEndTime);
								if((tEndTime - tStartTime) >= m_nTimeOut){
									return SOCKET_ERROR;
								}
							}
							continue;
						}
						return 0;
					}
				}
				_time32(&tStartTime);
				nRecvSeekLen = nRecvSeekLen - nRecvLen;
				nTotalRecvLen = nTotalRecvLen + nRecvLen;
				lpBuf = lpBuf + nRecvLen;
				nProgressSize = nProgressSize + nRecvLen;
				if ((1024 * 1024) <= nProgressSize) {
					ReceiveProgress();
					nProgressSize = 0;
				}	
			}
			else{			
				break;
			}
		} 	
	}
	else{
		while(1){
			if(0 < nRecvSeekLen){
				Sleep(5);
				int nRecvLen = recv(m_Socket,lpBuf,nRecvSeekLen,nFlags);
				if(0 == nRecvLen) return 0;
				if(SOCKET_ERROR == nRecvLen){
					m_nLastError = WSAGetLastError();
					if(WSAEWOULDBLOCK == m_nLastError){	
						if(0 < m_nTimeOut){
							_time32(&tEndTime);
							if((tEndTime - tStartTime) >= m_nTimeOut){
								return SOCKET_ERROR;
							}
						}
						continue;
					}
					else{
						m_strError = GetWSAErrToString();
					}
					return 0;//SOCKET_ERROR;
				}
				_time32(&tStartTime);
				nRecvSeekLen = nRecvSeekLen - nRecvLen;
				nTotalRecvLen = nTotalRecvLen + nRecvLen;
				lpBuf = lpBuf + nRecvLen;
				nProgressSize = nProgressSize + nRecvLen;
				if ((1024 * 1024) <= nProgressSize) {
					ReceiveProgress();
					nProgressSize = 0;
				}
			}
			else{			
				break;
			}
		} 	
	}

	return nTotalRecvLen;
}

int  CSNIDSocket::Close()
{
	int nRe = 0;

	if(TRUE == m_bSSL){
		if(NULL != m_SSLSocket){
			SSL_shutdown(m_SSLSocket);
			nRe = CloseSocketEx(m_Socket);
			SSL_free(m_SSLSocket);
			SSL_CTX_free(m_pCTX);
			m_Socket		= INVALID_SOCKET;
			m_SSLSocket		= NULL;
			m_pCTX			= NULL;
			m_pMeth			= NULL;
		}
		else{
			nRe = CloseSocketEx(m_Socket);
			m_Socket = INVALID_SOCKET;
		}
		m_bConnect	= FALSE;
	}
	else{
		nRe = CloseSocketEx(m_Socket);
		m_Socket = INVALID_SOCKET;
		m_bConnect = FALSE;
	}
	return nRe;
}

int CSNIDSocket::ConnectEx(SOCKET ServerSocket, const sockaddr* pServerName, int nServerNamenamelen, long nTimeout)
{
	int            nRc = 0;
	ULONG          ulB;
	struct timeval tTime;
	fd_set         FdSet;

	ulB = TRUE; 
	ioctlsocket(ServerSocket, FIONBIO, &ulB);

	if(SOCKET_ERROR == connect(ServerSocket, pServerName, sizeof(SOCKADDR))) {
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			FD_ZERO(&FdSet);
			FD_SET(ServerSocket, &FdSet);
			tTime.tv_sec  = nTimeout / 1000L;
			tTime.tv_usec = (nTimeout % 1000) * 1000;
			nRc = select(int(ServerSocket) +1, NULL, &FdSet, NULL, &tTime);
		}
	}
	ulB = FALSE; 
	ioctlsocket(ServerSocket, FIONBIO, &ulB);

	return (nRc > 0) ? 0 : SOCKET_ERROR;
}

BOOL CSNIDSocket::IsConnect()
{
	return m_bConnect; 
}

BOOL CSNIDSocket::ReConnect()
{
	BOOL bRe = FALSE;

	if(TRUE == Connect(m_strSvcIP, m_nSvcPort)){
		bRe = TRUE;
	}	

	return bRe;	
}

CString CSNIDSocket::GetWSAErrToString()
{
	CString strError;
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	strError = (LPCTSTR)lpMsgBuf;
	LocalFree( lpMsgBuf );

	strError.Replace(_T("\r"), _T(""));
	strError.Replace(_T("\n"), _T(""));

	return strError;

}

void CSNIDSocket::SetConnectTimeOut(int nTimeOutSec)
{
	m_nConnectTimeOut = nTimeOutSec*1000;
}

int CSNIDSocket::SSLWriteEx(void *lpBufData, int nBufLen, int nFlags)
{
	if (NULL == m_SSLSocket) return 0;

	char *lpBuf = (char*)lpBufData;

	unsigned long nReCount = 0;
	unsigned long nTotalCount = nBufLen;
	unsigned long nReTotalCount = 0;

	__time32_t tStartTime;
	__time32_t tEndTime;
	_time32(&tStartTime);

	SSL_set_mode(m_SSLSocket, SSL_MODE_ENABLE_PARTIAL_WRITE);

	while(1){
		if(0 < nTotalCount){
			Sleep(5);
			nReCount = SSL_write(m_SSLSocket,lpBuf,nTotalCount);
			if(0 == nReCount) return 0;
			if(SOCKET_ERROR == nReCount){
				int Err = SSL_get_error(m_SSLSocket,nReCount);
#ifdef _DEBUG
				switch(Err) {
				case SSL_ERROR_NONE:
					TRACE(_T("SSL_ERROR_NONE \n"));
					break;
				case SSL_ERROR_WANT_WRITE:
					TRACE(_T("SSL_ERROR_WANT_WRITE \n"));
					break;
				case SSL_ERROR_WANT_READ:
					TRACE(_T("SSL_ERROR_WANT_READ \n"));
					break;
				case SSL_ERROR_WANT_X509_LOOKUP:
					TRACE(_T("SSL_ERROR_WANT_X509_LOOKUP \n"));
					break;
				case SSL_ERROR_SYSCALL:
					TRACE(_T("SSL_ERROR_SYSCALL \n"));
					break;
				case SSL_ERROR_SSL:
					{
						char tmp[1042] = {0};
						ERR_error_string_n(ERR_get_error(),tmp,1042);
						TRACE("SSL_ERROR_SSL : %s \n",tmp );
						break;
					}
				case SSL_ERROR_ZERO_RETURN:
					TRACE(_T("SSL_ERROR_ZERO_RETURN \n"));
					break;
				}
#endif
				if(SSL_ERROR_WANT_WRITE == Err){
					//if(0 < nReTotalCount) 	return nReTotalCount;
					if(0 < m_nTimeOut){
						_time32(&tEndTime);
						if((tEndTime - tStartTime) >= m_nTimeOut){
							return SOCKET_ERROR;
						}
					}
					continue;
				}
				return SOCKET_ERROR;
			}
			_time32(&tStartTime);
			nTotalCount = nTotalCount - nReCount;
			nReTotalCount = nReTotalCount + nReCount;
			lpBuf = lpBuf + nReCount;
		}
		else{			
			break;
		}
	} 	
	return nReTotalCount;

}

int CSNIDSocket::SSLConnectEx(SOCKET ServerSocket,SSL* pSSL,int nTimeOut)
{
	ULONG          ulB;
	struct timeval tTime;
	fd_set		   wrFdSet;
	fd_set		   rdFdSet;

	if (SSL_is_init_finished(pSSL)) return 0;
	if (!SSL_in_connect_init(pSSL)) ::SSL_set_connect_state (pSSL);

	ulB = TRUE; 
	ioctlsocket(ServerSocket, FIONBIO, &ulB);

	int nStatus = 0;
	FD_ZERO(&wrFdSet);
	FD_ZERO(&rdFdSet);

	do
	{
		nStatus = SSL_connect(pSSL);
		switch (SSL_get_error (pSSL, nStatus))
		{
		case SSL_ERROR_NONE:
			nStatus = 0;               
			break;                  
		case SSL_ERROR_WANT_WRITE:
			FD_SET(ServerSocket, &wrFdSet);
			nStatus = 1;               
			break;

		case SSL_ERROR_WANT_READ:
			FD_SET(ServerSocket, &rdFdSet);
			nStatus = 1;               
			break;

		case SSL_ERROR_ZERO_RETURN:
			nStatus = -1;
			break;

		case SSL_ERROR_SYSCALL:
			if (WSAGetLastError() == WSAEWOULDBLOCK && nStatus == -1)
			{
				nStatus = 1;               
				if (SSL_want_write (pSSL)){
					FD_SET(ServerSocket, &wrFdSet);
				}
				else{
					if (SSL_want_read (pSSL)){
						FD_SET(ServerSocket, &rdFdSet);
					}
					else{
						nStatus = -1;        
					}
				}
			}
			else{
				nStatus = -1;
			}
			break;

		default:
			nStatus = -1;
			break;
		}

		if (nStatus == 1)
		{
			tTime.tv_sec  = nTimeOut / 1000L;
			tTime.tv_usec = (nTimeOut % 1000) * 1000;

			nStatus = select (int(ServerSocket) + 1,
				(0 == rdFdSet.fd_count ? 0 : &rdFdSet),
				(0 == wrFdSet.fd_count ? 0 : &wrFdSet),
				0,
				&tTime);

			if (nStatus >= 1){
				nStatus = 1;
			}
			else{                 
				nStatus = -1;
			}
		}

	} while (nStatus == 1 && !SSL_is_init_finished(pSSL));

	ulB = FALSE; 
	ioctlsocket(ServerSocket, FIONBIO, &ulB);

	return (nStatus == -1 ? SOCKET_ERROR : 0);


}
int CSNIDSocket::CloseSocketEx(SOCKET& Socket)
{
	if (INVALID_SOCKET != Socket) {
		LINGER Tmp = { 1,0 };
		setsockopt(Socket, SOL_SOCKET, SO_LINGER, (char*)&Tmp, sizeof(LINGER));
		shutdown(Socket, 0);
		closesocket(Socket);
		Socket = INVALID_SOCKET;
		return ERROR_SUCCESS;
	}
	return -1;
}