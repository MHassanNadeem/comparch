#ifndef PREFETCHER_H
#define PREFETCHER_H

#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "cache.h"

class Prefetcher{
public:
	string name;
    int prefetchDegree;
    list<uint64_t> prefetchQueue; /* list of blocks to be prefetched */
    
public:
    Prefetcher(int prefetchDegree);
    ~Prefetcher();

    void addToQueue(uint64_t blockNumber);
    virtual void seedMiss(uint64_t pc, uint64_t blockNumber) = 0;
    virtual void seedHit(uint64_t pc, uint64_t blockNumber);
    virtual void printInfo();
    
};

#endif /* PREFETCHER_H */
