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

uint64_t LRUCache::getSetNumber(uint64_t blockNumber){
    return blockNumber%numSets;
}

uint64_t LRUCache::getBlockNumber(uint64_t byteAddress){
    return byteAddress/blockSize;
}

uint64_t LRUCache::getBlockAddress(uint64_t byteAddress){
    return getBlockNumber(byteAddress)*blockSize;
}

bool LRUCache::isBlockPresent(uint64_t blockNumber){
    return !(map.find(blockNumber) == map.end());
}

bool LRUCache::isBlockPrefetched(uint64_t blockNumber){
    return (*map[blockNumber]).wasPrefetched;
}

void LRUCache::touchBlock(uint64_t blockNumber, bool isPrefetched){
	uint64_t setNumber = getSetNumber(blockNumber);
    struct CacheBlock cacheBlock;

    /* if not in the cache */
    if(!isBlockPresent(blockNumber)){
    	/* TODO: Should we increment numBlocksPrefetched if block to be prefetched is already in the cache? */
    	if(isPrefetched){
    		stats.numBlocksPrefetched++;
    	}
    	/* -- Cache Miss -- */
        PRINT("Block not cached, adding %lu", blockNumber);
        cacheBlock = {.blockNumber = blockNumber, .wasPrefetched = isPrefetched};

        /* Set is full */
        if (q[setNumber].size() == associativity){
            /* EVICT: delete LRU element */
            struct CacheBlock lru = q[setNumber].back();
            PRINT("Cache full, deleting block (%lu)", lru.blockNumber);
            if(lru.wasPrefetched && !lru.used){
            	stats.numPrefetchedBlocksNotUsed++;
            }
            q[setNumber].pop_back();
            map.erase(lru.blockNumber);
        }
    }else{
    	/* -- Cache Hit -- */
        cacheBlock = *map[blockNumber];
        q[setNumber].erase(map[blockNumber]);
    }

    /* Mark cache as used if request was a demand request (i.e. not a prefetch request) */
    cacheBlock.used = cacheBlock.used || !isPrefetched;

    /* Put most recently pushed block to the front of the queue and update reference in the map */
    q[setNumber].push_front(cacheBlock);
    map[blockNumber] = q[setNumber].begin();
}


void LRUCache::prefetchBlock(uint64_t blockNumber){
    touchBlock(blockNumber, true);
}

void LRUCache::accessBlock(uint64_t blockNumber){
    touchBlock(blockNumber, false);
}


void LRUCache::display(){
    for(uint64_t iSet = 0; iSet<numSets; iSet++){
        printf("Set %lu: ", iSet);
        for (auto it = q[iSet].begin(); it != q[iSet].end(); it++){
            printf("%lu ", (*it).blockNumber);
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
