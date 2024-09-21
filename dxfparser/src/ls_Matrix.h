#pragma once

#include "ls_defs.h"


typedef struct {
    lsReal m[3][3]; // 3x3����
} lsMatrix;

//��ʼ����λ����
void ls_matrix_identity(lsMatrix* matrix);

//����˷�
void ls_matrix_multiply(lsMatrix* result, const lsMatrix* a, const lsMatrix* b);

/**
 * @brief ƽ��.
 */
void ls_matrix_translate(lsMatrix* matrix, lsReal tx, lsReal ty);

/**
 * @brief ����.
 */
void ls_matrix_scale(lsMatrix* matrix, lsReal sx, lsReal sy);

/**
 * @brief ��ת.
 */
void ls_matrix_rotate(lsMatrix* matrix, lsReal angle);










