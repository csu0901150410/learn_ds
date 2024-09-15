#include <string.h>// for memset
#include <stdlib.h>// for atoi

#include "ls_dxf.h"
#include "ls_log.h"
#include "ls_utils.h"

#include "ls_line_segment.h"

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

const char *ls_dxf_get_row_string(lsDxf *dxf)
{
    return dxf->str;
}

bool ls_dxf_parse(lsDxf *dxf)
{
    int code;
    const char *rowString = NULL;
    while (1)
    {
        if (!ls_dxf_read_record(dxf, &code))
            break;// 读取结束或者读取完毕，退出循环

        if (0 == code)
        {
            rowString = ls_dxf_get_row_string(dxf);

            // dxf文件解析到最后一行EOF，结束解析
            if (ls_utils_is_string_equal(rowString, "EOF"))
                return true;

            // dxf数据从 [0 : SECTION] 开始
            // see [https://help.autodesk.com/view/ACD/2023/CHS/?guid=GUID-D939EA11-0CEC-4636-91A8-756640A031D3]
            if (ls_utils_is_string_equal(rowString, "SECTION"))
            {
                ls_dxf_read_record(dxf, &code);

                // 识别到 [2 : ENTITY] 则处理图元数据段
                if (2 == code)
                {
                    rowString = ls_dxf_get_row_string(dxf);
                    if (ls_utils_is_string_equal(rowString, "ENTITIES"))
                        ls_dxf_process_entity(dxf);
                }
            }
        }
    }

    return true;
}

bool ls_dxf_process_entity(lsDxf *dxf)
{
    int code;
    const char *rowString = NULL;
    while (1)
    {
        if (!ls_dxf_read_record(dxf, &code))
            return false;

        // 遇到 [0 : ENDSEC] 则当前段数据结束
        if (0 == code && ls_utils_is_string_equal(rowString, "ENDSEC"))
            return true;

        // 处理线段
        if (ls_utils_is_string_equal(rowString, "LINE"))
        {
            ls_dxf_process_line(dxf);
        }
    }

    return true;
}

bool ls_dxf_process_line(lsDxf *dxf)
{
    int code;
    const char *rowString = NULL;
    lsLineSegment segment = { {0, 0}, {1, 1} };
    while (1)
    {
        if (!ls_dxf_read_record(dxf, &code))
            return false;

        switch (code)
        {
        case 0:
        {
            // 遇到组码0，表示当前线段数据段结束，线段解析完毕
            ls_log_info("Segment coord info, x : %.3f, y : %.3f\n", segment.s.x, segment.s.y);
            return true;
        }
        break;

        case 10:
        {
            // 取x坐标数据
            //segment.x = xxx;
        }
        break;

        case 20:
        {
            // 取y坐标数据
            //segment.y = xxx;
        }
        break;
        }
    }

    return true;
}
