#pragma once
#include <atomic>
#include <thread>
#include <iostream>

class Tnode {
public:
    std::atomic<bool> locked;

    Tnode();
};

class CLHLock {
public:
    std::atomic<Tnode*> tail;
    static thread_local std::atomic<Tnode*> myNode;
    static thread_local std::atomic<Tnode*> myPred;
    int size; // dummy variable just to make all class template standard for initialising the constructor

    CLHLock(int* numthread);

    void init();

    void lock();

    void unlock();
};