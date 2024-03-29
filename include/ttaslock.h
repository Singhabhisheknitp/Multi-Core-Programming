#pragma once
#include <atomic>
#include <iostream>

class TTASLock {
public:
    std::atomic<bool> state;
    int size; // dummy variable just to make all class template standard for initialising the constructor

    TTASLock(int* numthread);

    void lock();

    void unlock();
};