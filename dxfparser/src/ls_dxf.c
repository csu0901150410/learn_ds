#include <string.h> // for memset
#include <stdlib.h> // for atoi
#include <assert.h> // for assert

#include "ls_dxf.h"
#include "ls_log.h"
#include "ls_utils.h"

#include "ls_line_segment.h"
#include "ls_entity.h"

lsDxf *ls_dxf_create()
{
    lsDxf *dxf = (lsDxf*)malloc(sizeof(lsDxf));
    assert(dxf);
    return dxf;
}

void ls_dxf_destroy(lsDxf **root)
{
    if (NULL == root || NULL == *root)
        return;

    ls_dxf_deinit(*root);
    free(*root);
    *root = NULL;
}

bool ls_dxf_init(lsDxf *dxf, const char *filename)
{
    // 打开文件，清零字串缓存
    dxf->fp = NULL;
    errno_t errorcode;
    errorcode = fopen_s(&dxf->fp, filename, "r");
    if (0 != errorcode)
    {
        ls_log_info("Open dxf file %s failed, error code: %d\n", filename, errorcode);
        return false;
    }

    memset(dxf->str, 0, MAX_BUF_SIZE);

    dxf->list = ls_list_create();

    return true;
}

void ls_dxf_deinit(lsDxf *dxf)
{
    if (NULL == dxf)
        return;

    // 文件打开之后一定要记得关闭
    if (NULL != dxf->fp)
        fclose(dxf->fp);

    // 释放图元
    for (lsListIterator it = ls_list_iterator_start(dxf->list); !ls_list_iterator_done(&it); ls_list_iterator_step(&it))
    {
        lsEntity *ent = (lsEntity*)ls_list_iterator_get_data(&it);
        if (NULL == ent)
            continue;
        ls_entity_destroy(&ent);
    }

    // 释放链表
    ls_list_destroy(&dxf->list);
}

bool ls_dxf_read_record(lsDxf *dxf, int *code)
{
    int tmpcode;
    if (!ls_dxf_read_group_code(dxf, &tmpcode))
        return false;
    *code = tmpcode; // 用临时变量接受读取的组码，防止读取失败时修改了外部的组码

    ls_dxf_read_row_string(dxf);

    if (feof(dxf->fp))
        return false; // feof返回非0，表示文件读取结束

    // see [https://en.cppreference.com/w/c/io/ferror]
    return !ferror(dxf->fp); // ferror返回非0，表示出错
}

bool ls_dxf_read_group_code(lsDxf *dxf, int *code)
{
    char buf[5] = {0}; // 貌似目前组码最大就四位数
    fread_s(buf, sizeof(buf), 1, 4, dxf->fp);
    *code = atoi(buf);

    return !ferror(dxf->fp); // ferror返回非0，表示出错
}

bool ls_dxf_read_row_string(lsDxf *dxf)
{
    // 注意这里始终保证缓存数组中最后一个是0，避免真的读到MAX_BUF_SIZE个字符并且最后一个不是0，后续字符串操作出错
    if (NULL == fgets(dxf->str, MAX_BUF_SIZE - 1, dxf->fp))
        return false;

    return !ferror(dxf->fp); // ferror返回非0，表示出错
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
            break; // 读取结束或者读取完毕，退出循环

        if (0 == code)
        {
            rowString = ls_dxf_get_row_string(dxf);

            // dxf文件解析到最后一行EOF，结束解析
            if (ls_utils_is_string_equal_n(rowString, "EOF", 3))
                return true;

            // dxf数据从 [0 : SECTION] 开始
            // see [https://help.autodesk.com/view/ACD/2023/CHS/?guid=GUID-D939EA11-0CEC-4636-91A8-756640A031D3]
            if (ls_utils_is_string_equal_n(rowString, "SECTION", 7))
            {
                ls_dxf_read_record(dxf, &code);

                // 识别到 [2 : ENTITY] 则处理图元数据段
                if (2 == code)
                {
                    rowString = ls_dxf_get_row_string(dxf);
                    if (ls_utils_is_string_equal_n(rowString, "ENTITIES", 8))
                        ls_dxf_process_entity(dxf);
                }
            }
        }
    }

    return true;
}

bool ls_dxf_process_entity(lsDxf *dxf)
{
    // 改造思路：实际上线段处理函数中已经读到下一个图元了，所以entity解析这里无需再在循环中读取一个Record，
    // 而是等待线段处理函数返回的时候，开启下一轮循环，直到读取到0组码

    int code;
    const char *rowString = NULL;

    // 进入本函数的时候，ENTITY字段已被读取，先往下读一行，再进入循环
    if (!ls_dxf_read_record(dxf, &code))
        return false;
    // 读取到0组码，才会读取相应的字符串，其他的组码，不作为开启解析图元的条件
    if (0 == code)
        rowString = ls_dxf_get_row_string(dxf);

    while (1)
    {
        // 遇到 [0 : ENDSEC] 则当前段数据结束
        if (ls_utils_is_string_equal_n(rowString, "ENDSEC", 6))
            return true;

        // 处理线段
        else if (ls_utils_is_string_equal_n(rowString, "LINE", 4))
            ls_dxf_process_line(dxf);

        // 其他的数据，不关心，再读一行
        else
        {
            if (!ls_dxf_read_record(dxf, &code))
                return false;
            if (0 == code)
                rowString = ls_dxf_get_row_string(dxf);
        }
    }

    return true;
}

// bool ls_dxf_process_line(lsDxf* dxf, lsShapes* shapes)
bool ls_dxf_process_line(lsDxf *dxf)
{
    int code;
    lsPoint start = {0, 0}, end = {0, 0};

    while (1)
    {
        if (!ls_dxf_read_record(dxf, &code))
            return false;

        switch (code)
        {
        case 0:
        {
            lsEntity *pEnt = ls_entity_create_segment(start, end);
            assert(pEnt);
            ls_list_append(dxf->list, pEnt);

            // 遇到组码0，表示当前线段数据段结束，线段解析完毕
			// ls_log_info("Segment start, x : %f, y : %f\n", start.x, start.y);
			// ls_log_info("Segment end, x : %f, y : %f\n", end.x, end.y);
            return true; // 返回，其实这时候下一个LINE的Record已经读取到了，让外部循环重新进入调用当前函数的分支即可
        }
        break;

        // 只读取二维坐标，不处理z
        case 10:
        {
            start.x = (lsReal)atof(ls_dxf_get_row_string(dxf));
        }
        break;

        case 20:
        {
            start.y = (lsReal)atof(ls_dxf_get_row_string(dxf));
        }
        break;

        case 11:
        {
            end.x = (lsReal)atof(ls_dxf_get_row_string(dxf));
        }
        break;

        case 21:
        {
            end.y = (lsReal)atof(ls_dxf_get_row_string(dxf));
        }
        break;
        }
    }

    return true;
}
