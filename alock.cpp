#include<iostream>
#include<thread>
#include<mutex>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
using namespace std;

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
    // locklatency<<"T,Time_alock\n";
    locklatency<<"T, Time_alock\n";
    int threadnum = 1;
    while (  threadnum <= 40){
    vector<thread> threads;
    auto start_alock = chrono::high_resolution_clock::now(); 
    for(int i = 1; i <= threadnum; i++) { threads.push_back(thread(cs_alock, threadnum));}
    for(auto& thread : threads) {thread.join();}
    auto end_alock = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_alock - start_alock;
    double time_alock = elapsed.count()/threadnum;

    locklatency<<threadnum<<","<<time_alock<<"\n";
    threadnum = threadnum + 4;
}
locklatency.close();
return 0;
}
