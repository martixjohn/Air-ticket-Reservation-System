/*
*       graphics.c
*       包含所有基础绘图相关函数
*/
#include "header.h"
/*
*   函数: SetFont(HDC hdc, int cHeight, int cWidth, USER_FONT_STYLE fs)
*
*   目标: 快捷设置字体
*
*   输入：	hdc		设备环境句柄
*			cHeight	需要设置的高度
*			cWidth	需要设置的宽度
*			fs		字体样式，在header.h中指定
*						组合方式：[TYPE1]+[TYPE2]+[FONTWEIGHT]
*						默认(DEFAULT_FONT_STYLE)：微软雅黑，无特殊样式，背景透明，字体粗细400(FW_NORMAL)
*
*   返回：	HFONT   字体句柄，需要删除
*/
HFONT SetFont(HDC hdc, int cHeight, int cWidth, USER_FONT_STYLE fs)
{
	HFONT hFont;
	LOGFONT logfont;

	logfont.lfHeight = cHeight;
	logfont.lfWidth = cWidth;
	logfont.lfEscapement = 0;//字符排列角度
	logfont.lfOrientation = 0;//字符本身旋转的角度
	logfont.lfWeight = fs % 1000;
	logfont.lfItalic = fs % 1000000 / 100000;//斜体
	logfont.lfUnderline = fs % 10000000 / 1000000;//下划线
	logfont.lfStrikeOut = FALSE;//是否字符中央加横线
	logfont.lfCharSet = DEFAULT_CHARSET;//字符集
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;//字体的精确度
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;//裁剪字符的方法
	logfont.lfQuality = PROOF_QUALITY;//字体质量
	logfont.lfPitchAndFamily = DEFAULT_PITCH;//选择字体的间距和字体家族
	switch (fs % 100000 / 10000 * 10000)//字体的名称
	{

	case YAHEI_FONT:default:
		lstrcpy(logfont.lfFaceName, APP_DEFAULT_FONT);
		break;
	}
	//创建字体句柄
	hFont = CreateFontIndirect(&logfont);
	if (!hFont) return NULL;
	//默认设置背景透明
	if(fs % 100000000 / 10000000 * 10000000 != F_TRANSPARENT_BK_MODE_FALSE) SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, hFont);
	
	return hFont;
}


/*
*   函数: MoveRect(RECT * pRc, int x, int y)
*
*   目标: 移动矩形
*
*   输入：
*       pRc         指向矩形的指针
*       x           x偏移量
*       y           y偏移量
*
*   返回：BOOL        判断是否成功
*/
VOID MoveRect(RECT* pRc, int x, int y)
{
    SetRect(pRc, pRc->left + x, pRc->top + y, pRc->right + x, pRc->bottom + y);
}
/*
*   函数: OnPaint(HWND hwnd, HDC hdcMemBk)
*
*   目标: 在指定窗口中，加载内存DC到设备环境句柄
*
*   输入：
*       hwnd         目标窗口句柄
*       hdcMem       内存DC
*
*   返回：BOOL        判断是否成功
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
*   函数: DefInitWindow
*
*   目标: 默认初始化窗口，需要在WM_SIZE消息下调用，应该在定义cxClient,cyClient后立即调用
*
*   输入：
*       _In_
*            hwnd                   窗口句柄
*            cxClient,cyClient      客户区大小
*            hbrBk                  客户区背景画刷
*       _Out_
*            prcClient              待设置客户区矩形
*            phdcMemBk              客户区位图内存DC
*            phbmpBk                客户区位图
*
*
*   返回：BOOL     代表函数调用是否成功
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
*   函数：DefDelBodyChild(HDC hdcMemBk, HBITMAP hbmpBk)
*   目标：Delete Init中的相关内容，释放内存
*   输入：hdcMemBk     客户区内存DC
*         hbmpBk       客户区背景位图
*
*   返回：BOOL     代表函数调用是否成功
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
    自写子窗口控件
*/
//单选按钮
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
    static UINT num = 0;//记录控件第几次创建
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

        //初始化
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