#ifndef LINKED_LIST_H

#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "UTILS/PrintColor.h"
#include "linked_list_error.h"
#include "assert_list_ok.h"

typedef int list_elem_t;

const long long     DATA_LEFT_CANARY   = 0xDEDDEDDED;

const long long     DATA_RIGHT_CANARY  = 0xEDAEDAEDA;

const list_elem_t               EMPTY  = -66666666;


struct LinkedListElem
{
    list_elem_t value;
    int prev;           // index in data!!!!
    int next;           // index in data!!!!
};

struct LinkedList
{
    LinkedListElem* data;
    int capacity;
    int size;

    int tail;           // index in data!!!!
    int head;           // index in data!!!!
};

#define LINKED_LIST_DUMP(linked_lst)  LinkedListDump(linked_lst, __FILE__, __LINE__, __func__)

int            LinkedListInit          (LinkedList* linked_lst, int capacity);

void           PrintLinkedList         (LinkedList* linked_lst);
        
int            FindFreeIndex           (LinkedList* linked_lst);  // returns index from data
        
int            GetDataIndex            (LinkedList* linked_lst, int seq_idx);
        
int            LinkedListGetValue      (LinkedList* linked_lst, int seq_idx);
        
int            LinkedListAppend        (LinkedList* linked_lst, list_elem_t value);  // add new elem at the end, after tail
        
int            LinkedListPrepend       (LinkedList* linked_lst, list_elem_t value);
        
int            LinkedListInsert        (LinkedList* linked_lst, list_elem_t value, int seq_idx); // gets sequence index, not data index

int            LinkedListDelete        (LinkedList* linked_lst, int seq_idx);

ListStatus     LinkedListVerify        (LinkedList* linked_lst);

void           LinkedListDump          (LinkedList* linked_lst, const char* file, int line, const char* func);

void           LinkedListDestroy       (LinkedList* linked_lst);

#endif