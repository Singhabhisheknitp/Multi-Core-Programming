#include "../include/clhlock.h"
#include <iostream>

thread_local Tnode* CLHLock::myNode;
thread_local Tnode*CLHLock::myPred;

Tnode::Tnode() {
    locked = false;
}

CLHLock::CLHLock(int* numthread) {
    tail = new Tnode();  
    size =  *numthread;
}

void CLHLock::init(){
    myNode = new Tnode();
    myPred = (nullptr);
}

void CLHLock::lock() {
    init();
    Tnode* curr = myNode;  
    curr->locked = true;
    Tnode* pred = tail.exchange(curr);
    myPred= pred;
    while(pred->locked){};  
}

void CLHLock::unlock() {
    Tnode* curr = myNode;
    curr->locked = false;
    myNode = (myPred);    
}

