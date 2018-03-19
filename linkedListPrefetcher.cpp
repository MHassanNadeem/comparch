#include "linkedListPrefetcher.h"

LinkedListPrefetcher::LinkedListPrefetcher(LRUCache *cache, int prefetchDegree, size_t sizeGHB) : Prefetcher(cache, prefetchDegree){
	this->sizeGHB = sizeGHB;
	this->name = "Linked List Prefetcher";
}


LinkedListPrefetcher::~LinkedListPrefetcher(){

}


void LinkedListPrefetcher::seedMiss(uint64_t pc, uint64_t missBlockNumber){
	/* Youngest address is at the bottom/end() of the queue */

	if(map.find(missBlockNumber) != map.end()){
		/* found */
		auto it = map[missBlockNumber];
		cout<<"--block found "<< missBlockNumber<<endl;
		it++; /* start prefetching from the next block */
		for(int i = 0; i < prefetchDegree && it != GHBQueue.end(); i++, it++){
			prefetch(*it);
			cout<<"Prefetching "<<*it<<endl;
		}
	}

	if (GHBQueue.size() == sizeGHB){
		/* remove the oldest address from map and GHB*/
		map.erase(GHBQueue.front());
		GHBQueue.pop_front();
	}

	GHBQueue.push_back(missBlockNumber);
	map[missBlockNumber] = --GHBQueue.end(); /* Keep track of the youngest address in the map */
}

void LinkedListPrefetcher::printInfo(){
	printf("Prefetcher = %s\n", name.c_str());
	printf("Prefetch Degree = %d\n", prefetchDegree);
	printf("GHB size = %lu\n", sizeGHB);
}
