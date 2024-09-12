#pragma once

#include "ls_defs.h"

typedef struct
{
    lsReal x;
    lsReal y;
    lsReal z;
} lsVector;

typedef lsVector lsPoint;

lsPoint ls_point_create(lsReal x, lsReal y);
