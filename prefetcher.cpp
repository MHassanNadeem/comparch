#include "prefetcher.h"

Prefetcher::Prefetcher(int prefetchDegree){
    this->prefetchDegree = prefetchDegree;
}


Prefetcher::~Prefetcher(){
    
}

void Prefetcher::addToQueue(uint64_t blockNumber){
	prefetchQueue.push_back(blockNumber);
}

/* Non abstract empty function because most prefetchers would not need to know the cache hits */
void Prefetcher::seedHit(uint64_t pc, uint64_t blockNumber){
    
}

void Prefetcher::printInfo(){
	printf("Prefetcher = %s\n", name.c_str());
}
