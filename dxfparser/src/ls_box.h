#pragma once

#include <stdbool.h> // for bool

#include "ls_vector.h"

typedef struct {
	lsReal left;
	lsReal right;
	lsReal bottom;
	lsReal top;
} lsBox;

void ls_box_init(lsBox *box);

bool ls_box_valid(const lsBox *box);

/**
 * @brief lsBox合并
 * 
 * @param srcbox \p srcbox 和 \p dstbox 合并后，更新 \p srcbox
 * @param dstbox 被合并的box
 */
void ls_box_combine(lsBox *srcbox, const lsBox *dstbox);

lsReal ls_box_width(const lsBox *box);

lsReal ls_box_height(const lsBox *box);

lsPoint ls_box_center(const lsBox *box);
