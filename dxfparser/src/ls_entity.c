#include "ls_entity.h"

#include <stdlib.h> // for malloc
#include <assert.h> // for assert

lsEntity* ls_entity_create_segment(lsVector start, lsVector end)
{
	lsEntity* entity = (lsEntity*)malloc(sizeof(lsEntity));
	assert(entity);

	lsLineSegment* seg = ls_line_segment_create(start, end);

	entity->type = enum_geo_segment;
	entity->entity = seg;
	return entity;
}

lsEntity* ls_entity_create_arc(lsVector start, lsVector end, lsVector center, bool bccw)
{
	lsEntity* entity = (lsEntity*)malloc(sizeof(lsEntity));
	assert(entity);

	lsArc* arc = ls_arc_create(start, end, center, bccw);

	entity->type = enum_geo_arc;
	entity->entity = arc;
	return entity;
}

lsEntity* ls_entity_create_polygon(lsPolygon* polygon)
{
	lsEntity* entity = (lsEntity*)malloc(sizeof(lsEntity));
	assert(entity);

	entity->type = enum_geo_polygon;
	entity->entity = polygon;
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

lsBox ls_entity_get_box(const lsEntity *entity)
{
    lsBox box;
    ls_box_init(&box);

    if (NULL == entity || NULL == entity->entity)
        return box;
        
    switch (entity->type)
    {
    case enum_geo_segment:
    {
        lsBox subbox = ls_line_segment_get_box((lsLineSegment*)entity->entity);
        ls_box_combine(&box, &subbox);
    }
    break;
    
    case enum_geo_polygon:
    {
        lsBox subbox = ls_polygon_get_box((lsPolygon*)entity->entity);
        ls_box_combine(&box, &subbox);
    }
    break;
    
    case enum_geo_arc:
    {
        lsBox subbox = ls_arc_geo_get_box((lsArc *)entity->entity);
        ls_box_combine(&box, &subbox);
    }
    break;
    }

    return box;
}

void ls_entity_transform(lsEntity *entity, const lsMatrix *matrix)
{
    switch (entity->type)
    {
    case enum_geo_segment:
    {
        lsLineSegment* seg = (lsLineSegment*)entity->entity;
        if (NULL != seg)
            ls_line_segment_transform(seg, matrix);
    }
    break;

    case enum_geo_polygon:
    {
        lsPolygon* polygon = (lsPolygon*)entity->entity;
        if (NULL != polygon)
            ls_matrix_transform_polygon(matrix, polygon);
    }

    case enum_geo_arc:
    {
        lsArc* arc = (lsArc*)entity->entity;
        if (NULL != arc)
            ls_arc_transform(arc, matrix);
    }
    }
}
