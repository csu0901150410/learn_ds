#pragma once

#include <stdbool.h> // for bool

#include "ls_vector.h"
#include "ls_box.h"

typedef struct
{
    lsVector s;
    lsVector e;
    lsVector c;
    bool bccw;
} lsArc;

lsArc *ls_arc_create(lsVector start, lsVector end, lsVector center, bool bccw);

void ls_arc_destroy(lsArc **root);

/**
 * @brief将弧度转换为角度
 * 角度=弧度×( 180除以π）
 */
double radians_to_angle(double radians);

// 计算从圆心到某点的角度（返回0到360度范围内的角度）
double angle_center_to_point(lsVector point, lsVector center);

// 判断圆弧是否跨越某个角度
bool arc_intersects_angle(double start_angle, double end_angle, double target_angle, bool bccw);

// 获取圆弧的边界矩形
lsBox ls_arc_geo_get_box(const lsArc* arc);

lsReal ls_arc_get_start_angle(const lsArc *arc);

lsReal ls_arc_get_end_angle(const lsArc *arc);

lsReal ls_arc_get_radius(const lsArc *arc);
