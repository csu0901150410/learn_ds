#pragma once

#include "ls_defs.h"
#include "ls_matrix.h"

typedef struct
{
    lsReal x;
    lsReal y;
    lsReal z;
} lsVector;

typedef lsVector lsPoint;

lsPoint ls_point_create(lsReal x, lsReal y);

lsReal ls_vector_get_length(const lsVector *v);

void ls_vector_transform(lsVector *v, const lsMatrix *matrix);

void ls_point_transform(lsPoint *p, const lsMatrix *matrix);

/**
 * @brief 矩阵应用，点的变换.
 */
void ls_matrix_transform_point(lsMatrix* matrix, lsVector* point);
