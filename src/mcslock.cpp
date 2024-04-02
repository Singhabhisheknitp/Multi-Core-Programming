#include "../include/mcslock.h"

thread_local std::atomic<Tnode1*> MCSLock::myNode;
// Tnode1* MCSLock::sentinal;
Tnode1::Tnode1() {
    wait.store(false);
    next = nullptr;
}

MCSLock::MCSLock(int* numthread) {
    // sentinal= new Tnode1();
    tail.store(nullptr);
    size =  *numthread;
}

void MCSLock::init(){
    myNode.store(new Tnode1());
}

void MCSLock::lock() {
    init();
    Tnode1* cnode = myNode.load();  
    Tnode1* pnode = tail.exchange(cnode);
    if (pnode !=  nullptr) {
        cnode->wait = true;
        pnode->next = cnode;
        while(cnode->wait){}
    } 
}

void MCSLock::unlock() {
    Tnode1* cnode = myNode.load();
    if (cnode->next == nullptr) {
        if (tail.compare_exchange_strong(cnode, nullptr)) 
        {return;}
        while(cnode->next == nullptr){};
        (*cnode->next).wait = false;
        cnode->next = nullptr;
    } 
}