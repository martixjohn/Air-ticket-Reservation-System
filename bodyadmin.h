/*
	bodyadmin.h
	����Ա����ͷ�ļ�
	��������Body -> admin�����ͷ�ļ�
*/
#include "mainwnd.h"
/*
	AdminHome
*/
//�Ӵ���ID
#define ADMINHOMEID_USERINFO					0x00001100
#define ADMINHOMEID_FLIGHTINFO					0x00001200

/*
	AdminFlight
*/
//Message
//֪ͨAdminFlight��AdminUserManage,չʾĳ����Ӧ����ID����ϸ��Ϣ��wParam:IDDATA lparam:ָ���Ƿ����½��ĺ��� 0���� 1��
#define WM_U_SHOWFLIGHTDETAIL					WM_USER+301	
//֪ͨAdminFlight,չʾ�������Search���� wParam: TRUE���м�������DataFlightIDDATA_Store�� FALSE �����м���
#define WM_U_SHOWSEARCHUI						WM_USER+302
//֪ͨAdminFlight��AdminUserManage���ڴ��л�ȡ����������ݲ�չʾ
#define WM_U_SHOWSEARCHRET						WM_USER+303
//֪ͨAdminFlight_Search��AdminUserManageչʾ��ɾ���ɹ���
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