#include <math.h>

#include "ls_window.h"
#include "ls_draw_shapes.h"
#include "ls_list.h"

#include "ls_log.h"
#include "ls_dxf.h"
#include "ls_list.h"
#include "ls_entity.h"
#include "ls_polygon.h"
#include "ls_Matrix.h"


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

    ls_dxf_init(dxfReader, "dxf/arc.dxf");
    //ls_dxf_init(dxfReader, "dxf/polygon.dxf");
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

    lsReal windowHeight = ls_box_height(&windowbox);

    

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
                  
                    lsLineSegment seg = *line;
              
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

                    // 因为窗口坐标系y轴是向下的，所以需要变换一下，先沿图元Y轴正向移动窗口高度到达窗口原点，再将Y轴反向
                    seg.s.y = windowHeight - seg.s.y;
                    seg.e.y = windowHeight - seg.e.y;
                    
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
                    lsArc seg = *arc;

                    // 计算出起点终点的弧度，以便平移圆心、放缩半径后重新计算开始点结束点
                    lsReal sangle = ls_arc_get_start_angle(arc);
                    lsReal eangle = ls_arc_get_end_angle(arc);
                    lsReal radius = ls_arc_get_radius(arc);

                    seg.c.x -= entityCenter.x;
                    seg.c.y -= entityCenter.y;
                    seg.c.x *= scale;
                    seg.c.y *= scale;
                    seg.c.x += windowCenter.x;
                    seg.c.y += windowCenter.y;
                    seg.c.x -= windowOrigin.x;
                    seg.c.y -= windowOrigin.y;

                    // 圆心偏移好之后，缩放半径，重新计算起点终点
                    radius *= scale;
                    seg.s.x = seg.c.x + radius * cos(sangle);
                    seg.s.y = seg.c.y + radius * sin(sangle);
                    seg.e.x = seg.c.x + radius * cos(eangle);
                    seg.e.y = seg.c.y + radius * sin(eangle);

                    // 因为窗口坐标系y轴是向下的，所以需要变换一下，先沿图元Y轴正向移动窗口高度到达窗口原点，再将Y轴反向
                    seg.s.y = windowHeight - seg.s.y;
                    seg.e.y = windowHeight - seg.e.y;
                    seg.c.y = windowHeight - seg.c.y;

                    draw_arc(hdc, seg, RGB(255, 255, 255));



                    // 记录日志信息
                    ls_log_info("draw arc : center(%f, %f), start(%f, %f), end(%f, %f)\n",
                        seg.c.x, seg.c.y, seg.s.x, seg.s.y, seg.e.x, seg.e.y);
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

                        // 因为窗口坐标系y轴是向下的，所以需要变换一下，先沿图元Y轴正向移动窗口高度到达窗口原点，再将Y轴反向
                        seg.s.y = windowHeight - seg.s.y;
                        seg.e.y = windowHeight - seg.e.y;
                        
                        ls_log_info("draw line : s(%f, %f), e(%f, %f)\n", seg.s.x, seg.s.y, seg.e.x, seg.e.y);
                        draw_line(hdc, seg, RGB(255, 0, 0));
                    }
                }
            }
            break;
        }


        // 计算并绘制图元的 Box
        lsBox entbox = ls_entity_get_box(entity);
        lsPolygon* polygon = NULL;
        ls_polygon_from_box(&entbox, &polygon);

        if (polygon != NULL)
        {
            // 遍历 polygon，绘制每条边
            for (lsListIterator polyIt = ls_list_iterator_start(polygon->edges); !ls_list_iterator_done(&polyIt); ls_list_iterator_step(&polyIt))
            {
                lsLineSegment* pSeg = (lsLineSegment*)ls_list_iterator_get_data(&polyIt);
                lsLineSegment seg = *pSeg;

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

                // 因为窗口坐标系y轴是向下的，所以需要变换一下，先沿图元Y轴正向移动窗口高度到达窗口原点，再将Y轴反向
                seg.s.y = windowHeight - seg.s.y;
                seg.e.y = windowHeight - seg.e.y;

                ls_log_info("draw box line : s(%f, %f), e(%f, %f)\n", seg.s.x, seg.s.y, seg.e.x, seg.e.y);
                draw_line(hdc, seg, RGB(0, 255, 0)); // 绘制图元的 box 边界
            }

            // 销毁 polygon，释放内存
            ls_polygon_destroy(&polygon);
        }
    }
}
