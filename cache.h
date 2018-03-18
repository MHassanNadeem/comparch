#ifndef CACHE_H
#define CACHE_H

#include <unordered_map>
#include <bits/stdc++.h>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include "debug.h"

using namespace std;

struct CacheStats{
	uint64_t numBlocksPrefetched;
	uint64_t numPrefetchedBlocksNotUsed;
};

struct CacheBlock{
    uint64_t blockNumber;
    bool wasPrefetched;
    bool used;
};

class LRUCache{
    list<CacheBlock>* q;
    unordered_map<uint64_t, list<CacheBlock>::iterator> map; /* key = block number, value = cache block */
    
    uint64_t size; /* Capacity of cache in num of blocks */
    
    uint64_t associativity; /* 1 = directly mapped, size/blockSize = fully associative */
    uint64_t numSets; /* number of q = number of sets */
    uint64_t blockSize; /* size in bytes */
    uint64_t numBlocks;
    
    CacheStats stats = {0};


public:
    LRUCache(uint64_t, uint64_t, uint64_t);
    ~LRUCache();
    
    void prefetchBlock(uint64_t blockNumber);
    void accessBlock(uint64_t blockNumber);
    bool isBlockPresent(uint64_t);
    bool isBlockPrefetched(uint64_t);
    void display();
    void getAllCachedBlocks(vector<CacheBlock> &v);
    uint64_t getBlockNumber(uint64_t byteAddress);
    uint64_t getBlockAddress(uint64_t byteAddress);
    CacheStats getStats(){return stats;}
    
private:
    void touchBlock(uint64_t blockNumber, bool isPrefetched);
    uint64_t getSetNumber(uint64_t blockNumber);
};

#endif /* CACHE_H */
