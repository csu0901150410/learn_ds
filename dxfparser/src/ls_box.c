

#include "ls_box.h"
#include "ls_defs.h"

void ls_box_init(lsBox *box)
{
    box->left = LS_HUGE;
    box->right = -LS_HUGE;
    box->bottom = LS_HUGE;
    box->top = -LS_HUGE;
}

lsBox ls_box_from_point(lsPoint p1, lsPoint p2)
{
    lsBox box;
    box.left = MIN(p1.x, p2.x);
    box.right = MAX(p1.x, p2.x);
    box.bottom = MIN(p1.y, p2.y);
    box.top = MAX(p1.y, p2.y);
    return box;
}

bool ls_box_valid(const lsBox *box)
{
    if (NULL == box)
       return false; 

    if (box->left > box->right || box->bottom > box->top)
        return false;
    return true;
}

void ls_box_combine(lsBox *srcbox, const lsBox *dstbox)
{
    if (!ls_box_valid(srcbox))
    {
        *srcbox = *dstbox;
        return;
    }
    
    if (!ls_box_valid(dstbox))
        return;

    srcbox->left = MIN(srcbox->left, dstbox->left);
    srcbox->right = MAX(srcbox->right, dstbox->right);
    srcbox->bottom = MIN(srcbox->bottom, dstbox->bottom);
    srcbox->top = MAX(srcbox->top, dstbox->top);
}

lsReal ls_box_width(const lsBox *box)
{
    return box->right - box->left;
}

lsReal ls_box_height(const lsBox *box)
{
    return box->top - box->bottom;
}

lsPoint ls_box_center(const lsBox *box)
{
    lsPoint center;
    center.x = (box->left + box->right) / 2;
    center.y = (box->bottom + box->top) / 2;
    return center;
}

void ls_box_transform(lsBox *box, const lsMatrix *matrix)
{
    lsPoint lt = {box->left, box->top}, rb = {box->right, box->bottom};
    ls_point_transform(&lt, matrix);
    ls_point_transform(&rb, matrix);
    *box = ls_box_from_point(lt, rb);
}
