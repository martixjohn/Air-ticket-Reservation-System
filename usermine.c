/*
*       usermine.c
*       ���������û������������غ���
*/
#include "bodyuser.h"
//���ھ��
extern HWND hwndDetail;
/*
*   ����: UserMineProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ����UserMine����Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
LRESULT CALLBACK UserMineProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //��������
    HDC hdc;
    static RECT rcTitle;
    static HFONT hfTitle;
    //empty
    static HFONT hfEmpty;
    static HDC hdcEmpty;
    static HBITMAP hbmpEmpty;
    static BITMAP bmpEmpty;
    static RECT rcEmptyText;
    //ORDERLIST
    static RECT rcOrderList;
    static RECT rcItem[4];
    static SIZE sizeItem;
    static RECT rcCompany[4];
    static HFONT hfBig;
    static HFONT hfSmall;
    static HFONT hfOrderNum;
    static RECT rcDestAirport_Time[4], rcDepartAirport_Time[4];
    static RECT rcPrice_Type[4];
    static RECT rcOrderNum[4];
    static BOOL fItemHover[4] = { FALSE };
    static HBRUSH hbrItemHover;
    static RECT rcDelete[4];
    static HBRUSH hbrDelete;
    //PAGE NUM
    static RECT rcShowPageNum;
    int nowPage;
    static int allPage;
    //BUTTON
    static RECT rcNext, rcPrevious;
    static HBRUSH hbrButton;
    static HBRUSH hbrButtonHover;
    static BOOL fButtonHover[2] = { FALSE };
    //��������
    static LPORDERINFONODE pOINoDe_all;
    static IDDATA_NUM iddata_num;
    static IDDATA iddataAll[MAX_SEARCH_FLIGHT_ID];
    static IDDATA iddataNow[4] = {0};
    static LPORDERINFONODE orderNodeAll[MAX_SEARCH_FLIGHT_ID];//��Ŷ���ָ��
    static LPORDERINFONODE orderNodeNow[4] = {NULL};
    LPORDERINFONODE pOINoDe, pOINode_old, pOINode_next, pOINoDe_;
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
            DefInitWindow(hwnd, cxClient, cyClient, hbrBodyBk, &rcClient, &hdcMemBk, &hbmpBk);
            //TITLE
            SetRect(&rcTitle, cxClient / 20, cxClient / 20, cxClient * 5 / 20, cyClient * 3 / 20);
            hfTitle = SetFont(hdcMemBk, (rcTitle.bottom - rcTitle.top),
                (rcTitle.bottom - rcTitle.top) * 3 / 8, FONT_STYLE_DEFAULT + 500);
            SelectObject(hdcMemBk, hfTitle);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            DrawText(hdcMemBk, TEXT("�ҵĶ���"), -1, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
            
            //ORDERLIST RECT
            SetRect(&rcOrderList, cxClient / 10, rcTitle.bottom * 3 / 2, cxClient * 9 / 10, cyClient);
            //EMPTY
            hdc = GetDC(hwnd);
            hdcEmpty = CreateCompatibleDC(hdc);
            hbmpEmpty = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_EMPTY));
            GetObject(hbmpEmpty, sizeof(BITMAP), &bmpEmpty);
            SelectObject(hdcEmpty, hbmpEmpty);
            SetStretchBltMode(hdcMemBk, TRANSPARENT);
            SetRect(&rcEmptyText, rcOrderList.left, 
                rcOrderList.top + (rcOrderList.bottom - rcOrderList.top) * 22 / 40,
                rcOrderList.right, 
                rcOrderList.top + (rcOrderList.bottom - rcOrderList.top) * 25 / 40);
            hfEmpty = SetFont(hdcMemBk, (rcEmptyText.bottom - rcEmptyText.top),
                (rcEmptyText.bottom - rcEmptyText.top) * 3 / 8, FONT_STYLE_DEFAULT + 380);
            ReleaseDC(hwnd, hdc);
            //ITEM
            hbrItemHover = CreateSolidBrush(RGB(249,249,249));
            sizeItem.cx = rcOrderList.right - rcOrderList.left;
            sizeItem.cy = (rcOrderList.bottom - rcOrderList.top) / 5;
            for (i = 0; i < 4; i++)
            {
                SetRect(&rcItem[i],
                    rcOrderList.left, rcOrderList.top + sizeItem.cy * i / 20 + sizeItem.cy * i,
                    rcOrderList.right, rcOrderList.top + sizeItem.cy * i / 20 + sizeItem.cy * i + sizeItem.cy);
                SetRect(&rcOrderNum[i], rcItem[i].left + (rcItem[i].right - rcItem[i].left) / 40,
                    rcItem[i].top,
                    rcItem[i].right,
                    rcItem[i].top + (rcItem[i].bottom - rcItem[i].top) / 4);
                SetRect(&rcCompany[i], rcItem[i].left, rcOrderNum[i].bottom,
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) / 5, rcItem[i].bottom);
                SetRect(&rcDepartAirport_Time[i],
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) / 5,
                    rcOrderNum[i].bottom,
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 9 / 20,
                    rcItem[i].bottom);
                SetRect(&rcDestAirport_Time[i],
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 11 / 20,
                    rcOrderNum[i].bottom,
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 4 / 5,
                    rcItem[i].bottom);
                SetRect(&rcPrice_Type[i],
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 4 / 5,
                    rcOrderNum[i].bottom,
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 5 / 5,
                    rcItem[i].bottom);
                SetRect(&rcDelete[i],
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 9 / 10,
                    rcItem[i].top,
                    rcItem[i].left + (rcItem[i].right - rcItem[i].left) * 5 / 5,
                    rcItem[i].bottom);
            }
            hfBig = SetFont(hdcMemBk, (rcCompany[0].bottom - rcCompany[0].top) * 1 / 3,
                (rcCompany[0].bottom - rcCompany[0].top) * 1 / 3 * 3 / 8,
                FONT_STYLE_DEFAULT + 400);
            hfSmall = SetFont(hdcMemBk, (rcCompany[0].bottom - rcCompany[0].top) / 4,
                (rcCompany[0].bottom - rcCompany[0].top) / 4 * 3 / 8,
                FONT_STYLE_DEFAULT + 400);
            hfOrderNum = SetFont(hdcMemBk, (rcOrderNum[0].bottom - rcOrderNum[0].top) * 7 / 10,
                (rcOrderNum[0].bottom - rcOrderNum[0].top) * 7 / 10 * 3 / 8,
                FONT_STYLE_DEFAULT + 300);

            //BUTTON
            hbrButton = CreateSolidBrush(RGB(255, 255, 255));
            hbrButtonHover = CreateSolidBrush(RGB(0, 122, 204));
            SetRect(&rcPrevious, (rcItem[0].left + rcItem[0].right) * 3 / 8, rcOrderList.bottom - (rcItem[0].bottom - rcItem[0].top) / 2,
                (rcItem[0].left + rcItem[0].right) * 2 / 4 - 1, rcOrderList.bottom);
            SetRect(&rcNext, (rcItem[0].left + rcItem[0].right) * 2 / 4 + 1, rcOrderList.bottom - (rcItem[0].bottom - rcItem[0].top) / 2,
                (rcItem[0].left + rcItem[0].right) * 5 / 8, rcOrderList.bottom);

            //PAGE NUM
            SetRect(&rcShowPageNum,
                rcItem[0].left + (rcItem[0].right - rcItem[0].left) * 3 / 8,
                rcPrevious.top - (rcItem[0].bottom - rcItem[0].top) / 3,
                rcItem[0].left + (rcItem[0].right - rcItem[0].left) * 5 / 8, rcPrevious.top);


            SetBkMode(hdcMemBk, TRANSPARENT);
            SendMessage(hwnd, WM_U_UPDATEORDER, 0, 0);
            
            hbrDelete = CreateSolidBrush(RGB(250, 40, 0));
            fInited = TRUE;
        }
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;
    case WM_U_UPDATEORDER:
        
        iddata_num_rest = iddata_num = DataUserOrder(&pOINoDe_all, OILIST_GET_HEADNODE);
        ClearIDDATAARRToZero(iddataAll);
        ClearPointerArrToZero(orderNodeAll, MAX_SEARCH_FLIGHT_ID);
        pOINoDe = pOINoDe_all;
        for (i = 0; i < iddata_num && pOINoDe != NULL; i++)
        {
            iddataAll[i] = pOINoDe->oi.fldata.id;
            orderNodeAll[i] = pOINoDe;
            pOINoDe = pOINoDe->next;
        }

        allPage = iddata_num / 4;
        nowPage = (iddata_num - iddata_num_rest) / 4;
        if (iddata_num % 4 != 0) allPage++;
        if (iddata_num - iddata_num_rest % 4 != 0 || iddata_num_rest != 0) nowPage++;


        //������
        if (iddata_num == 0)
        {
            //size:cxClient/10
            FillRect(hdcMemBk, &rcOrderList, hbrBodyBk);
            StretchBlt(hdcMemBk, cxClient * 9 / 20, cyClient / 2 - cxClient / 20, cxClient / 10, cxClient / 10,
                hdcEmpty, 0, 0, bmpEmpty.bmWidth, bmpEmpty.bmHeight, SRCCOPY);
            SelectObject(hdcMemBk, hfEmpty);
            SetTextColor(hdcMemBk, RGB(70, 70, 70));
            DrawText(hdcMemBk, TEXT("�㻹û�ж���Ŷ"), -1, &rcEmptyText, DT_CENTER);
            fExisted = FALSE;
        }
        //������
        else
        {
            FillRect(hdcMemBk, &rcOrderList, hbrBodyBk);
            for (i = 0; i < 4; i++)
            {
                iddataNow[i] = iddataAll[i];//0Ҳд��
                orderNodeNow[i] = orderNodeAll[i];
            }
                
            SelectObject(hdcMemBk, hfBig);
            
            FillRect(hdcMemBk, &rcPrevious, hbrButton);
            SetTextColor(hdcMemBk, RGB(80, 80, 80));
            SetTextAlign(hdcMemBk, TA_LEFT);
            DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcPrevious, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            FillRect(hdcMemBk, &rcNext, hbrButton);
            DrawText(hdcMemBk, TEXT("��һҳ"), -1, &rcNext, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            SetTextColor(hdcMemBk, RGB(70, 70, 70));
            StringCchPrintf(szBuf, 32, TEXT("%d/%d"), nowPage, allPage);
            DrawText(hdcMemBk, szBuf, -1, &rcShowPageNum, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            //һ������ʾ4��������Ϣ
            for (i = 0; i < min(iddata_num, 4); i++)
            {

                FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));

                SetTextAlign(hdcMemBk, TA_LEFT);

                //OrderNum
                StringCchPrintf(szBuf, 32, TEXT("������ţ�%s"), orderNodeNow[i]->oi.szOrderNumber);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfOrderNum);
                DrawText(hdcMemBk, szBuf, -1, &rcOrderNum[i], DT_SINGLELINE | DT_VCENTER);

                //1
                DataFlight_GetCompanyName(szBuf, orderNodeNow[i]->oi.fldata.szFlightID);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfBig);
                TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                    rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) / 5,
                    szBuf, lstrlen(szBuf));

                DataFlight_GetFlightIDName(szBuf, orderNodeNow[i]->oi.fldata.szFlightID);
                SetTextColor(hdcMemBk, RGB(6, 89, 178));
                SelectObject(hdcMemBk, hfSmall);
                TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                    rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) * 7 / 12,
                    szBuf, lstrlen(szBuf));
                //2
                StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), orderNodeNow[i]->oi.fldata.dtDepart.hr, orderNodeNow[i]->oi.fldata.dtDepart.min);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfBig);
                TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                    rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) / 5,
                    szBuf, lstrlen(szBuf));
                StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), orderNodeNow[i]->oi.fldata.dtDepart.month, orderNodeNow[i]->oi.fldata.dtDepart.day);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfSmall);
                TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) * 5 / 10,
                    rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 17 / 60,
                    szBuf, lstrlen(szBuf));

                DataFlight_GetAirportName(szBuf, orderNodeNow[i]->oi.fldata.szDepart);
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
                StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), orderNodeNow[i]->oi.fldata.dtDest.hr, orderNodeNow[i]->oi.fldata.dtDest.min);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfBig);
                TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                    rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) / 5,
                    szBuf, lstrlen(szBuf));
                StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), orderNodeNow[i]->oi.fldata.dtDest.month, orderNodeNow[i]->oi.fldata.dtDest.day);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfSmall);
                TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) * 5 / 10,
                    rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 17 / 60,
                    szBuf, lstrlen(szBuf));
                DataFlight_GetAirportName(szBuf, orderNodeNow[i]->oi.fldata.szDest);
                SetTextColor(hdcMemBk, RGB(50, 50, 50));
                SelectObject(hdcMemBk, hfSmall);
                TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                    rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                    szBuf, lstrlen(szBuf));

                //5
                SetTextAlign(hdcMemBk, TA_RIGHT);
                StringCchPrintf(szBuf, 32, TEXT("��%.0f"), orderNodeNow[i]->oi.fldata.dPrice);
                SetTextColor(hdcMemBk, RGB(12, 104, 224));
                SelectObject(hdcMemBk, hfBig);
                TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                    rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) / 5,
                    szBuf, lstrlen(szBuf));

                lstrcpy(szBuf, orderNodeNow[i]->oi.fldata.szType);
                SetTextColor(hdcMemBk, RGB(85, 85, 85));
                SelectObject(hdcMemBk, hfSmall);
                TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                    rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) * 7 / 12,
                    szBuf, lstrlen(szBuf));
            }
            //��ʾ����

            fExisted = TRUE;
        }
        InvalidateRect(hwnd, &rcOrderList, FALSE);
        UpdateWindow(hwnd);

        return 0;
    case WM_LBUTTONUP:
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        if (fExisted == TRUE)
        {
            if (PT_ON_RECT(pt, rcNext) || PT_ON_RECT(pt, rcPrevious))
            {


                //��һҳ
                if (PT_ON_RECT(pt, rcPrevious))
                {

                    //��ҳ������
                    if (iddata_num_rest == iddata_num) return 0;
                    iddata_num_rest += 4;//��0

                    for (j = 0, i = iddata_num - iddata_num_rest;
                        i < min(iddata_num - iddata_num_rest + 4, MAX_SEARCH_FLIGHT_ID);
                        j++, i++)
                    {

                        iddataNow[j] = iddataAll[i];//0Ҳд��
                        orderNodeNow[j] = orderNodeAll[i];
                        
                    }
                    nowPage = (iddata_num - iddata_num_rest) / 4;
                    if (iddata_num - iddata_num_rest % 4 != 0) nowPage++;

                    //���������Ž��л��ƣ�
                    FillRect(hdcMemBk, &rcOrderList, hbrBodyBk);
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
                    for (i = 0; i < min(iddata_num_rest, 4); i++)
                    {

                        FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));

                        SetTextAlign(hdcMemBk, TA_LEFT);
                        //OrderNum
                        StringCchPrintf(szBuf, 32, TEXT("������ţ�%s"), orderNodeNow[i]->oi.szOrderNumber);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfOrderNum);
                        DrawText(hdcMemBk, szBuf, -1, &rcOrderNum[i], DT_SINGLELINE | DT_VCENTER);
                        //1
                        DataFlight_GetCompanyName(szBuf, orderNodeNow[i]->oi.fldata.szFlightID);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                            rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) / 5,
                            szBuf, lstrlen(szBuf));

                        DataFlight_GetFlightIDName(szBuf, orderNodeNow[i]->oi.fldata.szFlightID);
                        SetTextColor(hdcMemBk, RGB(6, 89, 178));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                            rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));
                        //2
                        StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), orderNodeNow[i]->oi.fldata.dtDepart.hr, orderNodeNow[i]->oi.fldata.dtDepart.min);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                            rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) / 5,
                            szBuf, lstrlen(szBuf));
                        StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), orderNodeNow[i]->oi.fldata.dtDepart.month, orderNodeNow[i]->oi.fldata.dtDepart.day);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) * 5 / 10,
                            rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 17 / 60,
                            szBuf, lstrlen(szBuf));

                        DataFlight_GetAirportName(szBuf, orderNodeNow[i]->oi.fldata.szDepart);
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
                        StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), orderNodeNow[i]->oi.fldata.dtDest.hr, orderNodeNow[i]->oi.fldata.dtDest.min);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) / 5,
                            szBuf, lstrlen(szBuf));
                        StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), orderNodeNow[i]->oi.fldata.dtDest.month, orderNodeNow[i]->oi.fldata.dtDest.day);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) * 5 / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 17 / 60,
                            szBuf, lstrlen(szBuf));
                        DataFlight_GetAirportName(szBuf, orderNodeNow[i]->oi.fldata.szDest);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));

                        //5
                        SetTextAlign(hdcMemBk, TA_RIGHT);
                        StringCchPrintf(szBuf, 32, TEXT("��%.0f"), orderNodeNow[i]->oi.fldata.dPrice);
                        SetTextColor(hdcMemBk, RGB(12, 104, 224));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                            rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) / 5,
                            szBuf, lstrlen(szBuf));

                        lstrcpy(szBuf, orderNodeNow[i]->oi.fldata.szType);
                        SetTextColor(hdcMemBk, RGB(85, 85, 85));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                            rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));


                    }
                    InvalidateRect(hwnd, &rcOrderList, FALSE);
                    UpdateWindow(hwnd);


                }
                //��һҳ
                if (PT_ON_RECT(pt, rcNext))
                {

                    //�ڰ���ǰ
                    if (iddata_num_rest <= 4) return 0;
                    iddata_num_rest -= 4;//��ǰҳʣ�µĸ���

                    for (j = 0, i = iddata_num - iddata_num_rest;
                        i < min(iddata_num - iddata_num_rest + 4, MAX_SEARCH_FLIGHT_ID);
                        j++, i++)
                    {
                        iddataNow[j] = iddataAll[i];//0Ҳд��
                        orderNodeNow[j] = orderNodeAll[i];
                    }
                    nowPage = (iddata_num - iddata_num_rest) / 4;
                    if (iddata_num - iddata_num_rest % 4 != 0) nowPage++;

                    //���������Ž��л��ƣ�
                    FillRect(hdcMemBk, &rcOrderList, hbrBodyBk);
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
                    for (i = 0; i < min(iddata_num_rest, 4); i++)
                    {
                        
                        FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));
                        SetTextAlign(hdcMemBk, TA_LEFT);
                        //OrderNum
                        StringCchPrintf(szBuf, 32, TEXT("������ţ�%s"), orderNodeNow[i]->oi.szOrderNumber);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfOrderNum);
                        DrawText(hdcMemBk, szBuf, -1, &rcOrderNum[i], DT_SINGLELINE | DT_VCENTER);
                        //1
                        DataFlight_GetCompanyName(szBuf, orderNodeNow[i]->oi.fldata.szFlightID);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                            rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) / 5,
                            szBuf, lstrlen(szBuf));

                        DataFlight_GetFlightIDName(szBuf, orderNodeNow[i]->oi.fldata.szFlightID);
                        SetTextColor(hdcMemBk, RGB(6, 89, 178));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                            rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));
                        //2
                        StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), orderNodeNow[i]->oi.fldata.dtDepart.hr, orderNodeNow[i]->oi.fldata.dtDepart.min);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                            rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) / 5,
                            szBuf, lstrlen(szBuf));
                        StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), orderNodeNow[i]->oi.fldata.dtDepart.month, orderNodeNow[i]->oi.fldata.dtDepart.day);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) * 5 / 10,
                            rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 17 / 60,
                            szBuf, lstrlen(szBuf));

                        DataFlight_GetAirportName(szBuf, orderNodeNow[i]->oi.fldata.szDepart);
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
                        StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), orderNodeNow[i]->oi.fldata.dtDest.hr, orderNodeNow[i]->oi.fldata.dtDest.min);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) / 5,
                            szBuf, lstrlen(szBuf));
                        StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), orderNodeNow[i]->oi.fldata.dtDest.month, orderNodeNow[i]->oi.fldata.dtDest.day);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) * 5 / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 17 / 60,
                            szBuf, lstrlen(szBuf));
                        DataFlight_GetAirportName(szBuf, orderNodeNow[i]->oi.fldata.szDest);
                        SetTextColor(hdcMemBk, RGB(50, 50, 50));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                            rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));

                        //5
                        SetTextAlign(hdcMemBk, TA_RIGHT);
                        StringCchPrintf(szBuf, 32, TEXT("��%.0f"), orderNodeNow[i]->oi.fldata.dPrice);
                        SetTextColor(hdcMemBk, RGB(12, 104, 224));
                        SelectObject(hdcMemBk, hfBig);
                        TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                            rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) / 5,
                            szBuf, lstrlen(szBuf));

                        lstrcpy(szBuf, orderNodeNow[i]->oi.fldata.szType);
                        SetTextColor(hdcMemBk, RGB(85, 85, 85));
                        SelectObject(hdcMemBk, hfSmall);
                        TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                            rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) * 7 / 12,
                            szBuf, lstrlen(szBuf));

                    }
                    InvalidateRect(hwnd, &rcOrderList, FALSE);
                    UpdateWindow(hwnd);

                }

            }
            else
            {
            //ɾ������
                for (i = 0; i < min(iddata_num_rest, 4); i++)
                {
                    if (PT_ON_RECT(pt, rcDelete[i]))
                    {
                        if (IDOK == MessageBox(hwnd, TEXT("ȷ���˶����˲����޷��������˶�����ȡ������"), TEXT("����"), MB_OKCANCEL))
                        {
                            lstrcpy(szBuf, orderNodeNow[i]->oi.szOrderNumber);//��¼�����ű��ڱȶ�
                            //���û�����
                            pOINode_old = pOINoDe = pOINoDe_all;
                            //����ɾ���ڵ�
                            while (pOINoDe != NULL)
                            {
                                //���»�ֱ�Ӹı�ڵ����ݣ���սڵ���Ϣ
                                if (orderNodeNow[i] == pOINoDe)
                                {
                                    //ȡ�����ӣ�������ͷβָ��
                                    pOINode_next = pOINoDe->next;
                                    pOINode_old->next = pOINode_next;
                                    //ע�������ֱ�Ӹı��ڴ��е�����DataUserOrder�д洢�ģ�
                                    //�����ͷָ��
                                    if (pOINoDe == pOINoDe_all)
                                    {
                                        DataUserOrder(&pOINode_next, OILIST_CHANGE_HEADNODE);
                                    }
                                    //ɾ���ڵ�
                                    free(pOINoDe);

                                    break;

                                }
                                pOINode_old = pOINoDe;
                                pOINoDe = pOINoDe->next;
                            }
                            //�����û�����
                            DataOrderLinkedListMem(&pOINoDe_, OILIST_GET_HEADNODE);
                            pOINode_old = pOINoDe = pOINoDe_;
                            //����ɾ���ڵ�
                            while (pOINoDe != NULL)
                            {
                                if (lstrcmp(pOINoDe->oi.szOrderNumber, szBuf) == 0)
                                {
                                    //ȡ������
                                    pOINode_next = pOINoDe->next;
                                    pOINode_old->next = pOINode_next;
                                    //�����ͷָ��
                                    if (pOINoDe == pOINoDe_)
                                    {
                                        DataOrderLinkedListMem(&pOINode_next, OILIST_CHANGE_HEADNODE);
                                    }
                                    //ɾ���ڵ�
                                    free(pOINoDe);
 
                                    break;
                                }
                                pOINode_old = pOINoDe;
                                pOINoDe = pOINoDe->next;
                            }
                            //��������Ʊ��+1
                            {
                                LPFLIGHTDATANODE pFlNode = DataFlight_GetPNode(orderNodeNow[i]->oi.fldata.id);
                                pFlNode->fl.iAmount++;
                            }
                            //д�뺽�������ļ�
                            DataFlightLinkedListFile(FLLIST_TO_FILE);
                            //д�붩���ļ�
                            DataOrderLinkedListFile(OILIST_TO_FILE);
                            //������ʾ
                            SendMessage(hwnd, WM_U_UPDATEORDER, 0, 0);
                        }
                        return 0;
                    }
                }
            }
        }
            

        return 0;
    case WM_MOUSEMOVE:
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        if (fExisted == TRUE)
        {
            for (i = 0; i < min(iddata_num_rest, 4); i++)
            {
                //����ڸ�Item��
                if (PT_ON_RECT(pt, rcItem[i]) && !fItemHover[i])
                {
                    FillRect(hdcMemBk, &rcItem[i], hbrItemHover);
                    SetTextAlign(hdcMemBk, TA_LEFT);

                    //OrderNum
                    StringCchPrintf(szBuf, 32, TEXT("������ţ�%s"), orderNodeNow[i]->oi.szOrderNumber);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfOrderNum);
                    DrawText(hdcMemBk, szBuf, -1, &rcOrderNum[i], DT_SINGLELINE | DT_VCENTER);

                    //1
                    DataFlight_GetCompanyName(szBuf, orderNodeNow[i]->oi.fldata.szFlightID);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                        rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) / 5,
                        szBuf, lstrlen(szBuf));

                    StringCchPrintf(szBuf, 32, TEXT("�ɻ���ţ�%s"), orderNodeNow[i]->oi.fldata.szPlaneID);
                    SetTextColor(hdcMemBk, RGB(6, 89, 178));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                        rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) * 7 / 12,
                        szBuf, lstrlen(szBuf));
                    //2
                    StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), orderNodeNow[i]->oi.fldata.dtDepart.hr, orderNodeNow[i]->oi.fldata.dtDepart.min);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                        rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) / 5,
                        szBuf, lstrlen(szBuf));
                    StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), orderNodeNow[i]->oi.fldata.dtDepart.month, orderNodeNow[i]->oi.fldata.dtDepart.day);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) * 5 / 10,
                        rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 17 / 60,
                        szBuf, lstrlen(szBuf));

                    DataFlight_GetAirportName(szBuf, orderNodeNow[i]->oi.fldata.szDepart);
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
                    StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), orderNodeNow[i]->oi.fldata.dtDest.hr, orderNodeNow[i]->oi.fldata.dtDest.min);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) / 5,
                        szBuf, lstrlen(szBuf));
                    StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), orderNodeNow[i]->oi.fldata.dtDest.month, orderNodeNow[i]->oi.fldata.dtDest.day);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) * 5 / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 17 / 60,
                        szBuf, lstrlen(szBuf));
                    DataFlight_GetAirportName(szBuf, orderNodeNow[i]->oi.fldata.szDest);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                        szBuf, lstrlen(szBuf));

                    //5
                    FillRect(hdcMemBk, &rcDelete[i], hbrDelete);
                    SetTextColor(hdcMemBk, RGB(225, 225, 225));
                    SelectObject(hdcMemBk, hfBig);
                    DrawText(hdcMemBk, TEXT("�˶�"), -1, &rcDelete[i], DT_SINGLELINE | DT_VCENTER | DT_CENTER);


                    InvalidateRect(hwnd, &rcItem[i], FALSE);
                    UpdateWindow(hwnd);
                    fItemHover[i] = TRUE;


                }
                //��겻�ڸ�Item��
                else if (!PT_ON_RECT(pt, rcItem[i]) && fItemHover[i]) {
                    //û��HOVERֱ������

                    FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));
                    SetTextAlign(hdcMemBk, TA_LEFT);

                    //OrderNum
                    StringCchPrintf(szBuf, 32, TEXT("������ţ�%s"), orderNodeNow[i]->oi.szOrderNumber);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfOrderNum);
                    DrawText(hdcMemBk, szBuf, -1, &rcOrderNum[i], DT_SINGLELINE | DT_VCENTER);

                    //1
                    DataFlight_GetCompanyName(szBuf, orderNodeNow[i]->oi.fldata.szFlightID);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                        rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) / 5,
                        szBuf, lstrlen(szBuf));

                    DataFlight_GetFlightIDName(szBuf, orderNodeNow[i]->oi.fldata.szFlightID);
                    SetTextColor(hdcMemBk, RGB(6, 89, 178));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                        rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) * 7 / 12,
                        szBuf, lstrlen(szBuf));
                    //2
                    StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), orderNodeNow[i]->oi.fldata.dtDepart.hr, orderNodeNow[i]->oi.fldata.dtDepart.min);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                        rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) / 5,
                        szBuf, lstrlen(szBuf));
                    StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), orderNodeNow[i]->oi.fldata.dtDepart.month, orderNodeNow[i]->oi.fldata.dtDepart.day);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) * 5 / 10,
                        rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 17 / 60,
                        szBuf, lstrlen(szBuf));

                    DataFlight_GetAirportName(szBuf, orderNodeNow[i]->oi.fldata.szDepart);
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
                    StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), orderNodeNow[i]->oi.fldata.dtDest.hr, orderNodeNow[i]->oi.fldata.dtDest.min);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) / 5,
                        szBuf, lstrlen(szBuf));
                    StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), orderNodeNow[i]->oi.fldata.dtDest.month, orderNodeNow[i]->oi.fldata.dtDest.day);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) * 5 / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 17 / 60,
                        szBuf, lstrlen(szBuf));
                    DataFlight_GetAirportName(szBuf, orderNodeNow[i]->oi.fldata.szDest);
                    SetTextColor(hdcMemBk, RGB(50, 50, 50));
                    SelectObject(hdcMemBk, hfSmall);
                    TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                        rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                        szBuf, lstrlen(szBuf));

                    //5
                    SetTextAlign(hdcMemBk, TA_RIGHT);
                    StringCchPrintf(szBuf, 32, TEXT("��%.0f"), orderNodeNow[i]->oi.fldata.dPrice);
                    SetTextColor(hdcMemBk, RGB(12, 104, 224));
                    SelectObject(hdcMemBk, hfBig);
                    TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                        rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) / 5,
                        szBuf, lstrlen(szBuf));

                    lstrcpy(szBuf, orderNodeNow[i]->oi.fldata.szType);
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
        for (i = 0; i < min(iddata_num_rest, 4); i++)
        {
            //û��HOVERֱ������
            if (!fItemHover[i]) continue;
            FillRect(hdcMemBk, &rcItem[i], (HBRUSH)GetStockObject(WHITE_BRUSH));
            SetTextAlign(hdcMemBk, TA_LEFT);


            //OrderNum
            StringCchPrintf(szBuf, 32, TEXT("������ţ�%s"), orderNodeNow[i]->oi.szOrderNumber);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfOrderNum);
            DrawText(hdcMemBk, szBuf, -1, &rcOrderNum[i], DT_SINGLELINE | DT_VCENTER);

            //1
            DataFlight_GetCompanyName(szBuf, orderNodeNow[i]->oi.fldata.szFlightID);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfBig);
            TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) / 5,
                szBuf, lstrlen(szBuf));

            DataFlight_GetFlightIDName(szBuf, orderNodeNow[i]->oi.fldata.szFlightID);
            SetTextColor(hdcMemBk, RGB(6, 89, 178));
            SelectObject(hdcMemBk, hfSmall);
            TextOut(hdcMemBk, rcCompany[i].left + (rcCompany[i].right - rcCompany[i].left) / 10,
                rcCompany[i].top + (rcCompany[i].bottom - rcCompany[i].top) * 7 / 12,
                szBuf, lstrlen(szBuf));
            //2
            StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), orderNodeNow[i]->oi.fldata.dtDepart.hr, orderNodeNow[i]->oi.fldata.dtDepart.min);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfBig);
            TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) / 10,
                rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) / 5,
                szBuf, lstrlen(szBuf));
            StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), orderNodeNow[i]->oi.fldata.dtDepart.month, orderNodeNow[i]->oi.fldata.dtDepart.day);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfSmall);
            TextOut(hdcMemBk, rcDepartAirport_Time[i].left + (rcDepartAirport_Time[i].right - rcDepartAirport_Time[i].left) * 5 / 10,
                rcDepartAirport_Time[i].top + (rcDepartAirport_Time[i].bottom - rcDepartAirport_Time[i].top) * 17 / 60,
                szBuf, lstrlen(szBuf));

            DataFlight_GetAirportName(szBuf, orderNodeNow[i]->oi.fldata.szDepart);
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
            StringCchPrintf(szBuf, 32, TEXT("%02d:%02d"), orderNodeNow[i]->oi.fldata.dtDest.hr, orderNodeNow[i]->oi.fldata.dtDest.min);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfBig);
            TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) / 5,
                szBuf, lstrlen(szBuf));
            StringCchPrintf(szBuf, 32, TEXT("  %02d��%02d��"), orderNodeNow[i]->oi.fldata.dtDest.month, orderNodeNow[i]->oi.fldata.dtDest.day);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfSmall);
            TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) * 5 / 10,
                rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 17 / 60,
                szBuf, lstrlen(szBuf));
            DataFlight_GetAirportName(szBuf, orderNodeNow[i]->oi.fldata.szDest);
            SetTextColor(hdcMemBk, RGB(50, 50, 50));
            SelectObject(hdcMemBk, hfSmall);
            TextOut(hdcMemBk, rcDestAirport_Time[i].left + (rcDestAirport_Time[i].right - rcDestAirport_Time[i].left) / 10,
                rcDestAirport_Time[i].top + (rcDestAirport_Time[i].bottom - rcDestAirport_Time[i].top) * 7 / 12,
                szBuf, lstrlen(szBuf));

            //5
            SetTextAlign(hdcMemBk, TA_RIGHT);
            StringCchPrintf(szBuf, 32, TEXT("��%.0f"), orderNodeNow[i]->oi.fldata.dPrice);
            SetTextColor(hdcMemBk, RGB(12, 104, 224));
            SelectObject(hdcMemBk, hfBig);
            TextOut(hdcMemBk, rcPrice_Type[i].right - (rcPrice_Type[i].right - rcPrice_Type[i].left) / 10,
                rcPrice_Type[i].top + (rcPrice_Type[i].bottom - rcPrice_Type[i].top) / 5,
                szBuf, lstrlen(szBuf));

            lstrcpy(szBuf, orderNodeNow[i]->oi.fldata.szType);
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
        DeleteDC(hdcEmpty);
        DeleteObject(hbmpEmpty);
        DeleteObject(hfTitle);DeleteObject(hfEmpty);
        DeleteObject(hfOrderNum);
        DeleteObject(hbrDelete);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}