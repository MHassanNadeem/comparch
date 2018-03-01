#include <unordered_map>
#include <bits/stdc++.h>
#include <stdint.h>
#include "debug.h"

using namespace std;

class LRUCache{
    list<int>* q;
    unordered_map<int, list<int>::iterator> map;
    
    int size; // Capacity of cache in num of blocks
    
    /* Number of blocks per spot = size of q */
    int associativity; /* 1 = directly mapped, size = fully associative */
    int numSets; /* number of q = number of sets */
    int blockSize; /* size in bytes */
    int numBlocks;
    
public:
    LRUCache(int, int, int);
    ~LRUCache();
    
    void add(int);
    bool isPresent(int);
    void display();
    
private:
    int getSetNumber(int);
    int getBlockNumber(int);
    int64_t getBlockAddress(int64_t);
};