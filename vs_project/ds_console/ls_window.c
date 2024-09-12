#include "ls_window.h"

// ���ڹ��̺����������ڵ���Ϣ
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:  // ������������Ϣ
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:    // ����滭��Ϣ
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // ��������л�ͼ����(������ѡ��ʹ�á��ָ�������)
        HPEN hPen = CreatePen(PS_SOLID,2,RGB(255,0,0));//����
        HGDIOBJ oldPen = SelectObject(hdc,hPen);

        HBRUSH hBrush = CreateSolidBrush(RGB(0,255,0));
        HGDIOBJ oldBursh = SelectObject(hdc,hBrush);

        //���ƾ���
        Rectangle(hdc,50,50,100,100);

        //������Բ
        Ellipse(hdc,200,50,400,150);

        //����ֱ��
        MoveToEx(hdc,50,200,NULL);//���
        LineTo(hdc,200,300);

        SelectObject(hdc,oldPen);
        SelectObject(hdc,oldBursh);


        DeleteObject(hPen);
        DeleteObject(hBrush);
 
        EndPaint(hwnd, &ps);

        return 0;
    }
 

    // ����Ĭ�ϵĴ��ڹ���
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
 
}

// ע�ᴰ����
void RegisterWindowClass(HINSTANCE hInstance, const wchar_t* className)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc))
    {
        OutputDebugString(L"RegisterClass failed");
    }
    RegisterClass(&wc);
}

// ����Ӧ�ó��򴰿�
HWND CreateAppWindow(HINSTANCE hInstance, const wchar_t* className, int nCmdShow)
{
    HWND hwnd = CreateWindowEx(
        0,
        className,
        L"My First Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
   

    if (hwnd != NULL)
    {
        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);
    }
    else
    {
        OutputDebugString(L"CreateWindowEx failed");
    }
     return hwnd;
}