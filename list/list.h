#ifndef LINKED_LIST_H

#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../utils/PrintColor.h"
#include "list_error.h"
#include "assert_list_ok.h"

typedef int list_elem_t;

const long long     DATA_LEFT_CANARY   = 0xDEDDEDDED;

const long long     DATA_RIGHT_CANARY  = 0xEDAEDAEDA;

const list_elem_t               EMPTY  = -66666666;


struct ListElem
{
    list_elem_t value;
    int prev;           // index in data!!!!
    int next;           // index in data!!!!
};

struct List
{
    ListElem* data;
    int capacity;
    int size;

    int list_status;    // bit shifts are going to be used, required type is 'int'

    int tail;           // index in data!!!!
    int head;           // index in data!!!!
    int free_head;      // index in data!!!!
};

#define LIST_DUMP(list)  ListDump(list, __FILE__, __LINE__, __func__)

int            ListInit          (List* list, int capacity);

void           ListPrint         (List* list);
        
int            ListGetFreeIndex  (List* list);  // returns index from data
        
int            ListGetDataIndex  (List* list, int seq_idx);
        
int            ListGetValue      (List* list, int seq_idx);
        
int            ListAppend        (List* list, list_elem_t value);  // add new elem at the end, after tail
        
int            ListPrepend       (List* list, list_elem_t value);
        
int            ListInsert        (List* list, list_elem_t value, int seq_idx); // gets sequence index, not data index

int            ListRemove        (List* list, int seq_idx);

int            ListFindValue     (List* list, list_elem_t value);  // returns seq_index

int            ListRfindValue    (List* list, list_elem_t value);  // returns seq_index

int            ListClear         (List* list);

ListStatus     ListVerify        (List* list);

void           ListDump          (List* list, const char* file, int line, const char* func);

void           ListDestroy       (List* list);

#endif