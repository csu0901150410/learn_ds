#ifndef LS_CONDITION_H
#define LS_CONDITION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//ɸѡ������Ϊ 0 ����ֵΪ "LINE" ʱɸѡ����
int line_entity_condition(int code, const char* value);
//ɸѡ: ָ���������
int coordinate_condition(int code, const char* value);
#endif 