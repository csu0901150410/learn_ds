#ifndef LS_CONDITION_H
#define LS_CONDITION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//筛选：组码为 0 并且值为 "LINE" 时筛选出来
int line_entity_condition(int code, const char* value);
//筛选: 指定坐标的行
int coordinate_condition(int code, const char* value);
#endif 