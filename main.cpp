#include "cache.h"
#include "memory.h"
#include <stdio.h>
#include <stdint.h>

void readfile();

int main(){
    printf("==============================\n");
    printf("My Cache\n");
    printf("==============================\n");
    
    LRUCache ca(4, 1, 4); /* Full: 5 4 1 3 */
    // LRUCache ca(4, 1, 1); /* Direct: 4 5 2 3 */
    
    ca.access(1);
    ca.access(2);
    ca.access(3);
    ca.access(1);
    ca.access(4);
    ca.access(5);
    ca.display();

    return 0;
}