#ifndef VARSTRIDEARRAYPREFETCHER_H
#define VARSTRIDEARRAYPREFETCHER_H

#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "cache.h"
#include "prefetcher.h"

struct VsEntry{
    uint64_t pc;
    uint64_t missAddr;
    uint64_t lastStride;
    uint64_t currentStride;
    // uint64_t lastAddr;
    uint64_t index;
    uint64_t* strideArray;
};

class VarStrideArrayPrefetcher : public Prefetcher{
    list<VsEntry> q;
    int num_pc;
    int num_strides;
    
public:
    VarStrideArrayPrefetcher(LRUCache *cache, int prefetchDegree, int num_pc, int num_strides);
    ~VarStrideArrayPrefetcher();

    void seedMiss(uint64_t pc, uint64_t addr);
    void printInfo();
    
};

#endif /* VARSTRIDEARRAYPREFETCHER */
