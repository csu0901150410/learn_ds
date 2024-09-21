#include "ls_line_segment.h"

#include <stdlib.h> // for malloc/min/max
#include <assert.h> // for assert

lsLineSegment *ls_line_segment_create(lsVector start, lsVector end)
{
    lsLineSegment *seg = (lsLineSegment *)malloc(sizeof(lsLineSegment));
    assert(seg);

    seg->s = start;
    seg->e = end;
    return seg;
}

void ls_line_segment_destroy(lsLineSegment **root)
{
    if (NULL == root || NULL == *root)
        return;
    free(*root);
    *root = NULL;
}

lsBox ls_line_segment_get_box(const lsLineSegment *line)
{
    lsBox box;
    ls_box_init(&box);
    
    if (NULL != line)
    {
        box.left = min(line->s.x, line->e.x);
        box.right = max(line->s.x, line->e.x);
        box.bottom = min(line->s.y, line->e.y);
        box.top = max(line->s.y, line->e.y);
    }
    
    return box;
}

void ls_matrix_transform_line(lsMatrix* matrix, lsLineSegment* line) {
    if (line == NULL) {
        return;
    }

    ls_matrix_transform_point(matrix, &(line->s));
    ls_matrix_transform_point(matrix, &(line->e));
}
