#include "linkedListPrefetcher.h"

LinkedListPrefetcher::LinkedListPrefetcher(LRUCache *cache, int prefetchDegree, size_t sizeGHB) : Prefetcher(cache, prefetchDegree){
	this->sizeGHB = sizeGHB;
}


LinkedListPrefetcher::~LinkedListPrefetcher(){

}


void LinkedListPrefetcher::seedMiss(uint64_t pc, uint64_t missBlockNumber){
	/* Youngest address is at the top/begin() of the queue */

	if(map.find(missBlockNumber) != map.end()){
		/* found */
		auto it = map[missBlockNumber];
//		printf("block found\n");
		for(int i = 0; i < prefetchDegree && it != GHBQueue.end(); i++, it++){
			prefetch(*it);
//				std::cout <<" next element::"<<*rit << std::endl;
		}
	}

	if (GHBQueue.size() == sizeGHB){
		/* remove the oldest address from map and GHB*/
		map.erase(GHBQueue.back());
		GHBQueue.pop_back();
	}

	GHBQueue.push_front(missBlockNumber);
	map[missBlockNumber] = GHBQueue.begin(); /* Keep track of the youngest address in the map */
}
