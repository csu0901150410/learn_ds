

#include "ls_Matrix.h"
#include "ls_defs.h"

void ls_matrix_identity(lsMatrix* matrix){
    memset(matrix->m, 0, sizeof(matrix->m));

    matrix->m[0][0] = 1.0;
    matrix->m[1][1] = 1.0;
    matrix->m[2][2] = 1.0;
}

void ls_matrix_multiply(lsMatrix* result, const lsMatrix* a, const lsMatrix* b) {
    lsMatrix temp;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp.m[i][j] = a->m[i][0] * b->m[0][j] +
                a->m[i][1] * b->m[1][j] +
                a->m[i][2] * b->m[2][j];
        }
    }
    memcpy(result->m, temp.m, sizeof(temp.m));
}

void ls_matrix_translate(lsMatrix* matrix, lsReal tx, lsReal ty) {
    lsMatrix translation;
    ls_matrix_identity(&translation);
    translation.m[2][0] = tx;
    translation.m[2][1] = ty;
    ls_matrix_multiply(matrix, matrix, &translation);
}

void ls_matrix_scale(lsMatrix* matrix, lsReal sx, lsReal sy) {
    lsMatrix scaling;
    ls_matrix_identity(&scaling);
    scaling.m[0][0] = sx;
    scaling.m[1][1] = sy;
    ls_matrix_multiply(matrix, matrix, &scaling);
}

void ls_matrix_rotate(lsMatrix* matrix, lsReal angle) {
    lsMatrix rotation;
    ls_matrix_identity(&rotation);

    lsReal radians = (lsReal)angle * LS_PI / 180.0;
    rotation.m[0][0] = cos(radians);
    rotation.m[0][1] = -sin(radians);
    rotation.m[1][0] = sin(radians);
    rotation.m[1][1] = cos(radians);

    ls_matrix_multiply(matrix, matrix, &rotation);
}

void ls_matrix_transform_point(lsMatrix* matrix, lsVector* point) {
    lsReal x = point->x;
    lsReal y = point->y;

    point->x = x * matrix->m[0][0] + y * matrix->m[1][0] + matrix->m[2][0];
    point->y = x * matrix->m[0][1] + y * matrix->m[1][1] + matrix->m[2][1];
}

void ls_matrix_transform_line(lsMatrix* matrix, lsLineSegment* line_seg) {
    if (line_seg == NULL) {
        return;
    }

    ls_matrix_transform_point(matrix, &(line_seg->s));
    ls_matrix_transform_point(matrix, &(line_seg->e));

}

void ls_matrix_transform_polygon(lsMatrix* matrix, lsPolygon* polygon) {
    if (polygon == NULL || polygon->edges == NULL) {
        return;
    }

    for (lsListIterator it = ls_list_iterator_start(polygon->edges);
        !ls_list_iterator_done(&it);
        ls_list_iterator_step(&it)) {

        lsLineSegment* edge = (lsLineSegment*)ls_list_iterator_get_data(&it);
        if (edge != NULL) {
            ls_matrix_transform_line(matrix, edge);
        }
    }
}

    void ls_matrix_transform_arc(lsMatrix * matrix, lsArc * arc_seg) {
        if (arc_seg == NULL) {
            return;
        }

        // 对圆弧的起点、终点和圆心应用矩阵变换
        ls_matrix_transform_point(matrix, &arc_seg->s);  // 变换起点
        ls_matrix_transform_point(matrix, &arc_seg->e);  // 变换终点
        ls_matrix_transform_point(matrix, &arc_seg->c);  // 变换圆心
}

