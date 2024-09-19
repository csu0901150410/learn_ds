#include "ls_box.h"

void init_box(lsBox* box) {
    box->minX = box->maxX = 0;
    box->minY = box->maxY = 0;
}

void update_box(lsBox* box, lsPoint point) {
    if (point.x < box->minX) box->minX = point.x;
    if (point.x > box->maxX) box->maxX = point.x;
    if (point.y < box->minY) box->minY = point.y;
    if (point.y > box->maxY) box->maxY = point.y;
}

