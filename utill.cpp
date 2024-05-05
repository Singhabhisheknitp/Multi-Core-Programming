#include<iostream>
#include<thread>
#include<mutex>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
#include <functional>
#include <random>
#include "src/msqueue.cpp"
#include "src/twolockqueue.cpp"
#include "src/optimistic.cpp"
using namespace std;
constexpr int OPERATIONS_PER_THREAD = 1000000;  
constexpr int   OTHER_WORK = 10;




template <class Queue>
void benchmark1(Queue& queue, int threadnum) {
    int ceiling = (OPERATIONS_PER_THREAD ) / threadnum;
    int x = 0;
    for (int i = 1; i < ceiling; i++) {
        if (i % 2 == 0){
        queue.enqueue(i);}
       
        else{
        queue.dequeue();}
        for (int i =0; i< OTHER_WORK; i++) {int x = x+1;} //just for some time to interleave
        
        
    }
}


template <class Queue>
void benchmark2(Queue& queue, int threadnum) {
    int ceiling = (OPERATIONS_PER_THREAD ) / threadnum;
    int x = 0;
    std::random_device rd;  
    std::mt19937 gen(rd());  
    std::uniform_int_distribution<> dis(0, 1);  

    for (int i = 1; i < ceiling; i++) {
        int random_number = dis(gen);  // Generate a random number

        if (random_number == 0) {
            queue.enqueue(i);
        } else {
            queue.dequeue();
        }

        for (int i =0; i< OTHER_WORK; i++) {int x = x+1;}  // Just for some time to interleave
    }
}


template<class Queue>
double queuetest(int threadnum, Queue& queue){ 
    vector<thread> threads;
    auto start = chrono::high_resolution_clock::now(); // START 

    for(int i = 1; i <= threadnum; i++) {threads.push_back(thread(benchmark1<Queue>, ref(queue), threadnum ));}
    for(auto& thread : threads) {thread.join();}

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = (end - start);// END
    return elapsed.count()*1e3;
}

template <class Queue>
void logger(const string& file_name, const string& queue_name, int step = 1, int thread_count = 4) {
    ofstream lock_latency_file("benchmark1ing/" + file_name);   
    lock_latency_file << "No of threads," << queue_name << "\n";
    Queue queue;
    int threadnum = 1;
    while (threadnum <= thread_count) { 
        double time_queue = queuetest(threadnum,  queue); 
        lock_latency_file << threadnum << "," << time_queue<< "\n";
        threadnum = threadnum*step;  
        cout<<"work done: "<<(threadnum/thread_count)*100<<endl; 
       
    }
    lock_latency_file.close();
    
}

template <class Queue>
void fixlistcaller(int thread_count = 4, int step = 1){
    ofstream fixlistcaller("benchmarking/fixlistcaller.csv");
    fixlistcaller << "No of threads, number of calls \n";
    Queue queue;
    int threadnum = 1;
    while (threadnum <= thread_count) { 
        int count = 0;
        double time_queue = queuetest(threadnum,  queue); 
        count = queue.fixlistcount.load();
        fixlistcaller << threadnum << "," << count << "\n";
        cout<<"work done: "<<threadnum<<endl;
        queue.resetFixlistCount();
        threadnum = threadnum*step;
    }
    fixlistcaller.close();
    
}

template <class Queue>
void casfailcounter(int thread_count = 4, int step = 1){
    ofstream casfailcounter("benchmarking/failedcascounter.csv");
    casfailcounter << "No of threads, number of failed CAS in 1000s\n";
    Queue queue;
    int threadnum = 1;
    while (threadnum <= thread_count) { 
        int count = 0;
        double time_queue = queuetest(threadnum,  queue); 
        count = queue.failedcas_count.load();
        casfailcounter << threadnum << "," << count*1e-3<< "\n";
        cout<<"work done: "<<threadnum<<endl;
        queue.resetFailedCasCount();
        threadnum = threadnum*step;
    }
    casfailcounter.close();
    
}