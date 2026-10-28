#include "linked_list.h"

int LinkedListInit(LinkedList* linked_lst, int capacity)
{
    assert(linked_lst);

    LinkedListElem* given_memory = (LinkedListElem*)malloc((capacity) * sizeof(LinkedListElem));

    if (given_memory == NULL)
    {
        printf("FUUUUCK, malloc failed\n");
        return 1;
    }
    
    linked_lst->data = given_memory; 
    linked_lst->size = 0;
    linked_lst->capacity = capacity;

    linked_lst->head = -1;
    linked_lst->tail = -1;

    for (int i = 0; i < capacity; i++) 
    {
        linked_lst->data[i].value = EMPTY;
        linked_lst->data[i].next = -1;
        linked_lst->data[i].prev = -1;
    }

    #ifdef DUMP_EACH_STEP
    LINKED_LIST_DUMP(linked_lst);
    #endif

    return 0;
}

void PrintLinkedList(LinkedList* linked_lst)
{
    assert(linked_lst);

    printf("\nLinked list elems:\n");
    if (linked_lst->capacity <= 0)
    {
        PRINT_RED("NON-POSITIVE CAPACITY - CANNOT PRINT ANYTHING\n");
        return;
    }

    printf("\nseq_index   data_index  value   prev    next\n");

    int elem_counter = 0;
    int curr_idx = linked_lst->head;

    while (curr_idx != -1)
    {   
        if (elem_counter >= linked_lst->capacity)
        {
            PRINT_RED("Loop found - PrintLinkedList is stopped\n");
            break; 
        }

        LinkedListElem* curr_elem = &linked_lst->data[curr_idx];
        
        printf("%9d %11d %6d %6d %7d\n", elem_counter, curr_idx, curr_elem->value, curr_elem->prev, curr_elem->next);
        
        curr_idx = curr_elem->next;
        elem_counter++;
    }
}


int FindFreeIndex(LinkedList* linked_lst)
{
    assert(linked_lst);
    ASSERT_LIST_OK(linked_lst);

    if (linked_lst->size == linked_lst->capacity)
        return -1;

    for (int i = 0; i < linked_lst->capacity; i++)
    {
        if (linked_lst->data[i].value == EMPTY)
            return i;
    }

    return -1;
}

int GetDataIndex(LinkedList* linked_lst, int seq_idx)
{
    assert(linked_lst);
    ASSERT_LIST_OK(linked_lst);

    if (seq_idx < 0 || seq_idx >= linked_lst->size)
    {
        PRINT_RED("seq_idx <%d> out of range\n", seq_idx);
        return -1;
    }

    int curr_idx = linked_lst->head;

    for (int i = 0; i < seq_idx; i++)
        curr_idx = linked_lst->data[curr_idx].next;

    return curr_idx;
}

int LinkedListGetValue(LinkedList* linked_lst, int seq_idx)
{
    assert(linked_lst);
    ASSERT_LIST_OK(linked_lst);

    if (seq_idx < 0 || seq_idx >= linked_lst->size)
    {
        PRINT_RED("Index %d is out of range\n\n", seq_idx);
        return EMPTY;
    }
    
    return linked_lst->data[GetDataIndex(linked_lst, seq_idx)].value;
}

int LinkedListAppend(LinkedList* linked_lst, list_elem_t value)
{
    assert(linked_lst);
    ASSERT_LIST_OK(linked_lst);

    int free_idx = FindFreeIndex(linked_lst);

    if (free_idx == -1)
    {
        PRINT_RED("LinkedListOverFlow: cannot insert new value into full list\n");
        return 1;
    }

    linked_lst->data[free_idx].value = value;

    if (linked_lst->size == 0)  // the very first insert
    {
        linked_lst->head = free_idx;
        linked_lst->tail = free_idx;
        linked_lst->data[free_idx].prev = -1;
    }

    else
    {
        linked_lst->data[linked_lst->tail].next = free_idx;
        linked_lst->data[free_idx].prev = linked_lst->tail;
        linked_lst->data[free_idx].next = -1;  // next elem for new tail is -1
        linked_lst->tail = free_idx;
    }

    #ifdef DUMP_EACH_STEP
    LINKED_LIST_DUMP(linked_lst);
    #endif

    linked_lst->size++;
    return 0;
}

int LinkedListPrepend(LinkedList* linked_lst, list_elem_t value)
{
    assert(linked_lst);
    ASSERT_LIST_OK(linked_lst);

    int free_idx = FindFreeIndex(linked_lst);

    if (free_idx == -1)
    {
        PRINT_RED("LinkedListOverFlow: cannot insert new value into full list\n");
        return 1;
    }

    linked_lst->data[free_idx].value = value;

    if (linked_lst->size == 0)  // the very first insert
    {
        linked_lst->head = free_idx;
        linked_lst->tail = free_idx;
        linked_lst->data[free_idx].prev = -1;
        linked_lst->data[free_idx].next = -1;
    }
    
    else
    {
        linked_lst->data[linked_lst->head].prev = free_idx;
        linked_lst->data[free_idx].prev = -1;  // prev elem for new head is -1
        linked_lst->data[free_idx].next = linked_lst->head;
        linked_lst->head = free_idx;
    }

    #ifdef DUMP_EACH_STEP
    LINKED_LIST_DUMP(linked_lst);
    #endif

    linked_lst->size++;
    return 0;
}

int LinkedListInsert(LinkedList* linked_lst, list_elem_t value, int seq_idx)
{
    assert(linked_lst);
    ASSERT_LIST_OK(linked_lst);

    if (seq_idx < 0 || seq_idx > linked_lst->size)
    {
        PRINT_RED("Index %d is out of range\n", seq_idx);
        return 1;
    }

    int free_idx = FindFreeIndex(linked_lst);

    if (free_idx == -1)
    {
        PRINT_RED("LinkedListOverFlow: cannot insert new value into full list\n");
        return 1;
    }

    linked_lst->data[free_idx].value = value;

    if (seq_idx == 0) // the same as prepend
    {
        linked_lst->data[free_idx].prev = -1;
        linked_lst->data[free_idx].next = linked_lst->head;

        if (linked_lst->size > 0)  linked_lst->data[linked_lst->head].prev = free_idx;

        else  linked_lst->tail = free_idx;   // case of empty linked_lst: inserted elem is both tail and head

        linked_lst->head = free_idx;
    }

    else if (seq_idx == linked_lst->size) // the same as append
    {
        linked_lst->data[free_idx].prev = linked_lst->tail;
        linked_lst->data[free_idx].next = -1;

        if (linked_lst->size > 0)  linked_lst->data[linked_lst->tail].next = free_idx;

        else  linked_lst->head = free_idx;  // case of empty linked_lst: inserted elem is both tail and head

        linked_lst->tail = free_idx;
    }

    else
    {
        int curr_on_seq_idx = GetDataIndex(linked_lst, seq_idx);
        int curr_prev = linked_lst->data[curr_on_seq_idx].prev;
        
        // linking new elem
        linked_lst->data[free_idx].prev = curr_prev;
        linked_lst->data[free_idx].next = curr_on_seq_idx;

        // linking old elems
        linked_lst->data[curr_prev].next = free_idx;
        linked_lst->data[curr_on_seq_idx].prev = free_idx;
    }

    #ifdef DUMP_EACH_STEP
    LINKED_LIST_DUMP(linked_lst);
    #endif

    linked_lst->size++;
    return 0;
}

int LinkedListDelete(LinkedList* linked_lst, int seq_idx)
{
    assert(linked_lst);
    ASSERT_LIST_OK(linked_lst);

    if (seq_idx < 0 || seq_idx >= linked_lst->size)
    {
        PRINT_RED("Index %d is out of range\n");
        return 1;
    }

    int data_del_idx = GetDataIndex(linked_lst, seq_idx);

    int prev_del_idx = linked_lst->data[data_del_idx].prev;
    int next_del_idx = linked_lst->data[data_del_idx].next;

    // destroying links of deleted elem
    linked_lst->data[data_del_idx].value = EMPTY;
    linked_lst->data[data_del_idx].prev = -1;
    linked_lst->data[data_del_idx].next = -1;



    if (seq_idx == 0) // deleting head
    {
        linked_lst->head = next_del_idx;

        if(linked_lst->head == -1)  linked_lst->tail = -1;  // list is empty

        else    linked_lst->data[linked_lst->head].prev = -1;  // new head does not have prev elem
    }

    else if (seq_idx == linked_lst->size - 1) // deleting tail
    {
        linked_lst->tail = prev_del_idx;

        if (linked_lst->tail == -1)  linked_lst->head = -1;  // list is empty

        else    linked_lst->data[linked_lst->tail].next = -1;  // new tail does not have next elem
    }

    else
    {
        linked_lst->data[prev_del_idx].next = next_del_idx;
        linked_lst->data[next_del_idx].prev = prev_del_idx;
    }

    #ifdef DUMP_EACH_STEP
    LINKED_LIST_DUMP(linked_lst);
    #endif

    linked_lst->size--;
    return 0;
}

ListStatus LinkedListVerify(LinkedList* linked_lst)
{
    assert(linked_lst);
    
    if (linked_lst == NULL)
        return LIST_NULL;  // immediately return LIST_NULL as we cannot check anything below (bro, list is NULL)

    int status = OKAY;  // bit operations give warning, so working with int first   

    if (linked_lst->data == NULL) 
    {
        status |= DATA_NULL;
    } 
    
    if (linked_lst->capacity <= 0)
        status |= NON_POSITIVE_CAPACITY;

    if (linked_lst->size < 0)
        status |= LIST_UNDERFLOW; // 

    if (linked_lst->capacity > 0 && linked_lst->size > linked_lst->capacity)
        status |= LIST_OVERFLOW;

    if (status & DATA_NULL || status & NON_POSITIVE_CAPACITY)   return (ListStatus)status; // no sense to check anything below

    // check empty list
    if (linked_lst->size == 0)
    {
        if (linked_lst->head != -1 || linked_lst->tail != -1)
            status |= LIST_LINKING_ERROR;
        return (ListStatus)status;
    }

    if (linked_lst->data[linked_lst->head].prev != -1)
        status |= LIST_LINKING_ERROR;

    if (linked_lst->data[linked_lst->tail].next != -1)
        status |= LIST_LINKING_ERROR;

    // iterate through the list
    int curr_idx = linked_lst->head;
    int elem_counter = 0;

    while (curr_idx != -1)
    {
        elem_counter++;
        
        if (elem_counter > linked_lst->size)
        {
            status |= LIST_FOUND_LOOP;
            break;
        }

        int next_idx = linked_lst->data[curr_idx].next;

        if (next_idx == -1 && linked_lst->tail != curr_idx)
            status |= LIST_LINKING_ERROR;

        else if (next_idx != -1 && linked_lst->data[next_idx].prev != curr_idx)
            status |= LIST_LINKING_ERROR;

        curr_idx = next_idx;
    }

    return (ListStatus)status;  // converting to ERROR_STATUS after working with int
}

void LinkedListDump(LinkedList* linked_lst, const char* file, int line, const char* func)
{   
    printf( "------------LinkedListDump----------------\n");
    printf("LinkedListDump was called in %s:%d in function %s\n", file, line, func);

    ListStatus list_status = LinkedListVerify(linked_lst);

    if (list_status == LIST_NULL || linked_lst == NULL) 
    {
        PRINT_RED("Linked list is NULL\n");
        printf("---------------------------------------------------\n");
        return;
    }

    printf("list_status = ");
    PrintAllErrors(list_status);
    printf("\n");

    printf("Size:      %d\n", linked_lst->size);
    printf("Capacity:  %d\n", linked_lst->capacity);

    printf("data [" GREEN "%p" RESET "]\n", linked_lst->data);

    if (!(list_status & NON_POSITIVE_CAPACITY || list_status & DATA_NULL))  PrintLinkedList(linked_lst);

    else    PRINT_RED("CANNOT READ DATA\n");

    printf("----------------------------------------------\n");

    PRINT_BLUE("MOVE TO NEXT STEP after current LINKED_LIST_DUMP: ");
    getchar();
}

void LinkedListDestroy(LinkedList* linked_lst)
{
    if (linked_lst == NULL)  return;

    ASSERT_LIST_OK(linked_lst);

    if (linked_lst->data != NULL)
    {   
        for (int i = 0; i < linked_lst->capacity; i++)
        {
            linked_lst->data[i].value = EMPTY;
            linked_lst->data[i].next  = -1;
            linked_lst->data[i].prev  = -1;
        }

        free(linked_lst->data);
        linked_lst->data = NULL;
    }

    linked_lst->size = 0;
    linked_lst->capacity = 0;
    linked_lst->head = -1;
    linked_lst->tail = -1;
} 