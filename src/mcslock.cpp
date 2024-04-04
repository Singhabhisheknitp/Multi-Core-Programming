#include "../include/mcslock.h"

thread_local Tnode1* MCSLock::myNode;
Tnode1::Tnode1() {
    locked = false;
    next = nullptr;
}

MCSLock::MCSLock(int* numthread) {
    tail.store(nullptr);
    size =  *numthread;
}

void MCSLock::init(){
    myNode = new Tnode1();
    // cout<<myNode<<endl;
}

void MCSLock::lock() {
    init();
    Tnode1* curr = myNode;  
    Tnode1* pred = tail.exchange(curr);

    // cout<<tail<<endl;
    // cout<<pred<<endl;
   
    if (pred !=  nullptr) {
        curr->locked = true;
        pred->next = curr;
        while(curr->locked){}
    } 

}


void MCSLock::unlock() {
    Tnode1* curr = myNode;
    if (curr->next == nullptr) {
        if (tail.compare_exchange_strong(curr, nullptr)) {
        return;
        }
        while(curr->next == nullptr){};
       
        
    
    } 
    curr->next->locked = false;
    curr->next = nullptr;
}