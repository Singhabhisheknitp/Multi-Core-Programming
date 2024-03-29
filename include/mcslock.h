#pragma once
#include <atomic>
#include <thread>
#include <iostream>

class Tnode1 {
public:
    std::atomic<bool> wait;
    Tnode1* next;

    Tnode1();
};

class MCSLock {
public:
    std::atomic<Tnode1*> tail;
    static Tnode1* sentinal;
    static thread_local std::atomic<Tnode1*> myNode;
    int size; // dummy variable just to make all class template standard for initialising the constructor

    MCSLock(int* numthread);

    void init();

    void lock();

    void unlock();
};