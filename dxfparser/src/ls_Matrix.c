#include <string.h>
#include <math.h>

#include "ls_matrix.h"
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
    ls_matrix_multiply(matrix, &scaling, matrix);
}

void ls_matrix_rotate(lsMatrix* matrix, lsReal angle) {
    lsMatrix rotation;
    ls_matrix_identity(&rotation);

    lsReal radians = (lsReal)angle * LS_PI / 180.0;
    rotation.m[0][0] = cos(radians);
    rotation.m[0][1] = -sin(radians);
    rotation.m[1][0] = sin(radians);
    rotation.m[1][1] = cos(radians);

    ls_matrix_multiply(matrix, &rotation, matrix);
}

lsMatrix ls_matrix_set_identity()
{
    lsMatrix matrix;
    memset(matrix.m, 0, sizeof(matrix.m));
    matrix.m[0][0] = 1.0;
    matrix.m[1][1] = 1.0;
    matrix.m[2][2] = 1.0;
    return matrix;
}

lsMatrix ls_matrix_set_translate(lsReal tx, lsReal ty)
{
    lsMatrix matrix = ls_matrix_set_identity();
    matrix.m[0][2] += tx;
    matrix.m[1][2] += ty;
    return matrix;
}

lsMatrix ls_matrix_set_scale(lsReal sx, lsReal sy)
{
    lsMatrix matrix = ls_matrix_set_identity();
    matrix.m[0][0] = sx;
    matrix.m[1][1] = sy;
    return matrix;
}

lsMatrix ls_matrix_set_rotate(lsReal angle)
{
    lsMatrix matrix = ls_matrix_set_identity();
    matrix.m[0][0] = cos(angle);
    matrix.m[0][1] = -sin(angle);
    matrix.m[1][0] = sin(angle);
    matrix.m[1][1] = cos(angle);
    return matrix;
}

lsMatrix ls_matrix_set_rotate360(lsReal angle)
{
    lsReal radian = DEGREE_2_RADIAN(angle);
    return ls_matrix_set_rotate(radian);
}

lsMatrix ls_matrix_multiply_n(int n, ...)
{
    lsMatrix result = ls_matrix_set_identity();
    
    va_list ap;
    va_start(ap, n);

    for (int i = 0; i < n; ++i)
    {
        const lsMatrix *m = va_arg(ap, lsMatrix*);
        ls_matrix_multiply(&result, m, &result);
    }

    va_end(ap);

    return result;
}
