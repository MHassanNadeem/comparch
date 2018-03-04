#include <stdio.h>

#ifndef DEBUG_ON
    #define DEBUG_ON 1
#endif

#if DEBUG_ON == 1
    #define DBG(var, type)          printf(#var" = %"#type"\n", var)
    #define DBGM(var, type, desc)   printf(desc" = %"#type"\n", var)
    #define PRINT(msg, ...)         printf(msg, ##__VA_ARGS__); printf("\n")
#else
    #define DBG(var, type)
    #define DBGM(var, type, desc)
    #define PRINT(msg, ...)
#endif