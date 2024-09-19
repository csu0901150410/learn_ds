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

void ls_entity_get_Points(const lsEntity* entity, lsVector** points, size_t* pointCount) {
	if (entity == NULL || points == NULL || pointCount == NULL) {
		return;
	}

	switch (entity->type) {
	case enum_geo_segment: {
		lsLineSegment* seg = (lsLineSegment*)entity->entity;
		*pointCount = 2; 
		*points = (lsVector*)malloc(2 * sizeof(lsVector));
		(*points)[0] = seg->s;
		(*points)[1] = seg->e;
		break;
	}
	case enum_geo_arc: {
		lsArc* arc = (lsArc*)entity->entity;
		*pointCount = 3;
		*points = (lsVector*)malloc(3 * sizeof(lsVector));
		(*points)[0] = arc->s;
		(*points)[1] = arc->e;
		(*points)[2] = arc->c;
		break;
	}
	case enum_geo_polygon: {
		
		break;
	}
	default: {
		// 未知的类型，点数量为 0
		*pointCount = 0;
		*points = NULL;
		break;
	}
	}
}

	void ls_entity_destroy(lsEntity * *root)
	{
		if (NULL == root || NULL == *root)
			return;
		if ((*root)->entity)
			free((*root)->entity);// 要考虑调用每个实体具体的释放函数
		free(*root);
		*root = NULL;
	}