#include "cache.h"
#include <stdio.h>

int main(){
    printf("==============================\n");
    printf("My Cache\n");
    printf("==============================\n");
    
    LRUCache ca(4, 1, 4);
    
    ca.add(1);
    ca.add(2);
    ca.add(3);
    ca.add(1);
    ca.add(4);
    ca.add(5);
    ca.display();
    
    /* Full: 5 4 1 3 */
    /* Direct: 4 5 2 3 */

    return 0;
}