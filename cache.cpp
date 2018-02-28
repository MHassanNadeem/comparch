#include "cache.h"

LRUCache::LRUCache(int size, int associativity){
    this->size = size;
    this->associativity = associativity;
    this->numSets = size/associativity;
    
    DBG(size, d);
    DBG(associativity, d);
    DBG(numSets, d);
    
    q = new list<int>[numSets];
}

LRUCache::~LRUCache(){
    delete [] q;
}

int LRUCache::getSetNumber(int x){
    return x%numSets;
}

bool LRUCache::isPresent(int x){
    return !(map.find(x) == map.end());
}

/* Refers key x with in the LRU cache */
void LRUCache::add(int x){
    int setNumber = getSetNumber(x);

    if(!isPresent(x)){
        // set is full
        if (q[setNumber].size() == associativity){
            //delete LRU element
            int last = q[setNumber].back();
            q[setNumber].pop_back();
            map.erase(last);
        }
    }else{
        q[setNumber].erase(map[x]);
    }
    
    // update reference
    q[setNumber].push_front(x);
    map[x] = q[setNumber].begin();
}
 
// display contents of cache
void LRUCache::display(){
    for(int iSet = 0; iSet<numSets; iSet++){
        printf("Set %d: ", iSet);
        for (auto it = q[iSet].begin(); it != q[iSet].end(); it++){
            cout << (*it) << " ";
        }
        printf("\n");
    }
 
    cout << endl;
}