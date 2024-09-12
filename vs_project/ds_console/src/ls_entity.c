#include "ls_entity.h"

#include <stdlib.h>// for malloc
#include <assert.h>// for assert

lsEntity *ls_entity_create_segment(lsVector start, lsVector end)
{
    lsEntity *entity = (lsEntity*)malloc(sizeof(lsEntity));
    assert(entity);

    lsLineSegment *seg = ls_line_segment_create(start, end);

    entity->type = enum_geo_segment;
    entity->entity = seg;
    return entity;
}

lsEntity *ls_entity_create_arc(lsVector start, lsVector end, lsVector center, bool bccw)
{
    lsEntity *entity = (lsEntity*)malloc(sizeof(lsEntity));
    assert(entity);

    lsArc *arc = ls_arc_create(start, end, center, bccw);

    entity->type = enum_geo_arc;
    entity->entity = arc;
    return entity;
}

void ls_entity_destroy(lsEntity **root)
{
    if (NULL == root || NULL == *root)
        return;
    free(*root);
    *root = NULL;
}
