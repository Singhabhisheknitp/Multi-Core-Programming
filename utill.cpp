#include<iostream>
#include<thread>
#include<mutex>
#include <vector>
#include <chrono>
#include <fstream>
#include <atomic>
#include <unistd.h>
#include <functional>
#include "src/msqueue.cpp"
#include "src/twolockqueue.cpp"
#include "src/optimistic.cpp"
using namespace std;
constexpr int OPERATIONS_PER_THREAD = 1000000;  


template <class Queue>
void threadWork(Queue& queue, int threadnum) {
    int ceiling = (OPERATIONS_PER_THREAD + threadnum - 1) / threadnum;
    for (int i = 1; i < ceiling; i++) {
        queue.enqueue(i);
        // queue.dequeue();
    }
}


template<class Queue>
double queuetest(int threadnum, Queue& queue){ 
    vector<thread> threads;
    auto start = chrono::high_resolution_clock::now(); // START 

    for(int i = 1; i <= threadnum; i++) {threads.push_back(thread(threadWork<Queue>, ref(queue), threadnum ));}
    for(auto& thread : threads) {thread.join();}

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;// END
    return elapsed.count()*1e3;
}

template <class Queue>
void logger(const string& file_name, const string& queue_name, int step = 1, int thread_count = 4) {
    ofstream lock_latency_file("benchmarking/" + file_name);   
    lock_latency_file << "No of threads, latency_" << queue_name << "\n";
    Queue queue;
    int threadnum = 1;
    while (threadnum <= thread_count) { 
        double time_queue = queuetest(threadnum,  queue); 
        lock_latency_file << threadnum << "," << time_queue<< "\n";
        threadnum = threadnum + step;   
    }
    lock_latency_file.close();
}