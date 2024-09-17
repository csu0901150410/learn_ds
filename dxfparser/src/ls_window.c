#include <math.h>

#include "ls_window.h"
#include "ls_shapes.h"
#include "dxf_io.h"
#include "ls_list.h"

#include "dxf_parser.h"

#include "ls_log.h"
#include "ls_dxf.h"
#include "ls_list.h"
#include "ls_entity.h"

#define M_PI 3.14159265358979323846

// 窗口过程函数，处理窗口的消息
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // 获取存储在窗口实例数据中的dxf结构体指针，以便绘制和释放使用
    lsDxf *dxfReader = (lsDxf*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (uMsg)
    {
    case WM_CREATE:
    {
        // 屏幕尺寸
        int screenw, screenh;
        screenw = GetSystemMetrics(SM_CXSCREEN);
        screenh = GetSystemMetrics(SM_CYSCREEN);

        // 窗口尺寸
        RECT rect;
        int windoww, windowh;
        GetWindowRect(hwnd, &rect);
        windoww = rect.right - rect.left;
        windowh = rect.bottom - rect.top;

        // 居中
        rect.left = (screenw - windoww) / 2;
        rect.top = (screenh - windowh) / 2;
        SetWindowPos(hwnd, NULL, rect.left, rect.top, windoww, windowh, SWP_NOZORDER);
    
        // 窗口创建的时候解析dxf文件
        ls_window_read_dxf(hwnd);
    }
    break;

    // 处理窗口销毁消息
    case WM_DESTROY:
    {
        ls_dxf_destroy(&dxfReader);
        PostQuitMessage(0);
    }
    break;

    // 处理绘画消息
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        ls_window_draw_shapes(hdc, dxfReader);

        EndPaint(hwnd, &ps);
        return 0;
    }
    break;

    // 调用默认的窗口过程
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

// 注册窗口类
bool RegisterWindowClass(HINSTANCE hInstance, const TCHAR *className)
{
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (!RegisterClass(&wc))
    {
        ls_log_info("RegisterClass failed\n");
        return false;
    }
    return true;
}

// 创建应用程序窗口
HWND CreateAppWindow(HINSTANCE hInstance, const TCHAR *className, int nCmdShow)
{
    HWND hwnd = CreateWindowEx(
        0,
        className,
        _T("Dxf Parser"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
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
        ls_log_info("CreateWindowEx failed\n");
    }
    return hwnd;
}

void ls_window_read_dxf(HWND hwnd)
{
    lsDxf *dxfReader = ls_dxf_create();

    ls_dxf_init(dxfReader, "dxf/bridge.dxf");

    ls_dxf_parse(dxfReader);

    // ls_dxf_destroy(&dxfReader);

    // 将dxf指针保存到窗口
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)dxfReader);
}

void ls_window_draw_shapes(HDC hdc, const lsDxf *dxf)
{
    if (NULL == dxf || NULL == dxf->list)
        return;

    // 遍历链表，取出图元，进行绘制
    for (lsListIterator it = ls_list_iterator_start(dxf->list); !ls_list_iterator_done(&it); ls_list_iterator_step(&it))
    {
        lsEntity *entity = ls_list_iterator_get_data(&it);
        if (NULL == entity)
            continue;

        switch (entity->type)
        {
            case enum_geo_segment:
            {
                lsLineSegment *line = (lsLineSegment*)entity->entity;
                if (NULL != line)
                {
                    //ls_log_info("draw line : s(%f, %f), e(%f, %f)\n", line->s.x, line->s.y, line->e.x, line->e.y);
                    // 临时版本，随便找个放大倍数和偏移坐标，只是为了看到图形，后续应该根据边界矩形来确定放大倍数和偏移坐标
                    // 后续图元解析、显示可以参考线段这个框架来做
                    lsLineSegment seg = *line;
                    seg.s.x *= 1000;
                    seg.s.y *= 1000;
                    seg.e.x *= 1000;
                    seg.e.y *= 1000;
					seg.s.x += 400;
					seg.s.y += 400;
					seg.e.x += 400;
					seg.e.y += 400;
                    ls_log_info("draw line : s(%f, %f), e(%f, %f)\n", seg.s.x, seg.s.y, seg.e.x, seg.e.y);
                    draw_line(hdc, seg, RGB(255, 0, 0));
                }
            }
        }
    }
}
