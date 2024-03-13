#include<iostream>
#include<thread>
#include<mutex>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
using namespace std;
//used for cs section of the loop
int* count1 = new int(0);
int* count2 = new int(0);
int* count3 = new int(0);

class Tnode1{
public:
    atomic<bool> wait;
    Tnode1* next;
    Tnode1(){
        wait = false;
        next = nullptr;
    }
};

 
class MCSLock {
    public:
    atomic<Tnode1*> tail;
    static thread_local atomic<Tnode1*> myNode;// These containers we are using only to recycle the nodes
    MCSLock() {
        atomic<Tnode1*> tail(nullptr);
        myNode.store(new Tnode1());
        }

    void lock() {
    Tnode1* cnode = myNode.load();
    Tnode1* pnode = tail.exchange(cnode);
    if (pnode != nullptr) {
    cnode->wait = true;
    pnode->next = cnode;
    while(cnode->wait){}
      } 
    }

    void unlock() {
        Tnode1* cnode = myNode.load();
        if (cnode->next == nullptr) {
        if (tail.compare_exchange_strong(cnode, nullptr)) 
        {return;}
        while(cnode->next == nullptr){};
        cnode->next->wait = false;
        cnode->next = nullptr;
        }

    }
};
thread_local atomic<Tnode1*> MCSLock::myNode;


void cs_mcslock(){
    MCSLock* r = new MCSLock();
    while(*count3 < 100000000){
        r->lock(); 
        (*count3)++;   
        r->unlock();}   
}


class Tnode {
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
        while(*count2 < 100000000){
        (*count2)++;     
        p->unlock();
 }}


class ALock {
public:
 // id variable local to each thread but global across the object as needed
   static thread_local atomic<int> id;
   atomic<int>* tail;
    bool* flag;
int size;
ALock(int numthread) {
    size = numthread;
    id.store(0);
    tail = new atomic<int>(0);
    flag = new bool[size]();
    flag[0] = true;
}

void lock() {
    int slot = tail->fetch_add(1) % size;
    id.store(slot);
    while (!flag[slot]) {}
}

    void unlock() {
        int slot = id.load();
        flag[slot] = false;
        flag[(slot + 1) % size] = true;}

       
    };
thread_local atomic<int> ALock::id(0);

void cs_alock(int numthread){
    ALock* p = new ALock(numthread);
        p->lock();
        while(*count1 < 100000000){
        (*count1)++;
        p->unlock();
    }}


int main(){
    ofstream locklatency("locklatency.csv");
    locklatency<<"T, Time_alocks, Time_clhlock, Time_mcslock\n";
    int threadnum = 1;
    while (threadnum <= 40){
    vector<thread> threads;

    auto start_alock = chrono::high_resolution_clock::now(); 
    for(int i = 1; i <= threadnum; i++) { threads.push_back(thread(cs_alock, threadnum));}
    for(auto& thread : threads) {thread.join();}
    auto end_alock = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed3 = end_alock - start_alock;
    double time_alock = elapsed3.count();
    threads.clear();


    auto start_clhlock = chrono::high_resolution_clock::now(); 
    for(int i = 1; i <= threadnum; i++) { threads.push_back(thread(cs_clhlock));}
    for(auto& thread : threads) {thread.join();}
    auto end_clhlock = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed4 = end_clhlock - start_clhlock;
    double time_clhlock = elapsed4.count();
    threads.clear();

    auto start_mcslock = chrono::high_resolution_clock::now();
    for(int i = 1; i <= threadnum; i++) { threads.push_back(thread(cs_mcslock));}
    for(auto& thread : threads) {thread.join();}
    auto end_mcslock = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed6 = end_mcslock - start_mcslock;
    double time_mcslock = elapsed6.count();

    locklatency<<threadnum<<","<<time_alock<<","<<time_clhlock<<","<<time_mcslock<<"\n";
    threadnum = threadnum + 4;
}
locklatency.close();
return 0;
}
