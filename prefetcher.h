#ifndef PREFETCHER_H
#define PREFETCHER_H

#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "cache.h"

class Prefetcher{
public:
    LRUCache *cache;
    int prefetchDegree;
    
public:
    Prefetcher(LRUCache *cache, int prefetchDegree);
    ~Prefetcher();

    virtual void seedMiss(uint64_t pc, uint64_t addr) = 0;
    virtual void seedHit(uint64_t pc, uint64_t addr);
    virtual void prefetch(uint64_t addr);
    
};

#endif /* PREFETCHER_H */
