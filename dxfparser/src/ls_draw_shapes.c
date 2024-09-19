#include "ls_draw_shapes.h"

#include <assert.h> // for assert
#include <stdlib.h> // for malloc free
#include <math.h>

// 绘制点
void draw_point(HDC hdc, lsPoint point, COLORREF color)
{
    SetPixel(hdc, (int)point.x, (int)point.y, color);
}

// 绘制线段
void draw_line(HDC hdc, lsLineSegment line, COLORREF color)
{
    HPEN hpen = CreatePen(PS_SOLID, 2, color);         // 创建
    HGDIOBJ oldPen = SelectObject(hdc, hpen);          // 选择
    MoveToEx(hdc, (int)line.s.x, (int)line.s.y, NULL); // 使用
    LineTo(hdc, (int)line.e.x, (int)line.e.y);
    SelectObject(hdc, oldPen); // 恢复
    DeleteObject(hpen);
}

// 绘制矩形
void draw_rect(HDC hdc, lsPoint LT_point, lsPoint RB_point, COLORREF color)
{
    HBRUSH hBrush = CreateSolidBrush(color);
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    Rectangle(hdc, (int)LT_point.x, (int)LT_point.y, (int)RB_point.x, (int)RB_point.y);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}

// 绘制圆形
void draw_cricle(HDC hdc, lsPoint center, int radius, COLORREF color)
{
    HBRUSH hBrush = CreateSolidBrush(color);
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    Ellipse(hdc, (int)center.x - radius, (int)center.y + radius, (int)center.x + radius, (int)center.y - radius);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}
// 绘制椭圆
void draw_ellipss(HDC hdc, lsPoint LT_point, lsPoint RB_point, COLORREF color)
{
    HBRUSH hBrush = CreateSolidBrush(color);
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    Ellipse(hdc, (int)LT_point.x, (int)LT_point.y, (int)RB_point.x, (int)RB_point.y);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}

// 绘制圆弧
void draw_arc(HDC hdc, lsArc arc, COLORREF color) {
    int radius = (int)sqrt(pow(arc.c.x - arc.s.x, 2) + pow(arc.c.y - arc.s.y, 2));
    
    HPEN hpen = CreatePen(PS_SOLID, 2, color);         // 创建
    HGDIOBJ oldPen = SelectObject(hdc, hpen);          // 选择
    // 绘制圆弧
    Arc(hdc, (int)arc.c.x - radius, (int)arc.c.y - radius, (int)arc.c.x + radius, (int)arc.c.y + radius, (int)arc.s.x, (int)arc.s.y, (int)arc.e.x, (int)arc.e.y);

    SelectObject(hdc, oldPen); // 恢复
    DeleteObject(hpen);
}

// 绘制闭合多边形
void draw_polygon(HDC hdc, lsPolygon *polygon, COLORREF color)
{
    // 把每一条线画出来
    
}
