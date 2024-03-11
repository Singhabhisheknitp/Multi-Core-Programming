#include<iostream>
#include<thread>
#include<mutex>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
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
TASLock* m = new TASLock() ;
void cs_TAS(){
    for (int j = 0; j < 100000; j++){
        m->lock();
        //CS 
    int c = 0; int d = 0; int e = 0; int f = 0;
    for (int i = 0; i < 100; i++){ 
        c = c+1; 
        d = d+1;
        e = e+1;
        f = f+1;
    }  
        m->unlock();
       
    }}


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
TTASLock* n = new TTASLock() ;
void cs_TTAS(){
    for (int j = 0; j < 100000; j++){
        n->lock();
        //CS 
    int c = 0; int d = 0; int e = 0; int f = 0;
    for (int i = 0; i < 100; i++){
        c = c+1; 
        d = d+1;
        e = e+1;
        f = f+1;
    }  
        n->unlock();
       
    }
}


int main(){
    
    
    ofstream locklatency("locklatency.csv");
    // locklatency<<"T,Time_TAS, Time_TTAS\n";
    locklatency<<"T, Time_TAS, Time_TTAS\n";


    int threadnum =1;
    while (  threadnum <= 40){
    vector<thread> threads;
    auto start_TAS = chrono::high_resolution_clock::now(); 
    for(int i = 1; i <= threadnum; i++) { threads.push_back(thread(cs_TAS));}
    for(auto& thread : threads) {thread.join();}
    auto end_TAS = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_TAS - start_TAS;
    double time_TAS = elapsed.count()/threadnum;

    threads.clear();
    auto start_TTAS = chrono::high_resolution_clock::now();
    for(int i = 1; i <= threadnum; i++) { threads.push_back(thread(cs_TTAS));}
    for(auto& thread : threads) {thread.join();}
    auto end_TTAS = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed1 = end_TTAS - start_TTAS;
    double time_TTAS = elapsed1.count()/threadnum;

    locklatency<<threadnum<<","<<time_TAS<<","<<time_TTAS<<"\n";
    threadnum = threadnum + 4;
   
}
    locklatency.close();
    return 0;
}

