#pragma once
#include "ls_vector.h"

#include <stdbool.h> // for bool
typedef struct
{
    lsVector* points;  // 多边形顶点数组
    int pointCount;    // 顶点数量
} lsPolygon;