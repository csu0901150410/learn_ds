#include "../src/ls_list.h"

#include <stdio.h>
#include <stdlib.h>

// int型数据节点打印函数
void node_int_print(const void *data)
{
    printf("value : %d\n", *(int*)data);
}

void test_list_basic()
{
    lsList *list = ls_list_create();

    int a[10] = {1, 2, 3, 4, 5, 6};

    ls_list_append(list, a + 0);
    ls_list_append(list, a + 1);
    ls_list_append(list, a + 2);
    ls_list_append(list, a + 3);
    ls_list_prepend(list, a + 4);
    ls_list_prepend(list, a + 5);

    ls_list_print(list, node_int_print);

    ls_list_destroy(&list);

    ls_list_print(list, node_int_print);// 再次打印，因链表已经销毁，所以没有输出了
}

void test_list_iterator()
{
    lsList *list = ls_list_create();

    int a[10] = {1, 2, 3, 4, 5, 6};

    ls_list_append(list, &a[0]);
    ls_list_append(list, a + 1);
    ls_list_append(list, a + 2);
    ls_list_append(list, a + 3);

    // 用迭代器遍历链表，自定义打印
    for (lsListIterator it = ls_list_iterator_start(list); !ls_list_iterator_done(&it); ls_list_iterator_step(&it))
    {
        // 可以参考这种取数据的方式，实现往链表上挂载任意数据
        // 即，数据域存储指向数据的指针，需要获取数据的时候，根据这个指针读取数据
        const int *pData = ls_list_iterator_get_data(&it);
        printf("node data : %d\n", *pData);
    }

    ls_list_destroy(&list);
}

int main()
{
    // test_list_iterator();

    test_list_basic();

    return 0;
}
