#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")


#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")



// TCP 연결 타임 아웃 시간
#ifdef _DEBUG
#define PRIMX_SOCK_CONNECT_TIMEOUT		10000			// 10초
#else
#define PRIMX_SOCK_CONNECT_TIMEOUT		20000			// 20초
#endif

// 기본 송수신 버퍼 사이즈 (바이트)
#define PRIMX_DEFAULT_SIZE				(1024*1024)		// 1Mbyte

// 데이터 전송 시 한번에 송신하는 최대 바이트. 
#define PRIMX_UNIT_MAX_SEND_LEN			(1024*14)		// 14Kbyte

// 파일 수신 시 한번에 수신하는 최대 사이즈
#define PRIMX_RECEIVE_FILE_UNIT_MAX_LEN	(10 * 1024 * 1024)		// 10Mbyte

#define CIPHER_FORMAT_STRING		"AES256+SHA256:3DES+SHA256:@STRENGTH"

class CSNIDSocket
{
public:
	CSNIDSocket(void);
	~CSNIDSocket(void);

	virtual void ReceiveProgress();

public:
	BOOL Connect(CString strIP,unsigned short nPort,BOOL bSSL = TRUE);

	void SetConnectTimeOut(int nTimeOutSec);
	BOOL ReConnect();
	BOOL IsConnect();

	int Close();
	int Receive(char* pRecvBuf, int nBufLen, int nFlags = 0 );
	int ReceiveFile(CString strFilePath,int nFileSize, int nFlags = 0 );
	int Send( void* pSendBuf, int nBufLen, int nFlags = 0 );
	void SetTimeOut(int  nTimeOutSec);

	CString GetWSAErrToString();
	

private:
	BOOL SetSocketInit();
	BOOL SetSSLSocketInit();

	int ConnectEx(SOCKET ServerSocket, const sockaddr* pServerName, int nServerNamenamelen, long nTimeout);
	int SendEx( void* pSendBuf, int nBufLen, int nFlags);
	int SSLWriteEx(void *lpBufData, int nBufLen, int nFlags = 0);
	int SSLConnectEx(SOCKET ServerSocket,SSL* pSSL,int nTimeOut=0);
	int CloseSocketEx(SOCKET& Socket);

	SOCKET				m_Socket;
	BOOL				m_bConnect;
	CString				m_strError;
	int					m_nLastError;
	int					m_nTimeOut;
	int					m_nConnectTimeOut;

	BOOL				m_bSSL;
	SSL*				m_SSLSocket;
	SSL_CTX*			m_pCTX;
	const SSL_METHOD*	m_pMeth;

	static BOOL			m_bSocketInit;
	static BOOL			m_bSSLInit;	

protected:
	CString				m_strSvcIP;
	unsigned short		m_nSvcPort;
	ULONG				m_nLocalIP; // network order(big-endian)

	BOOL SetBind(CString strBindIP);


};

