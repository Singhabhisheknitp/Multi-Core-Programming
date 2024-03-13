#include<iostream>
#include<thread>
#include<mutex>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
#include <mutex>

using namespace std;
class Tnode{
public:
    atomic<bool> wait;
    Tnode* next;
    Tnode(){
        wait = false;
        next = nullptr;
    }
};

 
class MCSLock {
    public:
    atomic<Tnode*> tail;
    static thread_local atomic<Tnode*> myNode;// These containers we are using only to recycle the nodes
    MCSLock() {
        atomic<Tnode*> tail(nullptr);
        myNode.store(new Tnode());
        }

    void lock() {
    Tnode* cnode = myNode.load();
    Tnode* pnode = tail.exchange(cnode);
    if (pnode != nullptr) {
    cnode->wait = true;
    pnode->next = cnode;
    while(cnode->wait){}
    cout<<"lock5\n";  } 
    }

    void unlock() {
        Tnode* cnode = myNode.load();
        if (cnode->next == nullptr) {
        if (tail.compare_exchange_strong(cnode, nullptr)) 
        {return;}
        while(cnode->next == nullptr){};
        cnode->next->wait = false;
        cnode->next = nullptr;
        }

    }
};
thread_local atomic<Tnode*> MCSLock::myNode;
int* count = new int(0);

void cs_mcslock(){
    MCSLock* p = new MCSLock();
    while(*count < 100000000){
        p->lock(); 
        (*count)++;   
        p->unlock();}   
}

int main(){
    ofstream locklatency("locklatency.csv");
    // locklatency<<"T,Time_mcslock\n";
    locklatency<<"T, Time_mcslock\n";
    int threadnum = 1;
    while (  threadnum <= 20){
    vector<thread> threads;
    
    auto start_mcslock = chrono::high_resolution_clock::now();
    for(int i = 1; i <= threadnum; i++) { threads.push_back(thread(cs_mcslock));}
    for(auto& thread : threads) {thread.join();}
    auto end_mcslock = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed6 = end_mcslock - start_mcslock;
    double Time_mcslock = elapsed6.count();

    locklatency<<threadnum<<","<<Time_mcslock<<"\n";
    threadnum = threadnum + 1;
}
locklatency.close();
return 0;
}
