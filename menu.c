/*
*       menu.c
*       �������в˵���غ���
*/
#include "mainwnd.h"
/*���ֶ���*/

static TCHAR szMenuOption[] = TEXT("MENUOPTION");
static TCHAR szMenuLogo[] = TEXT("MENULOGO");
/*����*/
static BOOL MenuRegister(HINSTANCE hInstance);
HWND hwndMenuOpt[4], hwndLogo;
/*
    ����Ա����ҳ ��Ʊ���� �û����� ����
    �û�  ����ҳ ����Ʊ �ҵĶ��� ����
*/

/*
*   ����: MenuRegister(HINSTANCE hInstance)
*
*   Ŀ��: ����˵������µĴ�����
*
*   ���룺hInstance   ʵ�����
*
*   ���أ�BOOL        �ж��Ƿ�ע��ɹ�
*/
static BOOL MenuRegister(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    HBRUSH hBrush;
    /*MenuOption*/
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = MenuOptionProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szMenuOption;
    if (!RegisterClass(&wndclass)) return FALSE;
    /*MenuLogo*/
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = MenuLogoProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szMenuLogo;
    if (!RegisterClass(&wndclass)) return FALSE;
    return TRUE;
}
/*
*   ����: MainMenuProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ����Menu����Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
LRESULT CALLBACK MainMenuProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static BOOL fInited = FALSE;
    static USERACCOUNT ua;
    static int cxClient, cyClient;
    static int idClicked = MENUID_NONE;//��ʾ��ѡ��
    static HWND hwndClicked = NULL;
    int i;
    switch (uMsg)
    {
    case WM_CREATE:
        DataUserAccount(&ua, UA_GET);
        MenuRegister(hInst);
        hwndLogo = CreateWindow(szMenuLogo,
            NULL, WS_CHILD,
            0, 0, 0, 0,
            hwnd, (HMENU)(MENUID_LOGO), hInst, NULL
        );
        for (i = 0; i < 4; i++)
        {
            hwndMenuOpt[i] = CreateWindow(szMenuOption,
                NULL, WS_CHILD,
                0, 0, 0, 0,
                hwnd, (HMENU)(MENUID_OPTION+i), hInst, NULL);
        }
        return 0;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            MoveWindow(hwndLogo, 0, 0, cxClient, cyClient / 5, FALSE);
            ShowWindow(hwndLogo, SW_SHOWNORMAL);
            for (i = 0; i < 4; i++)
            {
                //ע���������о��ȶ�ʧ�����¿����з�϶
                //cyClient / 5 + cyClient / 10 * i�滻cyClient / 5 + cyClient * i / 10 ���
                MoveWindow(hwndMenuOpt[i], 0, cyClient / 5 + cyClient / 10 * i, cxClient, cyClient / 10, FALSE);
                ShowWindow(hwndMenuOpt[i], SW_SHOWNORMAL);
            }
            fInited = TRUE;
        }

        return 0;
    case WM_DESTROY:
        DeleteObject(hbrMenuBk);
        return 0;
        //��option list����
    case WM_U_SET_CLICKED:
        //���浱ǰClicked
        idClicked = (int)wParam;
        hwndClicked = (HWND)lParam;
        //��Body����Clicked��Ϣ
        SendMessage(hwndBody, WM_U_CLICKCHANGED, (WPARAM)idClicked, (LPARAM)hwndClicked);
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
/*
*   ����: MenuOptionProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ����Menu�µ�ѡ��ڵ���Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
LRESULT CALLBACK MenuOptionProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HDC hdcMem[4];
    static HBITMAP hbmpBk[4];
    static int cxClient, cyClient;
    static RECT rcClient;
    static HFONT hFont;
    static HBRUSH hbrBkHover, hbrBkSelected;
    static TCHAR* szOptionName[2][4] = {
        TEXT("��ҳ"),TEXT("����Ʊ"),TEXT("�ҵĶ���"),TEXT("����"),
        TEXT("��ҳ"),TEXT("�������"),TEXT("�û�����"),TEXT("����")
    };
    static BOOL fFirstWindow = TRUE;
    static BOOL fClicked[4] = { FALSE,FALSE,FALSE,FALSE };
    HDC hdc;
    PAINTSTRUCT ps;
    TCHAR szBuf[128] = { TEXT('\0') };
    int i;
    int id = GetWindowLongPtr(hwnd, GWL_ID);
    switch (uMsg)
    {

    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        SetRect(&rcClient, 0, 0, cxClient, cyClient);
        hdc = GetDC(hwnd);
        hdcMem[id] = CreateCompatibleDC(hdc);
        hbmpBk[id] = CreateCompatibleBitmap(hdc, cxClient, cyClient);
        SelectObject(hdcMem[id], hbmpBk[id]);
        FillRect(hdcMem[id], &rcClient, hbrMenuBk);
        if (fFirstWindow)
        {
            hFont = CreateFont(cyClient * 3 / 10, cyClient * 3 / 10 * 3 / 8, 0, 0, 400,
                FALSE/*����б��*/, FALSE/*�����»���*/, FALSE/*����ɾ����*/,
                DEFAULT_CHARSET, //ʹ��Ĭ���ַ���
                OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS,
                PROOF_QUALITY, //Ĭ���������
                DEFAULT_PITCH,
                APP_DEFAULT_FONT //������
            );
            hbrBkHover = CreateSolidBrush(RGB(0, 50, 100));
            hbrBkSelected = CreateSolidBrush(RGB(0, 30, 70));
        }
        SelectObject(hdcMem[id], hFont);
        SetTextColor(hdcMem[id], RGB(255, 255, 255));
        SetBkMode(hdcMem[id], TRANSPARENT);
        DrawText(hdcMem[id], szOptionName[wndtype][id], -1, &rcClient, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        if (fFirstWindow)
        {
            fFirstWindow = FALSE;
            //��һ��ѡ��Ĭ��ѡ��
            FillRect(hdcMem[id], &rcClient, hbrBkHover);
            DrawText(hdcMem[id], szOptionName[wndtype][id], -1, &rcClient, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            fClicked[id] = TRUE;
        }
        ReleaseDC(hwnd, hdc);
        return 0;
    case WM_MOUSEMOVE:

        FillRect(hdcMem[id], &rcClient, hbrBkHover);
        DrawText(hdcMem[id], szOptionName[wndtype][id], -1, &rcClient, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        InvalidateRect(hwnd, &rcClient, FALSE);
        UpdateWindow(hwnd);

        {
            TRACKMOUSEEVENT tme = {
                sizeof(tme),TME_LEAVE,hwnd,30
            };
            TrackMouseEvent(&tme);
        }
        return 0;
    case WM_LBUTTONDOWN:
        FillRect(hdcMem[id], &rcClient, hbrBkSelected);
        DrawText(hdcMem[id], szOptionName[wndtype][id], -1, &rcClient, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        InvalidateRect(hwnd, &rcClient, FALSE);
        UpdateWindow(hwnd);
        
        return 0;
    case WM_LBUTTONUP:
        FillRect(hdcMem[id], &rcClient, hbrBkHover);
        DrawText(hdcMem[id], szOptionName[wndtype][id], -1, &rcClient, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        InvalidateRect(hwnd, &rcClient, FALSE);
        UpdateWindow(hwnd);
        //��־�Ѿ����£���û��Ҫ����
        if (fClicked[id])
        {
            return 0;
        }
        else
        {
            for (i = 0; i < 4; i++)
            {
                if (i == id) {
                    fClicked[id] = TRUE;
                    SendMessage(hwndMenu, WM_U_SET_CLICKED, (WPARAM)id, (LPARAM)hwnd);
                }
                else {
                    //����ȡ������
                    fClicked[i] = FALSE;
                    FillRect(hdcMem[i], &rcClient, hbrMenuBk);
                    DrawText(hdcMem[i], szOptionName[wndtype][i], -1, &rcClient, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
                    InvalidateRect(hwndMenuOpt[i], &rcClient, FALSE);
                    UpdateWindow(hwndMenuOpt[i]);
                }
            }
        }
        return 0;
    case WM_MOUSELEAVE:
        if (fClicked[id]) return 0;
        FillRect(hdcMem[id], &rcClient, hbrMenuBk);
        DrawText(hdcMem[id], szOptionName[wndtype][id], -1, &rcClient, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        InvalidateRect(hwnd, &rcClient, FALSE);
        UpdateWindow(hwnd);

        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top,
            hdcMem[id], ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
        EndPaint(hwnd, &ps);
        return 0;
    
    case WM_DESTROY:
        DeleteDC(hdcMem[id]);
        DeleteObject(hbmpBk[id]);
        DeleteObject(hFont);
        DeleteObject(hbrBkHover);
        DeleteObject(hbrBkSelected);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
/*
*   ����: MenuLogoProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ����Menu�µ�Logo���ڵ���Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
LRESULT CALLBACK MenuLogoProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static int cxClient, cyClient;
    static int cxLogo;
    static HDC hdcMem,hdcMemBmp;
    static HBITMAP hBmp;
    static HBITMAP hBmpBk;
    static BITMAP bmp;
    HDC hdc;
    PAINTSTRUCT ps;
    static RECT rcClient;
    switch (uMsg)
    {
    case WM_SIZE:
        
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        SetRect(&rcClient, 0, 0, cxClient, cyClient);
        hdc = GetDC(hwnd);
        hdcMem = CreateCompatibleDC(hdc);//del needed
        hBmpBk = CreateCompatibleBitmap(hdc, cxClient, cyClient);
        SelectObject(hdcMem, hBmpBk);

        hdcMemBmp = CreateCompatibleDC(hdc);//del needed
       
        hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_USER_BKCR_256_256));//del needed
        SelectObject(hdcMemBmp, hBmp);
        GetObject(hBmp, sizeof(BITMAP), &bmp);

        FillRect(hdcMem, &rcClient, hbrMenuBk);
        SetStretchBltMode(hdcMem, HALFTONE);
        cxLogo = min(cxClient / 2, cyClient / 2);
        StretchBlt(hdcMem, (cxClient - cxLogo) / 2, (cyClient - cxLogo) / 2, cxLogo, cxLogo,
            hdcMemBmp, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
        ReleaseDC(hwnd, hdc);
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
        EndPaint(hwnd, &ps);
        return 0;
    case WM_DESTROY:
        DeleteDC(hdcMem);
        DeleteDC(hdcMemBmp);
        DeleteObject(hBmp);
        DeleteObject(hBmpBk);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}