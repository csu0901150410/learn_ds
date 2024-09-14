#ifndef DXF_PARSER_H
#define DXF_PARSER_H

#include "ls_vector.h"
#include "ls_line_segment.h"
#include "ls_list.h"

//解析dxf文件。
// 参数：file_Path：要解析的文件（路径），lines:结构体指针,解析线段的保存
int parser_dxf(const char* file_path, lsList **head);

#endif 