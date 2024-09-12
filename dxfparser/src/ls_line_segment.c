#include "ls_line_segment.h"

#include <stdlib.h>// for malloc
#include <assert.h>// for assert

lsLineSegment *ls_line_segment_create(lsVector start, lsVector end)
{
    lsLineSegment *seg = (lsLineSegment*)malloc(sizeof(lsLineSegment));
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
