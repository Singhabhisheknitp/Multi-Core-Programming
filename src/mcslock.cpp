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
        // cout<<"please check"<<"\n";
        // cout<<curr->locked<<"\n";
        pred->next = curr;
        while(curr->locked){}
    } 
    // cout<<"first thread gone without setting its lock field true as its first"<<"\n";
}


void MCSLock::unlock() {
    Tnode1* curr = myNode;
    // cout<<curr->locked<<"\n";
    // cout<<curr->next<<"\n";
    
    if (curr->next == nullptr) {
        if (tail.compare_exchange_strong(curr, nullptr)) {
        // cout<<"lock released 1"<<endl;
        return;
        }
        while(curr->next == nullptr){};
       
        
    
    } 
    curr->next->locked = false;
    curr->next = nullptr;
    // cout<<"check lock released"<<endl;
}