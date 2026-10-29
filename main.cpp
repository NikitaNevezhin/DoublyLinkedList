#include "linked_list.h"

int main(void)
{
    LinkedList linked_lst = {};
    LinkedListInit(&linked_lst, 20);
    // LINKED_LIST_DUMP(&linked_lst);


    for (int i = 0; i < 10; i++) LinkedListAppend(&linked_lst, i * 10);

    // linked_lst.capacity = -1;

    for (int i = 0; i < 10; i++) LinkedListAppend(&linked_lst, i * 10);    

    // linked_lst.data[2].next = 1;
    // LinkedListInsert(&linked_lst, 69, 10);

    LINKED_LIST_DUMP(&linked_lst);

    LinkedListDestroy(&linked_lst);

    return EXIT_SUCCESS;
}
