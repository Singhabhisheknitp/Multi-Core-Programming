#include <thread>
#include <vector>
#include <chrono>
#include <iostream>
#include <atomic>
#include "utill.cpp"


int main() {
    
    int thread_count = 50;
    int step = 2;

    logger<MSqueue<int>>("msqueue.csv", "MSQueue", step, thread_count);
    


    return 0;
}