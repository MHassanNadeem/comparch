#ifndef LINKEDLISTPREFETCHER_H
#define LINKEDLISTPREFETCHER_H

#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "cache.h"
#include "prefetcher.h"

class LinkedListPrefetcher : public Prefetcher{
    uint64_t lastAddr;
    int stride;
    
public:
    LinkedListPrefetcher(LRUCache *cache, int prefetchDegree);
    ~LinkedListPrefetcher();

    void seedMiss(uint64_t pc, uint64_t addr);
    
};

#endif /* LINKEDLISTPREFETCHER */
