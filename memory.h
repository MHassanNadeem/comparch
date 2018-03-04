#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "cache.h"
#include "prefetcher.h"

using namespace std;

class Memory{
    
    LRUCache *cache;
    Prefetcher *prefetcher;
    
    uint64_t numCacheHits = 0;
    uint64_t numCacheMisses = 0;
    uint64_t numAccesses = 0;
    uint64_t numHitsPrefetch = 0; /* cache hits due to prefetching */
    
public:
    Memory(LRUCache *cache, Prefetcher *prefetcher);
    ~Memory();
    
    void access(uint64_t pc, uint64_t addr);
    
    double getHitRate();
    double getMissRate();
    double getCoverage();
    double getMisFetchRate();
};

#endif /* MEMORY_H */
