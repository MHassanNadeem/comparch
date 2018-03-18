#ifndef LINKEDLISTPREFETCHER_H
#define LINKEDLISTPREFETCHER_H

#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "cache.h"
#include "prefetcher.h"

class LinkedListPrefetcher : public Prefetcher{
	list<uint64_t> GHBQueue;
    size_t sizeGHB;
    
public:
    LinkedListPrefetcher(LRUCache *cache, int prefetchDegree, size_t sizeGHB);
    ~LinkedListPrefetcher();

    void seedMiss(uint64_t pc, uint64_t addr);
};

#endif /* LINKEDLISTPREFETCHER */
