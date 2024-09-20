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

//弧度
double angle_center_to_point(lsVector point, lsVector center) {
    double angle = radians_to_angle(atan2(point.y - center.y, point.x - center.x));
    return angle < 0 ? angle + 360.0 : angle;  // 保证角度在0到360度之间
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
    // lsBox box;
    // ls_box_init(&box); // 初始化边界框

    // if (NULL == arc)
    //     return box;
    
    //     // 计算起点和终点的角度
    //     lsReal start_angle = angle_center_to_point(arc->s, arc->c);
    //     lsReal end_angle = angle_center_to_point(arc->e, arc->c);
    //     // 半径
    //     lsReal radius = sqrt(pow(arc->s.x - arc->c.x, 2) + pow(arc->s.y - arc->c.y, 2));
    //     // box的初始边界
    //     box.left = min(arc->s.x, arc->e.x);
    //     box.right = max(arc->s.x, arc->e.x);
    //     box.bottom = min(arc->s.y, arc->e.y);
    //     box.top = max(arc->s.y, arc->e.y);

    // //检查是否超过坐标轴
    // // 检查与 x 轴相交（0°）
    // if (arc_intersects_angle(start_angle, end_angle, 0, arc->bccw)) {
    //     box.right = max(box.right, arc->c.x + radius);
    // }
    // // 检查与 y 轴相交（90°）
    // if (arc_intersects_angle(start_angle, end_angle, 90, arc->bccw)) {
    //     box.top = max(box.top, arc->c.y + radius);
    // }
    // // 检查与 x 轴相交（180°）
    // if (arc_intersects_angle(start_angle, end_angle, 180, arc->bccw)) {
    //     box.top = max(box.top, arc->c.y + radius);
    // }
    // // 检查与 x 轴相交（270°）
    // if (arc_intersects_angle(start_angle, end_angle, 270, arc->bccw)) {
    //     box.top = max(box.top, arc->c.y + radius);
    // }
    // return box;

    // 基本思路
    // 1.计算起点终点的角度
    // 2.根据起点终点角度，判断圆弧跨过哪几条坐标轴

    lsReal sangle = ls_arc_get_start_angle(arc);
    lsReal eangle = ls_arc_get_end_angle(arc);
    lsReal radius = ls_arc_get_radius(arc);
    lsBox box;
    ls_box_init(&box);

    // 调整成，逆时针
    if (!arc->bccw)
    {
        // 顺时针的，交换起点终点
        lsReal tmp = sangle;
        sangle = eangle;
        eangle = tmp;
    }

    // 看看四个方向的坐标轴要怎么样才能被跨过，注意此时必定是从start逆时针转到angle的弧
    
    // x轴正向，只要start > end，那么就会穿过+x
    if (sangle > eangle)
    {
        box.right = MAX(box.right, arc->c.x + radius);
    }
    
    // y轴正向
    if ((sangle < LS_PI * 2 && eangle > LS_PI * 2)
        || (sangle < LS_PI * 2 && sangle > eangle)
        || (eangle > LS_PI * 2 && sangle > eangle))
    {
        box.top = MAX(box.top, arc->c.y + radius);
    }

    // x轴负向
    if ((sangle < LS_PI && eangle > LS_PI)
        || (sangle < LS_PI && sangle > eangle)
        || (eangle > LS_PI && sangle > eangle))
    {
        box.left = MIN(box.left, arc->c.x - radius);
    }
    
    // y轴负向
    if ((sangle < LS_PI * 1.5 && eangle > LS_PI * 1.5)
        || (sangle < LS_PI * 1.5 && sangle > eangle)
        || (eangle > LS_PI * 1.5 && sangle > eangle))
    {
        box.bottom = MIN(box.bottom, arc->c.y - radius);
    }

    // 考虑端点
    box.left = MIN(box.left, MIN(arc->s.x, arc->e.x));
    box.right = MAX(box.right, MAX(arc->s.x, arc->e.x));
    box.bottom = MIN(box.bottom, MIN(arc->s.y, arc->e.y));
    box.top = MAX(box.top, MAX(arc->s.y, arc->e.y));
    return box;
}

lsReal ls_arc_get_start_angle(const lsArc *arc)
{
    // 圆心到起点的向量
    lsVector os;
    os.x = arc->s.x - arc->c.x;
    os.y = arc->s.y - arc->c.y;

    // 半径向量和(1, 0)的夹角
    lsReal angle = atan2(os.y, os.x);
    if (angle < 0)
        angle += 2 * LS_PI;// 调整到[0, 2PI)
    return angle;// 返回弧度
}

lsReal ls_arc_get_end_angle(const lsArc *arc)
{
    // 圆心到起点的向量
    lsVector oe;
    oe.x = arc->e.x - arc->c.x;
    oe.y = arc->e.y - arc->c.y;

    // 半径向量和(1, 0)的夹角
    lsReal angle = atan2(oe.y, oe.x);
    if (angle < 0)
        angle += 2 * LS_PI;// 调整到[0, 2PI)
    return angle;// 返回弧度
}

lsReal ls_arc_get_radius(const lsArc *arc)
{
    // 圆心到起点的向量
    lsVector os;
    os.x = arc->s.x - arc->c.x;
    os.y = arc->s.y - arc->c.y;
    return ls_vector_get_length(&os);
}
