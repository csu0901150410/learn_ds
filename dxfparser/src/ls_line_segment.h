#pragma once

#include "ls_vector.h"
#include "ls_box.h"
#include "ls_matrix.h"

typedef struct
{
    lsPoint s;
    lsPoint e;
} lsLineSegment;

lsLineSegment *ls_line_segment_create(lsVector start, lsVector end);

void ls_line_segment_destroy(lsLineSegment **root);

lsBox ls_line_segment_get_box(const lsLineSegment *line);

lsPoint ls_line_segment_get_mid(const lsLineSegment *line);

void ls_line_segment_transform(lsLineSegment *line, const lsMatrix *matrix);

void ls_matrix_transform_line(lsMatrix* matrix, lsLineSegment* line);
