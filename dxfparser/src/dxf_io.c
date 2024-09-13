#include "dxf_io.h"
#include <windows.h>

int copy_dxf_file(const char* source_path, const char* target_path) {
	FILE* source_file = NULL;
	FILE* target_file = NULL;

	if (0 != fopen_s(&source_file, source_path, "r")) {
		//printf("无法打开源文件:%s\n", source_path);
		return 1;//执行失败
	}

	if (0 != fopen_s(&target_file, target_path, "w")) {
		fclose(source_file);
		//printf("无法创建目标文件:%s\n", target_path);
		return 1;
	}

	//逐行读取并写入
	char buffer[1024];//每行的大小
	// fgets和fputs第一个参数类型是char *的，而&buffer类型是char (*)[1024]，即指向数组的指针类型，二者类型不匹配，不能传入&buffer
	// 而数组名buffer在使用时会退化成指向第一个元素的指针，其类型是char *，可以类型匹配
	while (fgets(buffer, sizeof(buffer), source_file) != NULL) {
		fputs(buffer, target_file);
	}

	fclose(source_file);
	fclose(target_file);

	printf("文件复制成功");
	return 0;
}