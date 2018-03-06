#include "varStrideArrayPrefetcher.h"

VarStrideArrayPrefetcher::VarStrideArrayPrefetcher(LRUCache *cache, int prefetchDegree) : Prefetcher(cache, prefetchDegree){
    
}


VarStrideArrayPrefetcher::~VarStrideArrayPrefetcher(){
    
}

void VarStrideArrayPrefetcher::seedMiss(uint64_t pc, uint64_t missAddr){
    /* todo */
}