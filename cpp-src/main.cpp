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

#define VERSION "0.1.0"

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


void runBenchmark(string fileName, int prefetchDegree, int prefetchingAlgo, int isPagePrediction);
void runMicroBenchmark(string fileName, int prefetchDegree, int prefetchingAlgo, int isPagePrediction);

int main(int argc, char *argv[]){
	time_t t;
	srand((unsigned) time(&t));

	if(argc == 2 && strcmp(argv[1],"version")==0){
		cout<<"version = "<<VERSION<<endl;
		exit(0);
	}

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

	if(fileName.compare(0,5,"MICRO") == 0){
		runMicroBenchmark(fileName, prefetchDegree, prefetchingAlgo, isPagePrediction);
	}else{
		runBenchmark(fileName, prefetchDegree, prefetchingAlgo, isPagePrediction);
	}

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

void addRandomAccesses(vector<pair<uint64_t, uint64_t>> &accessPattern){
	uint64_t NUM_BLOCKS = L2_CACHE_SIZE/L2_CACHE_BLOCK_SIZE;
	uint64_t START = 0xFFFFFFFFFFFFFFFF;
	for(int i=0; i<NUM_BLOCKS; i++){
		accessPattern.push_back( make_pair(rand(), START-(64LLU*i)) );
	}
}

vector<pair<uint64_t, uint64_t>> getArrayMicroBenchmark(){
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

	return accessPattern;
}

vector<pair<uint64_t, uint64_t>> getBadArrayMicroBenchmark(){
	vector<pair<uint64_t, uint64_t>> accessPattern;

	const uint64_t STRIDE = L2_CACHE_BLOCK_SIZE;

	for(int pc=1; pc<10000; pc++){
		uint64_t address = pc*100000;
		accessPattern.push_back( make_pair(pc, address) );
		accessPattern.push_back( make_pair(pc, address+STRIDE) );
		accessPattern.push_back( make_pair(pc, address+STRIDE+STRIDE) );
		// accessPattern.push_back( make_pair(pc, address+STRIDE+STRIDE+STRIDE) );
	}

	// addRandomAccesses(accessPattern);

	return accessPattern;
}

vector<pair<uint64_t, uint64_t>> getLinkedListMicroBenchmark(){
	const int LINKED_LIST_SIZE = 100000;
	const int NODE_SIZE = 100;
	void* addressess[LINKED_LIST_SIZE];
	set<uint64_t> addresses;

	vector<pair<uint64_t, uint64_t>> accessPattern;

	/* Generate unique random addresses */
	for(int i=0; i<LINKED_LIST_SIZE; i++){
		addresses.insert( rand()*L2_CACHE_BLOCK_SIZE );
	}

	/* populate accessPattern array with pc and addr */
	/* Iterate over the linked list */
	for(auto it=addresses.begin(); it!=addresses.end(); ++it){
		accessPattern.push_back( make_pair(0, *it) );
	}

	/* Iterate over the same list one more time */
	for(auto it=addresses.begin(); it!=addresses.end(); ++it){
		accessPattern.push_back( make_pair(0, *it) );
	}

	return accessPattern;
}

vector<pair<uint64_t, uint64_t>> getBadLinkedListMicroBenchmark(){
	const int LINKED_LIST_SIZE = 100000;
	const int NODE_SIZE = 100;
	void* addressess[LINKED_LIST_SIZE];
	set<uint64_t> addresses;

	vector<pair<uint64_t, uint64_t>> accessPattern;

	/* Generate unique random addresses */
	for(int i=0; i<LINKED_LIST_SIZE; i++){
		addresses.insert( rand()*L2_CACHE_BLOCK_SIZE );
	}

	/* populate accessPattern array with pc and addr */
	/* Iterate over the linked list */
	for(auto it=addresses.begin(); it!=addresses.end(); ++it){
		accessPattern.push_back( make_pair(0, *it) );
	}

	/* Time to mess up the linkedlist prefetcher */
	/* Access linked list in such a way that none of the prefetched blocks are used */
	uint64_t size = accessPattern.size();

	for(int i=0; i<size; i+=200){
		accessPattern.push_back( accessPattern[i] );
	}

	return accessPattern;
}


void runMicroBenchmark(string fileName, int prefetchDegree, int prefetchingAlgo, int isPagePrediction){
	vector<pair<uint64_t, uint64_t>> accessPattern;
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

	if(fileName.find("linkedlist+") != string::npos){
		accessPattern = getLinkedListMicroBenchmark();
	}else if(fileName.find("linkedlist-") != string::npos){
		accessPattern = getBadLinkedListMicroBenchmark();
	}else if(fileName.find("array+") != string::npos){
		accessPattern = getArrayMicroBenchmark();
	}else if(fileName.find("array-") != string::npos){
		accessPattern = getBadArrayMicroBenchmark();
	}else{
		printf("ERROR: Invalid MicroBenchmark specified\n");
		exit(-1);
	}


	for(int i=0; i<accessPattern.size(); i++){
		memory->access(accessPattern[i].first, accessPattern[i].second);
	}

	memory->minPrintStats();

	/* Free Memory */
	delete prefetcher;
	delete memory;
	delete cache;
	delete ram;
	delete ramCache;
}
