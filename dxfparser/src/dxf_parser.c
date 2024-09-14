#include "dxf_parser.h"
#include <stdio.h>
#include <stdlib.h>//malloc
#include < math.h >

int parser_dxf(const char* target_file_path, lsList* list) {
	//ȡ�ļ�
	FILE* fileName = NULL;
	if (0 != fopen_s( &fileName ,target_file_path, "r"))
	{
		perror("�ļ�����ʧ��");
		return 1;
	}

	int code;//����
	char rowBuffer[1024];//ÿ�еĴ�С,�洢
	lsLineSegment curr_line;//�߶νṹ�����
	int isParserLine = 0;//��ʶ�Ƿ����ڽ���LNIEʵ��

	//���н���,���������ݴ���rowBuffer
	while (fgets(rowBuffer, sizeof(rowBuffer), fileName)) {
		code = atoi(rowBuffer);
		if (!fgets(rowBuffer, sizeof(rowBuffer), fileName))//��������ĩ����������ѭ��
			break;

		//���������ȡ���ֵ
		if (code == 0 && strcmp(code, "LINE\n") == 0) {
			isParserLine = 1;//����ʶ��ʵ�塪����ʼ
			continue;
		}

		if (isParserLine)//ʶ��ʵ���Ĳ���
		{
			switch (code)
			{
				//	������յ������������߶νṹ�����
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

				//	����Ϊ0��˵����һ���µ�ʵ��Ŀ�ʼ
			case 0:
				if (strcmp(rowBuffer, "LINE\n") != 0) {//�߶ν�������
					ls_list_append(list, &curr_line);
					isParserLine = 0;//ʵ�������ʶ����0��
				}
				break;
			default:
				break;
			}
		}
	}
}