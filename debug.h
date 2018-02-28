#include <stdio.h>

#define DBG(var, type)          printf(#var" = %"#type"\n", var)
#define DBGM(var, type, desc)   printf(desc" = %"#type"\n", var)
#define PRINT(msg, ...)         printf(msg, ##__VA_ARGS__); printf("\n")