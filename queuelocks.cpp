#include<iostream>
#include<thread>
#include<mutex>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
using namespace std;
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
        int count = 0; 
        for (int i = 0; i < 1000000; i++){ count++;}    
        p->unlock();
 }


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
        //CS 
        int count = 0; 
        for (int i = 0; i < 1000000; i++){ count++;} 
      p->unlock();
       
    }


int main(){
    ofstream locklatency("locklatency.csv");
    locklatency<<"T, Time_alocks, Time_clhlock\n";
    int threadnum = 1;
    while (  threadnum <= 40){
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

    locklatency<<threadnum<<","<<time_alock<<","<<time_clhlock<<"\n";
    threadnum = threadnum + 4;
}
locklatency.close();
return 0;
}
