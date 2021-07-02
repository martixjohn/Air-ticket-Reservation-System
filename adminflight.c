/*
*       adminflight.c
*       �������й�������غ���
*/
#include "bodyadmin.h"

//ע�ᴰ����
static BOOL AdminFlightRegsiter(HINSTANCE hInstance);
//������������
static TCHAR szSearchName[] = TEXT("�������ര��");
static TCHAR szSearch_SearchBarName[] = TEXT("��������--������");
static TCHAR szSearch_SearchBar_ChildBarName[] = TEXT("��������--������--�ӿ�");
static TCHAR szSearch_RetName[] = TEXT("��������--�������");
static TCHAR szDetailName[] = TEXT("�������鴰��");
static TCHAR szDetail_BackButtonName[] = TEXT("���ذ�ť");
static TCHAR szDetail_BookButtonName[] = TEXT("���İ�ť");

//���ڹ���
static LRESULT CALLBACK AdminFlight_SearchProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK AdminFlight_Search_SearchBarProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK AdminFlight_Search_SearchBar_ChildBarProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static WNDPROC DefEditProc;
static LRESULT CALLBACK AdminFlight_Search_SearchBar_ChildBar_EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK AdminFlight_Search_RetProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK AdminFlight_DetailProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK AdminFlight_Detail_BackButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK AdminFlight_Detail_ClickButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//Search
static HWND hwndSearch;
static HWND hwndSearchBar, hwndSearchRet;
static HWND hwndInput_destdepart[2];
static HWND hwndInput_date[2];
//Detail
static HWND hwndDetail;
//SearchButton
static RECT rcSearch;
//ָ���Ƿ���ȫ�����½��������ݣ�ע�ⴴ�������ΪFALSE
static BOOL fFinalCreateNew = FALSE;
/*
*   ����: AdminFlightRegsiter(HINSTANCE hInstance)
*
*   Ŀ��: ע��AdminFlight�Ӵ�����
*
*   ���룺hInstance        ʵ�����
*
*   ���أ�BOOL             �ж�ע���Ƿ�ɹ�
*/
static BOOL AdminFlightRegsiter(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    HBRUSH hBrush;
    /*Search*/
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = AdminFlight_SearchProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szSearchName;
    if (!RegisterClass(&wndclass)) return FALSE;
    //SearchBar
    wndclass.lpfnWndProc = AdminFlight_Search_SearchBarProc;
    wndclass.lpszClassName = szSearch_SearchBarName;
    if (!RegisterClass(&wndclass)) return FALSE;

    wndclass.lpfnWndProc = AdminFlight_Search_SearchBar_ChildBarProc;
    wndclass.lpszClassName = szSearch_SearchBar_ChildBarName;
    if (!RegisterClass(&wndclass)) return FALSE;

    wndclass.lpfnWndProc = AdminFlight_Search_RetProc;
    wndclass.lpszClassName = szSearch_RetName;
    if (!RegisterClass(&wndclass)) return FALSE;

    /*Detail*/
    wndclass.lpfnWndProc = AdminFlight_DetailProc;
    wndclass.lpszClassName = szDetailName;
    if (!RegisterClass(&wndclass)) return FALSE;

    wndclass.lpfnWndProc = AdminFlight_Detail_BackButtonProc;
    wndclass.lpszClassName = szDetail_BackButtonName;
    if (!RegisterClass(&wndclass)) return FALSE;

    wndclass.lpfnWndProc = AdminFlight_Detail_ClickButtonProc;
    wndclass.lpszClassName = szDetail_BookButtonName;
    if (!RegisterClass(&wndclass)) return FALSE;

    return TRUE;
}

/*
*   ����: AdminFlightProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ��������������ڵ���Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
LRESULT CALLBACK AdminFlightProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    //��������

    switch (uMsg)
    {
    case WM_CREATE:
        AdminFlightRegsiter(hInst);
        return 0;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            DefInitWindow(hwnd, cxClient, cyClient, hbrBodyBk, &rcClient, &hdcMemBk, &hbmpBk);
            hwndSearch = CreateWindow(szSearchName,
                TEXT("��������"),
                WS_CHILD | WS_VISIBLE,//Ĭ����ʾ
                0, 0,
                cxClient, cyClient,
                hwnd, (HMENU)ADMINFLIGHTID_SEARCH, hInst, NULL
            );
            hwndDetail = CreateWindow(szDetailName,
                TEXT("��������"),
                WS_CHILD,
                0, 0,
                cxClient, cyClient,
                hwnd, (HMENU)ADMINFLIGHTID_DETAIL, hInst, NULL
            );
            fInited = TRUE;
        }
        return 0;

    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        return 0;
    case WM_U_SHOWFLIGHTDETAIL:

        ShowWindow(hwndSearch, SW_HIDE);
        SendMessage(hwndDetail, uMsg, wParam, lParam);
        ShowWindow(hwndDetail, SW_SHOW);
        return 0;
    case WM_U_SHOWSEARCHUI:
        ShowWindow(hwndDetail, SW_HIDE);
        SendMessage(hwndSearch, uMsg, wParam, lParam);
        ShowWindow(hwndSearch, SW_SHOW);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

/*Search����*/
/*
*   ����: AdminFlight_SearchProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ��������������Ϣ�����ڹ��̵���Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
static LRESULT CALLBACK AdminFlight_SearchProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    //��������
    static RECT rcAdd, rcAll, rcDel;
    static HFONT hfButton;
    static HBRUSH hbrBk;
    POINT ptMouse;
    LPFLIGHTDATANODE pFlNode = NULL;
    IDDATA iddataAll[MAX_SEARCH_FLIGHT_ID] = { 0 };
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
            hwndSearchBar = CreateWindow(szSearch_SearchBarName,
                TEXT("������"), WS_CHILD | WS_VISIBLE,
                cyClient / 40, cyClient / 20,
                cxClient * 5 / 20, cyClient * 9 / 10,
                hwnd, (HMENU)ADMINFLIGHTID_SEARCH_SEARCHBAR, hInst, NULL);

            //BUTTON
            SetRect(&rcAdd, cxClient * 12 / 40, cyClient / 20,
                cxClient * 15 / 40, cyClient * 2 / 20);
            SetRect(&rcAll, cxClient * 16 / 40, cyClient / 20,
                cxClient * 21 / 40, cyClient * 2 / 20);
            SetRect(&rcDel, cxClient * 22 / 40, cyClient / 20,
                cxClient * 26 / 40, cyClient * 2 / 20);
            hfButton = SetFont(hdcMemBk, (rcAdd.bottom - rcAdd.top) * 3 / 5,
                (rcAdd.bottom - rcAdd.top) * 3 / 5 * 3 / 8,
                FONT_STYLE_DEFAULT + 300);
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            FillRect(hdcMemBk, &rcAdd, (HBRUSH)GetStockObject(WHITE_BRUSH));
            DrawText(hdcMemBk, TEXT("��Ӻ���"), -1, &rcAdd, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            FillRect(hdcMemBk, &rcAll, (HBRUSH)GetStockObject(WHITE_BRUSH));
            DrawText(hdcMemBk, TEXT("�г����к���"), -1, &rcAll, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            //�Ӵ���
            hwndSearchRet = CreateWindow(szSearch_RetName,
                TEXT("�������"), WS_CHILD | WS_VISIBLE,
                cxClient * 5 / 20 + cyClient * 1 / 20, cyClient * 3 / 20,
                cxClient * 28 / 40, cyClient * 8 / 10,
                hwnd, (HMENU)ADMINFLIGHTID_SEARCH_SEARCHBAR, hInst, NULL);

            fInited = TRUE;
        }
        return 0;
    case WM_U_SHOWDELSUCCESS:
        SetTextColor(hdcMemBk, RGB(250, 0, 0));
        DrawText(hdcMemBk, TEXT("ɾ���ɹ���"), -1, &rcDel, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
        InvalidateRect(hwnd, &rcDel, FALSE);
        UpdateWindow(hwnd);
        SetTimer(hwnd, 1, 1000, NULL);
        return 0;
    case WM_TIMER:
        switch (wParam)
        {
        case 1:
            FillRect(hdcMemBk, &rcDel, hbrBk);
            InvalidateRect(hwnd, &rcDel, FALSE);
            UpdateWindow(hwnd);
            KillTimer(hwnd, 1);
            break;

        default:
            break;
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
        //��ʾ���к�������
        if (PT_ON_RECT(ptMouse, rcAll))
        {
            //�г����к�������
            DataFlightLinkedListMem(&pFlNode, FLLIST_GET_HEADNODE);
            for (i = 0; i < MAX_SEARCH_FLIGHT_ID && pFlNode != NULL; i++)
            {
                iddataAll[i] = pFlNode->fl.id;
                pFlNode = pFlNode->next;
            }
            DataFlightSearchRet(iddataAll, FL_SR_SET);
            SendMessage(hwndSearchRet, WM_U_SHOWSEARCHRET, 0, 0);
        }
        //��Ӻ��࣬ע�����ID
        if (PT_ON_RECT(ptMouse, rcAdd))
        {
            fFinalCreateNew = FALSE;
            if (NULL == (pFlNode = DataFlight_CreateNULLFlight()))
            {
                if (IDOK == MessageBox(hwnd, TEXT("���������Ѵ�����"), TEXT("��ʾ"), MB_OK))
                {
                    return 0;
                }
            }
            else
            {
                SendMessage(hwndBodyOpt[1][1], WM_U_SHOWFLIGHTDETAIL, (WPARAM)(pFlNode->fl.id), 1);
            }
        }
        return 0;
    case WM_MOUSEMOVE:
        ptMouse.x = GET_X_LPARAM(lParam);
        ptMouse.y = GET_Y_LPARAM(lParam);
        if (PT_ON_RECT(ptMouse, rcAdd))
        {
            FillRect(hdcMemBk, &rcAdd, hbrMenuBk);
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            DrawText(hdcMemBk, TEXT("��Ӻ���"), -1, &rcAdd, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcAdd, FALSE);
            UpdateWindow(hwnd);
        }
        else
        {
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            FillRect(hdcMemBk, &rcAdd, (HBRUSH)GetStockObject(WHITE_BRUSH));
            DrawText(hdcMemBk, TEXT("��Ӻ���"), -1, &rcAdd, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcAdd, FALSE);
            UpdateWindow(hwnd);
        }
        if (PT_ON_RECT(ptMouse, rcAll))
        {
            FillRect(hdcMemBk, &rcAll, hbrMenuBk);
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            DrawText(hdcMemBk, TEXT("�г����к���"), -1, &rcAll, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcAll, FALSE);
            UpdateWindow(hwnd);
        }
        else
        {
            SelectObject(hdcMemBk, hfButton);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            FillRect(hdcMemBk, &rcAll, (HBRUSH)GetStockObject(WHITE_BRUSH));
            DrawText(hdcMemBk, TEXT("�г����к���"), -1, &rcAll, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcAll, FALSE);
            UpdateWindow(hwnd);
        }

        return 0;

    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;

    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfButton);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//  Ŀ��: ��������������Ϣ��
static LRESULT CALLBACK AdminFlight_Search_SearchBarProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //��������
    HBRUSH hbrBk;
    static HFONT hf;
    static HWND hwndChildBar[3];
    static HWND hwndCheckBox[2];
    static RECT rcCheckBoxTitle[2];
    static HFONT hfCheckBoxTitle;

    static HBRUSH hbrSearch_click;
    static RECT rcSearch_Bk;
    int i;
    static BOOL fHover = FALSE, fClick = FALSE;

    //���ݴ���
    static BOOL fChecked[2] = { TRUE };//Ĭ��ѡ��
    POINT pt;
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

            //��ϸ����
            for (i = 0; i < 3; i++)
            {
                hwndChildBar[i] = CreateWindow(szSearch_SearchBar_ChildBarName,
                    TEXT("��ϸ������"),
                    WS_CHILD | WS_VISIBLE,
                    cxClient * 1 / 10, cyClient * 3 / 20 + cyClient * i / 5,
                    cxClient * 8 / 10, cyClient * 1 / 5,
                    hwnd, (HMENU)i, hInst, NULL);

            }
            //CheckBox
            hwndCheckBox[0] = CreateWindow(DIY_CHECKBOX,
                TEXT("���ò�"),
                WS_CHILD | WS_VISIBLE,
                cxClient * 1 / 10, cyClient / 20, cxClient * 1 / 13, cxClient * 1 / 13,
                hwnd, (HMENU)CHECKBOXID_ADMINFLIGHT_SEARCH_TYPE1, hInst, NULL);
            hwndCheckBox[1] = CreateWindow(DIY_CHECKBOX,
                TEXT("�����/�����"),
                WS_CHILD | WS_VISIBLE,
                cxClient * 4 / 10, cyClient / 20, cxClient * 1 / 13, cxClient * 1 / 13,
                hwnd, (HMENU)CHECKBOXID_ADMINFLIGHT_SEARCH_TYPE2, hInst, NULL);
            //Ĭ��ѡ��
            SendMessage(hwndCheckBox[0], DIY_CHECKBOX_SET_AUTO, 0, 0);
            SendMessage(hwndCheckBox[1], DIY_CHECKBOX_SET_AUTO, 0, 0);
            SetRect(&rcCheckBoxTitle[0],
                cxClient / 5, cyClient / 20,
                cxClient / 5 + cxClient / 4, cyClient / 20 + cxClient * 1 / 13);
            SetRect(&rcCheckBoxTitle[1],
                cxClient * 1 / 2, cyClient / 20,
                cxClient * 3 / 5 + cxClient / 2, cyClient / 20 + cxClient * 1 / 13);
            hfCheckBoxTitle = SetFont(hdcMemBk,
                rcCheckBoxTitle[0].bottom - rcCheckBoxTitle[0].top,
                (rcCheckBoxTitle[0].bottom - rcCheckBoxTitle[0].top) * 3 / 8,
                FONT_STYLE_DEFAULT + 400);
            SetTextColor(hdcMemBk, RGB(88, 88, 88));
            DrawText(hdcMemBk, TEXT("���ò�"),
                -1, &rcCheckBoxTitle[0], DT_SINGLELINE | DT_VCENTER | DT_LEFT);
            DrawText(hdcMemBk, TEXT("�����/�����"),
                -1, &rcCheckBoxTitle[1], DT_SINGLELINE | DT_VCENTER | DT_LEFT);

            //Search
            hbrSearch_click = CreateSolidBrush(RGB(10, 65, 149));
            SetRect(&rcSearch_Bk,
                cxClient / 4, cyClient * 8 / 10,
                cxClient * 3 / 4, cyClient * 9 / 10);
            SetRect(&rcSearch,
                rcSearch_Bk.left + (rcSearch_Bk.bottom - rcSearch_Bk.top) / 10,
                rcSearch_Bk.top + (rcSearch_Bk.bottom - rcSearch_Bk.top) / 10,
                rcSearch_Bk.right - (rcSearch_Bk.bottom - rcSearch_Bk.top) / 10,
                rcSearch_Bk.bottom - (rcSearch_Bk.bottom - rcSearch_Bk.top) / 10);

            FillRect(hdcMemBk, &rcSearch_Bk, hbrMenuBk);
            FillRect(hdcMemBk, &rcSearch, (HBRUSH)GetStockObject(WHITE_BRUSH));
            SetTextColor(hdcMemBk, crMenuBk);
            hf = SetFont(hdcMemBk, (rcSearch_Bk.bottom - rcSearch_Bk.top) / 2,
                (rcSearch_Bk.bottom - rcSearch_Bk.top) / 2 * 3 / 8, FONT_STYLE_DEFAULT + 450);
            SelectObject(hdcMemBk, (HFONT)hf);
            DrawText(hdcMemBk, TEXT("����"),
                -1, &rcSearch_Bk, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            fInited = TRUE;
        }
        return 0;
    case DIY_CHECKBOX_COMMAND:

        if (lParam == CHECKBOX_CHECKED)
            fChecked[wParam] = TRUE;
        else fChecked[wParam] = FALSE;

        return 0;
    case WM_LBUTTONDOWN:
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        for (i = 0; i < 2; i++)
        {
            if (PT_ON_RECT(pt, rcCheckBoxTitle[i]))
            {
                SendMessage(hwndCheckBox[i], DIY_CHECKBOX_SET_AUTO, 0, 0);
            }
        }
        if (PT_ON_RECT(pt, rcSearch_Bk))
        {
            if (!fClick)
            {
                FillRect(hdcMemBk, &rcSearch_Bk, hbrSearch_click);
                SelectObject(hdcMemBk, (HFONT)hf);
                SetTextColor(hdcMemBk, RGB(255, 255, 255));
                DrawText(hdcMemBk, TEXT("����"),
                    -1, &rcSearch_Bk, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                InvalidateRect(hwnd, &rcSearch_Bk, FALSE);
                UpdateWindow(hwnd);
                fClick = TRUE;
            }

        }
        return 0;
    case WM_LBUTTONUP:
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        if (PT_ON_RECT(pt, rcSearch_Bk))
        {
            fClick = FALSE;
            FillRect(hdcMemBk, &rcSearch_Bk, hbrMenuBk);
            SelectObject(hdcMemBk, (HFONT)hf);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            DrawText(hdcMemBk, TEXT("����"),
                -1, &rcSearch_Bk, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcSearch_Bk, FALSE);
            UpdateWindow(hwnd);
            fHover = TRUE;

            /*����Ϊ���ݴ���*/
            {
                FL_SEARCHDATA fl_sd;
                IDDATA iddata[MAX_SEARCH_FLIGHT_ID] = { 0 };
                BOOL fValid = TRUE;
                GetWindowText(hwndInput_destdepart[0], fl_sd.szDepart, LIMIT_SZDEPARTDEST);
                GetWindowText(hwndInput_destdepart[1], fl_sd.szDest, LIMIT_SZDEPARTDEST);
                GetWindowText(hwndInput_date[0], fl_sd.szDepartMonth, 3);
                GetWindowText(hwndInput_date[1], fl_sd.szDepartDay, 3);
                fValid = FALSE;
                //����DepartDest
                if ((fl_sd.szDepart[0] != TEXT('\0') && fl_sd.szDest[0] != TEXT('\0')))
                {
                    //ֻ����DepartDest
                    if ((fl_sd.szDepartMonth[0] == TEXT('\0') && fl_sd.szDepartDay[0] == TEXT('\0')))
                    {
                        fl_sd.fl_searchoption = FL_SEARCHOPTION_DEPARTANDDEST;
                        fValid = TRUE;
                    }
                    //��������DepartTime
                    else
                    {

                        //��Ч
                        if (DataValidDateText(fl_sd.szDepartMonth, fl_sd.szDepartDay))
                        {
                            fl_sd.fl_searchoption = FL_SEARCHOPTION_DEPARTANDDEST |
                                FL_SEARCHOPTION_DEPARTTIME;
                            fValid = TRUE;
                        }

                    }
                }
                if (!fValid)
                {
                    if (IDOK == MessageBox(hwnd, TEXT("       ��������ȷ������Ŷ     "), TEXT("��ʾ"), MB_OK))
                    {
                        return 0;
                    }
                }
                else
                {
                    if (fChecked[0] && fChecked[1])
                    {
                        //��������
                        DataFlightSearch_ReturnIDDATA(fl_sd, iddata);
                    }
                    else if (fChecked[0])
                    {
                        fl_sd.fl_searchoption = fl_sd.fl_searchoption | FL_SEARCHOPTION_TYPE;
                        lstrcpy(fl_sd.szType, FLIGHT_TYPE1);
                        DataFlightSearch_ReturnIDDATA(fl_sd, iddata);
                    }
                    else if (fChecked[1])
                    {
                        fl_sd.fl_searchoption = fl_sd.fl_searchoption | FL_SEARCHOPTION_TYPE;
                        lstrcpy(fl_sd.szType, FLIGHT_TYPE2);
                        DataFlightSearch_ReturnIDDATA(fl_sd, iddata);
                    }
                    else
                    {
                        if (IDOK == MessageBox(hwnd, TEXT("       �빴ѡ����һ�������Ŷ     "), TEXT("��ʾ"), MB_OK))
                        {
                            return 0;
                        }
                    }
                    //�ݴ�����
                    DataFlightSearchRet(iddata, FL_SR_SET);
                    //����չʾ�����������Ϣ
                    SendMessage(hwndSearchRet, WM_U_SHOWSEARCHRET, 0, 0);
                }

            }
        }

        return 0;
    case WM_MOUSEMOVE:
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);

        if (PT_ON_RECT(pt, rcSearch_Bk))
        {
            if (!fHover && !fClick && wParam != VK_LBUTTON)
            {
                FillRect(hdcMemBk, &rcSearch_Bk, hbrMenuBk);
                SelectObject(hdcMemBk, (HFONT)hf);
                SetTextColor(hdcMemBk, RGB(255, 255, 255));
                DrawText(hdcMemBk, TEXT("����"),
                    -1, &rcSearch_Bk, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
                InvalidateRect(hwnd, &rcSearch_Bk, FALSE);
                UpdateWindow(hwnd);
                fHover = TRUE;
            }
        }
        else
        {

            FillRect(hdcMemBk, &rcSearch_Bk, hbrMenuBk);
            FillRect(hdcMemBk, &rcSearch, (HBRUSH)GetStockObject(WHITE_BRUSH));
            SetTextColor(hdcMemBk, crMenuBk);
            SelectObject(hdcMemBk, (HFONT)hf);
            DrawText(hdcMemBk, TEXT("����"),
                -1, &rcSearch_Bk, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            InvalidateRect(hwnd, &rcSearch_Bk, FALSE);
            UpdateWindow(hwnd);
            fHover = FALSE;
            fClick = FALSE;

        }


        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hbrSearch_click);
        DeleteObject(hfCheckBoxTitle);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
static LRESULT CALLBACK AdminFlight_Search_SearchBar_ChildBarProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    //����
    static BOOL fInited[3] = { FALSE };
    static int cxClient, cyClient;
    static HDC hdcMemBk[3];
    static HBITMAP hbmpBk[3];
    static RECT rcClient;
    //��������
    static HBRUSH hbrBk;
    //TITLE
    static RECT rcTitle;
    static TCHAR szTitle[3][32] =
    { TEXT("  ������"),TEXT("  Ŀ�ĵ�"),TEXT("  ����ʱ��(��ѡ)") };
    static HFONT hfTitle;
    static HFONT hfInput;
    static HFONT hfInput_date;
    //BAR
    static RECT rcBar;

    static RECT rcMonth, rcDay;
    static BOOL fFirstWnd = TRUE;
    int id = (int)(HMENU)GetWindowLongPtr(hwnd, GWL_ID);
    int i;
    HDC hdc;

    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;

        if (fFirstWnd)
        {
            SetRect(&rcTitle, cxClient / 10, 0, cxClient * 9 / 10, cyClient * 2 / 5);
            hfTitle = SetFont(hdcMemBk[id], (rcTitle.bottom - rcTitle.top) * 11 / 20,
                (rcTitle.bottom - rcTitle.top) * 11 / 20 * 3 / 8, FONT_STYLE_DEFAULT + 400);
            SetRect(&rcBar, cxClient / 10, rcTitle.bottom, cxClient * 9 / 10, cyClient * 7 / 10);
            SetRect(&rcMonth, rcBar.left + (rcBar.right - rcBar.left) / 4, rcBar.top,
                rcBar.left + (rcBar.right - rcBar.left) * 2 / 4, rcBar.bottom);
            SetRect(&rcDay, rcBar.left + (rcBar.right - rcBar.left) * 3 / 4, rcBar.top,
                rcBar.right, rcBar.bottom);
            hfInput = CreateFont((rcBar.bottom - rcBar.top) * 7 / 10,
                (rcBar.bottom - rcBar.top) * 7 / 10 * 3 / 8, 0, 0, 400,
                FALSE/*����б��*/, FALSE/*�����»���*/, FALSE/*����ɾ����*/,
                DEFAULT_CHARSET, //ʹ��Ĭ���ַ���
                OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS,
                PROOF_QUALITY, //Ĭ���������
                DEFAULT_PITCH,
                APP_DEFAULT_FONT //������
            );
            hfInput_date = CreateFont((rcBar.bottom - rcBar.top) * 9 / 10,
                (rcBar.bottom - rcBar.top) * 9 / 10 * 3 / 8, 0, 0, 400,
                FALSE/*����б��*/, FALSE/*�����»���*/, FALSE/*����ɾ����*/,
                DEFAULT_CHARSET, //ʹ��Ĭ���ַ���
                OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS,
                PROOF_QUALITY, //Ĭ���������
                DEFAULT_PITCH,
                APP_DEFAULT_FONT //������
            );
            fFirstWnd = FALSE;
        }
        if (fInited[id] == FALSE)
        {
            hbrBk = (HBRUSH)GetStockObject(WHITE_BRUSH);
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk[id], &hbmpBk[id]);
            //TITLE
            SelectObject(hdcMemBk[id], hfTitle);
            SetBkMode(hdcMemBk[id], TRANSPARENT);
            SetTextColor(hdcMemBk[id], RGB(83, 83, 83));
            DrawText(hdcMemBk[id], szTitle[id], -1, &rcTitle, DT_SINGLELINE | DT_VCENTER);

            //BAR
            FillRect(hdcMemBk[id], &rcBar, hbrMenuBk);

            if (id == 0 || id == 1)
            {
                hwndInput_destdepart[id] = CreateWindow(TEXT("edit"),
                    NULL, WS_CHILD | WS_VISIBLE,
                    rcBar.left, rcBar.top,
                    rcBar.right - rcBar.left, (rcBar.bottom - rcBar.top) * 19 / 20,
                    hwnd, (HMENU)id, hInst, NULL);
                SendMessage(hwndInput_destdepart[id], WM_SETFONT, (WPARAM)hfInput, 0);
                DefEditProc = (WNDPROC)SetWindowLongPtr(hwndInput_destdepart[id], GWL_WNDPROC,
                    (LONG)AdminFlight_Search_SearchBar_ChildBar_EditProc);
            }
            //id=2
            else
            {

                SetTextColor(hdcMemBk[2], RGB(255, 255, 255));
                hwndInput_date[0] = CreateWindow(TEXT("edit"),
                    NULL, WS_CHILD | WS_VISIBLE | ES_RIGHT,
                    rcBar.left, rcBar.top,
                    (rcBar.right - rcBar.left) / 4, (rcBar.bottom - rcBar.top) * 19 / 20,
                    hwnd, (HMENU)id, hInst, NULL);

                DrawText(hdcMemBk[2], TEXT("��"), -1, &rcMonth, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                hwndInput_date[1] = CreateWindow(TEXT("edit"),
                    NULL, WS_CHILD | WS_VISIBLE | ES_RIGHT,
                    rcBar.left + (rcBar.right - rcBar.left) * 2 / 4, rcBar.top,
                    (rcBar.right - rcBar.left) / 4, (rcBar.bottom - rcBar.top) * 19 / 20,
                    hwnd, (HMENU)id, hInst, NULL);
                DrawText(hdcMemBk[2], TEXT("��"), -1, &rcDay, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                for (i = 0; i < 2; i++)
                {
                    SendMessage(hwndInput_date[i], WM_SETFONT, (WPARAM)hfInput_date, 0);
                    SendMessage(hwndInput_date[i], EM_LIMITTEXT, 2, 0);
                    DefEditProc = (WNDPROC)SetWindowLongPtr(hwndInput_date[i], GWL_WNDPROC,
                        (LONG)AdminFlight_Search_SearchBar_ChildBar_EditProc);
                }


            }
            fInited[id] = TRUE;
        }

        return 0;
    case CTLCOLOR_EDIT:
        hdc = (HDC)wParam;
        return (LRESULT)(HBRUSH)GetStockObject(WHITE_BRUSH);
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk[id]);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk[id], hbmpBk[id]);
        DeleteObject(hfTitle);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}
static LRESULT CALLBACK AdminFlight_Search_SearchBar_ChildBar_EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int id = (INT)GetWindowLongPtr(hwnd, GWL_ID);
    switch (uMsg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            SendMessage(hwndSearchBar, WM_LBUTTONUP,
                MK_LBUTTON,
                MAKELONG((rcSearch.right + rcSearch.left) / 2, (rcSearch.bottom + rcSearch.top) / 2));
            break;
        case VK_TAB:
            if (hwnd == hwndInput_destdepart[0])
            {
                SetFocus(hwndInput_destdepart[1]);
            }
            else if (hwnd == hwndInput_destdepart[1])
            {
                SetFocus(hwndInput_date[0]);
            }
            else if (hwnd == hwndInput_date[0])
            {
                SetFocus(hwndInput_date[1]);
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
//  Ŀ��: ���������������Ϣ��
static LRESULT CALLBACK AdminFlight_Search_RetProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    //��������
    static HBRUSH hbrBk;
    //���д���ѯ��ID
    static IDDATA iddataAll[MAX_SEARCH_FLIGHT_ID];
    static IDDATA_NUM iddata_num;

    static HFONT hfNotify;
    static HFONT hfTitle;
    TCHAR szBuf[32];
    static HICON hIcon;
    //ITEM
    LPFLIGHTDATANODE pFlNode;
    static SIZE sizeItem;
    static RECT rcItem[6];
    static RECT rcCompany[6];
    static HFONT hfBig;
    static HFONT hfSmall;
    static RECT rcDestAirport_Time[6], rcDepartAirport_Time[6];
    static RECT rcPrice_Type[6];
    static BOOL fItemHover[6] = { FALSE };
    static HBRUSH hbrItemHover;
    static RECT rcClickToDetail[6];
    //PAGE NUM
    static RECT rcShowPageNum;
    static int nowPage;
    static int allPage;
    //BUTTON
    static RECT rcNext, rcPrevious;
    static HBRUSH hbrButton;
    static HBRUSH hbrButtonHover;
    static BOOL fButtonHover[2] = { FALSE };

    //��ǰҳ���IDDATA

    static IDDATA iddataNow[6] = { 0 };
    //ʣ���IDDATA��Ŀ����һ�� ���٣���һ�����ӣ���ʱ��һ���г��������±���iddata_num-iddata_num_rest
    static int iddata_num_rest;//�п���Ϊ��
    static BOOL fExisted = FALSE;
    static RECT rcDel[6];
    static HBRUSH hbrDel;
    int i, j, k;
    static BOOL fFirstDel = TRUE;
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
            sizeItem.cy = cyClient / 7;
            hfNotify = SetFont(hdcMemBk, sizeItem.cy / 4, sizeItem.cy / 4 * 3 / 8,
                FONT_STYLE_DEFAULT + 500);
            hbrItemHover = CreateSolidBrush(RGB(245, 245, 245));
            for (i = 0; i < 6; i++)
            {
                SetRect(&rcItem[i],
                    0, sizeItem.cy * i / 20 + sizeItem.cy * i,
                    cxClient, sizeItem.cy * i / 20 + sizeItem.cy * i + sizeItem.cy);

                SetRect(&rcCompany[i], rcItem[i].left, rcItem[i].top,
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) / 5, rcItem[i].bottom);
                SetRect(&rcDepartAirport_Time[i],
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) / 5,
                    rcItem[i].top,
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 9 / 20,
                    rcItem[i].bottom);
                SetRect(&rcDestAirport_Time[i],
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 11 / 20,
                    rcItem[i].top,
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 4 / 5,
                    rcItem[i].bottom);
                SetRect(&rcPrice_Type[i],
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 4 / 5,
                    rcItem[i].top,
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 5 / 5,
                    rcItem[i].bottom);
                SetRect(&rcDel[i],
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 9 / 10,
                    rcItem[i].top,
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 5 / 5,
                    rcItem[i].bottom);
                SetRect(&rcClickToDetail[i],
                    rcItem[i].left,
                    rcItem[i].top,
                    rcDel[i].left,
                    rcItem[i].bottom);
            }
            
            hbrDel = CreateSolidBrush(RGB(255, 36, 36));
            hfTitle = SetFont(hdcMemBk, (rcClient.bottom - rcClient.top) * 1 / 12,
                (rcClient.bottom - rcClient.top) * 1 / 12 * 3 / 8,
                FONT_STYLE_DEFAULT + 400);
            SetTextColor(hdcMemBk, RGB(100, 100, 100));
            SelectObject(hdcMemBk, hfTitle);
            hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_APP_ICON));
            DrawIcon(hdcMemBk, cxClient / 4 + 10, (rcClient.bottom - rcClient.top) * 11 / 32, hIcon);
            DrawText(hdcMemBk, TEXT("\n\n\n\n  �����Ʊ������"), -1, &rcClient, DT_CENTER);
            hfBig = SetFont(hdcMemBk, (rcCompany[0].bottom - rcCompany[0].top) * 1 / 3,
                (rcCompany[0].bottom - rcCompany[0].top) * 1 / 3 * 3 / 8,
                FONT_STYLE_DEFAULT + 400);
            hfSmall = SetFont(hdcMemBk, (rcCompany[0].bottom - rcCompany[0].top) / 4,
                (rcCompany[0].bottom - rcCompany[0].top) / 4 * 3 / 8,
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



            fInited = TRUE;
        }
        return 0;
    case WM_U_SHOWSEARCHRET:
        //��ú��������������Ŀ
        iddata_num_rest = iddata_num = DataFlightSearchRet(iddataAll, FL_SR_GET);
        allPage = iddata_num / 6;
        nowPage = (iddata_num - iddata_num_rest) / 6;
        if (iddata_num % 6 != 0) allPage++;
        if ((iddata_num - iddata_num_rest) % 6 != 0 || iddata_num_rest != 0) nowPage++;
        //������
        if (iddata_num == 0)
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
            for (i = 0; i < 6; i++)
                iddataNow[i] = iddataAll[i];//0Ҳд��
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
            //һ������ʾ6��������Ϣ
            for (i = 0; i < min(iddata_num, 6); i++)
            {
                pFlNode = DataFlight_GetPNode(iddataNow[i]);
                FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));

                SetTextAlign(hdcMemBk, TA_LEFT);
                //1
                DataFlight_GetCompanyName(szBuf, pFlNode->fl.szFlightID);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfBig);
                TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                    rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) / 5,
                    szBuf, lstrlen(szBuf));

                DataFlight_GetFlightIDName(szBuf, pFlNode->fl.szFlightID);
                SetTextColor(hdcMemBk, RGB(6, 89, 178));
                SelectObject(hdcMemBk, hfSmall);
                TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                    rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) * 7 / 12,
                    szBuf, lstrlen(szBuf));
                //2
                StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), pFlNode->fl.dtDepart.hr, pFlNode->fl.dtDepart.min);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfBig);
                TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                    rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) / 5,
                    szBuf, lstrlen(szBuf));
                StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), pFlNode->fl.dtDepart.month, pFlNode->fl.dtDepart.day);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfSmall);
                TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) * 5 / 10,
                    rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 17 / 60,
                    szBuf, lstrlen(szBuf));

               
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfSmall);
                TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                    rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 7 / 12,
                    pFlNode->fl.szDepart, lstrlen(pFlNode->fl.szDepart));

                //3
                MoveToEx(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 19 / 40,
                    (rcItem[i].bottom + rcItem[i].top) / 2, NULL);
                LineTo(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 21 / 40,
                    (rcItem[i].bottom + rcItem[i].top) / 2);
                LineTo(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 21 / 40 - 5,
                    (rcItem[i].bottom + rcItem[i].top) / 2 - 5);

                //4
                StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), pFlNode->fl.dtDest.hr, pFlNode->fl.dtDest.min);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfBig);
                TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                    rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) / 5,
                    szBuf, lstrlen(szBuf));
                StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), pFlNode->fl.dtDest.month, pFlNode->fl.dtDest.day);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfSmall);
                TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) * 5 / 10,
                    rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 17 / 60,
                    szBuf, lstrlen(szBuf));
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfSmall);
                TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                    rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                    pFlNode->fl.szDest, lstrlen(pFlNode->fl.szDest));

                //5
                SetTextAlign(hdcMemBk, TA_RIGHT);
                StringCchPrintf(szBuf, 32, TEXT("��%.0f"), pFlNode->fl.dPrice);
                SetTextColor(hdcMemBk, RGB(12, 104, 224));
                SelectObject(hdcMemBk, hfBig);
                TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                    rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) / 5,
                    szBuf, lstrlen(szBuf));

                lstrcpy(szBuf, pFlNode->fl.szType);
                SetTextColor(hdcMemBk, RGB(85, 85, 85));
                SelectObject(hdcMemBk, hfSmall);
                TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                    rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) * 7 / 12,
                    szBuf, lstrlen(szBuf));
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
                    if (iddata_num_rest == iddata_num) return 0;
                    iddata_num_rest += 6;//��0

                    for (j = 0, i = iddata_num - iddata_num_rest;
                        i < min(iddata_num - iddata_num_rest + 6, MAX_SEARCH_FLIGHT_ID);
                        j++, i++)
                    {
                        iddataNow[j] = iddataAll[i];//0Ҳ����
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
                    //չʾ6�����ݣ�����ID��iddataNow��iddata_num_rest
                    for (i = 0; i < min(iddata_num_rest, 6); i++)
                    {
                        pFlNode = DataFlight_GetPNode(iddataNow[i]);
                        FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));

                        SetTextAlign(hdcMemBk, TA_LEFT);
                        //1
                        DataFlight_GetCompanyName(szBuf, pFlNode->fl.szFlightID);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                            rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) / 5,
                            szBuf, lstrlen(szBuf));

                        DataFlight_GetFlightIDName(szBuf, pFlNode->fl.szFlightID);
                        SetTextColor(hdcMemBk, RGB(6, 89, 178));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                            rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));
                        //2
                        StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), pFlNode->fl.dtDepart.hr, pFlNode->fl.dtDepart.min);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                            rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) / 5,
                            szBuf, lstrlen(szBuf));
                        StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), pFlNode->fl.dtDepart.month, pFlNode->fl.dtDepart.day);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) * 5 / 10,
                            rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 17 / 60,
                            szBuf, lstrlen(szBuf));

                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                            rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 7 / 12,
                            pFlNode->fl.szDepart, lstrlen(pFlNode->fl.szDepart));

                        //3
                        MoveToEx(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 19 / 40,
                            (rcItem[i].bottom + rcItem[i].top) / 2, NULL);
                        LineTo(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 21 / 40,
                            (rcItem[i].bottom + rcItem[i].top) / 2);
                        LineTo(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 21 / 40 - 5,
                            (rcItem[i].bottom + rcItem[i].top) / 2 - 5);

                        //4
                        StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), pFlNode->fl.dtDest.hr, pFlNode->fl.dtDest.min);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) / 5,
                            szBuf, lstrlen(szBuf));
                        StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), pFlNode->fl.dtDest.month, pFlNode->fl.dtDest.day);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) * 5 / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 17 / 60,
                            szBuf, lstrlen(szBuf));

                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                            pFlNode->fl.szDest, lstrlen(pFlNode->fl.szDest));

                        //5
                        SetTextAlign(hdcMemBk, TA_RIGHT);
                        StringCchPrintf(szBuf, 32, TEXT("��%.0f"), pFlNode->fl.dPrice);
                        SetTextColor(hdcMemBk, RGB(12, 104, 224));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                            rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) / 5,
                            szBuf, lstrlen(szBuf));

                        lstrcpy(szBuf, pFlNode->fl.szType);
                        SetTextColor(hdcMemBk, RGB(85, 85, 85));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                            rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));


                    }
                    InvalidateRect(hwnd, NULL, FALSE);
                    UpdateWindow(hwnd);


                }
                //��һҳ
                if (PT_ON_RECT(pt, rcNext))
                {

                    //�ڰ���ǰ
                    if (iddata_num_rest <= 6) return 0;
                    iddata_num_rest -= 6;//��ǰҳʣ�µĸ���

                    for (j = 0, i = iddata_num - iddata_num_rest;
                        i < min(iddata_num - iddata_num_rest + 6, MAX_SEARCH_FLIGHT_ID);
                        j++, i++)
                    {
                        iddataNow[j] = iddataAll[i];//0Ҳ����
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

                    //չʾ��6�����ݣ�����ID��iddataNow��iddata_num_rest
                    for (i = 0; i < min(iddata_num_rest, 6); i++)
                    {
                        pFlNode = DataFlight_GetPNode(iddataNow[i]);
                        FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));
                        SetTextAlign(hdcMemBk, TA_LEFT);
                        //1
                        DataFlight_GetCompanyName(szBuf, pFlNode->fl.szFlightID);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                            rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) / 5,
                            szBuf, lstrlen(szBuf));

                        DataFlight_GetFlightIDName(szBuf, pFlNode->fl.szFlightID);
                        SetTextColor(hdcMemBk, RGB(6, 89, 178));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                            rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));
                        //2
                        StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), pFlNode->fl.dtDepart.hr, pFlNode->fl.dtDepart.min);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                            rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) / 5,
                            szBuf, lstrlen(szBuf));
                        StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), pFlNode->fl.dtDepart.month, pFlNode->fl.dtDepart.day);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) * 5 / 10,
                            rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 17 / 60,
                            szBuf, lstrlen(szBuf));

                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                            rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 7 / 12,
                            pFlNode->fl.szDepart, lstrlen(pFlNode->fl.szDepart));

                        //3
                        MoveToEx(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 19 / 40,
                            (rcItem[i].bottom + rcItem[i].top) / 2, NULL);
                        LineTo(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 21 / 40,
                            (rcItem[i].bottom + rcItem[i].top) / 2);
                        LineTo(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 21 / 40 - 5,
                            (rcItem[i].bottom + rcItem[i].top) / 2 - 5);

                        //4
                        StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), pFlNode->fl.dtDest.hr, pFlNode->fl.dtDest.min);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) / 5,
                            szBuf, lstrlen(szBuf));
                        StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), pFlNode->fl.dtDest.month, pFlNode->fl.dtDest.day);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) * 5 / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 17 / 60,
                            szBuf, lstrlen(szBuf));

                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                            pFlNode->fl.szDest, lstrlen(pFlNode->fl.szDest));

                        //5
                        SetTextAlign(hdcMemBk, TA_RIGHT);
                        StringCchPrintf(szBuf, 32, TEXT("��%.0f"), pFlNode->fl.dPrice);
                        SetTextColor(hdcMemBk, RGB(12, 104, 224));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                            rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) / 5,
                            szBuf, lstrlen(szBuf));

                        lstrcpy(szBuf, pFlNode->fl.szType);
                        SetTextColor(hdcMemBk, RGB(85, 85, 85));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                            rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));

                    }
                    InvalidateRect(hwnd, NULL, FALSE);
                    UpdateWindow(hwnd);

                }

            }
            else
            {
                
                for (i = 0; i < min(iddata_num_rest, 6); i++)
                {
                    //չʾ����
                    if (PT_ON_RECT(pt, rcClickToDetail[i]))
                    {
                        SendMessage(hwndDetail, WM_U_SHOWFLIGHTDETAIL, iddataNow[i], 0);
                        ShowWindow(hwndDetail, SW_SHOW);
                        ShowWindow(hwndSearch, SW_HIDE);

                        return 0;
                    }
                    //ɾ������
                    if (PT_ON_RECT(pt, rcDel[i]))
                    {
                        if (fFirstDel)
                        {
                            if (IDOK == MessageBox(hwnd, TEXT("ȷ��ɾ�����˲����޷�����"), TEXT("ɾ������"), MB_OKCANCEL))
                            {
                                LPFLIGHTDATANODE pFlNode_Mem = NULL, pFlNode_next = NULL, pFlNode_pre = NULL;
                                DataFlightLinkedListMem(&pFlNode_Mem, FLLIST_GET_HEADNODE);
                                //��ɾ���Ľڵ�
                                pFlNode_pre = pFlNode = DataFlight_GetPNode(iddataNow[i]);
                                j = 0;
                                while (pFlNode_Mem != NULL)
                                {
                                    //������һ���ڵ�
                                    pFlNode_next = pFlNode_Mem->next;
                                    if (pFlNode_Mem == pFlNode)
                                    {
                                        if (j == 0)
                                        {
                                            DataFlightLinkedListMem(&pFlNode_next, FLLIST_CHANGE_HEADNODE);
                                        }
                                        pFlNode_pre->next = pFlNode_next;
                                        free(pFlNode);
                                        break;
                                    }
                                    j++;
                                    pFlNode_pre = pFlNode_Mem;
                                    pFlNode_Mem = pFlNode_Mem->next;
                                }
                            }
                            else {
                                break;
                            }
                            
                            fFirstDel = FALSE;
                        }
                        else
                        {
                            LPFLIGHTDATANODE pFlNode_Mem = NULL, pFlNode_next = NULL, pFlNode_pre = NULL;
                            DataFlightLinkedListMem(&pFlNode_Mem, FLLIST_GET_HEADNODE);
                            //��ɾ���Ľڵ�
                            pFlNode_pre = pFlNode = DataFlight_GetPNode(iddataNow[i]);
                            j = 0;
                            while (pFlNode_Mem != NULL)
                            {
                                //������һ���ڵ�
                                pFlNode_next = pFlNode_Mem->next;
                                if (pFlNode_Mem == pFlNode)
                                {
                                    if (j == 0)
                                    {
                                        DataFlightLinkedListMem(&pFlNode_next, FLLIST_CHANGE_HEADNODE);
                                    }
                                    pFlNode_pre->next = pFlNode_next;
                                    free(pFlNode);
                                    break;
                                }
                                j++;
                                pFlNode_pre = pFlNode_Mem;
                                pFlNode_Mem = pFlNode_Mem->next;
                            }
                        }
                        //�������ID�ı�
                        for (j = 0; j < MAX_SEARCH_FLIGHT_ID; j++)
                        {
                            if (iddataAll[j] == 0) break;
                            if (iddataAll[j] == iddataNow[i])
                            {
                                for (k = j; k < MAX_SEARCH_FLIGHT_ID - 1; k++)
                                {
                                    if (iddataAll[k] == 0) break;
                                    iddataAll[k] = iddataAll[k + 1];
                                }
                                break;
                            }
                        }
                        DataFlightSearchRet(iddataAll, FL_SR_SET);
                        DataFlightLinkedListFile(FLLIST_TO_FILE);
                        SendMessage(hwnd, WM_U_SHOWSEARCHRET, 0, 0);
                        SendMessage(hwndSearch, WM_U_SHOWDELSUCCESS, 0, 0);
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
            for (i = 0; i < min(iddata_num_rest, 6); i++)
            {
                //����ڸ�Item��
                if (PT_ON_RECT(pt, rcItem[i]) && !fItemHover[i])
                {
                    //û��HOVERǰ����
                    pFlNode = DataFlight_GetPNode(iddataNow[i]);
                    FillRect(hdcMemBk, &rcItem[i], hbrItemHover);
                    SetTextAlign(hdcMemBk, TA_LEFT);
                    //1
                    
                    StringCchPrintf(szBuf, 32, TEXT("Ʊ����%d"), pFlNode->fl.iAmount);
                    SetTextColor(hdcMemBk, RGB(6, 89, 178));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                        rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) / 5,
                        szBuf, lstrlen(szBuf));

                    StringCchPrintf(szBuf, 32, TEXT("%s"), pFlNode->fl.szPlaneID);
                    SetTextColor(hdcMemBk, RGB(6, 89, 178));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                        rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) * 7 / 12,
                        szBuf, lstrlen(szBuf));
                    //2
                    StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), pFlNode->fl.dtDepart.hr, pFlNode->fl.dtDepart.min);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                        rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) / 5,
                        szBuf, lstrlen(szBuf));
                    StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), pFlNode->fl.dtDepart.month, pFlNode->fl.dtDepart.day);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) * 5 / 10,
                        rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 17 / 60,
                        szBuf, lstrlen(szBuf));

                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                        rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 7 / 12,
                        pFlNode->fl.szDepart, lstrlen(pFlNode->fl.szDepart));

                    //3
                    MoveToEx(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 19 / 40,
                        (rcItem[i].bottom + rcItem[i].top) / 2, NULL);
                    LineTo(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 21 / 40,
                        (rcItem[i].bottom + rcItem[i].top) / 2);
                    LineTo(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 21 / 40 - 5,
                        (rcItem[i].bottom + rcItem[i].top) / 2 - 5);

                    //4
                    StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), pFlNode->fl.dtDest.hr, pFlNode->fl.dtDest.min);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) / 5,
                        szBuf, lstrlen(szBuf));
                    StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), pFlNode->fl.dtDest.month, pFlNode->fl.dtDest.day);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) * 5 / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 17 / 60,
                        szBuf, lstrlen(szBuf));

                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                        pFlNode->fl.szDest, lstrlen(pFlNode->fl.szDest));

                    //5
                    SetTextColor(hdcMemBk, RGB(255, 255, 255));
                    SelectObject(hdcMemBk, hfBig);
                    FillRect(hdcMemBk, &rcDel[i], hbrDel);
                    DrawText(hdcMemBk, TEXT("ɾ��"), -1, &rcDel[i], DT_CENTER | DT_VCENTER | DT_SINGLELINE);


                    InvalidateRect(hwnd, &rcItem[i], FALSE);
                    UpdateWindow(hwnd);
                    fItemHover[i] = TRUE;


                }
                //��겻�ڸ�Item��
                else if (!PT_ON_RECT(pt, rcItem[i]) && fItemHover[i]) {
                    //û��HOVERֱ������
                    pFlNode = DataFlight_GetPNode(iddataNow[i]);
                    FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));
                    SetTextAlign(hdcMemBk, TA_LEFT);
                    //1
                    DataFlight_GetCompanyName(szBuf, pFlNode->fl.szFlightID);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                        rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) / 5,
                        szBuf, lstrlen(szBuf));

                    DataFlight_GetFlightIDName(szBuf, pFlNode->fl.szFlightID);
                    SetTextColor(hdcMemBk, RGB(6, 89, 178));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                        rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) * 7 / 12,
                        szBuf, lstrlen(szBuf));
                    //2
                    StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), pFlNode->fl.dtDepart.hr, pFlNode->fl.dtDepart.min);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                        rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) / 5,
                        szBuf, lstrlen(szBuf));
                    StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), pFlNode->fl.dtDepart.month, pFlNode->fl.dtDepart.day);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) * 5 / 10,
                        rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 17 / 60,
                        szBuf, lstrlen(szBuf));

                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                        rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 7 / 12,
                        pFlNode->fl.szDepart, lstrlen(pFlNode->fl.szDepart));

                    //3
                    MoveToEx(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 19 / 40,
                        (rcItem[i].bottom + rcItem[i].top) / 2, NULL);
                    LineTo(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 21 / 40,
                        (rcItem[i].bottom + rcItem[i].top) / 2);
                    LineTo(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 21 / 40 - 5,
                        (rcItem[i].bottom + rcItem[i].top) / 2 - 5);

                    //4
                    StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), pFlNode->fl.dtDest.hr, pFlNode->fl.dtDest.min);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) / 5,
                        szBuf, lstrlen(szBuf));
                    StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), pFlNode->fl.dtDest.month, pFlNode->fl.dtDest.day);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) * 5 / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 17 / 60,
                        szBuf, lstrlen(szBuf));
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                        pFlNode->fl.szDest, lstrlen(pFlNode->fl.szDest));

                    //5
                    SetTextAlign(hdcMemBk, TA_RIGHT);
                    StringCchPrintf(szBuf, 32, TEXT("��%.0f"), pFlNode->fl.dPrice);
                    SetTextColor(hdcMemBk, RGB(12, 104, 224));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                        rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) / 5,
                        szBuf, lstrlen(szBuf));

                    lstrcpy(szBuf, pFlNode->fl.szType);
                    SetTextColor(hdcMemBk, RGB(85, 85, 85));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                        rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) * 7 / 12,
                        szBuf, lstrlen(szBuf));


                    InvalidateRect(hwnd, &rcItem[i], FALSE);
                    UpdateWindow(hwnd);
                    fItemHover[i] = FALSE;
                }

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
        for (i = 0; i < min(iddata_num_rest, 6); i++)
        {
            //û��HOVERֱ������
            if (!fItemHover[i]) continue;
            pFlNode = DataFlight_GetPNode(iddataNow[i]);
            FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));
            SetTextAlign(hdcMemBk, TA_LEFT);
            //1
            DataFlight_GetCompanyName(szBuf, pFlNode->fl.szFlightID);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfBig);
            TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) / 5,
                szBuf, lstrlen(szBuf));

            DataFlight_GetFlightIDName(szBuf, pFlNode->fl.szFlightID);
            SetTextColor(hdcMemBk, RGB(6, 89, 178));
            SelectObject(hdcMemBk, hfSmall);
            TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) * 7 / 12,
                szBuf, lstrlen(szBuf));
            //2
            StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), pFlNode->fl.dtDepart.hr, pFlNode->fl.dtDepart.min);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfBig);
            TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) / 5,
                szBuf, lstrlen(szBuf));
            StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), pFlNode->fl.dtDepart.month, pFlNode->fl.dtDepart.day);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfSmall);
            TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) * 5 / 10,
                rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 17 / 60,
                szBuf, lstrlen(szBuf));

            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfSmall);
            TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 7 / 12,
                pFlNode->fl.szDepart, lstrlen(pFlNode->fl.szDepart));

            //3
            MoveToEx(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 19 / 40,
                (rcItem[i].bottom + rcItem[i].top) / 2, NULL);
            LineTo(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 21 / 40,
                (rcItem[i].bottom + rcItem[i].top) / 2);
            LineTo(hdcMemBk, rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 21 / 40 - 5,
                (rcItem[i].bottom + rcItem[i].top) / 2 - 5);

            //4
            StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), pFlNode->fl.dtDest.hr, pFlNode->fl.dtDest.min);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfBig);
            TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) / 5,
                szBuf, lstrlen(szBuf));
            StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), pFlNode->fl.dtDest.month, pFlNode->fl.dtDest.day);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfSmall);
            TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) * 5 / 10,
                rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 17 / 60,
                szBuf, lstrlen(szBuf));

            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfSmall);
            TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                pFlNode->fl.szDest, lstrlen(pFlNode->fl.szDest));

            //5
            SetTextAlign(hdcMemBk, TA_RIGHT);
            StringCchPrintf(szBuf, 32, TEXT("��%.0f"), pFlNode->fl.dPrice);
            SetTextColor(hdcMemBk, RGB(12, 104, 224));
            SelectObject(hdcMemBk, hfBig);
            TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) / 5,
                szBuf, lstrlen(szBuf));

            lstrcpy(szBuf, pFlNode->fl.szType);
            SetTextColor(hdcMemBk, RGB(85, 85, 85));
            SelectObject(hdcMemBk, hfSmall);
            TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) * 7 / 12,
                szBuf, lstrlen(szBuf));


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

/*Detail����*/
//���ھ��
static HWND hwndContent1_Edit[12], hwndContent2_Edit[5];
static HWND hwndCheckBox[2];
//CHECKBOX״̬
static BOOL fChecked[2] = { FALSE };

/*
*   ����: AdminFlight_DetailProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ������ϸ������Ϣ�����ڹ��̵���Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
static LRESULT CALLBACK AdminFlight_DetailProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //��������
    static HWND hwndBackButton, hwndClickButton;
    static HBRUSH hbrBk;
    //TITLE
    static RECT rcTitle;
    static HFONT hfTitle;
    static HBRUSH hbrTitle;
    //CONTENT
    static RECT rcContent;
    static HBRUSH hbrContent;

    static HFONT hfContent_title;
    static HFONT hfContent_content;
    static HFONT hfContent_input;
    static RECT rcContent1, rcContent2;
    static RECT rcContent1_ti[2], rcContent1_con[6];
    static RECT rcContent2_ti[2], rcContent2_con[6];

    static RECT rcCheckBox[2];
    /*con1   edit1        con2     edit2
        0   0               0
        1   1
        2   2   6   8   10
        3   3
        4   4
        5   5   7   9   11

    */
    //DATA
    static IDDATA id = 0;
    static LPFLIGHTDATANODE pFlNode = NULL;

    TCHAR szBuf1[128] = TEXT("\0");
    int i;
    SIZE sizeText;
    SIZE sizeRc;
    RECT rc;
    POINT ptMouse;

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
            hwndBackButton = CreateWindow(szDetail_BackButtonName,
                TEXT("���ذ�ť"),
                WS_CHILD | WS_VISIBLE,
                cxClient / 25, cxClient / 25,
                cxClient / 25, cxClient / 25,
                hwnd, (HMENU)ADMINFLIGHTID_DETAIL_BACKBUTTON, hInst, NULL
            );
            SetBkMode(hdcMemBk, TRANSPARENT);
            //TITLE
            SetRect(&rcTitle, cxClient / 10, cxClient / 25, cxClient, cxClient * 2 / 25);
            hfTitle = SetFont(hdcMemBk,
                (rcTitle.bottom - rcTitle.top) * 4 / 5, (rcTitle.bottom - rcTitle.top) * 4 / 5 * 3 / 8,
                FONT_STYLE_DEFAULT + 450);
            hbrTitle = hbrBk;
            //CONTENT
            //BK
            SetRect(&rcContent, cxClient / 40, cyClient * 3 / 20, cxClient * 39 / 40, cyClient * 18 / 20);
            hbrContent = (HBRUSH)GetStockObject(WHITE_BRUSH);
            //cont
            /*con1*/
            SetRect(&rcContent1,
                rcContent.left,
                rcContent.top + (rcContent.bottom - rcContent.top) / 10,
                (rcContent.left + rcContent.right) / 2 - (rcContent.right - rcContent.left) / 40,
                rcContent.bottom - (rcContent.bottom - rcContent.top) * 2 / 10);
            //title
            for (i = 0; i < 2; i++)
            {
                SetRect(&rcContent1_ti[i],
                    rcContent1.left,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 2,
                    rcContent1.left + (rcContent1.right - rcContent1.left) * 2 / 10,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * (i + 1) / 2
                );
            }
            hfContent_title = SetFont(hdcMemBk,
                (rcContent1_ti[0].right - rcContent1_ti[0].left) / 4,
                (rcContent1_ti[0].right - rcContent1_ti[0].left) / 4 * 3 / 8,
                YAHEI_FONT + 500);
            //con
            hfContent_content = SetFont(hdcMemBk,
                (rcContent1.bottom - rcContent1.top) / 7 * 9 / 20,
                (rcContent1.bottom - rcContent1.top) / 7 * 9 / 20 * 3 / 8,
                FONT_STYLE_DEFAULT);
            hfContent_input = SetFont(hdcMemBk,
                (rcContent1.bottom - rcContent1.top) / 6 * 8 / 20,
                (rcContent1.bottom - rcContent1.top) / 6 * 8 / 20 * 3 / 8,
                YAHEI_FONT + 250);
            for (i = 0; i < 6; i++)
            {
                SetRect(&rcContent1_con[i],
                    rcContent1_ti[0].right,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6,
                    rcContent1_ti[0].right + (rcContent1_ti[0].right - rcContent1_ti[0].left),
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * (i + 1) / 6
                );
                //conid
                switch (i)
                {
                case 0:case 1:case 3:case 4:
                    hwndContent1_Edit[i] = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE,
                        rcContent1_con[0].right + 5,

                        rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,

                        (rcContent1.right - rcContent1.left)
                        - (rcContent1_ti[0].right - rcContent1_ti[0].left) - (rcContent1_con[0].right - rcContent1_con[0].left),

                        (rcContent1_con[0].bottom - rcContent1_con[0].top) * 2 / 5,
                        hwnd, (HMENU)i, hInst, NULL);
                    SendMessage(hwndContent1_Edit[i], EM_SETLIMITTEXT, 15, 0);
                    break;
                case 2:
                    hwndContent1_Edit[2] = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_CENTER | ES_NUMBER,
                        rcContent1_con[0].right + 5,

                        rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,

                        (rcContent1_con[0].right - rcContent1_con[0].left) / 2,

                        (rcContent1_con[0].bottom - rcContent1_con[0].top) * 2 / 5,
                        hwnd, (HMENU)2, hInst, NULL);
                    hwndContent1_Edit[6] = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_CENTER | ES_NUMBER,
                        rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) * 4 / 5,

                        rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,

                        (rcContent1_con[0].right - rcContent1_con[0].left) / 2,

                        (rcContent1_con[0].bottom - rcContent1_con[0].top) * 2 / 5,
                        hwnd, (HMENU)6, hInst, NULL);
                    hwndContent1_Edit[8] = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_CENTER | ES_NUMBER,
                        rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) * 8 / 5,

                        rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,

                        (rcContent1_con[0].right - rcContent1_con[0].left) / 2,

                        (rcContent1_con[0].bottom - rcContent1_con[0].top) * 2 / 5,
                        hwnd, (HMENU)8, hInst, NULL);
                    hwndContent1_Edit[10] = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_CENTER | ES_NUMBER,
                        rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) * 12 / 5,

                        rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,

                        (rcContent1_con[0].right - rcContent1_con[0].left) / 2,

                        (rcContent1_con[0].bottom - rcContent1_con[0].top) * 2 / 5,
                        hwnd, (HMENU)10, hInst, NULL);
                    SendMessage(hwndContent1_Edit[2], EM_SETLIMITTEXT, 2, 0);
                    SendMessage(hwndContent1_Edit[6], EM_SETLIMITTEXT, 2, 0);
                    SendMessage(hwndContent1_Edit[8], EM_SETLIMITTEXT, 2, 0);
                    SendMessage(hwndContent1_Edit[10], EM_SETLIMITTEXT, 2, 0);
                    break;
                case 5:
                    hwndContent1_Edit[5] = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_CENTER | ES_NUMBER,
                        rcContent1_con[0].right + 5,

                        rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,

                        (rcContent1_con[0].right - rcContent1_con[0].left) / 2,

                        (rcContent1_con[0].bottom - rcContent1_con[0].top) * 2 / 5,
                        hwnd, (HMENU)2, hInst, NULL);
                    hwndContent1_Edit[7] = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_CENTER | ES_NUMBER,
                        rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) * 4 / 5,

                        rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,

                        (rcContent1_con[0].right - rcContent1_con[0].left) / 2,

                        (rcContent1_con[0].bottom - rcContent1_con[0].top) * 2 / 5,
                        hwnd, (HMENU)6, hInst, NULL);
                    hwndContent1_Edit[9] = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_CENTER | ES_NUMBER,
                        rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) * 8 / 5,

                        rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,

                        (rcContent1_con[0].right - rcContent1_con[0].left) / 2,

                        (rcContent1_con[0].bottom - rcContent1_con[0].top) * 2 / 5,
                        hwnd, (HMENU)8, hInst, NULL);
                    hwndContent1_Edit[11] = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_CENTER | ES_NUMBER,
                        rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) * 12 / 5,

                        rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,

                        (rcContent1_con[0].right - rcContent1_con[0].left) / 2,

                        (rcContent1_con[0].bottom - rcContent1_con[0].top) * 2 / 5,
                        hwnd, (HMENU)10, hInst, NULL);
                    SendMessage(hwndContent1_Edit[5], EM_SETLIMITTEXT, 2, 0);
                    SendMessage(hwndContent1_Edit[7], EM_SETLIMITTEXT, 2, 0);
                    SendMessage(hwndContent1_Edit[9], EM_SETLIMITTEXT, 2, 0);
                    SendMessage(hwndContent1_Edit[11], EM_SETLIMITTEXT, 2, 0);
                    break;
                }
                
            }
            for (i = 0; i < 12; i++)
                SendMessage(hwndContent1_Edit[i], WM_SETFONT, (WPARAM)hfContent_input, 0);

            /*con2*/
            SetRect(&rcContent2,
                (rcContent.right + rcContent.left) / 2,
                rcContent.top + (rcContent.bottom - rcContent.top) / 10,
                rcContent.right - (rcContent.right - rcContent.left) / 20,
                rcContent.bottom - (rcContent.bottom - rcContent.top) * 2 / 10);
            //title
            for (i = 0; i < 2; i++)
            {
                SetRect(&rcContent2_ti[i],
                    rcContent2.left,
                    rcContent2.top + (rcContent2.bottom - rcContent2.top) * i / 2,
                    rcContent2.left + (rcContent2.right - rcContent2.left) * 2 / 10,
                    rcContent2.top + (rcContent2.bottom - rcContent2.top) * (i + 1) / 2
                );
            }
            //con
            for (i = 0; i < 6; i++)
            {

                SetRect(&rcContent2_con[i],
                    rcContent2_ti[0].right,
                    rcContent2.top + (rcContent2.bottom - rcContent2.top) * i / 6,
                    rcContent2_ti[0].right + (rcContent2_ti[0].right - rcContent2_ti[0].left) * 9 / 10,
                    rcContent2.top + (rcContent2.bottom - rcContent2.top) * (i + 1) / 6
                );
                if( i != 5 )
                { 
                    hwndContent2_Edit[i] = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE,
                        rcContent2_con[0].right + 5,

                        rcContent2.top + (rcContent2.bottom - rcContent2.top) * i / 6 + (rcContent2_con[0].bottom - rcContent2_con[0].top) * 6 / 20,

                        (rcContent2.right - rcContent2.left)- (rcContent2_ti[0].right - rcContent2_ti[0].left) - (rcContent2_con[0].right - rcContent2_con[0].left),

                        (rcContent2_con[0].bottom - rcContent2_con[0].top) * 2 / 5,
                        hwnd, (HMENU)(20+i), hInst, NULL);
                }
                else {
                    hwndCheckBox[0] = CreateWindow(DIY_CHECKBOX,
                        TEXT("���òհ�ť"),
                        WS_CHILD | WS_VISIBLE,
                        rcContent2_con[0].right + (rcContent2_ti[0].right - rcContent2_ti[0].left) / 8,
                        rcContent2.top + (rcContent2.bottom - rcContent2.top) * i / 6 + (rcContent2_con[0].bottom - rcContent2_con[0].top) * 6 / 20, 
                        (rcContent2_con[0].bottom - rcContent2_con[0].top) * 2 / 5, (rcContent2_con[0].bottom - rcContent2_con[0].top) * 2 / 5,
                        hwnd, (HMENU)CHECKBOXID_ADMINFLIGHT_DETAIL_TYPE1, hInst, NULL);
                    hwndCheckBox[1] = CreateWindow(DIY_CHECKBOX,
                        TEXT("�����/����հ�ť"),
                        WS_CHILD | WS_VISIBLE,
                        rcContent2_con[0].right + (rcContent2_ti[0].right - rcContent2_ti[0].left) * 5 / 4,
                        rcContent2.top + (rcContent2.bottom - rcContent2.top) * i / 6 + (rcContent2_con[0].bottom - rcContent2_con[0].top) * 6 / 20,
                        (rcContent2_con[0].bottom - rcContent2_con[0].top) * 2 / 5, (rcContent2_con[0].bottom - rcContent2_con[0].top) * 2 / 5,
                        hwnd, (HMENU)CHECKBOXID_ADMINFLIGHT_DETAIL_TYPE2, hInst, NULL);
                    SetRect(&rcCheckBox[0], 
                        rcContent2_con[0].right + (rcContent2_ti[0].right - rcContent2_ti[0].left) / 8,
                        rcContent2.top + (rcContent2.bottom - rcContent2.top) * i / 6 + (rcContent2_con[0].bottom - rcContent2_con[0].top) * 6 / 20,
                        rcContent2_con[0].right + (rcContent2_ti[0].right - rcContent2_ti[0].left) * 6 / 5,
                        rcContent2.top + (rcContent2.bottom - rcContent2.top) * i / 6 + (rcContent2_con[0].bottom - rcContent2_con[0].top) * 7 / 10);
                    SetRect(&rcCheckBox[1], 
                        rcContent2_con[0].right + (rcContent2_ti[0].right - rcContent2_ti[0].left) * 5 / 4,
                        rcContent2.top + (rcContent2.bottom - rcContent2.top) * i / 6 + (rcContent2_con[0].bottom - rcContent2_con[0].top) * 6 / 20,
                        rcContent2.right,
                        rcContent2.top + (rcContent2.bottom - rcContent2.top) * i / 6 + (rcContent2_con[0].bottom - rcContent2_con[0].top) * 7 / 10);

                }
            }
            for (i = 0; i < 5; i++)
            {
                SendMessage(hwndContent2_Edit[i], WM_SETFONT, (WPARAM)hfContent_input, 0);
                if (i < 3)
                {
                    //���չ�˾��ţ��ɻ����
                    SendMessage(hwndContent2_Edit[i], EM_SETLIMITTEXT, 15, 0);
                }
                else
                {
                    //Ʊ��Ʊ��
                    SendMessage(hwndContent2_Edit[i], EM_SETLIMITTEXT, 7, 0);
                    
                }
            }
            SetWindowLongPtr(hwndContent2_Edit[4], GWL_STYLE, WS_CHILD | WS_VISIBLE | ES_NUMBER);
            hwndClickButton = CreateWindow(szDetail_BookButtonName,
                TEXT("�޸İ�ť"),
                WS_CHILD | WS_VISIBLE,
                rcContent2.left + (rcContent2.right - rcContent2.left) * 7 / 9,
                rcContent2.bottom + (rcContent2.right - rcContent2.left) / 20,
                (rcContent2.right - rcContent2.left) / 4,
                (rcContent2.right - rcContent2.left) / 8,
                hwnd, (HMENU)ADMINFLIGHTID_DETAIL_BOOKBUTTON, hInst, NULL
            );

            fInited = TRUE;
            //��ʼ������
        }
        return 0;

    case WM_U_SHOWFLIGHTDETAIL:
        id = (IDDATA)wParam;
        pFlNode = DataFlight_GetPNode(id);
        SendMessage(hwndBackButton, uMsg, wParam, lParam);
        SendMessage(hwndClickButton, uMsg, wParam, lParam);
        //TITLE
        FillRect(hdcMemBk, &rcTitle, hbrTitle);
        SelectObject(hdcMemBk, hfTitle);
        SetTextColor(hdcMemBk, RGB(83, 83, 83));

        if (lParam == 1) StringCchPrintf(szBuf, 128, TEXT("�½�����"));
        else StringCchPrintf(szBuf, 128, TEXT("�༭������Ϣ"));

        DrawText(hdcMemBk, szBuf, -1, &rcTitle, DT_SINGLELINE | DT_VCENTER);
        //CONTENT
        FillRect(hdcMemBk, &rcContent, hbrContent);
        //title
        SelectObject(hdcMemBk, hfContent_title);
        SetTextColor(hdcMemBk, RGB(83, 83, 83));
        DrawText(hdcMemBk, TEXT("������"), -1, &rcContent1_ti[0], DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        DrawText(hdcMemBk, TEXT("Ŀ�ĵ�"), -1, &rcContent1_ti[1], DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        DrawText(hdcMemBk, TEXT("������Ϣ"), -1, &rcContent2_ti[0], DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        DrawText(hdcMemBk, TEXT("��Ʊ��Ϣ"), -1, &rcContent2_ti[1], DT_CENTER | DT_SINGLELINE | DT_VCENTER);
        //content
        SelectObject(hdcMemBk, hfContent_content);
        SetTextColor(hdcMemBk, RGB(83, 83, 83));
        //CON1
        for (i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:
                lstrcpy(szBuf1, TEXT("��������"));
                DataFlight_GetCityName(szBuf, pFlNode->fl.szDepart);
                SetWindowText(hwndContent1_Edit[0], szBuf);
                break;
            case 1:
                lstrcpy(szBuf1, TEXT("��������"));
                DataFlight_GetAirportName(szBuf, pFlNode->fl.szDepart);
                SetWindowText(hwndContent1_Edit[1], szBuf);
                break;
            case 2:
                lstrcpy(szBuf1, TEXT("����ʱ��"));
                StringCchPrintf(szBuf, 3, TEXT("%d"), pFlNode->fl.dtDepart.month);
                SetWindowText(hwndContent1_Edit[2], szBuf);
                TextOut(hdcMemBk, rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) / 2,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,
                    TEXT("��"), 1);
                StringCchPrintf(szBuf, 3, TEXT("%d"), pFlNode->fl.dtDepart.day);
                SetWindowText(hwndContent1_Edit[6], szBuf);
                TextOut(hdcMemBk, rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) * 4 / 5 + (rcContent1_con[0].right - rcContent1_con[0].left) / 2,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,
                    TEXT("��"), 1);
                StringCchPrintf(szBuf, 3, TEXT("%02d"), pFlNode->fl.dtDepart.hr);
                SetWindowText(hwndContent1_Edit[8], szBuf);
                TextOut(hdcMemBk, rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) * 8 / 5 + (rcContent1_con[0].right - rcContent1_con[0].left) / 2,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,
                    TEXT("ʱ"), 1);
                StringCchPrintf(szBuf, 3, TEXT("%02d"), pFlNode->fl.dtDepart.min);
                SetWindowText(hwndContent1_Edit[10], szBuf);
                TextOut(hdcMemBk, rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) * 12 / 5 + (rcContent1_con[0].right - rcContent1_con[0].left) / 2,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,
                    TEXT("��"), 1);
                break;
            case 3:
                lstrcpy(szBuf1, TEXT("Ŀ�ĳ���"));
                DataFlight_GetCityName(szBuf, pFlNode->fl.szDest);
                SetWindowText(hwndContent1_Edit[3], szBuf);
                break;
            case 4:
                lstrcpy(szBuf1,TEXT("Ŀ�Ļ���"));
                DataFlight_GetAirportName(szBuf, pFlNode->fl.szDest);
                SetWindowText(hwndContent1_Edit[4], szBuf);
                break;
            case 5:
                lstrcpy(szBuf1, TEXT("Ԥ�Ƶ���ʱ��"));
                StringCchPrintf(szBuf, 3, TEXT("%d"), pFlNode->fl.dtDest.month);
                SetWindowText(hwndContent1_Edit[5], szBuf);
                TextOut(hdcMemBk, rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) / 2,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,
                    TEXT("��"), 1);
                StringCchPrintf(szBuf, 3, TEXT("%d"), pFlNode->fl.dtDest.day);
                SetWindowText(hwndContent1_Edit[7], szBuf);
                TextOut(hdcMemBk, rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) * 4 / 5 + (rcContent1_con[0].right - rcContent1_con[0].left) / 2,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,
                    TEXT("��"), 1);
                StringCchPrintf(szBuf, 3, TEXT("%02d"), pFlNode->fl.dtDest.hr);
                SetWindowText(hwndContent1_Edit[9], szBuf);
                TextOut(hdcMemBk, rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) * 8 / 5 + (rcContent1_con[0].right - rcContent1_con[0].left) / 2,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,
                    TEXT("ʱ"), 1);
                StringCchPrintf(szBuf, 3, TEXT("%02d"), pFlNode->fl.dtDest.min);
                SetWindowText(hwndContent1_Edit[11], szBuf);
                TextOut(hdcMemBk, rcContent1_con[0].right + 5 + (rcContent1_con[0].right - rcContent1_con[0].left) * 12 / 5 + (rcContent1_con[0].right - rcContent1_con[0].left) / 2,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 6 / 20,
                    TEXT("��"), 1);
                break;

            }
            //��ֱ������ʾ����
            GetTextExtentPoint32(hdcMemBk, szBuf1, lstrlen(szBuf1), &sizeText);
            sizeRc.cx = rcContent1_con[i].right - rcContent1_con[i].left;
            sizeRc.cy = (sizeText.cx % sizeRc.cx > 0) ?
                ((sizeText.cx / sizeRc.cx + 1) * sizeText.cy) :
                ((sizeText.cx / sizeRc.cx) * sizeText.cy);
            SetRect(&rc, rcContent1_con[i].left,
                rcContent1_con[i].top + (rcContent1_con[i].bottom - rcContent1_con[i].top - sizeRc.cy) / 2,
                rcContent1_con[i].right,
                rcContent1_con[i].top + (rcContent1_con[i].bottom - rcContent1_con[i].top - sizeRc.cy) / 2 + sizeRc.cy);
            DrawText(hdcMemBk, szBuf1, -1, &rc, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
            MoveToEx(hdcMemBk, rcContent1_con[0].right + 5,
                rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 4 / 5, NULL);
            LineTo(hdcMemBk, rcContent1.right,
                rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6 + (rcContent1_con[0].bottom - rcContent1_con[0].top) * 4 / 5);
        }
        //CON2
        for (i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:
                lstrcpy(szBuf1, TEXT("���չ�˾"));
                DataFlight_GetCompanyName(szBuf, pFlNode->fl.szFlightID);
                SetWindowText(hwndContent2_Edit[0], szBuf);
                break;
            case 1:
                lstrcpy(szBuf1, TEXT("������"));
                DataFlight_GetFlightIDName(szBuf, pFlNode->fl.szFlightID);
                SetWindowText(hwndContent2_Edit[1], szBuf);
                break;
            case 2:
                lstrcpy(szBuf1, TEXT("�ɻ����"));
                SetWindowText(hwndContent2_Edit[2], pFlNode->fl.szPlaneID);
                break;
            case 3:
                lstrcpy(szBuf1, TEXT("Ʊ��\n(1λС��)"));
                StringCchPrintf(szBuf, LIMIT_SZDEPARTDEST, TEXT("%.1lf"), pFlNode->fl.dPrice);
                SetWindowText(hwndContent2_Edit[3], szBuf);
                break;
            case 4:
                lstrcpy(szBuf1, TEXT("ʣ��Ʊ��"));
                StringCchPrintf(szBuf, LIMIT_SZDEPARTDEST, TEXT("%d"), pFlNode->fl.iAmount);
                SetWindowText(hwndContent2_Edit[4], szBuf);
                break;
            case 5:
                lstrcpy(szBuf1, TEXT("��������"));
                TextOut(hdcMemBk, rcCheckBox[0].left + (rcContent2_con[0].bottom - rcContent2_con[0].top) * 2 / 5 + 4,
                    rcCheckBox[0].top,
                    FLIGHT_TYPE1, 3);
                TextOut(hdcMemBk, rcCheckBox[1].left + (rcContent2_con[0].bottom - rcContent2_con[0].top) * 2 / 5 + 4,
                    rcCheckBox[1].top,
                    FLIGHT_TYPE2, 7);

                //��ѡ
                if (lstrcmp(TEXT("���ò�"), pFlNode->fl.szType) == 0)
                {
                    fChecked[0] = TRUE; 
                    fChecked[1] = FALSE;
                    SendMessage(hwndCheckBox[0], DIY_CHECKBOX_SET, CHECKBOXID_ADMINFLIGHT_DETAIL_TYPE1, CHECKBOX_CHECKED);
                    SendMessage(hwndCheckBox[1], DIY_CHECKBOX_SET, CHECKBOXID_ADMINFLIGHT_DETAIL_TYPE2, CHECKBOX_UNCHECKED);
                }
                else
                {
                    fChecked[0] = FALSE;
                    fChecked[1] = TRUE;
                    SendMessage(hwndCheckBox[0], DIY_CHECKBOX_SET, CHECKBOXID_ADMINFLIGHT_DETAIL_TYPE1, CHECKBOX_UNCHECKED);
                    SendMessage(hwndCheckBox[1], DIY_CHECKBOX_SET, CHECKBOXID_ADMINFLIGHT_DETAIL_TYPE2, CHECKBOX_CHECKED);
                }
                break;

            }
            //��ֱ������ʾ����
            GetTextExtentPoint32(hdcMemBk, szBuf1, lstrlen(szBuf1), &sizeText);
            sizeRc.cx = rcContent2_con[i].right - rcContent2_con[i].left;
            sizeRc.cy = (sizeText.cx % sizeRc.cx > 0) ?
                ((sizeText.cx / sizeRc.cx + 1) * sizeText.cy) :
                ((sizeText.cx / sizeRc.cx) * sizeText.cy);
            SetRect(&rc, rcContent2_con[i].left,
                rcContent2_con[i].top + (rcContent2_con[i].bottom - rcContent2_con[i].top - sizeRc.cy) / 2,
                rcContent2_con[i].right,
                rcContent2_con[i].top + (rcContent2_con[i].bottom - rcContent2_con[i].top - sizeRc.cy) / 2 + sizeRc.cy);
            DrawText(hdcMemBk, szBuf1, -1, &rc, DT_WORDBREAK | DT_EDITCONTROL);
            MoveToEx(hdcMemBk, rcContent2_con[0].right + 5,
                rcContent2.top + (rcContent2.bottom - rcContent2.top) * i / 6 + (rcContent2_con[0].bottom - rcContent2_con[0].top) * 4 / 5, NULL);
            LineTo(hdcMemBk, rcContent2.right,
                rcContent2.top + (rcContent2.bottom - rcContent2.top) * i / 6 + (rcContent2_con[0].bottom - rcContent2_con[0].top) * 4 / 5);
        }
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case DIY_CHECKBOX_COMMAND:
        
        switch(wParam)
        {
        case CHECKBOXID_ADMINFLIGHT_DETAIL_TYPE1:
            if (lParam == CHECKBOX_CHECKED)
            {
                fChecked[0] = TRUE;
                if (fChecked[1])
                {
                    SendMessage(hwndCheckBox[1], DIY_CHECKBOX_SET, 0, CHECKBOX_UNCHECKED);
                    fChecked[1] = FALSE;
                }
            }  
            break;
        case CHECKBOXID_ADMINFLIGHT_DETAIL_TYPE2:
            if (lParam == CHECKBOX_CHECKED)
            {
                fChecked[1] = TRUE;
                if (fChecked[0])
                {
                    SendMessage(hwndCheckBox[0], DIY_CHECKBOX_SET, 0, CHECKBOX_UNCHECKED);
                    fChecked[0] = FALSE;
                }
            }
        }
        return 0;
    case WM_LBUTTONUP:
        ptMouse.x = GET_X_LPARAM(lParam);
        ptMouse.y = GET_Y_LPARAM(lParam);
        if (PT_ON_RECT(ptMouse, rcCheckBox[0]))
        {
            fChecked[0] = TRUE;
            fChecked[1] = FALSE;
            SendMessage(hwndCheckBox[0], DIY_CHECKBOX_SET, 0, CHECKBOX_CHECKED);
            SendMessage(hwndCheckBox[1], DIY_CHECKBOX_SET, 0, CHECKBOX_UNCHECKED);
        }
        if (PT_ON_RECT(ptMouse, rcCheckBox[1]))
        {
            fChecked[0] = FALSE;
            fChecked[1] = TRUE;
            SendMessage(hwndCheckBox[0], DIY_CHECKBOX_SET, 0, CHECKBOX_UNCHECKED);
            SendMessage(hwndCheckBox[1], DIY_CHECKBOX_SET, 0, CHECKBOX_CHECKED);
        }
        return 0;

    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfTitle);
        DeleteObject(hfContent_input);
        DeleteObject(hfContent_title);
        DeleteObject(hfContent_content);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//  Ŀ��: �����ذ�ť����Ϣ��
static LRESULT CALLBACK AdminFlight_Detail_BackButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
    case WM_U_SHOWFLIGHTDETAIL:
        id = (IDDATA)wParam;
        if(lParam == 1)
            fNew = TRUE;
        else
            fNew = FALSE;


        return 0;
    case WM_LBUTTONUP:
        //�½�����
        if (fNew)
        {
            //����û�û�б༭�½�������Ϣ����������ڴ�
            if (!fFinalCreateNew)
            {
                DataFlightLinkedListMem(&pFlNode_now, FLLIST_GET_HEADNODE);
                pFlNode_pre = pFlNode_now;
                while (pFlNode_now != NULL)
                {
                    if (pFlNode_now->fl.id == id)
                    {
                        pFlNode_pre->next = pFlNode_now->next;
                        free(pFlNode_now);
                        break;
                    }
                    pFlNode_pre = pFlNode_now;
                    pFlNode_now = pFlNode_now->next;
                }
            }
            else
            {
                fFinalCreateNew = FALSE;
            }
        }

        ShowWindow(hwndDetail, SW_HIDE);
        ShowWindow(hwndSearch, SW_SHOW);
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
//  Ŀ��: ����ȷ�ϰ�ť����Ϣ��
static LRESULT CALLBACK AdminFlight_Detail_ClickButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[4][32] = { TEXT("\0") };
    TCHAR szBuf1[4][32] = { TEXT("\0") };
    TCHAR szBuf_[128] = TEXT("\0");
    //��������
    static HBRUSH hbrBk, hbrBk_hover, hbrBk_click;
    static HFONT hf;
    static BOOL fClickNow = FALSE;
    static IDDATA id;
    static LPFLIGHTDATANODE pFlNode;
    int i, j, k;
    BOOL fValid = TRUE;
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
            DrawText(hdcMemBk, TEXT("�޸ĺ���"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
            fInited = TRUE;
        }
        return 0;
    case WM_U_SHOWFLIGHTDETAIL:
        id = (IDDATA)wParam;
        pFlNode = DataFlight_GetPNode(id);
        return 0;
    case WM_LBUTTONDOWN:
        fClickNow = TRUE;
        FillRect(hdcMemBk, &rcClient, hbrBk_click);
        DrawText(hdcMemBk, TEXT("�޸ĺ���"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);

        return 0;
    case WM_LBUTTONUP:
        fClickNow = FALSE;
        FillRect(hdcMemBk, &rcClient, hbrBk_hover);
        DrawText(hdcMemBk, TEXT("�޸ĺ���"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        for (i = 0; i < 12; i++)
        {
            GetWindowText(hwndContent1_Edit[i], szBuf[0], 32);
            if (szBuf[0][0] == TEXT('\0'))
            {
                fValid = FALSE;
                break;
            }
        }
        for (i = 0; i < 5; i++)
        {
            GetWindowText(hwndContent2_Edit[i], szBuf[1], 32);
            if (szBuf[1][0] == TEXT('\0'))
            {
                fValid = FALSE;
                break;
            }
        }
        if (!fValid)
        {
            MessageBox(hwnd, TEXT("����������Ŷ��"), TEXT("��ʾ"), MB_OK);
            return 0;
        }
        if (IDCANCEL == MessageBox(hwnd, TEXT("ȷ�������޸ĵ���Ϣ��"), TEXT("�޸���ʾ"), MB_OKCANCEL))
        {
            return 0;
        }
        if (!fChecked[0] && !fChecked[1])
        {
            MessageBox(hwnd, TEXT("�빴ѡ����һ�������Ŷ��"), TEXT("��ʾ"), MB_OK);
            return 0;
        }
        //Depart
        //CITY NAME
        GetWindowText(hwndContent1_Edit[0], szBuf[0], 32);
        //AIRPORT
        GetWindowText(hwndContent1_Edit[1], szBuf[1], 32);
        //Dest
        //CITY NAME
        GetWindowText(hwndContent1_Edit[3], szBuf[2], 32);
        //AIRPORT
        GetWindowText(hwndContent1_Edit[4], szBuf[3], 32);
        
        if (!DataValidCharacterFlightIDOrDepartOrDest(szBuf[0]) || !DataValidCharacterFlightIDOrDepartOrDest(szBuf[1]) ||
            !DataValidCharacterFlightIDOrDepartOrDest(szBuf[2]) || !DataValidCharacterFlightIDOrDepartOrDest(szBuf[3]))
        {
            DataGetValidFlightInfoStd(szBuf_);
            MessageBox(hwnd, szBuf_, TEXT("��������ȷ������Ŷ��"), MB_OK);
            return 0;
        }
        StringCchPrintf(pFlNode->fl.szDepart, 32, TEXT("%s %s"), szBuf[0], szBuf[1]);
        StringCchPrintf(pFlNode->fl.szDest, 32, TEXT("%s %s"), szBuf[2], szBuf[3]);
        //DepartTime
        GetWindowText(hwndContent1_Edit[2], szBuf[0], 3);
        GetWindowText(hwndContent1_Edit[6], szBuf[1], 3);
        GetWindowText(hwndContent1_Edit[8], szBuf[2], 3);
        GetWindowText(hwndContent1_Edit[10], szBuf[3], 3);
        if (!DataValidDateText(szBuf[0], szBuf[1]) || atoi(szBuf[2]) > 23 || atoi(szBuf[3]) > 59)
        {
            DataGetValidFlightInfoStd(szBuf_);
            MessageBox(hwnd, szBuf_, TEXT("��������ȷ������Ŷ��"), MB_OK);
            return 0;
        }

        //DestTime
        GetWindowText(hwndContent1_Edit[5], szBuf1[0], 3);
        GetWindowText(hwndContent1_Edit[7], szBuf1[1], 3);
        GetWindowText(hwndContent1_Edit[9], szBuf1[2], 3);
        GetWindowText(hwndContent1_Edit[11], szBuf1[3], 3);
        if (!DataValidDateText(szBuf1[0], szBuf1[1]) || atoi(szBuf1[2]) > 23 || atoi(szBuf1[3]) > 59)
        {
            DataGetValidFlightInfoStd(szBuf_);
            MessageBox(hwnd, szBuf_, TEXT("��������ȷ������Ŷ��"), MB_OK);
            return 0;
        }
        //�Ƚ�ʱ��
        if (atoi(szBuf[0]) > atoi(szBuf1[0])) fValid = FALSE;
        if (atoi(szBuf[0]) == atoi(szBuf1[0]))
        {
            if (atoi(szBuf[1]) > atoi(szBuf1[1])) fValid = FALSE;
            if (atoi(szBuf[1]) == atoi(szBuf1[1]))
            {
                if (atoi(szBuf[2]) > atoi(szBuf1[2])) fValid = FALSE;
                if (atoi(szBuf[2]) == atoi(szBuf1[2]))
                {
                    if (atoi(szBuf[3]) >= atoi(szBuf1[3])) fValid = FALSE;
                }
            }
        }
        if (!fValid)
        {
            DataGetValidFlightInfoStd(szBuf_);
            MessageBox(hwnd, szBuf_, TEXT("��������ȷ������Ŷ��"), MB_OK);
            return 0;
        }
        pFlNode->fl.dtDepart.month = atoi(szBuf[0]);
        pFlNode->fl.dtDepart.day = atoi(szBuf[1]);
        pFlNode->fl.dtDepart.hr = atoi(szBuf[2]);
        pFlNode->fl.dtDepart.min = atoi(szBuf[3]);
        pFlNode->fl.dtDest.month = atoi(szBuf1[0]);
        pFlNode->fl.dtDest.day = atoi(szBuf1[1]);
        pFlNode->fl.dtDest.hr = atoi(szBuf1[2]);
        pFlNode->fl.dtDest.min = atoi(szBuf1[3]);
 

        //���չ�˾�����
        GetWindowText(hwndContent2_Edit[0], szBuf[0], 32);
        GetWindowText(hwndContent2_Edit[1], szBuf[1], 32);
        if (!DataValidCharacterFlightIDOrDepartOrDest(szBuf[0]) || !DataValidCharacterFlightIDOrDepartOrDest(szBuf[1]))
        {
            DataGetValidFlightInfoStd(szBuf_);
            MessageBox(hwnd, szBuf_, TEXT("��������ȷ������Ŷ��"), MB_OK);
            return 0;
        }
        StringCchPrintf(pFlNode->fl.szFlightID, 32, TEXT("%s %s"), szBuf[0], szBuf[1]);
        //�ɻ����
        GetWindowText(hwndContent2_Edit[2], szBuf[0], 32);
        lstrcpy(pFlNode->fl.szPlaneID, szBuf[0]);
        //Ʊ��,ע������\0һλ
        GetWindowText(hwndContent2_Edit[3], szBuf[1], 8);
        for (i = 0, j = 0, k = 8; i < 8 && szBuf[1][i] != TEXT('\0'); i++)
        {
            if (!((szBuf[1][i] >= TEXT('0') && szBuf[1][i] <= TEXT('9')) || szBuf[1][i] == TEXT('.')))
            {
                fValid = FALSE;
                break;
            }
            if (szBuf[1][i] == TEXT('.'))
            {
                j++;
                //С�������1 С�����ڵ�һλ
                if( j > 1 || i == 0)
                {
                    fValid = FALSE;
                    break;
                }
                //��¼С����λ��
                k = i;
            }
        }
        //С���������һλ  С�����λ������1
        if (i - k == 1 || i - k > 2) fValid = FALSE;
        //����5λ
        if (atoi(szBuf[1]) > 99999) fValid = FALSE;
        if (!fValid)
        {
            DataGetValidFlightInfoStd(szBuf_);
            MessageBox(hwnd, szBuf_, TEXT("��������ȷ������Ŷ��"), MB_OK);
            return 0;
        }
        pFlNode->fl.dPrice = atof(szBuf[1]);
        //Ʊ��
        GetWindowText(hwndContent2_Edit[4], szBuf[2], 8);
        pFlNode->fl.iAmount = atoi(szBuf[2]);
        if (fChecked[0])
        {
            lstrcpy(pFlNode->fl.szType, FLIGHT_TYPE1);
        }
        else
        {
            lstrcpy(pFlNode->fl.szType, FLIGHT_TYPE2);
        }
        MessageBox(hwnd, TEXT("�޸ĳɹ���"), TEXT("��ʾ��Ϣ"), MB_OK);
        //�ļ�����
        DataFlightLinkedListFile(FLLIST_TO_FILE);
        //������ʾ
        SendMessage(hwndSearchRet, WM_U_SHOWSEARCHRET, 0, 0);
        //�����½����࣬��ʾ�½��ɹ�
        fFinalCreateNew = TRUE;
        
        return 0;
    case WM_MOUSEMOVE:
    {
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, 10 };
        TrackMouseEvent(&tme);
    }
    if (!fClickNow)
    {
        FillRect(hdcMemBk, &rcClient, hbrBk_hover);
        DrawText(hdcMemBk, TEXT("�޸ĺ���"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
    }
    return 0;

    case WM_MOUSELEAVE:

        FillRect(hdcMemBk, &rcClient, hbrBk);
        DrawText(hdcMemBk, TEXT("�޸ĺ���"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
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
