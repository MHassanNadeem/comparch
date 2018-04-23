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
void runBenchmark(string fileName, int prefetchDegree, int prefetchingAlgo, int isPagePrediction);

int main(int argc, char *argv[]){
	time_t t;
	srand((unsigned) time(&t));

	/*
	1 - Name of benchmark file
	2 - Prefetching degree
	3 - 0 for const stride array prefetching, 1 for linkedlist prefetching
	4 - 0 for block prediction, 1 for page prediction
	*/

	if(argc < 5){
		printf("Invalid Input Arguments\n");
		exit(-1);
	}
	
	string fileName(argv[1]);
	int prefetchDegree   = strtol( argv[2], NULL, 10 );
	int prefetchingAlgo  = strtol( argv[3], NULL, 10 );
	int isPagePrediction = strtol( argv[4], NULL, 10 );

	runBenchmark(fileName, prefetchDegree, prefetchingAlgo, isPagePrediction);

    return 0;
}


void runBenchmark(string fileName, int prefetchDegree, int prefetchingAlgo, int isPagePrediction){
	Memory *memory, *ram;
	Prefetcher *prefetcher;
	LRUCache *cache, *ramCache;

	ram = NULL;
	ramCache = NULL;

	cache = new LRUCache(L2_CACHE_SIZE, L2_CACHE_BLOCK_SIZE, L2_CACHE_ASSOCIATIVITY);

	switch(prefetchingAlgo){
		case 0:
			prefetcher = new ConstStrideArrayPrefetcher(prefetchDegree, PREFETCHER_NUM_PC);
			break;
		case 1:
			prefetcher = new LinkedListPrefetcher(prefetchDegree, PREFETCHER_GHB_SIZE);
			break;
	}

	if(!isPagePrediction){ /* Block Prediction */
		memory = new Memory(cache, prefetcher);
	}else{ /* Page Prediction */
		ramCache = new LRUCache(RAM_CACHE_SIZE, RAM_CACHE_BLOCK_SIZE, RAM_CACHE_ASSOCIATIVITY);
		ram = new Memory(ramCache, nullptr);

		memory = new Memory(cache, prefetcher, ram, true);
	}

	uint64_t pc, addr;

	FileParser file(fileName);

    while( file.getNext(pc, addr) ){
    	memory->access(pc, addr);
    }

	memory->minPrintStats();

	/* Free Memory */
	delete prefetcher;
	delete memory;
	delete cache;
	delete ram;
	delete ramCache;
}


/**************************************************************************************/
/**************************************************************************************/
// Micro Benchmarks Below
/**************************************************************************************/
/**************************************************************************************/


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
	for(int pd = 0; pd<=100; pd+=10){
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
		for(int pd = 0; pd<=50; pd+=5){
			linkedListMicoBenchmarkRun_bp(accessPattern, pd);
//			linkedListMicoBenchmarkRun_pp(accessPattern, pd);
			printf("\n");
		}
}
