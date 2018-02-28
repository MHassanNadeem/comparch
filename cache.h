#include <unordered_map>
#include <bits/stdc++.h>
using namespace std;
 
class LRUCache{
    // store keys of cache
    list<int> dq;
 
    // store references of key in cache
    unordered_map<int, list<int>::iterator> ma;
    int csize; //maximum capacity of cache
 
public:
    LRUCache(int);
    void refer(int);
    void display();
};