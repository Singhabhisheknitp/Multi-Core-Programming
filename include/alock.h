#pragma once
#include <atomic>
#include <iostream>
#include <vector>
using namespace std;

class ALock {
public:
    ALock(int* numthread);
    void lock();
    void unlock();

private:
    static thread_local int id;
    atomic<int> tail;
    atomic<bool*> flag;
    int size;
};