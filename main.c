/*
******************************************************************
*
* 
* 
*                   �ɻ���Ʊϵͳ
*               ���ԣ�C(��ѭC99��׼)
*               ���뻷����VS2020
*               ����ƽ̨��Windows 7����
*               ���ߣ������� 2021-6
*               ˵�������ô�C Windows SDK��д
*               �汾��1.1
*               
*               
* 
******************************************************************
*/
/*
    main.c
    �������
*/
#include "header.h"
/*������Ϣ*/
HINSTANCE hInst;                                // ��ǰʵ��
UINT cxScreen, cyScreen;                        // �û���Ļ��С
EXIT_WAY e_w = EXIT_IN_LOGIN;                   // Ĭ���˳�����ʽ
UINT cxLogIn, cyLogIn;
UINT cxRegister, cyRegister;
UINT cxMain, cyMain;

/*���ھ��*/
HWND hwndLogIn;
/*���ֶ���*/
//��Ҫ����
TCHAR szAppName[7] = TEXT("�ɻ���Ʊϵͳ");       //������Ҫ����
//��¼����
TCHAR szLogIn[] = TEXT("��¼����");             //��¼����,���ν������ʱ��

/*����ǰ������*/
BOOL RegisterWndClass(HINSTANCE hInstance);

/*�Ӵ��ڿؼ�*/

/*
    �������
*/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    MSG msg;
    RegisterWndClass(hInstance);
    // ִ��Ӧ�ó����ʼ��:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    CreateDirectory(URL_PATH, NULL);

    // ����Ϣѭ��:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    //���ڴ��ͷ�
    DataFreeMemory();
    return (int)msg.wParam;
}

/*
*   ����: RegisterWndClass(HINSTANCE)
*
*   Ŀ��: ע����Ҫ�Ĵ�����
*
*   ���룺����ʵ�����
* 
*   ���أ�BOOL �жϺ��������Ƿ�ɹ�
*/
BOOL RegisterWndClass(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    HBRUSH hBrush;
    /*
        ��¼���洰����
    */
    //��¼������������
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = LogInProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szLogIn;
    if (!RegisterClass(&wndclass)) return FALSE;
    /*
        ��Ҫ������
    */
    //��Ҫ������
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.style = 0;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = hBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    if (!RegisterClass(&wndclass)) return FALSE;

    //DIY�Ӵ��ڿؼ�
    //CHECKBOX
    wndclass.lpfnWndProc = DIY_CheckBoxProc;
    wndclass.lpszClassName = DIY_CHECKBOX;
    if (!RegisterClass(&wndclass)) return FALSE;
    return TRUE;
}


/*
*   ����: InitInstance(HINSTANCE, int)
*
*   Ŀ��: ����ʵ�������������¼����
*
*   ע��:
*        �ڴ˺����У���ȫ�ֱ����б���ʵ�������
*        ��������ʾ��¼���ڡ�
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

    hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

    //����û���Ļ��Ϣ
    cxScreen = GetSystemMetrics(SM_CXSCREEN);
    cyScreen = GetSystemMetrics(SM_CYSCREEN);

    //���洰�ڴ�С
    cxRegister = cxLogIn = WNDSIZE_LOGIN_CX;
    cyRegister = cyLogIn = WNDSIZE_LOGIN_CY;
    cxMain = WNDSIZE_MAINWND_CX;
    cyMain = WNDSIZE_MAINWND_CY;

    //������¼���洰��
    hwndLogIn = CreateWindow(szLogIn,
        TEXT("��¼�ɻ���Ʊϵͳ"),
        WS_OVERLAPPED | WS_SYSMENU | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        cxLogIn, cyLogIn,
        NULL, NULL, hInstance, NULL);

    if (!hwndLogIn)
    {
        return FALSE;
    }

    //������ʾ��¼����
    ShowWindow(hwndLogIn, nCmdShow);
    UpdateWindow(hwndLogIn);
    return TRUE;
}
