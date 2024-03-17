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


template <class LockType> 
void critical_section(int* size){
   static LockType* m = new LockType(size);
   cout<<m<<"\n";
    
    for (int j = 0; j < 100000; j++){
        m->lock();
        //critical_section - start
        int c = 0; 
        for (int i = 0; i < 100; i++){ 
        c = c+1; 
        }  
        //critical_section - end
        m->unlock();
    }
}



double runtime_crticalsection(int threadnum, function<void()> func = nullptr ){ 
    vector<thread> threads;
    auto start = chrono::high_resolution_clock::now(); // START 
    for(int i = 1; i <= threadnum; i++) {threads.push_back(thread(func));}
    for(auto& thread : threads) {thread.join();}
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;// END
    return elapsed.count()/threadnum;
}

template <class LockType>
void use_lock_and_write_to_csv(const string& file_name, const string& lock_name, int step = 4, int thread_count = 40) {
    string path = string("/mnt/c/COL818/Multi-Core-Programming/benchmarking") + "/" + file_name; 
    ofstream lock_latency_file(path);   
    lock_latency_file << "T, Time_" << lock_name << "\n";

    int threadnum = 1;
    while (threadnum <= thread_count) {
        double time_lock = runtime_crticalsection(threadnum,  bind(critical_section<LockType>, &threadnum)); 
        lock_latency_file << threadnum << "," << time_lock << "\n";
        threadnum = threadnum + step;   
    }

    lock_latency_file.close();
}