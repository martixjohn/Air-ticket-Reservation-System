/*
*       setting.c
*       �����������ý�����غ�������ͨ�û������Ա����
*/
#include "bodyuser.h"

//���ƶ���,��������
static TCHAR szUserInfoName[] = TEXT("�û���Ϣ�޸�");
static TCHAR szUserInfo_VerifyName[] = TEXT("�û���Ϣ�޸���֤����");
static TCHAR szUserInfo_InputName[] = TEXT("�û���Ϣ�޸������");
static TCHAR szUserInfo_ButtonName[] = TEXT("�û���Ϣ�޸��ύ��ť");
static TCHAR szHelpName[] = TEXT("���ڳ���");
static TCHAR szBackButtonName[] = TEXT("�����з��ذ�ť");
static USERACCOUNT ua;
//����ǰ������
static BOOL UserSettingRegsiter(HINSTANCE hInstance);

//���ڹ���
static LRESULT CALLBACK UserSetting_UserInfoProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserSetting_UserInfo_VerifyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserSetting_UserInfo_Verify_EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserSetting_UserInfo_InputProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static WNDPROC DefEditProc1,DefEditProc2;
static LRESULT CALLBACK UserSetting_UserInfo_ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserSetting_UserInfo_Input_EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserSetting_HelpProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserSetting_BackButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//���ھ��
static HWND hwndUserInfo, hwndHelp;
static HWND hwndVerify;
//UserInfo
static HWND hwndInput[4];
static HWND hwndEdit[4];
static HWND hwndUserInfoButton;

/*
*   ����: UserSettingRegsiter(HINSTANCE hInstance)
*
*   Ŀ��: ע��UserSetting�Ӵ�����
*
*   ���룺hInstance        ʵ�����
*
*   ���أ�BOOL             �ж�ע���Ƿ�ɹ�
*/
static BOOL UserSettingRegsiter(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    HBRUSH hBrush;
    /*USER INFO*/
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = UserSetting_UserInfoProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szUserInfoName;
    if (!RegisterClass(&wndclass)) return FALSE;
    //Verify
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpfnWndProc = UserSetting_UserInfo_VerifyProc;
    wndclass.lpszClassName = szUserInfo_VerifyName;
    if (!RegisterClass(&wndclass)) return FALSE;
    //Input
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.lpfnWndProc = UserSetting_UserInfo_InputProc;
    wndclass.lpszClassName = szUserInfo_InputName;
    if (!RegisterClass(&wndclass)) return FALSE;

    //Button
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.lpfnWndProc = UserSetting_UserInfo_ButtonProc;
    wndclass.lpszClassName = szUserInfo_ButtonName;
    if (!RegisterClass(&wndclass)) return FALSE;

    //Help
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.lpfnWndProc = UserSetting_HelpProc;
    wndclass.lpszClassName = szHelpName;
    if (!RegisterClass(&wndclass)) return FALSE;

    //BackButton
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.lpfnWndProc = UserSetting_BackButtonProc;
    wndclass.lpszClassName = szBackButtonName;
    if (!RegisterClass(&wndclass)) return FALSE;


    return TRUE;
}
/*
*   ����: UserSettingProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ����UserSetting����Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
LRESULT CALLBACK UserSettingProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf1[128] = TEXT("\0");
    TCHAR szBuf2[128] = TEXT("\0");
    //��������
    HDC hdc;
    //TITLE
    static HFONT hfTitle;
    static RECT rcTitle;
    //CONTENT
    static RECT rcCon;
    static HPEN hpenOrg;
    static HPEN hpenHover;
    static BOOL fHover[2] = { FALSE };
    static RECT rcOption[2];
    static HFONT hfCon;
    //USER LOGO
    static RECT rcUserLogo;
    static HDC hdcMemUserLogo;
    static HBITMAP hbmpUserLogo;
    static BITMAP bmpUserLogo;
    static RECT rcUserLogoItem[2];
    static HFONT hfUserLogoItem;
    //USER EDIT
    static RECT rcUserInfo;

    //SETTING
    static RECT rcHelp;

    int i;
    POINT pt;
    switch (uMsg)
    {
    case WM_CREATE:
        UserSettingRegsiter(hInst);
        //�û���Ϣ��ȡ
        DataUserAccount(&ua, UA_GET);
        return 0;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            DefInitWindow(hwnd, cxClient, cyClient, hbrBodyBk, &rcClient, &hdcMemBk, &hbmpBk);
            
            
            //TITLE
            SetRect(&rcTitle, cxClient / 20, cxClient / 20, cxClient * 5 / 20, cyClient * 3 / 20);
            hfTitle = SetFont(hdcMemBk, (rcTitle.bottom - rcTitle.top),
                (rcTitle.bottom - rcTitle.top) * 3 / 8, FONT_STYLE_DEFAULT + 500);
            SelectObject(hdcMemBk, hfTitle);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            SetBkMode(hdcMemBk, TRANSPARENT);
            DrawText(hdcMemBk, TEXT("����"), -1, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_LEFT);

            //CONTENT
            hpenOrg = CreatePen(PS_SOLID | PS_INSIDEFRAME, cxClient / 150, crMenuBk);
            hpenHover = CreatePen(PS_SOLID | PS_INSIDEFRAME, cxClient / 150, RGB(11, 136, 204));
            SetRect(&rcCon, cxClient * 2 / 20, cyClient * 4 / 20, cxClient * 18 / 20, cyClient * 18 / 20);
            SelectObject(hdcMemBk, hbrMenuBk);
            SelectObject(hdcMemBk, (HPEN)hpenOrg);
            //USER LOGO
            SetRect(&rcUserLogo, rcCon.left, rcCon.top, rcCon.left + (rcCon.right - rcCon.left) * 2 / 5, rcCon.bottom);
            Rectangle(hdcMemBk, rcUserLogo.left, rcUserLogo.top, rcUserLogo.right, rcUserLogo.bottom);
            hdc = GetDC(hwnd);
            hdcMemUserLogo = CreateCompatibleDC(hdc);
            ReleaseDC(hwnd, hdc);
            hbmpUserLogo = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_USER_BKCR_256_256));
            SelectObject(hdcMemUserLogo, hbmpUserLogo);
            GetObject((HBITMAP)hbmpUserLogo, sizeof(BITMAP), &bmpUserLogo);
            SetStretchBltMode(hdcMemBk, HALFTONE);
            StretchBlt(hdcMemBk, rcUserLogo.left + (rcUserLogo.right - rcUserLogo.left) / 4,
                rcUserLogo.top + (rcUserLogo.bottom - rcUserLogo.top) / 8,
                (rcUserLogo.right - rcUserLogo.left) / 2, (rcUserLogo.right - rcUserLogo.left) / 2,
                hdcMemUserLogo, 0, 0, bmpUserLogo.bmWidth, bmpUserLogo.bmHeight, SRCCOPY);
            SetRect(&rcUserLogoItem[0], rcUserLogo.left, rcUserLogo.top + (rcUserLogo.bottom - rcUserLogo.top) * 26 / 40,
                rcUserLogo.right, rcUserLogo.top + (rcUserLogo.bottom - rcUserLogo.top) * 29 / 40);
            SetRect(&rcUserLogoItem[1], rcUserLogo.left, rcUserLogo.top + (rcUserLogo.bottom - rcUserLogo.top) * 30 / 40,
                rcUserLogo.right, rcUserLogo.top + (rcUserLogo.bottom - rcUserLogo.top) * 33 / 40);
            hfUserLogoItem = SetFont(hdcMemBk, (rcUserLogoItem[0].bottom - rcUserLogoItem[0].top) * 4 / 5,
                (rcUserLogoItem[0].bottom - rcUserLogoItem[0].top) * 4 / 5 * 3 / 8, FONT_STYLE_DEFAULT + 300);
            hfCon = SetFont(hdcMemBk, (rcUserLogoItem[0].bottom - rcUserLogoItem[0].top),
                (rcUserLogoItem[0].bottom - rcUserLogoItem[0].top) * 3 / 8, FONT_STYLE_DEFAULT + 300);
            SelectObject(hdcMemBk, hfUserLogoItem);


            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            StringCchPrintf(szBuf2, 128, TEXT("UID:%s"), ua.szUsername);
            DrawText(hdcMemBk, szBuf2, -1, &rcUserLogoItem[0], DT_SINGLELINE | DT_CENTER | DT_VCENTER);

            DataAccount_GetSelfInfoFromUA(szBuf1, UA_GETINFO_NICKNAME, &ua);
            StringCchPrintf(szBuf2, 128, TEXT("�ǳ�:%s"), szBuf1);
            DrawText(hdcMemBk, szBuf2, -1, &rcUserLogoItem[1], DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            
            

            //USER INFO EDIT
            SelectObject(hdcMemBk, hfCon);
            SetRect(&rcUserInfo, rcUserLogo.right + (rcCon.bottom - rcCon.top) / 20,
                rcCon.top, 
                rcCon.right,
                rcCon.top + (rcCon.bottom - rcCon.top) * 19 / 40);
            Rectangle(hdcMemBk, rcUserInfo.left, rcUserInfo.top, rcUserInfo.right, rcUserInfo.bottom);
            DrawText(hdcMemBk, TEXT("������Ϣ�鿴���޸�"), -1, &rcUserInfo, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            
            //SETTING
            SetRect(&rcHelp, rcUserLogo.right + (rcCon.bottom - rcCon.top) / 20,
                rcCon.top + (rcCon.bottom - rcCon.top) * 21 / 40,
                rcCon.right,
                rcCon.bottom);
            Rectangle(hdcMemBk, rcHelp.left, rcHelp.top, rcHelp.right, rcHelp.bottom);
            DrawText(hdcMemBk, TEXT("���ڳ���"), -1, &rcHelp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

            rcOption[0] = rcUserInfo;
            rcOption[1] = rcHelp;

            //�Ӵ��ڴ���
            hwndUserInfo = CreateWindow(szUserInfoName, NULL, WS_CHILD, 0, 0, cxClient, cyClient, 
                hwnd, (HMENU)USERSETTINGID_USERINFO, hInst, NULL);
            
            hwndHelp = CreateWindow(szHelpName, NULL, WS_CHILD, 0, 0, cxClient, cyClient,
                hwnd, (HMENU)USERSETTINGID_HELP, hInst, NULL);
            hwndVerify = CreateWindow(szUserInfo_VerifyName, TEXT("������֤"), WS_OVERLAPPED | WS_SYSMENU,
                rcUserInfo.left, rcUserInfo.top, cxClient / 3, cyClient / 4, NULL, 0, hInst, NULL);
            fInited = TRUE;
        }
        return 0;


    case WM_MOUSEMOVE:
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        for (i = 0; i < 2; i++)
        {
            if (PT_ON_RECT(pt, rcOption[i]) && !fHover[i])
            {
                SelectObject(hdcMemBk, (HBRUSH)GetStockObject(NULL_BRUSH));
                SelectObject(hdcMemBk, (HPEN)hpenHover);
                Rectangle(hdcMemBk, rcOption[i].left, rcOption[i].top, rcOption[i].right, rcOption[i].bottom);
                InvalidateRect(hwnd, &rcOption[i], FALSE);
                UpdateWindow(hwnd);
                fHover[i] = TRUE;
            }
            if (!PT_ON_RECT(pt, rcOption[i]) && fHover[i])
            {
                SelectObject(hdcMemBk, (HBRUSH)GetStockObject(NULL_BRUSH));
                SelectObject(hdcMemBk, (HPEN)hpenOrg);
                Rectangle(hdcMemBk, rcOption[i].left, rcOption[i].top, rcOption[i].right, rcOption[i].bottom);
                InvalidateRect(hwnd, &rcOption[i], FALSE);
                UpdateWindow(hwnd);
                fHover[i] = FALSE;
            }
        }

        return 0;
    case WM_LBUTTONUP:
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        if (PT_ON_RECT(pt, rcUserInfo))
        {
            ShowWindow(hwndVerify, SW_SHOW);
            UpdateWindow(hwndVerify);
            SetFocus(hwndVerify);
        }
        if (PT_ON_RECT(pt, rcHelp))
        {
            ShowWindow(hwndHelp, SW_SHOW);
        }
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfTitle); DeleteObject(hfCon);
        DeleteDC(hdcMemUserLogo);
        DeleteObject(hbmpUserLogo);
        DeleteObject(hfUserLogoItem);
        DeleteObject(hpenHover);
        DeleteObject(hpenOrg);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

/*USER INFO*/
/*
*   ����: UserSetting_UserInfoProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ����UserSetting�С��޸��û���Ϣ�����ڹ��̵���Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
static LRESULT CALLBACK UserSetting_UserInfoProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //��������
    static HWND hwndBackButton;
    //TITLE
    static HFONT hfTitle;
    static RECT rcTitle;
    //CONTENT
    static RECT rcCon;
    static SIZE sizeInput;
    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            DefInitWindow(hwnd, cxClient, cyClient, hbrBodyBk, &rcClient, &hdcMemBk, &hbmpBk);
            //�û���Ϣ��ȡ
            DataUserAccount(&ua, UA_GET);

            //BACKBUTTON
            hwndBackButton = CreateWindow(szBackButtonName,
                TEXT("���ذ�ť"),
                WS_CHILD | WS_VISIBLE,
                cxClient / 25, cxClient / 25,
                cxClient / 25, cxClient / 25,
                hwnd, (HMENU)USERBOOKINGID_DETAIL_BACKBUTTON, hInst, NULL
            );
            //TITLE
            SetRect(&rcTitle, cxClient * 3 / 25, cxClient / 25, cxClient, cxClient * 2 / 25);
            hfTitle = SetFont(hdcMemBk, (rcTitle.bottom - rcTitle.top),
                (rcTitle.bottom - rcTitle.top) * 3 / 8, FONT_STYLE_DEFAULT + 500);
            SelectObject(hdcMemBk, hfTitle);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            SetBkMode(hdcMemBk, TRANSPARENT);
            DrawText(hdcMemBk, TEXT("������Ϣ"), -1, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_LEFT);

            //CONTEMT
            SetRect(&rcCon, cxClient * 4 / 25, cxClient * 4 / 25, cxClient * 21 / 25, cyClient * 19 / 20);
            sizeInput.cx = (rcCon.right - rcCon.left) * 3 / 5;
            sizeInput.cy = (rcCon.bottom - rcCon.top) / 8;
            hwndInput[0] = CreateWindow(szUserInfo_InputName,
                TEXT("��������"),
                WS_CHILD | WS_VISIBLE,
                rcCon.left + ((rcCon.right - rcCon.left) - sizeInput.cx) / 2,
                rcCon.top,
                sizeInput.cx, sizeInput.cy,
                hwnd,
                (HMENU)USERSETTINGID_USERINFO_INPUTNAME, hInst, NULL);
            hwndInput[1] = CreateWindow(szUserInfo_InputName,
                TEXT("���֤����"),//�Զ�ʶ��������Ϣ
                WS_CHILD | WS_VISIBLE,
                rcCon.left + ((rcCon.right - rcCon.left) - sizeInput.cx) / 2,
                rcCon.top + sizeInput.cy * 4 / 3,
                sizeInput.cx, sizeInput.cy,
                hwnd,
                (HMENU)USERSETTINGID_USERINFO_INPUTIDCARD, hInst, NULL);
            hwndInput[2] = CreateWindow(szUserInfo_InputName,
                TEXT("�ǳ�����"),
                WS_CHILD | WS_VISIBLE,
                rcCon.left + ((rcCon.right - rcCon.left) - sizeInput.cx) / 2,
                rcCon.top + sizeInput.cy * 8 / 3,
                sizeInput.cx, sizeInput.cy,
                hwnd,
                (HMENU)USERSETTINGID_USERINFO_INPUTNICKNAME, hInst, NULL);
            hwndInput[3] = CreateWindow(szUserInfo_InputName,
                TEXT("�����޸�"),
                WS_CHILD | WS_VISIBLE,
                rcCon.left + ((rcCon.right - rcCon.left) - sizeInput.cx) / 2,
                rcCon.top + sizeInput.cy * 12 / 3,
                sizeInput.cx, sizeInput.cy,
                hwnd,
                (HMENU)USERSETTINGID_USERINFO_INPUTPASSWORD, hInst, NULL);
            hwndUserInfoButton = CreateWindow(szUserInfo_ButtonName,
                TEXT("�ύ��ť"),
                WS_CHILD | WS_VISIBLE,
                rcCon.left + ((rcCon.right - rcCon.left) - sizeInput.cx / 4) / 2,
                rcCon.top + sizeInput.cy * 16 / 3,
                sizeInput.cx / 4, sizeInput.cy,
                hwnd,
                (HMENU)USERSETTINGID_USERINFO_INPUTPASSWORD, hInst, NULL);
            fInited = TRUE;
        }
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfTitle);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
static HWND hwndPswdInput,hwndButton;
static LRESULT CALLBACK UserSetting_UserInfo_VerifyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[32] = TEXT("\0");
    //��������

    static HFONT hf;
    static HFONT hf_s;
    switch (uMsg)
    {

    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        
        DefInitWindow(hwnd, cxClient, cyClient, (HBRUSH)GetStockObject(WHITE_BRUSH), &rcClient, &hdcMemBk, &hbmpBk);
        hf = SetFont(hdcMemBk, cyClient * 1 / 5, cyClient * 1 / 5 * 3 / 8, FONT_STYLE_DEFAULT + 400);
        hf_s = SetFont(hdcMemBk, cyClient * 1 / 7, cyClient * 1 / 7 * 3 / 8, FONT_STYLE_DEFAULT + 100);
        hwndPswdInput = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_PASSWORD | WS_BORDER,
            cxClient / 8, cyClient / 4, cxClient * 3 / 4, cyClient * 1 / 4, hwnd, (HMENU)0, hInst, NULL
        );
        hwndButton = CreateWindow(TEXT("BUTTON"), TEXT("ȷ��"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            cxClient * 2 / 5,
            cyClient * 5 / 8, cxClient / 5, cyClient / 5, hwnd, (HMENU)1, hInst, NULL);
        DefEditProc2 = (WNDPROC)SetWindowLongPtr(hwndPswdInput, GWL_WNDPROC, (LONG)UserSetting_UserInfo_Verify_EditProc);
        SendMessage(hwndPswdInput, WM_SETFONT, (WPARAM)hf, 0);
        SendMessage(hwndPswdInput, EM_SETLIMITTEXT, (WPARAM)12, 0);
        SendMessage(hwndButton, WM_SETFONT, (WPARAM)hf_s, 0);
        SetFocus(hwndPswdInput);
        fInited = TRUE;
       
        return 0;
    case WM_COMMAND:

        if ((HWND)lParam == hwndButton)
        {
            USERACCOUNT ua;
            DataUserAccount(&ua, UA_GET);
            GetWindowText(hwndPswdInput, szBuf, 12);
            if (lstrcmp(szBuf, ua.szPassword) == 0)
            {
                SetWindowText(hwndPswdInput, NULL);
                ShowWindow(hwndUserInfo, SW_SHOW);
                UpdateWindow(hwndUserInfo);
                SendMessage(hwnd, WM_CLOSE, 0, 0);
            }
            else
            {
                if(IDOK==MessageBox(hwnd,TEXT("�������"),TEXT("��ʾ"),MB_OK))
                {
                    SetWindowText(hwndPswdInput, NULL);
                    ShowWindow(hwndMain, SW_SHOW);
                    UpdateWindow(hwndMain);
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                }
            }
        }
        return 0;
    case WM_SETFOCUS:
        SetFocus(hwndPswdInput);
        return 0;
        //�༭�ؼ���ʽ
    case WM_CTLCOLOREDIT:

        return (LRESULT)GetStockObject(WHITE_BRUSH);//������ˢ
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_CLOSE:
        ShowWindow(hwnd, SW_HIDE);
        UpdateWindow(hwnd);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hf); DeleteObject(hf_s);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
static LRESULT CALLBACK UserSetting_UserInfo_Verify_EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int id = (INT)GetWindowLongPtr(hwnd, GWL_ID);
    switch (uMsg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            SendMessage(GetParent(hwnd), WM_COMMAND, 0, (LPARAM)hwndButton);
            break;
        default:
            return CallWindowProc(DefEditProc2, hwnd, uMsg, wParam, lParam);
        }
        return 0;
    default:
        return CallWindowProc(DefEditProc2, hwnd, uMsg, wParam, lParam);
    }

}
static LRESULT CALLBACK UserSetting_UserInfo_InputProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited[4] = { FALSE };
    static int cxClient, cyClient;
    static HDC hdcMemBk[4];
    static HBITMAP hbmpBk[4];
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //��������
    static HBRUSH hbrBk;

    static HWND hwndBorder[4];

    static BOOL fFirst = TRUE;
    static HPEN hpenBorder;
    //title
    static RECT rcTitle;
    static TCHAR szTitle[4][10];
    //edit
    static HFONT hfEdit;
    static COLORREF crInput, crInput_click;
    UINT msgChild;
    int id = (int)GetWindowLongPtr(hwnd, GWL_ID);
    static USERACCOUNT ua;
    int i;
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
                rcTitle.bottom - rcTitle.top,
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
            DefEditProc1 = (WNDPROC)SetWindowLongPtr(hwndEdit[id], GWL_WNDPROC, (LONG)UserSetting_UserInfo_Input_EditProc);
            SendMessage(hwndEdit[id], WM_SETFONT, (WPARAM)hfEdit, 0);
            
            switch (id)
            {
            case 0:
                DataUserAccount(&ua, UA_GET);
                DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_NAME, &ua);
                StringCchPrintf(szTitle[0], 10, TEXT("��ʵ����"));
                SendMessage(hwndEdit[id], EM_LIMITTEXT, (WPARAM)LIMIT_NAME, 0);
                break;
            case 1:
                DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_IDCARD, &ua);
                StringCchPrintf(szTitle[1], 10, TEXT("���֤����"));
                SendMessage(hwndEdit[id], EM_LIMITTEXT, (WPARAM)18, 0);
                SetWindowLongPtr(hwndEdit[1], GWL_STYLE, WS_CHILD | WS_VISIBLE | ES_NUMBER);
                break;
            case 2:
                DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_NICKNAME, &ua);
                StringCchPrintf(szTitle[2], 10, TEXT("��  ��"));
                SendMessage(hwndEdit[id], EM_LIMITTEXT, (WPARAM)LIMIT_NICKNAME, 0);
                break;
            case 3:default:
                lstrcpy(szBuf, ua.szPassword);
                StringCchPrintf(szTitle[3], 10, TEXT("��  ��"));
                SendMessage(hwndEdit[id], EM_LIMITTEXT, (WPARAM)LIMIT_PASSWORD, 0);
                break;

            }
            if (id == 1)
            {
                USERTYPE ut = DataGetUserType();
                SelectObject(hdcMemBk[id], hfEdit);
                SetBkMode(hdcMemBk[id], TRANSPARENT);
                if (ut == UT_USER)
                {
                    SetWindowText(hwndEdit[id], szBuf);
                    SetTextColor(hdcMemBk[id], RGB(80, 80, 80));
                }
                else
                {
                    //����Ա�����Ϣ������
                    SetTextColor(hdcMemBk[id], RGB(100, 100, 100));
                    TextOut(hdcMemBk[id], rcTitle.right + (rcTitle.right - rcTitle.left) / 8, rcTitle.top,
                        TEXT("����Ա�����Ϣ������"), lstrlen(TEXT("����Ա�����Ϣ������")));
                    ShowWindow(hwndEdit[id], SW_HIDE);
                }
            }
            else
            {
                SetWindowText(hwndEdit[id], szBuf);
                SetTextColor(hdcMemBk[id], RGB(80, 80, 80));
            }
            SelectObject(hdcMemBk[id], hfEdit);
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
        
        for (i = 0; i < 4; i++)
        {
            if (i == id)
            {
                //��䵱ǰ��
                FillRect(hdcMemBk[i], &rcTitle, hbrBk);
                SetTextColor(hdcMemBk[i], crInput_click);
                DrawText(hdcMemBk[i], szTitle[i], -1, &rcTitle, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
                InvalidateRect(hwnd, &rcTitle, FALSE);
                UpdateWindow(hwnd);
            }
            else
            {
                FillRect(hdcMemBk[i], &rcTitle, hbrBk);
                SetTextColor(hdcMemBk[i], crInput);
                DrawText(hdcMemBk[i], szTitle[i], -1, &rcTitle, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
                InvalidateRect(hwndBorder[i], &rcTitle, FALSE);
                UpdateWindow(hwndBorder[i]);
            }
        }
        

        return 0;
    case WM_COMMAND:
        msgChild = HIWORD(wParam);
        if (msgChild == EN_SETFOCUS)
        {
            for (i = 0; i < 4; i++)
            {
                if (i == id)
                {
                    //��䵱ǰ��
                    FillRect(hdcMemBk[i], &rcTitle, hbrBk);
                    SetTextColor(hdcMemBk[i], crInput_click);
                    DrawText(hdcMemBk[i], szTitle[i], -1, &rcTitle, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
                    InvalidateRect(hwnd, &rcTitle, FALSE);
                    UpdateWindow(hwnd);
                }
                else
                {
                    FillRect(hdcMemBk[i], &rcTitle, hbrBk);
                    SetTextColor(hdcMemBk[i], crInput);
                    DrawText(hdcMemBk[i], szTitle[i], -1, &rcTitle, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
                    InvalidateRect(hwndBorder[i], &rcTitle, FALSE);
                    UpdateWindow(hwndBorder[i]);
                }
            }

        }
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk[id]);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk[id], hbmpBk[id]);
        DeleteObject(hfEdit); DeleteObject(hpenBorder);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
static LRESULT CALLBACK UserSetting_UserInfo_Input_EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int id = (INT)GetWindowLongPtr(hwnd, GWL_ID);
    switch (uMsg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            SendMessage(hwndUserInfoButton, WM_LBUTTONUP, 0, 0);
            break;
        case VK_TAB:
            switch (id)
            {
            case 0:
                if (ua.ut == UT_USER)
                    SetFocus(hwndInput[1]);
                else
                    SetFocus(hwndInput[2]);
                break;
            case 1:
                SetFocus(hwndInput[2]);
                break;
            case 2:
                SetFocus(hwndInput[3]);
                break;
            case 3:
                SetFocus(hwndInput[0]);
                break;
            }
            break;
        default:
            return CallWindowProc(DefEditProc1, hwnd, uMsg, wParam, lParam);
        }
        return 0;
    default:
        return CallWindowProc(DefEditProc1, hwnd, uMsg, wParam, lParam);
    }

}
static LRESULT CALLBACK UserSetting_UserInfo_ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
    TCHAR szBuf4[32] = TEXT("\0");
    //��������
    static HBRUSH hbrBk, hbrBk_hover, hbrBk_click;
    static HFONT hf;
    static BOOL fClickNow = FALSE;
    static TCHAR szText[5];
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
            hf = SetFont(hdcMemBk, cyClient / 2, cyClient / 2 * 3 / 8, FONT_STYLE_DEFAULT + 400);
            SelectObject(hdcMemBk, hf);
            StringCchPrintf(szText, 5, TEXT("ȷ���޸�"));
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
        //����
        GetWindowText(hwndEdit[0], szBuf1, 19);
        //���֤����
        if (ua.ut == UT_USER)
            GetWindowText(hwndEdit[1], szBuf2, 19);
        //�ǳ�
        GetWindowText(hwndEdit[2], szBuf3, 19);
        //����
        GetWindowText(hwndEdit[3], szBuf4, 19);
        if (!DataValidCharcterSelfInfo(szBuf1, UA_SETINFO_NAME) ||
            !DataValidCharcterSelfInfo(szBuf3, UA_SETINFO_NICKNAME) ||
            !DataValidCharcterUNorPW(szBuf4)
            )
        {

            if (ua.ut == UT_USER)
            {
                if (!DataValidCharcterSelfInfo(szBuf2, UA_SETINFO_IDCARD))
                {
                    //��ʾ��Ϣ
                    DataGetValidUA_SelfInfoStd(szBuf);
                    if (IDOK == MessageBox(hwnd, szBuf, TEXT("��ʾ��Ϣ"), MB_OK | MB_ICONWARNING))
                    {
                        return 0;
                    }
                }
            }
            else
            {
                //��ʾ��Ϣ
                DataGetValidUA_SelfInfoStd(szBuf);
                if (IDOK == MessageBox(hwnd, szBuf, TEXT("��ʾ��Ϣ"), MB_OK | MB_ICONWARNING))
                {
                    return 0;
                }
            }
        }
        int i;
        for (i = 0; i < LIMIT_PASSWORD && szBuf4[i] != TEXT('\0'); i++);
        if (i < 5)
        {
            DataGetValidUA_UOPStd(szBuf);
            if (IDOK == MessageBox(hwnd, szBuf, TEXT("��ʾ��Ϣ"), MB_OK | MB_ICONWARNING))
            {
                return 0;
            }
        }
        if (IDOK == MessageBox(hwnd, TEXT("ȷ���޸���ĸ�����Ϣ���޸ĺ���򽫻�����"), TEXT("�޸���ʾ"), MB_OKCANCEL))
        {
            //�ڴ����ݴ���
            DataAccount_SetSelfInfoToUA(szBuf1, UA_SETINFO_NAME, &ua);
            if(ua.ut == UT_USER)
                DataAccount_SetSelfInfoToUA(szBuf2, UA_SETINFO_IDCARD, &ua);
            DataAccount_SetSelfInfoToUA(szBuf3, UA_SETINFO_NICKNAME, &ua);
            lstrcpy(ua.szPassword, szBuf4);
            DataUserAccount(&ua, UA_CHANGE);
            
            //�ڴ�->�ļ�
            if (UT_USER == ua.ut) DataUAFile(UA_UPDATE_FILE, &ua);
            else {
                LPUSERACCOUNTNODE pUaNode;
                DataAccount_LinkedListMem(&pUaNode, UALIST_GET_ADMIN);
                while (pUaNode != NULL)
                {
                    if (lstrcmp(pUaNode->ua.szUsername, ua.szUsername) == 0)
                    {
                        pUaNode->ua = ua;
                        break;
                    }
                    pUaNode = pUaNode->next;
                }
                DataAccount_LinkedListFile(UALIST_TO_FILE_ADMIN);
            }
            if (IDOK == MessageBox(hwnd, TEXT("�޸ĳɹ��������û���Ϣ���޸ġ������µ�¼"), TEXT("�޸���ʾ"), MB_OK))
            {
                DestroyWindow(hwndMain);
                PostQuitMessage(0);
                return 0;
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

/*HELP AND ABOUT*/
/*
*   ����: UserSetting_HelpProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ����UserSetting�С����ڡ����ڹ��̵���Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
static LRESULT CALLBACK UserSetting_HelpProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[512] = TEXT("\0");
    //��������
    static HWND hwndBackButton;
    //TITLE
    static HFONT hfTitle;
    static RECT rcTitle;
    //CONTENT
    static RECT rcCon;
    static HFONT hfCon;
    static RECT rcVer;
    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            DefInitWindow(hwnd, cxClient, cyClient, hbrBodyBk, &rcClient, &hdcMemBk, &hbmpBk);
            //�û���Ϣ��ȡ
            DataUserAccount(&ua, UA_GET);
            //BACKBUTTON
            hwndBackButton = CreateWindow(szBackButtonName,
                TEXT("���ذ�ť"),
                WS_CHILD | WS_VISIBLE,
                cxClient / 25, cxClient / 25,
                cxClient / 25, cxClient / 25,
                hwnd, (HMENU)USERBOOKINGID_DETAIL_BACKBUTTON, hInst, NULL
            );
            //TITLE
            SetRect(&rcTitle, cxClient * 3 / 25, cxClient / 25, cxClient, cxClient * 2 / 25);
            hfTitle = SetFont(hdcMemBk, (rcTitle.bottom - rcTitle.top),
                (rcTitle.bottom - rcTitle.top) * 3 / 8, FONT_STYLE_DEFAULT + 500);
            SelectObject(hdcMemBk, hfTitle);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            SetBkMode(hdcMemBk, TRANSPARENT);
            DrawText(hdcMemBk, TEXT("���ڳ���"), -1, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_LEFT);

            //VER
            SetRect(&rcVer, cxClient * 3 / 25, cyClient * 4 / 20, cxClient * 12 / 25, cyClient * 18 / 20);
            FillRect(hdcMemBk, &rcVer, hbrMenuBk);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            DrawText(hdcMemBk, TEXT("����汾:1.1"), -1, &rcVer, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            //CON
            hfCon = SetFont(hdcMemBk, (rcTitle.bottom - rcTitle.top) * 3 / 5,
                (rcTitle.bottom - rcTitle.top) * 3 / 5 * 3 / 8, FONT_STYLE_DEFAULT);
            SetRect(&rcCon, cxClient * 12 / 25, cyClient * 4 / 20, cxClient * 22 / 25, cyClient * 18 / 20);
            FillRect(hdcMemBk, &rcCon, (HBRUSH)GetStockObject(WHITE_BRUSH));
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            StringCchPrintf(szBuf, 128, TEXT("\n\n\n    ��������˵��...\n"));
            lstrcat(szBuf, TEXT("    ������ʹ�������κ�������Ϳ�������ϵ��\n    ���򹹽�����:C\n"));
            lstrcat(szBuf, TEXT("    ���ǿ����ߵĵ�һ��Windows����\n    �кܶ಻��֮�������Ҽ��£�\n    Ҳϣ�����ָ��������֮��\n    \n"));
            lstrcat(szBuf, TEXT("\n    ������������Դ���������\n    GitHub@martixjohn"));
            DrawText(hdcMemBk, szBuf, -1, &rcCon, DT_LEFT);

            fInited = TRUE;
        }
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfTitle);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*
*   ����: UserSetting_BackButtonProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ����UserSetting�С����ذ�ť�����ڹ��̵���Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
static LRESULT CALLBACK UserSetting_BackButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //��������
    static HDC hdcMemButton, hdcMemButton_clicked;
    static HBITMAP hbmpButton, hbmpButton_clicked;
    HDC hdc;
    static BITMAP bmp;
    static BOOL fHoverNow = FALSE;
    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            DefInitWindow(hwnd, cxClient, cyClient, hbrBodyBk, &rcClient, &hdcMemBk, &hbmpBk);
            hdc = GetDC(hwnd);
            hdcMemButton = CreateCompatibleDC(hdc);
            hdcMemButton_clicked = CreateCompatibleDC(hdc);
            if (cxClient > 200)
            {
                hbmpButton = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACK_B));
                hbmpButton_clicked = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACK_SELECTED_B));
            }
            else
            {
                hbmpButton = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACK_M));
                hbmpButton_clicked = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACK_SELECTED_M));
            }

            SelectObject(hdcMemButton, hbmpButton);
            SelectObject(hdcMemButton_clicked, hbmpButton_clicked);
            GetObject(hbmpButton, sizeof(BITMAP), &bmp);

            SetStretchBltMode(hdcMemBk, HALFTONE);
            StretchBlt(hdcMemBk, 0, 0, cxClient, cyClient,
                hdcMemButton, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
            ReleaseDC(hwnd, hdc);
            fHoverNow = FALSE;
            fInited = TRUE;
        }
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_LBUTTONUP:
        ShowWindow(GetParent(hwnd), SW_HIDE);
        ShowWindow(hwndBodyOpt[0][3], SW_SHOW);
        return 0;

    case WM_MOUSEMOVE:
    {
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, 30 };
        TrackMouseEvent(&tme);
    }
    if (!fHoverNow)
    {
        StretchBlt(hdcMemBk, 0, 0, cxClient, cyClient,
            hdcMemButton_clicked, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        fHoverNow = TRUE;
    }
    return 0;
    case WM_MOUSELEAVE:
        if (fHoverNow)
        {
            StretchBlt(hdcMemBk, 0, 0, cxClient, cyClient,
                hdcMemButton, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
            InvalidateRect(hwnd, NULL, FALSE);
            UpdateWindow(hwnd);
            fHoverNow = FALSE;
        }

        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteDC(hdcMemButton); DeleteDC(hdcMemButton_clicked);
        DeleteObject(hbmpButton); DeleteObject(hbmpButton_clicked);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}