#ifndef PREFETCHER_H
#define PREFETCHER_H

#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "cache.h"

class Prefetcher{
    LRUCache *cache;
    int prefetchDegree;

    uint64_t lastAddr;
    int stride;
    
public:
    Prefetcher(LRUCache *cache, int prefetchDegree);
    ~Prefetcher();

    void seedMiss(uint64_t pc, uint64_t addr);
    void seedHit(uint64_t pc, uint64_t addr);
    void prefetch(uint64_t addr);
    
};

#endif /* PREFETCHER_H */
