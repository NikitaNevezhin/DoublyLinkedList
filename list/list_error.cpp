#include "list_error.h"

const char* GetErrorString(ListStatus list_status) 
{
    switch (list_status) 
    {
        case OKAY:                  return GREEN "OKAY" RESET;

        case DATA_NULL:             return  "DATA_NULL";

        case LIST_OVERFLOW:         return  "LIST_OVERFLOW";

        case LIST_UNDERFLOW:        return  "LIST_UNDERFLOW";

        case NON_POSITIVE_CAPACITY: return  "NON_POSITIVE_CAPACITY";

        case LIST_NULL:             return  "LIST_NULL";

        case LIST_LINKING_ERROR:    return  "LIST_LINKING_ERROR";

        case LIST_FOUND_LOOP:       return  "LIST_FOUND_POOP";

        case INDEX_OUT_OF_RANGE:    return  "INDEX_OUT_OF_RANGE";

        default:                    return  "You are seriously fucked up, man";
    }
}

void PrintError(ListStatus list_status, ListStatus error, bool* first)  // PrintAllErrors helper function
{
    if (list_status & error)
    {   
        if (*first)
            *first = false;
        
        else
            printf(RED " | " RESET);
        
        PRINT_RED("%s", GetErrorString(error));
    }

    return;
}

void PrintAllErrors(ListStatus list_status)
{
    if (list_status == OKAY)
    {
        printf(GREEN "List is OKAY\n" RESET);
        return;
    }

    bool first = true;

    PrintError(list_status, LIST_NULL, &first);
    PrintError(list_status, LIST_OVERFLOW, &first);
    PrintError(list_status, LIST_UNDERFLOW, &first);
    PrintError(list_status, DATA_NULL, &first);
    PrintError(list_status, NON_POSITIVE_CAPACITY, &first);
    PrintError(list_status, LIST_LINKING_ERROR, &first);
    PrintError(list_status, LIST_FOUND_LOOP, &first);
    PrintError(list_status, INDEX_OUT_OF_RANGE, &first);
}

void SprintError(char* dest, ListStatus list_status, ListStatus error, bool* first)  // SprintAllErrors helper function
{
    if (list_status & error)
    {   
        if (*first)
            *first = false;
        
        else
            sprintf(dest, " | ");
        
        sprintf(dest, "%s", GetErrorString(error));
    }

    return;
}

void SprintAllErrors(char* dest, ListStatus list_status)
{
    if (list_status == OKAY)
    {
        sprintf(dest, "List is OKAY");
        return;
    }

    bool first = true;

    SprintError(dest, list_status, LIST_NULL, &first);
    SprintError(dest, list_status, LIST_OVERFLOW, &first);
    SprintError(dest, list_status, LIST_UNDERFLOW, &first);
    SprintError(dest, list_status, DATA_NULL, &first);
    SprintError(dest, list_status, NON_POSITIVE_CAPACITY, &first);
    SprintError(dest, list_status, LIST_LINKING_ERROR, &first);
    SprintError(dest, list_status, LIST_FOUND_LOOP, &first);
    SprintError(dest, list_status, INDEX_OUT_OF_RANGE, &first);
}

void ProcessError(ListStatus list_status)
{
    if (list_status == OKAY) return;
    if (list_status & LIST_NULL)             PRINT_RED("Error: List pointer is NULL\n");
    if (list_status & LIST_OVERFLOW)         PRINT_RED("Error: ListOverFlowError\n" );
    if (list_status & LIST_UNDERFLOW)        PRINT_RED("Error: ListUnderFlowError\n" );
    if (list_status & DATA_NULL)             PRINT_RED("Error: List data is NULL pointer!\n" );
    if (list_status & NON_POSITIVE_CAPACITY) PRINT_RED("Error: List capacity is not positive\n" );
    if (list_status & LIST_LINKING_ERROR)    PRINT_RED("Error: Some links between elems are invalid\n" );
    if (list_status & LIST_FOUND_LOOP)       PRINT_RED("Error: List contains loop\n" );
    if (list_status & INDEX_OUT_OF_RANGE)    PRINT_RED("Error: Failed attempt to use index out of range\n");
    abort();
    
    return;
}