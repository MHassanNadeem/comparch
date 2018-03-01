#include "cache.h"

LRUCache::LRUCache(int size, int blockSize, int associativity){
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
    
    q = new list<int>[numSets];
}

LRUCache::~LRUCache(){
    delete [] q;
}

int LRUCache::getSetNumber(int x){
    return x%(numSets*blockSize);
}

int LRUCache::getBlockNumber(int x){
    return x%numBlocks;
}

int64_t LRUCache::getBlockAddress(int64_t x){
    return getBlockNumber(x)*blockSize;
}

bool LRUCache::isPresent(int x){
    return !(map.find(x) == map.end());
}

/* Refers key x with in the LRU cache */
void LRUCache::add(int x){
    int setNumber = getSetNumber(x);
    int blockNumber = getBlockNumber(x);

    if(!isPresent(blockNumber)){
        // set is full
        if (q[setNumber].size() == associativity){
            //delete LRU element
            int last = q[setNumber].back();
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
    for(int iSet = 0; iSet<numSets; iSet++){
        printf("Set %d: ", iSet);
        for (auto it = q[iSet].begin(); it != q[iSet].end(); it++){
            cout << (*it) << " ";
        }
        printf("\n");
    }
 
    cout << endl;
}