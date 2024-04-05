#include "../include/mcslock.h"

thread_local Tnode1* MCSLock::myNode = new Tnode1();
Tnode1::Tnode1() {
    locked.store(false);
    next = nullptr;
}

MCSLock::MCSLock(int* numthread) {
    tail.store(nullptr);
    size =  *numthread;// dummy varibale just to keep constructor call of all lock onject similar
}

void MCSLock::lock() {
    Tnode1* curr = myNode;  
    Tnode1* pred = tail.exchange(curr);
    if (pred !=  nullptr) {
        curr->locked.store(true);
        pred->next.store(curr);
        while(curr->locked.load()){}
    } 

}


void MCSLock::unlock() {
    Tnode1* curr = myNode;
    Tnode1* succ = curr->next.load();
    if (succ == nullptr) {
        if (tail.compare_exchange_strong(curr, nullptr)) {
        return;
        }
        
        while(succ == nullptr){};

    } 
    succ->locked.store(false);
    curr->next = nullptr;
}