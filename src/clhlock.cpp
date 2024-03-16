#include<iostream>
#include<thread>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
#include <functional>
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
    static thread_local atomic<Tnode*> myNode;
    static thread_local atomic<Tnode*> myPred;
    static void initialize() {
        myNode.store(new Tnode());
        myPred.store(nullptr);
    }

    CLHLock() {
        tail = new Tnode(); 
        }

    void lock() {
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



