#pragma once
#ifndef LS_SHAPES_H
#define LS_SHAPES_H

#include "src/ls_vector.h"
#include "src/ls_line_segment.h"
#include "src/ls_arc.h"
#include "windows.h"

// ���Ƶ�
void draw_point(HDC hdc, lsPoint point, COLORREF color);

// �����߶�
void draw_line(HDC hdc, lsLineSegment line, COLORREF color);

// ���ƾ���
void draw_rect(HDC hdc, lsPoint LT_point, lsPoint RB_point, COLORREF color);

// ����Բ��
void draw_cricle(HDC hdc, lsPoint center, int radius, COLORREF color);

// ������Բ
void draw_ellipss(HDC hdc, lsPoint LT_point, lsPoint RB_point, COLORREF color);

// ���ƶ����
void draw_polygon(HDC hdc, POINT* points, int numPionts, COLORREF color);

#endif // !LS_SHAPES_H
