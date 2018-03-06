#include "prefetcher.h"

Prefetcher::Prefetcher(LRUCache *cache, int prefetchDegree){
    this->prefetchDegree = prefetchDegree;
    this->cache = cache;
}


Prefetcher::~Prefetcher(){
    
}

void Prefetcher::prefetch(uint64_t addr){
    cache->prefetch(addr);
}

/* Non abstract empty function because most prefetchers would not need to know the cache hits */
void Prefetcher::seedHit(uint64_t pc, uint64_t addr){
    
}