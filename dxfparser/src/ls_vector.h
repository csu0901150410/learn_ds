#pragma once

#include "ls_defs.h"
#include "ls_Matrix.h"

typedef struct
{
    lsReal x;
    lsReal y;
    lsReal z;
} lsVector;

typedef lsVector lsPoint;

lsPoint ls_point_create(lsReal x, lsReal y);

lsReal ls_vector_get_length(const lsVector *v);

/**
 * @brief 矩阵应用，点的变换.
 */
void ls_matrix_transform_point(lsMatrix* matrix, lsVector* point);
