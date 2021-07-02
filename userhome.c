/*
*       userhome.c
*       包含所有用户主页面相关函数
*/
#include "bodyuser.h"

//注册窗口类
static BOOL UserHomeRegister(HINSTANCE);
//子窗口
static TCHAR szTrendName[] = TEXT("热门航班");
static TCHAR szSearchName[] = TEXT("搜索航班");
static TCHAR szSearch_DestDepartName[] = TEXT("搜索航班_目的出发");
static TCHAR szSearch_DateName[] = TEXT("搜索航班_航班时间");
static TCHAR szSearch_SearchButtonName[] = TEXT("搜索按钮");
static LRESULT CALLBACK UserHome_TrendListProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserHome_SearchProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserHome_Search_DestDepartProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserHome_Search_DateProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserHome_Search_SearchButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static WNDPROC DefSearchEditProc;
static LRESULT CALLBACK SearchEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//窗口句柄
static HWND hwndTrend, hwndSearch;

/*
*   函数: UserHomeRegister(HINSTANCE hInstance)
*
*   目标: 注册UserHome子窗口类
*
*   输入：hInstance        实例句柄 
*
*   返回：BOOL             判断注册是否成功
*/
static BOOL UserHomeRegister(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    HBRUSH hBrush;
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = UserHome_TrendListProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szTrendName;
    if (!RegisterClass(&wndclass)) return FALSE;
    
    /*Search*/
    wndclass.lpfnWndProc = UserHome_SearchProc;
    wndclass.lpszClassName = szSearchName;
    if (!RegisterClass(&wndclass)) return FALSE;
    wndclass.lpfnWndProc = UserHome_Search_DestDepartProc;
    wndclass.lpszClassName = szSearch_DestDepartName;
    if (!RegisterClass(&wndclass)) return FALSE;
    wndclass.lpfnWndProc = UserHome_Search_DateProc;
    wndclass.lpszClassName = szSearch_DateName;
    if (!RegisterClass(&wndclass)) return FALSE;
    wndclass.lpfnWndProc = UserHome_Search_SearchButtonProc;
    wndclass.lpszClassName = szSearch_SearchButtonName;
    if (!RegisterClass(&wndclass)) return FALSE;
    return TRUE;
}

/*
*   函数: UserHomeProc(HWND, UINT, WPARAM, LPARAM)
*
*   目标: 处理UserHome的消息。
*
*   输入：
*   hWnd       窗口句柄
*   message    消息通知码
*   wParam     额外信息
*   lParam     额外信息
*
*   返回：LRESULT 记录结果返回Windows
*/
LRESULT CALLBACK UserHomeProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //客户区相关
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    
    //欢迎标识
    static HFONT hfWelcome, hfTime;
    static USERACCOUNT ua;
    static RECT rcWelcome;
    time_t timeNow;
    struct tm tmNow;
    static TCHAR szNickName[LIMIT_NICKNAME];
    static TCHAR szTime[128] = TEXT("\0");
    switch (uMsg)
    {
    case WM_CREATE:
        UserHomeRegister(hInst);
        //获取用户账号信息，主要是用户名
        DataUserAccount(&ua, UA_GET);

        return 0;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            //-------初始化开始
            DefInitWindow(hwnd, cxClient, cyClient, hbrBodyBk, &rcClient, &hdcMemBk, &hbmpBk);
            
            hwndSearch = CreateWindow(szSearchName,
                TEXT("搜索航班"),
                WS_CHILD | WS_VISIBLE,
                cxClient * 1 / 10, cyClient * 5 / 20,
                cxClient * 4 / 10, cyClient * 12 / 20,
                hwnd, (HMENU)USERHOMEID_SEARCH, hInst, NULL
            );

            hwndTrend = CreateWindow(szTrendName,
                TEXT("热门机票"),
                WS_CHILD | WS_VISIBLE,
                cxClient * 12 / 20, cyClient * 3 / 20,
                cxClient * 6 / 20, cyClient * 14 / 20,
                hwnd, (HMENU)USERHOMEID_TREND, hInst, NULL
            );
            
            //欢迎标识
            SetRect(&rcWelcome, cxClient * 2 / 20, cyClient * 15 / 80, cxClient, cyClient * 10 / 40);
            hfWelcome = SetFont(hdcMemBk, rcWelcome.bottom - rcWelcome.top,
                (rcWelcome.bottom - rcWelcome.top) * 3 / 8, FONT_STYLE_DEFAULT + 500);
            DataAccount_GetSelfInfoFromUA(szNickName, UA_GETINFO_NICKNAME, &ua);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            SetTimer(hwnd, TID_WELCOME, 10, NULL);
            //时间标识
            time(&timeNow);//系统时间
            localtime_s(&tmNow, &timeNow);//当地时间
            StringCchPrintf(szTime, 128, TEXT("今天是%d年%d月%d日"), tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday);
            SetTextColor(hdcMemBk, RGB(106, 106, 106));

            //-------初始化结束
            fInited = TRUE;
        }
        return 0;
    case WM_TIMER:
        //欢迎动画
        if (wParam == TID_WELCOME)
        {
            static int times = 0;
            static int alltimes = 300;
            static int R, G, B, dstR, dstG, dstB;
            static int detR, detG, detB;
            if (times == 0)
            {
                SetTextColor(hdcMemBk, crBodyBk);
                R = GetRValue(crBodyBk);
                G = GetGValue(crBodyBk);
                B = GetBValue(crBodyBk);
                dstR = dstG = dstB = 80;
                detR = dstR - R; detG = dstG - G; detB = dstB - B;
            }
            FillRect(hdcMemBk, &rcWelcome, hbrBodyBk);
            SelectObject(hdcMemBk, hfWelcome);
            
            if (times <= alltimes / 7)
            {
                R += detR / (alltimes / 7);
                G += detG / (alltimes / 7);
                B += detB / (alltimes / 7);
                SetTextColor(hdcMemBk, RGB(R, G, B));
                DrawText(hdcMemBk, TEXT("欢迎您"), -1, &rcWelcome, DT_SINGLELINE | DT_VCENTER);
            }
            else if (times <= alltimes * 2 / 7)
            {
                SetTextColor(hdcMemBk, RGB(R, G, B));
                DrawText(hdcMemBk, TEXT("欢迎您"), -1, &rcWelcome, DT_SINGLELINE | DT_VCENTER);
            }
            else if (times <= alltimes * 3 / 7)
            {
                R -= detR / (alltimes / 7);
                G -= detG / (alltimes / 7);
                B -= detB / (alltimes / 7);
                SetTextColor(hdcMemBk, RGB(R, G, B));
                DrawText(hdcMemBk, TEXT("欢迎您"), -1, &rcWelcome, DT_SINGLELINE | DT_VCENTER);
            }
            else if (times <= alltimes * 4 / 7)
            {
                R += detR / (alltimes / 7);
                G += detG / (alltimes / 7);
                B += detB / (alltimes / 7);
                SetTextColor(hdcMemBk, RGB(R, G, B));
                DrawText(hdcMemBk, szNickName, -1, &rcWelcome, DT_SINGLELINE | DT_VCENTER);
            }
            else if (times <= alltimes * 5 / 7)
            {
                SetTextColor(hdcMemBk, RGB(R, G, B));
                DrawText(hdcMemBk, szNickName, -1, &rcWelcome, DT_SINGLELINE | DT_VCENTER);
            }
            else if(times <= alltimes * 6 / 7)
            {
                R -= detR / (alltimes / 7);
                G -= detG / (alltimes / 7);
                B -= detB / (alltimes / 7);
                SetTextColor(hdcMemBk, RGB(R, G, B));
                DrawText(hdcMemBk, szNickName, -1, &rcWelcome, DT_SINGLELINE | DT_VCENTER);
            }
            else
            {
                R += detR / (alltimes / 7);
                G += detG / (alltimes / 7);
                B += detB / (alltimes / 7);
                SetTextColor(hdcMemBk, RGB(R, G, B));
                DrawText(hdcMemBk, szTime, -1, &rcWelcome, DT_SINGLELINE | DT_VCENTER);
            }
            InvalidateRect(hwnd, &rcWelcome, FALSE);
            UpdateWindow(hwnd);
            times++;
            if (times == alltimes)
            {
                KillTimer(hwnd, TID_WELCOME);
                times = 0;
            }
        }
        return 0;
    case WM_LBUTTONDOWN:
        SendMessage(hwndSearch, WM_U_CANCLESELECTED, 0, 0);
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

//   目标: 处理UserHome中“热门航班”的消息。
static LRESULT CALLBACK UserHome_TrendListProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //基础
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    static HBRUSH hbrBk;
    TCHAR szBuf[128] = TEXT("\0");
    //基础结束
    static HFONT hfTitle;
    static HBRUSH hbrTitle;
    static RECT rcTitle, rcTrend;

    static int cxTrend, cyTrend;
    static int cxTrendList, cyTrendList;
    static RECT rcTrendList[5];
    static HPEN hpenLine;
    LPFLIGHTDATANODE pFlNode;
    IDDATA_NUM iddataFlightNum;
    static IDDATA_NUM iddataTrendNum;
    static int cyTrendCity, cyTrendPrice, cyTrendDescription;
    static HFONT hfTrendCity, hfTrendPrice, hfTrendDescription;
    static TCHAR szTrendCityName[5][32];
    static RECT rcTrendCityName[5];
    static IDDATA idTrend[5] = { 0 };
    IDDATA idAll[MAX_SEARCH_FLIGHT_ID] = { 0 };
    POINT pt;
    
    IDDATA_NUM i, j, k;
    
    
    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            hbrBk = hbrBodyBk;
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk, &hbmpBk);
            
            //title
            hbrTitle = CreateSolidBrush(RGB(0, 117, 224));
            SetRect(&rcTitle, 0, 0, cxClient, cyClient * 3 / 26);
            FillRect(hdcMemBk, &rcTitle, hbrTitle);
            hfTitle = SetFont(hdcMemBk,
                (rcTitle.bottom - rcTitle.top) * 2 / 4, 
                (rcTitle.bottom - rcTitle.top) * 2 / 4 * 3 / 8, 
                FONT_STYLE_DEFAULT + 500);
            SelectObject(hdcMemBk, hfTitle);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            SetBkMode(hdcMemBk, TRANSPARENT);
            DrawText(hdcMemBk, TEXT("    热门航班"), -1, &rcTitle, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
            //Trend内容
            cxTrend = cxClient;
            cyTrend = cyClient - rcTitle.bottom + rcTitle.top;
            SetRect(&rcTrend, 0, rcTitle.bottom, cxTrend, cyClient);
            FillRect(hdcMemBk, &rcTrend, (HBRUSH)GetStockObject(WHITE_BRUSH));
            cxTrendList = cxTrend * 8 / 10;
            cyTrendList = cyTrend * 3 / 21;
            hpenLine = CreatePen(PS_SOLID, 1, RGB(200,200,200));
            SelectObject(hdcMemBk, hpenLine);
            //TrendCity相关
            cyTrendCity = cyTrendList * 5 / 12;
            hfTrendCity = SetFont(hdcMemBk,
                cyTrendCity,
                cyTrendCity * 3 / 8,
                FONT_STYLE_DEFAULT + 500);
            
            //TrendPrice相关
            cyTrendPrice = cyTrendList * 1 / 3;
            hfTrendPrice = SetFont(hdcMemBk,
                cyTrendPrice,
                cyTrendPrice * 3 / 8,
                FONT_STYLE_DEFAULT + 500);

            //TrendDescription相关
            cyTrendDescription = cyTrendList * 1 / 3;
            hfTrendDescription = SetFont(hdcMemBk,
                cyTrendDescription,
                cyTrendDescription * 3 / 8,
                FONT_STYLE_DEFAULT + 500);
            //随机展示内容
            iddataFlightNum = DataFlightLinkedListMem(&pFlNode, FLLIST_GET_HEADNODE);
            //存储所有ID
            for (j = 0; j < MAX_SEARCH_FLIGHT_ID && pFlNode != NULL; j++)
            {
                idAll[j] = pFlNode->fl.id;
                pFlNode = pFlNode->next;
            }
            for (i = 0; i < min(5, iddataFlightNum); i++)
            {
                SetRect(&rcTrendList[i],
                    (cxTrend - cxTrendList) / 2, 
                    rcTrend.top + cyTrendList / 3 * (i + 1) + cyTrendList * i,
                    (cxTrend - cxTrendList) / 2 + cxTrendList,
                    rcTrend.top + cyTrendList / 3 * (i + 1) + cyTrendList * i + cyTrendList);
                FillRect(hdcMemBk, &rcTrendList[i], (HBRUSH)GetStockObject(WHITE_BRUSH));
                if (i != 4)
                {
                    MoveToEx(hdcMemBk, (cxTrend - cxTrendList) / 2, rcTrendList[i].bottom + cyTrendList / 10, NULL);
                    LineTo(hdcMemBk, (cxTrend - cxTrendList) / 2 + cxTrendList, rcTrendList[i].bottom + cyTrendList / 10);
                }
                //链表操作
                {
                    TCHAR szBuf1[128] = TEXT("\0"), szBuf2[128] = TEXT("\0");
                    static TCHAR szCity[32] = TEXT("\0"), szCityOld[5][32] = { TEXT("\0") };
                    TCHAR szPrice[10] = TEXT("\0"); TCHAR szDescription[10] = TEXT("\0");
                    k = 0;
                RE_SEARCH_TREND:    
                    j = rand() % iddataFlightNum;
                    idTrend[i] = idAll[j];
                    pFlNode = DataFlight_GetPNode(idTrend[i]);
                    DataFlight_GetCityName(szBuf1, pFlNode->fl.szDepart);
                    DataFlight_GetCityName(szBuf2, pFlNode->fl.szDest);
                    //城市名
                    StringCchPrintf(szCity, 32, TEXT("%s -> %s"), szBuf1, szBuf2);
                    //存在相同，票数为0，不显示
                    for (j = 0; j < 5; j++)
                    {
                        if (j == i) continue;
                        else
                        {
                            if (lstrcmp(szCity, szCityOld[j]) == 0)
                            {
                                k++;
                                if (k == 5)return 0;
                                goto RE_SEARCH_TREND;
                            }
                        }
                    }
                    if (pFlNode->fl.iAmount == 0)
                    {
                        goto RE_SEARCH_TREND;
                    }
                    SelectObject(hdcMemBk, hfTrendCity);
                    SetTextColor(hdcMemBk, RGB(76, 76, 76));
                    SetTextAlign(hdcMemBk, TA_LEFT);
                    SetRect(&rcTrendCityName[i], rcTrendList[i].left, rcTrendList[i].top, 
                        cxTrendList * 4 / 5, rcTrendList[i].top + cyTrendCity);
                    DrawText(hdcMemBk, szCity, -1, &rcTrendCityName[i], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                    //保存当前名称，以免重复
                    lstrcpy(szCityOld[i], szCity);
                    lstrcpy(szTrendCityName[i], szCity);
                    //价格
                    StringCchPrintf(szPrice, 10, TEXT("￥%.1lf"), pFlNode->fl.dPrice);
                    SelectObject(hdcMemBk, hfTrendPrice);
                    SetTextColor(hdcMemBk, RGB(0, 134, 246));
                    SetTextAlign(hdcMemBk, TA_RIGHT);
                    TextOut(hdcMemBk, rcTrendList[i].right, rcTrendList[i].top + cyTrendCity - cyTrendPrice, szPrice, lstrlen(szPrice));

                    //描述
                    StringCchPrintf(szDescription, 10, TEXT("%d月%d日"), pFlNode->fl.dtDepart.month, pFlNode->fl.dtDepart.day);
                    SelectObject(hdcMemBk, hfTrendDescription);
                    SetTextColor(hdcMemBk, RGB(153, 153, 153));
                    SetTextAlign(hdcMemBk, TA_LEFT);
                    TextOut(hdcMemBk, rcTrendList[i].left, rcTrendList[i].bottom - cyTrendDescription,
                        szDescription, lstrlen(szDescription));
                    iddataTrendNum++;
                }  //链表操作
            }//for结束
            fInited = TRUE;
        }

        return 0;
    case WM_MOUSEMOVE:
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        for (i = 0; i < min(iddataTrendNum, 5); i++)
        {
            if (PT_ON_RECT(pt, rcTrendList[i]))
            {
                SelectObject(hdcMemBk, hfTrendCity);
                SetTextColor(hdcMemBk, RGB(0, 134, 246));
                SetTextAlign(hdcMemBk, TA_LEFT);
                FillRect(hdcMemBk, &rcTrendCityName[i], GetStockObject(WHITE_BRUSH));
                DrawText(hdcMemBk, szTrendCityName[i], -1, &rcTrendCityName[i], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                InvalidateRect(hwnd, &rcTrendCityName[i], FALSE);
                UpdateWindow(hwnd);
                break;
            }
            else
            {
                SelectObject(hdcMemBk, hfTrendCity);
                SetTextColor(hdcMemBk, RGB(76, 76, 76));
                SetTextAlign(hdcMemBk, TA_LEFT);
                FillRect(hdcMemBk, &rcTrendCityName[i], GetStockObject(WHITE_BRUSH));
                DrawText(hdcMemBk, szTrendCityName[i], -1, &rcTrendCityName[i], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                InvalidateRect(hwnd, &rcTrendCityName[i], FALSE);
                UpdateWindow(hwnd);
            }
        }

        return 0;
    case WM_LBUTTONUP:
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        for (i = 0; i < min(iddataTrendNum,5); i++)
        {
            if (PT_ON_RECT(pt, rcTrendList[i]))
            {
                SelectObject(hdcMemBk, hfTrendCity);
                SetTextColor(hdcMemBk, RGB(76, 76, 76));
                SetTextAlign(hdcMemBk, TA_LEFT);
                FillRect(hdcMemBk, &rcTrendCityName[i], GetStockObject(WHITE_BRUSH));
                DrawText(hdcMemBk, szTrendCityName[i], -1, &rcTrendCityName[i], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                SendMessage(hwndMenuOpt[1], WM_LBUTTONUP, 0, 0);
                SendMessage(hwndBodyOpt[0][1], WM_U_SHOWFLIGHTDETAIL, (WPARAM)idTrend[i], 0);
                break;
            }
            
        }

        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hbrBk);
        DeleteObject(hbrTitle);
        DeleteObject(hpenLine);
        DeleteObject(hfTitle);
        DeleteObject(hfTrendCity);
        DeleteObject(hfTrendPrice);
        DeleteObject(hfTrendDescription);
        
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//   目标: 处理UserHome中“搜索航班”的搜索界面的消息。
static LRESULT CALLBACK UserHome_SearchProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //基础
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //基础结束

    static HBRUSH hbrBk;
    static HWND hwndDepart, hwndDest, hwndDate;
    static HBRUSH hbrBorder;
    static int cxBorder;
    static RECT rcBoder;
    static SIZE sizeDestDepart;
    static HFONT hfTitle;
    static RECT rcTitle;
    static int cyTitle;

    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {   
            hbrBk = hbrBodyBk;
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk, &hbmpBk);
            cxBorder = 3;
            SetRect(&rcTitle, 0, 0, cxClient, cyClient * 4 / 10);
            sizeDestDepart.cx = (cxClient - cxBorder) * 1 / 2;
            sizeDestDepart.cy = cyClient * 1 / 4;
            hfTitle = SetFont(hdcMemBk, (rcTitle.bottom-rcTitle.top) * 3 / 4,
                (rcTitle.bottom - rcTitle.top) * 3 / 4 * 3 / 8, FONT_STYLE_DEFAULT + 200);
            SelectObject(hdcMemBk, hfTitle);
            SetTextColor(hdcMemBk, RGB(90, 90, 90));
            SetBkMode(hdcMemBk, TRANSPARENT);
            DrawText(hdcMemBk, TEXT("即刻出发"), -1, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
            SetRect(&rcBoder, 
                sizeDestDepart.cx, rcTitle.bottom,
                sizeDestDepart.cx + cxBorder, sizeDestDepart.cy + rcTitle.bottom);
            hwndDepart = CreateWindow(szSearch_DestDepartName,
                TEXT("出发地"),
                WS_CHILD | WS_VISIBLE,
                0, rcTitle.bottom,
                sizeDestDepart.cx, sizeDestDepart.cy,
                hwnd, (HMENU)USERHOMEID_SEARCH_DEPART, hInst, NULL);
            hwndDest = CreateWindow(szSearch_DestDepartName,
                TEXT("目的地"),
                WS_CHILD | WS_VISIBLE,
                sizeDestDepart.cx + cxBorder, rcTitle.bottom,
                sizeDestDepart.cx, sizeDestDepart.cy,
                hwnd, (HMENU)USERHOMEID_SEARCH_DEST, hInst, NULL);
            
            hwndDate = CreateWindow(szSearch_DateName,
                TEXT("时间选择"),
                WS_CHILD | WS_VISIBLE,
                0, rcTitle.bottom + sizeDestDepart.cy,
                sizeDestDepart.cx * 2 + cxBorder, sizeDestDepart.cy * 4 / 5,
                hwnd, (HMENU)USERHOMEID_SEARCH_DATE, hInst, NULL);
            hbrBorder = (HBRUSH)CreateSolidBrush(RGB(150, 150, 150));
            SelectObject(hdcMemBk, (HBRUSH)hbrBorder);
            FillRect(hdcMemBk, &rcBoder, hbrBorder);
            
            fInited = TRUE;
        }
        return 0;
    case WM_U_CANCLESELECTED:case WM_LBUTTONDOWN:
        SetFocus(hwnd);
        SendMessage(hwndDepart, WM_U_CANCLESELECTED, 0, 0);
        SendMessage(hwndDest, WM_U_CANCLESELECTED, 0, 0);
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;

    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hbrBorder);
        DeleteObject(hfTitle);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}


static HWND hwndSearchDepartInput,hwndSearchDestInput;
static HWND hwndSearchDepartMonthInput, hwndSearchDepartDayInput;

//   目标: 处理UserHome中“搜索航班”的搜索框的消息。
static LRESULT CALLBACK SearchEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hwndParent;
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        hwndParent = GetParent(hwnd);
        SendMessage(hwndParent, WM_LBUTTONUP, 0, 0);
        break;
    }
    return CallWindowProc(DefSearchEditProc, hwnd, uMsg, wParam, lParam);
}

//   目标: 处理UserHome中“搜索航班”的“目的出发地”的消息。
static LRESULT CALLBACK UserHome_Search_DestDepartProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //基础
    static BOOL fFirstWindow = TRUE, fInited[2] = { FALSE,FALSE };
    static int cxClient, cyClient;
    static HDC hdcMemBk[2];
    static HBITMAP hbmpBk[2];
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //基础结束

    //选中状态
    static BOOL fSelected[2] = { FALSE, FALSE };
    //展开状态
    static BOOL fExtended[2] = { FALSE, FALSE };
    static COLORREF crShadow, crShadow_selected;
    static HBRUSH hbrBk, hbrShadow, hbrShadow_selected;
    static HFONT hfTitle;
    static TCHAR szTitle[2][4] = { TEXT("\0"), TEXT("\0") };
    static int cyTitle, cxTitle;
    static int cyShadow;
    static RECT rcShadow, rcMain;
    //input
    static HWND hwndInput[2];
    static HFONT hfInput;
    
    //id获取
    int id = GetWindowLongPtr(hwnd, GWL_ID);
    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fFirstWindow)
        {
            hbrBk = (HBRUSH)GetStockObject(WHITE_BRUSH);
            hbrShadow = CreateSolidBrush((crShadow = RGB(215, 215, 215)));
            hbrShadow_selected = CreateSolidBrush((crShadow_selected = crMenuBk));
            cyTitle = cyClient * 1 / 4;
            cxTitle = cyTitle * 3 / 8;
            cyShadow = cyClient / 50;
            hfTitle = CreateFont(cyTitle, cxTitle, 0, 0, 400,
                FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
                DEFAULT_CHARSET, //使用默认字符集
                OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS,
                PROOF_QUALITY, //默认输出质量
                DEFAULT_PITCH,
                APP_DEFAULT_FONT //字体名
            );
            hfInput = CreateFont(cyClient * 1 / 3, cyClient * 1 / 3 * 3 / 8, 0, 0, 400,
                FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
                DEFAULT_CHARSET, //使用默认字符集
                OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS,
                PROOF_QUALITY, //默认输出质量
                DEFAULT_PITCH,
                APP_DEFAULT_FONT //字体名
            );
            fFirstWindow = FALSE;
        }
        if (fInited[id] == FALSE)
        {
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk[id], &hbmpBk[id]);
            rcShadow = rcMain = rcClient;
            rcMain.bottom = rcClient.bottom * 19 / 20;
            rcShadow.top = rcMain.bottom;
            SelectObject(hdcMemBk[id], (HBRUSH)hbrBk);
            FillRect(hdcMemBk[id], &rcMain, hbrBk);
            SelectObject(hdcMemBk[id], (HBRUSH)hbrShadow);
            FillRect(hdcMemBk[id], &rcShadow, hbrShadow);
            SelectObject(hdcMemBk[id], hfTitle);
            SetBkMode(hdcMemBk[id], TRANSPARENT);
            SetTextColor(hdcMemBk[id], RGB(122, 122, 122));
            hwndInput[id] = CreateWindow(TEXT("edit"),
                NULL, WS_CHILD,
                cxClient / 10, cyClient * 1 / 2,
                cxClient * 8 / 10, cyClient * 1 / 3,
                hwnd,
                (HMENU)id, hInst, NULL);
            switch (id)
            {
            case USERHOMEID_SEARCH_DEPART:
                StringCchPrintf(szTitle[id], 4, TEXT("出发地"));
                hwndSearchDepartInput = hwndInput[id];
                break;
            case USERHOMEID_SEARCH_DEST:
                StringCchPrintf(szTitle[id], 4, TEXT("目的地"));
                hwndSearchDestInput = hwndInput[id];
                break;
            }
            //DrawText(hdcMemBk[id], szTitle[id], -1, &rcMain, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            TextOut(hdcMemBk[id], (cxClient - cxTitle * 2 * 3) / 2, (cyClient - cyTitle) / 2, szTitle[id], lstrlen(szTitle[id]));
            DefSearchEditProc = (WNDPROC)SetWindowLongPtr(hwndInput[id], GWL_WNDPROC, (LONG)SearchEditProc);
            SendMessage(hwndInput[id], EM_LIMITTEXT, 32, 0);
            SendMessage(hwndInput[id], WM_SETFONT, (WPARAM)hfInput, 0);

            fInited[id] = TRUE;
        }
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk[id]);
        return 0;
    case WM_CTLCOLOREDIT:
        return (LRESULT)(HBRUSH)GetStockObject(WHITE_BRUSH);
    case WM_LBUTTONDOWN:
        SetFocus(hwnd);
        return 0;
    case WM_LBUTTONUP:
        if (fSelected[id] == FALSE)
        {
            //选中当前
            SetTimer(hwnd, TID_SELECT + id, 1, NULL);
            fSelected[id] = TRUE;
            //取消其他
            switch (id)
            {
            case USERHOMEID_SEARCH_DEST:
                if (fSelected[USERHOMEID_SEARCH_DEPART])
                {
                    SendMessage(GetDlgItem(hwndSearch, USERHOMEID_SEARCH_DEPART), WM_U_CANCLESELECTED, 0, 0);
                }

                break;
            case USERHOMEID_SEARCH_DEPART:
                if (fSelected[USERHOMEID_SEARCH_DEST])
                {
                    SendMessage(GetDlgItem(hwndSearch, USERHOMEID_SEARCH_DEST), WM_U_CANCLESELECTED, 0, 0);
                }
                break;
            }
        }

        return 0;
    case WM_U_CANCLESELECTED:
        if (fSelected[id])
        {
            SetTimer(hwnd, TID_CANCLESELECT + id, 1, NULL);
            fSelected[id] = FALSE;
        }
        return 0;
    
    case WM_TIMER:
        //选中动画
        if (wParam == TID_SELECT + id)
        {
            static int time = 0;
            HBRUSH hbrShadowNow;
            COLORREF crShadowNow;
            static int iNowR, iNowG, iNowB;
            static int idstR, idstG, idstB;
            static int idetR, idetG, idetB;
            static SIZE sizeNowTitle, sizeDetTitle, sizeDestTitle;
            static POINT ptDestTitle, ptNowTitle, ptDetTitle;
            HFONT hfTitleNow;
            if (time == 0)
            {
                //底部
                crShadowNow = crShadow;
                idstR = GetRValue(crShadow_selected);
                idstG = GetGValue(crShadow_selected);
                idstB = GetBValue(crShadow_selected);
                iNowR = GetRValue(crShadow);
                iNowG = GetGValue(crShadow);
                iNowB = GetBValue(crShadow);
                idetR = idstR - iNowR;
                idetG = idstG - iNowG;
                idetB = idstB - iNowB;
                if (fExtended[id] == FALSE)
                {
                    //TITLE
                    sizeNowTitle.cy = cyTitle;
                    sizeNowTitle.cx = cxTitle;
                    sizeDestTitle.cx = sizeNowTitle.cx;
                    sizeDestTitle.cy = sizeNowTitle.cy ;
                    sizeDetTitle.cx = sizeDestTitle.cx - sizeNowTitle.cx;
                    sizeDetTitle.cy = sizeDestTitle.cy - sizeNowTitle.cy;
                    ptNowTitle.x = (cxClient - cxTitle * 2 * 3) / 2;
                    ptNowTitle.y = (cyClient - cyTitle) / 2;
                    ptDestTitle.x = ptDestTitle.y = cyTitle / 8;
                    ptDetTitle.x = ptDestTitle.x - ptNowTitle.x;
                    ptDetTitle.y = ptDestTitle.y - ptNowTitle.y;
                }
            }
            else
            {
                iNowR += idetR / 10;
                iNowG += idetG / 10;
                iNowB += idetB / 10;
                crShadowNow = RGB(iNowR, iNowG, iNowB);
                if (fExtended[id] == FALSE)
                {
                    sizeNowTitle.cx += sizeDetTitle.cx / 10;
                    sizeNowTitle.cy += sizeDetTitle.cy / 10;
                    ptNowTitle.x += ptDetTitle.x / 10;
                    ptNowTitle.y += ptDetTitle.y / 10;
                }
            }
            time++;
            //Shawdow sub
            hbrShadowNow = CreateSolidBrush(crShadowNow);
            FillRect(hdcMemBk[id], &rcShadow, hbrShadowNow);
            //选出
            SelectObject(hdcMemBk[id], hbrBk);
            DeleteObject(hbrShadowNow);
            if (fExtended[id] == FALSE)
            {
                FillRect(hdcMemBk[id], &rcMain, hbrBk);
                //TITLE
                hfTitleNow = SetFont(hdcMemBk[id], sizeNowTitle.cy, sizeNowTitle.cx, FONT_STYLE_DEFAULT);
            
                TextOut(hdcMemBk[id], ptNowTitle.x, ptNowTitle.y, szTitle[id], lstrlen(szTitle[id]));
                //选回
                SelectObject(hdcMemBk[id], (HFONT)hfTitle);
                //删除
                DeleteObject((HFONT)hfTitleNow);
            }
            
            InvalidateRect(hwnd, &rcClient, FALSE);
            UpdateWindow(hwnd);
            
            if (time == 10)
            {
                time = 0;
                fExtended[id] = TRUE;
                ShowWindow(hwndInput[id], SW_SHOW);
                SetFocus(hwndInput[id]);
                KillTimer(hwnd, TID_SELECT + id);
            }

            break;
        }
        //取消选中动画
        if (wParam == TID_CANCLESELECT + id)
        {
            static int time = 0;
            HBRUSH hbrShadowNow;
            COLORREF crShadowNow;
            static int iNowR, iNowG, iNowB;
            static int idstR, idstG, idstB;
            static int idetR, idetG, idetB;
            if (time == 0)
            {
                crShadowNow = crShadow_selected;
                idstR = GetRValue(crShadow);
                idstG = GetGValue(crShadow);
                idstB = GetBValue(crShadow);
                iNowR = GetRValue(crShadow_selected);
                iNowG = GetGValue(crShadow_selected);
                iNowB = GetBValue(crShadow_selected);
                idetR = idstR - iNowR;
                idetG = idstG - iNowG;
                idetB = idstB - iNowB;
            }
            else
            {
                iNowR += idetR / 10;
                iNowG += idetG / 10;
                iNowB += idetB / 10;
                crShadowNow = RGB(iNowR, iNowG, iNowB);
            }
            hbrShadowNow = CreateSolidBrush(crShadowNow);
            FillRect(hdcMemBk[id], &rcShadow, hbrShadowNow);
            DeleteObject(hbrShadowNow);
            InvalidateRect(hwnd, &rcShadow, FALSE);
            UpdateWindow(hwnd);
            time++;
            if (time == 10)
            {
                time = 0;
                FillRect(hdcMemBk[id], &rcShadow, hbrShadow);
                InvalidateRect(hwnd, &rcShadow, FALSE);
                UpdateWindow(hwnd);
                KillTimer(hwnd, TID_CANCLESELECT + id);
            }

            break;
        }

        return 0;
     
    case WM_DESTROY:
        DefDelWindow(hdcMemBk[id], hbmpBk[id]);
        DeleteObject(hbrShadow);
        DeleteObject(hbrShadow_selected);
        DeleteObject(hfTitle);
        DeleteObject(hfInput);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

//   目标: 处理UserHome中“搜索航班”的“出发时间”的消息。
static LRESULT CALLBACK UserHome_Search_DateProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //基础
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //基础结束
    static HWND hwndSearchButton;
    static HBRUSH hbrBk, hbrShadow;
    static RECT rcMonthText, rcDayText, rcMonthInputSub, rcDayInputSub;
    static SIZE sizeInput,sizeText;
    static HFONT hfText, hfInput;

    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            hbrShadow = CreateSolidBrush(RGB(215, 215, 215));
            hbrBk = (HBRUSH)GetStockObject(WHITE_BRUSH);
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk, &hbmpBk);
            
            sizeText.cx = cxClient * 1 / 10;
            sizeText.cy = cyClient / 2;
            sizeInput.cx = sizeText.cx;
            sizeInput.cy = sizeText.cy * 19 / 20;
            hfText = SetFont(hdcMemBk, sizeInput.cy, sizeInput.cy * 3 / 8, FONT_STYLE_DEFAULT + 450);
            hfInput = CreateFont(sizeInput.cy, sizeInput.cy * 3 / 8, 0, 0, 400,
                FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
                DEFAULT_CHARSET, //使用默认字符集
                OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS,
                PROOF_QUALITY, //默认输出质量
                DEFAULT_PITCH,
                APP_DEFAULT_FONT //字体名
            );
            SelectObject(hdcMemBk, (HFONT)hfText);
            SetBkMode(hdcMemBk, TRANSPARENT);

            //月输入
            hwndSearchDepartMonthInput = CreateWindow(TEXT("edit"),
                NULL,
                WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
                cxClient * 2 / 20, cyClient / 4,
                sizeInput.cx, sizeInput.cy,
                hwnd, (HMENU)USERHOMEID_SEARCH_DATE_INPUT1, hInst, NULL);
            SetRect(&rcMonthInputSub,
                cxClient * 2 / 20,                  cyClient / 4 + sizeInput.cy,
                cxClient * 2 / 20 + sizeInput.cx,   cyClient / 4 + sizeText.cy);
            FillRect(hdcMemBk, &rcMonthInputSub, hbrShadow);
            //月单位
            SetRect(&rcMonthText, 
                rcMonthInputSub.right + sizeInput.cx / 4,
                cyClient / 4, 
                rcMonthInputSub.right + sizeInput.cx / 4 + sizeText.cx,
                cyClient / 4 + sizeText.cy);
            DrawText(hdcMemBk, TEXT("月"), 1, &rcMonthText, DT_CENTER);

            //日输入
            hwndSearchDepartDayInput = CreateWindow(TEXT("edit"),
                NULL,
                WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
                rcMonthText.right + sizeInput.cx / 4, cyClient / 4,
                sizeInput.cx, sizeInput.cy,
                hwnd, (HMENU)USERHOMEID_SEARCH_DATE_INPUT2, hInst, NULL);
            SetRect(&rcDayInputSub,
                rcMonthText.right + sizeInput.cx / 4,                   cyClient / 4 + sizeInput.cy,
                rcMonthText.right + sizeInput.cx / 4 + sizeInput.cx,    cyClient / 4 + sizeText.cy);
            FillRect(hdcMemBk, &rcDayInputSub, hbrShadow);
            //日单位
            SetRect(&rcDayText,
                rcDayInputSub.right + sizeInput.cx / 4,
                cyClient / 4,
                rcDayInputSub.right + sizeInput.cx / 4 + sizeText.cx,
                cyClient / 4 + sizeText.cy);
            DrawText(hdcMemBk, TEXT("日"), 1, &rcDayText, DT_CENTER);
            //搜索按钮
            hwndSearchButton = CreateWindow(szSearch_SearchButtonName,
                TEXT("搜索按钮"),
                WS_CHILD | WS_BORDER | WS_VISIBLE,
                cxClient * 13 / 20, cyClient / 4,
                cxClient * 6 / 20, cyClient / 2,
                hwnd, (HMENU)USERHOMEID_SEARCH_SEARCHBUTTON, hInst, NULL);
            SendMessage(hwndSearchDepartMonthInput, WM_SETFONT, (WPARAM)hfInput, 0);
            SendMessage(hwndSearchDepartDayInput, WM_SETFONT, (WPARAM)hfInput, 0);
            SendMessage(hwndSearchDepartMonthInput, EM_SETLIMITTEXT, 2, 0);
            SendMessage(hwndSearchDepartDayInput, EM_SETLIMITTEXT, 2, 0);
            fInited = TRUE;
        }
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_CTLCOLOREDIT:
    {
        HDC hdcEdit = (HDC)wParam;
        HWND hwndEdit = (HWND)wParam;
        SetTextColor(hdcEdit, RGB(144, 144, 144));
        return (LRESULT)(HBRUSH)GetStockObject(WHITE_BRUSH);
    }
    case WM_LBUTTONDOWN:
        SendMessage(hwndSearch, WM_U_CANCLESELECTED, 0, 0);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfText);
        DeleteObject(hfInput);
        DeleteObject(hbrShadow);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//   目标: 处理UserHome中“搜索航班”的搜索按钮的消息。
static LRESULT CALLBACK UserHome_Search_SearchButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //基础
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //基础结束
    static HBRUSH hbrBk, hbrBk_selected, hbrBk_hover;
    static HFONT hfTitle;
    static BOOL fClicked = FALSE;
    static COLORREF crBk, crBk_hover;
    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            hbrBk = CreateSolidBrush(crBk = RGB(0, 112, 187));
            hbrBk_hover = CreateSolidBrush(crBk_hover = RGB(0, 122, 204));
            hbrBk_selected = CreateSolidBrush(RGB(0,60,150));
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk, &hbmpBk);
            SelectObject(hdcMemBk, hfTitle);
            hfTitle = SetFont(hdcMemBk, cyClient / 2, cyClient / 2 * 3 / 8, FONT_STYLE_DEFAULT);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            SetBkMode(hdcMemBk, TRANSPARENT);
            DrawText(hdcMemBk, TEXT("搜索"), -1, &rcClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            fInited = TRUE;
        }
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_MOUSEMOVE:
    {
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT),TME_LEAVE,hwnd,30 };
        TrackMouseEvent(&tme);
        if (!fClicked)
        {
            FillRect(hdcMemBk, &rcClient, hbrBk_hover);
            DrawText(hdcMemBk, TEXT("搜索"), -1, &rcClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            InvalidateRect(hwnd, NULL, FALSE);
            UpdateWindow(hwnd);
        }
        
    }
        return 0;
    case WM_MOUSELEAVE:
        
        FillRect(hdcMemBk, &rcClient, hbrBk);
        DrawText(hdcMemBk, TEXT("搜索"), -1, &rcClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        return 0;
    case WM_LBUTTONDOWN:
        fClicked = TRUE;
        
        FillRect(hdcMemBk, &rcClient, hbrBk_selected);
        DrawText(hdcMemBk, TEXT("搜索"), -1, &rcClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        return 0;
    case WM_LBUTTONUP:
        fClicked = FALSE;
        FillRect(hdcMemBk, &rcClient, hbrBk_hover);
        DrawText(hdcMemBk, TEXT("搜索"), -1, &rcClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        /*以下为数据处理*/ 
        {
            FL_SEARCHDATA fl_sd;
            IDDATA iddata[MAX_SEARCH_FLIGHT_ID] = { 0 };
            BOOL fValid = TRUE;
            GetWindowText(hwndSearchDepartInput, fl_sd.szDepart, 32);
            GetWindowText(hwndSearchDestInput, fl_sd.szDest, 32);
            GetWindowText(hwndSearchDepartMonthInput, fl_sd.szDepartMonth, 3);
            GetWindowText(hwndSearchDepartDayInput, fl_sd.szDepartDay, 3);
            if (fl_sd.szDepart[0] == TEXT('\0') || fl_sd.szDest[0] == TEXT('\0') ||
                fl_sd.szDepartMonth[0] == TEXT('\0') ||
                fl_sd.szDepartDay[0] == TEXT('\0') ||
                FALSE == DataValidDateText(fl_sd.szDepartMonth, fl_sd.szDepartDay)
                )
            {
                fValid = FALSE;
            }
            if (!fValid)
            {
                if (IDOK == MessageBox(hwnd, TEXT("       请输入正确的数据哦     "), TEXT("提示"), MB_OK))
                {
                    return 0;
                }
            }
            else
            {
                fl_sd.fl_searchoption = FL_SEARCHOPTION_DEPARTANDDEST | FL_SEARCHOPTION_DEPARTTIME;
                //搜索数据
                DataFlightSearch_ReturnIDDATA(fl_sd, iddata);
                //暂存数据
                DataFlightSearchRet(iddata, FL_SR_SET);
                SendMessage(hwndMenuOpt[1], WM_LBUTTONUP, 0, 0);
                SendMessage(hwndBodyOpt[0][1], WM_U_SHOWSEARCHUI, (WPARAM)TRUE, 0);
            }

        }
        
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfTitle);
        DeleteObject(hbrBk_hover);
        DeleteObject(hbrBk);
        DeleteObject(hbrBk_selected);
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
