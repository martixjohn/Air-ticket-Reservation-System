/*
	bodyadmin.h
	管理员界面头文件
	包含所有Body -> admin所需的头文件
*/
#include "mainwnd.h"
/*
	AdminHome
*/
//子窗口ID
#define ADMINHOMEID_USERINFO					0x00001100
#define ADMINHOMEID_FLIGHTINFO					0x00001200

/*
	AdminFlight
*/
//Message
//通知AdminFlight或AdminUserManage,展示某个对应航班ID的详细信息，wParam:IDDATA lparam:指定是否是新建的航班 0不是 1是
#define WM_U_SHOWFLIGHTDETAIL					WM_USER+301	
//通知AdminFlight,展示搜索结果Search界面 wParam: TRUE进行检索（从DataFlightIDDATA_Store） FALSE 不进行检索
#define WM_U_SHOWSEARCHUI						WM_USER+302
//通知AdminFlight或AdminUserManage从内存中获取搜索结果数据并展示
#define WM_U_SHOWSEARCHRET						WM_USER+303
//通知AdminFlight_Search或AdminUserManage展示“删除成功”
#define WM_U_SHOWDELSUCCESS						WM_USER+304
//ID
#define ADMINFLIGHTID_SEARCH									0x00002100
#define ADMINFLIGHTID_SEARCH_SEARCHBAR							0x00002110
#define ADMINFLIGHTID_SEARCH_SEARCHBAR_CHILD					0x00002110
#define ADMINFLIGHTID_SEARCH_SEARCHBAR_CHILD_DEPART				0
#define ADMINFLIGHTID_SEARCH_SEARCHBAR_CHILD_DEST				1
#define ADMINFLIGHTID_SEARCH_SEARCHBAR_CHILD_DEPARTTIME			2
#define ADMINFLIGHTID_SEARCH_SEARCHBAR_CHILD_DESTTIME			3

#define ADMINFLIGHTID_DETAIL									0x00002200
#define ADMINFLIGHTID_DETAIL_BACKBUTTON							0x00002210
#define ADMINFLIGHTID_DETAIL_BOOKBUTTON							0x00002220

/*Admin UserManage*/
#define ADMINUSERMANAGEID_SEARCHRET								0x00003100
#define ADMINUSERMANAGEID_ADDADMIN								0x00003200
#define ADMINUSERMANAGEID_ADDADMIN_BACKBUTTON					0x00003210
#define ADMINUSERMANAGEID_ADDADMIN_ADDBUTTON					0x00003220