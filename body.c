/*
*       body.c
*       包含所有主要窗口主体内容相关函数
*/
#include "mainwnd.h"
int wndtype = 0;
//HWND to Menu
extern HWND hwndList[4], hwndLogo;

//子窗口句柄
HWND hwndBodyOpt[2][4];

//注册窗口类
static BOOL BodyRegister(HINSTANCE hInstance);          

//左侧菜单栏窗口类名定义
static TCHAR* szOptionName[2][4] = {
    {TEXT("用户首页"),TEXT("订机票"),TEXT("我的订单"),TEXT("用户设置")},
    {TEXT("管理员首页"),TEXT("机票管理"),TEXT("用户管理"),TEXT("管理员设置")}
};
//窗口过程指针,C99允许变量初始化
WNDPROC BodyOptWndProc[2][4] = {
    {UserHomeProc,UserBookingProc,UserMineProc,UserSettingProc},
    {AdminHomeProc,AdminFlightProc,AdminUserManageProc,UserSettingProc}
};
/*
*   函数: MenuRegister(HINSTANCE hInstance)
*
*   目标: 注册Body窗口下的窗口类
*
*   输入：hInstance   实例句柄
*
*   返回：BOOL        判断是否注册成功
*/
static BOOL BodyRegister(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    HBRUSH hBrush;
    int i,j;
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.style = 0;
    
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 4; j++)
        {
            wndclass.lpfnWndProc = BodyOptWndProc[i][j];
            wndclass.lpszClassName = szOptionName[i][j];
            if (!RegisterClass(&wndclass)) return FALSE;
        }
    }
    

    return TRUE;
}

/*
*   函数: MainBodyProc(HWND, UINT, WPARAM, LPARAM)
*
*   目标: 处理主要窗口中的Body窗口（右侧）的消息。
*
*   输入：
*   hWnd       窗口句柄
*   message    消息通知码
*   wParam     额外信息
*   lParam     额外信息
*
*   返回：LRESULT 记录结果返回Windows
*/
LRESULT CALLBACK MainBodyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //基础
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //基础结束
    static USERTYPE ut;                 //用户权限保存
    int i;
    static HWND hwndMenuNow = NULL;
    static HWND hwndBodyOptNow = NULL, hwndBodyOptPre = NULL;
    static int idMenuClicked,idMenuClickedPre;
    static RECT rcMask;
    HWND hwndMenuClicked;

    switch (uMsg)
    {
    case WM_CREATE:
        // 获取当前用户类型
        ut = DataGetUserType();

        //从文件读取所有订单数据
        DataOrderLinkedListFile(OILSIT_FROM_FILE);
        //读取航班文件数据到内存
        if (ut == UT_USER)
        {
            //航班数据获取
            if (FALSE == DataFlightLinkedListFile(FLLSIT_FROM_FILE))
            {
                if (IDOK == MessageBox(hwnd, TEXT("航班数据不存在，或出错，请联系管理员！"), TEXT("提示信息"), MB_OK))
                {
                    DestroyWindow(GetParent(hwnd));
                }
            }
            
            //初始化，获得所有该用户订单数据
            DataOrder_GetUserOrderToMem();
            wndtype = 0;
        }
        if (ut == UT_ADMIN || ut == UT_SUPERADMIN)
        {
            //航班数据获取
            DataFlightLinkedListFile(FLLSIT_FROM_FILE);
            //从文件中获取普通用户账户数据
            DataAccount_LinkedListFile(UALIST_FROM_FILE);
            //管理员账户数据
            DataAccount_LinkedListFile(UALIST_FROM_FILE_ADMIN);
            
            wndtype = 1;
        }

        //注册窗口类
        BodyRegister(hInst);
        for (i = 0; i < 4; i++)
        {
            hwndBodyOpt[wndtype][i] = CreateWindow(szOptionName[wndtype][i],
                NULL,
                WS_CHILD | WS_CLIPCHILDREN,
                0, 0, 0, 0,
                hwnd, (HMENU)i, hInst, NULL);
        }
        SetRect(&rcMask, 0, 0, cxClient, cyClient);
        return 0;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {   
            DefInitWindow(hwnd, cxClient, cyClient, hbrBodyBk, &rcClient, &hdcMemBk, &hbmpBk);
            for (i = 0; i < 4; i++)
            {
                MoveWindow(hwndBodyOpt[wndtype][i], 0, 0, cxClient, cyClient, FALSE);
            }
            ShowWindow(hwndBodyOpt[wndtype][0], SW_SHOWNORMAL);
            hwndBodyOptPre = hwndBodyOpt[wndtype][0];
            fInited = TRUE;
        }

        return 0;
    case WM_U_CLICKCHANGED:
        idMenuClicked = (int)wParam;
        hwndMenuNow = hwndMenuClicked = (HWND)lParam;
        
        if (hwndBodyOptPre != NULL) ShowWindow(hwndBodyOptPre, SW_HIDE);
        
        //记录即将显示的子窗口句柄并显示
        hwndBodyOptNow = hwndBodyOpt[wndtype][idMenuClicked];
        //显示
        idMenuClickedPre = idMenuClicked;
        ShowWindow(hwndBodyOptNow, SW_SHOW);
        //保存当前的子窗口句柄以便隐藏
        hwndBodyOptPre = hwndBodyOpt[wndtype][idMenuClicked];

        return 0;

    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;

    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hbrBodyBk);
        //保存所有订单数据
        DataOrderLinkedListFile(OILIST_TO_FILE);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

