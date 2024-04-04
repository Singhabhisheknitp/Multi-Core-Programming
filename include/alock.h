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
    static thread_local int id;
    atomic<int> tail;
    bool* flag;
    int size;
};