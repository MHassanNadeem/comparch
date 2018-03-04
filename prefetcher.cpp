#include "prefetcher.h"

/* Constant Stride Prefetching for arrays */

Prefetcher::Prefetcher(LRUCache *cache, int prefetchDegree){
    this->prefetchDegree = prefetchDegree;
    this->cache = cache;
}


Prefetcher::~Prefetcher(){

}

void Prefetcher::seedMiss(uint64_t pc, uint64_t missAddr){
    int curStride = missAddr - lastAddr;
    
    if(curStride == stride){
        for(int i=0; i<prefetchDegree; i++){
            prefetch(missAddr + i*stride);
        }
    }

    lastAddr = missAddr;
    stride=curStride;
}


void Prefetcher::seedHit(uint64_t pc, uint64_t addr){

    lastAddr = addr;
}

void Prefetcher::prefetch(uint64_t addr){
    cache->prefetch(addr);
}
