#pragma once

#include "ls_defs.h"
#include "ls_vector.h"
#include "ls_line_segment.h"
#include "ls_polygon.h"
#include "ls_arc.h"

typedef struct {
    lsReal m[3][3]; // 3x3矩阵，
} lsMatrix;

//初始话单位矩阵
void ls_matrix_identity(lsMatrix* matrix);

//矩阵乘法
void ls_matrix_multiply(lsMatrix* result, const lsMatrix* a, const lsMatrix* b);

/**
 * @brief 平移.
 */
void ls_matrix_translate(lsMatrix* matrix, lsReal tx, lsReal ty);

/**
 * @brief 缩放.
 */
void ls_matrix_scale(lsMatrix* matrix, lsReal sx, lsReal sy);

/**
 * @brief 旋转.
 */
void ls_matrix_rotate(lsMatrix* matrix, lsReal angle);

/**
 * @brief 矩阵应用，点的变换.
 */
void ls_matrix_transform_point(lsMatrix* matrix, lsVector* point);

/**
 * @brief.矩阵应用，线的变换
 */
void ls_matrix_transform_line(lsMatrix* matrix, lsLineSegment* line_seg);


/**
 * @brief .矩阵应用，多边形的变换
 */
void ls_matrix_transform_polygon(lsMatrix* matrix, lsPolygon* polygon);

/**
 * @brief .矩阵应用，圆弧的变换
 */
void ls_matrix_transform_arc(lsMatrix* matrix, lsArc* arc_seg);






