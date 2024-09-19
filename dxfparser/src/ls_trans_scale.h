#pragma once
#include"ls_box.h"

typedef struct {
    lsReal scaleX;
    lsReal scaleY;
    lsReal translateX;
    lsReal translateY;
} lsTransScale;

/**
 * @brief 计算缩放比例和转换参数.
 */
lsTransScale TransformParams(const lsBox* box, lsReal screenWidth, lsReal screenHeight);

/**
 * @brief  顶点应用缩放和转换.
 */
void apply_Trans_entity(lsVector* point, const lsTransScale* params);




