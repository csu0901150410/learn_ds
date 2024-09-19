#pragma once

#include "ls_vector.h"
#include "ls_line_segment.h"
#include "ls_arc.h"
#include "ls_list.h"
#include "ls_polygon.h"

typedef enum
{
    enum_geo_segment = 0,
    enum_geo_arc,
    enum_geo_polygon,
} enum_geo_type;

typedef struct
{
    enum_geo_type type;
    void *entity;
} lsEntity;

lsEntity *ls_entity_create_segment(lsVector start, lsVector end);

lsEntity *ls_entity_create_arc(lsVector start, lsVector end, lsVector center, bool bccw);

// 这些create函数，遵循一个规则，比方说这里是 polygon to entity，那么形参一定是polygon
lsEntity *ls_entity_create_polygon(lsPolygon *polygon);

void ls_entity_get_Points(const lsEntity* entity, lsVector** points, size_t* pointCount);

void ls_entity_destroy(lsEntity **root);
