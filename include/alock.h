#pragma once
#include <atomic>
#include <thread>
#include <iostream>

class ALock {
public:
    static thread_local std::atomic<int> id;
    std::atomic<int>* tail;
    std::atomic<bool>* flag;
    int size;

    ALock(int* numthread);

    void init();

    void lock();

    void unlock();
};