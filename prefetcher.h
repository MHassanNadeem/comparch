#ifndef PREFETCHER_H
#define PREFETCHER_H

#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "cache.h"

class Prefetcher{
public:
	string name;
    LRUCache *cache;
    int prefetchDegree;
    
public:
    Prefetcher(LRUCache *cache, int prefetchDegree);
    ~Prefetcher();

    virtual void seedMiss(uint64_t pc, uint64_t blockNumber) = 0;
    virtual void seedHit(uint64_t pc, uint64_t blockNumber);
    virtual void prefetch(uint64_t blockNumber);
    virtual void printInfo();
    
};

#endif /* PREFETCHER_H */
