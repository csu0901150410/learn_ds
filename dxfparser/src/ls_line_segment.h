#pragma once

#include "ls_vector.h"
#include "ls_box.h"

typedef struct
{
    lsPoint s;
    lsPoint e;
} lsLineSegment;

lsLineSegment *ls_line_segment_create(lsVector start, lsVector end);

void ls_line_segment_destroy(lsLineSegment **root);

lsBox ls_line_segment_get_box(const lsLineSegment *line);
