#pragma once

// 节点结构体定义
typedef struct _tag_lsNode
{
    struct _tag_lsNode *next;// 节点指针域  note : 这里涉及到不完全类型指针的使用
    int data;// 节点数据域
} lsNode;

/**
 * @brief 节点创建
 * 
 * @param data 节点数据
 * @return lsNode* 返回节点指针
 */
lsNode *ls_node_create(int data);

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
 * @param data 数据
 */
void ls_list_append(const lsList *list, int data);

/**
 * @brief 链表打印
 * 
 * @param list 链表指针
 */
void ls_list_print(const lsList *list);
