#pragma once
#include <atomic>
#include <iostream>
class TASLock {
public:
    std::atomic<bool> state;
    int size; // dummy variable just to make all class template standard for initialising the constructor

    TASLock(int* numthread);

    void lock();

    void unlock();
};


