#include <thread>
#include <vector>
#include <chrono>
#include <iostream>
#include <atomic>
#include "utill.cpp"



int main() {
    int thread_count = 32;
    int step = 2;

    logger<MSqueue<int>>("msqueue.csv", "MSQueue", step, thread_count);
    logger<Twolockqueue<int>>("twolockqueue.csv", "Twolockqueue", step, thread_count);
    logger<OptimisticQueue<int>>("optimistic.csv", "Optimisticqueue", step, thread_count);

    // fixlistcaller<OptimisticQueue<int>>(thread_count, step);
    // casfailcounter<OptimisticQueue<int>>(thread_count, step);
    
    return 0;
}