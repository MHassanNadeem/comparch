#include "varStrideArrayPrefetcher.h"

VarStrideArrayPrefetcher::VarStrideArrayPrefetcher(LRUCache *cache, int prefetchDegree) : Prefetcher(cache, prefetchDegree){
    
}


VarStrideArrayPrefetcher::~VarStrideArrayPrefetcher(){
    
}

void VarStrideArrayPrefetcher::seedMiss(uint64_t pc, uint64_t missAddr){
    /* todo */
    
	int curStride = missAddr - lastAddr;
	int sum = 0;

	for (i=0; i<n-1 ; i++)
		if (stride[n-1] == stride [i] && curStride == stride[i+1] ) {
		for (int a = 0; a<prefetchDegree; a++) {
			sum += stride[i + 2 + a];
			prefetch(missAddr +sum);
		}
	}

	lastAddr = missAddr;
	stride[n] = curStride;
	n++;
}
