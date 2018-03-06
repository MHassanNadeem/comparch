#include "cache.h"
#include "memory.h"
#include "prefetcher.h"
#include "constStrideArrayPrefetcher.h"
#include <stdio.h>
#include <stdint.h>

void readfile();

int main(){
    printf("==============================\n");
    printf("My Cache\n");
    printf("==============================\n");
    
    LRUCache cache(4, 1, 4);
    ConstStrideArrayPrefetcher prefetcher(&cache, 1);
    Memory memory(&cache, &prefetcher);


    return 0;
}

void cacheTestCode(){
    LRUCache ca(4, 1, 4); /* Full: 5 4 1 3 */
    // LRUCache ca(4, 1, 1); /* Direct: 4 5 2 3 */
    
    int64_t addresses[] = {1,2,3,1,4,5};

    for(int i=0; i<sizeof(addresses)/sizeof(int64_t); i++){
        ca.access(addresses[i]);
    }

    ca.display();
}