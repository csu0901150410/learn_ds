#include "ls_shapes.h"

// 绘制点
void draw_point(HDC hdc, lsPoint point, COLORREF color) {
	SetPixel(hdc,point.x,point.y,color);
}

// 绘制线段
void draw_line(HDC hdc, lsLineSegment line, COLORREF color) {
	HPEN hpen = CreatePen(PS_SOLID,2,color);//创建
	HGDIOBJ oldPen = SelectObject(hdc,hpen);//选择
	MoveToEx(hdc, line.s.x, line.s.y,NULL);//使用
	LineTo(hdc, line.e.x, line.e.y);
	SelectObject(hdc, oldPen);//恢复
	DeleteObject(hpen);
}

// 绘制矩形
void draw_rect(HDC hdc, lsPoint LT_point, lsPoint RB_point, COLORREF color) {
	HBRUSH hBrush = CreateSolidBrush(color);
	HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
	Rectangle(hdc, LT_point.x , LT_point.y, RB_point.x, RB_point.y);
	SelectObject(hdc,oldBrush);
	DeleteObject(hBrush);
}

// 绘制圆形
void draw_cricle(HDC hdc, lsPoint center, int radius, COLORREF color) {
	HBRUSH hBrush = CreateSolidBrush(color);
	HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
	Ellipse(hdc,center.x-radius,center.y+radius,center.x+radius,center.y-radius);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}
// 绘制椭圆
void draw_ellipss(HDC hdc, lsPoint LT_point, lsPoint RB_point, COLORREF color){
	HBRUSH hBrush = CreateSolidBrush(color);
	HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
	Ellipse(hdc, LT_point.x, LT_point.y, RB_point.x, RB_point.y);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}
// 绘制多边形
void draw_polygon(HDC hdc, POINT* points, int numPionts, COLORREF color) {
	HBRUSH hBrush = CreateSolidBrush(color);
	HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
	Polygon(hdc,points,numPionts);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}

