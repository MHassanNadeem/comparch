#include "linkedListPrefetcher.h"

LinkedListPrefetcher::LinkedListPrefetcher(LRUCache *cache, int prefetchDegree, size_t sizeGHB) : Prefetcher(cache, prefetchDegree){
	this->sizeGHB = sizeGHB;
}


LinkedListPrefetcher::~LinkedListPrefetcher(){

}

void LinkedListPrefetcher::seedMiss(uint64_t pc, uint64_t missBlockNumber){
	for (auto rit = GHBQueue.crbegin(); rit != GHBQueue.crend(); ++rit){
		if(*rit == missBlockNumber){
			/* found */
			printf("block found\n");
			for(int i = 0; i < prefetchDegree && rit != GHBQueue.crend(); i++,rit--){
				prefetch(*rit);
//				std::cout <<" next element::"<<*rit << std::endl;
			}

			break;
		}
	}

	if (GHBQueue.size() == sizeGHB){
		GHBQueue.pop_front();
	}

	GHBQueue.push_back (missBlockNumber);
}
