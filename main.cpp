#include "list/list.h"

int main(void)
{
    remove("graph_dump.html");
    List list = {};
    ListInit(&list, 10);
    // LINKED_LIST_DUMP(&list);


    for (int i = 0; i < 10; i++)
    {
        ListAppend(&list, i * 100);
        LIST_GRAPH_DUMP(&list, "graph_dump.html");
    }

    ListRemove(&list, 3);

    // list.capacity = -1;

    // for (int i = 0; i < 10; i++) 
    // {
    //     ListRemove(&list, 0);  
    //     LIST_GRAPH_DUMP(&list, "graph_dump.html");
    // }

    // list.data[6].prev = 0;
    // list.data[9].next = 0;

    // list.data[3].next = 5;
    list.data[5].prev = 3;

    list.data[0].prev = 9999;

    LIST_GRAPH_DUMP(&list, "graph_dump.html");
    // ListInsert(&list, 69, 10);

    // ListRemove(&list, 5);
    // LIST_DUMP(&list);
    ListDestroy(&list);

    return EXIT_SUCCESS;
}