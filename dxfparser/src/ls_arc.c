#include "ls_arc.h"

#include <stdlib.h> // for malloc
#include <assert.h> // for assert

lsArc *ls_arc_create(lsVector start, lsVector end, lsVector center, bool bccw)
{
    lsArc *arc = (lsArc *)malloc(sizeof(lsArc));
    assert(arc);

    arc->s = start;
    arc->e = end;
    arc->c = center;
    arc->bccw = bccw;
    return arc;
}

void ls_arc_destroy(lsArc **root)
{
    if (NULL == root || NULL == *root)
        return;
    free(*root);
    *root = NULL;
}
