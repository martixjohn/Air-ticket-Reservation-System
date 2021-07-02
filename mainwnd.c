/*
*       mainwnd.c
*       包含所有主要窗口界面相关函数
*/
#include "mainwnd.h"
UINT cxMain, cyMain;

static BOOL MainRegister(HINSTANCE hInstance);
/*Menu*/
static TCHAR szMenu[] = TEXT("MENU");
HBRUSH hbrMenuBk, hbrBodyBk;
COLORREF crMenuBk, crBodyBk;
HWND hwndMenu;
/*BODY*/
static TCHAR szBody[] = TEXT("BODY");
HWND hwndBody;

/*
*   函数: MainRegister(HINSTANCE hInstance)
*
*   目标: 注册主窗口下的窗口类
*
*   输入：hInstance   实例句柄
*
*   返回：BOOL        判断是否注册成功
*/
static BOOL MainRegister(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    /*MENU菜单*/
    hbrMenuBk = (HBRUSH)CreateSolidBrush(crMenuBk = RGB(43, 87, 154));
    wndclass.style = 0;
    wndclass.lpfnWndProc = MainMenuProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hbrMenuBk;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szMenu;
    if (!RegisterClass(&wndclass)) return FALSE;
    /*BODY主体内容*/
    hbrBodyBk = CreateSolidBrush(crBodyBk = RGB(238, 241, 246));
    wndclass.style = 0;
    wndclass.lpfnWndProc = MainBodyProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szBody;
    if (!RegisterClass(&wndclass)) return FALSE;

    return TRUE;
}


/*
*   函数: WndProc(HWND, UINT, WPARAM, LPARAM)
*
*   目标: 处理主窗口的消息。
*
*   输入：
*   hWnd       窗口句柄
*   message    消息通知码
*   wParam     额外信息
*   lParam     额外信息
*
*   返回：LRESULT 记录结果返回Windows
*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static BOOL fCreatedChild = FALSE;
    switch (uMsg)
    {
    case WM_CREATE:
        MainRegister(hInst);
        hwndMenu = CreateWindow(szMenu,
            TEXT("左侧菜单"),
            WS_CHILD | WS_CLIPCHILDREN,
            0, 0, 0, 0,
            hwnd, (HMENU)MAIN_MENU, hInst, NULL);
        hwndBody = CreateWindow(szBody,
            TEXT("右侧主体内容"),
            WS_CHILD | WS_CLIPCHILDREN,
            0, 0, 0, 0,
            hwnd, (HMENU)MAIN_BODY, hInst, NULL);
        return 0;

    case WM_SIZE:
        //注意窗口最小化也会调用WM_SIZE，子窗口不会
        cxMain = LOWORD(lParam);
        cyMain = HIWORD(lParam);
        if (cxMain == 0 || cyMain == 0) return 0;
        
        if (fCreatedChild == FALSE)
        {
            MoveWindow(hwndMenu, 0, 0, cxMain * 1 / 8, cyMain, FALSE);//此处子窗口调用了WM_SIZE
            ShowWindow(hwndMenu, SW_SHOWNORMAL);
            MoveWindow(hwndBody, cxMain * 1 / 8, 0, cxMain * 7 / 8, cyMain, FALSE);
            ShowWindow(hwndBody, SW_SHOWNORMAL);

            fCreatedChild = TRUE;
        }

        return 0;
    case WM_DESTROY:
        e_w = EXIT_IN_MAINWND;
        PostQuitMessage(0);
        return 0;
        
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
