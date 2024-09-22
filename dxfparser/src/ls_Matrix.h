#pragma once

#include "ls_defs.h"


typedef struct {
    lsReal m[3][3]; // 3x3����
} lsMatrix;

//��ʼ����λ����
void ls_matrix_identity(lsMatrix* matrix);

/**
 * @brief ����˷� result = a * b
 * 
 * @param result 
 * @param a 
 * @param b 
 */
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

/**
 * @brief ������λ����
 * 
 * @return lsMatrix 
 */
lsMatrix ls_matrix_set_identity();

/**
 * @brief ����ƽ�ƾ���
 * 
 * @param tx x��ƽ����
 * @param ty y��ƽ����
 * @return lsMatrix 
 */
lsMatrix ls_matrix_set_translate(lsReal tx, lsReal ty);

/**
 * @brief �������ž���
 * 
 * @param sx x��������
 * @param sy y��������
 * @return lsMatrix 
 */
lsMatrix ls_matrix_set_scale(lsReal sx, lsReal sy);

/**
 * @brief ������ת����
 * 
 * @param angle ��ת����
 * @return lsMatrix 
 */
lsMatrix ls_matrix_set_rotate(lsReal angle);

/**
 * @brief ������ת����
 * 
 * @param angle ��ת�Ƕ�
 * @return lsMatrix 
 */
lsMatrix ls_matrix_set_rotate360(lsReal angle);

/**
 * @brief n������������ˣ�����lsMatrixָ��
 * 
 * ���� : ls_matrix_multiply_n(3, m1, m2, m3) �� result = m1 * m2 * m3;
 * 
 * @param n ��˾���ĸ���
 * @param ... 
 * @return lsMatrix 
 */
lsMatrix ls_matrix_multiply_n(int n, ...);
