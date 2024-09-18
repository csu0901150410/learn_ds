#pragma once

#include "ls_list.h"

#include "ls_line_segment.h"

typedef struct
{
    lsList *edges;// ���ɶ���εı���
} lsPolygon;

lsPolygon *ls_polygon_create();

void ls_polygon_destroy(lsPolygon **root);

void ls_polygon_append_seg(lsPolygon *polygon, lsLineSegment seg);
