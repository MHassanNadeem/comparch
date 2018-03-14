#include "linkedListPrefetcher.h"

LinkedListPrefetcher::LinkedListPrefetcher(LRUCache *cache, int prefetchDegree) : Prefetcher(cache, prefetchDegree){
    
}


LinkedListPrefetcher::~LinkedListPrefetcher(){
    
}

void LinkedListPrefetcher::seedMiss(uint64_t pc, uint64_t missAddr){
    /* todo */
    

}
