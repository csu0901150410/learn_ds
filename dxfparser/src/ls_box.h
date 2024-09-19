#pragma once

#include "ls_vector.h"
#include "ls_entity.h"

typedef struct {
	//lsVector bottom_left;
	//lsVector top_right;
	lsReal minX;
	lsReal maxX;
	lsReal minY;
	lsReal maxY;
}lsBox;

void init_box(lsBox* box);

void update_box(lsBox* box, lsPoint point);