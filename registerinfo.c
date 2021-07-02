/*
*       registerinfo.c
*       �������������û���Ϣ������غ���
*/
#include "login.h"
extern HWND hwndMain;
//���ƶ���
static TCHAR szInputName[] = TEXT("ע�ᴰ�������");
static TCHAR szButtonName[] = TEXT("ע�ᴰ���ύ");
//ע�ᴰ����
static BOOL RegisterInfoWndClass(HINSTANCE hInstance);
//���ڹ���
LRESULT CALLBACK RegisterInfoProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK RegisterInputProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK RegisterButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static WNDPROC DefEditProc;
//���ھ��
HWND hwndRegister;
static HWND hwndInput[3];
//edit�ؼ�
static HWND hwndEdit[3];
static HWND hwndButton;
/*
*   ����: RegisterInfoWndClass(HINSTANCE hInstance)
*
*   Ŀ��: ע��RegisterInfo�Ӵ�����
*
*   ���룺hInstance        ʵ�����
*
*   ���أ�BOOL             �ж�ע���Ƿ�ɹ�
*/
static BOOL RegisterInfoWndClass(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    HBRUSH hBrush;

    //����򴰿���
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = RegisterInputProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szInputName;
    if (!RegisterClass(&wndclass)) return FALSE;
    //��ť
    wndclass.lpfnWndProc = RegisterButtonProc;
    wndclass.lpszClassName = szButtonName;
    if (!RegisterClass(&wndclass)) return FALSE;

    return TRUE;
}
//������
LRESULT CALLBACK RegisterInfoProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //��������
    static HBRUSH hbrBk;
    static HFONT hfTitle;
    static RECT rcTitle;
    static SIZE sizeInput;

    switch (uMsg)
    {
    case WM_CREATE:
        hwndRegister = hwnd;//����þ��
        RegisterInfoWndClass(hInst);
        return 0;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            hbrBk = (HBRUSH)GetStockObject(WHITE_BRUSH);
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk, &hbmpBk);
            sizeInput.cx = cxClient / 2;
            sizeInput.cy = cyClient * 1 / 11;
            //TITLE
            
            SetRect(&rcTitle, 
                0, cyClient / 10, cxClient, cyClient * 1 / 5);
            hfTitle = SetFont(hdcMemBk,
                rcTitle.bottom - rcTitle.top, (rcTitle.bottom - rcTitle.top) * 3 / 8, 
                FONT_STYLE_DEFAULT + 500);
            DrawText(hdcMemBk, TEXT("���Ƹ�����Ϣ"), -1, &rcTitle,
                DT_SINGLELINE | DT_VCENTER | DT_CENTER);

            hwndInput[0] = CreateWindow(szInputName,
                TEXT("��������"),
                WS_CHILD | WS_VISIBLE,
                (cxClient - sizeInput.cx) / 2, cyClient / 3,
                sizeInput.cx, sizeInput.cy,
                hwnd,
                (HMENU)REGISTER_INPUT_NAME, hInst, NULL);
            hwndInput[1] = CreateWindow(szInputName,
                TEXT("���֤����"),//�Զ�ʶ��������Ϣ
                WS_CHILD | WS_VISIBLE,
                (cxClient - sizeInput.cx) / 2, cyClient / 3 + sizeInput.cy * 3 / 2,
                sizeInput.cx, sizeInput.cy,
                hwnd,
                (HMENU)REGISTER_INPUT_IDCARD, hInst, NULL);
            hwndInput[2] = CreateWindow(szInputName,
                TEXT("�ǳ�����"),
                WS_CHILD | WS_VISIBLE,
                (cxClient - sizeInput.cx) / 2, cyClient / 3 + sizeInput.cy * 3,
                sizeInput.cx, sizeInput.cy,
                hwnd,
                (HMENU)REGISTER_INPUT_NICKNAME, hInst, NULL);
            hwndButton = CreateWindow(szButtonName,
                TEXT("�ύ��ť"),
                WS_CHILD | WS_VISIBLE,
                (cxClient - sizeInput.cx) / 2 + sizeInput.cx - sizeInput.cy * 19 / 10,
                cyClient / 3 + sizeInput.cy * 5,
                sizeInput.cy * 19 / 10, 
                sizeInput.cy * 4 / 5,
                hwnd,
                (HMENU)REGISTER_INPUT_BUTTON, hInst, NULL);
            fInited = TRUE;
        }
        return 0;
    case WM_LBUTTONDOWN:
        SetFocus(hwnd);
        return 0;
    case WM_KEYDOWN:
        SetFocus(hwndInput[0]);
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_CLOSE:
        if (IDOK ==
            MessageBox(hwnd, TEXT("ȷ�������Ƹ�����Ϣ���������ƣ����޷����ж�Ʊ"),
                TEXT("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONQUESTION))
        {
            e_w = EXIT_IN_REGISTER;
            DestroyWindow(hwnd);
        }
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfTitle);
        if(e_w == EXIT_IN_REGISTER)
            PostQuitMessage(0);//ֱ���˳�
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//�������Ϣ
LRESULT CALLBACK RegisterInputProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited[3] = { FALSE };
    static int cxClient, cyClient;
    static HDC hdcMemBk[3];
    static HBITMAP hbmpBk[3];
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //��������
    static HBRUSH hbrBk;
  
    static HWND hwndBorder[3];

    int id;
    static BOOL fFirst = TRUE;
    static HPEN hpenBorder;
    //title
    static RECT rcTitle;
    static TCHAR szTitle[3][10];
    //edit
    static HFONT hfEdit;
    static COLORREF crInput, crInput_click;
    int ia, ib;
    UINT msgChild;
    HMENU hMenu = (HMENU)GetWindowLongPtr(hwnd, GWL_ID);
    switch ((INT)hMenu)
    {
    case REGISTER_INPUT_NAME:
        id = 0;
        break;
    case REGISTER_INPUT_IDCARD:
        id = 1;
        break;
    case REGISTER_INPUT_NICKNAME:
        id = 2;
        break;
    default:
        id = 0;
    }

    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fFirst) {
            SetRect(&rcTitle, cxClient * 1 / 20, 
                cyClient * 5 / 20,
                cxClient * 6 / 20, 
                cyClient * 13 / 20);
            hfEdit = SetFont(hdcMemBk[id], 
                rcTitle.bottom-rcTitle.top,
                (rcTitle.bottom - rcTitle.top) * 3 / 8, YAHEI_FONT + 500);
            hpenBorder = CreatePen(PS_SOLID, cyClient / 5, RGB(62, 62, 62));
            crInput = RGB(80, 80, 80);
            crInput_click = RGB(20, 20, 20);
            fFirst = FALSE;
        }
        if (fInited[id] == FALSE)
        {
            hbrBk = GetStockObject(WHITE_BRUSH);
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk[id], &hbmpBk[id]);
            //border
            hwndBorder[id] = hwnd;
            SelectObject(hdcMemBk[id], (HBRUSH)GetStockObject(NULL_BRUSH));
            SelectObject(hdcMemBk[id], hpenBorder);
            Rectangle(hdcMemBk[id], 0, 0, cxClient, cyClient);
            hwndEdit[id] = CreateWindow(TEXT("edit"),
                NULL,
                WS_CHILD | WS_VISIBLE,
                rcTitle.right + (rcTitle.right - rcTitle.left) / 8, rcTitle.top,
                cxClient * 6 / 10, rcTitle.bottom - rcTitle.top, hwnd,
                (HMENU)id, hInst, NULL);
            DefEditProc = (WNDPROC)SetWindowLongPtr(hwndEdit[id], GWL_WNDPROC, (LONG)EditProc);
            SendMessage(hwndEdit[id], WM_SETFONT, (WPARAM)hfEdit, 0);
            switch (id)
            {
            case 0:

                StringCchPrintf(szTitle[0], 10, TEXT("��ʵ����"));
                SendMessage(hwndEdit[id], EM_LIMITTEXT, (WPARAM)LIMIT_NAME, 0);
                break;
            case 1:
                StringCchPrintf(szTitle[1], 10, TEXT("���֤����"));
                SendMessage(hwndEdit[id], EM_LIMITTEXT, (WPARAM)18, 0);
                SetWindowLongPtr(hwndEdit[1], GWL_STYLE, WS_CHILD | WS_VISIBLE | ES_NUMBER);
                break;
            case 2:
                StringCchPrintf(szTitle[2], 10, TEXT("��  ��"));
                SendMessage(hwndEdit[id], EM_LIMITTEXT, (WPARAM)LIMIT_NICKNAME, 0);
                break;

            }

            SelectObject(hdcMemBk[id], hfEdit);
            SetTextColor(hdcMemBk[id], RGB(80, 80, 80));
            SetBkMode(hdcMemBk[id], TRANSPARENT);
            DrawText(hdcMemBk[id], szTitle[id], -1, &rcTitle, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
            fInited[id] = TRUE;
        }
        return 0;
    case WM_SETFOCUS:
        SetFocus(hwndEdit[id]);
        return 0;
    case WM_LBUTTONUP:
        SetFocus(hwndEdit[id]);
        //�������
        switch (id) {
        case 0:
            ia = 1;
            ib = 2;
            break;
        case 1:
            ia = 0;
            ib = 2;
            break;
        case 2:
            ia = 0;
            ib = 1;
            break;
        }
        //�����ǰ��
        FillRect(hdcMemBk[ia], &rcTitle, hbrBk);
        SetTextColor(hdcMemBk[ia], crInput);
        DrawText(hdcMemBk[ia], szTitle[ia], -1, &rcTitle, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
        InvalidateRect(hwndBorder[ia], &rcTitle, FALSE);
        UpdateWindow(hwndBorder[ia]);
        FillRect(hdcMemBk[ib], &rcTitle, hbrBk);
        SetTextColor(hdcMemBk[ib], crInput);
        DrawText(hdcMemBk[ib], szTitle[ib], -1, &rcTitle, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
        InvalidateRect(hwndBorder[ib], &rcTitle, FALSE);
        UpdateWindow(hwndBorder[ib]);
        //��䵱ǰ��
        FillRect(hdcMemBk[id], &rcTitle, hbrBk);
        SetTextColor(hdcMemBk[id], crInput_click);
        DrawText(hdcMemBk[id], szTitle[id], -1, &rcTitle, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
        InvalidateRect(hwnd, &rcTitle, FALSE);
        UpdateWindow(hwnd);
        return 0;
    case WM_COMMAND:
        msgChild = HIWORD(wParam);
        if (msgChild == EN_SETFOCUS)
        {
            switch (id) {
            case 0:
                ia = 1;
                ib = 2;
                break;
            case 1:
                ia = 0;
                ib = 2;
                break;
            case 2:
                ia = 0;
                ib = 1;
                break;
            }
            //�����ǰ��
            FillRect(hdcMemBk[ia], &rcTitle, hbrBk);
            SetTextColor(hdcMemBk[ia], crInput);
            DrawText(hdcMemBk[ia], szTitle[ia], -1, &rcTitle, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
            InvalidateRect(hwndBorder[ia], &rcTitle, FALSE);
            UpdateWindow(hwndBorder[ia]);
            FillRect(hdcMemBk[ib], &rcTitle, hbrBk);
            SetTextColor(hdcMemBk[ib], crInput);
            DrawText(hdcMemBk[ib], szTitle[ib], -1, &rcTitle, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
            InvalidateRect(hwndBorder[ib], &rcTitle, FALSE);
            UpdateWindow(hwndBorder[ib]);
            //��䵱ǰ��
            FillRect(hdcMemBk[id], &rcTitle, hbrBk);
            SetTextColor(hdcMemBk[id], crInput_click);
            DrawText(hdcMemBk[id], szTitle[id], -1, &rcTitle, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
            InvalidateRect(hwnd, &rcTitle, FALSE);
            UpdateWindow(hwnd);
        }
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk[id]);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk[id], hbmpBk[id]);
        DeleteObject(hfEdit);DeleteObject(hpenBorder);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
static LRESULT CALLBACK EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int id = (INT)GetWindowLongPtr(hwnd,GWL_ID);
    switch (uMsg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            SendMessage(hwndButton, WM_LBUTTONUP, 0, 0);
            break;
        case VK_TAB:
            switch (id)
            {
            case 0:
                SetFocus(hwndInput[1]);
                break;
            case 1:
                SetFocus(hwndInput[2]);
                break;
            case 2:
                SetFocus(hwndInput[0]);
                break;
            }
            break;
        default:
            return CallWindowProc(DefEditProc, hwnd, uMsg, wParam, lParam);
        }
        return 0;
    default:
        return CallWindowProc(DefEditProc, hwnd, uMsg, wParam, lParam);
    }
    
}
//ע�ᰴť��Ϣ
static LRESULT CALLBACK RegisterButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    TCHAR szBuf1[32] = TEXT("\0");
    TCHAR szBuf2[32] = TEXT("\0");
    TCHAR szBuf3[32] = TEXT("\0");
    //��������
    static HBRUSH hbrBk, hbrBk_hover, hbrBk_click;
    static HFONT hf;
    static BOOL fClickNow = FALSE;
    static TCHAR szText[5];
    USERACCOUNT ua;
    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            hbrBk = CreateSolidBrush(RGB(128, 69, 197));
            hbrBk_hover = CreateSolidBrush(RGB(120, 60, 190));
            hbrBk_click = CreateSolidBrush(RGB(100, 50, 180));
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk, &hbmpBk);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            hf = SetFont(hdcMemBk, cyClient / 2, cyClient / 2 * 3 / 8, FONT_STYLE_DEFAULT + 450);
            SelectObject(hdcMemBk, hf);
            StringCchPrintf(szText, 5, TEXT("ȷ����Ϣ"));
            DrawText(hdcMemBk, szText, -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
            fInited = TRUE;
        }
        return 0;

    case WM_LBUTTONDOWN:
        fClickNow = TRUE;
        FillRect(hdcMemBk, &rcClient, hbrBk_click);
        DrawText(hdcMemBk, szText, -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);

        return 0;
    case WM_LBUTTONUP:
        fClickNow = FALSE;
        FillRect(hdcMemBk, &rcClient, hbrBk_hover);
        DrawText(hdcMemBk, szText, -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        GetWindowText(hwndEdit[0], szBuf1, 19);
        GetWindowText(hwndEdit[1], szBuf2, 19);
        GetWindowText(hwndEdit[2], szBuf3, 19);
        if (!DataValidCharcterSelfInfo(szBuf1, UA_SETINFO_NAME) ||
            !DataValidCharcterSelfInfo(szBuf2, UA_SETINFO_IDCARD) ||
            !DataValidCharcterSelfInfo(szBuf3, UA_SETINFO_NICKNAME)
            )
        {
            DataGetValidUA_SelfInfoStd(szBuf);
            if (IDOK == MessageBox(hwnd, szBuf, TEXT("��ʾ��Ϣ"), MB_OK | MB_ICONWARNING))
            {
                return 0;
            }
        }
        if (IDOK == MessageBox(hwnd, TEXT("ȷ���޸���ĸ�����Ϣ��"), TEXT("ע����ʾ"), MB_OKCANCEL))
        {
            //�ڴ����ݴ���
            DataUserAccount(&ua, UA_GET);
            DataAccount_SetSelfInfoToUA(szBuf1, UA_SETINFO_NAME, &ua);
            DataAccount_SetSelfInfoToUA(szBuf2, UA_SETINFO_IDCARD, &ua);
            DataAccount_SetSelfInfoToUA(szBuf3, UA_SETINFO_NICKNAME, &ua);
            DataUserAccount(&ua, UA_CHANGE);
            
            //�ڴ�->�ļ�
            DataUAFile(UA_UPDATE_FILE, &ua);

            e_w = EXIT_IN_MAINWND;
            StringCchPrintf(szBuf, 128, TEXT("�޸ĳɹ�����ӭ����%s"), szBuf3);
            if (IDOK == MessageBox(hwnd, szBuf, TEXT("��ӭ��¼�ɻ���Ʊϵͳ"),MB_OK))
            {
                DestroyWindow(hwndRegister);
            
                //����MainWindow
                hwndMain = CreateWindow(szAppName,
                    TEXT("�ɻ���Ʊϵͳ"),
                    WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
                    CW_USEDEFAULT, CW_USEDEFAULT,
                    cxMain, cyMain,
                    NULL, NULL, hInst, NULL);
                ShowWindow(hwndMain, SW_SHOWNORMAL);
                UpdateWindow(hwndMain);
            }


        }
        return 0;
    case WM_MOUSEMOVE:
    {
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, 30 };
        TrackMouseEvent(&tme);
    }
    if (!fClickNow)
    {
        FillRect(hdcMemBk, &rcClient, hbrBk_hover);
        DrawText(hdcMemBk, szText, -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
    }
    return 0;

    case WM_MOUSELEAVE:

        FillRect(hdcMemBk, &rcClient, hbrBk);
        DrawText(hdcMemBk, szText, -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);

        fClickNow = FALSE;
        return 0;

    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hbrBk); DeleteObject(hbrBk_hover); DeleteObject(hbrBk_click);
        DeleteObject(hf);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}