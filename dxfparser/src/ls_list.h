#pragma once

#include <stdbool.h>

// 函数指针，节点数据打印回调函数
typedef void (*callback_node_print)(const void *);

// 节点结构体定义
typedef struct _tag_lsNode
{
    struct _tag_lsNode *next;// 节点指针域  note : 这里涉及到不完全类型指针的使用
    void *data;// 节点数据域
} lsNode;

/**
 * @brief 节点创建
 * 
 * @param data 数据指针
 * @return lsNode* 返回节点指针
 */
lsNode *ls_node_create(void *data);

// 链表结构体定义
typedef struct
{
    lsNode *head;// 链表内部维护的头节点
} lsList;

/**
 * @brief 链表创建
 * 
 * @return lsList* 返回链表指针
 */
lsList *ls_list_create();

/**
 * @brief 链表销毁
 * 
 * @param root 链表的地址
 */
void ls_list_destroy(lsList **root);

/**
 * @brief 链表尾部追加节点
 * 
 * @param list 链表指针
 * @param data 数据指针
 */
void ls_list_append(const lsList *list, void *data);

/**
 * @brief 链表头部追加节点
 * 
 * @param list 链表指针
 * @param data 数据
 */
void ls_list_prepend(const lsList *list, void *data);

/**
 * @brief 链表打印
 * 
 * @param list 链表指针
 */
void ls_list_print(const lsList *list, callback_node_print cb);

// 链表迭代器
typedef struct
{
    lsNode *current;
} lsListIterator;

/**
 * @brief 创建并初始化一个链表迭代器
 * 
 * @param list 需要迭代的链表指针
 * @return lsListIterator 返回迭代器
 */
lsListIterator ls_list_iterator_start(const lsList *list);

/**
 * @brief 判断迭代器是否到达链表尾节点之后
 * 
 * @param it 迭代器指针
 * @return true 迭代器已到达尾节点之后
 * @return false 迭代器未到达尾节点之后
 */
bool ls_list_iterator_done(lsListIterator *it);

/**
 * @brief 迭代器前进一个节点
 * 
 * @param it 迭代器指针
 */
void ls_list_iterator_step(lsListIterator *it);

/**
 * @brief 通过迭代器获取当前节点数据
 * 
 * @param it 迭代器指针
 * @return void* 返回节点数据指针
 */
void *ls_list_iterator_get_data(lsListIterator *it);
