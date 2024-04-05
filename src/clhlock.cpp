#include "../include/clhlock.h"
#include <iostream>

thread_local Tnode* CLHLock::myNode = new Tnode();
thread_local Tnode*CLHLock::myPred = nullptr;

Tnode::Tnode() {
    locked = false;
}

CLHLock::CLHLock(int* numthread) {
    tail = new Tnode();  
    size =  *numthread;
}


void CLHLock::lock() {
    Tnode* curr = myNode;  
    curr->locked.store(true);
    Tnode* pred = tail.exchange(curr);
    myPred= pred;
    while(pred->locked.load()){};  
}

void CLHLock::unlock() {
    Tnode* curr = myNode;
    curr->locked.store(false);
    myNode = myPred;    
}

