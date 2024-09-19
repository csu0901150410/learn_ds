#pragma once

#include "ls_list.h"

#include "ls_line_segment.h"
#include "ls_box.h"

typedef struct
{
    lsList *edges;// ���ɶ���εı���
} lsPolygon;

lsPolygon *ls_polygon_create();

void ls_polygon_destroy(lsPolygon **root);

void ls_polygon_append_seg(lsPolygon *polygon, lsLineSegment seg);

lsBox ls_polygon_get_box(const lsPolygon *polygon);
