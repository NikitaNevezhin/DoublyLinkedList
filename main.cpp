#include "linked_list.h"

int main(void)
{
    LinkedList linked_lst = {};
    LinkedListInit(&linked_lst, 20);

    for (int i = 1; i < 10; i++) LinkedListAppend(&linked_lst, i * 10);

    linked_lst.data[2].next = 1;

    LINKED_LIST_DUMP(&linked_lst);

    return EXIT_SUCCESS;
}
