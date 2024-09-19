
#include "ls_trans_scale.h"

lsTransScale TransformParams(const lsBox* box, lsReal screenWidth, lsReal screenHeight) {
    lsTransScale params;

    // ����߽���εĿ�Ⱥ͸߶�
    lsReal boxWidth = box->maxX - box->minX;
    lsReal boxHeight = box->maxY - box->minY;

    // ���ű���
    params.scaleX = screenWidth / boxWidth;
    params.scaleY = screenHeight / boxHeight;

    // ���ű���:��Сֵ
    lsReal scale = fminf(params.scaleX, params.scaleY);

    // ����ƽ�Ʋ���
    params.translateX = -box->minX * scale;
    params.translateY = -box->minY * scale;

    return params;
}


void apply_Trans_entity(lsVector* point, const lsTransScale* params) {
    point->x = point->x * params->scaleX + params->translateX;
    point->y = point->y * params->scaleY + params->translateY;
}