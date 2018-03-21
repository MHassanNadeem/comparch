#include "constStrideArrayPrefetcher.h"

ConstStrideArrayPrefetcher::ConstStrideArrayPrefetcher(int prefetchDegree, int num_pc) : Prefetcher(prefetchDegree){
    this->num_pc = num_pc;
    this->name = "Constant Stride Array Prefetcher";

    /* Initialize the data structure with full capacity,
       this is to make other functions simpler */
    struct CsEntry csEntry = {0,0,0};
    for(int i=0; i<num_pc; i++){
        q.push_front(csEntry);
    }
}


ConstStrideArrayPrefetcher::~ConstStrideArrayPrefetcher(){
    
}

void ConstStrideArrayPrefetcher::seedMiss(uint64_t pc, uint64_t missBlockNumber){
    struct CsEntry csEntry;

    /* This iterator will refer to the required struct that contains our pc */
    list<CsEntry>::iterator itcs = q.begin(); /* Initilze to LRU element, which will be updated by default if pc not found */

    /* Find the iterator to the struct in the q that matches our pc  */
    for (auto it = q.begin(); it != q.end(); it++){
        if( (*it).pc == pc){
            itcs = it;
            break;
        }
    }

    /* copy and delete the node to be updated so that it can be added back at the end of the queue to mantain MRU element at the end */
    csEntry = (*itcs);
    q.erase(itcs);

    /* Algorithm copied from the slides */
    csEntry.pc = pc;
    int64_t curStride = missBlockNumber - csEntry.lastAddr;
    
    if(curStride == csEntry.stride){
        for(int i=1; i<=prefetchDegree; i++){
            addToQueue(missBlockNumber + i*csEntry.stride);
        }
    }

    csEntry.lastAddr = missBlockNumber;
    csEntry.stride=curStride;
    /* Algorithm end */

    q.push_back(csEntry);
}

void ConstStrideArrayPrefetcher::printInfo(){
	printf("Prefetcher = %s\n", name.c_str());
	printf("Prefetch Degree = %d\n", prefetchDegree);
	printf("Num PC = %d\n", num_pc);
}
