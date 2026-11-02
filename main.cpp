#include "list/list.h"

int main(void)
{
    List list = {};
    ListInit(&list, 20);
    // LINKED_LIST_DUMP(&list);


    for (int i = 0; i < 10; i++) ListAppend(&list, i * 100);

    // list.capacity = -1;

    // for (int i = 0; i < 10; i++) ListRemove(&list, 0);    

    // list.data[9].next = 1;
    // ListInsert(&list, 69, 10);

    // ListRemove(&list, 5);
    // LIST_DUMP(&list);
    ListGraphDump(&list, "test_dump.dot", "test.png");

    ListDestroy(&list);

    return EXIT_SUCCESS;
}