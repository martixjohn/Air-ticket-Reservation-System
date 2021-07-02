/*
*       adminhome.c
*       包含所有管理员主页相关函数
*/
#include "bodyadmin.h"
//注册窗口类
static BOOL AdminHomeRegister(HINSTANCE hInstance);
//名称定义
static TCHAR szUserInfoName[] = TEXT("用户数据统计信息");
static TCHAR szFlightInfoName[] = TEXT("航班数据统计信息");

//子窗口过程及对应句柄
static LRESULT CALLBACK AdminHome_UserInfoProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static HWND hwndUserInfo;
static LRESULT CALLBACK AdminHome_FlightInfoProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static HWND hwndFlightInfo;

/*
*   函数: AdminHomeRegister(HINSTANCE hInstance)
*
*   目标: 注册AdminHome子窗口类
*
*   输入：hInstance        实例句柄
*
*   返回：BOOL             判断注册是否成功
*/
static BOOL AdminHomeRegister(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    HBRUSH hBrush;
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = AdminHome_UserInfoProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szUserInfoName;
    if (!RegisterClass(&wndclass)) return FALSE;

    
    wndclass.lpfnWndProc = AdminHome_FlightInfoProc;
    wndclass.lpszClassName = szFlightInfoName;
    if (!RegisterClass(&wndclass)) return FALSE;

    return TRUE;
}

/*主窗口*/
LRESULT CALLBACK AdminHomeProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //基础
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0"); TCHAR szBuf1[128] = TEXT("\0");
    //基础结束
    static HFONT hfWelcome, hfTime;
    static HICON hIcon;
    static USERACCOUNT ua;
    static RECT rcWelcome, rcTime;
    time_t timeNow;
    struct tm tmNow;
    switch (uMsg)
    {
    case WM_CREATE:
        DataUserAccount(&ua, UA_GET);
        AdminHomeRegister(hInst);
        return 0;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            //-------初始化开始
            DefInitWindow(hwnd, cxClient, cyClient, hbrBodyBk, &rcClient, &hdcMemBk, &hbmpBk);

            //欢迎标识
            hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_FLIGHT));
            DrawIcon(hdcMemBk, cxClient / 20, cyClient * 2 / 20, hIcon);
            SetRect(&rcWelcome, cxClient * 2 / 20, cyClient * 2 / 20, cxClient, cyClient * 3 / 20);
            hfWelcome = SetFont(hdcMemBk, rcWelcome.bottom - rcWelcome.top,
                (rcWelcome.bottom - rcWelcome.top) * 3 / 8, FONT_STYLE_DEFAULT);
            DataAccount_GetSelfInfoFromUA(szBuf1, UA_GETINFO_NICKNAME, &ua);
            StringCchPrintf(szBuf, 128, TEXT("欢迎您，管理员 %s"), szBuf1);
            DrawText(hdcMemBk, szBuf, -1, &rcWelcome, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
            //时间标识
            SetRect(&rcTime, cxClient * 12 / 20, cyClient * 1 / 10, cxClient, cyClient * 1 / 7);
            time(&timeNow);//系统时间
            localtime_s(&tmNow, &timeNow);//当地时间
            StringCchPrintf(szBuf, 128, TEXT("今天是%d年%d月%d日"), tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday);
            hfWelcome = SetFont(hdcMemBk, rcTime.bottom - rcTime.top,
                (rcTime.bottom - rcTime.top) * 3 / 8, FONT_STYLE_DEFAULT);
            SetTextColor(hdcMemBk, RGB(106, 106, 106));
            DrawText(hdcMemBk, szBuf, -1, &rcTime, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            //子窗口
            hwndUserInfo = CreateWindow(szUserInfoName,
                TEXT("用户数据"),
                WS_CHILD | WS_VISIBLE,
                cxClient * 2 / 20, cyClient * 5 / 20,
                cxClient * 7 / 20, cyClient * 12 / 20,
                hwnd, (HMENU)ADMINHOMEID_USERINFO, hInst, NULL
            );
            hwndFlightInfo = CreateWindow(szFlightInfoName,
                TEXT("航班数据"),
                WS_CHILD | WS_VISIBLE,
                cxClient * 11 / 20, cyClient * 5 / 20,
                cxClient * 7 / 20, cyClient * 12 / 20,
                hwnd, (HMENU)ADMINHOMEID_USERINFO, hInst, NULL
            );
            //-------初始化结束
            fInited = TRUE;
        }
        return 0;
    case WM_LBUTTONDOWN:

        SetFocus(hwnd);
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfWelcome);
        DeleteObject(hfTime);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//  目标：处理用户信息子窗口消息
static LRESULT CALLBACK AdminHome_UserInfoProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //基础
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //基础结束
    HBRUSH hbrBk;
    //TITLE
    static HFONT hfTitle;
    static RECT rcTitle;
    static RECT rcTitleflag;

    //CON
    static HFONT hfCon;
    static RECT rcUserNum;
    static HDC hdcMemTemp;
    static RECT rcUserSexNum;
    static HBITMAP hbmpUserNum, hbmpUserSex;
    /*数据处理*/
    //用户账户
    static LPUSERACCOUNTNODE pUaNode, pUaNode_head;
    static size_t iUserNum;
    static size_t iUserSexNum[2] = { 0 };//0男1女
    //temp
    BITMAP bmp;
    HDC hdc;

    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            hbrBk = (HBRUSH)GetStockObject(WHITE_BRUSH);
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk, &hbmpBk);
            SetRect(&rcTitle, cxClient / 20, cyClient / 20, cxClient, cyClient * 2 / 20);
            SetRect(&rcTitleflag, 0, rcTitle.top, rcTitle.left, rcTitle.bottom);
            hfTitle = SetFont(hdcMemBk,
                rcTitle.bottom-rcTitle.top, (rcTitle.bottom - rcTitle.top) * 3 / 8, FONT_STYLE_DEFAULT);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            SetBkMode(hdcMemBk, TRANSPARENT);
            FillRect(hdcMemBk, &rcTitleflag, hbrMenuBk);
            DrawText(hdcMemBk, TEXT("     用户数据"), -1, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
            //获取普通用户数据
            DataAccount_LinkedListMem(&pUaNode_head, UALIST_GET);
            
            SetRect(&rcUserNum, cxClient * 19 / 40, rcTitle.bottom + cyClient / 10 + cxClient / 16,
                cxClient * 9 / 10, rcTitle.bottom + cyClient / 10 + cxClient / 4 + cxClient / 16);
            SetRect(&rcUserSexNum, cxClient * 19 / 40, rcUserNum.bottom + cyClient / 10 + cxClient / 16,
                cxClient * 9 / 10, rcUserNum.bottom + cyClient / 10 + cxClient / 4 + cxClient / 16);
            hfCon = SetFont(hdcMemBk,
                cxClient / 4 / 4, cxClient / 4 / 4 * 3 / 8,
                FONT_STYLE_DEFAULT);
            pUaNode = pUaNode_head;
            while (pUaNode != NULL)
            {
                iUserNum++; 
                DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_SEX, &(pUaNode->ua));
                if (lstrcmp(szBuf, TEXT("男")) == 0)
                {
                    iUserSexNum[0]++;
                }
                if (lstrcmp(szBuf, TEXT("女")) == 0)
                {
                    iUserSexNum[1]++;
                }

                pUaNode = pUaNode->next;
            }
            //BMP
            hbmpUserNum = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_USERS));
            hbmpUserSex = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SEX));
            hdc = GetDC(hwnd);
            hdcMemTemp = CreateCompatibleDC(hdc);

            //USERNUM
            SelectObject(hdcMemTemp, hbmpUserNum);
            GetObject(hbmpUserNum, sizeof(BITMAP), &bmp);
            SetStretchBltMode(hdcMemBk, HALFTONE);
            StretchBlt(hdcMemBk, rcTitle.left + cxClient / 10, rcTitle.bottom + cyClient / 10,
                cxClient / 4, cxClient / 4,
                hdcMemTemp, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
            SelectObject(hdcMemTemp, hbmpUserSex);
            GetObject(hbmpUserSex, sizeof(BITMAP), &bmp);
            StretchBlt(hdcMemBk, rcTitle.left + cxClient / 10, rcUserNum.bottom + cyClient / 10,
                cxClient / 4, cxClient / 4,
                hdcMemTemp, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
            ReleaseDC(hwnd, hdc);
            SelectObject(hdcMemBk, hfCon);
            StringCchPrintf(szBuf, 32, TEXT("用户数目\n%d"), iUserNum);
            DrawText(hdcMemBk, szBuf, -1, &rcUserNum, DT_LEFT);
            StringCchPrintf(szBuf, 32, TEXT("用户性别\n男%d 女%d"), iUserSexNum[0], iUserSexNum[1]);
            DrawText(hdcMemBk, szBuf, -1, &rcUserSexNum, DT_LEFT);
            
            //用户
            fInited = TRUE;
        }
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfTitle); DeleteObject(hfCon);
        DeleteDC(hdcMemTemp);
        DeleteObject(hbmpUserNum);
        DeleteObject(hbmpUserSex);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//  目标：处理航班信息子窗口消息
static LRESULT CALLBACK AdminHome_FlightInfoProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //基础
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //基础结束
    HBRUSH hbrBk;
    //TITLE
    static HFONT hfTitle;
    static RECT rcTitle;
    static RECT rcTitleflag;
    static HBRUSH hbrTitleflag;

    //CON
    static HFONT hfCon;
    static HDC hdcMemTemp;
    static RECT rcFlightNum, rcOrderNum;
    static HBITMAP hbmpFlightNum, hbmpOrderNum;
    static IDDATA_NUM iFlightNum, iOrderNum;
    static LPFLIGHTDATANODE pFlNode;
    static LPORDERINFONODE pOINode;
    BITMAP bmp;
    HDC hdc;
    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            hbrBk = (HBRUSH)GetStockObject(WHITE_BRUSH);
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk, &hbmpBk);
            SetRect(&rcTitle, cxClient / 20, cyClient / 20, cxClient, cyClient * 2 / 20);
            SetRect(&rcTitleflag, 0, rcTitle.top, rcTitle.left, rcTitle.bottom);
            hfTitle = SetFont(hdcMemBk,
                rcTitle.bottom - rcTitle.top, (rcTitle.bottom - rcTitle.top) * 3 / 8, FONT_STYLE_DEFAULT);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            SetBkMode(hdcMemBk, TRANSPARENT);
            hbrTitleflag = CreateSolidBrush(RGB(161, 1, 251));
            FillRect(hdcMemBk, &rcTitleflag, hbrTitleflag);
            DrawText(hdcMemBk, TEXT("     航班数据"), -1, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
            

            SetRect(&rcFlightNum, cxClient * 19 / 40, rcTitle.bottom + cyClient / 10 + cxClient / 16,
                cxClient * 9 / 10, rcTitle.bottom + cyClient / 10 + cxClient / 4 + cxClient / 16);
            SetRect(&rcOrderNum, cxClient * 19 / 40, rcFlightNum.bottom + cyClient / 10 + cxClient / 16,
                cxClient * 9 / 10, rcFlightNum.bottom + cyClient / 10 + cxClient / 4 + cxClient / 16);
            hfCon = SetFont(hdcMemBk,
                cxClient / 4 / 4, cxClient / 4 / 4 * 3 / 8,
                FONT_STYLE_DEFAULT);
            
            //BMP
            hbmpFlightNum = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PLANECIRCLE));
            hbmpOrderNum = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ORDER));
            hdc = GetDC(hwnd);
            hdcMemTemp = CreateCompatibleDC(hdc);

            
            SelectObject(hdcMemTemp, hbmpFlightNum);
            GetObject(hbmpFlightNum, sizeof(BITMAP), &bmp);
            SetStretchBltMode(hdcMemBk, HALFTONE);
            StretchBlt(hdcMemBk, rcTitle.left + cxClient / 10, rcTitle.bottom + cyClient / 10,
                cxClient / 4, cxClient / 4,
                hdcMemTemp, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
            SelectObject(hdcMemTemp, hbmpOrderNum);
            GetObject(hbmpOrderNum, sizeof(BITMAP), &bmp);
            StretchBlt(hdcMemBk, rcTitle.left + cxClient / 10, rcFlightNum.bottom + cyClient / 10,
                cxClient / 4, cxClient / 4,
                hdcMemTemp, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
            ReleaseDC(hwnd, hdc);
            SelectObject(hdcMemBk, hfCon);
            iFlightNum = DataFlightLinkedListMem(&pFlNode, FLLIST_GET_HEADNODE);
            iOrderNum = DataOrderLinkedListMem(&pOINode, OILIST_GET_HEADNODE);
            StringCchPrintf(szBuf, 32, TEXT("航班数目\n%d"), iFlightNum);
            DrawText(hdcMemBk, szBuf, -1, &rcFlightNum, DT_LEFT);
            StringCchPrintf(szBuf, 32, TEXT("订单数目\n%d"), iOrderNum);
            DrawText(hdcMemBk, szBuf, -1, &rcOrderNum, DT_LEFT);
            fInited = TRUE;
        }
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteDC(hdcMemTemp);
        DeleteObject(hfTitle); DeleteObject(hbrTitleflag);
        DeleteObject(hbmpFlightNum); DeleteObject(hbmpOrderNum);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}