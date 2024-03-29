#include "../include/clhlock.h"

thread_local std::atomic<Tnode*> CLHLock::myNode;
thread_local std::atomic<Tnode*> CLHLock::myPred;

Tnode::Tnode() {
    locked = false;
}

CLHLock::CLHLock(int* numthread) {
    tail = new Tnode();  
    size =  *numthread;
}

void CLHLock::init(){
    myNode.store(new Tnode());
    myPred.store(nullptr);
}

void CLHLock::lock() {
    init();
    Tnode* curr = myNode.load();    
    curr->locked = true;
    Tnode* pred = tail.exchange(curr);
    myPred.store(pred);
    while(pred->locked){};  
}

void CLHLock::unlock() {
    Tnode* curr = myNode.load();
    curr->locked = false;
    myNode.store(myPred.load());    
}

