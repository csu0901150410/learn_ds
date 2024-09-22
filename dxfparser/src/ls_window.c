#include <math.h>

#include "ls_window.h"
#include "ls_draw_shapes.h"
#include "ls_list.h"

#include "ls_log.h"
#include "ls_dxf.h"
#include "ls_list.h"
#include "ls_entity.h"
#include "ls_polygon.h"
#include "ls_matrix.h"

#define M_PI 3.14159265358979323846

// 窗口过程函数，处理窗口的消息
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 获取存储在窗口实例数据中的dxf结构体指针，以便绘制和释放使用
	lsDxf* dxfReader = (lsDxf*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

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
bool RegisterWindowClass(HINSTANCE hInstance, const TCHAR* className)
{
	WNDCLASS wc = { 0 };
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
HWND CreateAppWindow(HINSTANCE hInstance, const TCHAR* className, int nCmdShow)
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
	lsDxf* dxfReader = ls_dxf_create();

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

void test_matrix(HDC hdc, lsReal w, lsReal h)
{
	// 重写这个变换的过程
	lsLineSegment seg = { {w / 4, h / 2}, {w * 3 / 4, h / 2} };
	draw_line(hdc, seg, CLR_WHITE);

	lsPoint mid = ls_line_segment_get_mid(&seg);
	lsMatrix tozero = ls_matrix_set_translate(-mid.x, -mid.y);
	lsMatrix matscale = ls_matrix_set_scale(0.5, 0.5);
	lsMatrix matrot = ls_matrix_set_rotate360(90);
	lsMatrix tocenter = ls_matrix_set_translate(mid.x, mid.y);
	lsMatrix transform = ls_matrix_multiply_n(4, &tozero, &matscale, &matrot, &tocenter);

	lsLineSegment trans_seg = seg;
	ls_line_segment_transform(&trans_seg, &transform);
	draw_line(hdc, trans_seg, CLR_RED);
}

void ls_window_draw_shapes(HWND hwnd, HDC hdc, const lsDxf* dxf)
{
	if (NULL == dxf || NULL == dxf->list)
		return;

	// 取图元边界矩形，计算缩放倍数和偏移向量
	lsBox box;
	ls_box_init(&box);
	for (lsListIterator it = ls_list_iterator_start(dxf->list); !ls_list_iterator_done(&it); ls_list_iterator_step(&it))
	{
		const lsEntity* entity = (lsEntity*)ls_list_iterator_get_data(&it);
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

	//test_matrix(hdc, ls_box_width(&windowbox), ls_box_height(&windowbox));

	// 计算图元坐标变换到屏幕坐标的缩放系数，该系数由屏幕坐标/图元坐标得到
	lsReal entw = ls_box_width(&box), enth = ls_box_height(&box);
	if (0 == entw || 0 == enth)
		return;
	lsReal scalex = ls_box_width(&windowbox) / entw;
	lsReal scaley = ls_box_height(&windowbox) / enth;
	// 取较小者进行放大，如果取较大者进行放大，有一边会超出
	lsReal scale = MIN(scalex, scaley) * 0.9;// 乘一个系数，不然边缘看不到

	// 计算中心点和原点
	lsPoint entityCenter = ls_box_center(&box);
	lsPoint windowCenter = ls_box_center(&windowbox);
	lsPoint windowOrigin = { windowbox.left, windowbox.bottom };

	lsReal windowHeight = ls_box_height(&windowbox);

	// 遍历链表，取出图元，进行绘制
	for (lsListIterator it = ls_list_iterator_start(dxf->list); !ls_list_iterator_done(&it); ls_list_iterator_step(&it))
	{
		lsEntity* entity = ls_list_iterator_get_data(&it);
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

				lsMatrix transformMatrix = generate_transform_matrix(entityCenter, scale, windowCenter, windowOrigin, windowHeight);
				ls_line_segment_transform(&seg, &transformMatrix);

				ls_log_info("draw line : s(%f, %f), e(%f, %f)\n", seg.s.x, seg.s.y, seg.e.x, seg.e.y);
				draw_line(hdc, seg, CLR_RED);
			}
		}
		break;

		case enum_geo_arc:
		{
			lsArc* arc = (lsArc*)entity->entity;
			if (NULL != arc)
			{
				lsArc seg = *arc;

				lsMatrix transformMatrix = generate_transform_matrix(entityCenter, scale, windowCenter, windowOrigin, windowHeight);
				ls_arc_transform(&seg, &transformMatrix);

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
					
					lsMatrix transformMatrix =generate_transform_matrix(entityCenter,scale, windowCenter, windowOrigin, windowHeight);
					ls_line_segment_transform(&seg, &transformMatrix);

					ls_log_info("draw line : s(%f, %f), e(%f, %f)\n", seg.s.x, seg.s.y, seg.e.x, seg.e.y);
					draw_line(hdc, seg, CLR_RED);
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

				lsMatrix transformMatrix = generate_transform_matrix(entityCenter, scale, windowCenter, windowOrigin, windowHeight);
				ls_line_segment_transform(&seg, &transformMatrix);

				ls_log_info("draw box line : s(%f, %f), e(%f, %f)\n", seg.s.x, seg.s.y, seg.e.x, seg.e.y);
				draw_line(hdc, seg, CLR_GREEN); // 绘制图元的 box 边界
			}
		}

		// 销毁 polygon，释放内存
		ls_polygon_destroy(&polygon);
	}
}

lsMatrix generate_transform_matrix(lsPoint entityCenter, lsReal scale, lsPoint windowCenter, lsPoint windowOrigin, lsReal windowHeight)
{
	lsMatrix tozero = ls_matrix_set_translate(-entityCenter.x, -entityCenter.y);

	lsMatrix matscale = ls_matrix_set_scale(scale, scale);

	// 图元移动到屏幕中心
	lsMatrix tocenter = ls_matrix_set_translate(windowCenter.x, windowCenter.y);

	// 4. 将窗口坐标系中的图元移到窗口原点
	lsMatrix towindoworigin = ls_matrix_set_translate(-windowOrigin.x, -windowOrigin.y);

	lsMatrix flip_y = ls_matrix_set_scale(1.0, -1.0);  // Y轴翻转
	lsMatrix translate_y = ls_matrix_set_translate(0, windowHeight);  // 向下平移窗口高度

	return ls_matrix_multiply_n(6, &tozero, &matscale, &tocenter, &towindoworigin, &flip_y, &translate_y);
}