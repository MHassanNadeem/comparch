#ifndef VARSTRIDEARRAYPREFETCHER_H
#define VARSTRIDEARRAYPREFETCHER_H

#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "cache.h"
#include "prefetcher.h"

class VarStrideArrayPrefetcher : public Prefetcher{
    uint64_t lastAddr;
    int stride;
    
public:
    VarStrideArrayPrefetcher(LRUCache *cache, int prefetchDegree);
    ~VarStrideArrayPrefetcher();

    void seedMiss(uint64_t pc, uint64_t addr);
    
};

#endif /* VARSTRIDEARRAYPREFETCHER */
