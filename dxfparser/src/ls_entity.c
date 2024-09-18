#include "ls_entity.h"

#include <stdlib.h> // for malloc
#include <assert.h> // for assert

lsEntity *ls_entity_create_segment(lsVector start, lsVector end)
{
    lsEntity *entity = (lsEntity *)malloc(sizeof(lsEntity));
    assert(entity);

    lsLineSegment *seg = ls_line_segment_create(start, end);

    entity->type = enum_geo_segment;
    entity->entity = seg;
    return entity;
}

lsEntity *ls_entity_create_arc(lsVector start, lsVector end, lsVector center, bool bccw)
{
    lsEntity *entity = (lsEntity *)malloc(sizeof(lsEntity));
    assert(entity);

    lsArc *arc = ls_arc_create(start, end, center, bccw);

    entity->type = enum_geo_arc;
    entity->entity = arc;
    return entity;
}

lsEntity* ls_entity_create_polygon(lsList* points)
{
    if (points == NULL || points->head == NULL)
        return NULL;

    lsEntity* entity = (lsEntity*)malloc(sizeof(lsEntity));
    assert(entity);

    entity->type = enum_geo_polygon;  // 定义的多边形类型
    entity->entity = points;  // 将顶点链表存入实体
    return entity;
}


void ls_entity_destroy(lsEntity **root)
{
    if (NULL == root || NULL == *root)
        return;
    if ((*root)->entity)
        free((*root)->entity);// 要考虑调用每个实体具体的释放函数
    free(*root);
    *root = NULL;
}
