

#pragma pack(1)


//////////////////////////////////////////////////////////////////////////////
// 문자열 길이 정의

//============================================================================
//	interface structures
//============================================================================

///////////////////////////////////////////////////////////////////////////////
// PRIMX_CMD_HD.Version :
#define PRIMX_PROTOCOL_VER_MAJOR				1
#define PRIMX_PROTOCOL_VER_MINOR1				0
#define PRIMX_PROTOCOL_VER_MINOR2				0
#define PRIMX_PROTOCOL_VER_MINOR3				0

// PRIMX_CMD_HD.Result :
#define PRIMX_CMD_REPLY_NONE					100
#define PRIMX_CMD_REPLY_OK						100
#define PRIMX_CMD_REPLY_FAIL					200
#define PRIMX_CMD_REPLY_BUSY					300

// PRIMX_CMD_HD.Result :
#define PRIMX_CMD_REPLY_NONE					100
#define PRIMX_CMD_REPLY_OK						100
#define PRIMX_CMD_REPLY_FAIL					200
#define PRIMX_CMD_REPLY_BUSY					300

// PRIMX_CMD_HD.ContentsType :
#define PRIMX_CONTENTS_TYPE_COMPRESSED			100
#define PRIMX_CONTENTS_TYPE_PLAIN_TEXT			200


/// command header structure
typedef struct _PRIMX_CMD_HD_
{
	UCHAR Version[4];                     // protocol version. '1.0.0.0'
	UCHAR Mark[4];                        // 'MXCS'
	UCHAR AgentID[40];
	unsigned __int64  CmdTime;            // command time
	UINT  ContentsType;                   // body (contents) type
	UCHAR RandomPad[10];				  // 무작위 난수
	UINT  TimeOut;
	UINT  DecompressSize;                 // decompressed body length
	UINT  BodyLen;                        // body length
	UCHAR Pad[68];                        // padding bytes (무작위 난수로 초기화)
	
	void Init()
	{
		Version[0] = PRIMX_PROTOCOL_VER_MAJOR;
		Version[1] = PRIMX_PROTOCOL_VER_MINOR1;
		Version[2] = PRIMX_PROTOCOL_VER_MINOR2;
		Version[3] = PRIMX_PROTOCOL_VER_MINOR3;
		Mark[0] = 'M';
		Mark[1] = 'X';
		Mark[2] = 'C';
		Mark[3] = 'S';
		memset(AgentID, 0x00, sizeof(AgentID));
		CmdTime = 0;
		ContentsType = PRIMX_CONTENTS_TYPE_PLAIN_TEXT;
		memset(RandomPad, 0x00, sizeof(RandomPad));
		TimeOut = 0;
		DecompressSize = 0;
		BodyLen = 0;
		memset(Pad, 0x00, sizeof(Pad));
	};

	void Clear()
	{
		memset(Version, 0x00, sizeof(Version));
		memset(Mark, 0x00, sizeof(Mark));
		memset(AgentID, 0x00, sizeof(AgentID));
		CmdTime = 0;
		ContentsType = 0;
		memset(RandomPad, 0x00, sizeof(RandomPad));
		TimeOut = 0;
		DecompressSize = 0;
		BodyLen = 0;
		memset(Pad, 0x00, sizeof(Pad));
	};

	_PRIMX_CMD_HD_()
	{
		Init();
	};

	void SetByteOder()
	{
		CmdTime = htonll(CmdTime);
		ContentsType = htonl(ContentsType);                   
		TimeOut = htonl(TimeOut);
		DecompressSize = htonl(DecompressSize);            
		BodyLen = htonl(BodyLen);                     

	};

} PRIMX_CMD_HD, *pPRIMX_CMD_HD;
#define PRIMX_CMD_HD_LEN    sizeof(PRIMX_CMD_HD)




#pragma pack()



