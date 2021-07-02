/*
	err.h
	错误报告头文件
*/
#define APP_ERROR_CODE
#define ERR_REGISTER_FAIL										0x00000001
#define ERR_MEM_CREATE_FAIL										0x00000002
#define ERR_FPOPEN_FAIL											0x00000003

#define APP_ERROR_CODE_POS
#define ERR_IN_DataFlightLinkedListFile							0x00010000
#define ERR_IN_DataAccount_LinkedListFile						0x00020000
#define ERR_IN_DataUAFile										0x00030000
#define ERR_IN_DataOrderLinkedListFile							0x00040000
#define ERR_IN_DataOrder_GetUserOrderToMem						0x00050000
#define ERR_IN_DataOrder_CreateSingleOrder						0x00060000
#define	ERR_IN_DataFlight_CreateNULLFlight						0x00070000
#define ERR_IN_AdminUserManage_AddAdmin_AddButtonProc			0x00080000
//错误报告函数
VOID ErrorReport(UINT code);