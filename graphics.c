/*
*       graphics.c
*       �������л�����ͼ��غ���
*/
#include "header.h"
/*
*   ����: SetFont(HDC hdc, int cHeight, int cWidth, USER_FONT_STYLE fs)
*
*   Ŀ��: �����������
*
*   ���룺	hdc		�豸�������
*			cHeight	��Ҫ���õĸ߶�
*			cWidth	��Ҫ���õĿ��
*			fs		������ʽ����header.h��ָ��
*						��Ϸ�ʽ��[TYPE1]+[TYPE2]+[FONTWEIGHT]
*						Ĭ��(DEFAULT_FONT_STYLE)��΢���źڣ���������ʽ������͸���������ϸ400(FW_NORMAL)
*
*   ���أ�	HFONT   ����������Ҫɾ��
*/
HFONT SetFont(HDC hdc, int cHeight, int cWidth, USER_FONT_STYLE fs)
{
	HFONT hFont;
	LOGFONT logfont;

	logfont.lfHeight = cHeight;
	logfont.lfWidth = cWidth;
	logfont.lfEscapement = 0;//�ַ����нǶ�
	logfont.lfOrientation = 0;//�ַ�������ת�ĽǶ�
	logfont.lfWeight = fs % 1000;
	logfont.lfItalic = fs % 1000000 / 100000;//б��
	logfont.lfUnderline = fs % 10000000 / 1000000;//�»���
	logfont.lfStrikeOut = FALSE;//�Ƿ��ַ�����Ӻ���
	logfont.lfCharSet = DEFAULT_CHARSET;//�ַ���
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;//����ľ�ȷ��
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;//�ü��ַ��ķ���
	logfont.lfQuality = PROOF_QUALITY;//��������
	logfont.lfPitchAndFamily = DEFAULT_PITCH;//ѡ������ļ����������
	switch (fs % 100000 / 10000 * 10000)//���������
	{

	case YAHEI_FONT:default:
		lstrcpy(logfont.lfFaceName, APP_DEFAULT_FONT);
		break;
	}
	//����������
	hFont = CreateFontIndirect(&logfont);
	if (!hFont) return NULL;
	//Ĭ�����ñ���͸��
	if(fs % 100000000 / 10000000 * 10000000 != F_TRANSPARENT_BK_MODE_FALSE) SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, hFont);
	
	return hFont;
}


/*
*   ����: MoveRect(RECT * pRc, int x, int y)
*
*   Ŀ��: �ƶ�����
*
*   ���룺
*       pRc         ָ����ε�ָ��
*       x           xƫ����
*       y           yƫ����
*
*   ���أ�BOOL        �ж��Ƿ�ɹ�
*/
VOID MoveRect(RECT* pRc, int x, int y)
{
    SetRect(pRc, pRc->left + x, pRc->top + y, pRc->right + x, pRc->bottom + y);
}
/*
*   ����: OnPaint(HWND hwnd, HDC hdcMemBk)
*
*   Ŀ��: ��ָ�������У������ڴ�DC���豸�������
*
*   ���룺
*       hwnd         Ŀ�괰�ھ��
*       hdcMem       �ڴ�DC
*
*   ���أ�BOOL        �ж��Ƿ�ɹ�
*/
BOOL OnPaint(HWND hwnd, HDC hdcMem)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top,
        ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top,
        hdcMem, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
    EndPaint(hwnd, &ps);
    return TRUE;
}
/*
*   ����: DefInitWindow
*
*   Ŀ��: Ĭ�ϳ�ʼ�����ڣ���Ҫ��WM_SIZE��Ϣ�µ��ã�Ӧ���ڶ���cxClient,cyClient����������
*
*   ���룺
*       _In_
*            hwnd                   ���ھ��
*            cxClient,cyClient      �ͻ�����С
*            hbrBk                  �ͻ���������ˢ
*       _Out_
*            prcClient              �����ÿͻ�������
*            phdcMemBk              �ͻ���λͼ�ڴ�DC
*            phbmpBk                �ͻ���λͼ
*
*
*   ���أ�BOOL     �����������Ƿ�ɹ�
*/
BOOL DefInitWindow(HWND hwnd, int cxClient, int cyClient, HBRUSH hbrBk, RECT* prcClient, HDC* phdcMemBk, HBITMAP* phbmpBk)
{
    HDC hdc;
    SetRect(prcClient, 0, 0, cxClient, cyClient);
    hdc = GetDC(hwnd);
    //Bk
    *phdcMemBk = CreateCompatibleDC(hdc);
    *phbmpBk = CreateCompatibleBitmap(hdc, cxClient, cyClient);
    SelectObject(*phdcMemBk, *phbmpBk);
    ReleaseDC(hwnd, hdc);
    FillRect(*phdcMemBk, prcClient, hbrBk);
    return TRUE;
}

/*
*   ������DefDelBodyChild(HDC hdcMemBk, HBITMAP hbmpBk)
*   Ŀ�꣺Delete Init�е�������ݣ��ͷ��ڴ�
*   ���룺hdcMemBk     �ͻ����ڴ�DC
*         hbmpBk       �ͻ�������λͼ
*
*   ���أ�BOOL     �����������Ƿ�ɹ�
*/
BOOL DefDelWindow(HDC hdcMemBk, HBITMAP hbmpBk)
{
    DeleteDC(hdcMemBk);
    DeleteObject(hbmpBk);
    return TRUE;
}

BOOL DefInitBodyWindow(HWND hwnd, int cxClient, int cyClient, HBRUSH hbrBk, RECT* prcClient, HDC* phdcMemBk, HBITMAP* phbmpBk)
{
    HDC hdc;
    SetRect(prcClient, 0, 0, cxClient, cyClient);
    hdc = GetDC(hwnd);
    //Bk
    *phdcMemBk = CreateCompatibleDC(hdc);
    *phbmpBk = CreateCompatibleBitmap(hdc, cxClient, cyClient);
    SelectObject(*phdcMemBk, *phbmpBk);
    ReleaseDC(hwnd, hdc);
    FillRect(*phdcMemBk, prcClient, hbrBk);
    return TRUE;
}
/*
    ��д�Ӵ��ڿؼ�
*/
//��ѡ��ť
LRESULT CALLBACK DIY_CheckBoxProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    static SIZE sizeClient[DIY_MAX_WND];
    static HDC hdcMemBk[DIY_MAX_WND];
    static HBITMAP hbmpBk[DIY_MAX_WND];

    static HDC hdcMem[DIY_MAX_WND];
    static HDC hdcMem_click[DIY_MAX_WND];

    static HBITMAP hbmp;
    static HBITMAP hbmp_click;
    static BITMAP bmp;
    static BOOL fClick[DIY_MAX_WND] = { FALSE };
    static UINT num = 0;//��¼�ؼ��ڼ��δ���
    HDC hdc;
    long id = GetWindowLongPtr(hwnd,GWL_ID);
    switch (uMsg)
    {

    case WM_SIZE:
        sizeClient[id].cx = LOWORD(lParam);
        sizeClient[id].cy = HIWORD(lParam);
        if (sizeClient[id].cx == 0 || sizeClient[id].cy == 0) return 0;

        hdc = GetDC(hwnd);
        hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHECKBOX_UNCHECKED));
        hbmp_click = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHECKBOX_CHECKED));
        GetObject(hbmp, sizeof(BITMAP), &bmp);
        //Bk
        hdcMemBk[id] = CreateCompatibleDC(hdc);
        hbmpBk[id] = CreateCompatibleBitmap(hdc, sizeClient[id].cx, sizeClient[id].cy);
        SelectObject(hdcMemBk[id], hbmpBk[id]);

        //UNCHECKED
        hdcMem[id] = CreateCompatibleDC(hdc);
        SelectObject(hdcMem[id], hbmp);

        //CHECKED
        hdcMem_click[id] = CreateCompatibleDC(hdc);
        SelectObject(hdcMem_click[id], hbmp_click);

        ReleaseDC(hwnd, hdc);

        //��ʼ��
        SetStretchBltMode(hdcMemBk[id], HALFTONE);
        StretchBlt(hdcMemBk[id], 0, 0, sizeClient[id].cx, sizeClient[id].cy,
            hdcMem[id], 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

        return 0;
    case WM_LBUTTONUP:case DIY_CHECKBOX_SET_AUTO:
        if (!fClick[id])
        {
            StretchBlt(hdcMemBk[id], 0, 0, sizeClient[id].cx, sizeClient[id].cy,
                hdcMem_click[id], 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
            SendMessage(GetParent(hwnd), DIY_CHECKBOX_COMMAND,
                (WPARAM)(HMENU)id, (LPARAM)CHECKBOX_CHECKED);
            fClick[id] = TRUE;
        }
        else
        {
            StretchBlt(hdcMemBk[id], 0, 0, sizeClient[id].cx, sizeClient[id].cy,
                hdcMem[id], 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
            SendMessage(GetParent(hwnd), DIY_CHECKBOX_COMMAND,
                (WPARAM)(HMENU)id, (LPARAM)CHECKBOX_UNCHECKED);
            fClick[id] = FALSE;
        }
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        return 0;

    case DIY_CHECKBOX_SET:
        switch (lParam)
        {
        case CHECKBOX_CHECKED:
            fClick[id] = TRUE;
            StretchBlt(hdcMemBk[id], 0, 0, sizeClient[id].cx, sizeClient[id].cy,
                hdcMem_click[id], 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
            SendMessage(GetParent(hwnd), DIY_CHECKBOX_COMMAND,
                (WPARAM)(HMENU)id, (LPARAM)CHECKBOX_CHECKED);
            break;
        case CHECKBOX_UNCHECKED:
            fClick[id] = FALSE;
            StretchBlt(hdcMemBk[id], 0, 0, sizeClient[id].cx, sizeClient[id].cy,
                hdcMem[id], 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
            SendMessage(GetParent(hwnd), DIY_CHECKBOX_COMMAND,
                (WPARAM)(HMENU)id, (LPARAM)CHECKBOX_UNCHECKED);
            break;
        }
        InvalidateRect(hwnd, NULL, FALSE);
        UpdateWindow(hwnd);
        return 0;
    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk[id]);
        return 0;

    case WM_DESTROY:
        DefDelWindow(hdcMemBk[id], hbmpBk[id]);
        DeleteDC(hdcMem[id]);DeleteDC(hdcMem_click[id]);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}