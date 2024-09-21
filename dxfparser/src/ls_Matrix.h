#pragma once

#include "ls_defs.h"


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










