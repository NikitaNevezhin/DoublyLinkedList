#ifndef ColorPrint_h

#define ColorPrint_h

#include <stdio.h>

#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"
#define INVERSION   "\033[7m"

#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define PURPLE      "\033[35m"
#define LIGHT_BLUE  "\033[36m"
#define WHITE       "\033[37m"

#define RESET  "\033[0m"

#define PRINT_BLACK(format, ...)            printf(BLACK       format RESET, ##__VA_ARGS__)
#define PRINT_RED(format, ...)              printf(RED         format RESET, ##__VA_ARGS__)
#define PRINT_GREEN(format, ...)            printf(GREEN       format RESET, ##__VA_ARGS__)
#define PRINT_YELLOW(format, ...)           printf(YELLOW      format RESET, ##__VA_ARGS__)
#define PRINT_BLUE(format, ...)             printf(BLUE        format RESET, ##__VA_ARGS__)
#define PRINT_PURPLE(format, ...)           printf(PURPLE      format RESET, ##__VA_ARGS__)
#define PRINT_LIGHT_BLUE(format, ...)       printf(LIGHT_BLUE  format RESET, ##__VA_ARGS__)
#define PRINT_WHITE(format, ...)            printf(WHITE       format RESET, ##__VA_ARGS__)

// ##__VA_ARGS__ gets away the comma in case __VA_ARGS__ is empty and everything works properly

#endif