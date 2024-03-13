#include<iostream>
#include<thread>
#include<mutex>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
using namespace std;
class Tnode{
public:
    atomic<bool> wait;
    Tnode(){
        wait = false;
    }
};

 
class CLHLock {
public:
atomic<Tnode*> tail;
static thread_local atomic<Tnode*> myNode;// These containers we are using only to recycle the nodes
static thread_local atomic<Tnode*> myPred;
CLHLock() {
    tail = new Tnode();
    myNode.store(new Tnode());
    myPred.store(nullptr);
    }

void lock() {
   Tnode* cnode = myNode.load();
   cnode->wait = true;
   Tnode* pnode = tail.exchange(cnode);
   myPred.store(pnode);
   while(pnode->wait){};   
}

void unlock() {
    Tnode* cnode = myNode.load();
    cnode->wait = false;
    myNode.store(myPred.load());    
    }
};
thread_local atomic<Tnode*> CLHLock::myNode;
thread_local atomic<Tnode*> CLHLock::myPred;





void cs_clhlock(){
    CLHLock* p = new CLHLock();
      
        p->lock();
        //CS
        int count = 0; 
        for (int i = 0; i < 1000000; i++){ count++;}    
        p->unlock();
}

int main(){
    ofstream locklatency("locklatency.csv");
    // locklatency<<"T,Time_clhlock\n";
    locklatency<<"T, Time_clhlock\n";
    int threadnum = 1;
    while (  threadnum <= 40){
    vector<thread> threads;
    auto start_clhlock = chrono::high_resolution_clock::now(); 
    for(int i = 1; i <= threadnum; i++) { threads.push_back(thread(cs_clhlock));}
    for(auto& thread : threads) {thread.join();}
    auto end_clhlock = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_clhlock - start_clhlock;
    double time_clhlock = elapsed.count();

    locklatency<<threadnum<<","<<time_clhlock<<"\n";
    threadnum = threadnum + 4;
}
locklatency.close();
return 0;
}
