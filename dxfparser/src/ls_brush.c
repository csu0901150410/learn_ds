#include "ls_brush.h"

HBRUSH ls_brush_Create(Brush brush) {
	return CreateSolidBrush(brush.color);
}

void ls_brush_Destroy(HBRUSH hBrush) {
	DeleteObject(hBrush);
}