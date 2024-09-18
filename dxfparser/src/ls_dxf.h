#ifndef __LS_DXF_H__
#define __LS_DXF_H__

#include <stdio.h>
#include <stdbool.h>

#include "ls_common.h"
#include "ls_list.h"

/**
 * @brief dxf文件读取结构体及相关函数设计
 * 
 * 目标是按两行一组的规则读取dxf文件，第一行是组码，第二行是字符串，由此设计结构体，包含文件指针和字符数组，
 * 这样可以避免每次都直接传递文件指针并且将fgets等操作暴露出来，而字符数组可以避免每次都创建一个新的数组。
 * 
 * 结构体设计好之后，着手设计函数接口，注意不要先想着具体的函数实现细节，而是仅仅从读取逻辑上先将用到的函数
 * 原型（即函数声明）写好，然后再实现每一个函数，写得过程中如果需要新的函数再添加。
 * 
 * 因为结构体内部包含一个函数指针，所以需要两个函数去分别完成文件的打开和关闭，由此设计init和deinit函数。
 * 文件读取的规则是两行一起读，所以把两行记为一个Record去读，其中需要读取组码和RowString。
 * 
 */

typedef struct
{
    FILE *fp;// dxf文件指针
    char str[MAX_BUF_SIZE];// 缓存从dxf读到的行数据，组码之后的数据
    lsList *list;
} lsDxf;

/**
 * @brief 在堆中申请lsDxf结构体
 * 
 * @return lsDxf* 返回lsDxf结构体指针
 */
lsDxf *ls_dxf_create();

/**
 * @brief 销毁申请的lsDxf结构体堆内存
 * 
 * @param root 指向lsDxf结构体指针的指针变量
 */
void ls_dxf_destroy(lsDxf **root);

/**
 * @brief 初始化lsDxf结构体
 * 
 * @param dxf lsDxf结构体指针
 * @param filename dxf文件名
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool ls_dxf_init(lsDxf *dxf, const char *filename);

/**
 * @brief 反初始化lsDxf结构体
 * 
 * @param dxf lsDxf结构体指针
 */
void ls_dxf_deinit(lsDxf *dxf);

/**
 * @brief 从dxf文件中读取一个记录
 * 
 * @param dxf lsDxf结构体指针
 * @param code 输出参数，读到的组码指针
 * @return true 读取成功
 * @return false 读取失败
 */
bool ls_dxf_read_record(lsDxf *dxf, int *code);

/**
 * @brief 从dxf文件中读取一个组码
 * 
 * @param dxf lsDxf结构体指针
 * @param code 输出参数，读到的组码指针
 * @return true 读取成功
 * @return false 读取失败
 */
bool ls_dxf_read_group_code(lsDxf *dxf, int *code);

/**
 * @brief 从dxf文件中读取一行字串
 * 
 * @param dxf lsDxf结构体指针
 * @return true 读取成功
 * @return false 读取失败
 */
bool ls_dxf_read_row_string(lsDxf *dxf);

/**
 * @brief 获取当前组码对应的字符串
 * 
 * @param dxf lsDxf结构体指针
 * @return const char* 返回内部字符数组的首元素地址，避免直接访问内部字符数组
 */
const char *ls_dxf_get_row_string(lsDxf *dxf);

/**
 * @brief dxf文件解析
 * 
 * @param dxf lsDxf结构体指针
 * @return true 解析成功
 * @return false 解析失败
 */
bool ls_dxf_parse(lsDxf *dxf);

/**
 * @brief dxf解析处理ENTITY
 * 
 * @param dxf lsDxf结构体指针
 * @return true 处理成功
 * @return false 处理失败
 */
bool ls_dxf_process_entity(lsDxf *dxf);

/**
 * @brief dxf解析处理LINE
 * 
 * @param dxf lsDxf结构体指针
 * @return true 处理成功
 * @return false处理失败
 */
bool ls_dxf_process_line(lsDxf *dxf);

/**
 * @brief dxf解析处理arc
 *
 * @param dxf lsDxf结构体指针
 * @return true 处理成功
 * @return false处理失败
 */
bool ls_dxf_process_arc(lsDxf* dxf);

/**
 * @brief dxf解析处理polygon
 *
 * @param dxf lsDxf结构体指针
 * @return true 处理成功
 * @return false处理失败
 */
bool ls_dxf_process_polygon(lsDxf* dxf);
#endif// __LS_DXF_H__

