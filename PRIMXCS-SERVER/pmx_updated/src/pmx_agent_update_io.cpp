/// @file pmx_agent_update_io.cpp
/// @author Park, sung soo
/// @date 2023-07-21
/// update channel i/o handling

#include <errno.h>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <BodyCompressor.h>
#include <pmx_io.h>
#include <pmx_db_io.h>
#include <pmx_agent_update_io.h>

#ifndef __PMX_AGENT_UPDATE_IO_CPP__
#define __PMX_AGENT_UPDATE_IO_CPP__

//-----------------------------------------------------------------------------
// METHOD DEFINITIONS
//-----------------------------------------------------------------------------
CAgentUpdateIO::CAgentUpdateIO()
{
	m_pDBObj    = NULL;
}

CAgentUpdateIO::~CAgentUpdateIO()
{
	if (NULL != m_pDBObj)
	{
		m_pDBObj->Disconnect();
		delete m_pDBObj;
		m_pDBObj = NULL;
	}
}

SWC_BOOL CAgentUpdateIO::ConnectToDatabase(IN WCString sHost, 
										   IN USHORT nPort, 
										   IN WCString sUser, 
										   IN WCString sPwd, 
										   IN WCString sDBName)
{
	if (NULL != m_pDBObj) delete m_pDBObj;
	m_pDBObj = new CMySQLDBObj();

	m_pDBObj->Init(LPCSTR(sHost), 
				   LPCSTR(sDBName),
				   "UTF8",
				   "UTF8",
				   LPCSTR(sUser),
				   LPCSTR(sPwd),
				   nPort);
	m_pDBObj->InitSSL(TRUE, 
					  PMX_CU_MARIADB_CLIENT_KEY_PEM_FILE_PATH,
					  PMX_CU_MARIADB_CLIENT_CERT_PEM_FILE_PATH,
					  PMX_CU_MARIADB_CA_PEM_FILE_PATH,
					  NULL, 
					  NULL);
	
	if (FALSE == m_pDBObj->Connect())
	{
		_LOG_ERR("database connection failed: (%d) %s", m_pDBObj->GetDatabaseErrorNo(), m_pDBObj->GetLastErrorString());
		delete m_pDBObj;
		m_pDBObj = NULL;
		return FALSE;
	}
	m_pDBObj->SetAutoCommit(FALSE); // auto-commit off

	return TRUE;
}

VOID CAgentUpdateIO::RunCheck(UINT nInterval)
{
	if (NULL == m_pDBObj) return;

	WCString sModuleName = "";
	sModuleName.Format("%s[%d]", PMX_MODULE_NAME_UPDATED_AGENT_IO_HANDLER, GetID());

	CDBIO::RunSessionMngQuery(m_pDBObj, LPCSTR(sModuleName));
}

VOID CAgentUpdateIO::RunJob(IN CSSLIONode* pNode)
{
	if (NULL == pNode) return;
	if (INVALID_SOCKET == pNode->GetFd()) return;

	/*
	_LOG_DBG("%s | %s",
		(TRUE == pNode->IsRecvMode() ? "RECV MODE ON" : "RECV MODE OFF"),
		(TRUE == pNode->IsSendMode() ? "SEND MODE ON" : "SEND MODE OFF"));
	*/
	
	// receive mode
	if (TRUE == pNode->IsRecvMode())
	{
		INT nRecv = 0;
		if (0 > (nRecv = pNode->Read(pNode->GetRecvPtr() + pNode->GetRecvPos(), 
									 pNode->GetRecvLen() - pNode->GetRecvPos())))
		{
			if (EINTR == errno || EAGAIN == errno)
			{
				SWC::swc_sleep(0, 1);
				if (TRUE == pNode->CheckRecvTimeOut())
				{
					_LOG_ERR("Read() timeout (20 seconds). std:%d\n", pNode->GetFd());
					GetParentPtr()->SetEnableOneShotNode(pNode);
					pNode->InitAsRecv(GetParentPtr()->m_nInitRecvLen);
					GetParentPtr()->SetDisableOneShotNode(pNode);
					return;
				}
			}
			else
			{
				_LOG_ERR("Read() failed. std:%d. error:%s\n", pNode->GetFd(), strerror(errno));
				GetParentPtr()->DetachNode(pNode);
				return;
			}
		}
		else if (0 == nRecv)
		{
			SWC::swc_sleep(0, 1);
			if (TRUE == pNode->CheckRecvTimeOut())
			{
				_LOG_ERR("Read() timeout (20 seconds). std:%d\n", pNode->GetFd());
				GetParentPtr()->SetEnableOneShotNode(pNode);
				pNode->InitAsRecv(GetParentPtr()->m_nInitRecvLen);
				GetParentPtr()->SetDisableOneShotNode(pNode);
				return;
			}
		}
		else // nRecv > 0
		{
			pNode->SetRecvPosAdd(nRecv);

			// check if all data received, run command
			if (pNode->GetRecvLen() == pNode->GetRecvPos())
			{
				if ((pNode->GetRecvLen() == GetParentPtr()->m_nInitRecvLen) &&
					(FALSE == pNode->IsHeadRcvd()))
				{
					_LOG_DBG("header received");
					pNode->SetHeadRcvd();
					// header received ...
					// check header validity
					if (FALSE == CheckHeadValidity(pNode))
					{
						GetParentPtr()->SetEnableOneShotNode(pNode);
						pNode->InitAsRecv(GetParentPtr()->m_nInitRecvLen);
						GetParentPtr()->SetDisableOneShotNode(pNode);
					}
					else
					{
						// reg. receive mode as body length
						UINT nBdLen = GetBodyLen(pNode);
						if (0 == nBdLen)
						{
							GetParentPtr()->SetEnableOneShotNode(pNode);
							if (TRUE == CmdOperate(pNode))
							{
								_LOG_DBG("body length is zero");
							}
							pNode->InitAsRecv(GetParentPtr()->m_nInitRecvLen);
							GetParentPtr()->SetDisableOneShotNode(pNode);
						}
						else
						{
							if (SOCK_MAX_RECV_LEN >= nBdLen)
							{
								pNode->SetRecvMode(nBdLen);
								_LOG_DBG("body length is %u. reg.", nBdLen);
								GetParentPtr()->PushJobQueue(pNode);
							}
							else // exceptional condition
							{
								_LOG_ERR("abnormal body length (%u - more than 100 Mb). session reset", nBdLen);
								GetParentPtr()->DetachNode(pNode);
								return;
							}
						}
					}
				}
				else
				{
					_LOG_DBG("body received");

					// run command
					GetParentPtr()->SetEnableOneShotNode(pNode);
					CmdOperate(pNode);
					pNode->InitAsRecv(GetParentPtr()->m_nInitRecvLen);
					GetParentPtr()->SetDisableOneShotNode(pNode);
				}
			}
			// receive more
			else if (pNode->GetRecvLen() > pNode->GetRecvPos())
			{
				GetParentPtr()->PushJobQueue(pNode);
			}
			else
			{
				GetParentPtr()->DetachNode(pNode);
			}
		}
	}
	else
	{
		//_LOG_ERR("not receive mode. std:%d", pNode->GetFd());
	}

	// send mode
	if (TRUE == pNode->IsSendMode())
	{
		INT nSent = 0;
		if (0 >= (nSent = pNode->Write(pNode->GetSendPtr() + pNode->GetSendPos(),
									   pNode->GetSendLen() - pNode->GetSendPos())))
		{
			if (EINTR == errno || EAGAIN == errno)
			{
				SWC::swc_sleep(0, 1);
				if (0 < (pNode->GetSendLen() - pNode->GetSendPos()))
				{
					GetParentPtr()->PushJobQueue(pNode);
				}
				return;
			}
			_LOG_ERR("send() failed. std:%d. error:%s", pNode->GetFd(), strerror(errno));
			pNode->GetSSLError(TRUE);
			GetParentPtr()->DetachNode(pNode);
		}
		else
		{
			pNode->SetSendPosAdd(nSent);
			_LOG_DBG("%d bytes sent (total %d bytes)", pNode->GetSendPos(), pNode->GetSendLen());

			// check if all data sent, init. send-mode
			if (pNode->GetSendLen() == pNode->GetSendPos())
			{
				// if file data sending mode
				if (TRUE == pNode->IsFileSendMode())
				{
					if(NULL != pNode->GetFDesc())
					{
						// next block send
						if (pNode->GetFSentBytes() < pNode->GetFTotalSize())
						{
							pNode->SetFileSendNext();
							if (pNode->GetFSentBytes() == pNode->GetFTotalSize())
							{
								_LOG_DBG("file sending completed (total %u, sent %u)", pNode->GetFTotalSize(), pNode->GetFSentBytes());
								pNode->ClrFileSendMode();
								pNode->ResetSend();
							}
							else
							{
								GetParentPtr()->PushJobQueue(pNode);
							}
						}
						else
						{
							_LOG_DBG("file sending completed (total %us,. sent %u)", pNode->GetFTotalSize(), pNode->GetFSentBytes());
						}
					}
				}
				else
				{
					pNode->ResetSend();
				}
			}
			else if (pNode->GetSendLen() > pNode->GetSendPos())
			{
				GetParentPtr()->PushJobQueue(pNode);
			}
		}
	}
}

SWC_BOOL CAgentUpdateIO::CheckHeadValidity(IN CSSLIONode* pNode)
{
	if (NULL == pNode) return FALSE;
	pS_PMX_AGENT_CMD_HD pHd = (pS_PMX_AGENT_CMD_HD)pNode->GetTempBuf();

	if (NULL == pHd) return FALSE;
	if (FALSE == pHd->CheckMark()) 
	{
		_LOG_ERR("Mark mismatched. [%02X][%02X][%02X][%02X]",
			pHd->Mark[0], pHd->Mark[1], pHd->Mark[2], pHd->Mark[3]);
		return FALSE;
	}
	return TRUE;
}

UINT CAgentUpdateIO::GetBodyLen(IN CSSLIONode* pNode)
{
	if (NULL == pNode) return 0;
	pS_PMX_AGENT_CMD_HD pHd = (pS_PMX_AGENT_CMD_HD)pNode->GetTempBuf();
	if (NULL == pHd) return 0;
	return (UINT)pHd->GetBodyLen();
}

SWC_BOOL CAgentUpdateIO::ExecTransactionQueries(IN WCArray<WCString>& arrQueries, OUT WCString& sError)
{
	SWC_BOOL bRet = TRUE;

	if (NULL == m_pDBObj) return FALSE;
	if (0 == arrQueries.GetCount()) return FALSE;

	for(UINT nID = 0; nID < arrQueries.GetCount(); nID++)
	{
		if (FALSE == CDBIO::RunTransaction(m_pDBObj, arrQueries[nID], sError))
		{
			m_pDBObj->Rollback();
			bRet = FALSE;
			break;
		}
	}
	if(TRUE == bRet) m_pDBObj->Commit();
	return bRet;
}

SWC_BOOL CAgentUpdateIO::SendReply(IN CSSLIONode* pNode, IN S_PMX_AGENT_CMD_HD& hd, IN WCString sReplyXml)
{
	SWC_BOOL bRet = TRUE;
	INT nDCompLen = 0, nCompLen = 0;
	WCString sSend = "";
	S_PMX_AGENT_CMD_HD sHd;

	// TODO: for debugging
	_LOG_DBG("\n%s", LPCSTR(sReplyXml));

	sHd.Init();
	sHd.ContentsType = hd.ContentsType;

	if (PMX_CONTENTS_TYPE_COMPRESSED == hd.GetContentsType())
	{
		sSend = BodyCompressor::CompressedBody(LPCSTR(sReplyXml), nDCompLen, nCompLen);
		sHd.SetDecompressSize(nDCompLen);
		sHd.SetBodyLen(sSend.GetLength());

		/* TODO: for debugging
		{
			_LOG_DBG("org xml len: %d / compressed xml len: %d / nDCompLen: %d / nCompLen: %d", 
				sReplyXml.GetLength(), sSend.GetLength(), nDCompLen, nCompLen);
			_LOG_DBG("[ORIGINAL]\n%s", LPCSTR(sReplyXml));
			_LOG_DBG("[COMPRESSED]\n%s", LPCSTR(sSend));
		}
		//*/
	}
	else
	{
		sSend = sReplyXml;
		sHd.SetBodyLen(sSend.GetLength());
	}

	if (S_PMX_AGENT_CMD_HD_LEN == pNode->Write(&sHd, S_PMX_AGENT_CMD_HD_LEN, PMX_DEFAULT_IO_TIMEOUT))
	{
		if (sHd.GetBodyLen() != (UINT)pNode->Write((PVOID)LPCSTR(sSend), sHd.GetBodyLen(), PMX_DEFAULT_IO_TIMEOUT))
		{
			_LOG_ERR("sending body failed (length:%u): (%d)%s", sHd.GetBodyLen(), errno, strerror(errno));
			bRet = FALSE;
		}
	}
	else
	{
		_LOG_ERR("sending head failed (length:%u, body length:%u, contents type:%s, decompressed length:%u): (%d)%s",
			(UINT)S_PMX_AGENT_CMD_HD_LEN, sHd.GetBodyLen(), 
			(PMX_CONTENTS_TYPE_COMPRESSED == sHd.GetContentsType() ? "compressed" : "plain text"),
			sHd.GetDecompressSize());
		bRet = FALSE;
	}

	return bRet;
}

SWC_BOOL CAgentUpdateIO::CmdOperate(IN CSSLIONode* pNode)
{
	SWC_BOOL bRet = TRUE;
  
	if (NULL == pNode) return FALSE;
	pS_PMX_AGENT_CMD_HD pHd = (pS_PMX_AGENT_CMD_HD)pNode->GetTempBuf();
	S_PMX_AGENT_CMD_HD hdCopy;

	if (NULL == pHd) return FALSE;
	memcpy((PVOID)&hdCopy, (PVOID)pHd, S_PMX_AGENT_CMD_HD_LEN);

	if (FALSE == hdCopy.CheckMark())
	{
		_LOG_ERR("Mark mismatched. [%02X][%02X][%02X][%02X]",
			hdCopy.Mark[0], hdCopy.Mark[1], hdCopy.Mark[2], hdCopy.Mark[3]);
		return FALSE;
	}

	// set time
	if (0 == hdCopy.GetCommandTime())
	{
		time_t tCur = 0;
		uint64_t tTemp = 0;

		time(&tCur);
		tTemp = (uint64_t)tCur;
		hdCopy.SetCommandTime((uint64_t)tTemp);
	}

	// extract body
	WCMemPool tmpBuf;
	tmpBuf.Attach(pNode->GetRecvPtr(), pNode->GetRecvLen());
	CHAR* pBody = (CHAR*)tmpBuf.GetPtr();
	WCString sXml = (CHAR*)"";

	if (PMX_CONTENTS_TYPE_COMPRESSED == hdCopy.GetContentsType())
	{
		sXml = BodyCompressor::Uncompressedbody(pBody, hdCopy.GetDecompressSize());
	}
	else // PMX_CONTENTS_TYPE_PLAIN_TEXT
	{
		sXml = pBody;
		sXml = sXml.Left(hdCopy.GetBodyLen());
	}
	tmpBuf.Free();

	// TODO: for debugging ...
	_LOG_DBG("\n%s", LPCSTR(sXml));

	// extract request command from xml
	CPmxXmlIO objXml(sXml);
	if (PMX_ERR_XML_RW_SUCCESS != objXml.GetLastErrNo())
	{
		_LOG_ERR("command info. extracting failed");		
		return FALSE;
	}
	
	// operate each command
	_LOG_DBG("Code:%s SubCode:%s", LPCSTR(objXml.GetCode()), LPCSTR(objXml.GetSubCode()));
	//_LOG_DBG("%s", LPCSTR(sXml)); // for debugging

	if (objXml.GetCode() == PMX_XML_VAL_UPDATEFILE)
	{
		bRet = CmdRequestUpdateFile(pNode, hdCopy, objXml);
	}
	else
	{
		_LOG_ERR("invalid or unknown command code: %s", LPCSTR(objXml.GetCode()));
		return FALSE;
	}
	return bRet;
}

SWC_BOOL CAgentUpdateIO::CmdRequestUpdateFile(IN CSSLIONode*         pNode, 
											  IN S_PMX_AGENT_CMD_HD& hd, 
											  IN CPmxXmlIO&          objXml)
{
	SWC_BOOL bRet = TRUE;
	WCString sFailCode = "", sFailMsg = "", sReplyXml = "";
	WCString sInstalledVersion = "", sNeedPatchYN = "", sCurrentProductVersion = "", sFilePath = "", sFileHash = "";
	INT nProductType = 0;
	UINT nFileSize = 0;
	WCArray<WCString> arrExServerURL;
        WCString sTypeFull ="";	
	// extract request info.
	if (FALSE == objXml.ExtracrtUpdateFileRequest(nProductType, sInstalledVersion, sTypeFull))
	{
		sFailCode = PMX_ERROR_E20004;
		sFailMsg  = PMX_ERROR_MSG_E20004;
		_LOG_ERR("Code:%s SubCode:%s FailCode:%s, FailMsg:%s", 
			LPCSTR(objXml.GetCode()), LPCSTR(objXml.GetSubCode()), LPCSTR(sFailCode), LPCSTR(sFailMsg));
		bRet = FALSE;
	}

	 // POLICY 파일 처리 (파일명 파라미터 포함)
	if (sTypeFull.Left(6) == "POLICY") {
        	bRet = TRUE;
		WCString sPolicyFileName = (sTypeFull.Find('|') != -1) ? sTypeFull.Mid(sTypeFull.Find('|') + 1) : "";
        // 파일 경로 설정
        WCString sFilePath = PMX_PATH_PRIMXSERVER_PRODUCT_POLICIES_ZONECENTRAL_POLICY;
        sFilePath += "/";
        sFilePath += sPolicyFileName;
        _LOG_ERR("path = %s/%s", LPCSTR(sFilePath), LPCSTR(sPolicyFileName));
        if (false == std::filesystem::exists(LPCSTR(sFilePath))) {
                sFailCode = PMX_ERROR_E30002;
                sFailMsg.Format("정책 파일 없음: %s", LPCSTR(sFilePath));
                bRet = FALSE;
        } else {
                nFileSize = SWC::swc_get_file_size(sFilePath);
                if (nFileSize == 0) {
                    sFailCode = PMX_ERROR_E30003;
                    sFailMsg.Format("정책 파일 크기 0: %s", LPCSTR(sFilePath));
                    bRet = FALSE;
                        } else {
                        sCurrentProductVersion = "1.0.0.0";  // 임의 버전
                        sFileHash = "";
                        pNode->SetFileSendMode(LPCSTR(sFilePath), SOCK_MAX_FILE_SEND_BLOCK_SIZE);
                        }
                }
        // 응답 전송
            sReplyXml = objXml.MakeUpdateFileReplyXml(
                bRet,
                999 ,
                sCurrentProductVersion,
                sFileHash,
                arrExServerURL,
                nFileSize,
                sFailCode,
                sFailMsg
            );
        SendReply(pNode, hd, sReplyXml);

        return bRet;
        }

// extract update info.
	if (TRUE == bRet)
	{
		if (FALSE == CDBIO::ExtractProductUpdateInfo(m_pDBObj, 
													 LPCSTR(objXml.GetAgentID()),
													 nProductType,
													 LPCSTR(sInstalledVersion),
													 sNeedPatchYN,
													 sCurrentProductVersion,
													 arrExServerURL,
													 sFilePath,
													 sFileHash,
													 sFailCode,
													 sFailMsg))
		{
			if (0 == sFailCode.GetLength()) sFailCode = PMX_ERROR_E10003;
			if (0 == sFailMsg.GetLength())  sFailMsg  = PMX_ERROR_MSG_E10003;
			_LOG_ERR("Code:%s SubCode:%s FailCode:%s, FailMsg:%s", 
				LPCSTR(objXml.GetCode()), LPCSTR(objXml.GetSubCode()), LPCSTR(sFailCode), LPCSTR(sFailMsg));
			bRet = FALSE;
		}
	}

	if (TRUE == bRet)
	{
		// check if need patch product
		if (sNeedPatchYN == PMX_XML_VAL_NO)
		{
			if (0 == sFailCode.GetLength()) sFailCode = PMX_ERROR_E40001;
			if (0 == sFailMsg.GetLength())  sFailMsg  = PMX_ERROR_MSG_E40001;
			_LOG_ERR("Code:%s SubCode:%s FailCode:%s, FailMsg:%s", 
				LPCSTR(objXml.GetCode()), LPCSTR(objXml.GetSubCode()), LPCSTR(sFailCode), LPCSTR(sFailMsg));
			bRet = FALSE;
		}
		else
		{
			// extract file size
			if (true == std::filesystem::exists(LPCSTR(sFilePath)))
			{
				nFileSize = SWC::swc_get_file_size(sFilePath);

				if (0 == nFileSize)
				{
					sFailCode = PMX_ERROR_E30003;
					sFailMsg.Format("%s. %s", PMX_ERROR_MSG_E30003, LPCSTR(sFilePath));
					_LOG_ERR("Code:%s SubCode:%s FailCode:%s, FailMsg:%s", 
							LPCSTR(objXml.GetCode()), LPCSTR(objXml.GetSubCode()), LPCSTR(sFailCode), LPCSTR(sFailMsg));
					bRet = FALSE;
				}
			}
			else
			{
				sFailCode = PMX_ERROR_E30002;
				sFailMsg.Format("%s. %s", PMX_ERROR_MSG_E30002, LPCSTR(sFilePath));
				_LOG_ERR("Code:%s SubCode:%s FailCode:%s, FailMsg:%s", 
						LPCSTR(objXml.GetCode()), LPCSTR(objXml.GetSubCode()), LPCSTR(sFailCode), LPCSTR(sFailMsg));
				bRet = FALSE;
			}
		}
	}

	sReplyXml = objXml.MakeUpdateFileReplyXml(bRet, 
											  nProductType, 
											  sCurrentProductVersion, 
											  sFileHash, 
											  arrExServerURL, 
											  nFileSize, 
											  sFailCode, 
											  sFailMsg);

	// send reply
	if (0 < sReplyXml.GetLength())
	{
		if (FALSE == SendReply(pNode, hd, sReplyXml)) 
		{
			return FALSE;
		}
		else
		{
			if ((TRUE == bRet) && (0 < nFileSize))
			{
				// set as send file 
				pNode->SetFileSendMode(LPCSTR(sFilePath), SOCK_MAX_FILE_SEND_BLOCK_SIZE);
			}
		}
	}
	return bRet;
}


//-----------------------------------------------------------------------------
#endif // __PMX_AGENT_UPDATE_IO_CPP__
