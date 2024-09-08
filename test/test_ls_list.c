#include "../src/ls_list.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
    lsList *list = ls_list_create();

    ls_list_append(list, 1);
    ls_list_append(list, 2);
    ls_list_append(list, 3);
    ls_list_append(list, 4);

    ls_list_print(list);

    ls_list_destroy(&list);

    ls_list_print(list);// 再次打印，因链表已经销毁，所以没有输出了

    return 0;
}
