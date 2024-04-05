#pragma once
#include <atomic>
#include <thread>
#include <iostream>
using namespace  std;

class Tnode {
public:
    atomic<bool> locked;

    Tnode();
};

class CLHLock {
public:
    std::atomic<Tnode*> tail;
    static thread_local  Tnode* myNode;
    static thread_local  Tnode* myPred;
    int size; // dummy variable just to make all class template standard for initialising the constructor

    CLHLock(int* numthread);

    void lock();

    void unlock();
};