#include <string.h>// for memset
#include <stdlib.h>// for atoi

#include "ls_dxf.h"
#include "ls_log.h"

bool ls_dxf_init(lsDxf *dxf, const char *filename)
{
    // 打开文件，清零字串缓存
    dxf->fp = NULL;
    errno_t errorcode;
	errorcode = fopen_s(&dxf->fp ,filename, "r");
    if (0 != errorcode)
    {
        ls_log_info("Open dxf file %s failed, error code: %d\n", filename, errorcode);
        return false;
    }

    memset(dxf->str, 0, MAX_BUF_SIZE);
    return true;
}

void ls_dxf_deinit(lsDxf *dxf)
{
    // 文件打开之后一定要记得关闭
    if (NULL != dxf->fp)
        fclose(dxf->fp);
}

bool ls_dxf_read_record(lsDxf *dxf, int *code)
{
    int tmpcode;
    if (!ls_dxf_read_group_code(dxf, &tmpcode))
        return false;
    *code = tmpcode;// 用临时变量接受读取的组码，防止读取失败时修改了外部的组码

    ls_dxf_read_row_string(dxf);

    if (feof(dxf->fp))
        return false;// feof返回非0，表示文件读取结束

    // see [https://en.cppreference.com/w/c/io/ferror]
    return !ferror(dxf->fp);// ferror返回非0，表示出错
}

bool ls_dxf_read_group_code(lsDxf *dxf, int *code)
{
    char buf[5] = { 0 };// 貌似目前组码最大就四位数
    fread_s(buf, sizeof(buf), 1, 4, dxf->fp);
    *code = atoi(buf);
    
    return !ferror(dxf->fp);// ferror返回非0，表示出错
}

bool ls_dxf_read_row_string(lsDxf *dxf)
{
    // 注意这里始终保证缓存数组中最后一个是0，避免真的读到MAX_BUF_SIZE个字符并且最后一个不是0，后续字符串操作出错
    if (NULL == fgets(dxf->str, MAX_BUF_SIZE - 1, dxf->fp))
        return false;
    
    return !ferror(dxf->fp);// ferror返回非0，表示出错
}
