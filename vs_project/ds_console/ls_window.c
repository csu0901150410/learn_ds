#include "ls_window.h"

// 窗口过程函数，处理窗口的消息
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:  // 处理窗口销毁消息
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:    // 处理绘画消息
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // 在这里进行绘图操作(创建、选择、使用、恢复、销毁)
        HPEN hPen = CreatePen(PS_SOLID,2,RGB(255,0,0));//画笔
        HGDIOBJ oldPen = SelectObject(hdc,hPen);

        HBRUSH hBrush = CreateSolidBrush(RGB(0,255,0));
        HGDIOBJ oldBursh = SelectObject(hdc,hBrush);

        //绘制矩形
        Rectangle(hdc,50,50,100,100);

        //绘制椭圆
        Ellipse(hdc,200,50,400,150);

        //绘制直线
        MoveToEx(hdc,50,200,NULL);//起点
        LineTo(hdc,200,300);

        SelectObject(hdc,oldPen);
        SelectObject(hdc,oldBursh);


        DeleteObject(hPen);
        DeleteObject(hBrush);
 
        EndPaint(hwnd, &ps);

        return 0;
    }
 

    // 调用默认的窗口过程
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
 
}

// 注册窗口类
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

// 创建应用程序窗口
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