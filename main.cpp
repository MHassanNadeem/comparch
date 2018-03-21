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
//    ConstStrideArrayPrefetcher csPrefetcher(10, PREFETCHER_NUM_PC);
    LinkedListPrefetcher llPrefetcher(20, PREFETCHER_GHB_SIZE);
    Memory memory(&l2Cache, &llPrefetcher, &ram, false);


    string fileName = "/media/hassan/7A5452395451F7F9/BenchMarks/pinatrace_mcf_605.out";
//    runBenchmark(fileName, &memory);


//    arrayMicroBenchmark();
    linkedListMicroBenchmark();
//
    return 0;
}

void runBenchmark(string fileName, Memory *memory){
	uint64_t pc, addr;

	FileParser file(fileName);

    while( file.getNext(pc, addr) ){
    	memory->access(pc, addr);
    }
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


/* Without prefetcher */
void arrayMicoBenchmarkRun_np(vector<pair<uint64_t, uint64_t>> &accessPattern){
	LRUCache cache(L2_CACHE_SIZE, L2_CACHE_BLOCK_SIZE, L2_CACHE_ASSOCIATIVITY);
	Memory memory(&cache, nullptr);

	for(int i=0; i<accessPattern.size(); i++){
		memory.access(accessPattern[i].first, accessPattern[i].second);
	}

	printf("Const Stride Array MicroBenchmark without prefetching\n");
	memory.printStats();
}

/* Block Prefetcher */
void arrayMicoBenchmarkRun_bp(vector<pair<uint64_t, uint64_t>> &accessPattern, int prefetchDegree){
	LRUCache cache(L2_CACHE_SIZE, L2_CACHE_BLOCK_SIZE, L2_CACHE_ASSOCIATIVITY);
	ConstStrideArrayPrefetcher csPrefetcher(prefetchDegree, PREFETCHER_NUM_PC);
	Memory memory(&cache, &csPrefetcher);

	for(int i=0; i<accessPattern.size(); i++){
		memory.access(accessPattern[i].first, accessPattern[i].second);
	}

	printf("Const Stride Array MicroBenchmark with Block PD: %d\n", prefetchDegree);
	memory.printStats();
}

/* Page Prefetcher */
void arrayMicoBenchmarkRun_pp(vector<pair<uint64_t, uint64_t>> &accessPattern, int prefetchDegree){
    LRUCache ramCache(RAM_CACHE_SIZE, RAM_CACHE_BLOCK_SIZE, RAM_CACHE_ASSOCIATIVITY);
    Memory ram(&ramCache, nullptr);

	LRUCache cache(L2_CACHE_SIZE, L2_CACHE_BLOCK_SIZE, L2_CACHE_ASSOCIATIVITY);
	ConstStrideArrayPrefetcher csPrefetcher(prefetchDegree, PREFETCHER_NUM_PC);
    Memory memory(&cache, &csPrefetcher, &ram, true);

	for(int i=0; i<accessPattern.size(); i++){
		memory.access(accessPattern[i].first, accessPattern[i].second);
	}

	printf("Const Stride Array MicroBenchmark with Page PD: %d\n", prefetchDegree);
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

	struct TestData testData[6] = {
		{1, 1000000000, 100000, 32},
		{2, 2000000000, 100000, 40},
		{3, 3000000000, 100000, 48},
		{4, 4000000000, 100000, 80},
		{5, 5000000000, 100000, 88},
		{6, 6000000000, 100000, 104}
	};


	for(int i=0; i<6; i++){
		for(int j=0; j<testData[i].arraySize; j++){
			accessPattern.push_back( make_pair(testData[i].pc, testData[i].startAddress + j*testData[i].stride) );
		}
	}


//	arrayMicoBenchmarkRun_np(accessPattern);
	for(int pd = 10; pd<=100; pd+=10){
//		arrayMicoBenchmarkRun_bp(accessPattern, pd);
		arrayMicoBenchmarkRun_pp(accessPattern, pd);
		printf("\n");
	}
}

void linkedListMicoBenchmarkRun_np(vector<pair<uint64_t, uint64_t>> &accessPattern){
	LRUCache cache(L2_CACHE_SIZE, L2_CACHE_BLOCK_SIZE, L2_CACHE_ASSOCIATIVITY);
	Memory memory(&cache, nullptr);

	for(int i=0; i<accessPattern.size(); i++){
		memory.access(accessPattern[i].first, accessPattern[i].second);
	}

	printf("Linked List MicroBenchmark No Prefetcher\n");
	memory.printStats();

}


void linkedListMicoBenchmarkRun_bp(vector<pair<uint64_t, uint64_t>> &accessPattern, int prefetchDegree){
	LRUCache cache(L2_CACHE_SIZE, L2_CACHE_BLOCK_SIZE, L2_CACHE_ASSOCIATIVITY);
	LinkedListPrefetcher llPrefetcher(prefetchDegree, PREFETCHER_GHB_SIZE);
    Memory memory(&cache, &llPrefetcher);

	for(int i=0; i<accessPattern.size(); i++){
		memory.access(accessPattern[i].first, accessPattern[i].second);
	}

	printf("Linked List MicroBenchmark with Block PD: %d\n", prefetchDegree);
	memory.printStats();

}


void linkedListMicoBenchmarkRun_pp(vector<pair<uint64_t, uint64_t>> &accessPattern, int prefetchDegree){
	LRUCache ramCache(RAM_CACHE_SIZE, RAM_CACHE_BLOCK_SIZE, RAM_CACHE_ASSOCIATIVITY);
    Memory ram(&ramCache, nullptr);

	LRUCache cache(L2_CACHE_SIZE, L2_CACHE_BLOCK_SIZE, L2_CACHE_ASSOCIATIVITY);
	LinkedListPrefetcher llPrefetcher(prefetchDegree, PREFETCHER_GHB_SIZE);
    Memory memory(&cache, &llPrefetcher, &ram, true);

	for(int i=0; i<accessPattern.size(); i++){
		memory.access(accessPattern[i].first, accessPattern[i].second);
	}

	printf("Linked List MicroBenchmark with Page PD: %d\n", prefetchDegree);
	memory.printStats();

}

void linkedListMicroBenchmark(){
	const int LINKED_LIST_SIZE = 100000;
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
//		for(uint64_t i=(long)RAND_MAX+5,j=0; i<(UINT64_MAX-L2_CACHE_BLOCK_SIZE) && j<L2_CACHE_SIZE/L2_CACHE_BLOCK_SIZE; i+=L2_CACHE_BLOCK_SIZE,j++){
//			accessPattern.push_back( make_pair(0, i) );
//		}

		for(auto it=addresses.begin(); it!=addresses.end(); ++it){
			accessPattern.push_back( make_pair(0, *it) );
		}

//		linkedListMicoBenchmarkRun_np(accessPattern);
		for(int pd = 5; pd<=50; pd+=5){
//			linkedListMicoBenchmarkRun_bp(accessPattern, pd);
			linkedListMicoBenchmarkRun_pp(accessPattern, pd);
			printf("\n");
		}
}
