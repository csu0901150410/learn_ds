#pragma once

#include "ls_vector.h"

typedef struct
{
    lsPoint s;
    lsPoint e;
} lsLineSegment;

lsLineSegment *ls_line_segment_create(lsVector start, lsVector end);

void ls_line_segment_destroy(lsLineSegment **root);
