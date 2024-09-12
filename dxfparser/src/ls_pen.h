#pragma once
#ifndef LS_PEN_H
#define LS_PEN_H

#include <windows.h>
typedef struct
{
    int style;
    int width;
    COLORREF color;
} Pen;

// 创建画笔
HPEN ls_Pen_Create(Pen pen);

// 释放画笔
void ls_Pen_Destroy(HPEN hPen);

#endif