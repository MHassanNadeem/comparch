#include "cache.h"


LRUCache::LRUCache(uint64_t size, uint64_t blockSize, uint64_t associativity){
    assert(size >= blockSize);
    assert(size >= associativity);
    assert(associativity <= size/blockSize);

    this->size = size;
    this->blockSize = blockSize;
    this->associativity = associativity;
    this->numBlocks = size/blockSize;
    this->numSets = numBlocks/associativity;
    
    DBG(size, lu);
    DBG(blockSize, lu);
    DBG(associativity, lu);
    DBG(numBlocks, lu);
    DBG(numSets, lu);
    
    q = new list<CacheBlock>[numSets];
}

LRUCache::~LRUCache(){
    delete [] q;
}

uint64_t LRUCache::getSetNumber(uint64_t addr){
    return getBlockNumber(addr)%numSets;
}

uint64_t LRUCache::getBlockNumber(uint64_t addr){
    return addr/blockSize;
}

uint64_t LRUCache::getBlockAddress(uint64_t addr){
    return getBlockNumber(addr)*blockSize;
}

bool LRUCache::isPresent(uint64_t blockNumber){
    return !(map.find(blockNumber) == map.end());
}

bool LRUCache::isPrefetched(uint64_t x){
    uint64_t blockNumber = getBlockNumber(x);
    
    return (*map[blockNumber]).wasPrefetched;
    
}


void LRUCache::add(uint64_t addr, bool isPrefetched){
    uint64_t setNumber = getSetNumber(addr);
    uint64_t blockNumber = getBlockNumber(addr);

    struct CacheBlock cacheBlock;
    
    /* if not in the cache */
    if(!isPresent(blockNumber)){
        PRINT("Block not cached, adding %lu(%lu)", addr, blockNumber);
        cacheBlock = {.blockNumber = blockNumber, .addr = addr, .wasPrefetched = isPrefetched};
        
        // set is full
        if (q[setNumber].size() == associativity){
            // delete LRU element
            struct CacheBlock last = q[setNumber].back();
            PRINT("Cache full, deleting block (%lu)", last.blockNumber);
            q[setNumber].pop_back();
            map.erase(last.addr);
        }
    }else{
        assert(isPrefetched == false);
        cacheBlock = *map[blockNumber]; cacheBlock.addr = addr;
        q[setNumber].erase(map[blockNumber]);
    }
    
    // update reference
    q[setNumber].push_front(cacheBlock);
    map[blockNumber] = q[setNumber].begin();
}

void LRUCache::prefetch(uint64_t x){
    add(x, true);
}

void LRUCache::access(uint64_t x){
    add(x, false);
}
 
// display contents of cache
void LRUCache::display(){
    for(uint64_t iSet = 0; iSet<numSets; iSet++){
        printf("Set %lu: ", iSet);
        for (auto it = q[iSet].begin(); it != q[iSet].end(); it++){
            printf("%lu(%lu) ", (*it).addr, (*it).blockNumber);
        }
        printf("\n");
    }
 
    cout << endl;
}

void LRUCache::getAllCachedBlocks(vector<CacheBlock> &v){
    for(uint64_t iSet = 0; iSet<numSets; iSet++){
        for (auto it = q[iSet].begin(); it != q[iSet].end(); it++){
            v.push_back(*it);
        }
    }
}
