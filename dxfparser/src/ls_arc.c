#include "ls_arc.h"

#include <stdlib.h> // for malloc
#include <assert.h> // for assert
#include <math.h>


#include "ls_defs.h"

#define M_PI 3.14159265358979323846

lsArc *ls_arc_create(lsVector start, lsVector end, lsVector center, bool bccw)
{
    lsArc *arc = (lsArc *)malloc(sizeof(lsArc));
    assert(arc);

    arc->s = start;
    arc->e = end;
    arc->c = center;
    arc->bccw = bccw;
    return arc;
}

void ls_arc_destroy(lsArc **root)
{
    if (NULL == root || NULL == *root)
        return;
    free(*root);
    *root = NULL;
}

double radians_to_angle(double radians) {
    return radians * (180.0 / M_PI);
}

double angle_center_to_point(lsVector point, lsVector center) {
    double angle = radians_to_angle(atan2(point.y - center.y, point.x - center.x));
    if (angle < 0) {
        angle += 360.0;  // 保证角度在0到360度之间
    }
    return angle;
}
bool arc_intersects_angle(double start_angle, double end_angle, double target_angle, bool bccw) {
    if (bccw) {  // 逆时针
        if (start_angle <= end_angle) {
            return (target_angle >= start_angle && target_angle <= end_angle);
        }
        else {
            return (target_angle >= start_angle || target_angle <= end_angle);
        }
    }
    else {  // 顺时针
        if (start_angle >= end_angle) {
            return (target_angle <= start_angle && target_angle >= end_angle);
        }
        else {
            return (target_angle <= start_angle || target_angle >= end_angle);
        }
    }
}

lsBox ls_arc_geo_get_box(const lsArc* arc){
    lsBox box;
    ls_box_init(&box); // 初始化边界框

    if (NULL == arc)
        return box;
    
        // 计算起点和终点的角度
        lsReal start_angle = angle_center_to_point(arc->s, arc->c);
        lsReal end_angle = angle_center_to_point(arc->e, arc->c);
        // 半径
        lsReal radius = sqrt(pow(arc->s.x - arc->c.x, 2) + pow(arc->s.y - arc->c.y, 2));
        // box的初始边界
        box.left = min(arc->s.x, arc->e.x);
        box.right = max(arc->s.x, arc->e.x);
        box.bottom = min(arc->s.y, arc->e.y);
        box.top = max(arc->s.y, arc->e.y);

    //检查是否超过坐标轴
    // 检查与 x 轴相交（0°）
    if (arc_intersects_angle(start_angle, end_angle, 0, arc->bccw)) {
        box.right = max(box.right, arc->c.x + radius);
    }
    // 检查与 y 轴相交（90°）
    if (arc_intersects_angle(start_angle, end_angle, 90, arc->bccw)) {
        box.top = max(box.top, arc->c.y + radius);
    }
    // 检查与 x 轴相交（180°）
    if (arc_intersects_angle(start_angle, end_angle, 180, arc->bccw)) {
        box.top = max(box.top, arc->c.y + radius);
    }
    // 检查与 x 轴相交（270°）
    if (arc_intersects_angle(start_angle, end_angle, 270, arc->bccw)) {
        box.top = max(box.top, arc->c.y + radius);
    }
    return box;
}
