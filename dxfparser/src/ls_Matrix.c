

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
    matrix->m[0][2] += tx;
    matrix->m[1][2] += ty;
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





