#ifndef LINKED_LIST_ERROR_H

#define LINKED_LIST_ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "UTILS/PrintColor.h"

enum ListStatus
{
    OKAY = 0,
    DATA_NULL = 1 << 1,
    LIST_OVERFLOW = 1 << 2,
    LIST_UNDERFLOW = 1 << 3,
    NON_POSITIVE_CAPACITY = 1 << 4,
    LIST_NULL = 1 << 5,
};

const char*     GetErrorString      (ListStatus list_status);

void            PrintError          (ListStatus list_status, ListStatus error, bool* first);

void            PrintAllErrors      (ListStatus list_status);

void            ProcessError        (ListStatus list_status);

#endif