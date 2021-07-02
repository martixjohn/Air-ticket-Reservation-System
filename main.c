/*
******************************************************************
*
* 
* 
*                   飞机订票系统
*               语言：C(遵循C99标准)
*               编译环境：VS2020
*               运行平台：Windows 7以上
*               作者：颜震宇 2021-6
*               说明：采用纯C Windows SDK编写
*               版本：1.1
*               
*               
* 
******************************************************************
*/
/*
    main.c
    程序入口
*/
#include "header.h"
/*基本信息*/
HINSTANCE hInst;                                // 当前实例
UINT cxScreen, cyScreen;                        // 用户屏幕大小
EXIT_WAY e_w = EXIT_IN_LOGIN;                   // 默认退出程序方式
UINT cxLogIn, cyLogIn;
UINT cxRegister, cyRegister;
UINT cxMain, cyMain;

/*窗口句柄*/
HWND hwndLogIn;
/*名字定义*/
//主要窗口
TCHAR szAppName[7] = TEXT("飞机订票系统");       //程序主要窗口
//登录窗口
TCHAR szLogIn[] = TEXT("登录界面");             //登录窗口,初次进入程序时打开

/*函数前向声明*/
BOOL RegisterWndClass(HINSTANCE hInstance);

/*子窗口控件*/

/*
    程序入口
*/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    MSG msg;
    RegisterWndClass(hInstance);
    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    CreateDirectory(URL_PATH, NULL);

    // 主消息循环:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    //堆内存释放
    DataFreeMemory();
    return (int)msg.wParam;
}

/*
*   函数: RegisterWndClass(HINSTANCE)
*
*   目标: 注册主要的窗口类
*
*   输入：程序实例句柄
* 
*   返回：BOOL 判断函数调用是否成功
*/
BOOL RegisterWndClass(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    HBRUSH hBrush;
    /*
        登录界面窗口类
    */
    //登录界面主窗口类
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = LogInProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szLogIn;
    if (!RegisterClass(&wndclass)) return FALSE;
    /*
        主要窗口类
    */
    //主要窗口类
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    if (!RegisterClass(&wndclass)) return FALSE;

    //DIY子窗口控件
    //CHECKBOX
    wndclass.lpfnWndProc = DIY_CheckBoxProc;
    wndclass.lpszClassName = DIY_CHECKBOX;
    if (!RegisterClass(&wndclass)) return FALSE;
    return TRUE;
}


/*
*   函数: InitInstance(HINSTANCE, int)
*
*   目标: 保存实例句柄并创建登录窗口
*
*   注释:
*        在此函数中，在全局变量中保存实例句柄并
*        创建和显示登录窗口。
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

    hInst = hInstance; // 将实例句柄存储在全局变量中

    //获得用户屏幕信息
    cxScreen = GetSystemMetrics(SM_CXSCREEN);
    cyScreen = GetSystemMetrics(SM_CYSCREEN);

    //保存窗口大小
    cxRegister = cxLogIn = WNDSIZE_LOGIN_CX;
    cyRegister = cyLogIn = WNDSIZE_LOGIN_CY;
    cxMain = WNDSIZE_MAINWND_CX;
    cyMain = WNDSIZE_MAINWND_CY;

    //创建登录界面窗口
    hwndLogIn = CreateWindow(szLogIn,
        TEXT("登录飞机订票系统"),
        WS_OVERLAPPED | WS_SYSMENU | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        cxLogIn, cyLogIn,
        NULL, NULL, hInstance, NULL);

    if (!hwndLogIn)
    {
        return FALSE;
    }

    //立即显示登录界面
    ShowWindow(hwndLogIn, nCmdShow);
    UpdateWindow(hwndLogIn);
    return TRUE;
}
