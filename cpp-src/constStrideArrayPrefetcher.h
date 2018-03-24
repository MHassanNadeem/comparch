#ifndef CONSTSTRIDEARRAYPREFETCHER_H
#define CONSTSTRIDEARRAYPREFETCHER_H

#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "cache.h"
#include "prefetcher.h"

struct CsEntry{
    uint64_t pc;
    uint64_t lastAddr;
    int64_t  stride;
};

class ConstStrideArrayPrefetcher : public Prefetcher{
    list<CsEntry> q;
    int num_pc;
    
public:
    ConstStrideArrayPrefetcher(int prefetchDegree, int num_pc);
    ~ConstStrideArrayPrefetcher();

    void seedMiss(uint64_t pc, uint64_t blockNumber);
    void printInfo();
    
};

#endif /* CONSTSTRIDEARRAYPREFETCHER */
