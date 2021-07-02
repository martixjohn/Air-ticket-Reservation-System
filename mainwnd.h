/*
*   mainwnd.h
*	������MainWindow���ͷ�ļ�
*/
#include "header.h"

/*��Ϣ��Ĭ�Ϸ���0��ʾ�޴���*/
#define WM_U_GET_PER			WM_USER+10			//wParam USERTYPE
//��ȡ�˵���ID����hwndList
#define WM_U_GET_OPTION         WM_USER+200         //return ID
//��ȡ�˵���ѡ�еĲ˵���ID��������MainMenu
#define WM_U_GET_CLICKED        WM_USER+201         //wParam  GET_CLICKEDID/HWND    return ��Ӧ
#define    GET_CLICKEDID        100
#define    GET_CLICKEDHWND      200
//���ص�ǰѡ�е�ID����MenuOption�����͵�MainMenu
#define WM_U_SET_CLICKED        WM_USER+202         //wParam ID lParam hwnd  
//֪ͨѡ������˸ı䣬��MainMenu������MainBody
#define WM_U_CLICKCHANGED       WM_USER+203         //wParam ID lParam hwnd 

#define WM_U_GETMEMDC           WM_USER+204         //return hdcMem
/*TimerId*/
#define TID_BODYWINDOW_SHOW     10
/*�Ӵ���ID*/
//MainWindow

#define MAIN_MENU			0x00002100
#define MAIN_BODY			0x00002200
#define MAIN_MASK           0x00002300

/*
    ����Ա����ҳ ��Ʊ���� �û����� ����
    �û�  ����ҳ ����Ʊ �ҵĶ��� ����
    idֵ��0-4
*/
//Menuѡ��
#define MENUID_OPTION		            0
#define MENUID_OPTION1		            MENUID_OPTION+0		
#define MENUID_OPTION2		            MENUID_OPTION+1
#define MENUID_OPTION3		            MENUID_OPTION+2
#define MENUID_OPTION4		            MENUID_OPTION+3
#define MENUID_LOGO			            MENUID_OPTION+4
#define MENUID_NONE                     MENUID_OPTION+100
//Body�Ӵ���
#define BODYID_OPTION1                  MENUID_OPTION1
#define BODYID_OPTION2                  MENUID_OPTION2
#define BODYID_OPTION3                  MENUID_OPTION3
#define BODYID_OPTION4                  MENUID_OPTION4
#define BODYID_NONE                     MENUID_NONE
#define BODYID_USERHOME		            BODYID_OPTION1
#define BODYID_USERBOOKING		        BODYID_OPTION2
#define BODYID_USERMINE		            BODYID_OPTION3
#define BODYID_USERSETTING	            BODYID_OPTION4
#define BODYID_ADMINHOME	            BODYID_OPTION1
#define BODYID_ADMINFLIGHT		        BODYID_OPTION2
#define BODYID_ADMINUSERMANAGE		    BODYID_OPTION3
#define BODYID_ADMINSETTING	            BODYID_OPTION4
/*���ڹ���*/
//Menu
extern HWND hwndMenu;
extern HBRUSH hbrMenuBk;
extern COLORREF crMenuBk;
extern HWND hwndMenuOpt[4], hwndLogo;
LRESULT CALLBACK MainMenuProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MenuOptionProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MenuLogoProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//Body
extern HWND hwndBodyOpt[2][4];
extern HBRUSH hbrBodyBk;
extern COLORREF crBodyBk;
extern HWND hwndBody;
LRESULT CALLBACK MainBodyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//    �Ӵ��ڹ���
LRESULT CALLBACK UserHomeProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK UserBookingProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK UserMineProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK UserSettingProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AdminHomeProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AdminFlightProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AdminUserManageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AdminSettingProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);