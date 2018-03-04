#include "memory.h"

Memory::Memory(LRUCache *cache, Prefetcher *prefetcher){
    this->cache = cache;
    this->prefetcher = prefetcher;
}

Memory::~Memory(){
    
}

void Memory::access(uint64_t pc, uint64_t addr){
    numAccesses++;
    
    if(cache->isPresent(addr)){
        prefetcher->seedHit(pc, addr);
        numCacheHits++;
        if(cache->isPrefetched(addr)){
            numHitsPrefetch++;
        }
    }else{
        prefetcher->seedMiss(pc, addr);
        numCacheMisses++;
    }

    cache->access(addr);
}


double Memory::getHitRate(){
    return (double)numCacheHits/numAccesses;
}

double Memory::getMissRate(){
    return 1 - getHitRate();
}

double Memory::getCoverage(){
    uint64_t cacheMissesEliminatedByPrefetching = numHitsPrefetch;
    uint64_t totalCacheMisses = cacheMissesEliminatedByPrefetching + numCacheMisses;
    return (double) cacheMissesEliminatedByPrefetching/totalCacheMisses;
}

double Memory::getMisFetchRate(){
    return 0.0;
}