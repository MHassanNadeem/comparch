#ifndef CONSTSTRIDEARRAYPREFETCHER_H
#define CONSTSTRIDEARRAYPREFETCHER_H

#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "cache.h"
#include "prefetcher.h"

class ConstStrideArrayPrefetcher : public Prefetcher{
    uint64_t lastAddr;
    int stride;
    
public:
    ConstStrideArrayPrefetcher(LRUCache *cache, int prefetchDegree);
    ~ConstStrideArrayPrefetcher();

    void seedMiss(uint64_t pc, uint64_t addr);
    
};

#endif /* CONSTSTRIDEARRAYPREFETCHER */
