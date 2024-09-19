
#include "ls_trans_scale.h"

lsTransScale TransformParams(const lsBox* box, lsReal screenWidth, lsReal screenHeight) {
    lsTransScale params;

    // 计算边界矩形的宽度和高度
    lsReal boxWidth = box->maxX - box->minX;
    lsReal boxHeight = box->maxY - box->minY;

    // 缩放比例
    params.scaleX = screenWidth / boxWidth;
    params.scaleY = screenHeight / boxHeight;

    // 缩放比例:最小值
    lsReal scale = fminf(params.scaleX, params.scaleY);

    // 计算平移参数
    params.translateX = -box->minX * scale;
    params.translateY = -box->minY * scale;

    return params;
}


void apply_Trans_entity(lsVector* point, const lsTransScale* params) {
    point->x = point->x * params->scaleX + params->translateX;
    point->y = point->y * params->scaleY + params->translateY;
}