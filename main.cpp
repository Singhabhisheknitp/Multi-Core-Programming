#include <thread>
#include <vector>
#include <chrono>
#include <iostream>
#include <atomic>
#include "utill.cpp"



int main() {
    int thread_count = 10;
    int step = 1;

    logger<MSQueue<int>>("msqueue.csv", "MSQueue", step, thread_count);
    // logger<Twolockqueue<int>>("twolockqueue.csv", "Twolockqueue", step, thread_count);
    // logger<OptimisticQueue<int>>("optimistic.csv", "Optimisticqueue", step, thread_count);
    
    return 0;
}