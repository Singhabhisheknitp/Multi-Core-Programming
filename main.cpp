#include <thread>
#include <vector>
#include <chrono>
#include <iostream>
#include <atomic>
#include "utill.cpp"



int main() {
    int thread_count = 16;
    int step = 2;

    // //queue performance logger
    // logger<MSqueue<int>>("msqueue.csv", "MSQueue",benchmark1, step, thread_count);
    // logger<Twolockqueue<int>>("twolockqueue.csv", "Twolockqueue",benchmark1, step, thread_count);
    // logger<OptimisticQueue<int>>("optimistic.csv", "Optimisticqueue", benchmark1, step, thread_count);


    // //fixlist calls for msqueue
    // fixlistcaller<OptimisticQueue<int>>("50%enqueue.csv",benchmark2,thread_count, step);
    // fixlistcaller<OptimisticQueue<int>>("enqdeqpair.csv",benchmark1,thread_count, step);
    

    // //casfailcounter calls
    // casfailcounter<MSqueue<int>>("msqueue.csv", benchmark2, thread_count, step);
    // casfailcounter<OptimisticQueue<int>>("optimistic.csv", benchmark2, thread_count, step);

    //ABA failsafe MS queue & optimistic Queue
    // logger<OptimisticQueueABA<int>>("optimisticABA.csv", "OptimisticQueueABA", benchmark1, step, thread_count);
    // logger<MSQueueABA<int>>("msqueueABA.csv", "MSQueueABA", benchmark1, step, thread_count);


    return 0;
}