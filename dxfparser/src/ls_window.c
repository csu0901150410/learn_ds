#include <math.h>

#include "ls_window.h"
#include "ls_draw_shapes.h"
#include "ls_list.h"

#include "ls_log.h"
#include "ls_dxf.h"
#include "ls_list.h"
#include "ls_entity.h"
#include "ls_polygon.h"


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

        ls_window_draw_shapes(hwnd, hdc, dxfReader);

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

    // ls_dxf_init(dxfReader, "dxf/arc.dxf");
    ls_dxf_init(dxfReader, "dxf/polygon.dxf");
    ls_dxf_parse(dxfReader);
    

    // ls_dxf_destroy(&dxfReader);

    // 将dxf指针保存到窗口
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)dxfReader);
}

lsBox ls_window_get_window_box(HWND hwnd)
{
    // 窗口尺寸
    RECT rect;
    // GetWindowRect(hwnd, &rect);
    GetClientRect(hwnd, &rect);// 应该是获取绘图区域的矩形
    
    lsBox box;
    box.left = (lsReal)rect.left;
    box.right = (lsReal)rect.right;
    box.bottom = (lsReal)rect.top;
    box.top = (lsReal)rect.bottom;// 注意RECT的坐标系和lsBox中的坐标系是不同的
    
    return box;
}

// 尽量不要定义全局变量

void ls_window_draw_shapes(HWND hwnd, HDC hdc, const lsDxf *dxf)
{
    if (NULL == dxf || NULL == dxf->list)
        return;

    // 取图元边界矩形，计算缩放倍数和偏移向量
    lsBox box;
    ls_box_init(&box);
    for (lsListIterator it = ls_list_iterator_start(dxf->list); !ls_list_iterator_done(&it); ls_list_iterator_step(&it))
    {
        const lsEntity *entity = (lsEntity*)ls_list_iterator_get_data(&it);
        if (NULL == entity)
            continue;
            
        lsBox entbox = ls_entity_get_box(entity);
        ls_box_combine(&box, &entbox);
    }

    if (!ls_box_valid(&box))
        return;
        
    lsBox windowbox = ls_window_get_window_box(hwnd);
    if (!ls_box_valid(&windowbox))
        return;

    // 计算图元坐标变换到屏幕坐标的缩放系数，该系数由屏幕坐标/图元坐标得到
    lsReal entw = ls_box_width(&box), enth = ls_box_height(&box);
    if (0 == entw || 0 == enth)
        return;
    lsReal scalex = ls_box_width(&windowbox) / entw;
    lsReal scaley = ls_box_height(&windowbox) / enth;
    // 取较小者进行放大，如果取较大者进行放大，有一边会超出
    lsReal scale = MIN(scalex, scaley) * 0.9;// 乘一个系数，不然边缘看不到

    // 看来还要定义一些向量运算和矩阵运算的函数
    lsPoint entityCenter = ls_box_center(&box);
    lsPoint windowCenter = ls_box_center(&windowbox);
    lsPoint windowOrigin = {windowbox.left, windowbox.bottom};

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
                lsLineSegment* line = (lsLineSegment*)entity->entity;
                if (NULL != line)
                {
                    //ls_log_info("draw line : s(%f, %f), e(%f, %f)\n", line->s.x, line->s.y, line->e.x, line->e.y);
                    // 临时版本，随便找个放大倍数和偏移坐标，只是为了看到图形，后续应该根据边界矩形来确定放大倍数和偏移坐标
                    // 后续图元解析、显示可以参考线段这个框架来做
                    lsLineSegment seg = *line;
                    // seg.s.x *= 1000;
                    // seg.s.y *= 1000;
                    // seg.e.x *= 1000;
                    // seg.e.y *= 1000;
                    // seg.s.x += 400;
                    // seg.s.y += 400;
                    // seg.e.x += 400;
                    // seg.e.y += 400;

                    // 1.全部图元中心移动到原点
                    // 2.图元缩放
                    // 3.图元移动到屏幕中心
                    // 4.屏幕坐标转窗口坐标

                    seg.s.x -= entityCenter.x;
                    seg.s.y -= entityCenter.y;
                    seg.e.x -= entityCenter.x;
                    seg.e.y -= entityCenter.y;
                    seg.s.x *= scale;
                    seg.s.y *= scale;
                    seg.e.x *= scale;
                    seg.e.y *= scale;
                    seg.s.x += windowCenter.x;
                    seg.s.y += windowCenter.y;
                    seg.e.x += windowCenter.x;
                    seg.e.y += windowCenter.y;
                    seg.s.x -= windowOrigin.x;
                    seg.s.y -= windowOrigin.y;
                    seg.e.x -= windowOrigin.x;
                    seg.e.y -= windowOrigin.y;
                    
                    ls_log_info("draw line : s(%f, %f), e(%f, %f)\n", seg.s.x, seg.s.y, seg.e.x, seg.e.y);
                    draw_line(hdc, seg, RGB(255, 0, 0));
                }
            }
            break;

            case enum_geo_arc:
            {
                lsArc* arc = (lsArc*)entity->entity;
                if (NULL != arc)
                {
                    lsArc scaledArc = *arc;
                    scaledArc.s.x *= 0.1f;
                    scaledArc.s.y *= 0.1f;
                    scaledArc.e.x *= 0.1f;
                    scaledArc.e.y *= 0.1f;
                    scaledArc.c.x *= 0.1f;
                    scaledArc.c.y *= 0.1f;
                    scaledArc.s.x += 0;
                    scaledArc.s.y += 0;
                    scaledArc.e.x += 0;
                    scaledArc.e.y += 0;
                    scaledArc.c.x += 300;
                    scaledArc.c.y += 200;
                    draw_arc(hdc, scaledArc, RGB(0, 255, 0));
                }
            }
            break;

            case enum_geo_polygon:
            {
                lsPolygon* polygon = (lsPolygon*)entity->entity;
                if (NULL != polygon)
                {
                    // draw_polygon(hdc, polygon, RGB(0, 0, 255));
                    for (lsListIterator it = ls_list_iterator_start(polygon->edges); !ls_list_iterator_done(&it); ls_list_iterator_step(&it))
                    {
                        lsLineSegment* pSeg = (lsLineSegment*)ls_list_iterator_get_data(&it);

                        lsLineSegment seg = *pSeg;
                        
                        // seg.s.x *= 0.1f;
                        // seg.s.y *= 0.1f;
                        // seg.e.x *= 0.1f;
                        // seg.e.y *= 0.1f;
                        // seg.s.x += 0;
                        // seg.s.y += 0;
                        // seg.e.x += 0;
                        // seg.e.y += 0;
                        
                        // 1.全部图元中心移动到原点
                        // 2.图元缩放
                        // 3.图元移动到屏幕中心
                        // 4.屏幕坐标转窗口坐标

                        seg.s.x -= entityCenter.x;
                        seg.s.y -= entityCenter.y;
                        seg.e.x -= entityCenter.x;
                        seg.e.y -= entityCenter.y;
                        seg.s.x *= scale;
                        seg.s.y *= scale;
                        seg.e.x *= scale;
                        seg.e.y *= scale;
                        seg.s.x += windowCenter.x;
                        seg.s.y += windowCenter.y;
                        seg.e.x += windowCenter.x;
                        seg.e.y += windowCenter.y;
                        seg.s.x -= windowOrigin.x;
                        seg.s.y -= windowOrigin.y;
                        seg.e.x -= windowOrigin.x;
                        seg.e.y -= windowOrigin.y;
                        
                        ls_log_info("draw line : s(%f, %f), e(%f, %f)\n", seg.s.x, seg.s.y, seg.e.x, seg.e.y);
                        draw_line(hdc, seg, RGB(255, 0, 0));
                    }
                }
            }
            break;
        }
    }
}
