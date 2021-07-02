/*
*       userbooking.c
*       �������ж���Ʊ������غ���
*/
#include "bodyuser.h"
//ע�ᴰ����
static BOOL UserBookingRegsiter(HINSTANCE hInstance);
//���ֶ���

static TCHAR szSearchName[] = TEXT("����Ʊ�����������ര��");
static TCHAR szSearch_SearchBarName[] = TEXT("��������--������");
static TCHAR szSearch_SearchBar_ChildBarName[] = TEXT("��������--������--�ӿ�");
static TCHAR szSearch_RetName[] = TEXT("��������--�������");
static TCHAR szDetailName[] = TEXT("����Ʊ--�������鴰��");
static TCHAR szDetail_BackButtonName[] = TEXT("���ذ�ť");
static TCHAR szDetail_BookButtonName[] = TEXT("���İ�ť");

//���ڹ���
static LRESULT CALLBACK UserBooking_SearchProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserBooking_Search_SearchBarProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserBooking_Search_SearchBar_ChildBarProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static WNDPROC DefEditProc;
static LRESULT CALLBACK UserBooking_Search_SearchBar_ChildBar_EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserBooking_Search_RetProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserBooking_DetailProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserBooking_Detail_BackButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK UserBooking_Detail_BookButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//���ھ��
//Search
static HWND hwndSearch;
static HWND hwndSearchBar, hwndSearchRet;
static HWND hwndInput_destdepart[2];
static HWND hwndInput_date[2];
HWND hwndDetail;
//SearchButton
static RECT rcSearch;


/*
*   ����: UserHomeRegister(HINSTANCE hInstance)
*
*   Ŀ��: ע��UserBooking�Ӵ�����
*
*   ���룺hInstance        ʵ�����
*
*   ���أ�BOOL             �ж�ע���Ƿ�ɹ�
*/
static BOOL UserBookingRegsiter(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    HBRUSH hBrush;
    /*Search*/
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = UserBooking_SearchProc;
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
    wndclass.lpfnWndProc = UserBooking_Search_SearchBarProc;
    wndclass.lpszClassName = szSearch_SearchBarName;
    if (!RegisterClass(&wndclass)) return FALSE;

    wndclass.lpfnWndProc = UserBooking_Search_SearchBar_ChildBarProc;
    wndclass.lpszClassName = szSearch_SearchBar_ChildBarName;
    if (!RegisterClass(&wndclass)) return FALSE;

    wndclass.lpfnWndProc = UserBooking_Search_RetProc;
    wndclass.lpszClassName = szSearch_RetName;
    if (!RegisterClass(&wndclass)) return FALSE;

    /*Detail*/
    wndclass.lpfnWndProc = UserBooking_DetailProc;
    wndclass.lpszClassName = szDetailName;
    if (!RegisterClass(&wndclass)) return FALSE;
    
    wndclass.lpfnWndProc = UserBooking_Detail_BackButtonProc;
    wndclass.lpszClassName = szDetail_BackButtonName;
    if (!RegisterClass(&wndclass)) return FALSE;

    wndclass.lpfnWndProc = UserBooking_Detail_BookButtonProc;
    wndclass.lpszClassName = szDetail_BookButtonName;
    if (!RegisterClass(&wndclass)) return FALSE;

    return TRUE;
}
//��������б�,��������
/*
*   ����: UserBookingProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ����UserBooking�е���Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
LRESULT CALLBACK UserBookingProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
        UserBookingRegsiter(hInst);
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
                hwnd, (HMENU)USERBOOKINGID_SEARCH, hInst, NULL
            );
            hwndDetail = CreateWindow(szDetailName,
                TEXT("��������"),
                WS_CHILD,
                0, 0,
                cxClient, cyClient,
                hwnd, (HMENU)USERBOOKINGID_DETAIL, hInst, NULL
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
//  Ŀ��: ���������������е���Ϣ��
static LRESULT CALLBACK UserBooking_SearchProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    //��������
    static RECT rcTitle_Ret;
    static HFONT hfTitle_Ret;
    HBRUSH hbrBk;
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
                hwnd, (HMENU)USERBOOKINGID_SEARCH_SEARCHBAR, hInst, NULL);

            SetRect(&rcTitle_Ret, cxClient * 5 / 20 + cyClient * 1 / 20, cyClient / 20,
                cxClient * 5 / 20 + cyClient * 3 / 4, cyClient * 3 / 20);
            hfTitle_Ret = SetFont(hdcMemBk, (rcTitle_Ret.bottom - rcTitle_Ret.top) / 3,
                (rcTitle_Ret.bottom - rcTitle_Ret.top) / 3 * 3 / 8,
                FONT_STYLE_DEFAULT + 500);
            SelectObject(hdcMemBk, hfTitle_Ret);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            DrawText(hdcMemBk, TEXT("�������"), -1, &rcTitle_Ret, DT_SINGLELINE | DT_VCENTER);
            hwndSearchRet = CreateWindow(szSearch_RetName,
                TEXT("�������"), WS_CHILD | WS_VISIBLE,
                rcTitle_Ret.left, rcTitle_Ret.bottom,
                cxClient * 28 / 40, cyClient * 8 / 10,
                hwnd, (HMENU)USERBOOKINGID_SEARCH_SEARCHBAR, hInst, NULL);
            
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
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;

    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//  Ŀ��: ���������������е������������Ϣ
static LRESULT CALLBACK UserBooking_Search_SearchBarProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
    static BOOL fHover=FALSE, fClick=FALSE;

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
                hwnd, (HMENU)DIY_HMENU_BEGIN, hInst, NULL);
            hwndCheckBox[1] = CreateWindow(DIY_CHECKBOX,
                TEXT("�����/�����"),
                WS_CHILD | WS_VISIBLE,
                cxClient * 4 / 10, cyClient / 20, cxClient * 1 / 13, cxClient * 1 / 13,
                hwnd, (HMENU)(DIY_HMENU_BEGIN + 1), hInst, NULL);
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
        
        if(lParam == CHECKBOX_CHECKED)
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
                IDDATA iddata[MAX_SEARCH_FLIGHT_ID] = {0};
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
            if (!fHover&&!fClick&&wParam!=VK_LBUTTON)
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
    //case WM_MOUSELEAVE:
    //    fHover = FALSE;
    //    fClick = FALSE;
    //    FillRect(hdcMemBk, &rcSearch_Bk, hbrMenuBk);
    //    FillRect(hdcMemBk, &rcSearch, (HBRUSH)GetStockObject(WHITE_BRUSH));
    //    SetTextColor(hdcMemBk, crMenuBk);

    //    SelectObject(hdcMemBk, (HFONT)hf);
    //    DrawText(hdcMemBk, TEXT("����"),
    //        -1, &rcSearch_Bk, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
    //    InvalidateRect(hwnd, &rcSearch_Bk, FALSE);
    //    UpdateWindow(hwnd);
    //    return 0;
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

//  Ŀ��: ���������������е�������������Ӵ�����Ϣ
static LRESULT CALLBACK UserBooking_Search_SearchBar_ChildBarProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
    { TEXT("  ������"),TEXT("  Ŀ�ĵ�"),TEXT("  ����ʱ��(��ѡ)")};
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
                    (LONG)UserBooking_Search_SearchBar_ChildBar_EditProc);
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
                        (LONG)UserBooking_Search_SearchBar_ChildBar_EditProc);
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
static LRESULT CALLBACK UserBooking_Search_SearchBar_ChildBar_EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
                MAKELONG((rcSearch.right + rcSearch.left)/2, (rcSearch.bottom + rcSearch.top) / 2));
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

//  Ŀ��: ���������������е��Ҳ����������Ϣ
static LRESULT CALLBACK UserBooking_Search_RetProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{

    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    //��������
    static HBRUSH hbrBk;
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
    static IDDATA iddataNow[6] = {0};
    //ʣ���IDDATA��Ŀ����һ�� ���٣���һ�����ӣ���ʱ��һ���г��������±���iddata_num-iddata_num_rest
    static int iddata_num_rest;//�п���Ϊ��
    static BOOL fExisted = FALSE;
    int i, j;

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
                
            }
            
            hfTitle = SetFont(hdcMemBk, (rcClient.bottom - rcClient.top) * 1 / 12,
                (rcClient.bottom - rcClient.top) * 1 / 12 * 3 / 8,
                FONT_STYLE_DEFAULT + 400);
            SetTextColor(hdcMemBk, RGB(100, 100, 100));
            SelectObject(hdcMemBk, hfTitle);
            hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_APP_ICON));
            DrawIcon(hdcMemBk, cxClient / 4 + 10, (rcClient.bottom - rcClient.top) * 11 / 32, hIcon);
            DrawText(hdcMemBk, TEXT("\n\n\n\n  ������Ʊ������"), -1, &rcClient, DT_CENTER);
            hfBig = SetFont(hdcMemBk, (rcCompany[0].bottom - rcCompany[0].top) * 1 / 3,
                (rcCompany[0].bottom - rcCompany[0].top) * 1 / 3 * 3 / 8,
                FONT_STYLE_DEFAULT + 400);
            hfSmall = SetFont(hdcMemBk, (rcCompany[0].bottom - rcCompany[0].top) / 4,
                (rcCompany[0].bottom - rcCompany[0].top) / 4 * 3 / 8,
                FONT_STYLE_DEFAULT + 400);


            //BUTTON
            hbrButton = CreateSolidBrush(RGB(255,255,255));
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
            for (i=0; i < min(iddata_num,6); i++)
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
                    rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 17 / 60 ,
                    szBuf, lstrlen(szBuf));

                DataFlight_GetAirportName(szBuf, pFlNode->fl.szDepart);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfSmall);
                TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                    rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 7 / 12,
                    szBuf, lstrlen(szBuf));

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
                DataFlight_GetAirportName(szBuf, pFlNode->fl.szDest);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfSmall);
                TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                    rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                    szBuf, lstrlen(szBuf));

                //5
                SetTextAlign(hdcMemBk, TA_RIGHT);
                StringCchPrintf(szBuf, 32,  TEXT("��%.0f"), pFlNode->fl.dPrice);
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

                        DataFlight_GetAirportName(szBuf, pFlNode->fl.szDepart);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                            rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));

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
                        DataFlight_GetAirportName(szBuf, pFlNode->fl.szDest);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));

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

                        DataFlight_GetAirportName(szBuf, pFlNode->fl.szDepart);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                            rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));

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
                        DataFlight_GetAirportName(szBuf, pFlNode->fl.szDest);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));

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
                //չʾ����
                for (i = 0; i < min(iddata_num_rest, 6); i++)
                {
                    if (PT_ON_RECT(pt, rcItem[i]))
                    {
                        SendMessage(hwndDetail, WM_U_SHOWFLIGHTDETAIL, iddataNow[i], 0);
                        ShowWindow(hwndDetail, SW_SHOW);
                        ShowWindow(hwndSearch, SW_HIDE);

                        return 0;
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
                    DataFlight_GetCompanyName(szBuf, pFlNode->fl.szFlightID);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                        rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) / 5,
                        szBuf, lstrlen(szBuf));

                    StringCchPrintf(szBuf, 32, TEXT("�ɻ���ţ�%s"), pFlNode->fl.szPlaneID);
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

                    DataFlight_GetAirportName(szBuf, pFlNode->fl.szDepart);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                        rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 7 / 12,
                        szBuf, lstrlen(szBuf));

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
                    DataFlight_GetAirportName(szBuf, pFlNode->fl.szDest);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                        szBuf, lstrlen(szBuf));

                    //5
                    SetTextAlign(hdcMemBk, TA_RIGHT);
                    StringCchPrintf(szBuf, 32, TEXT("Ʊ����%d"), pFlNode->fl.iAmount);
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

                    DataFlight_GetAirportName(szBuf, pFlNode->fl.szDepart);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                        rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 7 / 12,
                        szBuf, lstrlen(szBuf));

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
                    DataFlight_GetAirportName(szBuf, pFlNode->fl.szDest);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                        szBuf, lstrlen(szBuf));

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

            DataFlight_GetAirportName(szBuf, pFlNode->fl.szDepart);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfSmall);
            TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 7 / 12,
                szBuf, lstrlen(szBuf));

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
            DataFlight_GetAirportName(szBuf, pFlNode->fl.szDest);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfSmall);
            TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                szBuf, lstrlen(szBuf));

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

        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}


//Detail����
/*
*   ����: UserBooking_DetailProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ����UserBooking�С���ϸ������Ϣ�����ڹ��̵���Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
static LRESULT CALLBACK UserBooking_DetailProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //��������
    static HWND hwndBackButton, hwndBookButton;
    static HBRUSH hbrBk;
    //TITLE
    static RECT rcTitle;
    static HFONT hfTitle;
    static HBRUSH hbrTitle;
    //CONTENT
    static RECT rcContent;
    static HBRUSH hbrContent;
    static HPEN hpenLine;
    static HFONT hfContent_title;
    static HFONT hfContent_content;
    static RECT rcContent1, rcContent2;
    static RECT rcContent1_ti[2], rcContent1_con[6];
    static RECT rcContent2_ti[2], rcContent2_con[6];
                            
    //DATA
    static IDDATA id = 0;
    static LPFLIGHTDATANODE pFlNode = NULL;

    TCHAR szBuf1[128] = TEXT("\0");
    int i;
    SIZE sizeText;
    SIZE sizeRc;
    RECT rc;
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
                hwnd, (HMENU)USERBOOKINGID_DETAIL_BACKBUTTON, hInst, NULL
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
            SetRect(&rcContent, cxClient / 10, cyClient * 4 / 20, cxClient * 9 / 10, cyClient * 18 / 20);

            hpenLine = CreatePen(PS_SOLID, 1, RGB(210, 210, 210));
            SelectObject(hdcMemBk, hpenLine);
            hbrContent = (HBRUSH)GetStockObject(WHITE_BRUSH);
            //cont
            //con1
            SetRect(&rcContent1,
                rcContent.left,
                rcContent.top + (rcContent.bottom - rcContent.top) / 10,
                (rcContent.left + rcContent.right) / 2 - (rcContent.right-rcContent.left) / 40,
                rcContent.bottom - (rcContent.bottom - rcContent.top) * 2 / 10);
                //title
            for (i = 0; i < 2; i++)
            {
                SetRect(&rcContent1_ti[i],
                    rcContent1.left,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 2,
                    rcContent1.left + (rcContent1.right - rcContent1.left) * 3 / 10,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * (i + 1) / 2
                );
            }
            hfContent_title = SetFont(hdcMemBk,
                (rcContent1_ti[0].right - rcContent1_ti[0].left) / 5,
                (rcContent1_ti[0].right - rcContent1_ti[0].left) / 5 * 3 / 8,
                FONT_STYLE_DEFAULT + 450);
                //con
            for (i = 0; i < 6; i++)
            {
                SetRect(&rcContent1_con[i],
                    rcContent1_ti[0].right,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * i / 6,
                    rcContent1.right,
                    rcContent1.top + (rcContent1.bottom - rcContent1.top) * (i + 1) / 6
                );
            }
            hfContent_content = SetFont(hdcMemBk,
                (rcContent1_con[0].right - rcContent1_con[0].left) / 13,
                (rcContent1_con[0].right - rcContent1_con[0].left) / 13 * 3 / 8,
                FONT_STYLE_DEFAULT + 400);
            
            //con2
            SetRect(&rcContent2,
                (rcContent.right + rcContent.left) / 2 + (rcContent.right - rcContent.left) / 40,
                rcContent.top + (rcContent.bottom - rcContent.top) / 10,
                rcContent.right - (rcContent.right - rcContent.left) / 20,
                rcContent.bottom - (rcContent.bottom - rcContent.top) * 2 / 10);
                //title
            for (i = 0; i < 2; i++)
            {
                SetRect(&rcContent2_ti[i],
                    rcContent2.left,
                    rcContent2.top + (rcContent2.bottom - rcContent2.top) * i / 2,
                    rcContent2.left + (rcContent2.right - rcContent2.left) * 3 / 10,
                    rcContent2.top + (rcContent2.bottom - rcContent2.top) * (i + 1) / 2
                );
            }
                //con
            for (i = 0; i < 6; i++)
            {

                SetRect(&rcContent2_con[i],
                    rcContent2_ti[0].right,
                    rcContent2.top + (rcContent2.bottom - rcContent2.top) * i / 6,
                    rcContent2.right,
                    rcContent2.top + (rcContent2.bottom - rcContent2.top) * (i + 1) / 6
                );                

            }
            hwndBookButton = CreateWindow(szDetail_BookButtonName,
                TEXT("�ύ��Ʊ��ť"),
                WS_CHILD | WS_VISIBLE,
                rcContent2.left + (rcContent2.right - rcContent2.left) * 7 / 9,
                rcContent2.bottom + (rcContent2.right - rcContent2.left) / 20,
                (rcContent2.right - rcContent2.left) / 4,
                (rcContent2.right - rcContent2.left) / 8,
                hwnd, (HMENU)USERBOOKINGID_DETAIL_BOOKBUTTON, hInst, NULL
            );

            fInited = TRUE;
            //��ʼ������
        }
        return 0;
    case WM_U_SHOWFLIGHTDETAIL:
        id = (IDDATA)wParam;
        pFlNode = DataFlight_GetPNode(id);
        SendMessage(hwndBookButton, uMsg, wParam, lParam);
        //TITLE
        FillRect(hdcMemBk, &rcTitle, hbrTitle);
        SelectObject(hdcMemBk, hfTitle);
        SetTextColor(hdcMemBk, RGB(83, 83, 83));
        StringCchPrintf(szBuf, 128, TEXT("%s  ��  %s  �ĺ�����Ϣ"),
            pFlNode->fl.szDepart, pFlNode->fl.szDest);
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
        for (i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:
                DataFlight_GetCityName(szBuf, pFlNode->fl.szDepart);
                StringCchPrintf(szBuf1, LIMIT_SZDEPARTDEST, TEXT("��������: %s"), szBuf);
                break;
            case 1:
                DataFlight_GetAirportName(szBuf, pFlNode->fl.szDepart);
                StringCchPrintf(szBuf1, LIMIT_SZDEPARTDEST, TEXT("��������: %s"), szBuf);
                break;
            case 2:
                StringCchPrintf(szBuf1, LIMIT_SZDEPARTDEST, TEXT("����ʱ��: %d��%d��%02d:%02d"),
                    pFlNode->fl.dtDepart.month,pFlNode->fl.dtDepart.day, 
                    pFlNode->fl.dtDepart.hr,pFlNode->fl.dtDepart.min);
                break;
            case 3:
                DataFlight_GetCityName(szBuf, pFlNode->fl.szDest);
                StringCchPrintf(szBuf1, LIMIT_SZDEPARTDEST, TEXT("Ŀ�ĳ���: %s"), szBuf);
                break;
            case 4:
                DataFlight_GetAirportName(szBuf, pFlNode->fl.szDepart);
                StringCchPrintf(szBuf1, LIMIT_SZDEPARTDEST, TEXT("Ŀ�Ļ���: %s"), szBuf);
                break;
            case 5:
                StringCchPrintf(szBuf1, LIMIT_SZDEPARTDEST, TEXT("Ԥ�Ƶ���ʱ��: %d��%d��%02d:%02d"),
                    pFlNode->fl.dtDest.month, pFlNode->fl.dtDest.day,
                    pFlNode->fl.dtDest.hr, pFlNode->fl.dtDest.min);
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
            DrawText(hdcMemBk, szBuf1, -1, &rc, DT_WORDBREAK | DT_EDITCONTROL);
            
        }
        for (i = 0; i < 6; i++)
        {
            switch (i)
            {
            case 0:
                DataFlight_GetCompanyName(szBuf, pFlNode->fl.szFlightID);
                StringCchPrintf(szBuf1, LIMIT_SZDEPARTDEST, TEXT("���չ�˾: %s"), szBuf);
                break;
            case 1:
                DataFlight_GetFlightIDName(szBuf, pFlNode->fl.szFlightID);
                StringCchPrintf(szBuf1, LIMIT_SZDEPARTDEST, TEXT("�����: %s"), szBuf);
                break;
            case 2:
                StringCchPrintf(szBuf1, LIMIT_SZDEPARTDEST, TEXT("�ɻ����: %s"),
                    pFlNode->fl.szPlaneID);
                break;
            case 3:
                StringCchPrintf(szBuf1, LIMIT_SZDEPARTDEST, TEXT("Ʊ�ۣ�%.1lf"), pFlNode->fl.dPrice);
                break;
            case 4:
                StringCchPrintf(szBuf1, LIMIT_SZDEPARTDEST, TEXT("ʣ��Ʊ��: %d"), pFlNode->fl.iAmount);
                break;
            case 5:
                StringCchPrintf(szBuf1, LIMIT_SZDEPARTDEST, TEXT("��������: %s"), pFlNode->fl.szType);
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

        }

        MoveToEx(hdcMemBk, rcContent.left + (rcContent.right - rcContent.left) / 2, rcContent.top, NULL);
        LineTo(hdcMemBk, rcContent.left + (rcContent.right - rcContent.left) / 2, rcContent.bottom);
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;

    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hfTitle);
        DeleteObject(hpenLine);
        DeleteObject(hfContent_title);
        DeleteObject(hfContent_content);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//  Ŀ��: �����ذ�ť����Ϣ��
static LRESULT CALLBACK UserBooking_Detail_BackButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
        ShowWindow(hwndDetail, SW_HIDE);
        ShowWindow(hwndSearch, SW_SHOW);
        return 0;

    case WM_MOUSEMOVE:
    {
        TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd, 30};
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

//  Ŀ��: �����İ�ť����Ϣ��
static LRESULT CALLBACK UserBooking_Detail_BookButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //��������
    static HBRUSH hbrBk, hbrBk_hover, hbrBk_click;
    static HFONT hf;
    static BOOL fClickNow = FALSE;
    static int idBooking;
    static LPFLIGHTDATANODE pFlNode = NULL;
    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {
            hbrBk = CreateSolidBrush(RGB(255, 150, 7));
            hbrBk_hover = CreateSolidBrush(RGB(255, 174, 64));
            hbrBk_click = CreateSolidBrush(RGB(255, 100, 7));
            DefInitWindow(hwnd, cxClient, cyClient, hbrBk, &rcClient, &hdcMemBk, &hbmpBk);
            SetTextColor(hdcMemBk, RGB(255, 255, 255));
            hf = SetFont(hdcMemBk, cyClient / 2, cyClient / 2 * 3 / 8, FONT_STYLE_DEFAULT + 450);
            SelectObject(hdcMemBk, hf);
            DrawText(hdcMemBk, TEXT("������Ʊ"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
            fInited = TRUE;
        }
        return 0;
    case WM_U_SHOWFLIGHTDETAIL:
        idBooking = (IDDATA)wParam;
        //ͨ��ID��øýڵ���Ϣ
        pFlNode = DataFlight_GetPNode(idBooking);
        return 0;
    case WM_LBUTTONDOWN:
        fClickNow = TRUE;
        FillRect(hdcMemBk, &rcClient, hbrBk_click);
        DrawText(hdcMemBk, TEXT("������Ʊ"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        
        return 0;
    case WM_LBUTTONUP:
        fClickNow = FALSE;
        FillRect(hdcMemBk, &rcClient, hbrBk_hover);
        DrawText(hdcMemBk, TEXT("������Ʊ"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        if (IDOK == MessageBox(hwnd, TEXT("�����ڽ��ж�Ʊ�������Ƿ�ȷ��Ҫ��Ʊ"), TEXT("��Ʊ��ʾ"), MB_OKCANCEL | MB_ICONQUESTION))
        {
            
            DataOrder_CreateSingleOrder(idBooking);  //���ɶ������ݵ��ڴ�
            DataOrderLinkedListFile(OILIST_TO_FILE);//�������浽�ļ�---ģ�������
            if (IDOK == MessageBox(hwnd, TEXT("��Ʊ�ɹ�!�������ҵĶ����в鵽"), TEXT("��Ʊ��ʾ"), MB_OK | MB_ICONINFORMATION))
            {
                SendMessage(hwndBodyOpt[0][2], WM_U_UPDATEORDER, 0, 0);
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
        DrawText(hdcMemBk, TEXT("������Ʊ"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
    }
    return 0;

    case WM_MOUSELEAVE:
        
        FillRect(hdcMemBk, &rcClient, hbrBk);
        DrawText(hdcMemBk, TEXT("������Ʊ"), -1, &rcClient, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
            
        fClickNow = FALSE;
        return 0;

    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hbrBk);DeleteObject(hbrBk_hover);DeleteObject(hbrBk_click);
        DeleteObject(hf);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
