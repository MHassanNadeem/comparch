#include "cache.h"

LRUCache::LRUCache(int64_t size, int64_t blockSize, int64_t associativity){
    this->size = size;
    this->blockSize = blockSize;
    this->associativity = associativity;
    this->numBlocks = size/blockSize;
    this->numSets = numBlocks/associativity;
    
    DBG(size, d);
    DBG(blockSize, d);
    DBG(associativity, d);
    DBG(numBlocks, d);
    DBG(numSets, d);
    
    q = new list<int64_t>[numSets];
}

LRUCache::~LRUCache(){
    delete [] q;
}

int64_t LRUCache::getSetNumber(int64_t x){
    return x%(numSets*blockSize);
}

int64_t LRUCache::getBlockNumber(int64_t x){
    return x%numBlocks;
}

int64_t LRUCache::getBlockAddress(int64_t x){
    return getBlockNumber(x)*blockSize;
}

bool LRUCache::isPresent(int64_t x){
    return !(map.find(x) == map.end());
}

/* Refers key x with in the LRU cache */
void LRUCache::add(int64_t x){
    int64_t setNumber = getSetNumber(x);
    int64_t blockNumber = getBlockNumber(x);

    if(!isPresent(blockNumber)){
        // set is full
        if (q[setNumber].size() == associativity){
            //delete LRU element
            int64_t last = q[setNumber].back();
            q[setNumber].pop_back();
            map.erase(last);
        }
    }else{
        q[setNumber].erase(map[blockNumber]);
    }
    
    // update reference
    q[setNumber].push_front(blockNumber);
    map[blockNumber] = q[setNumber].begin();
}
 
// display contents of cache
void LRUCache::display(){
    for(int64_t iSet = 0; iSet<numSets; iSet++){
        printf("Set %d: ", iSet);
        for (auto it = q[iSet].begin(); it != q[iSet].end(); it++){
            cout << (*it) << " ";
        }
        printf("\n");
    }
 
    cout << endl;
}