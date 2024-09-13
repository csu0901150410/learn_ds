#include "dxf_io.h"

int copy_dxf_file(const char* source_path, const char* target_path) {
	FILE* source_file = NULL;
	FILE* target_file = NULL;
	if (NULL == fopen_s(&source_file,source_path, "r") ) {
		printf("�޷���Դ�ļ�:%s\n", source_path);
		return 1;//ִ��ʧ��
	}

	if (NULL == fopen_s(&target_file,target_path, "w")) {
		printf("�޷�����Ŀ���ļ�:%s\n", target_path);
		fclose(source_file);
		return 1;
	}

	//���ж�ȡ��д��
	char buffer[1024];//ÿ�еĴ�С
	while (fgets(buffer, sizeof(buffer), source_file) != NULL) {
		fputs(buffer, target_file);
	}

	fclose(source_file);
	fclose(target_file);

	printf("�ļ����Ƴɹ�");
	return 0;
}