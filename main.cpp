#include "fileParser.h"
#include "cache.h"
#include "memory.h"
#include "prefetcher.h"
#include "constStrideArrayPrefetcher.h"
#include "varStrideArrayPrefetcher.h"
#include "linkedListPrefetcher.h"
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <utility>

#define NUM_BYTES_IN_MB 		1048576LU

#define L2_CACHE_SIZE			2*NUM_BYTES_IN_MB
#define L2_CACHE_BLOCK_SIZE		64
#define L2_CACHE_ASSOCIATIVITY 	8

#define RAM_CACHE_SIZE			L2_CACHE_BLOCK_SIZE*64*1024LU
#define RAM_CACHE_BLOCK_SIZE	L2_CACHE_BLOCK_SIZE*64
#define RAM_CACHE_ASSOCIATIVITY	1

#define PREFETCHER_NUM_PC		256
#define PREFETCHER_NUM_STRIDES	256
#define PREFETCHER_GHB_SIZE		(L2_CACHE_SIZE/L2_CACHE_BLOCK_SIZE)*100LU

void arrayMicroBenchmark();
void linkedListMicroBenchmark();

int main(){
	time_t t;
	srand((unsigned) time(&t));
    printf("===============================\n");
    printf("|          Project 5          |\n");
    printf("===============================\n");
    
    LRUCache ramCache(RAM_CACHE_SIZE, RAM_CACHE_BLOCK_SIZE, RAM_CACHE_ASSOCIATIVITY);
    Memory ram(&ramCache, nullptr);

    LRUCache l2Cache(L2_CACHE_SIZE, L2_CACHE_BLOCK_SIZE, L2_CACHE_ASSOCIATIVITY);
//    ConstStrideArrayPrefetcher csPrefetcher(&l2Cache, 10, PREFETCHER_NUM_PC);
    LinkedListPrefetcher llPrefetcher(&l2Cache, 20, PREFETCHER_GHB_SIZE);
    Memory memory(&l2Cache, &llPrefetcher, &ram);


    uint64_t pc, addr;
    FileParser file1("526_blender_r_pinatrace.out");

//    while( file1.getNext(pc, addr) ){
////        printf("0x%lx - 0x%lx\n", pc, addr);
//    	memory.access(pc, addr);
//    }
//
//    memory.printStats();

//    arrayMicroBenchmark();
    linkedListMicroBenchmark();
//
    return 0;
}

void runTraceConstStride(string fileName, int ){

}

void cacheTestCode(){
    LRUCache ca(4, 1, 4); /* Full: 5 4 1 3 */
    // LRUCache ca(4, 1, 1); /* Direct: 4 5 2 3 */
    
    int64_t addresses[] = {1,2,3,1,4,5};

    for(int i=0; i<sizeof(addresses)/sizeof(int64_t); i++){
        ca.accessBlock(addresses[i]);
    }

    ca.display();
}


void arrayMicoBenchmarkRun(vector<pair<uint64_t, uint64_t>> &accessPattern){
	LRUCache cache(L2_CACHE_SIZE, L2_CACHE_BLOCK_SIZE, L2_CACHE_ASSOCIATIVITY);
	ConstStrideArrayPrefetcher csPrefetcher(&cache, 10, PREFETCHER_NUM_PC);
	Memory memory(&cache, &csPrefetcher);

	LRUCache cache2(2*NUM_BYTES_IN_MB, 64, 8);
	Memory memory2(&cache2, nullptr);

	for(int i=0; i<accessPattern.size(); i++){
		memory.access(accessPattern[i].first, accessPattern[i].second);
		memory2.access(accessPattern[i].first, accessPattern[i].second);
	}

	memory2.printStats();

	memory.printStats();
}

void arrayMicroBenchmark(){
	vector<pair<uint64_t, uint64_t>> accessPattern;

	struct TestData{
		uint64_t pc;
		uint64_t startAddress;
		uint64_t arraySize;
		int64_t  stride;
	};

	struct TestData testData[4] = {
		{1, 1000000000, 100000, 30},
		{2, 2000000000, 100000, 40},
		{3, 3000000000, 100000, 50},
		{4, 4000000000, 100000, 70}
	};


	for(int i=0; i<4; i++){
		for(int j=0; j<testData[i].arraySize; j++){
			accessPattern.push_back( make_pair(testData[i].pc, testData[i].startAddress + j*testData[i].stride) );
		}
	}

	arrayMicoBenchmarkRun( accessPattern );
}

void linkedListMicoBenchmarkRun(vector<pair<uint64_t, uint64_t>> &accessPattern){
	LRUCache cache1(2*NUM_BYTES_IN_MB, 64, 8);
	LinkedListPrefetcher llPrefetcher(&cache1, 30, PREFETCHER_GHB_SIZE);
	Memory memoryWithPrefetcher(&cache1, &llPrefetcher);

	LRUCache cache2(2*NUM_BYTES_IN_MB, 64, 8);
	Memory memoryNoPrefetcher(&cache2, nullptr);

	for(int i=0; i<accessPattern.size(); i++){
		memoryWithPrefetcher.access(accessPattern[i].first, accessPattern[i].second);
		memoryNoPrefetcher.access(accessPattern[i].first, accessPattern[i].second);
	}

	printf("Linked List Prefetcher Enabled: \n");
	memoryWithPrefetcher.printStats();

	printf("Linked List Prefetcher Disabled: \n");
	memoryNoPrefetcher.printStats();
}

void linkedListMicroBenchmark(){
	const int LINKED_LIST_SIZE = 10000;
	const int NODE_SIZE = 100;
	void* addressess[LINKED_LIST_SIZE];
	set<uint64_t> addresses;

	vector<pair<uint64_t, uint64_t>> accessPattern;

	/* Generate unique random addresses */
	for(int i=0; i<LINKED_LIST_SIZE; i++){
		addresses.insert( rand() );
	}

	/* populate accessPattern array with pc and addr */
		for(auto it=addresses.begin(); it!=addresses.end(); ++it){
			accessPattern.push_back( make_pair(0, *it) );
//			cout<<"++++ "<<(*it)/L2_CACHE_BLOCK_SIZE<<endl;
		}

		/* Add noise so that some of the cached linked list is evicted */
		for(uint64_t i=(long)RAND_MAX+5,j=0; i<(UINT64_MAX-L2_CACHE_BLOCK_SIZE) && j<L2_CACHE_SIZE/L2_CACHE_BLOCK_SIZE; i+=L2_CACHE_BLOCK_SIZE,j++){
			accessPattern.push_back( make_pair(0, i) );
		}

		for(auto it=addresses.begin(); it!=addresses.end(); ++it){
			accessPattern.push_back( make_pair(0, *it) );
		}

	linkedListMicoBenchmarkRun(accessPattern);
}
