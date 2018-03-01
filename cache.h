#ifndef CACHE_H
#define CACHE_H

#include <unordered_map>
#include <bits/stdc++.h>
#include <stdint.h>
#include "debug.h"

using namespace std;

class LRUCache{
    list<int64_t>* q;
    unordered_map<int64_t, list<int64_t>::iterator> map;
    
    int64_t size; // Capacity of cache in num of blocks
    
    /* Number of blocks per spot = size of q */
    int64_t associativity; /* 1 = directly mapped, size = fully associative */
    int64_t numSets; /* number of q = number of sets */
    int64_t blockSize; /* size in bytes */
    int64_t numBlocks;
    
public:
    LRUCache(int64_t, int64_t, int64_t);
    ~LRUCache();
    
    void add(int64_t);
    bool isPresent(int64_t);
    void display();
    
private:
    int64_t getSetNumber(int64_t);
    int64_t getBlockNumber(int64_t);
    int64_t getBlockAddress(int64_t);
};

#endif /* CACHE_H */
