#pragma once

#include "ls_vector.h"

#include <stdbool.h>// for bool

typedef struct
{
    lsVector s;
    lsVector e;
    lsVector c;
    bool bccw;
} lsArc;

lsArc *ls_arc_create(lsVector start, lsVector end, lsVector center, bool bccw);

void ls_arc_destroy(lsArc **root);
