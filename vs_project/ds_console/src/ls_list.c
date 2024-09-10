#include "ls_list.h"

#include <stdio.h>// for printf
#include <stdlib.h>// for malloc free
#include <assert.h>// for assert

lsNode *ls_node_create(int data)
{
    lsNode *node = (lsNode *)malloc(sizeof(lsNode));
    assert(node);
    node->next = NULL;
    node->data = data;
    return node;
}

lsList *ls_list_create()
{
    lsList *list = (lsList *)malloc(sizeof(lsList));
    assert(list);

    lsNode *head = ls_node_create(0);
    assert(head);
    list->head = head;
    return list;
}

void ls_list_destroy(lsList **root)
{
    // root是外部链表指针变量的地址
    if (NULL == root)
        return;

    // *root是外部链表指针变量
    lsList *list = *root;
    if (NULL == list)
        return;
    
    // 链表内部从头节点开始，删除链表上的所有挂载的节点
    lsNode *curr = list->head;
    while (1)
    {
        // 当前节点指针为空，当前节点不存在，所有节点已遍历完，退出循环
        if (NULL == curr)
            break;

        // 先保存当前节点的下一个节点，防止当前节点释放之后，丢失当前节点之后的连接关系
        lsNode *next = curr->next;

        // 释放当前节点
        free(curr);

        // 迭代，当前节点移动到下一个节点
        curr = next;
    }

    // 释放链表本身的结构体空间
    free(list);

    // 将外部链表指针变量值设为NULL，防止此处链表释放后，外部链表指针变量仍然指向释放后的链表空间
    // note : 这种情况，空间已释放，但是指针还指向释放后的空间，这个指针就叫野指针，使用野指针访问空间是不合法的
    *root = NULL;
}

// 这里的const是防止意外修改掉内部head指针的指向，导致链表被破坏
void ls_list_append(const lsList *list, int data)
{
    if (NULL == list)
        return;
    assert(list->head);// 链表只要创建了，头节点指针就不能为空，也就是链表内部一定有一个头节点存在

    lsNode *node = ls_node_create(data);
    assert(node);

    // 链表内部从头节点开始，遍历所有节点，找到尾节点
    lsNode *curr = list->head;
    while (1)
    {
        // 尾节点的特征是next指针域为空，没有下一个节点
        if (NULL == curr->next)
            break;

        // 迭代，当前节点移动到下一个节点
        curr = curr->next;
    }

    // curr指向了尾节点，将尾节点的指针域指向新节点
    curr->next = node;
}

void ls_list_head_append(const lsList *list, int data){
    if(NULL==list)
        return;
    assert(list->head);

    lsNode *Node = ls_node_create(data);
    assert(Node);

    lsNode *curr = list->head;
    
    Node->next = curr->next;
    curr->next = Node;
}

// 这里的const是防止意外修改掉内部head指针的指向，导致链表被破坏
void ls_list_print(const lsList *list)
{
    if (NULL == list)
        return;
    assert(list->head);

    // 遍历链表上除了头节点外的节点
    lsNode *curr = list->head->next;
    int index = 0;
    printf("list print start ...\n");
    while (1)
    {
        // 当前节点指针为空，当前节点不存在，所有节点已遍历完，退出循环
        if (NULL == curr)
            break;

        // 具体的打印动作
        printf("index : %d, value : %d\n", index++, curr->data);

        // 迭代，当前节点移动到下一个节点
        curr = curr->next;
    }
    printf("list print end ...\n");
}

lsListIterator ls_list_iterator_start(const lsList *list)
{
    lsListIterator it;
    it.current = (NULL == list) ? NULL : list->head->next;
    return it;
}

bool ls_list_iterator_done(lsListIterator *it)
{
    return (NULL == it->current);
}

void ls_list_iterator_step(lsListIterator *it)
{
    assert(NULL != it->current);
    it->current = it->current->next;
}

void *ls_list_iterator_get_data(lsListIterator *it)
{
    assert(NULL != it->current);
    return &(it->current->data);
}
