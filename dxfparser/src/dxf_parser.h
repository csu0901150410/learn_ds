#ifndef DXF_PARSER_H
#define DXF_PARSER_H

#include "ls_vector.h"
#include "ls_line_segment.h"
#include "ls_list.h"

//����dxf�ļ���
// ������file_Path��Ҫ�������ļ���·������lines:�ṹ��ָ��,�����߶εı���
int parser_dxf(const char* file_path, lsList **head);

#endif 