#pragma once
#include "ls_vector.h"

#include <stdbool.h> // for bool
typedef struct
{
    lsVector* points;  // ����ζ�������
    int pointCount;    // ��������
} lsPolygon;