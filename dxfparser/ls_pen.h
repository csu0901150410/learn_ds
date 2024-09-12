#pragma once
#ifndef LS_PEN_H
#define LS_PEN_H

#include <windows.h>
typedef struct {
    int style;     
    int width;    
    COLORREF color; 
} Pen;

// ��������
HPEN ls_Pen_Create(Pen pen);

// �ͷŻ���
void ls_Pen_Destroy(HPEN hPen);

#endif