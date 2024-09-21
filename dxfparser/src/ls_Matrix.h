#pragma once

#include "ls_defs.h"
#include "ls_vector.h"
#include "ls_line_segment.h"
#include "ls_polygon.h"
#include "ls_arc.h"

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

/**
 * @brief ����Ӧ�ã���ı任.
 */
void ls_matrix_transform_point(lsMatrix* matrix, lsVector* point);

/**
 * @brief.����Ӧ�ã��ߵı任
 */
void ls_matrix_transform_line(lsMatrix* matrix, lsLineSegment* line_seg);


/**
 * @brief .����Ӧ�ã�����εı任
 */
void ls_matrix_transform_polygon(lsMatrix* matrix, lsPolygon* polygon);

/**
 * @brief .����Ӧ�ã�Բ���ı任
 */
void ls_matrix_transform_arc(lsMatrix* matrix, lsArc* arc_seg);






