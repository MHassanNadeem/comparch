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

#define NUM_BYTES_IN_MB 1048576LU

void arrayMicroBenchmark();
void linkedListMicroBenchmark();

int main(){
	time_t t;
	srand((unsigned) time(&t));
    printf("===============================\n");
    printf("|          Project 5          |\n");
    printf("===============================\n");
    
//    LRUCache cache(4, 1, 4);
//    ConstStrideArrayPrefetcher csPrefetcher(&cache, 5, 10);
//    Memory memory(&cache, &csPrefetcher);


//    uint64_t pc, addr;
//    FileParser file1("pinatrace13.out");
//
//    while( file1.getNext(pc, addr) ){
//        printf("0x%lx - 0x%lx\n", pc, addr);
//    }

    arrayMicroBenchmark();
//    linkedListMicroBenchmark();

    return 0;
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
	LRUCache cache(2*NUM_BYTES_IN_MB, 64, 8);
	ConstStrideArrayPrefetcher csPrefetcher(&cache, 10, 256);
	Memory memory(&cache, &csPrefetcher);

	LRUCache cache2(2*NUM_BYTES_IN_MB, 64, 8);
	Memory memory2(&cache2, nullptr);

	for(int i=0; i<accessPattern.size(); i++){
		memory.access(accessPattern[i].first, accessPattern[i].second);
		memory2.access(accessPattern[i].first, accessPattern[i].second);
	}

	printf("With Const Stride Array Prefetcher Disabled\n");
	memory2.printStats();

	printf("With Const Stride Array Prefetcher Enabled\n");
	memory.printStats();
}

void arrayMicroBenchmark(){
	vector<pair<uint64_t, uint64_t>> accessPattern;

	struct TestData{
		uint64_t pc;
		uint64_t startAddress;
		uint64_t arraySize;
		uint64_t stride;
	};

	struct TestData testData[4] = {
		{1,          1, 10000, 10},
		{2, 1000000000, 10000, 20},
		{3, 2000000000, 10000, 30},
		{4, 3000000000, 10000, 40}
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
	LinkedListPrefetcher llPrefetcher(&cache1, 10, 256);
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
	const int LINKED_LIST_SIZE = 100;
	set<uint64_t> addresses;

	vector<pair<uint64_t, uint64_t>> accessPattern;

	/* Generate unique random addresses */
	for(int i=0; i<LINKED_LIST_SIZE; i++){
		addresses.insert( rand() );
	}

	/* populate accessPattern array with pc and addr */
//	for(int i=0; i<2; i++){
		for(auto it=addresses.begin(); it!=addresses.end(); ++it){
			accessPattern.push_back( make_pair(0, *it) );
		}

		/* Add noise so that some of the cached linked list is evicted */
//		for(int j=0; j<100000; j++){
//			uint64_t uniqueRandom;
//			do{
//				uniqueRandom = rand();
//			}while(addresses.count(uniqueRandom) == 1);
//			accessPattern.push_back( make_pair(0, uniqueRandom) );
//		}

//		for(int j=0; j<10000; j++){
//			uint64_t uniqueRandom;
//			do{
//				uniqueRandom = rand();
//			}while(addresses.count(uniqueRandom) == 1);
//			accessPattern.push_back( make_pair(0, uniqueRandom) );
//		}

		for(auto it=addresses.begin(); it!=addresses.end(); ++it){
			accessPattern.push_back( make_pair(0, *it) );
		}
//	}

	linkedListMicoBenchmarkRun(accessPattern);
}
