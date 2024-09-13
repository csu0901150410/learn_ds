#include "dxf_io.h"

int copy_dxf_file(const char* source_path, const char* target_path) {
	FILE* source_file = NULL;
	FILE* target_file = NULL;
	if (NULL == fopen_s(&source_file,source_path, "r") ) {
		printf("无法打开源文件:%s\n", source_path);
		return 1;//执行失败
	}

	if (NULL == fopen_s(&target_file,target_path, "w")) {
		printf("无法创建目标文件:%s\n", target_path);
		fclose(source_file);
		return 1;
	}

	//逐行读取并写入
	char buffer[1024];//每行的大小
	while (fgets(buffer, sizeof(buffer), source_file) != NULL) {
		fputs(buffer, target_file);
	}

	fclose(source_file);
	fclose(target_file);

	printf("文件复制成功");
	return 0;
}