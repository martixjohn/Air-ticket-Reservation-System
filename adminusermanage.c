/*
*       adminusermanage.c
*       ���������û�������غ���
*/
#include "bodyadmin.h"
//���ƶ���
static TCHAR szSearchRetName[] = TEXT("�û�����-�����������");
static TCHAR szAddAdminName[] = TEXT("�û�����-��ӹ���");
static TCHAR szAddAdmin_BackButtonName[] = TEXT("�û�����-��ӹ���-����");
static TCHAR szAddAdmin_AddButtonName[] = TEXT("�û�����-��ӹ���-��Ӱ�ť");
static USERTYPE ut;//��ǰ����Ա�û�����
//�Ӵ��ڹ���
static LRESULT CALLBACK AdminUserManage_SearchEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static WNDPROC DefEditWndProc;
static LRESULT CALLBACK AdminUserManage_SearchRetProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK AdminUserManage_AddAdmin(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK AdminUserManage_AddAdmin_BackButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK AdminUserManage_AddAdmin_AddButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//���ھ��
static HWND hwndSearchRet, hwndSearchInput;
static HWND hwndAddAdmin;
static HWND hwndCheckBox[2];

static RECT rcSearch;

//ע�ᴰ����
/*
*   ����: AdminUserManageRegsiter(HINSTANCE hInstance)
*
*   Ŀ��: ע��AdminUserManage�Ӵ�����
*
*   ���룺hInstance        ʵ�����
*
*   ���أ�BOOL             �ж�ע���Ƿ�ɹ�
*/
static BOOL AdminUserManageRegsiter(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    HBRUSH hBrush;
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = AdminUserManage_SearchRetProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szSearchRetName;
    if (!RegisterClass(&wndclass)) return FALSE;
    //Admin
    wndclass.lpfnWndProc = AdminUserManage_AddAdmin;
    wndclass.lpszClassName = szAddAdminName;
    if (!RegisterClass(&wndclass)) return FALSE;
    wndclass.lpfnWndProc = AdminUserManage_AddAdmin_BackButtonProc;
    wndclass.lpszClassName = szAddAdmin_BackButtonName;
    if (!RegisterClass(&wndclass)) return FALSE;
    wndclass.lpfnWndProc = AdminUserManage_AddAdmin_AddButtonProc;
    wndclass.lpszClassName = szAddAdmin_AddButtonName;
    if (!RegisterClass(&wndclass)) return FALSE;
    return TRUE;
}

/*
*   ����: AdminUserManageProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: �����û�������Ҫ�����е���Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
LRESULT CALLBACK AdminUserManageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    //��������
    //BAR
    static RECT rcAdd, rcAll, rcAll_admin, rcInputBorder, rcCheckBox[2];
    //CHECKBOX
    static BOOL fChecked[2] = {FALSE};
    static HFONT hfButton, hfInput;
    static HBRUSH hbrBk;
    static RECT rcSheetHead, rcSheetHeadItem[6];
    static HBRUSH hbrSearch, hbrSearch_hover, hbrButton1_hover, hbrButton2_hover, hbrButton3_hover;
    POINT ptMouse;

    //����
    static LPUSERACCOUNTNODE pUaNode_all = NULL;
    LPUSERACCOUNTNODE pUaNode = NULL;
    LPUSERACCOUNTNODE pUaNodeArr[MAX_SEARCH_UA_ID] = { NULL };
    int i = 0;
    TCHAR szBuf[128]=TEXT("\0");

    switch (uMsg)
    {
    case WM_CREATE:
        AdminUserManageRegsiter(hInst);
        return 0;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            hbrBk = hbrBodyBk;
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk, &hbmpBk);
            //��ȡ�û����ͣ���ADMIN����SUPERADMIN
            ut = DataGetUserType();
            //������
            SetRect(&rcInputBorder, cxClient / 40, cyClient * 4 / 80,
                cxClient * 13 / 40, cyClient * 10 / 80);
            FillRect(hdcMemBk, &rcInputBorder, (HBRUSH)GetStockObject(WHITE_BRUSH));
            
            hwndSearchInput = CreateWindow(TEXT("EDIT"),
                NULL, WS_CHILD | WS_VISIBLE,
                cxClient * 4 / 40 + 3, cyClient * 4 / 80 + cyClient * 3 / 160,
                cxClient * 8 / 40, cyClient * 3 / 80,
                hwnd, (HMENU)ADMINFLIGHTID_SEARCH_SEARCHBAR, hInst, NULL);
            DefEditWndProc = (WNDPROC)SetWindowLongPtr(hwndSearchInput, GWL_WNDPROC, (LONG)AdminUserManage_SearchEditProc);
            hfInput = SetFont(hdcMemBk, cyClient * 3 / 80,
                cyClient * 3 / 80  * 3 / 8,
                FONT_STYLE_DEFAULT);
            SelectObject(hdcMemBk, hfInput);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            DrawText(hdcMemBk, TEXT("    UID"), -1, &rcInputBorder, DT_SINGLELINE | DT_VCENTER);
            SendMessage(hwndSearchInput, WM_SETFONT, (WPARAM)hfInput, 0);
            SendMessage(hwndSearchInput, EM_SETLIMITTEXT, (WPARAM)LIMIT_USERNAME, 0);

            hfButton = SetFont(hdcMemBk, cyClient / 20 * 3 / 5,
                cyClient / 20 * 3 / 5 * 3 / 8,
                FONT_STYLE_DEFAULT + 300);
            hbrSearch = CreateSolidBrush(RGB(45, 180, 255));
            hbrSearch_hover = CreateSolidBrush(RGB(38, 170, 255));
            hbrButton1_hover = CreateSolidBrush(RGB(53, 210, 199));
            hbrButton2_hover = CreateSolidBrush(RGB(55, 222, 206));
            hbrButton3_hover = CreateSolidBrush(RGB(60, 230, 211));
            //CHECKBOX BUTTON
            if (ut == UT_SUPERADMIN)
            {
                SetRect(&rcCheckBox[0],
                    cxClient * 13 / 40, cyClient * 4 / 80,
                    cxClient * 16 / 40, cyClient * 10 / 80);
                FillRect(hdcMemBk, &rcCheckBox[0], (HBRUSH)GetStockObject(WHITE_BRUSH));
                hwndCheckBox[0] = CreateWindow(DIY_CHECKBOX,
                    TEXT("��ͨ�û�"),
                    WS_CHILD | WS_VISIBLE,
                    cxClient * 13 / 40, cyClient * 4 / 80 + (cyClient * 6 / 80 - cyClient / 30) / 2,
                    cyClient / 30, cyClient / 30,
                    hwnd, (HMENU)CHECKBOXID_ADMINUSERMANNAGE_USER, hInst, NULL);
                DrawText(hdcMemBk, TEXT("       �û�"), -1, &rcCheckBox[0], DT_SINGLELINE | DT_VCENTER);

                SetRect(&rcCheckBox[1],
                    cxClient * 16 / 40, cyClient * 4 / 80,
                    cxClient * 20 / 40, cyClient * 10 / 80);
                FillRect(hdcMemBk, &rcCheckBox[1], (HBRUSH)GetStockObject(WHITE_BRUSH));
                hwndCheckBox[1] = CreateWindow(DIY_CHECKBOX,
                    TEXT("����Ա"),
                    WS_CHILD | WS_VISIBLE,
                    cxClient * 16 / 40, cyClient * 4 / 80 + (cyClient * 6 / 80 - cyClient / 30) / 2,
                    cyClient / 30, cyClient / 30,
                    hwnd, (HMENU)CHECKBOXID_ADMINUSERMANNAGE_ADMIN, hInst, NULL);
                DrawText(hdcMemBk, TEXT("       ����Ա"), -1, &rcCheckBox[1], DT_SINGLELINE | DT_VCENTER);

                fChecked[0] = TRUE;
                fChecked[1] = FALSE;
                SendMessage(hwndCheckBox[0], DIY_CHECKBOX_SET, CHECKBOXID_ADMINUSERMANNAGE_USER, CHECKBOX_CHECKED);
                SendMessage(hwndCheckBox[1], DIY_CHECKBOX_SET, CHECKBOXID_ADMINUSERMANNAGE_ADMIN, CHECKBOX_UNCHECKED);
                SetRect(&rcSearch,
                    cxClient * 20 / 40, cyClient * 4 / 80,
                    cxClient * 24 / 40, cyClient * 10 / 80);

                SetRect(&rcAdd, cxClient * 25 / 40, cyClient * 4 / 80,
                    cxClient * 29 / 40, cyClient * 10 / 80);
                FillRect(hdcMemBk, &rcAdd, (HBRUSH)GetStockObject(WHITE_BRUSH));
                DrawText(hdcMemBk, TEXT("��ӹ���Ա"), -1, &rcAdd, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                SetRect(&rcAll, cxClient * 30 / 40, cyClient * 4 / 80,
                    cxClient * 34 / 40, cyClient * 10 / 80);
                FillRect(hdcMemBk, &rcAll, (HBRUSH)GetStockObject(WHITE_BRUSH));
                DrawText(hdcMemBk, TEXT("�г������û�"), -1, &rcAll, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                SetRect(&rcAll_admin, cxClient * 35 / 40, cyClient * 4 / 80,
                    cxClient * 39 / 40, cyClient * 10 / 80);
                FillRect(hdcMemBk, &rcAll_admin, (HBRUSH)GetStockObject(WHITE_BRUSH));
                DrawText(hdcMemBk, TEXT("�г����й���"), -1, &rcAll_admin, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                hwndAddAdmin = CreateWindow(szAddAdminName, NULL,
                    WS_CHILD, 0, 0, cxClient, cyClient, hwnd, (HMENU)ADMINUSERMANAGEID_ADDADMIN, hInst, NULL);
            }
            else
            {
                SetRect(&rcSearch,
                    cxClient * 13 / 40, cyClient * 4 / 80,
                    cxClient * 17 / 40, cyClient * 10 / 80);
                SetRect(&rcAll, cxClient * 35 / 40, cyClient * 4 / 80,
                    cxClient * 39 / 40, cyClient * 10 / 80);
                FillRect(hdcMemBk, &rcAll, (HBRUSH)GetStockObject(WHITE_BRUSH));
                DrawText(hdcMemBk, TEXT("�г������û�"), -1, &rcAll, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            }


            //��ʼ����ʽ
            FillRect(hdcMemBk, &rcSearch, hbrSearch);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            DrawText(hdcMemBk, TEXT("�����û�"), -1, &rcSearch, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

            //��ͷ
            SetRect(&rcSheetHead, cxClient * 1 / 40, cyClient * 11 / 80, cxClient * 39 / 40, cyClient * 15 / 80);
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            FillRect(hdcMemBk, &rcSheetHead, (HBRUSH)GetStockObject(WHITE_BRUSH));
            for (i = 0; i < 6; i++)
            {
                SetRect(&rcSheetHeadItem[i], rcSheetHead.left + (rcSheetHead.right - rcSheetHead.left) * i / 6,
                    rcSheetHead.top,
                    rcSheetHead.left + (rcSheetHead.right - rcSheetHead.left) * (i+1) / 6,
                    rcSheetHead.bottom);
                switch (i)
                {
                case 0:
                    lstrcpy(szBuf, TEXT("�û���"));
                    break;
                case 1:
                    lstrcpy(szBuf, TEXT("�û�����"));
                    break; 
                case 2:
                    lstrcpy(szBuf, TEXT("�û��ǳ�"));
                    break;
                case 3:
                    lstrcpy(szBuf, TEXT("�û�����"));
                    break;
                case 4:
                    lstrcpy(szBuf, TEXT("�û����֤"));
                    break;
                case 5:
                    lstrcpy(szBuf, TEXT("�û�������"));
                    break;
                }
                DrawText(hdcMemBk, szBuf, -1, &rcSheetHeadItem[i], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            }
            //��������
            hwndSearchRet = CreateWindow(szSearchRetName,
                TEXT("�������"), WS_CHILD | WS_VISIBLE,
                cxClient * 1 / 40, cyClient * 31 / 160,
                cxClient * 38 / 40, cyClient * 64 / 80,
                hwnd, (HMENU)ADMINUSERMANAGEID_SEARCHRET, hInst, NULL);
            //�����ͨ�û�����
            DataAccount_LinkedListMem(&pUaNode_all, UALIST_GET);
            fInited = TRUE;
        }
        return 0;

    case WM_U_SHOWSEARCHUI:
        //��Ҫ��������
        if (wParam == TRUE)
        {
            //����չʾ�����������Ϣ
            SendMessage(hwndSearchRet, WM_U_SHOWSEARCHRET, 0, 0);
        }
        return 0;
    case WM_LBUTTONUP:
        ptMouse.x = GET_X_LPARAM(lParam);
        ptMouse.y = GET_Y_LPARAM(lParam);
        //��ʾ�����û�
        if (PT_ON_RECT(ptMouse, rcAll))
        {
            i = 0;
            pUaNode = pUaNode_all;
            while (pUaNode != NULL)
            {
                pUaNodeArr[i] = pUaNode;
                pUaNode = pUaNode->next;
                i++;
            }
            DataAccountSearchRet(pUaNodeArr, UAL_SR_SET);
            SendMessage(hwndSearchRet, WM_U_SHOWSEARCHRET, 0, 0);
        }
        if (PT_ON_RECT(ptMouse, rcAll_admin) && ut == UT_SUPERADMIN)
        {
            i = 0;
            DataAccount_LinkedListMem(&pUaNode, UALIST_GET_ADMIN);
            while (pUaNode != NULL)
            {
                pUaNodeArr[i] = pUaNode;
                pUaNode = pUaNode->next;
                i++;
            }
            DataAccountSearchRet(pUaNodeArr, UAL_SR_SET);
            SendMessage(hwndSearchRet, WM_U_SHOWSEARCHRET, 0, 0);
        }
        //��ӹ���Ա
        if (PT_ON_RECT(ptMouse, rcAdd) && ut == UT_SUPERADMIN)
        {
            ShowWindow(hwndCheckBox[0], SW_HIDE);
            ShowWindow(hwndCheckBox[1], SW_HIDE);
            ShowWindow(hwndSearchInput, SW_HIDE);
            ShowWindow(hwndSearchRet, SW_HIDE);
            ShowWindow(hwndAddAdmin, SW_SHOW);
            UpdateWindow(hwndAddAdmin);
        }
        if (PT_ON_RECT(ptMouse, rcSearch))
        {
            GetWindowText(hwndSearchInput, szBuf, LIMIT_USERNAME);
            switch (ut)
            {
            case UT_ADMIN:
                pUaNode = pUaNode_all;
                while (pUaNode != NULL)
                {
                    if (lstrcmp(pUaNode->ua.szUsername, szBuf) == 0)
                    {
                        pUaNodeArr[0] = pUaNode;
                        break;
                    }
                    pUaNode = pUaNode->next;
                }
                DataAccountSearchRet(pUaNodeArr, UAL_SR_SET);
                SendMessage(hwndSearchRet, WM_U_SHOWSEARCHRET, 0, 0);
                return 0;
            case UT_SUPERADMIN:
                //�û�
                if (fChecked[0])
                {
                    pUaNode = pUaNode_all;
                    while (pUaNode != NULL)
                    {
                        if (lstrcmp(pUaNode->ua.szUsername, szBuf) == 0)
                        {
                            pUaNodeArr[0] = pUaNode;
                            break;
                        }
                        pUaNode = pUaNode->next;
                    }
                }
                //����
                if (fChecked[1])
                {
                    DataAccount_LinkedListMem(&pUaNode, UALIST_GET_ADMIN);
                    while (pUaNode != NULL)
                    {
                        if (lstrcmp(pUaNode->ua.szUsername, szBuf) == 0)
                        {
                            pUaNodeArr[0] = pUaNode;
                            break;
                        }
                        pUaNode = pUaNode->next;
                    }
                }
                if (!fChecked[0] && !fChecked[1])
                {
                    MessageBox(hwnd, TEXT("�����ٹ�ѡһ��Ŷ��"), TEXT("��ʾ"), MB_OK);
                    return 0;
                }
                DataAccountSearchRet(pUaNodeArr, UAL_SR_SET);
                SendMessage(hwndSearchRet, WM_U_SHOWSEARCHRET, 0, 0);
                return 0;
            }

        }
        if (PT_ON_RECT(ptMouse, rcCheckBox[0]))
        {
            SendMessage(hwndCheckBox[0], DIY_CHECKBOX_SET_AUTO, 0, 0);
        }
        if (PT_ON_RECT(ptMouse, rcCheckBox[1]))
        {
            SendMessage(hwndCheckBox[1], DIY_CHECKBOX_SET_AUTO, 0, 0);
        }
        return 0;
    case WM_MOUSEMOVE:
        ptMouse.x = GET_X_LPARAM(lParam);
        ptMouse.y = GET_Y_LPARAM(lParam);
        if (PT_ON_RECT(ptMouse, rcAdd) && ut == UT_SUPERADMIN)
        {
            FillRect(hdcMemBk, &rcAdd, hbrButton1_hover);
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            DrawText(hdcMemBk, TEXT("��ӹ���Ա"), -1, &rcAdd, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcAdd, FALSE);
            UpdateWindow(hwnd);
        }
        else
        {
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            FillRect(hdcMemBk, &rcAdd, (HBRUSH)GetStockObject(WHITE_BRUSH));
            DrawText(hdcMemBk, TEXT("��ӹ���Ա"), -1, &rcAdd, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcAdd, FALSE);
            UpdateWindow(hwnd);
        }
        if (PT_ON_RECT(ptMouse, rcAll))
        {
            FillRect(hdcMemBk, &rcAll, hbrButton2_hover);
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            DrawText(hdcMemBk, TEXT("�г������û�"), -1, &rcAll, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcAll, FALSE);
            UpdateWindow(hwnd);
        }
        else
        {
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            FillRect(hdcMemBk, &rcAll, (HBRUSH)GetStockObject(WHITE_BRUSH));
            DrawText(hdcMemBk, TEXT("�г������û�"), -1, &rcAll, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcAll, FALSE);
            UpdateWindow(hwnd);
        }
        if (PT_ON_RECT(ptMouse, rcSearch))
        {
            FillRect(hdcMemBk, &rcSearch, hbrSearch_hover);
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            DrawText(hdcMemBk, TEXT("�����û�"), -1, &rcSearch, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcSearch, FALSE);
            UpdateWindow(hwnd);
        }
        else
        {
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            FillRect(hdcMemBk, &rcSearch, hbrSearch);
            DrawText(hdcMemBk, TEXT("�����û�"), -1, &rcSearch, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcSearch, FALSE);
            UpdateWindow(hwnd);
        }
        if (PT_ON_RECT(ptMouse, rcAll_admin) && ut == UT_SUPERADMIN)
        {
            FillRect(hdcMemBk, &rcAll_admin, hbrButton3_hover);
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            DrawText(hdcMemBk, TEXT("�г����й���"), -1, &rcAll_admin, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcAll_admin, FALSE);
            UpdateWindow(hwnd);
        }
        else
        {
            FillRect(hdcMemBk, &rcAll_admin, (HBRUSH)GetStockObject(WHITE_BRUSH));
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            DrawText(hdcMemBk, TEXT("�г����й���"), -1, &rcAll_admin, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcAll_admin, FALSE);
            UpdateWindow(hwnd);
        }
        return 0;

    case DIY_CHECKBOX_COMMAND:
        switch (wParam)
        {
        case CHECKBOXID_ADMINUSERMANNAGE_USER:
            if (lParam == CHECKBOX_CHECKED)
            {
                fChecked[0] = TRUE;
            }
            else
            {
                fChecked[0] = FALSE;
            }
            break;
        case CHECKBOXID_ADMINUSERMANNAGE_ADMIN:
            if (lParam == CHECKBOX_CHECKED)
            {
                fChecked[1] = TRUE;
            }
            else
            {
                fChecked[1] = FALSE;
            }
        }
        return 0;

    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;

    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfInput); DeleteObject(hfButton);
        DeleteObject(hbrSearch);DeleteObject(hbrSearch_hover);
        DeleteObject(hbrButton1_hover); DeleteObject(hbrButton2_hover);DeleteObject(hbrButton3_hover);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
/*������*/
static LRESULT CALLBACK AdminUserManage_SearchEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYUP:
        if (wParam == VK_RETURN)
        {
            SendMessage(hwndBodyOpt[1][2], WM_LBUTTONUP, MK_LBUTTON, MAKELONG((rcSearch.right + rcSearch.left) / 2, (rcSearch.bottom + rcSearch.top) / 2));
        }
        break;
    }
    return CallWindowProc(DefEditWndProc, hwnd, uMsg, wParam, lParam);
}
/*�������*/
static LRESULT CALLBACK AdminUserManage_SearchRetProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
        //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    //��������
    static HBRUSH hbrBk;
    static HFONT hfNotify;
    static HFONT hfTitle;
    TCHAR szBuf[32];
    //ITEM
#define ITEM_NUM    9
    static SIZE sizeItem;
    static RECT rcItem[ITEM_NUM];
    static RECT rcInnerItem[ITEM_NUM][6];
    static HFONT hfBig;
    static HFONT hfMedium;
    static HFONT hfSmall;
    static BOOL fItemHover[ITEM_NUM] = { FALSE };
    static HBRUSH hbrItemHover;
    static RECT rcClickToDetail[ITEM_NUM];   
    static int iNum_all, iNum_rest;
    static LPUSERACCOUNTNODE pUaNode_allArr[MAX_SEARCH_UA_ID];
    static LPUSERACCOUNTNODE pUaNode_nowArr[ITEM_NUM];
    LPUSERACCOUNTNODE pUaNode = NULL, pUaNode_pre = NULL;
    //PAGE NUM
    static RECT rcShowPageNum;
    static int nowPage;
    static int allPage;
    //BUTTON
    static RECT rcNext, rcPrevious;
    static HBRUSH hbrButton;
    static HBRUSH hbrButtonHover;
    static BOOL fButtonHover[2] = { FALSE };

    static BOOL fExisted = FALSE;
    static RECT rcDel[ITEM_NUM];
    static HBRUSH hbrDel;
    int i, j, k;
    size_t temp;

    POINT pt;
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
            sizeItem.cx = cxClient;
            sizeItem.cy = cyClient * 9 / 10 / ITEM_NUM;
            hfNotify = SetFont(hdcMemBk, sizeItem.cy / 2, sizeItem.cy / 2 * 3 / 8,
                FONT_STYLE_DEFAULT + 500);
            hbrItemHover = CreateSolidBrush(RGB(245, 245, 245));
            for (i = 0; i < ITEM_NUM; i++)
            {
                SetRect(&rcItem[i],
                    0, sizeItem.cy * i / 20 + sizeItem.cy * i,
                    cxClient, sizeItem.cy * i / 20 + sizeItem.cy * i + sizeItem.cy);
                for (j = 0; j < 6; j++)
                {
                    SetRect(&rcInnerItem[i][j],
                        rcItem[i].left + (rcItem[i].right - rcItem[i].left) * j / 6,
                        rcItem[i].top,
                        rcItem[i].left + (rcItem[i].right - rcItem[i].left) * (j + 1) / 6,
                        rcItem[i].bottom);
                }
                SetRect(&rcDel[i], rcInnerItem[i][5].left + (rcInnerItem[i][5].right - rcInnerItem[i][5].left) * 2 / 3,
                    rcItem[i].top,
                    rcItem[i].right,
                    rcItem[i].bottom);
            }
            hbrDel = CreateSolidBrush(RGB(255, 36, 36));
            hfTitle = SetFont(hdcMemBk, (rcClient.bottom - rcClient.top) * 1 / 12,
                (rcClient.bottom - rcClient.top) * 1 / 12 * 3 / 8,
                FONT_STYLE_DEFAULT + 400);
            SetTextColor(hdcMemBk, RGB(100, 100, 100));
            SelectObject(hdcMemBk, hfTitle);

            hfBig = SetFont(hdcMemBk, (rcItem[0].bottom - rcItem[0].top) * 1 / 2,
                (rcItem[0].bottom - rcItem[0].top) * 1 / 2 * 3 / 8,
                FONT_STYLE_DEFAULT);
            hfMedium = SetFont(hdcMemBk, (rcItem[0].bottom - rcItem[0].top) * 1 / 3,
                (rcItem[0].bottom - rcItem[0].top) * 1 / 3 * 3 / 8,
                FONT_STYLE_DEFAULT + 100);
            hfSmall = SetFont(hdcMemBk, (rcItem[0].bottom - rcItem[0].top) / 4,
                (rcItem[0].bottom - rcItem[0].top) / 4 * 3 / 8,
                FONT_STYLE_DEFAULT + 400);


            //BUTTON
            hbrButton = CreateSolidBrush(RGB(255, 255, 255));
            hbrButtonHover = CreateSolidBrush(RGB(0, 122, 204));
            SetRect(&rcPrevious, cxClient * 10 / 16, cyClient - (rcItem[0].bottom - rcItem[0].top) * 5 / 8,
                cxClient * 13 / 16 - 1, cyClient);
            SetRect(&rcNext, cxClient * 13 / 16 + 1, cyClient - (rcItem[0].bottom - rcItem[0].top) * 5 / 8,
                cxClient, cyClient);

            //PAGE NUM
            SetRect(&rcShowPageNum,
                rcPrevious.left - (rcPrevious.right - rcPrevious.left),
                rcPrevious.top,
                rcPrevious.left, rcPrevious.bottom);
            SelectObject(hdcMemBk, hfNotify);
            DrawText(hdcMemBk, TEXT("�����û�����"), -1, &rcClient, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

            fInited = TRUE;
        }
        return 0;
    case WM_U_SHOWSEARCHRET:
        //��ú��������������Ŀ
        iNum_rest = iNum_all = DataAccountSearchRet(pUaNode_allArr, FL_SR_GET);
        allPage = iNum_all / ITEM_NUM;
        nowPage = (iNum_all - iNum_rest) / ITEM_NUM;
        if (iNum_all % ITEM_NUM != 0) allPage++;
        if ((iNum_all - iNum_rest) % ITEM_NUM != 0 || iNum_rest != 0) nowPage++;
        //������
        if (iNum_all == 0)
        {
            RECT rcNotify = { 0,0,sizeItem.cx,sizeItem.cy };
            FillRect(hdcMemBk, &rcClient, hbrBk);
            FillRect(hdcMemBk, &rcNotify, (HBRUSH)GetStockObject(WHITE_BRUSH));
            SelectObject(hdcMemBk, hfNotify);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            DrawText(hdcMemBk, TEXT("δ���ҵ��κ����ݣ�"), -1, &rcNotify, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
            fExisted = FALSE;
        }
        //������
        else
        {
            fExisted = TRUE;
            for (i = 0; i < ITEM_NUM; i++)
                pUaNode_nowArr[i] = pUaNode_allArr[i];
            SelectObject(hdcMemBk, hfBig);
            FillRect(hdcMemBk, &rcClient, hbrBodyBk);
            FillRect(hdcMemBk, &rcPrevious, hbrButton);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            SetTextAlign(hdcMemBk, TA_LEFT);
            DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcPrevious, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            FillRect(hdcMemBk, &rcNext, hbrButton);
            DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcNext, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            SetTextColor(hdcMemBk, RGB(70, 70, 70));
            StringCchPrintf(szBuf, 32, TEXT("%d/%d"), nowPage, allPage);
            DrawText(hdcMemBk, szBuf, -1, &rcShowPageNum, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
 
            for (i = 0; i < min(iNum_rest, ITEM_NUM); i++)
            {
                FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));
                SelectObject(hdcMemBk, hfMedium);
                SetTextColor(hdcMemBk, RGB(70, 70, 70));
                DrawText(hdcMemBk, pUaNode_nowArr[i]->ua.szUsername,-1,&rcInnerItem[i][0],DT_SINGLELINE|DT_VCENTER|DT_CENTER);

                switch (pUaNode_nowArr[i]->ua.ut)
                {
                case UT_USER:
                    lstrcpy(szBuf, TEXT("��ͨ�û�"));
                    break;
                case UT_ADMIN:
                    lstrcpy(szBuf, TEXT("����Ա"));
                    break;
                case UT_SUPERADMIN:
                    lstrcpy(szBuf, TEXT("��������Ա"));
                    break;
                }
                DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][1], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_NICKNAME, &(pUaNode_nowArr[i]->ua));
                DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][2], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                DrawText(hdcMemBk, pUaNode_nowArr[i]->ua.szPassword, -1, &rcInnerItem[i][3], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_IDCARD, &(pUaNode_nowArr[i]->ua));
                DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][4], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                temp = DataAccount_GetOrderNum(&(pUaNode_nowArr[i]->ua));
                StringCchPrintf(szBuf, 32, TEXT("%d"), temp);
                DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][5], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            }
            //��ʾ����

        }
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_LBUTTONUP:
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        if (fExisted)
        {
            if (PT_ON_RECT(pt, rcNext) || PT_ON_RECT(pt, rcPrevious))
            {
                //��һҳ
                if (PT_ON_RECT(pt, rcPrevious))
                {
                    //��ҳ������
                    if (iNum_rest == iNum_all) return 0;
                    iNum_rest += ITEM_NUM;//��0

                    for (j = 0, i = iNum_all - iNum_rest;
                        i < min(iNum_all - iNum_rest + ITEM_NUM, MAX_SEARCH_FLIGHT_ID);
                        j++, i++)
                    {
                        pUaNode_nowArr[j] = pUaNode_allArr[i];//0Ҳ����
                    }
                    nowPage--;

                    //���������Ž��л��ƣ�
                    FillRect(hdcMemBk, &rcClient, hbrBodyBk);
                    SelectObject(hdcMemBk, hfBig);
                    SetTextAlign(hdcMemBk, TA_LEFT);

                    //ҳ��
                    SetTextColor(hdcMemBk, RGB(70, 70, 70));
                    StringCchPrintf(szBuf, 32, TEXT("%d/%d"), nowPage, allPage);
                    DrawText(hdcMemBk, szBuf, -1, &rcShowPageNum, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                    //��ť
                    FillRect(hdcMemBk, &rcPrevious, hbrButtonHover);
                    SetTextColor(hdcMemBk, RGB(255, 255, 255));
                    DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcPrevious, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                    FillRect(hdcMemBk, &rcNext, hbrButton);
                    SetTextColor(hdcMemBk, RGB(80, 80, 80));
                    DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcNext, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                    fButtonHover[0] = TRUE;

                    for (i = 0; i < min(iNum_rest, ITEM_NUM); i++)
                    {
                        FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));
                        SelectObject(hdcMemBk, hfMedium);
                        DrawText(hdcMemBk, pUaNode_nowArr[i]->ua.szUsername, -1, &rcInnerItem[i][0], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                        switch (pUaNode_nowArr[i]->ua.ut)
                        {
                        case UT_USER:
                            lstrcpy(szBuf, TEXT("��ͨ�û�"));
                            break;
                        case UT_ADMIN:
                            lstrcpy(szBuf, TEXT("����Ա"));
                            break;
                        case UT_SUPERADMIN:
                            lstrcpy(szBuf, TEXT("��������Ա"));
                            break;
                        }
                        DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][1], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                        DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_NICKNAME, &(pUaNode_nowArr[i]->ua));
                        DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][2], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                        DrawText(hdcMemBk, pUaNode_nowArr[i]->ua.szPassword, -1, &rcInnerItem[i][3], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                        DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_IDCARD, &(pUaNode_nowArr[i]->ua));
                        DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][4], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                        temp = DataAccount_GetOrderNum(&(pUaNode_nowArr[i]->ua));
                        StringCchPrintf(szBuf, 32, TEXT("%d"), temp);
                        DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][5], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                    }
                    InvalidateRect(hwnd, NULL, FALSE);
                    UpdateWindow(hwnd);


                }
                //��һҳ
                if (PT_ON_RECT(pt, rcNext))
                {

                    //�ڰ���ǰ
                    if (iNum_rest <= ITEM_NUM) return 0;
                    iNum_rest -= ITEM_NUM;//��ǰҳʣ�µĸ���

                    for (j = 0, i = iNum_all - iNum_rest;
                        i < min(iNum_all - iNum_rest + ITEM_NUM, MAX_SEARCH_FLIGHT_ID);
                        j++, i++)
                    {
                        pUaNode_nowArr[j] = pUaNode_allArr[i];//0Ҳ����
                    }
                    nowPage++;

                    //���������Ž��л��ƣ�
                    FillRect(hdcMemBk, &rcClient, hbrBodyBk);
                    SelectObject(hdcMemBk, hfBig);
                    SetTextAlign(hdcMemBk, TA_LEFT);
                    //ҳ��
                    SetTextColor(hdcMemBk, RGB(70, 70, 70));
                    StringCchPrintf(szBuf, 32, TEXT("%d/%d"), nowPage, allPage);
                    DrawText(hdcMemBk, szBuf, -1, &rcShowPageNum, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                    //��ť
                    FillRect(hdcMemBk, &rcNext, hbrButtonHover);
                    SetTextColor(hdcMemBk, RGB(255, 255, 255));
                    DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcNext, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                    FillRect(hdcMemBk, &rcPrevious, hbrButton);
                    SetTextColor(hdcMemBk, RGB(80, 80, 80));
                    DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcPrevious, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                    fButtonHover[1] = TRUE;


                    for (i = 0; i < min(iNum_rest, ITEM_NUM); i++)
                    {
                        FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));
                        SelectObject(hdcMemBk, hfMedium);
                        DrawText(hdcMemBk, pUaNode_nowArr[i]->ua.szUsername, -1, &rcInnerItem[i][0], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                        switch (pUaNode_nowArr[i]->ua.ut)
                        {
                        case UT_USER:
                            lstrcpy(szBuf, TEXT("��ͨ�û�"));
                            break;
                        case UT_ADMIN:
                            lstrcpy(szBuf, TEXT("����Ա"));
                            break;
                        case UT_SUPERADMIN:
                            lstrcpy(szBuf, TEXT("��������Ա"));
                            break;
                        }
                        DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][1], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                        DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_NICKNAME, &(pUaNode_nowArr[i]->ua));
                        DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][2], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                        DrawText(hdcMemBk, pUaNode_nowArr[i]->ua.szPassword, -1, &rcInnerItem[i][3], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                        DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_IDCARD, &(pUaNode_nowArr[i]->ua));
                        DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][4], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                        temp = DataAccount_GetOrderNum(&(pUaNode_nowArr[i]->ua));
                        StringCchPrintf(szBuf, 32, TEXT("%d"), temp);
                        DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][5], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                    }
                    InvalidateRect(hwnd, NULL, FALSE);
                    UpdateWindow(hwnd);

                }

            }
            else
            {

                for (i = 0; i < min(iNum_rest, ITEM_NUM); i++)
                {
                    //ɾ������
                    if (PT_ON_RECT(pt, rcDel[i]))
                    {

                        if (IDOK == MessageBox(hwnd, TEXT("ȷ��ɾ�����˲����޷�����"), TEXT("ɾ������"), MB_OKCANCEL))
                        {
                            StringCchPrintf(szBuf, 32, TEXT("����ɾ��%s���û�����"), pUaNode_nowArr[i]->ua.szUsername);
                            MessageBox(hwnd, szBuf, TEXT("ɾ���ɹ�"), MB_OK);
                            DataAccount_LinkedListMem(&pUaNode, UALIST_GET);
                            j = 0;
                            pUaNode_pre = pUaNode;
                            while (pUaNode != NULL)
                            {
                                if (lstrcmp(pUaNode->ua.szUsername, pUaNode_nowArr[i]->ua.szUsername) == 0)
                                {
                                    if (j == 0)
                                    {
                                        DataAccount_LinkedListMem(&(pUaNode->next), UALIST_CHANGE);
                                    }
                                    pUaNode_pre->next = pUaNode->next;
                                    free(pUaNode);
                                    break;
                                }
                                j++;
                                pUaNode_pre = pUaNode;
                                pUaNode = pUaNode->next;
                            }
                            DataAccount_LinkedListMem(&pUaNode, UALIST_GET_ADMIN);
                            j = 0;
                            pUaNode_pre = pUaNode;
                            while (pUaNode != NULL)
                            {
                                if (lstrcmp(pUaNode->ua.szUsername, pUaNode_nowArr[i]->ua.szUsername) == 0)
                                {
                                    if (j == 0)
                                    {
                                        DataAccount_LinkedListMem(&(pUaNode->next), UALIST_CHANGE);
                                    }
                                    pUaNode_pre->next = pUaNode->next;
                                    free(pUaNode);
                                    break;
                                }
                                j++;
                                pUaNode_pre = pUaNode;
                                pUaNode = pUaNode->next;
                            }

                        }
                        else {
                            return 0;
                            break;
                        }
                        //�������ID�ı�
                         //�������ID�ı�
                        for (j = 0; j < MAX_SEARCH_FLIGHT_ID; j++)
                        {
                            if (pUaNode_allArr[j] == NULL) break;
                            if (pUaNode_allArr[j] == pUaNode_nowArr[i])
                            {
                                for (k = j; k < MAX_SEARCH_FLIGHT_ID - 1; k++)
                                {
                                    if (pUaNode_allArr[k] == NULL) break;
                                    pUaNode_allArr[k] = pUaNode_allArr[k + 1];
                                }
                                break;
                            }
                        }
                        DataAccountSearchRet(pUaNode_allArr, UAL_SR_SET);
                        DataAccount_LinkedListFile(UALIST_TO_FILE);
                        DataAccount_LinkedListFile(UALIST_TO_FILE_ADMIN);
                        SendMessage(hwnd, WM_U_SHOWSEARCHRET, 0, 0);

                        break;
                    }
                }
            }
        }
        return 0;
    case WM_MOUSEMOVE:
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        if (fExisted)
        {
            //����
            for (i = 0; i < min(iNum_rest, ITEM_NUM); i++)
            {
                //����ڸ�Item��
                if (PT_ON_RECT(pt, rcItem[i]) && !fItemHover[i])
                {
                    FillRect(hdcMemBk, &rcItem[i], hbrItemHover);
                    SelectObject(hdcMemBk, hfMedium);
                    SetTextColor(hdcMemBk, RGB(70, 70, 70));
                    DrawText(hdcMemBk, pUaNode_nowArr[i]->ua.szUsername, -1, &rcInnerItem[i][0], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                    switch (pUaNode_nowArr[i]->ua.ut)
                    {
                    case UT_USER:
                        lstrcpy(szBuf, TEXT("��ͨ�û�"));
                        break;
                    case UT_ADMIN:
                        lstrcpy(szBuf, TEXT("����Ա"));
                        break;
                    case UT_SUPERADMIN:
                        lstrcpy(szBuf, TEXT("��������Ա"));
                        break;
                    }
                    DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][1], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                    DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_NICKNAME, &(pUaNode_nowArr[i]->ua));
                    DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][2], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                    DrawText(hdcMemBk, pUaNode_nowArr[i]->ua.szPassword, -1, &rcInnerItem[i][3], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                    DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_IDCARD, &(pUaNode_nowArr[i]->ua));
                    DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][4], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                    temp = DataAccount_GetOrderNum(&(pUaNode_nowArr[i]->ua));
                    StringCchPrintf(szBuf, 32, TEXT("%d"), temp);
                    DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][5], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                    FillRect(hdcMemBk, &rcDel[i], hbrDel);
                    SetTextColor(hdcMemBk, RGB(255, 255, 255));
                    DrawText(hdcMemBk, TEXT("ɾ��"), -1, &rcDel[i], DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                    InvalidateRect(hwnd, &rcItem[i], FALSE);
                    UpdateWindow(hwnd);
                    fItemHover[i] = TRUE;

                }
                //��겻�ڸ�Item��
                else if (!PT_ON_RECT(pt, rcItem[i]) && fItemHover[i]) {

                    FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));
                    SelectObject(hdcMemBk, hfMedium);
                    SetTextColor(hdcMemBk, RGB(70, 70, 70));
                    DrawText(hdcMemBk, pUaNode_nowArr[i]->ua.szUsername, -1, &rcInnerItem[i][0], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                    switch (pUaNode_nowArr[i]->ua.ut)
                    {
                    case UT_USER:
                        lstrcpy(szBuf, TEXT("��ͨ�û�"));
                        break;
                    case UT_ADMIN:
                        lstrcpy(szBuf, TEXT("����Ա"));
                        break;
                    case UT_SUPERADMIN:
                        lstrcpy(szBuf, TEXT("��������Ա"));
                        break;
                    }
                    DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][1], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                    DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_NICKNAME, &(pUaNode_nowArr[i]->ua));
                    DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][2], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                    DrawText(hdcMemBk, pUaNode_nowArr[i]->ua.szPassword, -1, &rcInnerItem[i][3], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

                    DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_IDCARD, &(pUaNode_nowArr[i]->ua));
                    DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][4], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                    temp = DataAccount_GetOrderNum(&(pUaNode_nowArr[i]->ua));
                    StringCchPrintf(szBuf, 32, TEXT("%d"), temp);
                    DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][5], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                    fItemHover[i] = FALSE;
                }
                InvalidateRect(hwnd, &rcItem[i], FALSE);
                UpdateWindow(hwnd);

            }

            if (PT_ON_RECT(pt, rcPrevious) && !fButtonHover[0])
            {
                FillRect(hdcMemBk, &rcPrevious, hbrButtonHover);
                SelectObject(hdcMemBk, hfBig);
                SetTextColor(hdcMemBk, RGB(255, 255, 255));
                SetTextAlign(hdcMemBk, TA_LEFT);
                DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcPrevious, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                fButtonHover[0] = TRUE;
                InvalidateRect(hwnd, &rcPrevious, FALSE);
                UpdateWindow(hwnd);
            }
            else if (!PT_ON_RECT(pt, rcPrevious) && fButtonHover[0])
            {
                FillRect(hdcMemBk, &rcPrevious, hbrButton);
                SelectObject(hdcMemBk, hfBig);
                SetTextColor(hdcMemBk, RGB(80, 80, 80));
                SetTextAlign(hdcMemBk, TA_LEFT);
                DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcPrevious, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                fButtonHover[0] = FALSE;
                InvalidateRect(hwnd, &rcPrevious, FALSE);
                UpdateWindow(hwnd);
            }

            if (PT_ON_RECT(pt, rcNext) && !fButtonHover[1])
            {
                FillRect(hdcMemBk, &rcNext, hbrButtonHover);
                SelectObject(hdcMemBk, hfBig);
                SetTextColor(hdcMemBk, RGB(255, 255, 255));
                SetTextAlign(hdcMemBk, TA_LEFT);
                DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcNext, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                fButtonHover[1] = TRUE;
                InvalidateRect(hwnd, &rcNext, FALSE);
                UpdateWindow(hwnd);
            }
            else if (!PT_ON_RECT(pt, rcNext) && fButtonHover[1])
            {
                FillRect(hdcMemBk, &rcNext, hbrButton);
                SelectObject(hdcMemBk, hfBig);
                SetTextColor(hdcMemBk, RGB(80, 80, 80));
                SetTextAlign(hdcMemBk, TA_LEFT);
                DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcNext, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                fButtonHover[1] = FALSE;
                InvalidateRect(hwnd, &rcNext, FALSE);
                UpdateWindow(hwnd);
            }
            {
                TRACKMOUSEEVENT tme = { sizeof(tme),TME_LEAVE,hwnd,10 };
                TrackMouseEvent(&tme);
            }
        }
        return 0;
    case WM_MOUSELEAVE:
        for (i = 0; i < min(iNum_rest, ITEM_NUM); i++)
        {
            //û��HOVERֱ������
            if (!fItemHover[i]) continue;
            FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));
            SelectObject(hdcMemBk, hfMedium);
            SetTextColor(hdcMemBk, RGB(70, 70, 70));
            DrawText(hdcMemBk, pUaNode_nowArr[i]->ua.szUsername, -1, &rcInnerItem[i][0], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

            switch (pUaNode_nowArr[i]->ua.ut)
            {
            case UT_USER:
                lstrcpy(szBuf, TEXT("��ͨ�û�"));
                break;
            case UT_ADMIN:
                lstrcpy(szBuf, TEXT("����Ա"));
                break;
            case UT_SUPERADMIN:
                lstrcpy(szBuf, TEXT("��������Ա"));
                break;
            }
            DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][1], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

            DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_NICKNAME, &(pUaNode_nowArr[i]->ua));
            DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][2], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

            DrawText(hdcMemBk, pUaNode_nowArr[i]->ua.szPassword, -1, &rcInnerItem[i][3], DT_SINGLELINE | DT_VCENTER | DT_CENTER);

            DataAccount_GetSelfInfoFromUA(szBuf, UA_GETINFO_IDCARD, &(pUaNode_nowArr[i]->ua));
            DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][4], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            temp = DataAccount_GetOrderNum(&(pUaNode_nowArr[i]->ua));
            StringCchPrintf(szBuf, 32, TEXT("%d"), temp);
            DrawText(hdcMemBk, szBuf, -1, &rcInnerItem[i][5], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            fItemHover[i] = FALSE;
            InvalidateRect(hwnd, &rcItem[i], FALSE);
            UpdateWindow(hwnd);
            fItemHover[i] = FALSE;
        }
        FillRect(hdcMemBk, &rcPrevious, hbrButton);
        SelectObject(hdcMemBk, hfBig);
        SetTextColor(hdcMemBk, RGB(80, 80, 80));
        SetTextAlign(hdcMemBk, TA_LEFT);
        DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcPrevious, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
        fButtonHover[0] = FALSE;
        InvalidateRect(hwnd, &rcPrevious, FALSE);
        FillRect(hdcMemBk, &rcNext, hbrButton);
        DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcNext, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
        fButtonHover[1] = FALSE;
        InvalidateRect(hwnd, &rcNext, FALSE);
        UpdateWindow(hwnd);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfNotify);
        DeleteObject(hfBig); DeleteObject(hfSmall);
        DeleteObject(hbrItemHover);
        DeleteObject(hbrButtonHover);
        DeleteObject(hbrButton);
        DeleteObject(hfTitle);
        DeleteObject(hbrDel);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);

    
}

/*��ӹ���Ա*/
static BOOL fTypeCheck[2];
static HWND hwndTypeCheck[2];
static HWND hwndEdit[4];
static HWND hwndBackButton, hwndClickButton;
//   Ŀ��: ������ӹ���Ա�����е���Ϣ��
static LRESULT CALLBACK AdminUserManage_AddAdmin(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //��������
    static HBRUSH hbrBk;
    //TITLE
    static RECT rcTitle;
    static HFONT hfTitle;
    //CONTENT
    static RECT rcContent;
    static HBRUSH hbrContent;
    static HFONT hfInput;
    static RECT rcCheckBox[2];
    static HFONT hfCheckBox;
    POINT ptMouse;
    int i;
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
            hwndBackButton = CreateWindow(szAddAdmin_BackButtonName,
                TEXT("���ذ�ť"),
                WS_CHILD | WS_VISIBLE,
                cxClient / 25, cxClient / 25,
                cxClient / 25, cxClient / 25,
                hwnd, (HMENU)ADMINUSERMANAGEID_ADDADMIN_BACKBUTTON, hInst, NULL);
            SetBkMode(hdcMemBk, TRANSPARENT);

            SetRect(&rcTitle, 0, cxClient / 25, cxClient, cxClient * 2 / 25);
            hfTitle = SetFont(hdcMemBk, cxClient / 25, cxClient / 25 * 3 / 8, FONT_STYLE_DEFAULT + 300);
            DrawText(hdcMemBk, TEXT("��ӹ���Ա"), -1, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            hwndClickButton = CreateWindow(szAddAdmin_AddButtonName,
                TEXT("��Ӱ�ť"),
                WS_CHILD | WS_VISIBLE,
                cxClient * 9 / 20, cyClient * 16 / 20,
                cxClient / 10, cyClient * 3 / 40,
                hwnd, (HMENU)ADMINUSERMANAGEID_ADDADMIN_ADDBUTTON, hInst, NULL);
            SetRect(&rcContent, cxClient / 4, cyClient / 6, cxClient * 3 / 4, cyClient * 9 / 10);
            FillRect(hdcMemBk, &rcContent, (HBRUSH)GetStockObject(WHITE_BRUSH));
            hfInput = SetFont(hdcMemBk, cyClient / 21, cyClient / 21 * 3 / 8, FONT_STYLE_DEFAULT);
            for (i = 0; i < 4; i++)
            {
                switch (i)
                {
                case 0:
                    lstrcpy(szBuf, TEXT("�û���"));
                    break;
                case 1:
                    lstrcpy(szBuf, TEXT("��  ��"));
                    break;
                case 2:
                    lstrcpy(szBuf, TEXT("��  ��"));
                    break;
                case 3:
                    lstrcpy(szBuf, TEXT("��  ��"));
                    break;
                }
                TextOut(hdcMemBk, cxClient * 7 / 20, cyClient * 12 / 40 + cyClient * i / 10, szBuf, lstrlen(szBuf));
                hwndEdit[i] = CreateWindow(TEXT("EDIT"),
                    NULL, WS_CHILD | WS_VISIBLE,
                    cxClient * 17 / 40, cyClient * 12 / 40 + cyClient * i / 10,
                    cxClient * 9 / 40, cyClient / 20,
                    hwnd, (HMENU)i, hInst, NULL);
                SendMessage(hwndEdit[i], WM_SETFONT, (WPARAM)hfInput, 0);
                MoveToEx(hdcMemBk, cxClient * 17 / 40, cyClient * 12 / 40 + cyClient * i / 10 + cyClient / 19, NULL);
                LineTo(hdcMemBk, cxClient * 26 / 40, cyClient * 12 / 40 + cyClient * i / 10 + cyClient / 19);
            }
            SendMessage(hwndEdit[0], EM_SETLIMITTEXT, LIMIT_USERNAME, 0);
            SendMessage(hwndEdit[1], EM_SETLIMITTEXT, LIMIT_PASSWORD, 0);
            SendMessage(hwndEdit[2], EM_SETLIMITTEXT, LIMIT_NICKNAME, 0);
            SendMessage(hwndEdit[3], EM_SETLIMITTEXT, LIMIT_NAME, 0);
            hwndTypeCheck[0] = CreateWindow(DIY_CHECKBOX, TEXT("��ͨ����"), WS_CHILD | WS_VISIBLE,
                cxClient * 16 / 40, cyClient * 28 / 40,
                cxClient / 45, cxClient / 45,
                hwnd, (HMENU)CHECKBOXID_ADMINUSERMANAGE_ADDADMIN_ADMIN, hInst, NULL);
            hfCheckBox = SetFont(hdcMemBk, cxClient / 45, cxClient / 45 * 3 / 8, FONT_STYLE_DEFAULT);
            TextOut(hdcMemBk, cxClient * 17 / 40, cyClient * 28 / 40, TEXT("��ͨ����"), 4);
            SetRect(&rcCheckBox[0], cxClient * 16 / 40, cyClient * 28 / 40, cxClient / 2, cyClient * 28 / 40 + cxClient / 45);

            hwndTypeCheck[1] = CreateWindow(DIY_CHECKBOX, TEXT("��������"), WS_CHILD | WS_VISIBLE,
                cxClient * 21 / 40, cyClient * 28 / 40,
                cxClient / 45, cxClient / 45,
                hwnd, (HMENU)CHECKBOXID_ADMINUSERMANAGE_ADDADMIN_SUPERADMIN, hInst, NULL);
            hfCheckBox = SetFont(hdcMemBk, cxClient / 45, cxClient / 45 * 3 / 8, FONT_STYLE_DEFAULT);
            TextOut(hdcMemBk, cxClient * 22 / 40, cyClient * 28 / 40, TEXT("��������"), 4);
            SetRect(&rcCheckBox[1], cxClient * 21 / 40, cyClient * 28 / 40, cxClient * 24 / 40, cyClient * 28 / 40 + cxClient / 45);

            fTypeCheck[0] = TRUE;
            fTypeCheck[1] = FALSE;
            SendMessage(hwndTypeCheck[0], DIY_CHECKBOX_SET, 0, CHECKBOX_CHECKED);
            fInited = TRUE;
            //��ʼ������
        }
        return 0;

    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case DIY_CHECKBOX_COMMAND:
        switch (wParam)
        {
        case CHECKBOXID_ADMINUSERMANAGE_ADDADMIN_ADMIN:
            if (lParam == CHECKBOX_CHECKED)
            {
                fTypeCheck[0] = TRUE;
                if (fTypeCheck[1])
                {
                    fTypeCheck[1] = FALSE;
                    SendMessage(hwndTypeCheck[1], DIY_CHECKBOX_SET, 0, CHECKBOX_UNCHECKED);
                }
            }
            else
            {
                fTypeCheck[0] = FALSE;
            }
            break;
        case CHECKBOXID_ADMINUSERMANAGE_ADDADMIN_SUPERADMIN:
            if (lParam == CHECKBOX_CHECKED)
            {
                fTypeCheck[1] = TRUE;
                if (fTypeCheck[0])
                {
                    fTypeCheck[0] = FALSE;
                    SendMessage(hwndTypeCheck[0], DIY_CHECKBOX_SET, 0, CHECKBOX_UNCHECKED);
                }
            }
            else
            {
                fTypeCheck[1] = FALSE;
            }
        }
        return 0;
    case WM_LBUTTONUP:
        ptMouse.x = GET_X_LPARAM(lParam);
        ptMouse.y = GET_Y_LPARAM(lParam);
        if (PT_ON_RECT(ptMouse, rcCheckBox[0]))
        {
            fTypeCheck[0] = TRUE;
            fTypeCheck[1] = FALSE;
            SendMessage(hwndTypeCheck[0], DIY_CHECKBOX_SET, 0, CHECKBOX_CHECKED);
            SendMessage(hwndTypeCheck[1], DIY_CHECKBOX_SET, 0, CHECKBOX_UNCHECKED);
        }
        if (PT_ON_RECT(ptMouse, rcCheckBox[1]))
        {
            fTypeCheck[0] = FALSE;
            fTypeCheck[1] = TRUE;
            SendMessage(hwndTypeCheck[0], DIY_CHECKBOX_SET, 0, CHECKBOX_UNCHECKED);
            SendMessage(hwndTypeCheck[1], DIY_CHECKBOX_SET, 0, CHECKBOX_CHECKED);
        }
        return 0;

    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfTitle);
        DeleteObject(hfInput);
        DeleteObject(hfCheckBox);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//   Ŀ��: ������ӹ���Ա�����еķ��ذ�ť��Ϣ
static LRESULT CALLBACK AdminUserManage_AddAdmin_BackButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
    static IDDATA id;
    static BOOL fNew = FALSE;
    LPFLIGHTDATANODE pFlNode_pre = NULL, pFlNode_now = NULL;
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
        ShowWindow(hwndAddAdmin, SW_HIDE);
        ShowWindow(hwndCheckBox[0], SW_SHOW);
        ShowWindow(hwndCheckBox[1], SW_SHOW);
        ShowWindow(hwndSearchInput, SW_SHOW);
        ShowWindow(hwndSearchRet, SW_SHOW);
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
//   Ŀ��: ������ӹ���Ա�����е���Ӱ�ť��Ϣ
static LRESULT CALLBACK AdminUserManage_AddAdmin_AddButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[4][32] = { TEXT("\0") };
    TCHAR szBuf_[128] = TEXT("\0");
    //��������
    static HBRUSH hbrBk, hbrBk_hover, hbrBk_click;
    static HFONT hf;
    static BOOL fClickNow = FALSE;
    BOOL fValid = TRUE;
    int i;
    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            hbrBk = CreateSolidBrush(RGB(0, 122, 204));
            hbrBk_hover = CreateSolidBrush(RGB(0, 130, 210));
            hbrBk_click = CreateSolidBrush(RGB(0, 110, 190));
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk, &hbmpBk);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            hf = SetFont(hdcMemBk, cyClient / 2, cyClient / 2 * 3 / 8, YAHEI_FONT + 450);
            SelectObject(hdcMemBk, hf);
            DrawText(hdcMemBk, TEXT("ȷ�����"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
            fInited = TRUE;
        }
        return 0;
    case WM_LBUTTONDOWN:
        fClickNow = TRUE;
        FillRect(hdcMemBk, &rcClient, hbrBk_click);
        DrawText(hdcMemBk, TEXT("ȷ�����"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);

        return 0;
    case WM_LBUTTONUP:
        fClickNow = FALSE;
        FillRect(hdcMemBk, &rcClient, hbrBk_hover);
        DrawText(hdcMemBk, TEXT("ȷ�����"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        if (IDCANCEL == MessageBox(hwnd, TEXT("ȷ�������"), TEXT("����µĹ���Ա"), MB_OKCANCEL))
        {
            return 0;
        }
        GetWindowText(hwndEdit[0], szBuf[0], 32);//�û���  
        GetWindowText(hwndEdit[1], szBuf[1], 32);//����
        {
            USERACCOUNT ua;
            lstrcpy(ua.szUsername, szBuf[0]);
            lstrcpy(ua.szPassword, szBuf[1]);
            if (!DataUAValid(ua)) 
            {
                DataGetValidUA_UOPStd(szBuf_);
                if (IDOK == MessageBox(hwnd, szBuf_, TEXT("��ʾ��Ϣ"), MB_OK | MB_ICONWARNING))
                {
                    return 0;
                }
            }
        }
        
        GetWindowText(hwndEdit[2], szBuf[2], 32);//�ǳ�  
        GetWindowText(hwndEdit[3], szBuf[3], 32);//����
        if (!DataValidCharcterSelfInfo(szBuf[2], UA_SETINFO_NICKNAME) ||
            !DataValidCharcterSelfInfo(szBuf[3], UA_SETINFO_NAME))
        {
            DataGetValidUA_SelfInfoStd(szBuf_);
            if (IDOK == MessageBox(hwnd, szBuf_, TEXT("��ʾ��Ϣ"), MB_OK | MB_ICONWARNING))
            {
                return 0;
            }
        }
        if (!fTypeCheck[0] && !fTypeCheck[1])
        {
            if (IDOK == MessageBox(hwnd, TEXT("���ٹ�ѡһ�����Ա����Ŷ"), TEXT("��ʾ��Ϣ"), MB_OK | MB_ICONWARNING))
            {
                return 0;
            }
        }
        {
            //�����Ƿ�����ͬ���û�
            USERACCOUNT ua;
            lstrcpy(ua.szUsername, szBuf[0]);
            lstrcpy(ua.szPassword, szBuf[1]);
            if (UA_NOTFOUND != DataAccountSearch_File(ua))
            {
                if (IDOK == MessageBox(hwnd, TEXT("�Ѵ����û���һ�µ��û���"), TEXT("�½�����"), MB_OK))
                {
                    return 0;
                }
            }
        }
        
        //�������
        {
            LPUSERACCOUNTNODE pUaNode = NULL, pUaNode_new;
            DataAccount_LinkedListMem(&pUaNode, UALIST_GET_ADMIN);
            while (pUaNode->next != NULL)
            {
                pUaNode = pUaNode->next;
            }
            if ((pUaNode_new = (LPUSERACCOUNTNODE)malloc(sizeof(USERACCOUNTNODE))) == NULL)
            {
                ErrorReport(ERR_IN_AdminUserManage_AddAdmin_AddButtonProc | ERR_MEM_CREATE_FAIL);
                return 0;
            }
            lstrcpy(pUaNode_new->ua.szUsername, szBuf[0]);
            lstrcpy(pUaNode_new->ua.szPassword, szBuf[1]);
            if (fTypeCheck[0])
            {
                pUaNode_new->ua.ut = UT_ADMIN;
                i = 0;
            }
            else
            {
                pUaNode_new->ua.ut = UT_SUPERADMIN;
                i = 1;
            }
            StringCchPrintf(pUaNode_new->ua.szSelfInfo, LIMIT_SELFINFO, TEXT("%s,%s,%d"), szBuf[2], szBuf[3], i);
            pUaNode_new->next = NULL;
            pUaNode->next = pUaNode_new;
            DataAccount_LinkedListFile(UALIST_TO_FILE_ADMIN);//���������ļ�

        }
        if (IDOK == MessageBox(hwnd, TEXT("�����ɹ�"), TEXT("�����µĹ���Ա"), MB_OK))
        {
            SendMessage(hwndBackButton, WM_LBUTTONUP, 0, 0);
        }
        return 0;
    case WM_MOUSEMOVE:
    {
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, 10 };
        TrackMouseEvent(&tme);
    }
    if (!fClickNow)
    {
        FillRect(hdcMemBk, &rcClient, hbrBk_hover);
        DrawText(hdcMemBk, TEXT("ȷ�����"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
    }
    return 0;

    case WM_MOUSELEAVE:

        FillRect(hdcMemBk, &rcClient, hbrBk);
        DrawText(hdcMemBk, TEXT("ȷ�����"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
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
