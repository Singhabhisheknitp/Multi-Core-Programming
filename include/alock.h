#pragma once
#include <atomic>
#include <iostream>
using namespace std;

class ALock {
public:
    ALock(int* numthread);
    void init();
    void lock();
    void unlock();

private:
    static thread_local std::atomic<int> id;
    atomic<int> tail;
    atomic<bool>* flag;
    int size;
};