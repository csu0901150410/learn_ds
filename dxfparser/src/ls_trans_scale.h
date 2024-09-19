#pragma once
#include"ls_box.h"

typedef struct {
    lsReal scaleX;
    lsReal scaleY;
    lsReal translateX;
    lsReal translateY;
} lsTransScale;

/**
 * @brief �������ű�����ת������.
 */
lsTransScale TransformParams(const lsBox* box, lsReal screenWidth, lsReal screenHeight);

/**
 * @brief  ����Ӧ�����ź�ת��.
 */
void apply_Trans_entity(lsVector* point, const lsTransScale* params);




