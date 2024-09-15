#ifndef DXF_PARSER_H
#define DXF_PARSER_H

#include "ls_vector.h"
#include "ls_line_segment.h"
#include "ls_list.h"
#include "ls_condition.h"

// 组码和值的结构
typedef struct {
    int code;          // 组码
    char value[1024];  // 组码对应的值
} DxfCodePair;


// 筛选条件，函数指针类型。
// EntityProcessor 是一个处理 DxfCodePair（组码-值对）的函数。
//DxfCodePair* codePair 作为参数，指向一个 DxfCodePair 的指针，即每个组码 - 值对的数据。
typedef int (*EntityProcessor)(DxfCodePair* codePair);

//解析dxf文件。
// 参数：file_Path：要解析的文件（路径），lines:结构体指针,解析线段的保存
int parser_dxf(const char* file_path, EntityProcessor processor);

void filter_dxf_with_condion(FILE* fp, EntityProcessor processor);
void test_read_dxf(const char *filename);

#endif 