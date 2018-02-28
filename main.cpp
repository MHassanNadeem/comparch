#include "cache.h"
#include <stdio.h>

int main(){
    printf("==============================\n");
    printf("My Cache\n");
    printf("==============================\n");
    
    LRUCache ca(4);

    ca.refer(1);
    ca.refer(2);
    ca.refer(3);
    ca.refer(1);
    ca.refer(4);
    ca.refer(5);
    ca.display();

    return 0;
}