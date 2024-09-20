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
