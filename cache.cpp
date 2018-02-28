#include "cache.h"

LRUCache::LRUCache(int n){
    csize = n;
}
 
/* Refers key x with in the LRU cache */
void LRUCache::refer(int x){
    // not present in cache
    if (ma.find(x) == ma.end()){
        // cache is full
        if (dq.size() == csize){
            //delete least recently used element
            int last = dq.back();
            dq.pop_back();
            ma.erase(last);
        }
    // present in cache
    }else{
        dq.erase(ma[x]);
    }
 
    // update reference
    dq.push_front(x);
    ma[x] = dq.begin();
}
 
// display contents of cache
void LRUCache::display(){
    for (auto it = dq.begin(); it != dq.end(); it++){
        cout << (*it) << " ";
    }
 
    cout << endl;
}