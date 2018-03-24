#include "varStrideArrayPrefetcher.h"

VarStrideArrayPrefetcher::VarStrideArrayPrefetcher(int prefetchDegree, int num_pc, int num_strides) : Prefetcher(prefetchDegree){
    this-> num_pc = num_pc;
    this->num_strides = num_strides;
    this->name = "Variable Stride Array Prefetcher";
    
    /* Initialize the data structure with full capacity,
       this is to make other functions simpler */
    for(int i=0; i<num_pc; i++){
        struct VsEntry vsEntry = {0,0,0,0,0, new uint64_t[num_strides]};
        q.push_front(vsEntry);
    }
}


VarStrideArrayPrefetcher::~VarStrideArrayPrefetcher(){
    for(int i=0; i<num_pc; i++){
        struct VsEntry vsEntry = q.front();
        delete[] vsEntry.strideArray;
    }
}

void VarStrideArrayPrefetcher::seedMiss(uint64_t pc, uint64_t missAddr){
    struct VsEntry vsEntry;

    /* This iterator will refer to the required struct that contains our pc */
    list<VsEntry>::iterator itcs = q.end(); /* Initilze to LRU element, which will be updated by default if pc not found */

    /* Find the iterator to the struct in the q that matches our pc  */
    for (auto it = q.begin(); it != q.end(); it++){
        if( (*it).pc == pc){
            itcs = it;
            break;
        }
    }
    
    /* PC not found, need to evict LRU struct */
    if(itcs == q.end()){ /* if pc not found */
        itcs = q.begin();
        vsEntry = (*itcs);
        /* reusing old struct */
        vsEntry.index = 0;
        vsEntry.pc = pc;
    }else{
        vsEntry = (*itcs);
    }
    
    q.erase(itcs);
    
    /* keep track of last two strides */
    vsEntry.lastStride = vsEntry.currentStride;
    vsEntry.currentStride = missAddr - vsEntry.missAddr;

    /* struct with pc found */
    vsEntry.strideArray[(vsEntry.index++)%num_strides] = missAddr;
    
    /* try to match */
    uint64_t start_index = vsEntry.index<num_strides?0:(vsEntry.index%num_strides + 1); // needs to be rechecked
    uint64_t end_index = vsEntry.index - 1;
    
    for(uint64_t i = start_index; i<end_index; i++){
        uint64_t addr1 = vsEntry.strideArray[i];
        uint64_t addr2 = vsEntry.strideArray[i+1];
        
        if(addr1 == vsEntry.lastStride && addr2 == vsEntry.currentStride){
            for(int j = 0; j<prefetchDegree; j++){
            	addToQueue( vsEntry.strideArray[i+1+1+j] );
            }
            break;
        }
    }

    q.push_back(vsEntry);
}

void VarStrideArrayPrefetcher::printInfo(){
	printf("Prefetcher = %s\n", name.c_str());
	printf("Prefetch Degree = %d\n", prefetchDegree);
	printf("Num PC = %d\n", num_pc);
	printf("Num Strides = %d", num_strides);
}
