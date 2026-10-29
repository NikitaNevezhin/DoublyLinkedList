#ifndef ASSERT_LIST_OK_H

#define ASSERT_LIST_OK_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_error.h"

    
#define ASSERT_LIST_OK(list)                                                                    \
    do                                                                                          \
    {                                                                                           \
        ListStatus list_status = ListVerify(list);                                              \
        if (list_status != OKAY || list->list_status != OKAY)                                   \
        {                                                                                       \
            LIST_DUMP(list);                                                                    \
            ProcessError(list_status);                                                          \
        }                                                                                       \
    } while (0);
    
// do-while construction was used in order to use ASSERT_STACK_OK twice. 
// Before there was a problem of stack_status redefinition
// Now I've learned that inside while and for construction we have our own local variables
// Which can only be viewed inside the construction
// So now when using ASSERT_STACK_OK twice we just create two while-local variables stack_status
// And they do not redefine each other. 
// Thank you, random dude from StackOverFlow, who had the same problem as me

#endif