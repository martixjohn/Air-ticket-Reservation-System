/*
*       body.c
*       ����������Ҫ��������������غ���
*/
#include "mainwnd.h"
int wndtype = 0;
//HWND to Menu
extern HWND hwndList[4], hwndLogo;

//�Ӵ��ھ��
HWND hwndBodyOpt[2][4];

//ע�ᴰ����
static BOOL BodyRegister(HINSTANCE hInstance);          

//���˵���������������
static TCHAR* szOptionName[2][4] = {
    {TEXT("�û���ҳ"),TEXT("����Ʊ"),TEXT("�ҵĶ���"),TEXT("�û�����")},
    {TEXT("����Ա��ҳ"),TEXT("��Ʊ����"),TEXT("�û�����"),TEXT("����Ա����")}
};
//���ڹ���ָ��,C99���������ʼ��
WNDPROC BodyOptWndProc[2][4] = {
    {UserHomeProc,UserBookingProc,UserMineProc,UserSettingProc},
    {AdminHomeProc,AdminFlightProc,AdminUserManageProc,UserSettingProc}
};
/*
*   ����: MenuRegister(HINSTANCE hInstance)
*
*   Ŀ��: ע��Body�����µĴ�����
*
*   ���룺hInstance   ʵ�����
*
*   ���أ�BOOL        �ж��Ƿ�ע��ɹ�
*/
static BOOL BodyRegister(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    HBRUSH hBrush;
    int i,j;
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    wndclass.style = 0;
    
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 4; j++)
        {
            wndclass.lpfnWndProc = BodyOptWndProc[i][j];
            wndclass.lpszClassName = szOptionName[i][j];
            if (!RegisterClass(&wndclass)) return FALSE;
        }
    }
    

    return TRUE;
}

/*
*   ����: MainBodyProc(HWND, UINT, WPARAM, LPARAM)
*
*   Ŀ��: ������Ҫ�����е�Body���ڣ��Ҳࣩ����Ϣ��
*
*   ���룺
*   hWnd       ���ھ��
*   message    ��Ϣ֪ͨ��
*   wParam     ������Ϣ
*   lParam     ������Ϣ
*
*   ���أ�LRESULT ��¼�������Windows
*/
LRESULT CALLBACK MainBodyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //����
    static BOOL fInited = FALSE;
    static int cxClient, cyClient;
    static HDC hdcMemBk;
    static HBITMAP hbmpBk;
    static RECT rcClient;
    TCHAR szBuf[128] = TEXT("\0");
    //��������
    static USERTYPE ut;                 //�û�Ȩ�ޱ���
    int i;
    static HWND hwndMenuNow = NULL;
    static HWND hwndBodyOptNow = NULL, hwndBodyOptPre = NULL;
    static int idMenuClicked,idMenuClickedPre;
    static RECT rcMask;
    HWND hwndMenuClicked;

    switch (uMsg)
    {
    case WM_CREATE:
        // ��ȡ��ǰ�û�����
        ut = DataGetUserType();

        //���ļ���ȡ���ж�������
        DataOrderLinkedListFile(OILSIT_FROM_FILE);
        //��ȡ�����ļ����ݵ��ڴ�
        if (ut == UT_USER)
        {
            //�������ݻ�ȡ
            if (FALSE == DataFlightLinkedListFile(FLLSIT_FROM_FILE))
            {
                if (IDOK == MessageBox(hwnd, TEXT("�������ݲ����ڣ����������ϵ����Ա��"), TEXT("��ʾ��Ϣ"), MB_OK))
                {
                    DestroyWindow(GetParent(hwnd));
                }
            }
            
            //��ʼ����������и��û���������
            DataOrder_GetUserOrderToMem();
            wndtype = 0;
        }
        if (ut == UT_ADMIN || ut == UT_SUPERADMIN)
        {
            //�������ݻ�ȡ
            DataFlightLinkedListFile(FLLSIT_FROM_FILE);
            //���ļ��л�ȡ��ͨ�û��˻�����
            DataAccount_LinkedListFile(UALIST_FROM_FILE);
            //����Ա�˻�����
            DataAccount_LinkedListFile(UALIST_FROM_FILE_ADMIN);
            
            wndtype = 1;
        }

        //ע�ᴰ����
        BodyRegister(hInst);
        for (i = 0; i < 4; i++)
        {
            hwndBodyOpt[wndtype][i] = CreateWindow(szOptionName[wndtype][i],
                NULL,
                WS_CHILD | WS_CLIPCHILDREN,
                0, 0, 0, 0,
                hwnd, (HMENU)i, hInst, NULL);
        }
        SetRect(&rcMask, 0, 0, cxClient, cyClient);
        return 0;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        if (cxClient == 0 || cyClient == 0) return 0;
        if (fInited == FALSE)
        {   
            DefInitWindow(hwnd, cxClient, cyClient, hbrBodyBk, &rcClient, &hdcMemBk, &hbmpBk);
            for (i = 0; i < 4; i++)
            {
                MoveWindow(hwndBodyOpt[wndtype][i], 0, 0, cxClient, cyClient, FALSE);
            }
            ShowWindow(hwndBodyOpt[wndtype][0], SW_SHOWNORMAL);
            hwndBodyOptPre = hwndBodyOpt[wndtype][0];
            fInited = TRUE;
        }

        return 0;
    case WM_U_CLICKCHANGED:
        idMenuClicked = (int)wParam;
        hwndMenuNow = hwndMenuClicked = (HWND)lParam;
        
        if (hwndBodyOptPre != NULL) ShowWindow(hwndBodyOptPre, SW_HIDE);
        
        //��¼������ʾ���Ӵ��ھ������ʾ
        hwndBodyOptNow = hwndBodyOpt[wndtype][idMenuClicked];
        //��ʾ
        idMenuClickedPre = idMenuClicked;
        ShowWindow(hwndBodyOptNow, SW_SHOW);
        //���浱ǰ���Ӵ��ھ���Ա�����
        hwndBodyOptPre = hwndBodyOpt[wndtype][idMenuClicked];

        return 0;

    case WM_PAINT:
        OnPaint(hwnd, hdcMemBk);
        return 0;

    case WM_DESTROY:
        DefDelWindow(hdcMemBk, hbmpBk);
        DeleteObject(hbrBodyBk);
        //�������ж�������
        DataOrderLinkedListFile(OILIST_TO_FILE);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

