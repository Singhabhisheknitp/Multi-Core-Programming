#include <thread>
#include <vector>
#include <chrono>
#include <iostream>
#include <atomic>
#include "utill.cpp"



int main() {
    int thread_count = 16;
    int step = 2;

    //queue performance logger
    // logger<MSqueue<int>>("msqueue.csv", "MSQueue", step, thread_count);
    // logger<Twolockqueue<int>>("twolockqueue.csv", "Twolockqueue", step, thread_count);
    // logger<OptimisticQueue<int>>("optimistic.csv", "Optimisticqueue", step, thread_count);


    //fixlist calls for msqueue
    // fixlistcaller<OptimisticQueue<int>>("50%enqueue.csv",benchmark2,thread_count, step);
    // fixlistcaller<OptimisticQueue<int>>("enqdeqpair.csv",benchmark1,thread_count, step);
    


    casfailcounter<MSqueue<int>>("msqueue.csv", benchmark2, thread_count, step);
    casfailcounter<OptimisticQueue<int>>("optimistic.csv", benchmark2, thread_count, step);

    
    // for error plot

    // for (int i = 1; i < 10; i++) {    
    // errorplot<MSqueue<int>>("msqueue.csv", "MSQueue", step, thread_count, i);
    // // errorplot<Twolockqueue<int>>("twolockqueue.csv", "Twolockqueue", step, thread_count, i);
    // // errorplot<OptimisticQueue<int>>("optimistic.csv", "Optimisticqueue", step, thread_count, i);
    // }
    return 0;
}