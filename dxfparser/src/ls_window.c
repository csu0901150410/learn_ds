#include "ls_window.h"
#include "ls_shapes.h"
#include "dxf_io.h"

#include <math.h>

#define M_PI 3.14159265358979323846

// 窗口过程函数，处理窗口的消息
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        // 在窗口创建时执行 DXF 读取和写入
        copy_dxf_file("E:\vsLife\\dxf\\source.dxf", "E:\vsLife\\dxf\\target.dxf");
        break;
   
    case WM_DESTROY: // 处理窗口销毁消息
        copy_dxf_file("E:\vsLife\\dxf\\source.dxf", "E:\vsLife\\dxf\\target.dxf");
        
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: // 处理绘画消息
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        lsPoint point, start, end, center, LT_point, RB_point, LT_point_ell, RB_point_ell;

        point = ls_point_create(50, 200);
        start = ls_point_create(100, 200);
        end = ls_point_create(200, 300);
        LT_point = ls_point_create(400, 200);
        RB_point = ls_point_create(570, 300);

        LT_point_ell = ls_point_create(400, 370);
        RB_point_ell = ls_point_create(370, 400);

        center = ls_point_create(650, 200);
        int radius = 100;

        lsPoint points[] = {{100, 100}, {150, 50}, {200, 100}, {175, 150}, {125, 150}};
        int numPionts = 5;

        /**
         * @brief 特别要注意，这里是通过申请堆内存的方式创建线段的，但是后续并没有释放，这会造成内存泄漏，可以考虑增加非申请内存的图元create函数
         * 
         */

        lsLineSegment line = *ls_line_segment_create(start, end);

        // 绘制一个红点
        int marklen = 5;
        lsReal x = marklen * (lsReal)cos(M_PI / 4);
        lsReal y = marklen * (lsReal)sin(M_PI / 4);
        lsPoint pp = {point.x + x, point.y + y}, np = {point.x - x, point.y + y}, 
                nn = {point.x - x, point.y - y}, pn = {point.x + x, point.y - y};
        lsLineSegment seg1 = {pp, nn}, seg2 = {np, pn};
        draw_line(hdc, seg1, RGB(255, 0, 0));
        draw_line(hdc, seg2, RGB(255, 0, 0));
        draw_point(hdc, point, RGB(0, 0, 0));

        // 绘制线段
        draw_line(hdc, line, RGB(0, 255, 0));

        // 绘制矩形
        draw_rect(hdc, LT_point, RB_point, RGB(0, 0, 255));

        // 绘制圆形
        draw_cricle(hdc, center, radius, RGB(255, 0, 0));

        // 绘制椭圆
        draw_ellipss(hdc, LT_point_ell, RB_point_ell, RGB(255, 0, 255));

        // 绘制多边形
        draw_polygon(hdc, points, numPionts, RGB(0, 255, 255));

        EndPaint(hwnd, &ps);

        return 0;
    }

    // 调用默认的窗口过程
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// 注册窗口类
void RegisterWindowClass(HINSTANCE hInstance, const wchar_t *className)
{
    WNDCLASS wc = {0};
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
HWND CreateAppWindow(HINSTANCE hInstance, const wchar_t *className, int nCmdShow)
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
        NULL);

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