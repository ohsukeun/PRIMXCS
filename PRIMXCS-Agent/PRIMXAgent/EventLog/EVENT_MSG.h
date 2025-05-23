//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: EVMSG_INSTALLED
//
// MessageText:
//
// %1 ���� ���α׷��� ��ġ�߽��ϴ�.
//
#define EVMSG_INSTALLED                  0x00000064L

//
// MessageId: EVMSG_REMOVED
//
// MessageText:
//
// %1 ���񽺸� ���� �Ͽ����ϴ�.
//
#define EVMSG_REMOVED                    0x00000065L

//
// MessageId: EVMSG_NOTREMOVED
//
// MessageText:
//
// %1 ���񽺸� �������� ���߽��ϴ�.
//
#define EVMSG_NOTREMOVED                 0x00000066L

//
// MessageId: EVMSG_CTRLHANDLERNOTINSTALLED
//
// MessageText:
//
// �� ��Ʈ�� �ڵ鷯�� ��ġ�� �������ϴ�.
//
#define EVMSG_CTRLHANDLERNOTINSTALLED    0x00000067L

//
// MessageId: EVMSG_FAILEDINIT
//
// MessageText:
//
// ���μ��� �ʱ�ȭ ����
//
#define EVMSG_FAILEDINIT                 0x00000068L

//
// MessageId: EVMSG_STARTED
//
// MessageText:
//
// ���񽺸� �����մϴ�.
//
#define EVMSG_STARTED                    0x00000069L

//
// MessageId: EVMSG_BADREQUEST
//
// MessageText:
//
// ���񽺰� �������� �ʴ� ��û�� �޾Ҵ�.
//
#define EVMSG_BADREQUEST                 0x0000006AL

//
// MessageId: EVMSG_DEBUG
//
// MessageText:
//
// Debug: %1
//
#define EVMSG_DEBUG                      0x0000006BL

//
// MessageId: EVMSG_STOPPED
//
// MessageText:
//
// ���񽺸� �����մϴ�.
//
#define EVMSG_STOPPED                    0x0000006CL

//
// MessageId: EVMSG_ALERT
//
// MessageText:
//
// %1 
//
#define EVMSG_ALERT                      0x0000006DL

//
// MessageId: EVMSG_ERROR
//
// MessageText:
//
// %1 
//
#define EVMSG_ERROR                      0x0000006EL

//
// MessageId: EVMSG_INFOR
//
// MessageText:
//
// %1 
//
#define EVMSG_INFOR                      0x0000006FL

