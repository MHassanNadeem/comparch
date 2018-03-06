#include "constStrideArrayPrefetcher.h"

ConstStrideArrayPrefetcher::ConstStrideArrayPrefetcher(LRUCache *cache, int prefetchDegree) : Prefetcher(cache, prefetchDegree){
    
}


ConstStrideArrayPrefetcher::~ConstStrideArrayPrefetcher(){
    
}

void ConstStrideArrayPrefetcher::seedMiss(uint64_t pc, uint64_t missAddr){
    int curStride = missAddr - lastAddr;
    
    if(curStride == stride){
        for(int i=0; i<prefetchDegree; i++){
            prefetch(missAddr + i*stride);
        }
    }

    lastAddr = missAddr;
    stride=curStride;
}