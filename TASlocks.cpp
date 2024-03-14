#include<iostream>
#include<thread>
#include<mutex>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
#include <functional>
using namespace std;


class TASLock {
public:
    atomic<bool> state;
    TASLock() : state(false) {}

    void lock() {
        while (this->state.exchange(true)) {}
    }
    void unlock() {
        (this->state).store(false);
    }


};

class TTASLock {
public:
    atomic<bool> state;
    TTASLock() : state(false) {}

    void lock() {
        while (true) {
            while (this->state.load()) {}
            if (!this->state.exchange(true)) {
                return;
            }
        }
    }
    void unlock() {
        (this->state).store(false);
    }

};

template <class LockType>

void cs(LockType* lock){
    for (int j = 0; j < 100000; j++){
        lock->lock();
        //CS - start
    int c = 0; 
    for (int i = 0; i < 100; i++){ 
        c = c+1; 
    }  
         //CS - end
        lock->unlock();
    }

}

double run_crticalsection(int threadnum = 0, function<void()> func = nullptr ){
    vector<thread> threads;
    auto start = chrono::high_resolution_clock::now(); 
    for(int i = 1; i <= threadnum; i++) { threads.push_back(thread(func));}
    for(auto& thread : threads) {thread.join();}
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    return elapsed.count()/threadnum;
}

int main(){
    TTASLock* n = new TTASLock();
    TASLock* m = new TASLock();
    
    ofstream locklatency1("locklatency1.csv");
    int threadnum = 1;
    int thread_count = 40;
    int step = 4;
    
    locklatency1<<"T, Time_TAS, Time_TTAS\n";

    while (  threadnum <= thread_count){
    double time_TAS = run_crticalsection(threadnum, [m](){ cs(m); }); 
    double time_TTAS = run_crticalsection(threadnum, [n](){ cs(n); });

    locklatency1<<threadnum<<","<<time_TAS<<","<<time_TTAS<<"\n";
    threadnum = threadnum + step;
   
}
    locklatency1.close();
    return 0;
}

