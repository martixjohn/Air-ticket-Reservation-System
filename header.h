/*
	header.h
	程序主头文件
*/
#include <Windows.h>
#include <windowsx.h>
#include <strsafe.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"
#include "err.h"
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

/*
	程序基本
*/
//窗口大小
//需要在前声明定义cxScreen,cyScreen
//cxScreen,cyScreem 在第一次创建窗口时已定义大小
#define SCREENMINSIZE		min(cxScreen,cyScreen)
#define SCREENMAXSIZE		max(cxScreen,cyScreen)
#define WNDSIZE_LOGIN_CX	min(max(800,(cxScreen>cyScreen)?(cyScreen * 2 / 3 * 4 / 3):(cxScreen * 2 / 3)),cxScreen)
#define WNDSIZE_LOGIN_CY	min(max(600,(cxScreen>cyScreen)?(cyScreen * 2 / 3):(cxScreen * 2 /3 * 3 / 4)),cyScreen)
#define WNDSIZE_MAINWND_CX	min(max(600,(cxScreen>cyScreen)?(cyScreen * 4 / 5 * 16 / 9):(cxScreen * 4 / 5)),cxScreen)
#define WNDSIZE_MAINWND_CY	min(max(800,(cxScreen>cyScreen)?(cyScreen * 4 / 5):(cxScreen * 4 / 5 * 9 / 16)),cyScreen)

//编码
#ifdef UNICODE
#define fopen_s _wfopen_s
#define FCOUNT 2
#define atoi _wtoi
#define atof _wtof
#else
#define FCOUNT 1
#endif // UNICODE

//退出程序方式
typedef enum {
	EXIT_IN_LOGIN,EXIT_IN_MAINWND,EXIT_IN_REGISTER
}EXIT_WAY;

//全局变量
extern HINSTANCE hInst;
extern UINT cxScreen, cyScreen;
extern EXIT_WAY e_w;
extern TCHAR szAppName[7];

//初始化应用程序
BOOL InitInstance(HINSTANCE, int);

//退出程序清处堆内存
BOOL DataFreeMemory();

//LogIn
LRESULT CALLBACK LogInProc(HWND, UINT, WPARAM, LPARAM);
extern HWND hwndLogIn;
extern UINT cxLogIn, cyLogIn;

//RegisterInfo
LRESULT CALLBACK RegisterInfoProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern UINT cxRegister, cyRegister;

//MainWindow
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
extern HWND hwndMain;
extern int wndtype;
extern UINT cxMain, cyMain;

/*
	自写子窗口控件,默认message返回0
*/
#define DIY_MAX_WND             128
/*CHECKBOX*/
#define DIY_CHECKBOX				TEXT("DIY_CHECKBOX")
LRESULT CALLBACK DIY_CheckBoxProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//通知父窗口   wParam		按下的控件ID				lParam状态
#define DIY_CHECKBOX_COMMAND						(WM_USER+100)			
//状态
#define CHECKBOX_CHECKED							0
#define CHECKBOX_UNCHECKED							1
//通知控件，自动改变当前状态为相反状态	
#define DIY_CHECKBOX_SET_AUTO						(WM_USER+101)
//通知控件，改变当前状态	   		lParam状态
#define DIY_CHECKBOX_SET							(WM_USER+102)
//依靠ID来识别不同按钮	
#define DIY_HMENU_BEGIN								0

//ID
#define CHECKBOXID_ADMINFLIGHT_SEARCH_TYPE1								(DIY_HMENU_BEGIN + 0)
#define CHECKBOXID_ADMINFLIGHT_SEARCH_TYPE2								(DIY_HMENU_BEGIN + 1)
#define CHECKBOXID_ADMINFLIGHT_DETAIL_TYPE1								(DIY_HMENU_BEGIN + 2)
#define CHECKBOXID_ADMINFLIGHT_DETAIL_TYPE2								(DIY_HMENU_BEGIN + 3)
#define CHECKBOXID_ADMINUSERMANNAGE_USER								(DIY_HMENU_BEGIN + 4)
#define CHECKBOXID_ADMINUSERMANNAGE_ADMIN								(DIY_HMENU_BEGIN + 5)
#define CHECKBOXID_ADMINUSERMANAGE_ADDADMIN_ADMIN						(DIY_HMENU_BEGIN + 6)
#define CHECKBOXID_ADMINUSERMANAGE_ADDADMIN_SUPERADMIN					(DIY_HMENU_BEGIN + 7)


/*
	data
*/
//URL
//目录
#define URL_PATH	TEXT("data")
//用户数据
#define URL_UA		TEXT("data\\userdb")
//管理员数据
#define URL_ADMIN	TEXT("data\\admindb")
//航班数据
#define URL_DATA	TEXT("data\\datadb")
//程序配置文件
#define URL_CONF	TEXT("data\\.conf")
//订单数据
#define URL_ORDER	TEXT("data\\orderdb")


/*
	USERACCOUNT
*/

//---单个用户数据
//用户类型
typedef enum
{
	UT_USER, UT_ADMIN, UT_SUPERADMIN
}USERTYPE;
//限制：昵称12内，名字12内，身份证必须18
//---账号密码的限制
#define LIMIT_USERNAME				12
#define LIMIT_PASSWORD				12
#define LIMIT_NICKNAME				12
#define LIMIT_NAME					12
#define LIMIT_SELFINFO				128
#define LIMIT_ACCOUNT				256
typedef struct UserAccount{
	TCHAR szUsername[20];
	TCHAR szPassword[20];
	TCHAR szSelfInfo[128];//个人信息
	USERTYPE ut;
}USERACCOUNT, *LPUSERACCOUNT;


typedef enum {
	UA_GET, UA_CHANGE
}UA_OPERATE;

typedef enum {
	UALIST_GET, UALIST_CHANGE,
	UALIST_GET_ADMIN, UALIST_CHANGE_ADMIN
}UAL_OPERATE;

typedef enum {
	UA_ADDTO_FILE,
	UA_UPDATE_FILE//涉及链表
}UA_FILE;

typedef enum {
	UA_TO_TEXT, TEXT_TO_UA,
	FL_TO_TEXT, TEXT_TO_FL,
	OI_TO_TEXT, TEXT_TO_OI
}DATA_FORMAT;
size_t DataAccount_GetOrderNum(LPUSERACCOUNT pUa);
//---用户链表
#define MAX_SEARCH_UA_ID	512
typedef struct UserAccountNode {
	USERACCOUNT ua;
	struct UserAccountNode* next;
}USERACCOUNTNODE, * LPUSERACCOUNTNODE;
typedef enum {
	UALIST_TO_FILE, UALIST_FROM_FILE,
	UALIST_TO_FILE_ADMIN, UALIST_FROM_FILE_ADMIN
}UALIST_FILE;
#define  FILE_TO_UALIST UALIST_FROM_FILE


//从文件中搜索结果
typedef enum {
	UA_NOTFOUND,		//找不到
	UA_FOUND_ERR_PW,	//找到但密码错误
	UA_FOUND,			//找到匹配的普通用户账户
	UA_FOUND_ADMIN,		//找到匹配的管理员账户
	UA_FOUNDFILEERR		//遇到错误
}UA_SEARCHRET;
//获得用户信息
typedef enum {
	UA_GETINFO_NICKNAME,
	UA_GETINFO_NAME,
	UA_GETINFO_IDCARD,
	//IDCARD内含
	UA_GETINFO_SEX, UA_GETINFO_BIRTH
}UA_GETINFO;
//值为0 ADMIN	值为1 SUPERADMIN
#define UA_GETINFO_ADMINPER		UA_GETINFO_IDCARD

//设置用户信息
typedef enum {
	UA_SETINFO_NICKNAME,
	UA_SETINFO_NAME,
	UA_SETINFO_IDCARD
}UA_SETINFO;
//存取内存中的用户搜索结果信息
typedef enum {
	UAL_SR_SET, UAL_SR_GET
}UAL_SEARCHRET_MEM;
//---函数声明
BOOL DataAccount_GetSelfInfoFromUA(TCHAR* szBuf, UA_GETINFO ua_getinfo, LPUSERACCOUNT pUa);
BOOL DataAccount_SetSelfInfoToUA(TCHAR* szBuf, UA_SETINFO ua_setinfo, LPUSERACCOUNT pUa);
BOOL DataUserAccount(LPUSERACCOUNT pUa, UA_OPERATE ua_o);
BOOL DataUAFile(UA_FILE u_f, LPUSERACCOUNT pUa);
BOOL DataAccountFormat(LPUSERACCOUNT pUa, TCHAR* szBuf, DATA_FORMAT d_f, size_t* pLength);
UA_SEARCHRET DataAccountSearch_File(USERACCOUNT uaTrgt);
UA_SEARCHRET DataAccountSearchFileToMem(USERACCOUNT uaTrgt);
BOOL DataValidCharcterUNorPW(const TCHAR* szInfo);
BOOL DataUAValid(USERACCOUNT ua);
BOOL DataGetValidUA_UOPStd(TCHAR szInfo[128]);
BOOL DataGetValidUA_SelfInfoStd(TCHAR szInfo[128]);
BOOL DataValidCharcterSelfInfo(const TCHAR* szInfo, UA_SETINFO ua_setinfo);
USERTYPE DataGetUserType();
int DataAccountSearchRet(LPUSERACCOUNTNODE pUaNodeArr[MAX_SEARCH_UA_ID], UAL_SEARCHRET_MEM ual_sr_m); 

//UALIST
BOOL DataAccount_LinkedListMem(LPUSERACCOUNTNODE* ppUaN, UAL_OPERATE ual_o);
BOOL DataAccount_LinkedListFile(UALIST_FILE ual_f);


/*
	航班数据
*/
//ID值，用于标识航班
typedef WORD IDDATA, * LPIDDATA, IDDATA_NUM;
//---单个航班数据
#define LIMIT_SZDEPARTDEST			32
#define LIMIT_SZFLIGHTID			32
#define LIMIT_FLIGHTDATA			256
#define LIMIT_FLIGHTOTHER			32
#define LIMIT_ORDERDATA				384
typedef struct {
	int month;					//5		12	\0
	int day;					//5
	int hr;						//5
	int min;					//5
}DATETIME;

typedef struct {							//内存
	TCHAR szDepart[LIMIT_SZDEPARTDEST];			//32	城市+飞机场
	TCHAR szDest[LIMIT_SZDEPARTDEST];			//32
	DATETIME dtDepart;							//12
	DATETIME dtDest;							//12
	TCHAR szFlightID[LIMIT_SZFLIGHTID];			//32	航空公司+航班号	
	TCHAR szPlaneID[LIMIT_SZFLIGHTID];			//32	飞机编号
	double dPrice;							//8     12345.7	\0
	UINT iAmount;							//8		1234567	\0
	TCHAR szType[LIMIT_FLIGHTOTHER];		//32
	IDDATA id;								//此ID唯一
}FLIGHTDATA, * LPFLIGHTDATA;				//256TCHAR
#define FLIGHT_TYPE1		TEXT("经济舱")
#define FLIGHT_TYPE2		TEXT("公务舱/商务舱")
BOOL DataValidCharacterFlightIDOrDepartOrDest(const TCHAR* szBuf);
BOOL DataGetValidFlightInfoStd(TCHAR szInfo[128]);
//---航班链表

typedef struct FlightDataNode {
	FLIGHTDATA fl;
	struct FlightDataNode* next;
}FLIGHTDATANODE, * LPFLIGHTDATANODE;
typedef	enum {
	FLLIST_GET_HEADNODE, FLLIST_CHANGE_HEADNODE
}FLLIST_OPERATE;

typedef enum {
	FLLIST_TO_FILE, FLLSIT_FROM_FILE
}FL_FILE;
IDDATA_NUM DataFlightLinkedListMem(LPFLIGHTDATANODE* ppFlNode_Head, FLLIST_OPERATE fll_o);
BOOL DataFlightLinkedListFile(FL_FILE fl_file);
BOOL DataFlightFormat(LPFLIGHTDATA pFl, TCHAR* szBuf, DATA_FORMAT d_f, size_t* pLength);
LPFLIGHTDATANODE DataFlight_CreateNULLFlight();
BOOL DataFlight_GetCityName(TCHAR* szDst, const TCHAR* szSrc);
#define DataFlight_GetCompanyName DataFlight_GetCityName
BOOL DataFlight_GetAirportName(TCHAR* szDst, const TCHAR* szSrc);
#define DataFlight_GetFlightIDName DataFlight_GetAirportName

//航班ID范围
#define IDDATA_BEGIN			1
#define IDDATA_END				512
//---查询数据
#define MAX_SEARCH_FLIGHT_ID	IDDATA_END	//搜索结果数目上线
IDDATA DataFlight_GetAvailableID();

//为0表示不存在
typedef enum
{
	FL_SEARCHOPTION_DEPARTANDDEST      = 0x00000000,     //必选
	FL_SEARCHOPTION_DEPARTTIME         = 0x00000001,
	FL_SEARCHOPTION_DESTTIME           = 0x00000010,
	FL_SEARCHOPTION_TYPE			   = 0x00000100,
	FL_SEARCHOPTION_ALL                = 0x11111111
}FL_SEARCHOPTION;
typedef struct {
	TCHAR szDepart[LIMIT_SZDEPARTDEST];
	TCHAR szDest[LIMIT_SZDEPARTDEST];
	TCHAR szDepartMonth[3];
	TCHAR szDepartDay[3];
	TCHAR szDestMonth[3];
	TCHAR szDestDay[3];
	TCHAR szType[LIMIT_FLIGHTOTHER];
	FL_SEARCHOPTION fl_searchoption; //需要查询的数据，查询选项
}FL_SEARCHDATA, * LPFL_SEARCHDATA;//仅搜索非0的数据
//查询数据结果码
typedef enum {
	FL_NOTFOUND, FL_FOUND, 
	FL_ERRFOUND
}FL_SEARCHRET;
typedef enum {
	FL_SR_SET, FL_SR_GET
}FL_SEARCHRET_MEM;
LPFLIGHTDATANODE DataFlight_GetPNode(IDDATA id);
FL_SEARCHRET DataFlightSearch_ReturnIDDATA(FL_SEARCHDATA fl_sd, IDDATA IdData[MAX_SEARCH_FLIGHT_ID]);
IDDATA_NUM DataFlightSearchRet(IDDATA iddata[MAX_SEARCH_FLIGHT_ID], FL_SEARCHRET_MEM fl_sr_m);
BOOL DataValidDateText(const TCHAR* szMonth, const TCHAR* szDay);
void IDDATAFoundCommon(IDDATA dest[], const IDDATA src1[], const IDDATA src2[]);

/*
	订单数据
*/
//---单个订单数据
typedef struct {
	TCHAR szUsername[20];
	FLIGHTDATA fldata;			//注意管理员删除航班数据不能影响订单！
	TCHAR szOrderNumber[20];//2021 05 30 14 03 00001
}ORDERINFO, * LPORDERINFO;
typedef enum {
	OI_GET, OI_CHANGE
}OI_OPERATE;
BOOL DataOrderFormat(LPORDERINFO pOI, TCHAR* szBuf, DATA_FORMAT d_f, size_t* pLength);

BOOL DataOrder_GetUserOrderToMem();
BOOL DataOrder_CreateSingleOrder(IDDATA idData);


//---订单链表
typedef struct OrderInfoNode {
	ORDERINFO oi;
	struct OrderInfoNode* next;
}ORDERINFONODE, * LPORDERINFONODE;
typedef	enum {
	OILIST_GET_HEADNODE, OILIST_CHANGE_HEADNODE
}OILIST_OPERATE;
typedef enum {
	OILIST_TO_FILE, OILSIT_FROM_FILE
}OI_FILE;
IDDATA_NUM DataUserOrder(LPORDERINFONODE* ppOINode_Head, OILIST_OPERATE oil_o);
IDDATA_NUM DataOrderLinkedListMem(LPORDERINFONODE* ppOINode_Head, OILIST_OPERATE oil_o);
BOOL DataOrderLinkedListFile(OI_FILE oi_file);



/*
	基础算法
*/

typedef UINT USER_FONT_STYLE;
#define APP_DEFAULT_FONT TEXT("微软雅黑")
#define YAHEI_FONT						  10000
#define F_I_TRUE						 100000
#define F_I_FALSE							  0
#define F_U_TRUE						1000000
#define F_U_FALSE						      0
#define F_TRANSPARENT_BK_MODE_FALSE	   10000000
#define FONT_STYLE_DEFAULT	YAHEI_FONT + FW_NORMAL
HFONT SetFont(HDC hdc, int cHeight, int cWidth, USER_FONT_STYLE fs);

void ClearStrToZero(TCHAR* szText, size_t iLen);
void ClearUAStructToZero(LPUSERACCOUNT pUa);
void ClearIDDATAARRToZero(IDDATA iddata[MAX_SEARCH_FLIGHT_ID]);
void ClearPointerArrToZero(void* pArr[], size_t num);
#define PT_ON_RECT(pt,rc) (pt.x>rc.left&&pt.x<rc.right&&pt.y>rc.top&&pt.y<rc.bottom)


/*
	graphics
*/
BOOL OnPaint(HWND hwnd, HDC hdcMem);
VOID MoveRect(RECT* rc, int x, int y);
BOOL DefInitWindow(HWND hwnd, int cxClient, int cyClient, HBRUSH hbrBk, RECT* prcClient, HDC* phdcMemBk, HBITMAP* phbmpBk);
BOOL DefDelWindow(HDC hdcMemBk, HBITMAP hbmpBk);