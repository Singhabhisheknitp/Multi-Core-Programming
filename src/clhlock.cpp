#include<iostream>
#include<thread>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
#include <functional>
#include<mutex>
mutex z;
using namespace std;

class Tnode{
    public:
    atomic<bool> locked;

    Tnode(){
        locked = false;
        
    }
};

 
class CLHLock {
    public:
    atomic<Tnode*> tail;
    thread_local atomic<Tnode*> myNode;
    thread_local atomic<Tnode*> myPred;
    

    int size; // dummy variable just to make all class template standard for initialising the constructor

    CLHLock(int* numthread) {
        tail = new Tnode();  
        size =  *numthread;
        }
    
    void init(){
        myNode.store(new Tnode());
        myPred.store(nullptr);
    }

    void lock() {
        init();
        Tnode* curr = myNode.load();    
        curr->locked = true;
        Tnode* pred = tail.exchange(curr);
        myPred.store(pred);
        while(pred->locked){};  
        
    }

    void unlock() {
        Tnode* curr = myNode.load();
        curr->locked = false;
        myNode.store(myPred.load());    
        }
};

thread_local atomic<Tnode*> CLHLock::myNode;
thread_local atomic<Tnode*> CLHLock::myPred;



