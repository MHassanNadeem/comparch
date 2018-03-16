#ifndef CACHE_H
#define CACHE_H

#include <unordered_map>
#include <bits/stdc++.h>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include "debug.h"

using namespace std;

struct CacheBlock{
    uint64_t blockNumber;
    uint64_t addr; /* metadata, the byte address that was last accessed */
    bool wasPrefetched;
};

class LRUCache{
    list<CacheBlock>* q;
    unordered_map<uint64_t, list<CacheBlock>::iterator> map; /* key = block address, value = cache block */
    
    uint64_t size; /* Capacity of cache in num of blocks */
    
    uint64_t associativity; /* 1 = directly mapped, size/blockSize = fully associative */
    uint64_t numSets; /* number of q = number of sets */
    uint64_t blockSize; /* size in bytes */
    uint64_t numBlocks;
    
public:
    LRUCache(uint64_t, uint64_t, uint64_t);
    ~LRUCache();
    
    void add(uint64_t, bool);
    void prefetch(uint64_t);
    void access(uint64_t);
    bool isPresent(uint64_t);
    bool isPrefetched(uint64_t);
    void display();
    void getAllCachedBlocks(vector<CacheBlock> &v);
    
private:
    uint64_t getSetNumber(uint64_t);
    uint64_t getBlockNumber(uint64_t);
    uint64_t getBlockAddress(uint64_t);
};

#endif /* CACHE_H */
