#include "memory.h"

Memory::Memory(LRUCache *cache, Prefetcher *prefetcher){
    this->cache = cache;
    this->prefetcher = prefetcher;
}

Memory::~Memory(){
    
}

void Memory::access(uint64_t pc, uint64_t byteAddr){
	uint64_t blockNumber = cache->getBlockNumber(byteAddr);

    numAccesses++;
    
    if(cache->isBlockPresent(blockNumber)){
    	numCacheHits++;
        if(cache->isBlockPrefetched(blockNumber)){
            numHitsPrefetch++;
        }
        if(prefetcher) prefetcher->seedHit(pc, blockNumber);
    }else{
    	numCacheMisses++;
        if(prefetcher) prefetcher->seedMiss(pc, blockNumber);
    }

    cache->accessBlock(blockNumber);
}


double Memory::getHitRate(){
    return (double)numCacheHits/numAccesses;
}

double Memory::getMissRate(){
    return 1 - getHitRate();
}

double Memory::getCoverage(){
    uint64_t cacheMissesEliminatedByPrefetching = numHitsPrefetch;
    uint64_t totalCacheMisses = cacheMissesEliminatedByPrefetching + numCacheMisses;
    return (double) cacheMissesEliminatedByPrefetching/totalCacheMisses;
}

double Memory::getMisFetchRate(){
	uint64_t numPrefetchedBlocksNotUsed = cache->getStats().numPrefetchedBlocksNotUsed;
	uint64_t numBlocksPrefetched = cache->getStats().numBlocksPrefetched;
	DBG(numPrefetchedBlocksNotUsed, lu);
	DBG(numBlocksPrefetched, lu);
    return (double) numPrefetchedBlocksNotUsed/numBlocksPrefetched;
}


void Memory::printStats(){
	printf("\n=========================\n");

	printf("--------- CACHE ---------\n");
	cache->printInfo();

	printf("------- PREFETCHER-------\n");
	if(prefetcher == nullptr){
		printf("NULL\n");
	}else{
		prefetcher->printInfo();
	}

	printf("----- MEMORY STATS -----\n");
	printf("Hits = %lu\n", numCacheHits);
	printf("Misses = %lu\n", numCacheMisses);
	printf("Hit Rate = %f\n", getHitRate());
	printf("Miss Rate = %f\n", getMissRate());
	printf("Coverage = %f\n", getCoverage());
	printf("Misfetch Rate = %f\n", getMisFetchRate());
	printf("------------------------\n");
}
