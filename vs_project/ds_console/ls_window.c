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

        // ��������л�ͼ����
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    return 0;

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