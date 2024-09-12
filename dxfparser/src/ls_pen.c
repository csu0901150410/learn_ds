#include "ls_pen.h"

HPEN ls_Pen_Create(Pen pen)
{
    return CreatePen(pen.style, pen.width, pen.color);
}

void ls_Pen_Destroy(HPEN hPen)
{
    DeleteObject(hPen);
}