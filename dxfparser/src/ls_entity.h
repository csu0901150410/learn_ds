#pragma once

#include "ls_vector.h"
#include "ls_line_segment.h"
#include "ls_arc.h"

typedef enum
{
    enum_geo_segment = 0,
    enum_geo_arc,
} enum_geo_type;

typedef struct
{
    enum_geo_type type;
    void *entity;
} lsEntity;

lsEntity *ls_entity_create_segment(lsVector start, lsVector end);

lsEntity *ls_entity_create_arc(lsVector start, lsVector end, lsVector center, bool bccw);

void ls_entity_destroy(lsEntity **root);
