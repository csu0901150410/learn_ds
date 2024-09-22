#pragma once

#include "ls_defs.h"


typedef struct {
    lsReal m[3][3]; // 3x3矩阵，
} lsMatrix;

//初始话单位矩阵
void ls_matrix_identity(lsMatrix* matrix);

/**
 * @brief 矩阵乘法 result = a * b
 * 
 * @param result 
 * @param a 
 * @param b 
 */
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
 * @brief 创建单位矩阵
 * 
 * @return lsMatrix 
 */
lsMatrix ls_matrix_set_identity();

/**
 * @brief 创建平移矩阵
 * 
 * @param tx x轴平移量
 * @param ty y轴平移量
 * @return lsMatrix 
 */
lsMatrix ls_matrix_set_translate(lsReal tx, lsReal ty);

/**
 * @brief 创建缩放矩阵
 * 
 * @param sx x轴所放量
 * @param sy y轴所放量
 * @return lsMatrix 
 */
lsMatrix ls_matrix_set_scale(lsReal sx, lsReal sy);

/**
 * @brief 创建旋转矩阵
 * 
 * @param angle 旋转弧度
 * @return lsMatrix 
 */
lsMatrix ls_matrix_set_rotate(lsReal angle);

/**
 * @brief 创建旋转矩阵
 * 
 * @param angle 旋转角度
 * @return lsMatrix 
 */
lsMatrix ls_matrix_set_rotate360(lsReal angle);

/**
 * @brief n个矩阵依次左乘，传入lsMatrix指针
 * 
 * 比如 : ls_matrix_multiply_n(3, m1, m2, m3) 即 result = m1 * m2 * m3;
 * 
 * @param n 左乘矩阵的个数
 * @param ... 
 * @return lsMatrix 
 */
lsMatrix ls_matrix_multiply_n(int n, ...);
