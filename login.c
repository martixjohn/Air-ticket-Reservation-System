/*
*       login.c
*       包含所有登录界面相关函数
*/
#include "login.h"
/*名字定义*/

static TCHAR szLogIn_InputBox[] = TEXT("登录窗口输入盒子");      //登录子窗口输入框 
static TCHAR szLogIn_InputButton[] = TEXT("输入盒子输入框");
static TCHAR szLogIn_Button[] = TEXT("输入盒子提交按钮");
static TCHAR szRegisterInfo[] = TEXT("填写个人详细信息");
/*窗口句柄*/
extern HWND hwndLogIn;
HWND hwndMain;
static HWND hwndInputButton_U, hwndInputButton_P;
static HWND hwndRegisterBtn, hwndSignInBtn;
HWND hwndRegister;
static HWND hwndUNEdit, hwndPWEdit;
/*函数前向声明*/
static BOOL LogInRegisterWnd(HINSTANCE hInstance);
static LRESULT CALLBACK LogInInputBoxProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK LogInInputButtonProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK LogInButtonProc(HWND, UINT, WPARAM, LPARAM);
static BOOL LogInDataProcess(UINT uMsg, HWND hwndInputBorder);
extern LRESULT CALLBACK RegisterInfoProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//Edit窗口过程
static WNDPROC DefEditProc;
static LRESULT CALLBACK EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*
*   函数: LogInRegisterWnd(HINSTANCE)
*
*   目标: 注册登录界面的子窗口类。
*
*   输入：
*   hInst   实例句柄
*
*   返回：BOOL 判断程序是否正常运行
*/
static BOOL LogInRegisterWnd(HINSTANCE hInstance)
{
    HBRUSH hBrush;
    WNDCLASS wndclass;
    //登录界面输入子窗口类
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = LogInInputBoxProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szLogIn_InputBox;
    if (!RegisterClass(&wndclass)) return FALSE;

    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = LogInInputButtonProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szLogIn_InputButton;
    if (!RegisterClass(&wndclass)) return FALSE;

    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = LogInButtonProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szLogIn_Button;
    if (!RegisterClass(&wndclass)) return FALSE;

    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = RegisterInfoProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szRegisterInfo;
    if (!RegisterClass(&wndclass)) return FALSE;

    return TRUE;
}

/*
*   函数: LogInroc(HWND, UINT, WPARAM, LPARAM)
*
*   目标: 处理登录窗口主窗口的消息。
*
*   输入：
*   hWnd       窗口句柄
*   message    消息通知码
*   wParam     额外信息
*   lParam     额外信息
* 
*   返回：LRESULT 记录结果返回Windows
*/
LRESULT CALLBACK LogInProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static SIZE sizeClient;
    static HWND hwndChildInputBox;
    //背景相关
    static HDC hdcMem;
    static HBITMAP hBmpBk;
    static HBRUSH hBrBk;

    static RECT rcClient,rcBox,rcNote;
    static HFONT hFont;
    HDC hdc;
    static HFONT hFontTitle;
    static COLORREF crBk = RGB(178, 210, 255);
    switch (message)
    {
    case WM_CREATE:
        LogInRegisterWnd(hInst);
        hwndChildInputBox = CreateWindow(szLogIn_InputBox,
            TEXT("输入信息框架"),
            WS_CHILD | WS_CLIPCHILDREN,
            0, 0,
            0, 0,
            hwnd,
            (HMENU)LOG_INPUTBOX,
            hInst,
            NULL);
        hBrBk = CreateSolidBrush(crBk);

        //背景动画
        SetTimer(hwnd, TID_MAINBK, 500, NULL);
        SetTimer(hwnd, TID_MAINNOTE, 10, NULL);//10ms一次
        return 0;
    case WM_SIZE:
        sizeClient.cx = LOWORD(lParam);
        sizeClient.cy = HIWORD(lParam);
        if (sizeClient.cx == 0 || sizeClient.cy == 0) return 0;
        SetRect(&rcClient, 0, 0, sizeClient.cx, sizeClient.cy);
        SetRect(&rcBox, sizeClient.cx * 2 / 8, sizeClient.cy * 2 / 16,
            sizeClient.cx * 6 / 8, sizeClient.cy * 14 / 16);
        SetRect(&rcNote, 0, 0, sizeClient.cx * 3 / 16, sizeClient.cy * 2 / 20);

        //调整子窗口大小
        MoveWindow(hwndChildInputBox,
            rcBox.left,rcBox.top,
            rcBox.right - rcBox.left, rcBox.bottom - rcBox.top, TRUE);
        ShowWindow(hwndChildInputBox, SW_SHOWNORMAL);

        //背景
        hdc = GetDC(hwnd);
        hdcMem = CreateCompatibleDC(hdc);
        hBmpBk = CreateCompatibleBitmap(hdc, sizeClient.cx, sizeClient.cy);
        ReleaseDC(hwnd, hdc);
        SelectObject(hdcMem, hBmpBk);
        FillRect(hdcMem, &rcClient, hBrBk);

        //欢迎提示
        MoveRect(&rcNote, sizeClient.cx - (rcNote.right + rcNote.left) * 7 / 6, sizeClient.cy);
        FillRect(hdcMem, &rcNote, GetStockObject(BLACK_BRUSH));
        
        return 0;
    case WM_KEYDOWN:
        SetFocus(hwndUNEdit);
        return 0;
    case WM_LBUTTONDOWN:
        SetFocus(hwnd);
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMem);
        return 0;

    case WM_TIMER:
        switch (wParam)
        {
            //背景动画
        case TID_MAINBK:
        {
            //500ms一次，20次
            static int iNum;
            
            SetTimer(hwnd, TID_MAINBK_2, 10, NULL);//子动画
            iNum++;
            if (iNum == 20) KillTimer(hwnd, TID_MAINBK);
        }
            break;
        case TID_MAINBK_2:
        {
            //10ms一次，50次
            int iRandNum;
            static int timer = 1;
            static int ix, iy;
            static int r, g, b;
            static int dstR, dstG, dstB;
            static int detR, detG, detB;
            HBRUSH hbr;
            RECT rc1;
            if (timer == 50)
            {
                timer = 1;
                KillTimer(hwnd, TID_MAINBK_2);
                break;
            }

            if (timer == 1) {
                iRandNum = rand() % 5;
                
                ix = rand() % (sizeClient.cx - rcNote.right + rcNote.left + 25) - 25;
                iy = rand() % (sizeClient.cy - rcNote.bottom + rcNote.top + 25) - 25;

                r = GetRValue(crBk);
                g = GetGValue(crBk);
                b = GetBValue(crBk);
                dstR = rand() % 155 + 100;
                dstG = rand() % 155 + 100;
                dstB = rand() % 155 + 100;
                detR = r - dstR;
                detG = g - dstG;
                detB = b - dstB;
            }
            r -= detR / 50;
            g -= detG / 50;
            b -= detB / 50;
            hbr = CreateSolidBrush(RGB(r, g, b));
            rc1.left = ix;
            rc1.top = iy;
            rc1.right = ix + 50;
            rc1.bottom = iy + 50;

            FillRect(hdcMem, &rc1, hbr);
            InvalidateRect(hwnd, NULL, FALSE);
            UpdateWindow(hwnd);
            DeleteObject(hbr);
            timer++;
        }
            break;
            //欢迎动画
        case TID_MAINNOTE:
        {
            static int iNum=-20;//延迟开始
            static int detY = 0; 
            //共10次 * 2
            if (iNum == -20) {
                hFont = SetFont(hdcMem, 
                    (rcNote.bottom - rcNote.top) * 3 / 9, (rcNote.bottom - rcNote.top) * 3 / 9 * 2 / 5,
                    FONT_STYLE_DEFAULT);
            }
            iNum++;
            if (iNum < 0) return 0;
            else if (iNum == 0) detY = (rcNote.bottom - rcNote.top) / 10;
            else if (iNum <= 10) {//1-10
                MoveRect(&rcNote, 0, -detY);
                FillRect(hdcMem, &rcNote, GetStockObject(BLACK_BRUSH));
                SetTextColor(hdcMem, RGB(255, 255, 255));
                SetBkMode(hdcMem, TRANSPARENT);
                DrawText(hdcMem, TEXT("欢迎您，用户"), -1, &rcNote, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                InvalidateRect(hwnd, &rcNote, FALSE);
                UpdateWindow(hwnd);
            }
            else if (iNum < 201){//暂停绘制
            }
            else if(iNum <= 250){//201 - 250逐渐消失
                static r=0, g=0, b=0;
                static COLORREF crNote = RGB(0, 0, 0);
                static int dstR, dstG, dstB, detR, detG, detB;
                HBRUSH hbrNote;
                if (iNum == 201)
                {
                    dstR = GetRValue(crBk);
                    dstG = GetGValue(crBk);
                    dstB = GetBValue(crBk);
                    r = g = b = 0;
                    detR = dstR - r;
                    detG = dstG - g;
                    detB = dstB - b;
                }
                r += detR / 50;
                g += detG / 50;
                b += detB / 50;
                crNote = RGB(r, g, b);
                if (iNum == 250) crNote = crBk;
                hbrNote = CreateSolidBrush(crNote);
                FillRect(hdcMem, &rcNote, hbrNote);
                DeleteObject(hbrNote);
                InvalidateRect(hwnd, &rcNote, FALSE);
                UpdateWindow(hwnd);
                
            }else KillTimer(hwnd, TID_MAINNOTE);
            
        }
            break;
        }
        return 0;  
    case WM_U_REGISTER:case WM_U_SIGNIN:
        //数据处理
        LogInDataProcess(message, hwnd);
    return 0;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        DeleteDC(hdcMem);
        DeleteObject(hBmpBk);
        DeleteObject(hBrBk);
        DeleteObject(hFont);

        //退出位置为LOGIN才退出消息循环
        if(e_w == EXIT_IN_LOGIN)
            PostQuitMessage(0);
        return 0;
        
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

/*
*   函数: LogInInputBoxProc(HWND, UINT, WPARAM, LPARAM)
*
*   目标: 处理登录窗口下的输入区消息。
*
*   输入：
*   hWnd       窗口句柄
*   message    消息通知码
*   wParam     额外信息
*   lParam     额外信息
*
*   返回：LRESULT 记录结果返回Windows
*/
static LRESULT CALLBACK LogInInputBoxProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static SIZE sizeClient;
    
    static RECT rcClient;
    static HDC hdcMem;
    static HBITMAP hBmp;
    //Logo
    static HDC hdcMemLogo;
    static HBITMAP hBmpLogo;
    static BITMAP bmpLogo;
    //TITLE
    static HWND hwndTextTitle;
    static HFONT hfTitle;
    static BOOL fFirstTitle = TRUE;
    HDC hdc;
    HWND hwndChild;
    RECT rc;
    switch (uMsg)
    {
    case WM_CREATE:
        hwndTextTitle = CreateWindow(TEXT("static"),
            NULL,
            WS_CHILD | SS_OWNERDRAW,
            0, 0,
            0, 0,
            hwnd,
            (HMENU)LOG_INPUTBOX_USERNAME,
            hInst,
            NULL);
        
        hwndInputButton_U = CreateWindow(szLogIn_InputButton,
            TEXT("用户名输入"),
            WS_CHILD,
            0, 0, 0, 0,
            hwnd,
            (HMENU)LOG_INPUTBOX_USERNAME,
            hInst, NULL
        );

        hwndInputButton_P = CreateWindow(szLogIn_InputButton,
            TEXT("密码输入"),
            WS_CHILD,
            0, 0, 0, 0,
            hwnd,
            (HMENU)LOG_INPUTBOX_PASSWORD,
            hInst, NULL
        );
        hwndRegisterBtn = CreateWindow(szLogIn_Button,
            TEXT("注册按钮"),
            WS_CHILD,
            0, 0, 0, 0,
            hwnd,
            (HMENU)LOG_INPUTBOX_REGISTER,
            hInst, NULL
        );
        hwndSignInBtn = CreateWindow(szLogIn_Button,
            TEXT("登录按钮"),
            WS_CHILD,
            0, 0, 0, 0,
            hwnd,
            (HMENU)LOG_INPUTBOX_SIGNIN,
            hInst, NULL
        );
        return 0;
    case WM_SIZE:
        sizeClient.cx = LOWORD(lParam);
        sizeClient.cy = HIWORD(lParam);
        if (sizeClient.cx == 0 || sizeClient.cy == 0) return 0;
        SetRect(&rcClient, 0, 0, sizeClient.cx, sizeClient.cy);
        //MoveWindow
        MoveWindow(hwndTextTitle,
            sizeClient.cx * 2 / 7, sizeClient.cy * 1 / 16,
            sizeClient.cx * 3 / 7, sizeClient.cy * 5 / 16, TRUE);
        ShowWindow(hwndTextTitle, SW_SHOWNORMAL);

        MoveWindow(hwndInputButton_U, 
            sizeClient.cx * 1 / 8, sizeClient.cy * 6 / 16,
            sizeClient.cx * 6 / 8, sizeClient.cy * 3 / 16, TRUE);
        ShowWindow(hwndInputButton_U, SW_SHOWNORMAL);

        MoveWindow(hwndInputButton_P, 
            sizeClient.cx * 1 / 8, sizeClient.cy * 9 / 16,
            sizeClient.cx * 6 / 8, sizeClient.cy * 3 / 16, TRUE);
        ShowWindow(hwndInputButton_P, SW_SHOWNORMAL);

        MoveWindow(hwndRegisterBtn, 
            sizeClient.cx * 5 / 19, sizeClient.cy * 12 / 16,
            sizeClient.cx * 3 / 19 , sizeClient.cx * 2 / 19, TRUE);
        ShowWindow(hwndRegisterBtn, SW_SHOWNORMAL);

        MoveWindow(hwndSignInBtn, 
            sizeClient.cx * 11 / 19, sizeClient.cy * 12 / 16,
            sizeClient.cx * 3 / 19, sizeClient.cx * 2 / 19, TRUE);
        ShowWindow(hwndSignInBtn, SW_SHOWNORMAL);
        hdc = GetDC(hwnd);
        hdcMem = CreateCompatibleDC(hdc);
        hBmp = CreateCompatibleBitmap(hdc, sizeClient.cx, sizeClient.cy);
        SelectObject(hdcMem, hBmp);
        FillRect(hdcMem, &rcClient, (HBRUSH)GetStockObject(WHITE_BRUSH));

        //Logo
        hdcMemLogo = CreateCompatibleDC(hdc);
        if(sizeClient.cy * 2 / 16 < 200)
            hBmpLogo = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PAPERPLANE_S));
        else hBmpLogo = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PAPERPLANE));
        SelectObject(hdcMemLogo, hBmpLogo);
        GetObject(hBmpLogo, sizeof(BITMAP), &bmpLogo);
        SetStretchBltMode(hdcMem, HALFTONE);
        StretchBlt(hdcMem, sizeClient.cx / 14, sizeClient.cy * 3 / 19, sizeClient.cy * 2 / 16, sizeClient.cy * 2 / 16,
            hdcMemLogo, 0, 0, bmpLogo.bmWidth, bmpLogo.bmHeight, SRCCOPY);

        ReleaseDC(hwnd, hdc);
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMem);
        return 0;
    case WM_LBUTTONDOWN:
        SetFocus(hwnd);
        return 0;

        //静态文本
    case WM_CTLCOLORSTATIC:
        hdc = (HDC)wParam;
        hwndChild = (HWND)lParam;
        GetClientRect(hwndChild, &rc);
        //标题文字
        if (hwndChild == hwndTextTitle)
        {
            if (fFirstTitle)
            {
                hfTitle = CreateFont((rc.bottom - rc.top) * 3 / 7, (rc.bottom - rc.top) * 3 / 7 * 2 / 6,
                    0, 0, 600,
                    FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
                    DEFAULT_CHARSET, //使用默认字符集
                    OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS,
                    PROOF_QUALITY, //默认输出质量
                    DEFAULT_PITCH,
                    APP_DEFAULT_FONT //字体名
                );
            }
            SetBkMode(hdc, TRANSPARENT);
            SelectObject(hdc, hfTitle);
            SetTextColor(hdc, RGB(54,54,54));
            DrawText(hdc, TEXT("登录"), -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        }

        return (LRESULT)(HBRUSH)GetStockBrush(NULL_BRUSH);
        
    case WM_DESTROY:
        DeleteDC(hdcMem);
        DeleteDC(hdcMemLogo);
        DeleteObject(hBmp);
        DeleteObject(hBmpLogo);
        DeleteObject(hfTitle);
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


//  目标: 处理登录窗口下的提交按钮消息。
static LRESULT CALLBACK LogInButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    
    enum{
        idRegister=0,idSignIn=1
    }idButton;
    static HBRUSH hbrBk,hbrInner, hbrInner_selected, hbrInner_hover;
    static HPEN hpenBorder;
    static HFONT hFont;
    static SIZE sizeBtn;
    static RECT rcButton;
    static int iNumWnd = 0;//记录第几次Create
    static BOOL fFirstSize = TRUE;
    static HDC hdcMemClient[2];
    static HBITMAP hBmp[2];
    HDC hdcTemp;

    HMENU hMenu = (HMENU)GetWindowLongPtr(hwnd, GWL_ID);
    switch ((INT)hMenu)
    {
    case LOG_INPUTBOX_REGISTER:
        idButton = idRegister;
        break;
    case LOG_INPUTBOX_SIGNIN:
        idButton = idSignIn;
        break;
    default:
        idButton = idRegister;
    }
    switch (uMsg)
    {
    case WM_CREATE:
    
        iNumWnd++;
        if (iNumWnd == 1)//Create第一个
        {
            hpenBorder = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
            hbrBk = GetStockObject(WHITE_BRUSH);
            hbrInner = CreateSolidBrush(RGB(18, 126, 236));
            hbrInner_hover = CreateSolidBrush(RGB(1, 115, 230));
            hbrInner_selected = CreateSolidBrush(RGB(0, 104, 210));
        }
        break;
    
    case WM_SIZE:
        sizeBtn.cx = LOWORD(lParam);
        sizeBtn.cy = HIWORD(lParam);
        if (sizeBtn.cx == 0 || sizeBtn.cy == 0) return 0;
        SetRect(&rcButton, 0, 0, sizeBtn.cx, sizeBtn.cy);
        if (fFirstSize)
        {
            fFirstSize = FALSE;
            hFont = CreateFont(sizeBtn.cy * 1 / 2, sizeBtn.cy * 1 / 2 * 3 / 8, 0, 0, 400,
                FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
                DEFAULT_CHARSET, //使用默认字符集
                OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS,
                PROOF_QUALITY, //默认输出质量
                DEFAULT_PITCH,
                APP_DEFAULT_FONT //字体名
            );
        }

        //内存DC操作，初始化
        hdcTemp = GetDC(hwnd);
        hdcMemClient[idButton] = CreateCompatibleDC(hdcTemp);
        hBmp[idButton] = CreateCompatibleBitmap(hdcTemp, sizeBtn.cx, sizeBtn.cy);
        ReleaseDC(hwnd, hdcTemp);
        SelectObject(hdcMemClient[idButton], hBmp[idButton]);
        FillRect(hdcMemClient[idButton], &rcButton, hbrInner);
        SelectObject(hdcMemClient[idButton], hbrInner);
        SelectObject(hdcMemClient[idButton], hpenBorder);
        //Rectangle(hdcMem[idButton], 0, 0, sizeBtn.cx, sizeBtn.cy);
        SelectObject(hdcMemClient[idButton], hFont);
        SetBkMode(hdcMemClient[idButton], TRANSPARENT);
        SetTextColor(hdcMemClient[idButton], RGB(255, 255, 255));
        switch (idButton)
        {
        case idRegister://注册按钮
            DrawText(hdcMemClient[idButton], TEXT("注册"), -1, &rcButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            break;
        case idSignIn://登录按钮
            DrawText(hdcMemClient[idButton], TEXT("登录"), -1, &rcButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            break;
        }
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemClient[idButton]);
        return 0;
    case WM_LBUTTONDOWN:
        FillRect(hdcMemClient[idButton], &rcButton, hbrInner_selected);
        switch (idButton)
        {
        case idRegister://注册按钮
            DrawText(hdcMemClient[idButton], TEXT("注册"), -1, &rcButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            break;
        case idSignIn://登录按钮
            DrawText(hdcMemClient[idButton], TEXT("登录"), -1, &rcButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            break;
        }
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        return 0;
    case WM_LBUTTONUP:
        FillRect(hdcMemClient[idButton], &rcButton, hbrInner_hover);
        switch (idButton)
        {
        case idRegister://注册按钮
            DrawText(hdcMemClient[idButton], TEXT("注册"), -1, &rcButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            break;
        case idSignIn://登录按钮
            DrawText(hdcMemClient[idButton], TEXT("登录"), -1, &rcButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            break;
        }
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        switch (idButton)
        {
        case idRegister:
            SendMessage(hwndLogIn, WM_U_REGISTER, 0, 0);
            break;
        case idSignIn:
            SendMessage(hwndLogIn, WM_U_SIGNIN, 0, 0);
            break;
        }

        return 0;
    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON) return 0;
        {
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(tme);
            tme.dwFlags = TME_LEAVE;
            tme.dwHoverTime = 30;
            tme.hwndTrack = hwnd;
            TrackMouseEvent(&tme);
            FillRect(hdcMemClient[idButton], &rcButton, hbrInner_hover);
            switch (idButton)
            {
            case idRegister://注册按钮
                DrawText(hdcMemClient[idButton], TEXT("注册"), -1, &rcButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                break;
            case idSignIn://登录按钮
                DrawText(hdcMemClient[idButton], TEXT("登录"), -1, &rcButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                break;
            }
            InvalidateRect(hwnd, NULL, FALSE);
            UpdateWindow(hwnd);
        }
        return 0;

    case WM_MOUSELEAVE:
        FillRect(hdcMemClient[idButton], &rcButton, hbrInner);
        switch (idButton)
        {
        case idRegister://注册按钮
            DrawText(hdcMemClient[idButton], TEXT("注册"), -1, &rcButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            break;
        case idSignIn://登录按钮
            DrawText(hdcMemClient[idButton], TEXT("登录"), -1, &rcButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            break;
        }
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        return 0;
    case WM_DESTROY:
        DeleteObject(hbrInner);
        DeleteObject(hpenBorder);
        DeleteObject(hFont);
        DeleteDC(hdcMemClient[idButton]);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//  目标: 处理登录窗口下的Edit控件消息。
static LRESULT CALLBACK EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            SendMessage(hwndSignInBtn, WM_LBUTTONUP, 0, 0);
            break;
        case VK_TAB:
            if (hwnd == hwndUNEdit) SetFocus(hwndPWEdit);
            else SetFocus(hwndUNEdit);
            break;
        }
        
    }
    return CallWindowProc(DefEditProc, hwnd, uMsg, wParam, lParam);
}

//  目标: 处理登录窗口下的两个输入框消息。
static LRESULT CALLBACK LogInInputButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static SIZE sizeClient;
    static HWND hwndTitle[2];
    static HWND hwndInput[2];
    static HWND hwndUsername, hwndPassword;
    static enum {
        ID_USERNAME = 0, ID_PSWD = 1
    }idInput;//标识ID用户名或是密码

    static BITMAP bmpInputBorder;
    static HBITMAP hbmpInputBorder, hbmpInputBorder_Selected;
    static HDC hdcMemClient[2];
    static HBITMAP hbmpClient[2];
    static HFONT hFontInput_title,
        hFontInput,
        hFontWarnning;
    static HPEN hPenLine;
    static UINT_PTR timerId;
    static BOOL fWarnning[2] = { FALSE,FALSE };
    static RECT rcWarnning;
    static SIZE sizeInputBorder;
    static RECT rcInputBorder;
    static BOOL fFirstWindow = TRUE;
    //临时变量
    int i = 0;
    HWND hwndChild;
    HDC hdc;
    HDC hdcMem;
    RECT rcTemp;
    TCHAR szBuf[128] = {TEXT('\0')};
    UINT msgChild, idChild;
    POINT ptCursor;
    TCHAR szInputUserName[20] = {TEXT('\0')}, 
        szInputPassword[20] = {TEXT('\0')};
    HMENU hMenu = (HMENU)GetWindowLongPtr(hwnd, GWL_ID);
    switch ((INT)hMenu)
    {
    case LOG_INPUTBOX_USERNAME:
        idInput = ID_USERNAME;
        break;
    case LOG_INPUTBOX_PASSWORD:
        idInput = ID_PSWD;
        break;
    default:
        idInput = ID_USERNAME;
        
    }
    switch (uMsg)
    {
    case WM_CREATE:
        switch ((UINT)((LPCREATESTRUCT)lParam)->hMenu)
        {
            //标识是用户名还是密码
        case LOG_INPUTBOX_USERNAME:
            idInput = ID_USERNAME;
            hwndUsername = hwnd;
            break;
        case LOG_INPUTBOX_PASSWORD:
            idInput = ID_PSWD;
            hwndPassword = hwnd;
            break;
        }
        hwndTitle[idInput] = CreateWindow(TEXT("static"), NULL,
            WS_CHILD | SS_OWNERDRAW,
            0, 0, 0, 0,
            hwnd,
            (HMENU)(LOG_INPUTBOX_INPUTTITLE),
            hInst, NULL);
        if (idInput == ID_USERNAME)
            hwndUNEdit = hwndInput[idInput] = CreateWindow(TEXT("edit"), NULL,
                WS_CHILD | ES_LEFT,
                0, 0, 0, 0,
                hwnd,
                (HMENU)(LOG_INPUTBOX_INPUTCONTENT),
                hInst, NULL);
        else if (idInput == ID_PSWD)
            hwndPWEdit = hwndInput[idInput] = CreateWindow(TEXT("edit"), NULL,
                WS_CHILD | ES_PASSWORD | ES_LEFT,
                0, 0, 0, 0,
                hwnd,
                (HMENU)(LOG_INPUTBOX_INPUTCONTENT),
                hInst, NULL);
        DefEditProc = (WNDPROC)SetWindowLongPtr(hwndInput[idInput], GWL_WNDPROC, (LONG)EditProc);
        return 0;
        
    case WM_SIZE:
        sizeClient.cx = LOWORD(lParam);
        sizeClient.cy = HIWORD(lParam);
        if (sizeClient.cx == 0 || sizeClient.cy == 0) return 0;
        if (fFirstWindow)
        {
            fFirstWindow = FALSE;
            //加载输入框位图
            if (sizeClient.cx >= 800)
                hbmpInputBorder = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_INPUTBORDER_900_135));
            else if (sizeClient.cx > 400)
                hbmpInputBorder = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_INPUTBORDER_600_90));
            else if (sizeClient.cx > 300)
                hbmpInputBorder = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_INPUTBORDER_400_60));
            else if (sizeClient.cx > 200)
                hbmpInputBorder = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_INPUTBORDER_300_45));
            else
                hbmpInputBorder = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_INPUTBORDER_200_30));
            if (sizeClient.cx >= 800)
                hbmpInputBorder_Selected = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_INPUTBORDER_900_135_SELECTED));
            else if (sizeClient.cx > 400)
                hbmpInputBorder_Selected = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_INPUTBORDER_600_90_SELECTED));
            else if (sizeClient.cx > 300)
                hbmpInputBorder_Selected = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_INPUTBORDER_400_60_SELECTED));
            else if (sizeClient.cx > 200)
                hbmpInputBorder_Selected = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_INPUTBORDER_300_45_SELECTED));
            else
                hbmpInputBorder_Selected = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_INPUTBORDER_200_30_SELECTED));
            GetObject(hbmpInputBorder, sizeof(BITMAP), &bmpInputBorder);

            //设置InputBorder大小
            sizeInputBorder.cx = sizeClient.cx;
            sizeInputBorder.cy = sizeClient.cx * 3 / 20;
            SetRect(&rcInputBorder, 0, 0, sizeInputBorder.cx, sizeInputBorder.cy);
            
            //设置警告矩形
            SetRect(&rcWarnning,
                0, sizeInputBorder.cy, 
                sizeClient.cx, sizeInputBorder.cy * 17 / 12);
            hFontInput_title = CreateFont(
                sizeInputBorder.cy * 3 / 7/*高度*/, (sizeInputBorder.cy * 3 / 7) * 2 / 5/*宽度*/, 0, 0, 400,
                FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
                DEFAULT_CHARSET, //使用默认字符集
                OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
                DEFAULT_QUALITY, //默认输出质量
                FF_DONTCARE, //不指定字体族
                APP_DEFAULT_FONT //字体名
            );
            hFontInput = CreateFont(
                sizeInputBorder.cy * 3 / 5/*高度*/, (sizeInputBorder.cy * 3 / 5) * 3 / 8/*宽度*/, 0, 0, 300,
                FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
                DEFAULT_CHARSET, //使用默认字符集
                OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
                DEFAULT_QUALITY, //默认输出质量
                FF_DONTCARE, //不指定字体族
                APP_DEFAULT_FONT //字体名
            );
            hFontWarnning = CreateFont(
                (rcWarnning.bottom - rcWarnning.top)/*高度*/, (rcWarnning.bottom-rcWarnning.top) * 3 / 8/*宽度*/, 0, 0, 300,
                FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
                DEFAULT_CHARSET, //使用默认字符集
                OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
                DEFAULT_QUALITY, //默认输出质量
                FF_DONTCARE, //不指定字体族
                APP_DEFAULT_FONT //字体名
            );
            hPenLine = CreatePen(PS_SOLID, 2, RGB(128, 128, 128));
        }

        
        hdc = GetDC(hwnd);
        //设置客户区内存DC
        hdcMemClient[idInput] = CreateCompatibleDC(hdc);
        hbmpClient[idInput] = CreateCompatibleBitmap(hdc, sizeClient.cx, sizeClient.cy);
        SelectObject(hdcMemClient[idInput], hbmpClient[idInput]);
        SetRect(&rcTemp, 0, 0, sizeClient.cx, sizeClient.cy);
        FillRect(hdcMemClient[idInput], &rcTemp, GetStockBrush(WHITE_BRUSH));
        

        //创建内存DC临时存储InputBorder
        hdcMem = CreateCompatibleDC(hdc);
        SetStretchBltMode(hdcMemClient[idInput], HALFTONE);
        SelectObject(hdcMem, hbmpInputBorder);
        StretchBlt(hdcMemClient[idInput], 0, 0, sizeInputBorder.cx, sizeInputBorder.cy, hdcMem,
            0, 0, bmpInputBorder.bmWidth, bmpInputBorder.bmHeight, SRCCOPY);

        DeleteDC(hdcMem);
        
        ReleaseDC(hwnd, hdc);
        /*窗口移动，调整大小*/
        /*TITLE*/
        MoveWindow(hwndTitle[idInput], sizeInputBorder.cy / 3, 0,
            sizeInputBorder.cy * 3 / 2, sizeInputBorder.cy, TRUE);
        ShowWindow(hwndTitle[idInput], SW_SHOWNORMAL);
        /*编辑控件*/
        MoveWindow(hwndInput[idInput], sizeInputBorder.cy * 2, sizeInputBorder.cy * 1 / 5,
            sizeInputBorder.cx - sizeInputBorder.cy * 2 - sizeInputBorder.cy / 3, sizeInputBorder.cy * 3 / 5, TRUE);
        ShowWindow(hwndInput[idInput], SW_SHOWNORMAL);

        //设置字体
        SendMessage(hwndInput[idInput], WM_SETFONT, (WPARAM)hFontInput, (LPARAM)NULL);

        //设置ID和密码规范
        SendMessage(hwndInput[idInput], EM_LIMITTEXT, LIMIT_USERNAME, (LPARAM)NULL);
        
        return 0;

    //TITLE控件文字样式
    case WM_CTLCOLORSTATIC:
        hdc = (HDC)wParam;
        hwndChild = (HWND)lParam;
        GetClientRect(hwndChild, &rcTemp);
        SelectObject(hdc, hPenLine);
        MoveToEx(hdc, rcTemp.right - 5, (rcTemp.bottom - sizeClient.cx / 14) / 2, NULL);
        LineTo(hdc, rcTemp.right - 5, (rcTemp.bottom + sizeClient.cx / 14) / 2);
        switch (idInput)
        {
        case ID_USERNAME:
            wsprintf(szBuf, TEXT("用户名"));
            break;

        case ID_PSWD:
            wsprintf(szBuf, TEXT("密码"));
            break;
        }
        SelectObject(hdc, hFontInput_title);
        SetBkMode(hdc, TRANSPARENT);
        DrawText(hdc, szBuf, -1, &rcTemp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        return (LRESULT)GetStockBrush(NULL_BRUSH);

        //编辑控件样式
    case WM_CTLCOLOREDIT:
        hdc = (HDC)wParam;
        hwndChild = (HWND)lParam;

        return (LRESULT)GetStockObject(WHITE_BRUSH);//背景画刷

    //编辑控件命令
    case WM_COMMAND:
        idChild = LOWORD(wParam);
        msgChild = HIWORD(wParam);
        hwndChild = (HWND)lParam;
        switch (idChild)
        {
        case LOG_INPUTBOX_INPUTCONTENT://输入框消息

            if (msgChild == EN_MAXTEXT && fWarnning[idInput] == FALSE) {//超出范围，在内存DC绘图
                fWarnning[idInput] = TRUE;
                SetTextColor(hdcMemClient[idInput], RGB(255, 1, 2));
                SelectObject(hdcMemClient[idInput], hFontWarnning);
                FillRect(hdcMemClient[idInput], &rcWarnning, GetStockBrush(WHITE_BRUSH));
                DrawText(hdcMemClient[idInput], TEXT("字符过长     "), -1, &rcWarnning,
                    DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
                InvalidateRect(hwnd, &rcWarnning, TRUE);
                UpdateWindow(hwnd);
                SetTimer(hwnd, TID_DESWARNNING, 1000, NULL);
            }

            if (msgChild == EN_SETFOCUS)//有输入焦点
            {
                hdc = GetDC(hwnd);
                hdcMem = CreateCompatibleDC(hdc);
                ReleaseDC(hwnd, hdc);
                SelectObject(hdcMem, hbmpInputBorder_Selected);
                StretchBlt(hdcMemClient[idInput], 0, 0, sizeInputBorder.cx, sizeInputBorder.cy, hdcMem,
                    0, 0, bmpInputBorder.bmWidth, bmpInputBorder.bmHeight, SRCCOPY);
                DeleteDC(hdcMem);
                InvalidateRect(hwnd, NULL, TRUE);
                UpdateWindow(hwnd);
                
            }else if (msgChild == EN_KILLFOCUS)//失去输入焦点
            {
                hdc = GetDC(hwnd);
                hdcMem = CreateCompatibleDC(hdc);
                ReleaseDC(hwnd, hdc);
                SelectObject(hdcMem, hbmpInputBorder);
                StretchBlt(hdcMemClient[idInput], 0, 0, sizeInputBorder.cx, sizeInputBorder.cy, hdcMem,
                    0, 0, bmpInputBorder.bmWidth, bmpInputBorder.bmHeight, SRCCOPY);
                DeleteDC(hdcMem);
                InvalidateRect(hwnd, NULL, TRUE);
                UpdateWindow(hwnd); 
            }
            
            if (msgChild == EN_CHANGE && fWarnning[idInput] == FALSE)
            {
                GetWindowText(hwndChild, szBuf, 128);
                if (DataValidCharcterUNorPW(szBuf))
                {
                    break;
                }
                else  fWarnning[idInput] = TRUE;
                SetTextColor(hdcMemClient[idInput], RGB(255, 1, 2));
                SelectObject(hdcMemClient[idInput], hFontWarnning);
                FillRect(hdcMemClient[idInput], &rcWarnning, GetStockObject(WHITE_BRUSH));
                DrawText(hdcMemClient[idInput], TEXT("请输入字母或数字   "), -1, &rcWarnning, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
                InvalidateRect(hwnd, &rcWarnning, TRUE);
                UpdateWindow(hwnd);
                SetTimer(hwnd, TID_DESWARNNING, 1000, NULL);
            }

            break;
        }
        return 0;
    case WM_TIMER:
        switch (wParam)//ID
        {
        case TID_DESWARNNING:

            FillRect(hdcMemClient[idInput], &rcWarnning, GetStockBrush(WHITE_BRUSH));
            fWarnning[idInput] = FALSE;
            KillTimer(hwnd, wParam);
            InvalidateRect(hwnd, &rcWarnning, TRUE);
            UpdateWindow(hwnd);
            break;
        }
        return 0;
    case WM_LBUTTONDOWN:
        ptCursor.x = GET_X_LPARAM(lParam);
        ptCursor.y = GET_Y_LPARAM(lParam);
        if (PT_ON_RECT(ptCursor, rcInputBorder))
        {
            SetFocus(hwndInput[idInput]);
        }
        return 0;
    case WM_PAINT:
    {
        OnPaint(hwnd, hdcMemClient[idInput]);
        return 0;
    }
    case WM_DESTROY:
        DeleteDC(hdcMemClient[idInput]);
        DeleteObject(hbmpClient[idInput]);
        DeleteObject(hFontInput);
        DeleteObject(hFontInput_title);
        DeleteObject(hFontWarnning);
        DeleteObject(hbmpInputBorder);
        DeleteObject(hPenLine);
        break;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*
*   函数: LogInDataProcess(UINT uMsg, HWND hwnd)
*
*   目标: 处理登录数据的读取和存储。
*
*   输入：
*   hwnd       窗口句柄
*   uMsg       消息通知码
*           WM_U_SIGNIN
*           WM_U_REGISTER
* 
*   返回：BOOL判断登录注册操作是否成功，若成功进入MainWindow
*
*/
static BOOL LogInDataProcess(UINT uMsg, HWND hwnd)
{
    USERACCOUNT ua = { TEXT('\0'),TEXT('\0'), 0, 0};
    HWND hwndUsernameInput = NULL, hwndPasswordInput = NULL;    //记录编辑框控件句柄
    BOOL fGotoMain = FALSE;
    UA_SEARCHRET ua_sr;
    int i = 0;
    TCHAR szBuf[128] = TEXT("\0");
    TCHAR szBuf1[128] = TEXT("\0");
    //获取编辑框句柄
    hwndUsernameInput = GetDlgItem(hwndInputButton_U, LOG_INPUTBOX_INPUTCONTENT);
    hwndPasswordInput = GetDlgItem(hwndInputButton_P, LOG_INPUTBOX_INPUTCONTENT);
    //获取输入框内容，保存到ua
    GetWindowText(hwndUsernameInput, ua.szUsername, 20);
    GetWindowText(hwndPasswordInput, ua.szPassword, 20);
    if (ua.szUsername[0] == TEXT('\0') || ua.szPassword[0] == TEXT('\0'))
    {
        if (IDOK == MessageBox(hwndLogIn, TEXT("     请输入内容！     "), TEXT("提示"), MB_OK))
            return FALSE;
    }
    //检查合法性
    if (!DataUAValid(ua))
    {
        DataGetValidUA_UOPStd(szBuf);
        if (IDOK == MessageBox(hwndLogIn, szBuf, TEXT("请输入合法的账户ID和密码"), MB_OK))
        {
            return FALSE;
        }
    }
    //文件中搜索，若存在自动存入内存(已设置所有值,包含用户类型)
    ua_sr = DataAccountSearchFileToMem(ua);
    switch (uMsg)
    {
        
    case WM_U_REGISTER:
        //在文件中搜索是否存在，考虑了文件不存在的情况
        
        if (ua_sr == UA_FOUND || ua_sr == UA_FOUND_ADMIN || ua_sr == UA_FOUND_ERR_PW)
        {
            if (IDOK == MessageBox(hwndLogIn, TEXT("已存在该用户，无需注册"), TEXT("提示"), MB_OK))
                return FALSE;
        }
        //若无搜索结果执行注册
        StringCchPrintf(szBuf, 128, TEXT("你填入的信息为:\n用户：%-40s\n密码：%-40s\n是否执行注册？"), ua.szUsername, ua.szPassword);
        if (IDOK == MessageBox(hwndLogIn, szBuf, TEXT("注册信息"), MB_OKCANCEL))
        {
            DataUserAccount(&ua, UA_CHANGE);//存入内存
            if (DataUAFile(UA_ADDTO_FILE, &ua) == TRUE)
            {
                if (IDOK == MessageBox(hwndLogIn, TEXT("注册成功，是否登录？"), TEXT("注册成功"), MB_OKCANCEL))
                {
                    //先完善个人信息
                    e_w = EXIT_IN_REGISTER;
                    DestroyWindow(hwndLogIn);
                    hwndRegister = CreateWindow(szRegisterInfo,
                        TEXT("完善个人信息"),
                        WS_OVERLAPPED | WS_CLIPCHILDREN,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        cxRegister, cyRegister,
                        NULL, NULL, hInst, NULL);
                    ShowWindow(hwndRegister, SW_SHOWNORMAL);
                    UpdateWindow(hwndRegister);
                        
                }
            }
        }

        break;
    case WM_U_SIGNIN:
        if (ua_sr == UA_FOUND_ERR_PW)
        {
            if (IDOK == MessageBox(hwndLogIn, TEXT("用户密码错误！"), TEXT("提示"), MB_OK))
                return FALSE;
        }
        else if (ua_sr == UA_NOTFOUND || ua_sr == UA_FOUNDFILEERR)
        {
            if (IDOK == MessageBox(hwndLogIn, TEXT("不存在该用户！"), TEXT("提示"), MB_OK))
                return FALSE;
        }
        else if (ua_sr == UA_FOUND || ua_sr == UA_FOUND_ADMIN)
        {

            switch (ua_sr)
            {
            case UA_FOUND:
                DataUserAccount(&ua, UA_GET);
                //先检测个人信息是否完善
                if (FALSE == DataAccount_GetSelfInfoFromUA(szBuf1, UA_GETINFO_NICKNAME, &ua))
                {
                    if (IDOK == MessageBox(hwndLogIn, TEXT("欢迎您，用户，请先完善个人资料"), TEXT("登录成功"), MB_OK))
                    {
                        e_w = EXIT_IN_REGISTER;
                        DestroyWindow(hwndLogIn);
                        hwndRegister = CreateWindow(szRegisterInfo,
                            TEXT("完善个人信息"),
                            WS_OVERLAPPED | WS_CLIPCHILDREN | WS_SYSMENU,
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            cxRegister, cyRegister,
                            NULL, NULL, hInst, NULL);
                        ShowWindow(hwndRegister, SW_SHOWNORMAL);
                        UpdateWindow(hwndRegister);
                    }
                }
                else
                {
                    StringCchPrintf(szBuf, 128, TEXT("欢迎您！用户 %-20s"), szBuf1);
                    if (IDOK == MessageBox(hwndLogIn, szBuf, TEXT("登录成功"), MB_OK))
                    fGotoMain = TRUE;
                }
                break;

            case UA_FOUND_ADMIN:
                DataUserAccount(&ua, UA_GET);
                DataAccount_GetSelfInfoFromUA(szBuf1, UA_GETINFO_NICKNAME, &ua);
                StringCchPrintf(szBuf, 128, TEXT("欢迎您！管理员 %-20s"), szBuf1);
                if (IDOK == MessageBox(hwndLogIn, szBuf, TEXT("登录成功"), MB_OK))
                    fGotoMain = TRUE;
                break;

            }
            
        }

        break;
    }
    //创建主要窗口
    if (fGotoMain)
    {
        //退出程序方式改变，调用Destroy不会退出消息循环
        e_w = EXIT_IN_MAINWND;

        DestroyWindow(hwndLogIn);
        //创建MainWindow
        
        hwndMain = CreateWindow(szAppName,
            TEXT("飞机订票系统"),
            WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
            CW_USEDEFAULT, CW_USEDEFAULT,
            cxMain, cyMain,
            NULL, NULL, hInst, NULL);
        ShowWindow(hwndMain, SW_SHOWNORMAL);
        UpdateWindow(hwndMain);//会刷新窗口包括子窗口
        
    }
    return TRUE;
}