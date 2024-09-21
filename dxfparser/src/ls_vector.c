#include <math.h>

#include "ls_vector.h"

lsPoint ls_point_create(lsReal x, lsReal y)
{
    lsPoint point;
    point.x = x;
    point.y = y;
    point.z = 0;
    return point;
}

lsReal ls_vector_get_length(const lsVector *v)
{
    return sqrt(v->x * v->x + v->y * v->y);
}

void ls_matrix_transform_point(lsMatrix* matrix, lsVector* point) {
    lsReal x = point->x;
    lsReal y = point->y;
    lsReal z = 1;

    point->x = x * matrix->m[0][0] + y * matrix->m[0][1] + z * matrix->m[0][2];
    point->y = x * matrix->m[1][0] + y * matrix->m[1][1] + z * matrix->m[1][2];
    point->z = 1.0f;
}