#include "list.h"

int ListInit(List* list, int capacity)
{
    assert(list);

    ListElem* given_memory = (ListElem*)malloc((capacity) * sizeof(ListElem));

    if (given_memory == NULL)
    {
        printf("FUUUUCK, malloc failed\n");
        return 1;
    }
    
    list->data = given_memory; 
    list->size = 0;
    list->capacity = capacity;

    list->head = -1;
    list->tail = -1;
    list->free_head = 0;

    for (int i = 0; i < capacity - 1; i++) // the last elem is processed separately
    {
        list->data[i].value = EMPTY;
        list->data[i].next = i + 1;
        list->data[i].prev = -1;
    }

    list->data[capacity - 1].value = EMPTY;
    list->data[capacity - 1].prev = -1;
    list->data[capacity - 1].next = -1; // last elem has no next elem
    list->list_status = OKAY;

    #ifdef DUMP_EACH_STEP
    LIST_DUMP(list);
    #endif

    return 0;
}

void ListPrint(List* list)
{
    assert(list);

    printf("\nLinked list elems:\n");
    if (list->capacity <= 0)
    {
        PRINT_RED("NON-POSITIVE CAPACITY - CANNOT PRINT ANYTHING\n");
        return;
    }

    printf("\nseq_index   data_index  value   prev    next\n");

    int elem_counter = 0;
    int curr_idx = list->head;

    while (curr_idx != -1)
    {   
        if (elem_counter >= list->capacity)
        {
            PRINT_RED("Loop found - ListPrint is stopped\n");
            break; 
        }

        ListElem* curr_elem = &list->data[curr_idx];
        
        printf("%9d %11d %6d %6d %7d\n", elem_counter, curr_idx, curr_elem->value, curr_elem->prev, curr_elem->next);
        
        curr_idx = curr_elem->next;
        elem_counter++;
    }
}


int ListGetFreeIndex(List* list)
{
    assert(list);
    ASSERT_LIST_OK(list);

    int free_idx = list->free_head;

    if (free_idx != -1)
        list->free_head = list->data[free_idx].next;

    return free_idx;
}

int ListGetDataIndex(List* list, int seq_idx)
{
    assert(list);
    ASSERT_LIST_OK(list);

    if (seq_idx < 0 || seq_idx >= list->size)
    {
        PRINT_RED("seq_idx <%d> out of range\n", seq_idx);
        list->list_status |= INDEX_OUT_OF_RANGE;
        ASSERT_LIST_OK(list);
        return -1;
    }

    int curr_idx = list->head;

    for (int i = 0; i < seq_idx; i++)
        curr_idx = list->data[curr_idx].next;

    return curr_idx;
}

int ListGetValue(List* list, int seq_idx)
{
    assert(list);
    ASSERT_LIST_OK(list);

    if (seq_idx < 0 || seq_idx >= list->size)
    {
        PRINT_RED("Index %d is out of range\n\n", seq_idx);
        list->list_status |= INDEX_OUT_OF_RANGE;
        return EMPTY;
    }
    
    return list->data[ListGetDataIndex(list, seq_idx)].value;
}

int ListAppend(List* list, list_elem_t value)
{
    assert(list);
    ASSERT_LIST_OK(list);

    int free_idx = ListGetFreeIndex(list);

    if (free_idx == -1)
    {
        PRINT_RED("ListOverFlow: cannot insert new value into full list\n");
        list->list_status = LIST_OVERFLOW;
        ASSERT_LIST_OK(list);
        return 1;
    }

    list->data[free_idx].value = value;

    if (list->size == 0)  // the very first insert
    {
        list->head = free_idx;
        list->tail = free_idx;
        list->data[free_idx].prev = -1;
        list->data[free_idx].next = -1;
    }

    else
    {
        list->data[list->tail].next = free_idx;
        list->data[free_idx].prev = list->tail;
        list->data[free_idx].next = -1;  // next elem for new tail is -1
        list->tail = free_idx;
    }

    #ifdef DUMP_EACH_STEP
    LIST_DUMP(list);
    #endif

    list->size++;
    ASSERT_LIST_OK(list);
    return 0;
}

int ListPrepend(List* list, list_elem_t value)
{
    assert(list);
    ASSERT_LIST_OK(list);

    int free_idx = ListGetFreeIndex(list);

    if (free_idx == -1)
    {
        PRINT_RED("ListOverFlow: cannot insert new value into full list\n");
        list->list_status |= LIST_OVERFLOW;
        ASSERT_LIST_OK(list);
        return 1;
    }

    list->data[free_idx].value = value;

    if (list->size == 0)  // the very first insert
    {
        list->head = free_idx;
        list->tail = free_idx;
        list->data[free_idx].prev = -1;
        list->data[free_idx].next = -1;
    }
    
    else
    {
        list->data[list->head].prev = free_idx;
        list->data[free_idx].prev = -1;  // prev elem for new head is -1
        list->data[free_idx].next = list->head;
        list->head = free_idx;
    }

    #ifdef DUMP_EACH_STEP
    LIST_DUMP(list);
    #endif

    list->size++;
    return 0;
}

int ListInsert(List* list, list_elem_t value, int seq_idx)
{
    assert(list);
    ASSERT_LIST_OK(list);

    if (seq_idx < 0 || seq_idx > list->size)
    {
        PRINT_RED("Index %d is out of range\n", seq_idx);
        list->list_status |= INDEX_OUT_OF_RANGE;
        return 1;
    }

    int free_idx = ListGetFreeIndex(list);

    if (free_idx == -1)
    {
        PRINT_RED("ListOverFlow: cannot insert new value into full list\n");
        list->list_status |= LIST_OVERFLOW;
        ASSERT_LIST_OK(list);
        return 1;
    }

    list->data[free_idx].value = value;

    if (seq_idx == 0) // the same as prepend
    {
        list->data[free_idx].prev = -1;
        list->data[free_idx].next = list->head;

        if (list->size > 0)  list->data[list->head].prev = free_idx;

        else  list->tail = free_idx;   // case of empty list: inserted elem is both tail and head

        list->head = free_idx;
    }

    else if (seq_idx == list->size) // the same as append
    {
        list->data[free_idx].prev = list->tail;
        list->data[free_idx].next = -1;

        if (list->size > 0)  list->data[list->tail].next = free_idx;

        else  list->head = free_idx;  // case of empty list: inserted elem is both tail and head

        list->tail = free_idx;
    }

    else
    {
        int curr_on_seq_idx = ListGetDataIndex(list, seq_idx);
        int curr_prev = list->data[curr_on_seq_idx].prev;
        
        // linking new elem
        list->data[free_idx].prev = curr_prev;
        list->data[free_idx].next = curr_on_seq_idx;

        // linking old elems
        list->data[curr_prev].next = free_idx;
        list->data[curr_on_seq_idx].prev = free_idx;
    }

    #ifdef DUMP_EACH_STEP
    LIST_DUMP(list);
    #endif

    list->size++;
    return 0;
}

int ListRemove(List* list, int seq_idx)
{
    assert(list);
    ASSERT_LIST_OK(list);

    if (seq_idx < 0 || seq_idx >= list->size)
    {
        PRINT_RED("Index %d is out of range\n");
        list->list_status |= INDEX_OUT_OF_RANGE;
        ASSERT_LIST_OK(list);
        return 1;
    }

    int data_del_idx = ListGetDataIndex(list, seq_idx);

    int prev_del_idx = list->data[data_del_idx].prev;
    int next_del_idx = list->data[data_del_idx].next;

    // destroying links of deleted elem and putting it as a head of free elems
    list->data[data_del_idx].value = EMPTY;
    list->data[data_del_idx].prev = -1;
    list->data[data_del_idx].next = list->free_head;
    list->free_head = data_del_idx;



    if (seq_idx == 0) // deleting head
    {
        list->head = next_del_idx;

        if(list->head == -1)  list->tail = -1;  // list is empty

        else    list->data[list->head].prev = -1;  // new head does not have prev elem
    }

    else if (seq_idx == list->size - 1) // deleting tail
    {
        list->tail = prev_del_idx;

        if (list->tail == -1)  list->head = -1;  // list is empty

        else    list->data[list->tail].next = -1;  // new tail does not have next elem
    }

    else
    {
        list->data[prev_del_idx].next = next_del_idx;
        list->data[next_del_idx].prev = prev_del_idx;
    }

    #ifdef DUMP_EACH_STEP
    LIST_DUMP(list);
    #endif

    list->size--;
    return 0;
}

int ListFindValue(List* list, list_elem_t value)
{
    assert(list);
    ASSERT_LIST_OK(list);

    int curr_idx = list->head;
    int elem_counter = 0;

    while(curr_idx != -1)
    {
        if (list->data[curr_idx].value == value)
            return elem_counter;

        curr_idx = list->data[curr_idx].next;
        elem_counter++;
    }

    return -1;
}

int ListRfindValue(List* list, list_elem_t value)
{
    assert(list);
    ASSERT_LIST_OK(list);

    int curr_idx = list->head;
    int elem_counter = 0;
    int last_seen = -1;

    while(curr_idx != -1)
    {
        if (list->data[curr_idx].value == value)
            last_seen = elem_counter;

        curr_idx = list->data[curr_idx].next;
        elem_counter++;
    }

    return last_seen;    
}

int ListClear(List* list)
{
    assert(list);
    ASSERT_LIST_OK(list);

    list->size = 0;
    list->head = -1;
    list->tail = -1;
    list->free_head = 0;

    for (int i = 0; i < list->capacity - 1; i++) // the last elem is processed separately
    {
        list->data[i].value = EMPTY;
        list->data[i].next = i + 1;
        list->data[i].prev = -1;
    }

    list->data[list->capacity - 1].next = -1; // last elem has no next elem

    ASSERT_LIST_OK(list);
    return 0;
}

ListStatus ListVerify(List* list)
{
    assert(list);
    
    if (list == NULL)
        return LIST_NULL;  // immediately return LIST_NULL as we cannot check anything below (bro, list is NULL)

    int status = OKAY;  // bit operations give warning, so working with int first   

    if (list->data == NULL) 
    {
        status |= DATA_NULL;
    } 
    
    if (list->capacity <= 0)
        list->list_status |= NON_POSITIVE_CAPACITY;

    if (list->size < 0)
        list->list_status |= LIST_UNDERFLOW; // 

    if (list->capacity > 0 && list->size > list->capacity)
        list->list_status |= LIST_OVERFLOW;

    if (list->list_status & DATA_NULL || list->list_status & NON_POSITIVE_CAPACITY)   return (ListStatus)list->list_status; // no sense to check anything below

    // check empty list
    if (list->size == 0)
    {
        if (list->head != -1 || list->tail != -1)
            list->list_status |= LIST_LINKING_ERROR;
        return (ListStatus)list->list_status;
    }

    if (list->data[list->head].prev != -1)
        list->list_status |= LIST_LINKING_ERROR;

    if (list->data[list->tail].next != -1)
        list->list_status |= LIST_LINKING_ERROR;

    // iterate through the list
    int curr_idx = list->head;
    int elem_counter = 0;

    while (curr_idx != -1)
    {
        elem_counter++;
        
        if (elem_counter > list->size)
        {
            list->list_status |= LIST_FOUND_LOOP;
            break;
        }

        int next_idx = list->data[curr_idx].next;

        if (next_idx == -1 && list->tail != curr_idx)
            list->list_status |= LIST_LINKING_ERROR;

        else if (next_idx != -1 && list->data[next_idx].prev != curr_idx)
            list->list_status |= LIST_LINKING_ERROR;

        curr_idx = next_idx;
    }

    return (ListStatus)list->list_status;  // converting to ERROR_STATUS after working with int
}

void ListDump(List* list, const char* file, int line, const char* func)
{   
    printf( "------------ListDump----------------\n");
    printf("ListDump was called in %s:%d in function %s\n", file, line, func);

    ListStatus list_status = ListVerify(list);

    if (list_status == LIST_NULL || list == NULL) 
    {
        PRINT_RED("Linked list is NULL\n");
        printf("---------------------------------------------------\n");
        return;
    }

    printf("list_status = ");
    PrintAllErrors((ListStatus)list->list_status);
    printf("\n");

    printf("Size:      %d\n", list->size);
    printf("Capacity:  %d\n", list->capacity);
    printf("Head:      %d\n", list->head);
    printf("Tail:      %d\n", list->tail);
    printf("Free head: %d\n", list->free_head);

    printf("data [" GREEN "%p" RESET "]\n", list->data);

    if (!(list_status & NON_POSITIVE_CAPACITY || list_status & DATA_NULL))  ListPrint(list);

    else    PRINT_RED("CANNOT READ DATA\n");

    printf("----------------------------------------------\n");

    PRINT_BLUE("MOVE TO NEXT STEP after current LIST_DUMP: ");
    getchar();
}

void ListDestroy(List* list)
{
    if (list == NULL)  return;

    ASSERT_LIST_OK(list);

    if (list->data != NULL)
    {   
        for (int i = 0; i < list->capacity; i++)
        {
            list->data[i].value = EMPTY;
            list->data[i].next  = -1;
            list->data[i].prev  = -1;
        }

        free(list->data);
        list->data = NULL;
    }

    list->size = 0;
    list->capacity = 0;
    list->head = -1;
    list->tail = -1;
}

int ListGenerateNodeDump(FILE* dot_file, List* list, int data_idx, INDEX_TYPE idx_type) // ListGenerateDotFile helper func
{
    assert(dot_file);
    assert(list);

    const char* bgcolor = "#40E0D0"; // case of busy elems

    if (idx_type == FREE_HEAD || idx_type == FREE_MIDDLE)
        bgcolor = "#48A43F"; // case of free elems

    fprintf(
        dot_file, 
        "        node_%d [label=<\n"
        "            <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" COLOR=\"#354733\" BGCOLOR=\"%s\">\n"
        "                <TR><TD COLSPAN=\"2\" ALIGN=\"CENTER\" BORDER=\"1\">INDEX=%d</TD></TR>\n"
        "                <TR><TD COLSPAN=\"2\" ALIGN=\"CENTER\" BORDER=\"1\">DATA=%d</TD></TR>\n"
        "                <TR>\n"
        "                    <TD PORT=\"p\" WIDTH=\"70\" ALIGN=\"CENTER\" BORDER=\"1\">PREV=%d</TD>\n"
        "                    <TD PORT=\"n\" WIDTH=\"70\" ALIGN=\"CENTER\" BORDER=\"1\">NEXT=%d</TD>\n"
        "                </TR>\n"
        "            </TABLE>\n"
        "        >];\n\n",
        data_idx, bgcolor, data_idx, list->data[data_idx].value, list->data[data_idx].prev, list->data[data_idx].next
            );
    
    if (idx_type == HEAD)
    {
        fprintf(dot_file, "        {rank=same; node_%d; head_label;}\n", data_idx);
        fprintf(dot_file, "        head_label -> node_%d\n\n", data_idx);
    }

    else if (idx_type == TAIL)
    {
        fprintf(dot_file, "        {rank=same; node_%d; tail_label;}\n", data_idx);
        fprintf(dot_file, "        tail_label -> node_%d\n\n", data_idx);        
    }

    else if (idx_type == FREE_HEAD)
    {
        fprintf(dot_file, "        {rank=same; node_%d; free_head_label;}\n", data_idx);
        fprintf(dot_file, "        free_head_label -> node_%d\n\n", data_idx);
    }
        
    return 0;
}

int ListGenerateDotFile(List* list, const char* dot_filename)
{
    assert(list);
    assert(dot_filename);
    ASSERT_LIST_OK(list);

    FILE* dot_file = fopen(dot_filename, "w");

    if (dot_file == NULL)
    {
        PRINT_RED("%s failed to open %s - cannot generate .dot file for graph dump\n", __func__, dot_filename);
        return 1;
    }

    fprintf(dot_file, "digraph G {\n");
    fprintf(dot_file, "    rankdir=LR;\n");
    fprintf(dot_file, "    nodesep=0.6;\n\n");
    fprintf(dot_file, "    node [shape=plain, fontcolor=\"#212529\", fontname=\"Arial\", fontsize=11];\n");
    fprintf(dot_file, "    edge [color=\"#354733\", arrowsize=0.6]\n\n");
    fprintf(dot_file, "    subgraph cluster_busy_nodex {\n");
    fprintf(dot_file, "        head_label [shape=box, style=filled, color=\"#354733\", fillcolor=yellow, label=\"HEAD\", fontcolor=black];\n");
    fprintf(dot_file, "        tail_label [shape=box, style=filled, color=\"#354733\", fillcolor=purple, label=\"TAIL\", fontcolor=black];\n\n");
    
    int elem_counter = 0;
    int curr_idx = list->head;

    while (curr_idx != -1)
    {   
        if (elem_counter >= list->capacity)
        {
            PRINT_RED("Loop found - ListPrint is stopped\n");
            break; 
        }

        INDEX_TYPE idx_type = UNKNOWN;
        if (curr_idx == list->head)  idx_type = HEAD;
        
        else if (curr_idx == list->tail)  idx_type = TAIL;

        else    idx_type = MIDDLE;
        
        ListGenerateNodeDump(dot_file, list, curr_idx, idx_type);
        
        curr_idx = list->data[curr_idx].next;
        elem_counter++;
    }

    elem_counter = 0;
    curr_idx = list->head;

    while (curr_idx != -1)
    {   
        if (elem_counter >= list->capacity)
        {
            PRINT_RED("Loop found - ListPrint is stopped\n");
            break; 
        }
        
        ListElem* curr_elem = &list->data[curr_idx];

        if (curr_elem->prev != -1 && curr_elem->prev < list->capacity)
            fprintf(dot_file, "        node_%d:p -> node_%d:n\n", curr_idx, curr_elem->prev);

        if (curr_elem->next != -1 && curr_elem->next < list->capacity)
            fprintf(dot_file, "        node_%d:n -> node_%d:p\n", curr_idx, curr_elem->next);
        
        curr_idx = list->data[curr_idx].next;
        elem_counter++;
    }

    fprintf(dot_file, "    }\n\n");


    fprintf(dot_file, "    subgraph cluster_free_nodes {\n");
    fprintf(dot_file, "        free_head_label [shape=box, style=filled, color=\"#354733\", fillcolor=yellow, label=\"FREE\", fontcolor=black];\n\n");
    elem_counter = 0;
    curr_idx = list->free_head;

    while (curr_idx != -1)
    {
        if (elem_counter > list->capacity)
        {
            PRINT_RED("Loop found - ListPrint is stopped\n");
            break;             
        }

        INDEX_TYPE idx_type = UNKNOWN;
        if (curr_idx == list->free_head)  idx_type = FREE_HEAD;

        else    idx_type = FREE_MIDDLE;
        
        ListGenerateNodeDump(dot_file, list, curr_idx, idx_type);
        
        curr_idx = list->data[curr_idx].next;
        elem_counter++;        
    }

    elem_counter = 0;
    curr_idx = list->free_head;

    while (curr_idx != -1)
    {   
        if (elem_counter >= list->capacity)
        {
            PRINT_RED("Loop found - ListPrint is stopped\n");
            break; 
        }
        
        ListElem* curr_elem = &list->data[curr_idx];

        if (curr_elem->prev != -1 && curr_elem->prev < list->capacity)
            fprintf(dot_file, "        node_%d:p -> node_%d:n\n", curr_idx, curr_elem->prev);

        if (curr_elem->next != -1 && curr_elem->next < list->capacity)
            fprintf(dot_file, "        node_%d:n -> node_%d:p\n", curr_idx, curr_elem->next);
        
        curr_idx = list->data[curr_idx].next;
        elem_counter++;
    }

    fprintf(dot_file, "    }\n\n");

    fprintf(dot_file, "}\n");

    fclose(dot_file);

    return 0;
}

int ListGraphDump(List* list, const char* dot_filename, const char* png_filename)
{
    assert(list);
    assert(dot_filename);
    assert(png_filename);

    ListGenerateDotFile(list, dot_filename);

    char temp_term_command[100] = {};

    sprintf(temp_term_command, "dot -Tpng %s -o %s", dot_filename, png_filename);

    const char* terminal_command = temp_term_command;

    system(terminal_command);
    
    return 0;
}