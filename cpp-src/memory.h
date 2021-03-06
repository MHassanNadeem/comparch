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
    Prefetcher *prefetcher = nullptr;
    Memory *lowerMemory = nullptr;
    bool prefetchLowerMem = false;
    
    uint64_t numCacheHits = 0;
    uint64_t numCacheMisses = 0;
    uint64_t numAccesses = 0;
    uint64_t numHitsPrefetch = 0; /* cache hits due to prefetching */
    
public:
    Memory(LRUCache *cache, Prefetcher *prefetcher);
    Memory(LRUCache *cache, Prefetcher *prefetcher, Memory *lowerMemory, bool prefetchLowerMem);
    ~Memory();
    
    void access(uint64_t pc, uint64_t byteAddr);
    
    double getHitRate();
    double getMissRate();
    double getCoverage();
    double getMisFetchRate();
    void printStats();
    void minPrintStats();
};

#endif /* MEMORY_H */
