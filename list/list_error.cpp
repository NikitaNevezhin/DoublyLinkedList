#include "list_error.h"

const char* GetErrorString(ListStatus list_status) 
{
    switch (list_status) 
    {
        case OKAY:                  return GREEN "OKAY" RESET;

        case DATA_NULL:             return RED "DATA_NULL" RESET;

        case LIST_OVERFLOW:         return RED "LIST_OVERFLOW" RESET;

        case LIST_UNDERFLOW:        return RED "LIST_UNDERFLOW" RESET;

        case NON_POSITIVE_CAPACITY: return RED "NON_POSITIVE_CAPACITY" RESET;

        case LIST_NULL:             return RED "LIST_NULL" RESET;

        case LIST_LINKING_ERROR:    return RED "LIST_LINKING_ERROR" RESET;

        case LIST_FOUND_LOOP:       return RED "LIST_FOUND_POOP" RESET;

        case INDEX_OUT_OF_RANGE:    return RED "INDEX_OUT_OF_RANGE" RESET;

        default:                    return RED "You are seriously fucked up, man" RESET;
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
        
        printf("%s", GetErrorString(error));
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