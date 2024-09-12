#pragma once
#ifndef LS_BRUSH_H
#define LS_BRUSH_H

#include "windows.h"

typedef struct
{
    COLORREF color;
} Brush;

HBRUSH ls_brush_Create(Brush brush);

void ls_brush_Destroy(HBRUSH hbrush);
#endif //
