#include "dxf_parser.h"
#include <stdio.h>
#include <stdlib.h>//malloc
#include < math.h >

int parser_dxf(const char* target_file_path, lsList* list) {
	//取文件
	FILE* fileName = NULL;
	if (0 != fopen_s( &fileName ,target_file_path, "r"))
	{
		perror("文件读入失败");
		return 1;
	}

	int code;//组码
	char rowBuffer[1024];//每行的大小,存储
	lsLineSegment curr_line;//线段结构体变量
	int isParserLine = 0;//标识是否正在解析LNIE实体

	//逐行解析,将解析数据存入rowBuffer
	while (fgets(rowBuffer, sizeof(rowBuffer), fileName)) {
		code = atoi(rowBuffer);
		if (!fgets(rowBuffer, sizeof(rowBuffer), fileName))//解析到行末，跳出本次循环
			break;

		//处理祖码读取后的值
		if (code == 0 && strcmp(code, "LINE\n") == 0) {
			isParserLine = 1;//正在识别实体——开始
			continue;
		}

		if (isParserLine)//识别实体后的操作
		{
			switch (code)
			{
				//	起点与终点的坐标解析到线段结构体变量
			case 10:
				curr_line.s.x = atof(rowBuffer);
				break;
			case 20:
				curr_line.s.y = atof(rowBuffer);
				break;
			case 30:
				curr_line.s.z = atof(rowBuffer);
				break;
			case 11:
				curr_line.e.x = atof(rowBuffer);
				break;
			case 21:
				curr_line.e.y = atof(rowBuffer);
				break;
			case 31:
				curr_line.e.z = atof(rowBuffer);
				break;

				//	组码为0：说明是一个新的实体的开始
			case 0:
				if (strcmp(rowBuffer, "LINE\n") != 0) {//线段解析结束
					ls_list_append(list, &curr_line);
					isParserLine = 0;//实体解析标识符置0；
				}
				break;
			default:
				break;
			}
		}
	}
}